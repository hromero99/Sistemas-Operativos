#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
int par = 0, impar = 0;
pthread_mutex_t semPar, semImpar;

void *makeAddition (void* threadId){
  int* additionValue = calloc(sizeof(int), 1);
  int* identifier = (int*) threadId;
  if (additionValue == NULL){
    printf("Error while reserving memory");
    exit(-1);
  }
  int randomValue;
  for (int i=0; i< 100; i++){
    randomValue = rand()%100;
    *additionValue = *additionValue + randomValue;
  }
  if ( (*identifier) % 2){
    printf("Add %d to Par\n", *additionValue);
    pthread_mutex_lock(&semPar);
    par = par + *additionValue;
    pthread_mutex_unlock(&semPar);
  }
  else {
    printf("Add %d to Impar\n", *additionValue);
    pthread_mutex_lock(&semImpar);
    impar = impar + *additionValue;
    pthread_mutex_unlock(&semImpar);
  }
  pthread_exit((void*) additionValue);
}
int main(int argc, char const *argv[]) {
  if (argc != 2){
    printf("Error, you must give me the number of threads\n");
    exit(-1);
  }
  // Initialization of Semaphores for global vars
  pthread_mutex_init(&semPar, 0);
  pthread_mutex_init(&semImpar, 0);
  // Initialization of time for random number generation
  srand(time(NULL));
  // Creation of threads array and return values array
  int Nthreads = atoi(argv[1]);
  pthread_t threads[Nthreads];
  void* returnedValues[Nthreads];
  int threadIdentifier[Nthreads];
  for(int i = 0;i<Nthreads;i++){
    printf("ThreadIdentifier: %d\n", i);
    threadIdentifier[i] = i+1;
    pthread_create(&threads[i], NULL, makeAddition, (void*)&threadIdentifier[i]);
  }
  for(int i=0;i<Nthreads;i++){
    pthread_join(threads[i],&returnedValues[i]);
  }
  int totalAddition = 0;
  for(int i=0;i<Nthreads;i++){
    totalAddition += *((int*) returnedValues[i]);
  }
  printf("Total Addition of returned values %d",totalAddition);
  printf("Total Addition of par + impar: %d + %d = %d", par, impar, par+impar);
  return 0;
}
