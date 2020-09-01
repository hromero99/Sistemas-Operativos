// calculate the scalar product of 2 vectors
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

//Which make a dinamic buffer to use as parameter
typedef struct {
  int* array1;
  int* array2;
  int size;
}buffer;

void *proccess(void* arguments_void){
  buffer* arguments = (buffer*) arguments_void;
  int* result = calloc(sizeof(int), 1);
  int localResult;
  if (result == NULL){
    fprintf(stderr,"Error reservando memoria: %d", errno);
    exit(-1);
  }
  for (int i=0;i< arguments->size;i++){
    printf("Operation to do: %d*%d \n", arguments->array1[i], arguments->array2[i]);
    localResult += (arguments->array1[i] * arguments->array2[i]);
  }
  *result = localResult;
  pthread_exit((void*) result);
}
int main(int argc, char **argv) {
  if (argc != 3){
    printf("[!]Error: ./a.out ArraySize Nthreads");
    exit(EXIT_FAILURE);
  }
  int arraySize = atoi(argv[1]);
  int nThreads = atoi(argv[2]);
  if (nThreads>arraySize){
    printf("[!]Error: nthreads < arraySize that's can't be");
    exit(EXIT_FAILURE);
  }
  //Generate random values for two vectors
  int vector1[arraySize];
  int vector2[arraySize];
  srand(time(NULL));
  printf("--Original Vectors--\n");
  for(int i=0;i<arraySize;i++){
    vector1[i] = rand()%100;
    vector2[i] = rand()%100;
    printf("%d %d\n", vector1[i],vector2[i]);
  }
  printf("------------\n");
  pthread_t threads[nThreads];
  // Make division to split main array
  int elementsPerThread = arraySize / nThreads;
  // Check if the division has rest, that elements should add on first or last threads
  int modPerThread = arraySize % nThreads;
  //Create Array of buffer to pass as argument to function
  buffer arguments[nThreads];
  void* returnedValues[nThreads];
  for (int i=0;i<nThreads;i++){
    buffer arguments_thread;
    arguments_thread.array1 = &vector1[i*elementsPerThread];
    arguments_thread.array2 = &vector2[i*elementsPerThread];
    arguments_thread.size = elementsPerThread;
    // if is final thread ex: counter 2 and 2 threads must consume the last elements of impar array
    if (i == nThreads - 1){
      arguments_thread.size = elementsPerThread + modPerThread;
    }
    arguments[i] = arguments_thread;
    pthread_create(&threads[i], NULL, proccess,(void*) &arguments[i]);
  }
  for (int i=0;i<nThreads;i++){
    pthread_join(threads[i], &returnedValues[i]);
  }
  //Make addition to calculate the final result in base of returned values
  int finalReturnedValue=0;
  for (int i=0;i<nThreads; i++){
    finalReturnedValue += *((int*) returnedValues[i]);
  }
  printf("-----\n");
  printf("Return value in base of returned values: %d\n", finalReturnedValue);
  printf("-----\n");
  //Calculate value without threads to check if is correct
  int calculateFinalValue = 0;
  for (int i=0; i<arraySize;i++){
    calculateFinalValue += (vector1[i]*vector2[i]);
  }
  printf("Return value in base of calculate values: %d\n", calculateFinalValue);
  printf("-----\n");
  return 0;
}
