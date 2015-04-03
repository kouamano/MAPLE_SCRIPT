./client.maple.pl 150.26.237.19 10001 'for i from 1 to 16000000 do s:=s+i end do'>s1&
wait
echo $((`cat s1`))
