.include "pie_asm.h"

.section .text
.globl srcmb_s8
.type srcmb_s8, @function
srcmb_s8:
.balign 4
.option norvc
# x10: dst
# x11: src1
# x12: src2
# x13: right_shift
# x14: times
pie.zero.qacc
lp.setup.f  0, x14, (1f - 4)
    pie.vld.128.ip  q0, x11, 16
    pie.vld.128.ip  q1, x12, 16
    pie.vmulas.s8.qacc  q0, q1
    1:
pie.srcmb.s8.qacc q0, x13, 0, 1
pie.vst.128.ip  q0, x10, 16
ret