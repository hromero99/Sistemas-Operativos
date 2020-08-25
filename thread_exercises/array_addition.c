#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

// Define struct to pass intermediate arrays to threads
typedef struct {
  int* array1;
  int* array2;
  int size;
}params;

void *escalar(void* Tparams_void){
  params* Tparams = Tparams_void;
  int *returnValue = calloc(sizeof(int), 1);
  if (returnValue == NULL) {
    printf("Error reservando memoria %d\n", errno);
    exit(EXIT_FAILURE);
  }
  printf("elements in thread; Size: %d\n", Tparams -> size);
  for(int i=0;i<Tparams -> size;i++){
    printf("v1: %d v2: %d\t",Tparams->array1[i],Tparams->array2[i]);
    *returnValue = Tparams->array1[i]*Tparams->array2[i];
  }
  printf("\n");
  pthread_exit((void*) returnValue);
}

int main(int argc, char const *argv[]) {
  if (argc != 3){
    printf("Error: ./a.out Nthreads VectorSize");
    exit(EXIT_FAILURE);
  }
  int Nthreads =atoi(argv[1]);
  int vectorSize = atoi(argv[2]);
  int finalResult = 0;
  if (vectorSize < Nthreads){
    printf("Error: No puede habre mas threads que elementos");
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));
  int v1[vectorSize], v2[vectorSize];
  for(int i=0; i<vectorSize;i++){
    v1[i] = rand()%100;
    v2[i] = rand()%100;
    printf("%d %d\n", v1[i], v2[i]);
  }
  void *returnedValues[Nthreads];
  pthread_t threads[Nthreads];
  int vecIndex = 0;
  int ElementsPerThread = vectorSize / Nthreads;
  // Check if mod != 0, then last arrays will be greather
  int mod = vectorSize%Nthreads;
  params threadsParams[Nthreads];
  for(int i = 0; i<Nthreads;i++){
    params tparams;
    tparams.array1 = &v1[vecIndex];
    tparams.array2 = &v2[vecIndex];
    tparams.size = ElementsPerThread;
    if ( (mod != 0) && (i == Nthreads-1) ){
      tparams.size = (ElementsPerThread + mod) * i;
    }
    threadsParams[i] = tparams;

    // Assign different size to last thread
    vecIndex = vecIndex + ElementsPerThread;
    pthread_create(&threads[i], NULL, escalar, (void*)&threadsParams[i]);
  }
  for(int i= 0; i<Nthreads;i++){
    pthread_join(threads[i],&returnedValues[i]);
  }
  printf("Final vector:\n");
  for(int i= 0; i<Nthreads;i++){
    printf(" %d ", *((int*) returnedValues[i]));
    finalResult = finalResult +  *((int*) returnedValues[i]);
  }
  printf("\n");
  printf("Final Result: %d\n",finalResult);
  return 0;
}
