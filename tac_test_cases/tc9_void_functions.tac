0 BeginFunc print_double 1
1 PopParam n
2 t0 = n * 2
3 d = t0
4 printint d
5 Return
6 EndFunc print_double
7 BeginFunc print_square 1
8 PopParam m
9 t1 = m * m
10 sq = t1
11 printint sq
12 Return
13 EndFunc print_square
14 BeginFunc main 0
15 PushParam 5
16 Call print_double
17 PushParam 3
18 Call print_double
19 PushParam 4
20 Call print_square
21 PushParam 7
22 Call print_square
23 Return 0
24 EndFunc main
