0 BeginFunc main 0
1 mat[0] = 1
2 mat[4] = 2
3 mat[8] = 3
4 mat[12] = 4
5 mat[16] = 5
6 mat[20] = 6
7 t0 = mat[0]
8 printint t0
9 t1 = mat[8]
10 printint t1
11 t2 = mat[20]
12 printint t2
13 r = 0
14 s = 0
15 if r < 2 goto 17
16 goto 38
17 c = 0
18 if c < 3 goto 20
19 goto 34
20 t3 = r * 3
21 t4 = t3 + c
22 t5 = t4 * 4
23 t6 = mat[t5]
24 printint t6
25 t7 = c + 1
26 c = t7
27 if c < 3 goto 20
28 goto 34
34 t8 = r + 1
35 r = t8
36 if r < 2 goto 17
37 goto 38
38 Return 0
39 EndFunc main
