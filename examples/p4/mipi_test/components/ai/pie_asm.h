# Note: This is an auto-generated file. Any changes made here may get over-written.
# Please use generator flow for adding any changes in the following code
# Definitions file : RV_PIE_INSN_DESC_V1.2.0.csv

#pragma once
#ifdef __ASSEMBLER__

.set regnum_x0, 0
.set regnum_x1, 1
.set regnum_x2, 2
.set regnum_x3, 3
.set regnum_x4, 4
.set regnum_x5, 5
.set regnum_x6, 6
.set regnum_x7, 7
.set regnum_x8, 8
.set regnum_x9, 9
.set regnum_x10, 10
.set regnum_x11, 11
.set regnum_x12, 12
.set regnum_x13, 13
.set regnum_x14, 14
.set regnum_x15, 15
.set regnum_x16, 16
.set regnum_x17, 17
.set regnum_x18, 18
.set regnum_x19, 19
.set regnum_x20, 20
.set regnum_x21, 21
.set regnum_x22, 22
.set regnum_x23, 23
.set regnum_x24, 24
.set regnum_x25, 25
.set regnum_x26, 26
.set regnum_x27, 27
.set regnum_x28, 28
.set regnum_x29, 29
.set regnum_x30, 30
.set regnum_x31, 31

.set regnum_q0, 0
.set regnum_q1, 1
.set regnum_q2, 2
.set regnum_q3, 3
.set regnum_q4, 4
.set regnum_q5, 5
.set regnum_q6, 6
.set regnum_q7, 7

.macro pie.vmulas.u8.xacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000000000001111111 )
.endm

.macro pie.vmulas.s8.xacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000000000001111111 )
.endm

.macro pie.vmulas.u16.xacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000000000001111111 )
.endm

.macro pie.vmulas.s16.xacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000000000001111111 )
.endm

.macro pie.vmulas.u8.qacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000000000001011111 )
.endm

.macro pie.vmulas.s8.qacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000000000001011111 )
.endm

.macro pie.vmulas.u16.qacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000000000001011111 )
.endm

.macro pie.vmulas.s16.qacc.ld.xp.qup  _qu, _rs1, _rs2, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000000000001011111 )
.endm

.macro pie.vmulas.u8.xacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000111111 )
.endm

.macro pie.vmulas.s8.xacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000111111 )
.endm

.macro pie.vmulas.u16.xacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000111111 )
.endm

.macro pie.vmulas.s16.xacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000111111 )
.endm

.macro pie.vmulas.u8.qacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000011111 )
.endm

.macro pie.vmulas.s8.qacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000011111 )
.endm

.macro pie.vmulas.u16.qacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000011111 )
.endm

.macro pie.vmulas.s16.qacc.ld.ip.qup  _qu, _rs1, _offset_16_16, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 2) & 0x3) << 23) | ((( (\_offset_16_16 >> 4) >> 0) & 0x3) << 13) | 0b00000000000000000000000000011111 )
.endm

.macro pie.fft.ams.s16.ld.r32.decp  _qu, _rs1, _qz, _qv, _qx, _qy, _qw, _select_2 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_2 >> 0) & 0x1) << 23) | 0b11000000000000000000000001111011 )
.endm

.macro pie.fft.ams.s16.ld.incp.uaup  _qu, _rs1, _qz, _qv, _qx, _qy, _qw, _select_2 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_2 >> 0) & 0x1) << 23) | 0b10000000000000000000000001111011 )
.endm

.macro pie.fft.ams.s16.ld.incp  _qu, _rs1, _qz, _qv, _qx, _qy, _qw, _select_2 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_2 >> 0) & 0x1) << 23) | 0b00000000000000000000000001111011 )
.endm

.macro pie.fft.cmul.s16.st.xp  _qy, _qx, _qv, _rs1, _rs2, _select_8, _upd_4, _select_4 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_select_4 >> 1) & 0x1) << 30) | ((( \_select_4 >> 0) & 0x1) << 27) | ((( \_upd_4 >> 0) & 0x3) << 25) | ((( \_select_8 >> 2) & 0x1) << 24) | ((( \_select_8 >> 0) & 0x3) << 13) | 0b10000000000000000000000001011011 )
.endm

.macro pie.fft.ams.s16.st.incp  _qv, _qu, _rs2, _rs1, _qx, _qy, _qw, _select_2 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x3) << 13) | ((( regnum_\_qw >> 2) & 0x1) << 24) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_select_2 >> 0) & 0x1) << 30) | 0b00000000000000000000000001011011 )
.endm

.macro pie.vsub.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.u8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.s8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.u16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vsub.s16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100010000110000000111011 )
.endm

.macro pie.vmul.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.u8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.s8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.u16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmul.s16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001100000000110000000111011 )
.endm

.macro pie.vmin.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.u8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.s8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.u16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmin.s16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000010000110000000111011 )
.endm

.macro pie.vmax.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.u8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.s8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.u16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vmax.s16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00010001000000000110000000111011 )
.endm

