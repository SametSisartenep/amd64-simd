#define OP(o, m, ro, rm)	WORD $0x0F66; BYTE $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define OPi(o, m, ro, rm, i)	OP((o), (m), (ro), (rm));	\
			BYTE $(i)
#define OP4(o, m, ro, rm)	WORD $0x0F66; WORD $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define OP4i(o, m, ro, rm, i)	OP4((o), (m), (ro), (rm));	\
			BYTE $(i)
#define F3OP(o, m, ro, rm)	WORD $0x0FF3; BYTE $(o);	\
			BYTE $(((m)<<6)|((ro)<<3)|(rm))
#define F3OPi(o, m, ro, rm, i)	F3OP((o), (m), (ro), (rm));	\
			BYTE $(i)

/* MOVDQA */
#define MOVDQA_mr(off, s, d) OPi(0x6F, 0x1, (d), (s), (off))
#define MOVDQA_rm(off, s, d) OPi(0x7F, 0x1, (s), (d), (off))

/* MODQU */
#define MOVDQU_mr(off, s, d) F3OPi(0x6F, 0x1, (d), (s), (off))
#define MOVDQU_rm(off, s, d) F3OPi(0x7F, 0x1, (s), (d), (off))
