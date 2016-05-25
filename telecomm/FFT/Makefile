OBJ = main.o fftmisc.o  fourierf.o
FILE = main.c fftmisc.c  fourierf.c

all: fft objects

fft:
	${CC} ${OPT} -static ${FILE} -o ../../out/fft.${TGT} -lm

objects:
	${CC} ${OPT} main.c -c -o ../../out/fftmain.o.${TGT} -lm
	${CC} ${OPT} fftmisc.c -c -o ../../out/fftmisc.o.${TGT} -lm
	${CC} ${OPT} fourierf.c -c -o ../../out/fourierf.o.${TGT} -lm

clean:
	rm -rf *.o fft output*