.macro pie.vadd.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.u8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.s8.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.u16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.vadd.s16.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000110000000111011 )
.endm

.macro pie.cmul.u8.ld.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.s8.ld.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.u16.ld.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.s16.ld.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.u8.st.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.s8.st.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.u16.st.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.cmul.s16.st.incp  _qu, _rs1, _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 1) & 0x1) << 23) | ((( \_select_4 >> 0) & 0x1) << 19) | 0b00000001000000000100000000111011 )
.endm

.macro pie.fft.cmul.s16.ld.xp  _qu, _rs1, _rs2, _qv, _qy, _qx, _select_8 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_select_8 >> 1) & 0x3) << 30) | ((( \_select_8 >> 0) & 0x1) << 13) | 0b00000001000000000000000000111011 )
.endm

.macro pie.vmulas.u8.qacc.ldbc.incp.qup  _qu, _rs1, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000110000000111011 )
.endm

.macro pie.vmulas.s8.qacc.ldbc.incp.qup  _qu, _rs1, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000110000000111011 )
.endm

.macro pie.vmulas.u16.qacc.ldbc.incp.qup  _qu, _rs1, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000110000000111011 )
.endm

.macro pie.vmulas.s16.qacc.ldbc.incp.qup  _qu, _rs1, _qx, _qy, _qv, _qw 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x3) << 28) | ((( regnum_\_qv >> 2) & 0x1) << 19) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000110000000111011 )
.endm

.macro pie.vmulas.u8.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s8.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u16.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s16.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u8.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s8.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u16.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s16.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u8.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s8.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u16.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s16.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u8.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s8.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u16.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.s16.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000100000000111011 )
.endm

.macro pie.vmulas.u8.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s8.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u16.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s16.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u8.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s8.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u16.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s16.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u8.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s8.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u16.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s16.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u8.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s8.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.u16.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.vmulas.s16.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000100000000111011 )
.endm

.macro pie.src.q.qup.ld.ip  _qz, _qu, _qv, _rs1, _offset_256_16 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 6) & 0x1) << 23) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00000000000010000010000000111011 )
.endm

.macro pie.src.q.ld.ip  _qu, _rs1, _offset_256_16, _qv, _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 3) & 0xF) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x7) << 10) | 0b00000000000000000010000000111011 )
.endm

.macro pie.vsub.u32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01010000100010000000000000111011 )
.endm

.macro pie.vsub.s32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01010000100010000000000000111011 )
.endm

.macro pie.vsub.u32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01010000100010000000000000111011 )
.endm

.macro pie.vsub.s32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01010000100010000000000000111011 )
.endm

.macro pie.vmin.u32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b00010000100010000000000000111011 )
.endm

.macro pie.vmin.s32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b00010000100010000000000000111011 )
.endm

.macro pie.vmin.u32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b00010000100010000000000000111011 )
.endm

.macro pie.vmin.s32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b00010000100010000000000000111011 )
.endm

.macro pie.vmax.u32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01010000000010000000000000111011 )
.endm

.macro pie.vmax.s32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01010000000010000000000000111011 )
.endm

.macro pie.vmax.u32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01010000000010000000000000111011 )
.endm

.macro pie.vmax.s32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01010000000010000000000000111011 )
.endm

.macro pie.vadd.u32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b00010000000010000000000000111011 )
.endm

.macro pie.vadd.s32.ld.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b00010000000010000000000000111011 )
.endm

.macro pie.vadd.u32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b00010000000010000000000000111011 )
.endm

.macro pie.vadd.s32.st.incp  _qu, _rs1, _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b00010000000010000000000000111011 )
.endm

.macro pie.vsmulas.u8.qacc.ld.incp  _qu, _rs1, _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_16 >> 3) & 0x1) << 29) | ((( \_select_16 >> 0) & 0x7) << 20) | 0b00000000100010000000000000111011 )
.endm

.macro pie.vsmulas.s8.qacc.ld.incp  _qu, _rs1, _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_16 >> 3) & 0x1) << 29) | ((( \_select_16 >> 0) & 0x7) << 20) | 0b00000000100010000000000000111011 )
.endm

.macro pie.vsmulas.u16.qacc.ld.incp  _qu, _rs1, _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_16 >> 3) & 0x1) << 29) | ((( \_select_16 >> 0) & 0x7) << 20) | 0b00000000100010000000000000111011 )
.endm

.macro pie.vsmulas.s16.qacc.ld.incp  _qu, _rs1, _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_16 >> 3) & 0x1) << 29) | ((( \_select_16 >> 0) & 0x7) << 20) | 0b00000000100010000000000000111011 )
.endm

.macro pie.vmulas.u8.qacc.ldbc.incp  _qu, _rs1, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000000000000111011 )
.endm

.macro pie.vmulas.s8.qacc.ldbc.incp  _qu, _rs1, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000000000000111011 )
.endm

.macro pie.vmulas.u16.qacc.ldbc.incp  _qu, _rs1, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000000000000111011 )
.endm

