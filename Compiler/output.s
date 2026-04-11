
    .extern  printf
.text

    .globl   main
main:

    addi     sp, sp, -64
    sd       ra, 56(sp)
    sd       s0, 48(sp)
    sd       s1, 40(sp)
    sd       s2, 32(sp)
    sd       s3, 24(sp)
    sd       s4, 16(sp)
    fsd      fs0, 8(sp)
    fsd      fs1, 0(sp)
    addi     s0, sp, 64
    la       t4, .flt0
    fld      fs0, 0(t4)
    fmv.d    fa0, fs0
    la       t6, x
    fsd      fa0, 0(t6)
    # cast (char)
    la       t6, x
    fld      fa0, 0(t6)
    fcvt.w.d t4, fa0, rtz
    slli     t4, t4, 56
    srai     t4, t4, 56
    la       t6, x_to_char
    sb       t4, 0(t6)
    # cast (short)
    la       t6, x
    fld      fa0, 0(t6)
    fcvt.w.d t4, fa0, rtz
    slli     t4, t4, 48
    srai     t4, t4, 48
    la       t6, x_to_short
    sh       t4, 0(t6)
    # cast (int)
    la       t6, x
    fld      fa0, 0(t6)
    fcvt.w.d t4, fa0, rtz
    la       t6, x_to_int
    sw       t4, 0(t6)
    # cast (long)
    la       t6, x
    fld      fa0, 0(t6)
    fcvt.l.d t4, fa0, rtz
    la       t6, x_to_long
    sd       t4, 0(t6)
    # cast (float)
    la       t6, x
    fld      fa0, 0(t6)
    la       t6, x_to_float
    fcvt.s.d ft1, fa0
    fsw      ft1, 0(t6)
    la       a0, .fmt_float
    la       t6, x
    fld      fa0, 0(t6)
    fmv.x.d  a1, fa0
    call     printf
    la       a0, .fmt_char
    la       t6, x_to_char
    lb       t4, 0(t6)
    mv       a1, t4
    call     printf
    la       a0, .fmt_int
    la       t6, x_to_short
    lh       t4, 0(t6)
    mv       a1, t4
    call     printf
    la       a0, .fmt_int
    la       t6, x_to_int
    lw       t4, 0(t6)
    mv       a1, t4
    call     printf
    la       a0, .fmt_long
    la       t6, x_to_long
    ld       t4, 0(t6)
    mv       a1, t4
    call     printf
    la       a0, .fmt_float
    la       t6, x_to_float
    flw      ft1, 0(t6)
    fcvt.d.s fa0, ft1
    fmv.x.d  a1, fa0
    call     printf

    ld       s1, 40(sp)
    ld       s2, 32(sp)
    ld       s3, 24(sp)
    ld       s4, 16(sp)
    fld      fs0, 8(sp)
    fld      fs1, 0(sp)
    ld       s0, 48(sp)
    ld       ra, 56(sp)
    addi     sp, sp, 64
    li       a0, 0
    ret

.data
.fmt_int:        .asciz "%d\n"
.fmt_long:       .asciz "%ld\n"
.fmt_char:       .asciz "%c\n"
.fmt_float:      .asciz "%f\n"
x: .dword 0
x_to_char: .byte 0
x_to_float: .word 0
x_to_int: .word 0
x_to_long: .dword 0
x_to_short: .short 0
    .flt0: .double 1e+40
