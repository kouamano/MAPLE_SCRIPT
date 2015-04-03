#!/usr/bin/perl

use POSIX;
use Socket;
use Getopt::Std;

my ($socket, $in, $n, $host, $port, $state, $addr, $proto, $ent);
$argc = $#ARGV;
$socket = 'SOCKE';
$in = $socket;
$n = 0;

$host = $ARGV[0];
$port = $ARGV[1];
$state = $ARGV[2];

@n = gethostbyname($host);
$addr = $n[4];
$proto = getprotobyname('tcp');

socket($socket, PF_INET, SOCK_STREAM, $proto);
$ent = pack('S n a4 x8', AF_INET, $port, $addr);
connect($socket, $ent);
select($socket);
print $state;
$| = 1;
select(STDOUT);

while($line = <$in>){
	print "$line";
}

close($socket);
exit;
