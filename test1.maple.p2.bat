./client.maple.pl 150.26.237.19 10001 'for i from 1 to 10000000 do s:=s+i end do'>s1&
./client.maple.pl 150.26.237.19 10002 'for i from 10000001 to 20000000 do s:=s+i end do'>s2&
wait
echo $((`cat s1` + `cat s2`))
