FILE1 = bmhasrch.c  bmhisrch.c  bmhsrch.c  pbmsrch_small.c 
FILE2 = bmhasrch.c  bmhisrch.c  bmhsrch.c  pbmsrch_large.c 

all: search_small search_large

search_small: ${FILE1} Makefile
	${CC} -static ${FILE1} ${OPT} -o ../../out/search_small.${TGT}

search_large: ${FILE2} Makefile
	${CC} -static ${FILE2} ${OPT} -o ../../out/search_large.${TGT}

clean:
	rm -rf search_small search_large output*
