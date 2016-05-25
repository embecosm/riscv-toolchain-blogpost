all:  dijkstra_large dijkstra_small objects

dijkstra_large: dijkstra_large.c Makefile
	${CC} -static dijkstra_large.c ${OPT} -o ../../out/dijkstra_large.${TGT}

dijkstra_small: dijkstra_small.c Makefile
	${CC} -static dijkstra_small.c ${OPT} -o ../../out/dijkstra_small.${TGT}
objects:
	${CC} ${OPT} dijkstra_small.c -c -o ../../out/dijkstra_small.o.${TGT} -lm
	${CC} ${OPT} dijkstra_large.c -c -o ../../out/dijkstra_large.o.${TGT} -lm

clean:
	rm -rf dijkstra_large dijkstra_small output*