.macro pie.vmulas.s16.qacc.ldbc.incp  _qu, _rs1, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.xacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.xacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.qacc.ld.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.qacc.st.xp  _qu, _rs1, _rs2, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.xacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.xacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.qacc.ld.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.qacc.st.ip  _qu, _rs1, _offset_16_16, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000000000000000000000000111011 )
.endm

.macro pie.vldbc.32.ip  _qu, _rs1, _offset_256_4 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( (\_offset_256_4 >> 2) >> 6) & 0x3) << 28) | ((( (\_offset_256_4 >> 2) >> 3) & 0x7) << 20) | ((( (\_offset_256_4 >> 2) >> 0) & 0x7) << 7) | 0b00000001100010000110010000011011 )
.endm

.macro pie.vldbc.32  _qu, _rs1 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | 0b00000001000010000110010000011011 )
.endm

.macro pie.ld.128.usar.ip  _qu, _rs1, _offset_256_16 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 28) | ((( (\_offset_256_16 >> 4) >> 3) & 0x7) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x7) << 7) | 0b00000001000010000110000000011011 )
.endm

.macro pie.vldbc.8.ip  _qu, _rs1, _offset_256_4 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( (\_offset_256_4 >> 0) >> 6) & 0x3) << 28) | ((( (\_offset_256_4 >> 0) >> 3) & 0x7) << 20) | ((( (\_offset_256_4 >> 0) >> 0) & 0x7) << 7) | 0b00000001100000000111110000011011 )
.endm

.macro pie.vldbc.16.ip  _qu, _rs1, _offset_256_4 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( (\_offset_256_4 >> 1) >> 6) & 0x3) << 28) | ((( (\_offset_256_4 >> 1) >> 3) & 0x7) << 20) | ((( (\_offset_256_4 >> 1) >> 0) & 0x7) << 7) | 0b00000001100000000111110000011011 )
.endm

.macro pie.vldbc.8  _qu, _rs1 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | 0b00000001000000000111110000011011 )
.endm

.macro pie.vldbc.16  _qu, _rs1 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | 0b00000001000000000111110000011011 )
.endm

.macro pie.vld.l.64.ip  _qu, _rs1, _offset_256_8 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_8 >> 3) >> 7) & 0x1) << 30) | ((( (\_offset_256_8 >> 3) >> 3) & 0xF) << 20) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 7) | 0b00000001000000000111100000011011 )
.endm

.macro pie.vld.h.64.ip  _qu, _rs1, _offset_256_8 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_8 >> 3) >> 7) & 0x1) << 30) | ((( (\_offset_256_8 >> 3) >> 3) & 0xF) << 20) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 7) | 0b00000001000000000111100000011011 )
.endm

.macro pie.vst.l.64.ip  _qu, _rs1, _offset_256_8 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_8 >> 3) >> 7) & 0x1) << 30) | ((( (\_offset_256_8 >> 3) >> 3) & 0xF) << 20) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 7) | 0b00000001000000000111100000011011 )
.endm

.macro pie.vst.h.64.ip  _qu, _rs1, _offset_256_8 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_8 >> 3) >> 7) & 0x1) << 30) | ((( (\_offset_256_8 >> 3) >> 3) & 0xF) << 20) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 7) | 0b00000001000000000111100000011011 )
.endm

.macro pie.src.q.ld.xp  _qu, _rs1, _rs2, _qv, _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | 0b00000001000000000111010000011011 )
.endm

.macro pie.vld.128.ip  _qu, _rs1, _offset_256_16 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 30) | ((( (\_offset_256_16 >> 4) >> 3) & 0xF) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x7) << 7) | 0b00000001000000000111000000011011 )
.endm

.macro pie.vst.128.ip  _qu, _rs1, _offset_256_16 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 30) | ((( (\_offset_256_16 >> 4) >> 3) & 0xF) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x7) << 7) | 0b00000001000000000111000000011011 )
.endm

.macro pie.stf.64.ip  _rs1, _fs4, _offset_256_8 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_fs4 >> 0) & 0xF) << 7) | ((( (\_offset_256_8 >> 3) >> 6) & 0x3) << 29) | ((( (\_offset_256_8 >> 3) >> 3) & 0x7) << 25) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 20) | 0b00010001100000000110000000011011 )
.endm

.macro pie.ldf.64.ip  _rs1, _fd4, _offset_256_8 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_fd4 >> 0) & 0xF) << 7) | ((( (\_offset_256_8 >> 3) >> 6) & 0x3) << 29) | ((( (\_offset_256_8 >> 3) >> 3) & 0x7) << 25) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 20) | 0b00010001000000000110000000011011 )
.endm

.macro pie.ldxq.32  _qu, _qx, _rs1, _select_4, _select_8 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( \_select_4 >> 1) & 0x1) << 30) | ((( \_select_4 >> 0) & 0x1) << 22) | ((( \_select_8 >> 1) & 0x3) << 20) | ((( \_select_8 >> 0) & 0x1) << 10) | 0b00000001000000000110000000011011 )
.endm

