#!/usr/bin/perl

$PORT = $ARGV[0];

$stat = <<"EOF";
server := proc( sid )
	local s, res;
	use Sockets in
	interface(prettyprint=0);
	s := Read(sid,60);
	res := parse(s,statement);
	Write( sid, sprintf( " Client:%s-%d,\\r\\n Server:%s-%d,\\r\\n sid:%d,\\r\\n string:%s,\\r\\n result:%q.\\r\\n", GetPeerHost( sid ), GetPeerPort( sid ), GetHostName(), $PORT, sid, s, res) );
	end use
end proc:

Sockets:-Serve( $PORT, server );
EOF

system("echo '$stat' | maple");
