HEADERS = md5_hasher.h

default: program

program:
	gcc -c cracker.c md5_hasher.c md5_hasher.h
	gcc cracker.o md5_hasher.o -o md5_brutus

run:
	./md5_brutus 1 2 numeric c20ad4d76fe97759aa27a0c99bff6710

clean:
	-rm -f cracker.o
	-rm -f cracker
