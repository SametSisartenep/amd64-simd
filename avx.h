#define VEX_m_0F	(1)
#define VEX_m_0F38	(2)
#define VEX_m_0F3A	(3)
#define VEX_L_128	(0)
#define VEX_L_256	(1)
#define VEX_p_NO	(0)
#define VEX_p_66	(1)
#define VEX_p_F3	(2)
#define VEX_p_F2	(3)

#define VEX3(r, x, b, m, w, v, l, p)	BYTE $0xC4;				\
				BYTE $(((~r)<<7)|((~x)<<6)|((~b)<<5)|(m));	\
				BYTE $(((w)<<7)|((~v)<<3)|((l)<<2)|(p))
#define VEX2(r, b, l, p)	BYTE $0xC5;					\
			BYTE $(((~r)<<7)|((~v)<<3)|((l)<<2)|(p))
#define VOP(o, m, ro, rm)	BYTE $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define VOPi(o, m, ro, rm, i)	VOP((o), (m), (ro), (rm));	\
			BYTE $(i)


/* VZEROUPPER */
#define VZEROUPPER	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_NO); BYTE $0x77

/* VMOVAPD */
#define VMOVUPD_128mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);	\
				VOPi(0x10, 0x1, (d), (s), (off))
#define VMOVAPD_128rr(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);		\
			VOP(0x28, 0x3, (d), (s))
/* VDPPD */
#define VDPPD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F3A,0,(s0),VEX_L_128,VEX_p_66);	\
			VOPi(0x41, 0x3, (d), (s1), 0x31)

/* VFMADD231SD (128 bit) */
#define VFMADD231SD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);	\
			VOP(0xB9, 0x3, (d), (s1))

/* VFMADD231PD (128 bit) */
#define VFMADD231PD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);	\
			VOP(0xB8, 0x3, (d), (s1))
