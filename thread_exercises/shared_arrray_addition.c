#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t sem;
int globalArray[10] = {0,0,0,0,0,0,0,0,0,0};

void* addition(){
  int* randomValue = calloc(sizeof(int), 1);
  if (randomValue == NULL){
    printf("Error reservando memoria: %d\n",errno);
    exit(EXIT_FAILURE);
  }
  for(int i=0;i <100;i++){
    *randomValue = *(randomValue) + rand()%100;
  }
  int randomPosition = rand()%10;
  pthread_mutex_lock(&sem);
  globalArray[randomPosition] += *randomValue;
  pthread_mutex_unlock(&sem);
  sleep(1);
  pthread_exit((void*) randomValue);
}
int main(int argc, char const *argv[]) {
  if (argc != 2){
    printf("Error, ./a.out NThreads \n");
    exit(EXIT_FAILURE);
  }
  int Nthreads = atoi(argv[1]);
  srand(time(NULL));
  pthread_mutex_init(&sem,0);
  pthread_t threads[Nthreads];
  void* returnedValues[Nthreads];
  for (int i=0;i<Nthreads;i++){
    pthread_create(&threads[i], NULL, addition, NULL);
  }
  for( int i=0;i<Nthreads;i++){
    pthread_join(threads[i], &returnedValues[i]);
  }
  int finalArray = 0;
  int finalReturned = 0;
  printf("Final Array: \n");
  for(int i = 0; i<10;i++){
    printf(" %d ", globalArray[i]);
    finalArray += globalArray[i];
    if (i < Nthreads){
      finalReturned = finalReturned + *((int*) returnedValues[i]);
    }
  }
  printf("\n");
  printf("Final Array: %d Final Returned: %d", finalArray, finalReturned);

  return 0;
}
