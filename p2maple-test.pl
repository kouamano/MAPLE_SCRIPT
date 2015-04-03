#!/usr/bin/perl
use POSIX;
use Socket;
use Getopt::Std;
use threads;
use threads::shared;
my @R:shared;
$R[0] = 0;
$R[1] = 0;
$R = 0;
#my $thread3 = threads->new(\&client_maple, "150.26.237.19",10000,"2*4",1,1,1,$R[0]);
#&client_maple( "150.26.237.19",10000,"s:=0",1,1,1,$tmp);
#&client_maple( "150.26.237.19",10001,"s:=0",1,1,1,$tmp);
#my $thread3 = threads->new(\&client_maple, "150.26.237.19",10000,"for i from 1 to 10 do i end do",1,1,1,$R[0]);
#my $thread4 = threads->new(\&client_maple, "150.26.237.19",10001,"3*4",1,1,1,$R[1]);
my $thread3 = threads->new(\&client_maple, "150.26.237.19",10000,"for i from 1 to 10000000 do s:=s+i end do",1,1,1,$R[0]);
my $thread4 = threads->new(\&client_maple, "150.26.237.19",10001,"for i from 10000001 to 20000000 do s:=s+i end do",1,1,1,$R[1]);
$thread3->join; 
$thread4->join; 
print("R[0]:$R[0]");
print("R[1]:$R[1]");
$R = $R[0]+$R[1];
print("R:$R\n");

#----- sub routines -----#
sub test {
	my($start,$end,$s,$i);
	$start = $_[0];
	$end = $_[1];
	$s = 0;
	for($i=$start;$i<$end;$i++){
		$s = $s + $i;
	}
        threads->yield();
	#print("s:$s\n");
	#print("thr:$_[2]\n");
	$OUT[$_[2]] = $s;
	#print("OUT:$OUT[$_[2]]\n");
	#return($s);
}

sub client_maple{
        local($i,$socket,$in,$n,$host,$port,$state,$addr,$proto,$ent,$silent,$ret,$c,$v,@v);
        $argc = $#ARGV;
        $socket = 'SOCKE';
        $in = $socket;
        $n = 0;
        $silent = 0;
        $ret = 0;
	$write = 0;
        $host = $_[0];
        $port = $_[1];
        $state = $_[2];
        $silent = $_[3];        #1:silent mode
        $ret = $_[4];           #0:connection status, 1:result
	$write = $_[5];		#1:write to global variables
        @n = gethostbyname($host);
        if( ($addr = $n[4]) eq '' ){
                return(-1)
        }
        $proto = getprotobyname('tcp');
        if( socket($socket, PF_INET, SOCK_STREAM, $proto) ){
        }else{
                return(-1);
        }
        $ent = pack('S n a4 x8', AF_INET, $port, $addr);
        if( ($c = connect($socket, $ent)) eq '' ){
                return(-1);
        }
        select($socket);
        print($state);
        $| = 1;
        select(STDOUT);
	$i = 0;
	while($line = <$in>){
		$v[$i] = $line;
		$i++;
	}
        close($socket);
	$v = join('',@v);
        if($silent != 1){
		print($v);
        }
	if($write == 1){
		$_[6] = $v;
	}
        if($ret == 0){
                return($c);
        }elsif($ret == 1){
                return($v);
        }
}

