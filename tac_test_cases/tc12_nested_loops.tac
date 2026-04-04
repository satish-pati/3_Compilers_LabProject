0 BeginFunc main 0
1 count = 0
2 ni = 0
3 if ni < 3 goto 5
4 goto 20
5 nj = 0
6 if nj < 3 goto 8
7 goto 16
8 t0 = count + 1
9 count = t0
10 t1 = nj + 1
11 nj = t1
12 if nj < 3 goto 8
13 goto 16
16 t2 = ni + 1
17 ni = t2
18 if ni < 3 goto 5
19 goto 20
20 printint count
21 outer = 0
22 inner_sum = 0
23 if outer < 4 goto 25
24 goto 38
25 inner = 0
26 if inner < outer goto 28
27 goto 33
28 t3 = inner_sum + inner
29 inner_sum = t3
30 t4 = inner + 1
31 inner = t4
32 if inner < outer goto 28
33 t5 = outer + 1
34 outer = t5
35 if outer < 4 goto 25
36 goto 38
38 printint inner_sum
39 Return 0
40 EndFunc main