.macro pie.stxq.32  _qu, _qx, _rs1, _select_4, _select_8 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( \_select_4 >> 1) & 0x1) << 30) | ((( \_select_4 >> 0) & 0x1) << 22) | ((( \_select_8 >> 1) & 0x3) << 20) | ((( \_select_8 >> 0) & 0x1) << 10) | 0b00000001000000000110000000011011 )
.endm

.macro pie.vsub.u8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011111100000000011011 )
.endm

.macro pie.vsub.s8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011111100000000011011 )
.endm

.macro pie.vsub.u16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011111100000000011011 )
.endm

.macro pie.vsub.s16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011111100000000011011 )
.endm

.macro pie.vmul.u8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011110100000000011011 )
.endm

.macro pie.vmul.s8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011110100000000011011 )
.endm

.macro pie.vmul.u16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011110100000000011011 )
.endm

.macro pie.vmul.s16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011110100000000011011 )
.endm

.macro pie.vmin.u8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011101100000000011011 )
.endm

.macro pie.vmin.s8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011101100000000011011 )
.endm

.macro pie.vmin.u16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011101100000000011011 )
.endm

.macro pie.vmin.s16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011101100000000011011 )
.endm

.macro pie.vmax.u8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011100100000000011011 )
.endm

.macro pie.vmax.s8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011100100000000011011 )
.endm

.macro pie.vmax.u16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011100100000000011011 )
.endm

.macro pie.vmax.s16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011100100000000011011 )
.endm

.macro pie.vadd.u8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011000100000000011011 )
.endm

.macro pie.vadd.s8  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011000100000000011011 )
.endm

.macro pie.vadd.u16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000011000100000000011011 )
.endm

.macro pie.vadd.s16  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000011000100000000011011 )
.endm

.macro pie.cmul.u8  _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 0) & 0x3) << 15) | 0b00000001000010000100000000011011 )
.endm

.macro pie.cmul.s8  _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 0) & 0x3) << 15) | 0b00000001000010000100000000011011 )
.endm

.macro pie.cmul.u16  _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_4 >> 0) & 0x3) << 15) | 0b00000001000010000100000000011011 )
.endm

.macro pie.cmul.s16  _qz, _qx, _qy, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_4 >> 0) & 0x3) << 15) | 0b00000001000010000100000000011011 )
.endm

.macro pie.slcxxp.2q  _qu, _qv, _rs1, _rs2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000100000000011011 )
.endm

.macro pie.srcxxp.2q  _qu, _qv, _rs1, _rs2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000100000000011011 )
.endm

.macro pie.fft.r2bf.s16.st.incp  _qu, _qx, _qy, _rs1, _select_4 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_4 >> 0) & 0x3) << 28) | 0b01000001100110000010000000011011 )
.endm

.macro pie.ld.qacc.l.l.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 27) | ((( (\_offset_256_16 >> 4) >> 6) & 0x1) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b01000101100010000010000000011011 )
.endm

.macro pie.ld.qacc.h.l.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 27) | ((( (\_offset_256_16 >> 4) >> 6) & 0x1) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b01000101100010000010000000011011 )
.endm

.macro pie.st.qacc.l.l.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 27) | ((( (\_offset_256_16 >> 4) >> 6) & 0x1) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b01000101100010000010000000011011 )
.endm

.macro pie.st.qacc.h.l.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_16 >> 4) >> 7) & 0x1) << 27) | ((( (\_offset_256_16 >> 4) >> 6) & 0x1) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b01000101100010000010000000011011 )
.endm

.macro pie.ld.qacc.l.h.32.ip  _rs1, _offset_256_4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_4 >> 2) >> 7) & 0x1) << 27) | ((( (\_offset_256_4 >> 2) >> 6) & 0x1) << 25) | ((( (\_offset_256_4 >> 2) >> 0) & 0x3F) << 7) | 0b01000001100010000010000000011011 )
.endm

.macro pie.ld.qacc.h.h.32.ip  _rs1, _offset_256_4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_4 >> 2) >> 7) & 0x1) << 27) | ((( (\_offset_256_4 >> 2) >> 6) & 0x1) << 25) | ((( (\_offset_256_4 >> 2) >> 0) & 0x3F) << 7) | 0b01000001100010000010000000011011 )
.endm

.macro pie.st.qacc.l.h.32.ip  _rs1, _offset_256_4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | ((( (\_offset_256_4 >> 2) >> 7) & 0x1) << 27) | ((( (\_offset_256_4 >> 2) >> 6) & 0x1) << 25) | ((( (\_offset_256_4 >> 2) >> 0) & 0x3F) << 7) | 0b01000001100010000010000000011011 )
.endm

