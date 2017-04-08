# md5_brutus
MD5 Hash generator and cracker by:
- Sergio Anguita @zerjioang
- Aitor Brazaola @kronosnhz

This is the project files of subject "Computación de altas prestaciones" of
University of Deusto.

There are different programs inside of this project based on the technology
* Sequential based
* OpenMP

Under sequential approach are the following ones:
* Hasher (Based on existing algorithm from the net)
* Cracker (Developed by our own)

In OpenMP currently is only the hasher.

All programs have their own makefile to ease the process of compiling and
running except the hasher.

# Compilation
## Hasher
These are the steps needed to compile
```
gcc -c cracker.c md5_hasher.c md5_hasher.h
gcc cracker.o md5_hasher.o -o md5_brutus
clear
```
# Usage
## Generate all pin code for length 1
### Input
```
./md5_brutus 1 1 numeric
```
### Output
```
	execution_path:	./md5_brutus
	min_size:	1
	max_size:	1
	charset:	numeric
	Selected charset by user is numeric
	Selected charset is NUMERIC
		Key min is: 1
		Key max is: 1
		Final key space is:	10

0
cfcd208495d565ef66e7dff9f98764da
1
c4ca4238a0b923820dcc509a6f75849b
2
c81e728d9d4c2f636f067f89cc14862c
3
eccbc87e4b5ce2fe28308fd9f2a7baf3
4
a87ff679a2f3e71d9181a67b7542122c
5
e4da3b7fbbce2345d7772b0674a318d5
6
1679091c5a880faf6fb5e6087eb1b2dc
7
8f14e45fceea167a5a36dedd4bea2543
8
c9f0f895fb98ab9159f51fd0297e236d
9
45c48cce2e2d7fbdea1afc51c7c6ad26
```

## Cracker
Go to cracker's directory (sequential or OpenMP)
```
make
make run
```

For clean the directory
```
make clean
```

### Output
Sequential with ```c20ad4d76fe97759aa27a0c99bff6710``` as example input and max
size 2
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

OpenMP with ```c20ad4d76fe97759aa27a0c99bff6710``` as example input and max
size 2
```
./md5_brutus 1 3 4 numeric_space c20ad4d76fe97759aa27a0c99bff6710
	execution_path:	./md5_brutus
	min_size:	1
	max_size:	3
	threads:	4
	charset:	numeric_space
	target:	c20ad4d76fe97759aa27a0c99bff6710
	Dedicated threads to cracking:	4
[Thread 0 of 4] - 0 0
[Thread 0 of 4] - Iteration: 0 Key: 0 MD5: cfcd208495d565ef66e7dff9f98764da
[Thread 0 of 4] - 1 0
[Thread 0 of 4] - Iteration: 1 Key: 1 MD5: c4ca4238a0b923820dcc509a6f75849b
[Thread 0 of 4] - 2 0
[Thread 0 of 4] - Iteration: 2 Key: 2 MD5: c81e728d9d4c2f636f067f89cc14862c
[Thread 0 of 4] - 3 0
[Thread 0 of 4] - Iteration: 3 Key: 3 MD5: eccbc87e4b5ce2fe28308fd9f2a7baf3
[Thread 0 of 4] - 4 0
[Thread 0 of 4] - Iteration: 4 Key: 4 MD5: a87ff679a2f3e71d9181a67b7542122c
[Thread 0 of 4] - 5 0
[Thread 0 of 4] - Iteration: 5 Key: 5 MD5: e4da3b7fbbce2345d7772b0674a318d5
[Thread 0 of 4] - 6 0
[Thread 0 of 4] - Iteration: 6 Key: 6 MD5: 1679091c5a880faf6fb5e6087eb1b2dc
[Thread 0 of 4] - 7 0
[Thread 0 of 4] - Iteration: 7 Key: 7 MD5: 8f14e45fceea167a5a36dedd4bea2543
[Thread 0 of 4] - 8 0
[Thread 0 of 4] - Iteration: 8 Key: 8 MD5: c9f0f895fb98ab9159f51fd0297e236d
[Thread 0 of 4] - 9 0
[Thread 0 of 4] - Iteration: 9 Key: 9 MD5: 45c48cce2e2d7fbdea1afc51c7c6ad26
[Thread 0 of 4] - 10 0
[Thread 0 of 4] - Iteration: 10 Key:   MD5: 7215ee9c7d9dc229d2921a40e899ec5f
[Thread 0 of 4] - 11 0
[Thread 0 of 4] - Iteration: 11 Key: 00 MD5: b4b147bc522828731f1a016bfa72c073
[Thread 0 of 4] - 12 0
[Thread 0 of 4] - Iteration: 12 Key: 01 MD5: 96a3be3cf272e017046d1b2674a52bd3
[Thread 0 of 4] - 13 0
[Thread 0 of 4] - Iteration: 13 Key: 02 MD5: a2ef406e2c2351e0b9e80029c909242d
[Thread 0 of 4] - 14 0
[Thread 0 of 4] - Iteration: 14 Key: 03 MD5: e45ee7ce7e88149af8dd32b27f9512ce
[Thread 0 of 4] - 15 0
[Thread 1 of 4] - 366 0
[Thread 2 of 4] - 732 0
[Thread 0 of 4] - Iteration: 15 Key: 04 MD5: 7d0665438e81d8eceb98c1e31fca80c1
[Thread 0 of 4] - 16 0
[Thread 0 of 4] - Iteration: 16 Key: 05 MD5: 751d31dd6b56b26b29dac2c0e1839e34
[Thread 0 of 4] - 17 0
[Thread 0 of 4] - Iteration: 17 Key: 06 MD5: faeac4e1eef307c2ab7b0a3821e6c667
[Thread 0 of 4] - 18 0
[Thread 0 of 4] - Iteration: 18 Key: 07 MD5: d72d187df41e10ea7d9fcdc7f5909205
[Thread 0 of 4] - 19 0
[Thread 0 of 4] - Iteration: 19 Key: 08 MD5: fad6f4e614a212e80c67249a666d2b09
[Thread 0 of 4] - 20 0
[Thread 0 of 4] - Iteration: 20 Key: 09 MD5: 0a8005f5594bd67041f88c6196192646
[Thread 0 of 4] - 21 0
[Thread 0 of 4] - Iteration: 21 Key: 0  MD5: 2e5751b7cfd7f053cd29e946fb2649a4
[Thread 0 of 4] - 22 0
[Thread 0 of 4] - Iteration: 22 Key: 10 MD5: d3d9446802a44259755d38e6d163e820
[Thread 0 of 4] - 23 0
[Thread 0 of 4] - Iteration: 23 Key: 11 MD5: 6512bd43d9caa6e02c990b0a82652dca
[Thread 0 of 4] - 24 0
[Thread 0 of 4] - Iteration: 24 Key: 12 MD5: c20ad4d76fe97759aa27a0c99bff6710
[Thread 0 of 4] - FOUND THE PASSWD: 24 Key: 12 MD5: c20ad4d76fe97759aa27a0c99bff6710
[Thread 3 of 4] - 1098 0
[Thread 2 of 4] - Iteration: 732 Key: 5 6 MD5: 5a16b726336026b8ba4850301fb23ecb
[Thread 1 of 4] - Iteration: 366 Key: 2 3 MD5: a2d9b33304a1222854cafaf96187e00d
[Thread 3 of 4] - Iteration: 1098 Key: 8 9 MD5: 6ca096d268c20e3c3bd29ff17196c57c


	#################
	Password cracked:
	#################


	Attepts: 28 	 of 	 1463
	Password is: 12
```
