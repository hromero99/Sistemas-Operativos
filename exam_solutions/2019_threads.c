#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

//Common buffer
int buffer[10] = {0,0,0,0,0,0,0,0,0,0};
pthread_mutex_t sem;

void *addToArray(){
  int* value = calloc(sizeof(int), 1);
  int position = rand()%10;
  if (value == NULL) {
    printf("error reservando memoria: %d\n", errno);
    exit(EXIT_FAILURE);
  }
  for (int i=0; i<100; i++){
    *value = *value + rand()%100+1;
  }
  pthread_mutex_lock(&sem);
  buffer[position] = *value;
  pthread_mutex_unlock(&sem);
  sleep(1);
  pthread_exit((void*) value);
}

int main(int argc, char const *argv[]) {
  if (argc != 2){
    printf("Error: ./a.out NThreads \n");
    exit(EXIT_FAILURE);
  }
  int nThreads = atoi(argv[1]);
  pthread_t threads[nThreads];
  void* returnedValues[nThreads];
  srand(time(NULL));
  for (int i=0;i<nThreads;i++){
    if (pthread_create(&threads[i], NULL ,addToArray, NULL) != 0){
      printf("Error creando el thread numero %d, %d\n", i, errno);
    }
  }
  for (int i=0;i<nThreads;i++){
    if (pthread_join(threads[i],&returnedValues[i]) != 0){
      printf("Error uniendose al thread numero %d, %d\n", i, errno);
    }
  }
  int returnedAddition = 0;
  int bufferAddition = 0;
  printf("--Final Buffer--");
  for (int i = 0; i<10;i++){
    bufferAddition += buffer[i];
    if (i < nThreads){
      returnedAddition = returnedAddition + *((int*) returnedValues[i]);
    }
    printf(" %d ", buffer[i]);
  }
  printf("\n");
  printf("BufferAddition %d\t returnedAddition: %d\n", bufferAddition, returnedAddition);

  return 0;
}
