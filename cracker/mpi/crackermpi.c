#include <string.h>
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
int tasks_to_do;
MPI_Status Stat;

//for demo
#define DEMO_I 5
int i, recibido[DEMO_I], total;
int a[DEMO_I];
int sumatorio = 0;

void init_mpi(int argc, char **argv){
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

void showme(){
      printf("[Thread %d of %d]\t (%d)\n", rank, numtasks, len);
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

int execute(int start_value, int min, int max, char* target, const char* charset){
  //generated from globals
  int charset_length = strlen(charset);

  //calculate key space
  long key_space = 0L;

  //this code is only executed by master
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

  if(rank == MASTER){
      printf("\t\tInicializando datos...\n");
      for(i = 0; i < DEMO_I; i++){
          a[i] = i + 1;
      }
      printf("\t\tArray de datos inicializado\n");
  }

  int pass_per_tasks = key_space / numtasks;
  int restante = key_space % numtasks;
  int reparto = DEMO_I/numtasks;
  MPI_Scatter(a, reparto, MPI_INT, recibido, reparto, MPI_INT, 0, MPI_COMM_WORLD);

  for(i = 0; i < reparto; i++){
      sumatorio += recibido[i];
  }

  //printf("Soy el proceso %d de %d, (%d)\n", rank, numtasks, len);
  //printf("Mi sumatorio es: %d\n", sumatorio);
  MPI_Reduce(&sumatorio, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  //procesar lo que se ha quedado sin repartir por master
  if(rank == MASTER){
    if(restante > 0){
      //procesar las combinaciones que quedan  
      for(i = DEMO_I - reparto; i < DEMO_I; i++){
        total += a[i];
      }
      printf("El sumatorio total vale:\t%d\n", total);
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
  char* cores;
  char* charset;
  char* hash;
  int start_value;

  init_mpi(argc, argv);

  showme();
  
  //sustituir esto por getops
  execution_path = argv[0];
  min_size = argv[1];
  max_size = argv[2];
  cores = argv[3];
  charset = argv[4];
  hash = argv[5];
  start_value = 0;

  if( rank == MASTER ){
    printf("\texecution_path:\t%s\n", execution_path);
    printf("\tmin_size:\t%s\n", min_size);
    printf("\tmax_size:\t%s\n", max_size);
    printf("\tthreads:\t%s\n", cores);
    printf("\tcharset:\t%s\n", charset);
    printf("\ttarget:\t%s\n", hash);
  }
  
  return brute_force(start_value, min_size, max_size, hash, charset);
}