#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
  int* array1;
  int* array2;
  int size;
} buffer;

void * escalar(void* params_void){
  buffer* params = (buffer*) params_void;
  int* escalarResult = calloc(sizeof(int), 1);
  if (escalarResult == NULL){
    printf("Error allocating memory %d", errno);
    exit(EXIT_FAILURE);
  }
  printf("\n");
  printf("Array 1 in thread: ");
  for (int i = 0; i< params-> size;i++){
    printf("%d ", params->array1[i]);
  }
  printf("\n");
  printf("Array 2 in thread: ");
  for (int i = 0; i< params-> size;i++){
    printf("%d ", params->array2[i]);
  }
  for (int i = 0; i< params-> size;i++){
    *escalarResult = *escalarResult + (params->array1[i] * params->array2[i]);
  }
  printf("\nResult :%d \n", *escalarResult);
  pthread_exit((void*)escalarResult);
}
int main(int argc, char const *argv[]) {
  if (argc != 3){
    printf("Usage: ./a.out ArraySize NThreads\n");
    exit(EXIT_FAILURE);
  }
  int array_size = atoi(argv[1]);
  int nThreads = atoi(argv[2]);
  if (array_size < nThreads){
    printf("Error, el tamaño del array tiene que ser menor que el numero de threads");
    exit(EXIT_FAILURE);
  }
  int vector1[array_size], vector2[array_size];
  pthread_t threads[nThreads];
  buffer threadsArguments[nThreads];
  void *returnedValues[nThreads];
  // fill buffer to random value
  srand(time(NULL));
  for (int i=0;i<array_size;i++){
    vector1[i] = rand()%100;
    vector2[i] = rand()%100;
  }
  printf("-- Original Vectors --\n");

  for (int i=0;i<array_size;i++){
    printf(" %d ", vector1[i]);
  }
  printf("\n");
  for (int i=0;i<array_size;i++){
    printf(" %d ", vector2[i]);
  }
  printf("\n");
  // Calculate elements for each thread
  int elementsPerThread = array_size / nThreads;
  int elementsRest = array_size %nThreads;
  for (int i=0; i<nThreads; i++){
    buffer tempBuffer;
    tempBuffer.array1 = &vector1[i*elementsPerThread];
    tempBuffer.array2 = &vector2[i*elementsPerThread];
    tempBuffer.size = elementsPerThread;
    // If is the las thread and there are some elements, must handle
    if ( (i == nThreads -1) && (elementsRest != 0) ){
      tempBuffer.size = elementsPerThread + elementsRest;
    }
    threadsArguments[i] = tempBuffer;
    if (pthread_create(&threads[i], NULL,escalar, (void*) &threadsArguments[i] ) != 0){
      printf("Error while creating thread %d\n", errno);
    }
  }
  for (int i=0; i<nThreads;i++){
    pthread_join(threads[i], &returnedValues[i]);
  }
  int escalarReturned = 0;
  int escalarCalculated =0;
  for (int i=0; i<nThreads;i++){
      escalarReturned += *((int*) returnedValues[i]);
  }
  for (int i=0; i<array_size;i++){
      escalarCalculated += (vector1[i] * vector2[i]);
  }
  printf("ValueReturned %d \n ValueCalculated %d\n", escalarReturned, escalarCalculated);
  return 0;
}
