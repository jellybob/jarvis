require 'date'

module Jarvis
  class Heartbeat
    attr_accessor :adapter
    attr_accessor :address
    attr_accessor :device_type
    attr_accessor :manufacturer
    attr_accessor :device
    attr_accessor :received_at
    attr_accessor :logged_at
   
    def initialize(attributes = {})
      attributes.keys.each do |key|
        if self.respond_to?("#{key}=")
          self.send("#{key}=", attributes[key])
        end
      end

      self.received_at ||= DateTime.now
    end

    def received_at=(value)
      if value.is_a?(String)
        value = DateTime.parse(value)
      end

      @received_at = value
    end

    def logged_at=(value)
      if value.is_a?(String)
        value = DateTime.parse(value)
      end

      @logged_at = value
    end

    def to_h
      {
        adapter: adapter,
        address: address,
        device_type: device_type,
        manufacturer: manufacturer,
        device: device,
        received_at: received_at.iso8601,
        logged_at: logged_at && logged_at.iso8601
      }
    end
  end
end
