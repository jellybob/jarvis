# A simple example adapter that reads from a serial port.
require 'logger'
require 'serialport'
require 'typhoeus'
require 'multi_json'

PORT = ENV.fetch("PORT", "/dev/ttyACM1")
NAME = ENV.fetch("NAME", "Example")

UID = 0
DEVICE = 1
MANUFACTURER = 2
DEVICE_TYPE = 3

LOGGER = Logger.new($stdout)

LOGGER.info("Starting")
SerialPort.open(PORT, baud: 9600) do |tty|
  while true do
    heartbeats = {}

    # Pick up any new data being sent to us.
    tty.readlines.each do |line|
      fields = line.split("\t").collect(&:strip)
      if fields.shift == "HEARTBEAT"
        device = {
          adapter: "serial:#{NAME}",
          address: fields[UID],
          device: fields[DEVICE],
          manufacturer: fields[MANUFACTURER],
          device_type: fields[DEVICE_TYPE]
        }

        if device.values.any? { |f| f.nil? || f == "" }
          LOGGER.debug "Mangled heartbeat, dropping"
        else
          LOGGER.info "Heartbeat: #{device.inspect}"
          heartbeats[device["address"]] = device
        end
      else
        LOGGER.debug("Unknown message: #{line}")
      end
    end

    # Report heartbeats
    heartbeats.values.each do |heartbeat|
      response = Typhoeus.put(
        "http://localhost:4567/devices", 
        body: MultiJson.dump(heartbeat), 
        headers: { "Content-Type" => "application/json+jarvis" }
      )
      LOGGER.info "Reported heartbeat. Response: #{response.code}"
    end

    # Send out commands to devices.
    raw_queue = Typhoeus.get("http://localhost:4567/queue/serial%3A#{NAME}/flush")
    if raw_queue.code == 200
      command_queue = MultiJson.load(raw_queue.body)
      command_queue["queue"].each do |cmd|
        LOGGER.debug cmd.inspect
        tty.puts([
          "COMMAND",
          cmd["device"]["address"],
          cmd["command"],
          cmd["id"]
        ].join("\t"))
      end
    else
      LOGGER.warn "Failed pulling command queue. Response: #{raw_queue.code}"
    end

    sleep 1
  end
end
