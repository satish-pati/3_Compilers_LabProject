0 arr[0] = 10
1 arr[4] = 20
2 arr[8] = 30
3 arr[12] = 40
4 arr[16] = 50
5 n = 5
6 sum = 0
7 i = 0
8 if i < n goto 13
9 goto 17
10 t0 = i + 1
11 i = t0
12 goto 8
13 t1 = i * 4
14 t2 = sum + arr[t1]
15 sum = t2
16 goto 10
17 printint sum
