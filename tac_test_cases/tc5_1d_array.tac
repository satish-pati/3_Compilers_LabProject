0 BeginFunc main 0
1 arr[0] = 10
2 arr[4] = 20
3 arr[8] = 30
4 arr[12] = 40
5 arr[16] = 50
6 t0 = arr[0]
7 printint t0
8 t1 = arr[8]
9 printint t1
10 t2 = arr[16]
11 printint t2
12 i = 0
13 total = 0
14 if i < 5 goto 16
15 goto 24
16 t3 = i * 4
17 t4 = arr[t3]
18 t5 = total + t4
19 total = t5
20 t6 = i + 1
21 i = t6
22 if i < 5 goto 16
23 goto 24
24 printint total
25 Return 0
26 EndFunc main
