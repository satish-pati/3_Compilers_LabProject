0 BeginFunc add 2
1 PopParam p
2 PopParam q
3 t0 = p + q
4 s = t0
5 Return s
6 EndFunc add
7 BeginFunc multiply 2
8 PopParam x
9 PopParam y
10 t1 = x * y
11 prod = t1
12 Return prod
13 EndFunc multiply
14 BeginFunc main 0
15 PushParam 6
16 PushParam 4
17 t2 = Call add
18 result1 = t2
19 printint result1
20 PushParam 3
21 PushParam 7
22 t3 = Call multiply
23 result2 = t3
24 printint result2
25 PushParam result1
26 PushParam result2
27 t4 = Call add
28 total = t4
29 printint total
30 Return 0
31 EndFunc main
