require 'jarvis/version'
require 'logger'
require 'bunny'

module Jarvis
  class AmqpClient
    def self.run(name = nil)
      if name.nil?
        name = File.basename($0)
      end

      logger = ::Logger.new($stdout)
      logger.formatter = ->(severity, datetime, progname, msg) {
        "#{datetime.strftime("%Y-%m-%dT%H:%M:%S%z")} #{msg.delete(:message)} #{msg.collect { |k,v| "#{k}=#{v}" }.join(" ")}\n"
      }
      logger.info({ message: "Starting #{name}" })

      retry_count = 0

      begin
        logger.debug({ message: "Establishing RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION })
        Bunny.run do |client|
          logger.debug({ message: "Established RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION })
          yield client, logger
        end
      rescue Bunny::TCPConnectionFailed => e
        if retry_count > 4
          logger.fatal({ message: "Could not connect to RabbitMQ after 5 attempts, giving up", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION, attempt: retry_count })
          exit 1
        else
          retry_count += 1
          logger.warn({ message: "RabbitMQ connection failed, retrying", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION, attempt: retry_count })
          sleep 5
          retry
        end
      end
    end
  end
end
