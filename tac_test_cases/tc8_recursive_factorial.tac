0 BeginFunc fact 1
1 PopParam n
2 if n <= 1 goto 5
3 goto 6
5 Return 1
6 t0 = n - 1
7 sub = t0
8 PushParam sub
9 t1 = Call fact
10 r = t1
11 t2 = n * r
12 Return t2
13 EndFunc fact
14 BeginFunc main 0
15 PushParam 5
16 t3 = Call fact
17 f5 = t3
18 printint f5
19 PushParam 0
20 t4 = Call fact
21 f0 = t4
22 printint f0
23 PushParam 1
24 t5 = Call fact
25 f1 = t5
26 printint f1
27 Return 0
28 EndFunc main
