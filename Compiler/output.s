
    .extern  printf
.text

    .globl   main
main:

    addi     sp, sp, -16
    sd       ra, 8(sp)
    sd       s1, 0(sp)
    la       s1, arr
    mv       t4, s1
    li       t3, 10
    sw       t3, 0(t4)
    addi     t4, s1, 4
    li       t3, 20
    sw       t3, 0(t4)
    addi     t4, s1, 8
    li       t3, 30
    sw       t3, 0(t4)
    addi     t4, s1, 12
    li       t3, 40
    sw       t3, 0(t4)
    addi     t4, s1, 16
    li       t3, 50
    sw       t3, 0(t4)
    li       t2, 0
    li       t0, 0

.L8:
    li       t5, 5
    bge      t0, t5, .L15
    slli     t1, t0, 2
    mv       t6, t1
    mv       t5, s1
    add      t5, t5, t6
    lw       t5, 0(t5)
    add      t2, t2, t5
    addi     t0, t0, 1
    j        .L8

.L15:
    la       a0, .fmt_int
    mv       a1, t2
    call     printf

    ld       ra, 8(sp)
    ld       s1, 0(sp)
    addi     sp, sp, 16
    li       a0, 0
    ret

.data
.fmt_int:        .asciz "%d\n"
arr: .word 0, 0, 0, 0, 0
