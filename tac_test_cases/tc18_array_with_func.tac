0 BeginFunc double_val 1
1 PopParam v
2 t0 = v * 2
3 Return t0
4 EndFunc double_val
5 BeginFunc main 0
6 arr[0] = 3
7 arr[4] = 7
8 arr[8] = 2
9 arr[12] = 9
10 arr[16] = 5
11 i = 0
12 if i < 5 goto 14
13 goto 24
14 t1 = i * 4
15 t2 = arr[t1]
16 PushParam t2
17 t3 = Call double_val
18 arr[t1] = t3
19 t4 = i + 1
20 i = t4
21 if i < 5 goto 14
22 goto 24
24 j = 0
25 if j < 5 goto 27
26 goto 34
27 t5 = j * 4
28 t6 = arr[t5]
29 printint t6
30 t7 = j + 1
31 j = t7
32 if j < 5 goto 27
33 goto 34
34 Return 0
35 EndFunc main
