#include "regs.h"
#include "sse.h"
#include "avx.h"

DATA one(SB)/8,$1.0
GLOBL one(SB), $8

TEXT dotvec2_sse4(SB), 1, $0
	MOVUPD a+0(FP), X0
	MOVUPD b+24(FP), X1
	DPPD(rX1, rX0)		/* DPPD $0x31, X1, X0 */
	RET

TEXT dotvec2_avx(SB), 1, $0
	MOVQ SP, AX
	ADDQ $8, AX
	VMOVUPD_128mr(rAX, rX0)
	ADDQ $24, AX
	VMOVUPD_128mr(rAX, rX1)
	VDPPD(rX1, rX0, rX0)		/* VDPPD $0x31, X1, X0, X0 */
	VZEROUPPER
	RET

TEXT dotvec3_sse4(SB), 1, $0
	MOVUPD a+0(FP), X0
	MOVUPD b+32(FP), X1
	DPPD(rX1, rX0)		/* DPPD $0x31, X1, X0 */
	MOVSD a+16(FP), X1
	MULSD b+48(FP), X1
	ADDSD X1, X0
	RET

TEXT dotvec3_avx(SB), 1, $0
	MOVQ SP, AX
	ADDQ $8, AX
	VMOVUPD_128mr(rAX, rX0)
	ADDQ $32, AX
	VMOVUPD_128mr(rAX, rX1)
	VDPPD(rX1, rX0, rX0)		/* VDPPD $0x31, X1, X0, X0 */
	MOVSD a+16(FP), X1
	MOVSD b+48(FP), X2
	VFMADD231SD(rX1, rX2, rX0)
	VZEROUPPER
	RET

TEXT Pt2b(SB), 1, $0
	MOVQ BP, DI
	MOVSD x+8(FP), X0
	MOVSD X0, 0(DI)
	MOVSD y+16(FP), X0
	MOVSD X0, 8(DI)
	MOVSD w+24(FP), X0
	MOVSD X0, 16(DI)
	RET

TEXT hsubpd(SB), 1, $0
	MOVLPD a+0(FP), X0
	MOVHPD b+8(FP), X0
	HSUBPD(rX0, rX0)	/* HSUBPD X0, X0 */
	RET

TEXT crossvec3_sse(SB), 1, $0
	MOVLPD b+40(FP), X0
	MOVHPD a+8(FP), X0	/* X0 := [a.x][b.x] */
	MOVLPD a+16(FP), X1
	MOVHPD b+48(FP), X1	/* X1 := [b.y][a.y] */
	MOVLPD b+56(FP), X2
	MOVHPD a+24(FP), X2	/* X2 := [a.z][b.z] */
	MOVAPD X1, X3
	MULPD X2, X3
	HSUBPD(rX3, rX3)	/* x */
	MOVAPD X2, X4
	SHUFPD $0x1, X4, X4
	MULPD X0, X4
	HSUBPD(rX4, rX4)	/* y */
	MOVAPD X0, X5
	MULPD X1, X5
	SHUFPD $0x1, X5, X5
	HSUBPD(rX5, rX5)	/* z */
	MOVQ BP, DI
	MOVSD X3, 0(DI)
	MOVSD X4, 8(DI)
	MOVSD X5, 16(DI)
	XORPD X0, X0
	MOVSD X0, 24(DI)
	RET

TEXT crossvec3_avx(SB), 1, $0
	MOVQ SP, AX
	ADDQ $8, AX
	

TEXT fma(SB), 1, $0
	MOVSD a+0(FP), X0
	MOVSD b+8(FP), X1
	MOVSD c+16(FP), X2
	VFMADD231SD(rX1, rX2, rX0)
	RET

TEXT addpt2_sse(SB), 1, $0
	MOVUPD a+8(FP), X0
	MOVUPD b+32(FP), X1
	ADDPD X1, X0
	MOVSD a+24(FP), X2
	ADDSD b+48(FP), X2
	MOVQ BP, DI
	MOVUPD X0, (DI)
	MOVSD X2, 16(DI)
	RET

/* TODO: write only 24 bytes */
TEXT addpt2_avx(SB), 1, $0
	MOVQ SP, AX
	ADDQ $16, AX
	VMOVUPD_256mr(rAX, rX0)
	ADDQ $24, AX
	VMOVUPD_256mr(rAX, rX1)
	VADDPD_256rr(rX1, rX0, rX0)
	MOVQ BP, DI
	VMOVUPD_256rm(rX0, rDI)
	VZEROUPPER
	RET

TEXT addpt3_avx(SB), 1, $0
	MOVQ SP, AX
	ADDQ $16, AX
	VMOVUPD_256mr(rAX, rX0)
	ADDQ $32, AX
	VMOVUPD_256mr(rAX, rX1)
	VADDPD_256rr(rX1, rX0, rX0)
	MOVQ BP, DI
	VMOVUPD_256rm(rX0, rDI)
	VZEROUPPER
	RET
