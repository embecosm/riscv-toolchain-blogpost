all: susan objects

susan: susan.c Makefile
	${CC} -static ${OPT} -o ../../out/susan.${TGT} susan.c -lm 
objects:
	${CC} ${OPT} susan.c -c -o ../../out/susan.o.${TGT} -lm

clean:
	rm -rf susan output*
