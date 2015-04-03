#!/usr/bin/perl

use POSIX;
use Socket;
use Getopt::Std;

&client_maple("150.26.237.19",10000,"s:=0");
&client_maple("150.26.237.19",10000,"for i from 120 do s:=s+i end do");

sub client_maple{
        local($socket,$in,$n,$host,$port,$state,$addr,$proto,$ent,$silent,$ret,$c,$v);
        $argc = $#ARGV;
        $socket = 'SOCKE';
        $in = $socket;
        $n = 0;
        $silent = 0;
        $ret = 0;
        $host = $_[0];
        $port = $_[1];
        $state = $_[2];
        $silent = $_[3];        #1:silent mode
        $ret = $_[4];           #0:connection status, 1:result
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
        if($silent != 1){
                while($line = <$in>){
                        print "$line";
                }
        }
        close($socket);
        if($ret == 0){
                return($c);
        }elsif($ret == 1){
                return($v);
        }
}
