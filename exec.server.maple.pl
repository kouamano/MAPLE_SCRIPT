#!/usr/bin/perl

$PORT = $ARGV[0];

$stat = <<"EOF";
server := proc( sid )
	local s, res;
	use Sockets in
	interface(prettyprint=0);
	s := Read(sid,60);
	res := parse(s,statement);
	Write( sid, sprintf( "%q", res) );
	Write( sid, sprintf( "\n" ) );
	end use
end proc:

Sockets:-Serve( $PORT, server );
EOF

system("echo '$stat' | maple");
