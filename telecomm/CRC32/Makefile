all: crc objects

crc: crc_32.c Makefile
	${CC} -static crc_32.c ${OPT} -o ../../out/crc.${TGT}

objects:
	${CC} ${OPT} crc_32.c -c -o ../../out/crc_32.o.${TGT} -lm

clean:
	rm -rf crc output*
