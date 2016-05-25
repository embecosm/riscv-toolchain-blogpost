FILE1 = qsort_small.c
FILE2 = qsort_large.c

all: qsort_small qsort_large objects

qsort_small: qsort_small.c Makefile
	${CC} -static qsort_small.c ${OPT} -o ../../out/qsort_small.${TGT} -lm
qsort_large: qsort_large.c Makefile
	${CC} -static qsort_large.c ${OPT} -o ../../out/qsort_large.${TGT} -lm
objects:
	${CC} ${OPT} qsort_small.c -c -o ../../out/qsort_small.o.${TGT} -lm
	${CC} ${OPT} qsort_large.c -c -o ../../out/qsort_large.o.${TGT} -lm
clean:
	rm -rf qsort_small qsort_large output*
