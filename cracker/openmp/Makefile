HEADERS = md5_hasher.h

default: program

program.o: crackeromp.c $(HEADERS)
	clear
	gcc -c crackeromp.c md5_hasher.c -fopenmp

program: program.o
	gcc crackeromp.o md5_hasher.o -o md5_brutus -fopenmp

run:
	./md5_brutus 1 3 4 numeric_space c20ad4d76fe97759aa27a0c99bff6710

clean:
	-rm -f *.o
