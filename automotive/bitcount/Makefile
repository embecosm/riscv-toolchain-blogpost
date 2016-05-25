FILE = bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c bitfiles.c bitstrng.c bstr_i.c 

all: bitcnts objects

bitcnts: ${FILE} Makefile
	${CC} -static ${FILE} ${OPT} -o ../../out/bitcnts.${TGT}
objects:
	${CC} ${OPT} bitcnt_1.c -c -o ../../out/bitcnt_1.o.${TGT} -lm
	${CC} ${OPT} bitcnt_2.c -c -o ../../out/bitcnt_2.o.${TGT} -lm
	${CC} ${OPT} bitcnt_3.c -c -o ../../out/bitcnt_3.o.${TGT} -lm
	${CC} ${OPT} bitcnt_4.c -c -o ../../out/bitcnt_4.o.${TGT} -lm
	${CC} ${OPT} bitcnts.c -c -o ../../out/bitcnts.o.${TGT} -lm
	${CC} ${OPT} bitfiles.c -c -o ../../out/bitfiles.o.${TGT} -lm
	${CC} ${OPT} bitstrng.c -c -o ../../out/bitstring.o.${TGT} -lm
	${CC} ${OPT} bstr_i.c -c -o ../../out/bstr_i.o.${TGT} -lm

clean:
	rm -rf bitcnts output*
