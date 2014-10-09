require 'sinatra'
require 'sequel'
require 'logger'
require 'multi_json'
require 'redis'
require 'guid'

configure do
  DB = Sequel.connect(ENV["DATABASE_URL"])
  REDIS = Redis.new(url: ENV["REDIS_URL"])
end

helpers do
  def body_params
    @_body_params ||= MultiJson.load(request.body)
  end
end

get '/devices' do
  devices = DB[:devices].all

  content_type "application/json+jarvis"
  return MultiJson.dump(devices)
end

put '/devices' do
  details = {
    :adapter => body_params["adapter"],
    :address => body_params["address"],
    :device_type => body_params["device_type"],
    :device => body_params["device"],
    :manufacturer => body_params["manufacturer"],
    :last_seen => DateTime.now
  }

  device = DB[:devices].where(adapter: details[:adapter], address: details[:address]).first
  
  if device.nil?
    device = DB[:devices].insert(details)
  else
    DB[:devices].where(adapter: details[:adapter], address: details[:address]).update(details)
  end

  status 200
  content_type "application/json+jarvis"
  return MultiJson.dump({ device: details })
end

def return_command(command)
  content_type "application/json+jarvis"
  return MultiJson.dump({ command: command })
end

post '/commands' do
  command = body_params
  
  device_id = command["device_id"]

  device = DB[:devices].where(id: device_id).first
  command.merge!({
    id: Guid.new.hexdigest,
    state: :queued,
    queued_at: DateTime.now.iso8601,
    device: device
  })

  REDIS.multi do
    REDIS.set("command:#{command[:id]}", MultiJson.dump(command))
    REDIS.rpush("queue:#{device[:adapter]}", command[:id])
  end

  return_command(command)
end

get '/commands/:id' do
  if REDIS.exists("command:#{params[:id]}")
    command = MultiJson.load(REDIS.get("command:#{params[:id]}"))
    return_command(command)
  else
    content_type "application/json+jarvis"
    status 404

    return MultiJson.dump({ error: "Command not found" })
  end
end

get '/queue/:adapter/flush' do
  commands = []
  while id = REDIS.lpop("queue:#{params[:adapter]}")
    commands << MultiJson.load(REDIS.get("command:#{id}"))
  end

  content_type "application/json+jarvis"
  status 200
  return MultiJson.dump({ queue: commands })
end
