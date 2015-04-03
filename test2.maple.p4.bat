./client.maple.pl 150.26.237.19 10001 'for i from 1 to 6000000 do s:=s+i end do'>s1&
./client.maple.pl 150.26.237.19 10002 'for i from 6000001 to 12000000 do s:=s+i end do'>s2&
./client.maple.pl 150.26.237.19 10003 'for i from 12000001 to 18000000 do s:=s+i end do'>s3&
./client.maple.pl 150.26.237.19 10004 'for i from 18000001 to 24000000 do s:=s+i end do'>s4&
wait
echo $((`cat s1` + `cat s2` + `cat s3` + `cat s4`))
