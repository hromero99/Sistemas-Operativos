#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
//Global var to share between threads
int par = 0;
int impar = 0;
pthread_mutex_t mutex;

void* process(void* process_index){
  int* value = (int*)process_index;
  int* randomValue = calloc(sizeof(int), 1);
  printf("Thread: %d\n", *value);
  if (randomValue == NULL){
    perror("error reservando memoria\n");
    printf("errno value =%d\n", errno);
  }
  pthread_mutex_lock(&mutex);
  *randomValue = rand()%10+1;
  if ( (*value % 2) == 0 ){
    par += *randomValue;
    printf("par + %d\n",*randomValue);
  }
  else{
    impar += *randomValue;
    printf("impar + %d\n",*randomValue);
  }
  pthread_mutex_unlock(&mutex);
  usleep(10);
  pthread_exit((void*) randomValue);
}

int main(int argc, char **argv) {
  if (argc != 2){
    perror("Error: ./a.out Nthreads");
    exit(EXIT_FAILURE);
  }
  pthread_mutex_init(&mutex, 0);
  int Nthreads = atoi(argv[1]);
  int resultAddition = 0;
  pthread_t threads[Nthreads];
  srand(time(NULL));
  void *returnValues[Nthreads];
  int threadsId[Nthreads];
  for (int i =0; i<Nthreads; i++){
    threadsId[i] = i;
    pthread_create(&threads[i], NULL , process, (void*) &threadsId[i]);
  }
  for (int i =0; i<Nthreads; i++){
    pthread_join(threads[i], &returnValues[i]);
  }
  for (int i =0; i<Nthreads; i++){
    resultAddition += *((int*)returnValues[i]);
  }
  printf("par %d \t impar %d\n", par,impar);
  printf("Result of addition: %d\n", resultAddition);
  return 0;
}
