#!/usr/local/bin/ruby

require 'socket'
server = ARGV[0]
port = ARGV[1].to_i
state = ARGV[2]
sock = TCPSocket.open(server,port)
sock.print state
while line = sock.gets
	print line
end
sock.close
