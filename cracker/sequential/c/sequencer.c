#include <string.h>
#include <stdio.h>
#include <math.h>

#define SUCCESS_CODE 1
#define ERROR_CODE 127

//global constants
const char charset[] = "0123456789";
const int password_length = 4;

//main method
int main(int argc, char const *argv[])
{
    //generated from globals
    int charset_length = strlen(charset);

	double key_space = pow(charset_length, password_length);
	int idx;
	int idxArr[password_length];

	//initialize array with zeros
	for(idx=0; idx < password_length; idx++){
		idxArr[idx] = 0;
	}
    
	//generate keyspace
	for(idx=0; idx < key_space; idx++){
        int base = idx;
        int divisionNumber;
        for(divisionNumber = 0; divisionNumber < password_length; divisionNumber++){
            idxArr[password_length-divisionNumber-1] = base%charset_length;
            base = base/charset_length;
        }
        for(idx = 0; idx < password_length; idx++){
            printf("%c", charset[idxArr[idx]]);
        }
        printf("\n");
    }
	return SUCCESS_CODE;
}