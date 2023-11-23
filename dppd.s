#include "sse.h"

DATA one(SB)/8,$1.0
GLOBL one(SB), $8

TEXT dppd(SB), 1, $0
	MOVQ SP, AX
	MOVLPD(8, rAX, rX0)	/* MOVLPD a+0(FP), X0 */
	MOVHPD(16, rAX, rX0)	/* MOVHPD a+8(FP), X0 */
	MOVLPD(32, rAX, rX1)	/* MOVLPD b+0(FP), X1 */
	MOVHPD(40, rAX, rX1)	/* MOVHPD b+8(FP), X1*/
	DPPD(rX1, rX0)		/* DPPD $0x31, X1, X0 */
	RET

TEXT dppd3(SB), 1, $0
	MOVQ SP, AX
	MOVLPD(8, rAX, rX0)	/* MOVLPD a+0(FP), X0 */
	MOVHPD(16, rAX, rX0)	/* MOVHPD a+8(FP), X0 */
	MOVLPD(40, rAX, rX1)	/* MOVLPD b+0(FP), X1 */
	MOVHPD(48, rAX, rX1)	/* MOVHPD b+8(FP), X1 */
	DPPD(rX1, rX0)		/* DPPD $0x31, X1, X0 */
	MOVSD one(SB), X1
	MOVHPD(24, rAX, rX0)	/* MOVHPD a+16(FP), X0 */
	MOVHPD(56, rAX, rX1)	/* MOVHPD b+16(FP), X1 */
	DPPD(rX1, rX0)		/* DPPD $0x31, X1, X0 */
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
	MOVQ SP, AX
	MOVLPD(8, rAX, rX0)	/* MOVLPD a+0(FP), X0 */
	MOVHPD(16, rAX, rX0)	/* MOVHPD b+0(FP), X0 */
	HSUBPD(rX0, rX0)	/* HSUBPD X0, X0 */
	RET

TEXT xvec3(SB), 1, $0
	MOVQ SP, AX
	ADDQ $8, AX
	MOVLPD(40, rAX, rX0)	/* MOVLPD b+0(FP), X0 */
	MOVHPD(8, rAX, rX0)	/* MOVHPD a+0(FP), X0 */
	MOVLPD(16, rAX, rX1)	/* MOVLPD a+8(FP), X1 */
	MOVHPD(48, rAX, rX1)	/* MOVHPD b+8(FP), X1 */
	MOVLPD(56, rAX, rX2)	/* MOVLPD b+16(FP), X2 */
	MOVHPD(24, rAX, rX2)	/* MOVHPD a+16(FP), X2 */
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
