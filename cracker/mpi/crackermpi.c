﻿#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#include "md5_hasher.h"

//#define DEBUG

#define SUCCESS_CODE 0
#define ERROR_CODE 2
#define NOT_CHARSET_VALID 127

#define MULTIPLE_LEN

#define NOT_CRACKED 0
#define PAWNED 1

#define KEEP_RUNNING 0
#define STOP 1

#define DEFAULT_THREAD_NUM 1
#define MASTER 0

//global constants
static const char numeric[] = "0123456789";
static const char numeric_space[] = "0123456789 ";
static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
static const char alpha_space[] = "abcdefghijklmnopqrstuvwxyz ";

//global variables for MPI
int numtasks, rank, len; 
MPI_Status Stat;

//for cracker
#define BOUNDS 2
int *ranges_sources;
int ranges_received[BOUNDS];
int is_the_job_done = KEEP_RUNNING;

void init_mpi(int argc, char **argv){
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

void showme(){
	#ifdef DEBUG
		printf("[Thread %d of %d]\n", rank, numtasks);
	#endif
}
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
    //showme();
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
    current_key_length++;
  }while(current_key_length <= max && !found);
  return current_key_length-1;
}
#endif

void calculate_ranges(int key_space){
  int pass_per_tasks = key_space / numtasks;
  int restante = key_space % numtasks;
  if(restante != 0){
    pass_per_tasks -= 1;
  }
  int reparto = pass_per_tasks;
  if(rank == MASTER){
      printf("\t\tInicializando rango de datos...\n");
      int len = BOUNDS * numtasks;
      ranges_sources = (int*) malloc(sizeof(int) * len);
      printf("\t\tCalculando offset de rango: %d...\n", pass_per_tasks);
      int i=0;
      for(i = 0; i < len; i = i+BOUNDS){
	//valor anterior + pass_per_tasks
        if(i==0){
   	  ranges_sources[i] = 0;
   	  ranges_sources[i+1] = pass_per_tasks - 1;
 	}
	else{
	  ranges_sources[i] = ranges_sources[i-1] + 1;
	  ranges_sources[i+1] = ranges_sources[i] + pass_per_tasks;
	}
      }
      //add remaining to last range
      printf("\t\tAñadiendo restante: %d al ultimo rango...\n", restante);
      ranges_sources[len-1] = key_space-1;
      printf("\t\tRango de calculo generado\n");
  }
}
int execute(int start_value, int min, int max, char* target, const char* charset){
  //generated from globals
  int charset_length = strlen(charset);

  //calculate key space
  long key_space = 0L;

  #ifdef MULTIPLE_LEN
    int i=0;
    for(i = min; i <= max; i++){
      long value = ipow(charset_length, i);
      key_space += value;
      #ifdef DEBUG
      showme();
      if(rank == MASTER){
        printf("\t\tSingle key space for length %d is:\t%ld\n", i, value);
      }
      #endif
    }
  #else
    key_space = ipow(charset_length, max);
  #endif
  if(rank == MASTER){
    printf("\t\tBruteforce key space:\t%ld\n", key_space);
  }

  //calculate computing ranges
  calculate_ranges(key_space);

  //send ranges to threads
  MPI_Scatter(ranges_sources, BOUNDS, MPI_INT, ranges_received, BOUNDS, MPI_INT, 0, MPI_COMM_WORLD);

  //#####################
  //PARALLEL REGION START
  //#####################
  for(i = 0; i < BOUNDS; i++){
    #ifdef DEBUG
	printf("Received range: %d to %d\n", ranges_received[0], ranges_received[1]);
    #endif
  }

  int iterate;
  int stop_condition = KEEP_RUNNING;
  int last_passw_len = min;
  int payload;
  char* found;
  int password_cracked = NOT_CRACKED;
  long attempts = 0L;

  //overwrite values with master thread info
  start_value = ranges_received[0];
  key_space = ranges_received[1];

  //start cracking until found
  for(iterate = start_value ; iterate <= key_space && !stop_condition; iterate++){
    if(stop_condition == KEEP_RUNNING) {
      #ifdef DEBUG
         printf("[Thread %d of %d] - %d %d \n", rank, numtasks, iterate, stop_condition);
        #endif
        int idx = iterate;
        #ifdef MULTIPLE_LEN
          //password length will be variable depending on given key and min max values.
          int password_length = estimateLength(idx, min, max, charset_length);
          if(password_length != last_passw_len){
            payload = ipow(charset_length, last_passw_len);
            idx = iterate - payload;
          }
        #endif
        //get key given an index
        char *key = getKeyIndex(idx, password_length, charset);
        char* hash = compute(key, password_length);
        #ifdef DEBUG
         printf("[Thread %d of %d] - Iteration: %d Key: %s MD5: %s\n", rank, numtasks, iterate, key, hash);
        #endif
        //compare it
        if(strcmp(hash, target)==0){
          found = key;
          #ifdef DEBUG
            printf("[Thread %d of %d] - FOUND THE PASSWD: %d Key: %s MD5: %s\n", rank, numtasks, iterate, key, hash);
          #endif
          //release hash
          free(hash);
          hash = NULL;
          //pass cracked. stop
          password_cracked = PAWNED;
	  printf("Password is: %s\n", key);
          stop_condition = STOP;
        }

      //increase attempt count
      attempts = attempts + 1;

      //check if current thread found passwd or any other thread found it
      if(password_cracked == PAWNED || stop_condition == STOP){
        stop_condition = STOP;
      }
    }
  }//end for

  //###################
  //PARALLEL REGION END
  //###################

  MPI_Reduce(&stop_condition, &is_the_job_done, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == MASTER){
      printf("\n\n\t Job done: %d\t Pawned: %d\n", is_the_job_done, stop_condition);
    //this code is executed by master thread only
    if(is_the_job_done == PAWNED && stop_condition == STOP){
      printf("\n\n");
      printf("\t#################\n");
      printf("\tPassword cracked:\n");
      printf("\t#################\n\n" );
      printf("\n\tAttepts: %li \t of \t %li\n", attempts, key_space);
      printf("\tPassword is: %s\n\n", found);
      //release found key
      free(found);
      found = NULL;
    }
    else{
      printf("\n\n\tNO HASH FOUND. SORRY :(\n\n");
    }
  }  

  MPI_Finalize();
  return SUCCESS_CODE;
}

