HEADERS = md5_hasher.h

default: program

program.o: cracker.c $(HEADERS)
	gcc -c cracker.c md5_hasher.c

program: program.o
	gcc cracker.o md5_hasher.o -o md5_brutus

run:
	./md5_brutus 1 1 numeric

clean:
	-rm -f *.o
	-rm -f *.c~
	-rm -f *.h~
	-rm -f *.gch
	-rm -f cracker
