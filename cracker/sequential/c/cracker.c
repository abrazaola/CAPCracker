//comando para ejecutar
// clear; gcc cracker.c -o cracker; ./cracker 2 2 numeric

#include <string.h>
#include <stdio.h>

#define SUCCESS_CODE 1
#define ERROR_CODE 2
#define NOT_CHARSET_VALID 127
#define DEBUG
//#define PRINT_OUTPUT

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

int execute(int start_value, int min, int max, const char* charset)
{
	//generated from globals
	int charset_length = strlen(charset);

	//calculate key space
	long key_space = 0L;
	int i=0;

	printf("\t\tKey min is: %d\n", min);
	printf("\t\tKey max is: %d\n", max);

	key_space = ipow(charset_length, max);
	
	#ifdef MULTIPLE_LEN
		for(i = min; i <= max; i++){
		    //cuidado con el INTEGER OVERFLOW
		    long value = ipow(charset_length, i);
		    key_space += value;
		    #ifdef DEBUG
		    	printf("\t\tSingle key space for length %d is:\t%ld\n", i, value);
		    #endif
		}
	#endif

	#ifdef DEBUG
		printf("\t\tFinal key space is:\t%ld\n", key_space);
	#endif

	return ERROR_CODE;

	int idx;
	//hook para mantener la consistencia de un size fijo de clave
	int password_length = max;
	int idxArr[password_length];

	//initialize array with zeros
	for(idx=0; idx < password_length; idx++){
		idxArr[idx] = 0;
	}

	//generate keyspace
	for(idx=start_value; idx < key_space; idx++){
        int base = idx;
        int divisionNumber;
        for(divisionNumber = 0; divisionNumber < password_length; divisionNumber++){
            idxArr[password_length-divisionNumber-1] = base%charset_length;
            base = base/charset_length;
        }

	//debug
	#ifdef PRINT_OUTPUT 
		int i;
		for(i = 0; i < password_length; i++){
		    printf("%c", charset[idxArr[i]]);
		}
		printf("\n");
	#endif
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