int brute_force(int start_value, char* min, char* max, char* hash, char* charset_name)
{
  //convert los char* to int
  int max_len = atoi(max);
  int min_len = atoi(min);

  if(min_len > max_len){
    printf("\tPlease specify a min value smaller or equal than max value\n");
    return ERROR_CODE;
  }

  if(strcmp(charset_name, "numeric")==0){
    return execute(start_value, min_len, max_len,hash, numeric);
  }
  else if(strcmp(charset_name, "numeric_space")==0){
    return execute(start_value, min_len, max_len, hash, numeric_space);
  }
  else if(strcmp(charset_name, "alpha")==0){
    return execute(start_value, min_len, max_len, hash, alpha);
  }
  else if(strcmp(charset_name, "alpha_space")==0){
    return execute(start_value, min_len, max_len, hash, alpha_space);
  }
  return NOT_CHARSET_VALID;
}

int main(int argc, char **argv)
{

  char* execution_path;
  char* min_size;
  char* max_size;
  char* charset;
  char* hash;
  int start_value;

  init_mpi(argc, argv);

  showme();
  
  //sustituir esto por getops
  execution_path = argv[0];
  min_size = argv[1];
  max_size = argv[2];
  charset = argv[3];
  hash = argv[4];
  start_value = 0;

  if( rank == MASTER ){
    printf("\texecution_path:\t%s\n", execution_path);
    printf("\tmin_size:\t%s\n", min_size);
    printf("\tmax_size:\t%s\n", max_size);
    printf("\tcharset:\t%s\n", charset);
    printf("\ttarget:\t%s\n", hash);
  }
  
  return brute_force(start_value, min_size, max_size, hash, charset);
}