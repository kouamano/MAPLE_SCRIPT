./client.maple.pl 150.26.237.19 10001 'for i from 1 to 5000000 do s:=s+i end do'>s1&
./client.maple.pl 150.26.237.19 10002 'for i from 5000001 to 10000000 do s:=s+i end do'>s2&
./client.maple.pl 150.26.237.19 10003 'for i from 10000001 to 15000000 do s:=s+i end do'>s3&
./client.maple.pl 150.26.237.19 10004 'for i from 15000001 to 20000000 do s:=s+i end do'>s4&
wait
echo $((`cat s1` + `cat s2` + `cat s3` + `cat s4`))
