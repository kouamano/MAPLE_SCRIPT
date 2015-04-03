#!/usr/bin/perl
use POSIX;
use Socket;
use Getopt::Std;
use threads;
use threads::shared;

$PROMPTHEAD = 'maple';
$PROMPTBODY = '';
$PROMPTTAIL = ' << ';
$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
$INDI = '';
$STARTPORT = 10000;
$MAXNPORTS = 0;
$HOST = '150.26.237.19';
$VERBOSE = 0;
$tmp = 1;
@PORTS = ();
my %DPID = ();
my %USERVARS:shared = ();
#%USERVARS = ();
my %THR = ();
my $thr = 0;

print($PROMPT);
while(<STDIN>){
	if($_ =~ s/\\\n$//){
		push(@str,$_);
	}else{
		$_ =~ s/\n//;
		push(@str,$_);
		$str=join('',@str);
		if($str eq '\v'){
			$VERBOSE = 1;
		}elsif($str eq '\-v'){
			$VERBOSE = 0;
		}elsif($str eq '\help'){
			print('\clear',"\n");
			print('\exit',"\n");
			print('\help',"\n");
			print('\respown',"\n");
			print('\startmaple',"\n");
			print('\v',"\n");
		}elsif($str =~ /^\\startmaple/){
			($com,$n) = split(' ',$str);
			if($n eq ''){
				if($MAXNPORTS == 0){
					$NUMPORTS = @PORTS;
					$PORT = $STARTPORT+@PORTS;
					$PORTS[$NUMPORTS] = $PORT;
					&start_maple($PORTS[$NUMPORTS]);
					$INDI = $NUMPORTS + 1;
					$PROMPTBODY="[".$INDI."]";
					$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
					$MAXNPORTS++;
				}else{
					$NUMPORTS = @PORTS;
					$PORT = $STARTPORT+@PORTS+1;
					$PORTS[$NUMPORTS] = $STARTPORT+$MAXNPORTS;
					&start_maple($PORTS[$NUMPORTS]);
					$INDI = $NUMPORTS + 1;
					$PROMPTBODY="[".$INDI."]";
					$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
					$MAXNPORTS++;
				}
			}elsif($n =~ /^[0-9]+/){
				for($i=0;$i<$n;$i++){
					if($MAXNPORTS == 0){
						$NUMPORTS = @PORTS;
						$PORT = $STARTPORT+@PORTS;
						$PORTS[$NUMPORTS] = $PORT;
						&start_maple($PORTS[$NUMPORTS]);
						$INDI = $NUMPORTS + 1;
						$PROMPTBODY="[".$INDI."]";
						$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
						$MAXNPORTS++;
					}else{
						$NUMPORTS = @PORTS;
						$PORT = $STARTPORT+@PORTS+1;
						$PORTS[$NUMPORTS] = $STARTPORT+$MAXNPORTS;
						&start_maple($PORTS[$NUMPORTS]);
						$INDI = $NUMPORTS + 1;
						$PROMPTBODY="[".$INDI."]";
						$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
						$MAXNPORTS++;
					}
				}
			}
		}elsif($str eq '\exit'){
			foreach $s (@PORTS) {
				&client_maple($HOST,$s,'quit');
			}
			exit;
		}elsif($str eq '\ports'){
			print("HOST:$HOST\n");
			print("No.\tPort\tAlive\n");
			$i=0;
			foreach $s (@PORTS) {
				$c = &client_maple($HOST,$s,"1",1);
				print("$i\t$s\t$c\n");
				$i++;
			}
		}elsif($str eq '\respown'){
			$i=0;
			foreach $s (@PORTS) {
				$c = &client_maple($HOST,$s,"1",1);
				if($c le 0){
					&start_maple($s);
				}
				$i++;
			}
		}elsif($str eq '\clear'){
			$i = 0;
			@b = ();
			foreach $s (@PORTS) {
				$c = &client_maple($HOST,$s,"1",1);
				if($c > 0){
					$b[$i] = $s;
					$i++;
				}else{
				}
			}
			@PORTS = ();
			@PORTS = @b;
			$NUMPORTS = @PORTS;
			$NUMPORTS--;
			if(@PORTS == 0){
				$PROMPTBODY='';
			}else{
				$INDI = $NUMPORTS + 1;
				$PROMPTBODY="[".$INDI."]";
			}
			$PROMPT=$PROMPTHEAD.$PROMPTBODY.$PROMPTTAIL;
		}elsif($str eq '\names'){
			foreach (%USERVARS) {
				print(" $_");
			}
		}elsif($str eq '\pid'){
			printf("$$\n");
		}elsif($str =~ /^[0-9]+</){
			($s,@state) = split('<',$str);
			$state = join(' ',@state);
			$state =~ s/#[a-zA-Z]+/$USERVARS{$&}/g;
			$state =~ s/\n//g;
			if($VERBOSE == 1){
				print("kernel:$s:\n");
				print("send:$state:\n");
			}
			if($PORTS[$s] =~ /[0-9]+/){
				&client_maple($HOST,$PORTS[$s],$state);
			}else{
				print(":not served:\n");
			}
		}elsif($str =~ /^[0-9]+&</){
			($s,@state) = split('&<',$str);
			$state = join(' ',@state);
			$state =~ s/#[a-zA-Z]+/$USERVARS{$&}/g;
			$state =~ s/\n//g;
			if($VERBOSE == 1){
				print("kernel:$s:\n");
				print("send:$state:\n");
			}
			if($PORTS[$s] =~ /[0-9]+/){
				$THR{$thr} = threads->new(\&client_maple,$HOST,$PORTS[$s],$state);
				$DPID = fork();
				#$DPID{$thr} = fork;
				#$cpid = $DPID{$thr};
				print("child:$cpid\n");
				$THR{$thr}->join;
				$thr++;
				if($DPID == 0){
					die;
				}
			}else{
				print(":not served:\n");
			}
		}elsif($str =~ /^ALL</){
			($s,@state) = split('<',$str);
			$state = join(' ',@state);
			$state =~ s/#[a-zA-Z]+/$USERVARS{$&}/g;
			$state =~ s/\n//g;
			$i = 0;
			foreach $s (@PORTS) {
				if($VERBOSE == 1){
					print("kernel:$i:\n");
					print("send:$state:\n");
				}
				&client_maple($HOST,$s,$state);
				$i++;
			}
		}elsif($str =~ /^#[a-zA-Z]+:[0-9]+</){
			($var,@tail) = split(':',$str);
			$tail = join('',@tail);
			($s,$state) = split('<',$tail);
			$state =~ s/#[a-zA-Z]+/$USERVARS{$&}/g;
			$state =~ s/\n//g;
			if($VERBOSE == 1){
				print("VAR:$var\n");
				print("ID:$s\n");
				print("STATE:$state\n");
				&client_maple($HOST,$PORTS[$s],$state,0,1,1,$USERVARS{$var});
			}elsif($VERBOSE == 0){
				&client_maple($HOST,$PORTS[$s],$state,0,1,1,$USERVARS{$var});
			}
		}elsif($str =~ /^#[a-zA-Z]+:[0-9]+&</){
			($var,@tail) = split(':',$str);
			$tail = join('',@tail);
			($s,$state) = split('&<',$tail);
			$state =~ s/#[a-zA-Z]+/$USERVARS{$&}/g;
			$state =~ s/\n//g;
			if($VERBOSE == 1){
				print("VAR:$var\n");
				print("ID:$s\n");
				print("STATE:$state\n");
				$THR{$thr} = threads->new(\&client_maple,$HOST,$PORTS[$s],$state,0,1,1,$USERVARS{$var});
				$DPID = fork();
				$THR{$thr}->join;
				$thr++;
				if($DPID == 0){
					die;
				}
			}elsif($VERBOSE == 0){
				$THR{$thr} = threads->new(\&client_maple,$HOST,$PORTS[$s],$state,0,1,1,$USERVARS{$var});
				$DPID = fork;
				$THR{$thr}->join;
				$thr++;
				if($DPID == 0){
					die;
				}
			}
		}elsif($str =~ /^#[a-zA-Z]+;/){
			($var,$tail) = split(';',$str);
			print($USERVARS{$var});
		}
		@str = ();
	}
	print($PROMPT);
}

sub client_maple{
	local($i,$socket,$in,$n,$host,$port,$state,$addr,$proto,$ent,$silent,$ret,$c,$v,@v);
	$argc = $#ARGV;
	$socket = 'SOCKE';
	$in = $socket;
	$n = 0;
	$silent = 0;
	$ret = 0;
	$host = $_[0];
	$port = $_[1];
	$state = $_[2];
	$silent = $_[3];	#1:silent mode
	$ret = $_[4];		#0:connection status, 1:result
	$write = $_[5];
	$recv = $_[6];
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
	        print "$v";
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

sub start_maple{
	local($port,$stat);
	$port = $_[0];
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

Sockets:-Serve( $port, server );
EOF
	system("echo '$stat' | maple -t 2>/dev/null &");
}
