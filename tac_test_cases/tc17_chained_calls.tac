0 BeginFunc square 1
1 PopParam n
2 t0 = n * n
3 Return t0
4 EndFunc square
5 BeginFunc cube 1
6 PopParam n
7 PushParam n
8 t1 = Call square
9 sq = t1
10 t2 = sq * n
11 Return t2
12 EndFunc cube
13 BeginFunc sum_squares 2
14 PopParam a
15 PopParam b
16 PushParam a
17 t3 = Call square
18 sa = t3
19 PushParam b
20 t4 = Call square
21 sb = t4
22 t5 = sa + sb
23 Return t5
24 EndFunc sum_squares
25 BeginFunc main 0
26 PushParam 4
27 t6 = Call square
28 r1 = t6
29 printint r1
30 PushParam 3
31 t7 = Call cube
32 r2 = t7
33 printint r2
34 PushParam 3
35 PushParam 4
36 t8 = Call sum_squares
37 r3 = t8
38 printint r3
39 PushParam r1
40 t9 = Call cube
41 r4 = t9
42 printint r4
43 Return 0
44 EndFunc main
