FILE1 = basicmath_small.c   rad2deg.c  cubic.c   isqrt.c
FILE2 = basicmath_large.c   rad2deg.c  cubic.c   isqrt.c

all: basicmath_small basicmath_large objects

basicmath_small: ${FILE1} Makefile
	${CC} -static ${OPT} ${FILE1} -o ../../out/basicmath_small.${TGT} -lm
basicmath_large: ${FILE2} Makefile
	${CC} -static ${OPT} ${FILE2} -o ../../out/basicmath_large.${TGT} -lm
objects:
	${CC} ${OPT} basicmath_small.c -c -o ../../out/basicmath_small.o.${TGT} -lm
	${CC} ${OPT} basicmath_large.c -c -o ../../out/basicmath_large.o.${TGT} -lm
	${CC} ${OPT} rad2deg.c -c -o ../../out/rad2deg.o.${TGT} -lm
	${CC} ${OPT} cubic.c -c -o ../../out/cubic.o.${TGT} -lm
	${CC} ${OPT} isqrt.c -c -o ../../out/isqrt.o.${TGT} -lm

clean:
	rm -rf basicmath_small basicmath_large output*
