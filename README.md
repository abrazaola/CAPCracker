# md5_brutus
MD5 Hash generator by:
- @zerjioang
- Aitor Brazaola


This is the project files of subject "Computación de altas prestaciones" of University of Deusto.

# Compilation
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

The program also prints the rotations made in the hashing process.
