./client.maple.pl 150.26.237.19 10001 's:=0'
./client.maple.pl 150.26.237.19 10002 's:=0'
./client.maple.pl 150.26.237.19 10003 's:=0'
./client.maple.pl 150.26.237.19 10004 's:=0'

./client.maple.pl 150.26.237.19 10001 'for i from 1 to 7000000 do s:=s+i end do'>s1&
./client.maple.pl 150.26.237.19 10002 'for i from 7000001 to 14000000 do s:=s+i end do'>s2&
./client.maple.pl 150.26.237.19 10003 'for i from 14000001 to 21000000 do s:=s+i end do'>s3&
./client.maple.pl 150.26.237.19 10004 'for i from 21000001 to 28000000 do s:=s+i end do'>s4&

wait
echo $((`cat s1` + `cat s2` + `cat s3` + `cat s4`))