.macro pie.st.qacc.h.h.32.ip  _rs1, _offset_256_4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | ((( (\_offset_256_4 >> 2) >> 7) & 0x1) << 27) | ((( (\_offset_256_4 >> 2) >> 6) & 0x1) << 25) | ((( (\_offset_256_4 >> 2) >> 0) & 0x3F) << 7) | 0b01000001100010000010000000011011 )
.endm

.macro pie.vprelu.s8  _qz, _qx, _qy, _rs1 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 27) | 0b00000001100010000010000000011011 )
.endm

.macro pie.vprelu.s16  _qz, _qx, _qy, _rs1 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 27) | 0b00000001100010000010000000011011 )
.endm

.macro pie.ldqa.u8.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00000001000010000010000000011011 )
.endm

.macro pie.ldqa.s8.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00000001000010000010000000011011 )
.endm

.macro pie.ldqa.u16.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00000001000010000010000000011011 )
.endm

.macro pie.ldqa.s16.128.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 25) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00000001000010000010000000011011 )
.endm

.macro pie.vcmp.eq.u8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.eq.s8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.eq.u16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.eq.s16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.lt.u8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.lt.s8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.lt.u16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.lt.s16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.gt.u8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.gt.s8  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.gt.u16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vcmp.gt.s16  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000010000000011011 )
.endm

.macro pie.vld.l.64.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | 0b01000001000010000000000010011011 )
.endm

.macro pie.vld.h.64.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | 0b01000001000010000000000010011011 )
.endm

.macro pie.vst.l.64.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 29) | ((( 0 >> 0) & 0x1) << 28) | 0b01000001000010000000000010011011 )
.endm

.macro pie.vst.h.64.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 29) | ((( 1 >> 0) & 0x1) << 28) | 0b01000001000010000000000010011011 )
.endm

.macro pie.st.u.xacc.ip  _rs1, _offset_256_8 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 31) | ((( (\_offset_256_8 >> 3) >> 3) & 0x1F) << 25) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 20) | 0b01000001000010000000000000011011 )
.endm

.macro pie.st.s.xacc.ip  _rs1, _offset_256_8 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 31) | ((( (\_offset_256_8 >> 3) >> 3) & 0x1F) << 25) | ((( (\_offset_256_8 >> 3) >> 0) & 0x7) << 20) | 0b01000001000010000000000000011011 )
.endm

.macro pie.fft.r2bf.s16  _qu, _qz, _qx, _qy, _select_2 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( \_select_2 >> 0) & 0x1) << 15) | 0b10010001000010000000000000011011 )
.endm

.macro pie.ld.ua_state.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 29) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00010001000010000000000000011011 )
.endm

.macro pie.st.ua_state.ip  _rs1, _offset_256_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 29) | ((( (\_offset_256_16 >> 4) >> 6) & 0x3) << 20) | ((( (\_offset_256_16 >> 4) >> 0) & 0x3F) << 7) | 0b00010001000010000000000000011011 )
.endm

.macro pie.vsub.u32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000111000010000000000000011011 )
.endm

.macro pie.vsub.s32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000111000010000000000000011011 )
.endm

.macro pie.vmin.u32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000101000010000000000000011011 )
.endm

.macro pie.vmin.s32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000101000010000000000000011011 )
.endm

.macro pie.vmax.u32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000011000010000000000000011011 )
.endm

.macro pie.vmax.s32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000011000010000000000000011011 )
.endm

.macro pie.vadd.u32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000010000000000000011011 )
.endm

.macro pie.vadd.s32  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000010000000000000011011 )
.endm

.macro pie.vsmulas.u8.qacc  _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 25) | 0b00000001000000001000000000011011 )
.endm

.macro pie.vsmulas.s8.qacc  _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 25) | 0b00000001000000001000000000011011 )
.endm

.macro pie.vsmulas.u16.qacc  _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 25) | 0b00000001000000001000000000011011 )
.endm

.macro pie.vsmulas.s16.qacc  _qx, _qy, _select_16 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 25) | 0b00000001000000001000000000011011 )
.endm

.macro pie.vsl.32  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 31) | 0b00000001000000000000100000011011 )
.endm

.macro pie.vsr.32  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 31) | 0b00000001000000000000100000011011 )
.endm

.macro pie.slci.2q  _qu, _qv, _select_16 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 7) | 0b00000001000000000000000000011011 )
.endm

.macro pie.srci.2q  _qu, _qv, _select_16 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_16 >> 0) & 0xF) << 7) | 0b00000001000000000000000000011011 )
.endm

.macro pie.vrelu.s8  _qv, _rs2, _rs1 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 27) | 0b10000000000010000110000000011011 )
.endm

.macro pie.vrelu.s16  _qv, _rs2, _rs1 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 27) | 0b10000000000010000110000000011011 )
.endm

.macro pie.vmul.s16.s8xs8  _qu, _qv, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | 0b00000000000010000110000000011011 )
.endm

.macro pie.vldbc.32.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | 0b01000000000000000110000010011011 )
.endm

.macro pie.ld.128.usar.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | 0b00000000000000000110000010011011 )
.endm

