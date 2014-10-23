require 'jarvis/version'
require 'syslog/logger'
require 'bunny'

module Jarvis
  class AmqpClient
    class Logger
      def initialize(name)
        @logger = Syslog::Logger.new(name)
      end

      def log(level, message, attributes = {})
        @logger.send(level, "#{message} #{attributes.collect { |k,v| "#{k}=#{v}" }.join(" ")}")
      end
    end
    
    def self.run(name = nil)
      if name.nil?
        name = File.basename($0)
      end

      logger = Logger.new(name)
      logger.log(:info, "Starting #{name}")

      retry_count = 0

      begin
        logger.log(:debug, "Establishing RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION)
        Bunny.run do |client|
          logger.log(:debug, "Established RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION)
          yield client, logger
        end
      rescue Bunny::TCPConnectionFailed => e
        if retry_count > 4
          logger.log(:fatal, "Could not connect to RabbitMQ after 5 attempts, giving up", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION, attempt: retry_count)
          exit 1
        else
          retry_count += 1
          logger.log(:warn, "RabbitMQ connection failed, retrying", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION, attempt: retry_count)
          sleep 5
          retry
        end
      end
    end
  end
end
