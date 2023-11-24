#define rAX	0
#define rCX	1
#define rDX	2
#define rBX	3
#define rSP	4
#define rBP	5
#define rSI	6
#define rDI	7

#define rX0	0
#define rX1	1
#define rX2	2
#define rX3	3
#define rX4	4
#define rX5	5
#define rX6	6

#define VEX_m_0F	(1)
#define VEX_m_0F38	(2)
#define VEX_m_0F3A	(3)
#define VEX_L_128	(0)
#define VEX_L_256	(1)
#define VEX_p_NO	(0)
#define VEX_p_66	(1)
#define VEX_p_F3	(2)
#define VEX_p_F2	(3)

#define OP(o, m, ro, rm)	WORD $0x0F66; BYTE $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define OPi(o, m, ro, rm, i)	OP((o), (m), (ro), (rm));	\
			BYTE $(i)
#define OP4(o, m, ro, rm)	WORD $0x0F66; WORD $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define OP4i(o, m, ro, rm, i)	OP4((o), (m), (ro), (rm));	\
			BYTE $(i)

#define VEX3(r, x, b, m, w, v, l, p)	BYTE $0xC4;				\
				BYTE $(((~r)<<7)|((~x)<<6)|((~b)<<5)|(m));	\
				BYTE $(((w)<<7)|((~v)<<3)|((l)<<2)|(p))
#define VEX2(r, b, l, p)	BYTE $0xC5;					\
			BYTE $(((~r)<<7)|((~v)<<3)|((l)<<2)|(p))
#define VOP(o, m, ro, rm)	BYTE $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define VOPi(o, m, ro, rm, i)	VOP((o), (m), (ro), (rm));	\
			BYTE $(i)

/* MOVLPD */
//opcode = 660F12
//modrm  = 01 000 000 [AX → X0] / 01 001 000 [AX → X1]
//disp8 = 8 / 32
#define MOVLPD(off, s, d) OPi(0x12, 0x1, (d), (s), (off))

/* MOVHPD */
//opcode = 660F16
//modrm  = 01 000 000 [AX → X0] / 01 001 000 [AX → X1]
//disp8 = 16 / 40
#define MOVHPD(off, s, d) OPi(0x16, 0x1, (d), (s), (off))

/* HSUBPD */
//opcode = 660F7D = 01100110 00001111 01111101
//modrm = 11 000 000 [X0 → X0]
#define HSUBPD(s, d) OP(0x7D, 0x3, (d), (s))

/* DPPD */
//opcode = 660F3A41 = 01100110 00001111 00111010 01000001
//modrm  = 11 000 001 [X1 → X0]
//imm8   = 0011 0001
#define DPPD(s, d) OP4i(0x413A, 0x3, (d), (s), 0x31)

/* VMOVAPD */
#define VMOVUPD_128mr(off, s, d) VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);			\
			VOPi(0x10, 0x1, (d), (s), (off))
#define VMOVAPD_128rr(s, d) VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);			\
			VOP(0x28, 0x3, (d), (s))
/* VDPPD */
#define VDPPD(s0, s1, d) VEX3(0,0,0,VEX_m_0F3A,0,(s0),VEX_L_128,VEX_p_66);		\
			VOPi(0x41, 0x3, (d), (s1), 0x31)

/* VFMADD231SD (128 bit) */
#define VFMADD231SD(s0, s1, d) VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);	\
			VOP(0xB9, 0x3, (d), (s1))

/* VFMADD231PD (128 bit) */
#define VFMADD231PD(s0, s1, d) VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);	\
			VOP(0xB8, 0x3, (d), (s1))