.macro pie.vldbc.8.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | 0b00000000000000000110000000011011 )
.endm

.macro pie.vldbc.16.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | 0b00000000000000000110000000011011 )
.endm

.macro pie.vcmp.eq.u32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vcmp.eq.s32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vcmp.lt.u32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vcmp.lt.s32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vcmp.gt.u32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vcmp.gt.s32  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 31) | 0b01000000000010000100000000011011 )
.endm

.macro pie.vld.128.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 29) | 0b00010000000010000100000000011011 )
.endm

.macro pie.vst.128.xp  _qu, _rs1, _rs2 
.word (((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 29) | 0b00010000000010000100000000011011 )
.endm

.macro pie.stf.64.xp  _rs1, _rs2, _fs4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_fs4 >> 0) & 0xF) << 7) | 0b00100000000010000100000000011011 )
.endm

.macro pie.movi.8.q  _qz, _rs1, _select_16 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 27) | ((( \_select_16 >> 0) & 0xF) << 7) | 0b00000000000010000100000000011011 )
.endm

.macro pie.movi.16.q  _qz, _rs1, _select_16 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 27) | ((( \_select_16 >> 0) & 0xF) << 7) | 0b00000000000010000100000000011011 )
.endm

.macro pie.movi.8.a  _qv, _rd, _select_16 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 27) | ((( \_select_16 >> 0) & 0xF) << 15) | 0b10000010000000000101100000011011 )
.endm

.macro pie.movi.16.a  _qv, _rd, _select_16 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 27) | ((( \_select_16 >> 0) & 0xF) << 15) | 0b10000010000000000101100000011011 )
.endm

.macro pie.ldf.64.xp  _rs1, _rs2, _fd4 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_fd4 >> 0) & 0xF) << 7) | 0b00011010000000000101100000011011 )
.endm

.macro pie.ld.xacc.ip  _rs1, _offset_256_8 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( (\_offset_256_8 >> 3) >> 4) & 0xF) << 20) | ((( (\_offset_256_8 >> 3) >> 0) & 0xF) << 7) | 0b00010010000000000101100000011011 )
.endm

.macro pie.stf.128.xp  _rs1, _rs2, _fs3 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_fs3 >> 0) & 0x7) << 7) | 0b00000010000000000101110000011011 )
.endm

.macro pie.stf.128.ip  _rs1, _fs3, _offset_16_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_fs3 >> 0) & 0x7) << 7) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000010000000000101100000011011 )
.endm

.macro pie.srcmb.u8.qacc  _qv, _rs1, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000010000000000100100000011011 )
.endm

.macro pie.srcmb.s8.qacc  _qv, _rs1, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 0 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000010000000000100100000011011 )
.endm

.macro pie.srcmb.u16.qacc  _qv, _rs1, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000010000000000100100000011011 )
.endm

.macro pie.srcmb.s16.qacc  _qv, _rs1, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( 1 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000010000000000100100000011011 )
.endm

.macro pie.movx.r.sar.bytes  _rd 
.word (((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | 0b00010010001100000100000000011011 )
.endm

.macro pie.movx.w.sar.bytes  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | 0b00010010001100000100000000011011 )
.endm

.macro pie.movx.r.sar  _rd 
.word (((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | 0b00010010001000000100000000011011 )
.endm

.macro pie.movx.w.sar  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | 0b00010010001000000100000000011011 )
.endm

.macro pie.movx.r.perf  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | 0b00010010000100000100000000011011 )
.endm

.macro pie.movx.w.perf  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | 0b00010010000100000100000000011011 )
.endm

.macro pie.movx.r.fft.bit.width  _rd 
.word (((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | 0b00010010000000000100000000011011 )
.endm

.macro pie.movx.w.fft.bit.width  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | 0b00010010000000000100000000011011 )
.endm

.macro pie.ldf.128.xp  _rs1, _rs2, _fd3 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( \_fd3 >> 0) & 0x7) << 7) | 0b00000010000000000100010000011011 )
.endm

.macro pie.ldf.128.ip  _rs1, _fd3, _offset_16_16 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_fd3 >> 0) & 0x7) << 7) | ((( (\_offset_16_16 >> 4) >> 0) & 0xF) << 20) | 0b00000010000000000100000000011011 )
.endm

.macro pie.orq  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 1) & 0x1) << 28) | 0b00000000000000001100000000011011 )
.endm

.macro pie.xorq  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 1) & 0x1) << 28) | 0b00000000000000001100000000011011 )
.endm

.macro pie.andq  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 2 >> 0) & 0x1) << 23) | ((( 2 >> 1) & 0x1) << 28) | 0b00000000000000001100000000011011 )
.endm

.macro pie.notq  _qz, _qx 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( 3 >> 0) & 0x1) << 23) | ((( 3 >> 1) & 0x1) << 28) | 0b00000000000000001100000000011011 )
.endm

.macro pie.vmul.s16.s8xs16.l  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | 0b00000000000000000100000000011011 )
.endm

