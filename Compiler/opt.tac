0 arr[0] = 10
1 arr[4] = 20
2 arr[8] = 30
3 arr[12] = 40
4 arr[16] = 50
5 // DEAD CONST: n = 5
6 sum = 0
7 i = 0
8 if i >= 5 goto 15
9 t1 = i << 2
10 t2 = sum + arr[t1]
11 sum = t2
12 t0 = i + 1
13 i = t0
14 goto 8
15 printint sum
