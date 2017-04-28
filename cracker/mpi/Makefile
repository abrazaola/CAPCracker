HEADERS = md5_hasher.h

default: program

program.o: crackermpi.c $(HEADERS)
	clear
	mpicc -c crackermpi.c md5_hasher.c

program: program.o
	mpicc crackermpi.o md5_hasher.o -o md5_brutus

run:
	time mpirun -np 4 ./md5_brutus 5 5 alpha bcfa52b9f7724713a01055f1ccdfbc9d

demo:
	time mpirun -np 4 ./md5_brutus 1 4 alpha fe01ce2a7fbac8fafaed7c982a04e229

clean:
	-rm -f *.o
	rm md5_brutus
