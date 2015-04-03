#!/usr/local/bin/ruby

$SERVER = "150.26.237.19"
$STARTPORT = 10000

def startmaple (server,port)
	server = server.to_s
	port = port.to_i
	state1 = 
"server := proc( sid )
        local s, res;
        use Sockets in
        interface(prettyprint=0);
        s := Read(sid,60);
        res := parse(s,statement);
        Write( sid, sprintf( \"%q\", res) );
        Write( sid, sprintf( \"\\n\" ) );
        end use
end proc:\n"
	state2 = "Sockets:-Serve( "
	state4 = ", server );"
	state = state1+state2+port.to_s+state4
	`echo '#{state}' | maple -t 2>/dev/null &`
end

def mapleclient (server,port,state)
	require 'socket'
	sock = TCPSocket.open(server,port)
	sock.print state
	while line = sock.gets
		str = str.to_s + line.to_s
	end
	sock.close
	return str
end

#server = ARGV[0]
#port = ARGV[1]
#state = ARGV[2]
#ret = mapleclient($SERVER,port,state)
#print ret
#startmaple("150.26.237.19",10005)
print "maple << "
while line = $stdin.gets
	input = input.to_s
	if line =~ /\\$/
		line = line.gsub("\\\n","")
		input = input+line
	else
		input = input+line
		input = input.gsub("\n","")
		if input == '\e'
			exit
		elsif input =~ /\\startmaple/
			startmaple($SERVER,$STARTPORT)
		else
			print input
			#print "\n"
		end
		input = ''
		print "maple << "
	end
end
