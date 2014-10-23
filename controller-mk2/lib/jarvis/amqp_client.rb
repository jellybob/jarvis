require 'jarvis/version'
require 'logger'
require 'bunny'

module Jarvis
  class AmqpClient
    class Logger
      def initialize(target = $stdout)
        @logger = ::Logger.new(target)
        @logger.formatter = ->(severity, time, progname, msg) {
          "#{msg} version=#{Jarvis::VERSION} time=#{time.strftime("%FT%T%z")} severity=#{severity}\n"
        }
      end

      def log(level, message, attributes = {})
        @logger.send(level, "#{message} #{attributes.collect { |k,v| "#{k}=#{v}" }.join(" ")}")
      end
    end
    
    def self.run
      logger = Logger.new
      logger.log(:info, "Starting Jarvis Controller")
      logger.log(:debug, "Establishing RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION)
      
      Bunny.run do |client|
        logger.log(:debug, "Established RabbitMQ connection", uri: ENV["RABBITMQ_URL"], bunny_version: Bunny::VERSION)
        yield client, logger
      end
    end
  end
end
