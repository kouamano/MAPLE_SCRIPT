#!/usr/local/bin/ruby

require 'socket'
begin
server_url = ARGV[0]
t = TCPSocket.new(server_url,'www')
rescue
puts "error: #{$!}"
else
t.print "GET / HTTP/1.0\n\n"
ret = t.gets(nil)
t.close
print ret
end
