REQ[0]='for i from 1 to 7000000 do s:=s+i end do'
REQ[1]='for i from 7000001 to 14000000 do s:=s+i end do'
REQ[2]='for i from 14000001 to 21000000 do s:=s+i end do'
REQ[3]='for i from 21000001 to 28000000 do s:=s+i end do'
PORT=10000
echo ${REQ[0]} | ./mapleclient 150.26.237.19 $((PORT+1)) >s1 &
echo ${REQ[1]} | ./mapleclient 150.26.237.19 $((PORT+2)) >s2 &
echo ${REQ[2]} | ./mapleclient 150.26.237.19 $((PORT+3)) >s3 &
echo ${REQ[3]} | ./mapleclient 150.26.237.19 $((PORT+4)) >s4 &
wait
echo $((`cat s1` + `cat s2` + `cat s3` + `cat s4`))
