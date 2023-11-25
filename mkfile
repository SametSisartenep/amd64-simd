</$objtype/mkfile

BIN=/$objtype/bin
TARG=stuff
OFILES=\
	main.$O\
	min.$O\
	dppd.$O\

HFILES=\
	regs.h\
	sse.h\
	avx.h\

</sys/src/cmd/mkone

pulldeps:VQ:
	git/clone git://shithub.us/sigrid/bench9 || \
	git/clone https://git.sr.ht/~ft/bench9
