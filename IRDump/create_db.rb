require 'serialport'

DUMP_REGEXP = /^Decoded (?<TYPE>\w+): (?<ADDR>[A-F0-9]+) \((?<SIZE>\d{1,2}) bits\)$/

PORT = ENV.fetch("PORT", "/dev/ttyACM0")

codes = {}
trap("SIGINT") do
  puts "---"
  puts "Name,Type,Addr,Size"
  codes.each do |name, value|
    puts [ name, value[:type], value[:addr], value[:size] ].join(",")
  end

  exit 0
end

SerialPort.open(PORT, baud: 9600) do |tty|
  buffer = ""
  
  while true do
    while chunk = tty.getc
      buffer << chunk
    end
    
    if buffer =~ /\r\n$/
      line = buffer.gsub(/\r\n$/, "")
      buffer = ""

      matches = line.match(DUMP_REGEXP)
    
      # Skip over NEC repeat codes.
      unless matches.nil? || matches[:TYPE] == "NEC" && matches[:ADDR] == "FFFFFFFF"
        print "Name: "
        name = gets.strip

        codes[name] = { type: matches[:TYPE], addr: matches[:ADDR], size: matches[:SIZE] }
      end
    end
  end
end
