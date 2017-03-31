//comando para ejecutar
// clear; gcc cracker.c -o cracker; ./cracker 2 2 numeric

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "md5_hasher.h"

#define SUCCESS_CODE 1
#define ERROR_CODE 2
#define NOT_CHARSET_VALID 127
#define DEBUG
#define PRINT_OUTPUT

#define MULTIPLE_LEN

//global constants
static const char numeric[] = "0123456789";
static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";


int ipow(int source, int exp)
{
    long base = source;
    long result = 1.0L;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

char* getKeyIndex(int key_index, int key_size, const char *charset)
{
	//get memory for key
	char *key;
	key = (char*) malloc( sizeof(char) * (key_size) );

	//get charset length
	const int charset_length = strlen(charset);

	//calculate a key given an index
        int divisionNumber;
        for(divisionNumber = 0; divisionNumber < key_size; divisionNumber++){
		key[key_size-divisionNumber-1] = charset[key_index%charset_length];
		key_index = key_index/charset_length;
        }
	return key;
}

void printKey(char *key, int password_length, int charset_length)
{
	int i;
	for(i = 0; i < password_length; i++){
	    printf("%c", key[i]);
	}
	printf("\n");
}

#ifdef MULTIPLE_LEN
int estimateLength(int value, int min, int max, int charset_length){
	int key_size;
	int current_key_length = min;
	bool found = false; 
	do{
		key_space = ipow(charset_length, current_key_length);
		found = value;
		current_key_length++;
	}while(current_key_length <= max && !found);
	return current_key_length-1;
}
#endif

int execute(int start_value, int min, int max, const char* charset)
{
	//generated from globals
	int charset_length = strlen(charset);

	//calculate key space
	long key_space = 0L;
	int i=0;

	printf("\t\tKey min is: %d\n", min);
	printf("\t\tKey max is: %d\n", max);

	#ifdef MULTIPLE_LEN
		for(i = min; i <= max; i++){
		    //peta con INTEGER OVERFLOW
		    long value = ipow(charset_length, i);
		    key_space += value;
		    #ifdef DEBUG
		    	printf("\t\tSingle key space for length %d is:\t%ld\n", i, value);
		    #endif
		}
	#endif

	#ifndef MULTIPLE_LEN
		key_space = ipow(charset_length, max);
	#endif

	#ifdef DEBUG
		printf("\t\tFinal key space is:\t%ld\n", key_space);
	#endif

	int password_length;
	#ifndef MULTIPLE_LEN
		//hook para mantener la consistencia de un size fijo de clave
		password_length = max;
	#endif

	//key index
	int idx;

	//generate keyspace
	for(idx = start_value; idx < key_space; idx++){
		#ifdef MULTIPLE_LEN
			//password length will be variable depending on given key and min max values.
			password_length = estimateLength(start_value, min, max, charset_length);
		#endif
		//get key given an index
		char *key = getKeyIndex(idx, password_length, charset);
		//debug print
		#ifdef PRINT_OUTPUT
			//calculate the hash of given key
			char* hash = compute(key, password_length);
			//print it
			printf("Key: %s\t MD5: %s\n", key, hash);
		#endif
		//release hash
		free(hash);
		hash = NULL;
		//release key
		free(key);
		key = NULL;
	}
	return SUCCESS_CODE;
}

int brute_force(int start_value, char* min, char* max, char* charset_name)
{
    //convert los char* to int
    int max_len = atoi(max);
    int min_len = atoi(min);

    printf("\tSelected charset by user is %s\n", charset_name);

    if(strcmp(charset_name, "numeric")==0){
        printf("\tSelected charset is NUMERIC\n");
        return execute(start_value, min_len, max_len, numeric);
    }
    else if(strcmp(charset_name, "alpha")==0){
        printf("\tSelected charset is ALPHA\n");
        return execute(start_value, min_len, max_len, alpha);
    }
    return NOT_CHARSET_VALID;
}

int main(int argc, char **argv)
{
    //sustituir esto por getops
    char* execution_path = argv[0];
    printf("\texecution_path:\t%s\n", execution_path);
    char* min_size = argv[1];
    printf("\tmin_size:\t%s\n", min_size);
    char* max_size = argv[2];
    printf("\tmax_size:\t%s\n", max_size);
    char* charset = argv[3];
    printf("\tcharset:\t%s\n", charset);
    int start_value = 0;
    return brute_force(start_value, min_size, max_size, charset);
}