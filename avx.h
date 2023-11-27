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

/* VZEROALL */
#define VZEROALL	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_NO); BYTE $0x77

/* VMOVUPD */
#define VMOVUPD_128mr(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);	\
				VOP(0x10, 0x0, (d), (s))
#define VMOVUPD_128rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);		\
			VOP(0x11, 0x0, (s), (d))
#define VMOVUPD_256mr(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);	\
				VOP(0x10, 0x0, (d), (s))
#define VMOVUPD_256rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);		\
			VOP(0x11, 0x0, (s), (d))

/* VMOVAPD */
#define VMOVAPD_128mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);	\
				VOPi(0x28, 0x1, (d), (s), (off))
#define VMOVAPD_128rr(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);		\
			VOP(0x28, 0x3, (d), (s))
#define VMOVAPD_256mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);	\
				VOPi(0x28, 0x1, (d), (s), (off))
#define VMOVAPD_256rr(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);		\
			VOP(0x28, 0x3, (d), (s))

/* VMOVDQA */
#define VMOVDQA_128mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);	\
				VOPi(0x6F, 0x1, (d), (s), (off))
#define VMOVDQA_128rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_66);		\
			VOP(0x7F, 0x3, (s), (d))
#define VMOVDQA_256mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);	\
				VOPi(0x6F, 0x1, (d), (s), (off))
#define VMOVDQA_256rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_66);		\
			VOP(0x7F, 0x3, (s), (d))

/* VMODQU */
#define VMOVDQU_128mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_F3);	\
				VOPi(0x6F, 0x1, (d), (s), (off))
#define VMOVDQU_128rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_128,VEX_p_F3);		\
			VOP(0x7F, 0x3, (s), (d))
#define VMOVDQU_256mr(off, s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_F3);	\
				VOPi(0x6F, 0x1, (d), (s), (off))
#define VMOVDQU_256rm(s, d)	VEX3(0,0,0,VEX_m_0F,0,0,VEX_L_256,VEX_p_F3);		\
			VOP(0x7F, 0x3, (s), (d))

/* VADDPD */
#define VADDPD_128mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOPi(0x58, 0x1, (d), (s1), (off))
#define VADDPD_128rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);		\
				VOP(0x58, 0x3, (d), (s1))
#define VADDPD_256mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOPi(0x58, 0x1, (d), (s1), (off))
#define VADDPD_256rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);		\
				VOP(0x58, 0x3, (d), (s1))

/* VSUBPD */
#define VSUBPD_128mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOPi(0x5C, 0x1, (d), (s1), (off))
#define VSUBPD_128rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);		\
				VOP(0x5C, 0x3, (d), (s1))
#define VSUBPD_256mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOPi(0x5C, 0x1, (d), (s1), (off))
#define VSUBPD_256rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);		\
				VOP(0x5C, 0x3, (d), (s1))

/* VHADDPD */
#define VHADDPD_128mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOPi(0x7C, 0x1, (d), (s1), (off))
#define VHADDPD_128rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOP(0x7C, 0x3, (d), (s1))
#define VHADDPD_256mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOPi(0x7C, 0x1, (d), (s1), (off))
#define VHADDPD_256rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOP(0x7C, 0x3, (d), (s1))

/* VHSUBPD */
#define VHSUBPD_128mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOPi(0x7D, 0x1, (d), (s1), (off))
#define VHSUBPD_128rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_128,VEX_p_66);	\
				VOP(0x7D, 0x3, (d), (s1))
#define VHSUBPD_256mr(off, s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOPi(0x7D, 0x1, (d), (s1), (off))
#define VHSUBPD_256rr(s0, s1, d)	VEX3(0,0,0,VEX_m_0F,0,(s0),VEX_L_256,VEX_p_66);	\
				VOP(0x7D, 0x3, (d), (s1))

/* VDPPD */
#define VDPPD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F3A,0,(s0),VEX_L_128,VEX_p_66);	\
			VOPi(0x41, 0x3, (d), (s1), 0x31)

/* VFMADD231SD (128 bit) */
#define VFMADD231SD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);		\
			VOP(0xB9, 0x3, (d), (s1))
/* VFMADD231SD (256 bit) */
#define VFMADD231SD_256(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_256,VEX_p_66);	\
			VOP(0xB9, 0x3, (d), (s1))

/* VFMADD231PD (128 bit) */
#define VFMADD231PD(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_128,VEX_p_66);		\
			VOP(0xB8, 0x3, (d), (s1))
/* VFMADD231PD (256 bit) */
#define VFMADD231PD_256(s0, s1, d)	VEX3(0,0,0,VEX_m_0F38,1,(s0),VEX_L_256,VEX_p_66);	\
			VOP(0xB8, 0x3, (d), (s1))

/* VINSERTF128 */
#define VINSERTF128(i, s0, s1, d)	VEX3(0,0,0,VEX_m_0F3A,0,(s0),VEX_L_256,VEX_p_66);	\
			VOPi(0x18, 0x3, (d), (s1), (i))
