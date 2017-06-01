# md5_brutus

MD5 Hash generator and cracker by:
- Sergio Anguita @zerjioang
- Aitor Brazaola @kronosnhz

This is the project files of subject "Computación de altas prestaciones" of University of Deusto.
There are different programs inside of this project based on the technology used in them.
* Sequential based
* OpenMP

Under sequential approach are the following ones:
* Hasher (Based on existing algorithm from the net)
* Cracker (Developed by our own)

All programs have their own makefile to ease the process of compiling and running except the hasher.

# Cracker compilation guidelines
Current section shows compilation guidelines.
Follow the next instructions to compile the cracker
### TL;DR

#### Compilation

```
$ cd cracker\openmp
$ make
```

#### Test execution

```
$ cd cracker\openmp
$ make run
```
### Makefile
There is a makefile defined for easy compilation, but if you are curious, this is what the makefile is doing:

```
gcc -c crackeromp.c md5_hasher.c md5_hasher.h -fopenmp
gcc crackeromp.o md5_hasher.o -o md5_brutus -fopenmp
clear
```
# Usage

Since this version of the cracker is not using ```getOps()``` function is **MUST** follow the next param order:
````
 ./md5_brutus <min_length> <max_length> <dedicated_threads> <charset> <hash>
 ````
 Current charset options are:
 * numeric
 * numeric_space
 * alpha
 * alpha_space
 
 This charsets are defined as:
````
static const char numeric[] = "0123456789";
static const char numeric_space[] = "0123456789 ";
static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
static const char alpha_space[] = "abcdefghijklmnopqrstuvwxyz ";
````
# Execution example

## Sequential

Sequential with ```c20ad4d76fe97759aa27a0c99bff6710``` as example input and max size 2
```
./md5_brutus 1 2 numeric c20ad4d76fe97759aa27a0c99bff6710
	execution_path:	./md5_brutus
	min_size:	1
	max_size:	2
	charset:	numeric
	target:	c20ad4d76fe97759aa27a0c99bff6710
	Selected charset by user is numeric
	Selected charset is NUMERIC
		Key min is: 1
		Key max is: 2


	#################
	Password cracked:
	#################

	12
```

## OpenMP

OpenMP with ```bcfa52b9f7724713a01055f1ccdfbc9d``` as example input and max 

### Input
```
 time ./md5_brutus 5 5 1 alpha bcfa52b9f7724713a01055f1ccdfbc9d
```

 ### Output
```
	execution_path:	./md5_brutus
	min_size:	5
	max_size:	5
	threads:	1
	charset:	alpha
	target:	bcfa52b9f7724713a01055f1ccdfbc9d
	Dedicated threads to cracking:	1
		Bruteforce key space:	11881376


	#################
	Password cracked:
	#################


	Attepts: 9092599 	 of 	 11881376
	Password is: txipi


real	0m29.108s
user	0m25.760s
sys	    0m3.324s
```