.macro pie.vmul.s16.s8xs16.h  _qz, _qx, _qy 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | 0b00000000000000000100000000011011 )
.endm

.macro pie.vldhbc.16.incp  _qu, _qv, _rs1 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | 0b00000000100110000010100000011011 )
.endm

.macro pie.srs.u.xacc  _rd, _rs1, _select_2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 27) | 0b00000000100110000010000000011011 )
.endm

.macro pie.srs.s.xacc  _rd, _rs1, _select_2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 27) | 0b00000000100110000010000000011011 )
.endm

.macro pie.srcq.128.st.incp  _qx, _qy, _rs1 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | 0b00000000100010000010000000011011 )
.endm

.macro pie.srcmb.u8.q.qacc  _qv, _qw, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( 0 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000000000010000010100000011011 )
.endm

.macro pie.srcmb.s8.q.qacc  _qv, _qw, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( 0 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000000000010000010100000011011 )
.endm

.macro pie.srcmb.u16.q.qacc  _qv, _qw, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( 1 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000000000010000010100000011011 )
.endm

.macro pie.srcmb.s16.q.qacc  _qv, _qw, _select_2, _select2_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qw >> 0) & 0x7) << 20) | ((( 1 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | ((( \_select_2 >> 0) & 0x1) << 8) | ((( \_select2_2 >> 0) & 0x1) << 7) | 0b00000000000010000010100000011011 )
.endm

.macro pie.movx.r.xacc.l  _rd 
.word (((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | ((( 0 >> 0) & 0x1) << 28) | 0b00000000000110000010000000011011 )
.endm

.macro pie.movx.r.xacc.h  _rd 
.word (((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 12) | ((( 1 >> 0) & 0x1) << 28) | 0b00000000000110000010000000011011 )
.endm

.macro pie.movx.w.xacc.l  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | ((( 0 >> 0) & 0x1) << 28) | 0b00000000000110000010000000011011 )
.endm

.macro pie.movx.w.xacc.h  _rd, _rs1 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 12) | ((( 1 >> 0) & 0x1) << 28) | 0b00000000000110000010000000011011 )
.endm

.macro pie.min.u8.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010001010000000011011 )
.endm

.macro pie.min.s8.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010001010000000011011 )
.endm

.macro pie.min.u16.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010001010000000011011 )
.endm

.macro pie.min.s16.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010001010000000011011 )
.endm

.macro pie.max.u8.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000010000000011011 )
.endm

.macro pie.max.s8.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000010000000011011 )
.endm

.macro pie.max.u16.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 27) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000010000000011011 )
.endm

.macro pie.max.s16.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 27) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000010000000011011 )
.endm

.macro pie.ldqa.u8.128.xp  _rs1, _rs2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000010000000000010000000011011 )
.endm

.macro pie.ldqa.s8.128.xp  _rs1, _rs2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000010000000000010000000011011 )
.endm

.macro pie.ldqa.u16.128.xp  _rs1, _rs2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000010000000000010000000011011 )
.endm

.macro pie.ldqa.s16.128.xp  _rs1, _rs2 
.word (((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( ((regnum_\_rs2 & 0x7) | ((regnum_\_rs2 >> 1) & 0x8)) >> 0) & 0xF) << 20) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000010000000000010000000011011 )
.endm

.macro pie.vmulas.u8.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.s8.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.u16.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.s16.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.u8.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.s8.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.u16.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.vmulas.s16.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 30) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000000000010000000011011 )
.endm

.macro pie.src.q.qup  _qu, _qv, _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | 0b10000000100010001000000000011011 )
.endm

.macro pie.src.q  _qu, _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( regnum_\_qu >> 0) & 0x7) << 25) | 0b10000000100010000000000000011011 )
.endm

.macro pie.movi.32.q  _qz, _rs1, _select_4 
.word (((( regnum_\_qz >> 0) & 0x7) << 20) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_4 >> 0) & 0x3) << 7) | 0b00000010100010000000000000011011 )
.endm

.macro pie.movi.32.a  _qv, _rd, _select_4 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( \_select_4 >> 0) & 0x3) << 15) | 0b00000000100010000000000000011011 )
.endm

.macro pie.min.u32.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 31) | 0b00000010000110001000100000011011 )
.endm

.macro pie.min.s32.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 31) | 0b00000010000110001000100000011011 )
.endm

.macro pie.max.u32.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 0 >> 0) & 0x1) << 31) | 0b00000010000110000000100000011011 )
.endm

.macro pie.max.s32.a  _qv, _rd 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rd & 0x7) | ((regnum_\_rd >> 1) & 0x8)) >> 0) & 0xF) << 7) | ((( 1 >> 0) & 0x1) << 31) | 0b00000010000110000000100000011011 )
.endm

.macro pie.fft.vst.r32.decp  _qv, _rs1, _select_2 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | ((( \_select_2 >> 0) & 0x1) << 10) | 0b10000000000110000000100010011011 )
.endm

