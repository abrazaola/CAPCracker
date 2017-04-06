//comando para ejecutar
// clear; gcc cracker.c -o cracker; ./cracker 2 2 numeric

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "md5_hasher.h"

#define SUCCESS_CODE 0
#define ERROR_CODE 2
#define NOT_CHARSET_VALID 127
//#define DEBUG
#define PRINT_OUTPUT

#define MULTIPLE_LEN

#define NOT_CRACKED 0
#define PAWNED 1

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
  #ifdef DEBUG
  printf("Retrieving key %d of size %d\n", key_index, key_size);
  #endif
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
  int current_key_length = min;
  int found = 0;
  do{
    long key_space = ipow(charset_length, current_key_length);
    found = value < key_space;
    #ifdef DEBUG
    printf("possible length=%d, key_space=%li, key_value=%d, valid=%d\n", current_key_length, key_space, value, found);
    #endif
    current_key_length++;
  }while(current_key_length <= max && !found);
  return current_key_length-1;
}
#endif

int execute(int start_value, int min, int max, char* target, const char* charset)
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
  int iterate;

  //generate keyspace
  int last_passw_len = min;
  int payload = 0;

  //temp variable to hold result
  char* found;

  //stop condition
  int password_cracked = NOT_CRACKED;

  //init while start cracking point
  iterate = start_value;

  //start cracking until found
  while(iterate < key_space && !password_cracked){
    int idx = iterate;
    #ifdef MULTIPLE_LEN
    //password length will be variable depending on given key and min max values.
    int password_length = estimateLength(idx, min, max, charset_length);
    #endif
    if(password_length != last_passw_len){
      payload = ipow(charset_length, last_passw_len);
      idx = iterate - payload;
    }
    //get key given an index
    char *key = getKeyIndex(idx, password_length, charset);
    //calculate the hash of given key
    char* hash = compute(key, password_length);
    #ifdef DEBUG
    //print it
    printf("Iteration: %d Key: %s MD5: %s\n", iterate, key, hash);
    #endif
    //compare it
    if(strcmp(hash, target)==0){
      found = key;
      //release hash
      free(hash);
      hash = NULL;
      //pass cracked. stop
      password_cracked = PAWNED;
    }
    //try next
    iterate++;
  }
  //show result
  if(password_cracked == PAWNED){
    printf("\n\n\t#################\n");
    printf("\tPassword cracked:\n");
    printf("\t#################\n\n");
    printf("\t%s\n\n", found);
    //release found key
    free(found);
    found = NULL;
  }
  else{
    printf("\n\n\tNO HASH FOUND. SORRY :(\n\n");
  }
  return SUCCESS_CODE;
}

int brute_force(int start_value, char* min, char* max, char* hash, char* charset_name)
{
  //convert los char* to int
  int max_len = atoi(max);
  int min_len = atoi(min);

  printf("\tSelected charset by user is %s\n", charset_name);

  if(strcmp(charset_name, "numeric")==0){
    printf("\tSelected charset is NUMERIC\n");
    return execute(start_value, min_len, max_len,hash, numeric);
  }
  else if(strcmp(charset_name, "alpha")==0){
    printf("\tSelected charset is ALPHA\n");
    return execute(start_value, min_len, max_len, hash, alpha);
  }
  return NOT_CHARSET_VALID;
}

int main(int argc, char **argv)
{
  char* execution_path = argv[0];
  printf("\texecution_path:\t%s\n", execution_path);
  char* min_size = argv[1];
  printf("\tmin_size:\t%s\n", min_size);
  char* max_size = argv[2];
  printf("\tmax_size:\t%s\n", max_size);
  char* charset = argv[3];
  printf("\tcharset:\t%s\n", charset);
  char* hash = argv[4];
  printf("\ttarget:\t%s\n", hash);
  int start_value = 0;
  return brute_force(start_value, min_size, max_size, hash, charset);
}
