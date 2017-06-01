HEADERS = md5_hasher.h

default: program

program.o: crackeromp.c $(HEADERS)
	clear
	gcc -c crackeromp.c md5_hasher.c -fopenmp

program: program.o
	gcc crackeromp.o md5_hasher.o -o md5_brutus -fopenmp

run:
	time ./md5_brutus 5 5 4 alpha bcfa52b9f7724713a01055f1ccdfbc9d

clean:
	-rm -f *.o