.macro pie.fft.bitrev  _qv, _rs1 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( ((regnum_\_rs1 & 0x7) | ((regnum_\_rs1 >> 1) & 0x8)) >> 0) & 0xF) << 15) | 0b00000000000110000000110010011011 )
.endm

.macro pie.zero.q  _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | 0b00000000000110000000100010011011 )
.endm

.macro pie.vclamp.s16  _qv, _select_16 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( \_select_16 >> 0) & 0xF) << 15) | 0b00000000000110000000100000011011 )
.endm

.macro pie.vsl.8  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 31) | ((( 0 >> 0) & 0x1) << 10) | 0b00000000000110000000000000011011 )
.endm

.macro pie.vsl.16  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 31) | ((( 1 >> 0) & 0x1) << 10) | 0b00000000000110000000000000011011 )
.endm

.macro pie.vsr.8  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 31) | ((( 0 >> 0) & 0x1) << 10) | 0b00000000000110000000000000011011 )
.endm

.macro pie.vsr.16  _qu, _qv 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 31) | ((( 1 >> 0) & 0x1) << 10) | 0b00000000000110000000000000011011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000000000000011011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.xacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 0 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000000000000011011 )
.endm

.macro pie.vmulas.s16.s8xs16.l.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000000010000000000000011011 )
.endm

.macro pie.vmulas.s16.s8xs16.h.qacc  _qx, _qy 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( regnum_\_qy >> 0) & 0x7) << 10) | ((( 1 >> 0) & 0x1) << 28) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000000010000000000000011011 )
.endm

.macro pie.vzip.8  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 0 >> 0) & 0x1) << 10) | 0b00000000000000000000100010011011 )
.endm

.macro pie.vzip.16  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | ((( 1 >> 0) & 0x1) << 10) | 0b00000000000000000000100010011011 )
.endm

.macro pie.vunzip.8  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 0 >> 0) & 0x1) << 10) | 0b00000000000000000000100010011011 )
.endm

.macro pie.vunzip.16  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | ((( 1 >> 0) & 0x1) << 10) | 0b00000000000000000000100010011011 )
.endm

.macro pie.vzip.32  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 0 >> 0) & 0x1) << 23) | 0b00000000000000000000100000011011 )
.endm

.macro pie.vunzip.32  _qv, _qu 
.word (((( regnum_\_qv >> 0) & 0x7) << 28) | ((( regnum_\_qu >> 0) & 0x7) << 25) | ((( 1 >> 0) & 0x1) << 23) | 0b00000000000000000000100000011011 )
.endm

.macro pie.mov.u8.qacc  _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( 0 >> 0) & 0x1) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000100000000000000000011011 )
.endm

.macro pie.mov.s8.qacc  _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( 0 >> 0) & 0x1) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000100000000000000000011011 )
.endm

.macro pie.mov.u16.qacc  _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( 1 >> 0) & 0x1) << 10) | ((( 0 >> 0) & 0x1) << 31) | 0b00000000100000000000000000011011 )
.endm

.macro pie.mov.s16.qacc  _qx 
.word (((( regnum_\_qx >> 0) & 0x7) << 7) | ((( 1 >> 0) & 0x1) << 10) | ((( 1 >> 0) & 0x1) << 31) | 0b00000000100000000000000000011011 )
.endm

.macro pie.zero.xacc   
.word (((( 0 >> 0) & 0x1) << 28) | 0b00000000000000000000000000011011 )
.endm

.macro pie.zero.qacc   
.word (((( 1 >> 0) & 0x1) << 28) | 0b00000000000000000000000000011011 )
.endm

.macro addx _rd, _rs1, _rs2, _shamt
.word ((0b0110011 << 0) | (regnum_\_rd & 0x1F) << 7) | ((\_shamt & 0x3) << 12) | (0b0 << 14) | ((regnum_\_rs1 & 0x1F) << 15) | ((regnum_\_rs2 & 0x1F) << 20) | (0b0000010 << 25)
.endm

.macro subx _rd, _rs1, _rs2, _shamt
.word ((0b0110011 << 0) | (regnum_\_rd & 0x1F) << 7) | ((\_shamt & 0x3) << 12) | (0b1 << 14) | ((regnum_\_rs1 & 0x1F) << 15) | ((regnum_\_rs2 & 0x1F) << 20) | (0b0000010 << 25)
.endm

.macro lp.setup _L, _rs1, _uimmL
.word ((0b0001011 << 0) | ((\_L & 0x1) << 7) | (0b100 << 12) | ((regnum_\_rs1 & 0x1F) << 15) | ((((\_uimmL-(.)) >> 1) & 0xFFF) << 20))
.endm

.macro lp.setup.f _L, _rs1, _uimmL
.word ((0b0001011 << 0) | ((\_L & 0x1) << 7) | (0b110 << 12) | ((regnum_\_rs1 & 0x1F) << 15) | ((((\_uimmL-(.)) >> 1) & 0xFFF) << 20))
.endm

#endif /* __ASSEMBLER__ */
