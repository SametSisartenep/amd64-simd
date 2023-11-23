TEXT min(SB), 1, $0
	MOVSD a+0(FP), X0
	MINSD b+8(FP), X0
	RET
