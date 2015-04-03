./client.maple.pl 150.26.237.19 10001 'for i from 1 to 8000000 do s:=s+i end do'>s1&
./client.maple.pl 150.26.237.19 10002 'for i from 8000001 to 16000000 do s:=s+i end do'>s2&
wait
echo $((`cat s1` + `cat s2`))
