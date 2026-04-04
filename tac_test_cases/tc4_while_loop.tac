0 BeginFunc main 0
1 i = 0
2 sum = 0
3 if i < 10 goto 5
4 goto 9
5 t0 = sum + i
6 sum = t0
7 t1 = i + 1
8 i = t1
9 if i < 10 goto 5
10 printint sum
11 count = 1
12 prod = 1
13 if count <= 5 goto 15
14 goto 19
15 t2 = prod * count
16 prod = t2
17 t3 = count + 1
18 count = t3
19 if count <= 5 goto 15
20 printint prod
21 Return 0
22 EndFunc main
