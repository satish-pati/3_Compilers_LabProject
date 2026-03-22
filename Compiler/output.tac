0 BeginFunc main 0
1 a = 14
2 // IDENTITY: a = 14 (eliminated)
3 // DEAD VAR: b = 14
4 // DEAD CONST: t0 = 15
5 // DEAD STORE: c = 15
6 // PEEPHOLE: b = 14 (redundant peephole)
7 // PEEPHOLE: a = 14 (redundant peephole)
8 // DEAD CONST: t1 = 28
9 d = 28
10 // DEAD CONST: t2 = 28
11 // DEAD VAR: x = 28
12 // DEAD CONST: t3 = 14
13 // PEEPHOLE: a = 14 (redundant peephole)
14 // DEAD CONST: t4 = 14
15 // PEEPHOLE: b = 14 (redundant peephole)
16 // DEAD CONST: t5 = 56
17 c = 56
18 // DEAD CODE: goto 19
19 // DEAD BRANCH: if 56 <= 10 always false
20 // DEAD CONST: y = 0
21 printint 1
22 // DEAD CODE: goto 26
23 // DEAD CODE: if a <= 0 goto 26
24 // DEAD CODE: // DEAD CONST: z = 0
25 // DEAD CODE: printint 999
26 // DEAD BRANCH: if 6 <= 5 always false
27 p = 0
28 printint 2
29 // DEAD CONST: t6 = 28
30 // DEAD CONST: x = 28
31 // DEAD STORE: d = 5
32 d = 10
33 printint d
34 inputint p
35 // DEAD CONST: t7 = 0
36 // PEEPHOLE: p = 0 (redundant peephole)
37 printint a
38 printint c
39 printint p
40 Return 0
41 // DEAD CODE: a = 100
42 // DEAD CODE: printint a
43 EndFunc main
