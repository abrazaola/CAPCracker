HEADERS = md5_hasher.h

default: program

program:
	gcc -c crackeromp.c md5_hasher.c md5_hasher.h
	gcc crackeromp.o md5_hasher.o -o md5_brutus -fopenmp

run:
	export OMP_NUM_THREADS=2
	./md5_brutus 1 2 numeric c20ad4d76fe97759aa27a0c99bff6710

clean:
	-rm -f crackeromp.o
	-rm -f crackeromp
