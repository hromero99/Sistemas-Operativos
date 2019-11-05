#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global = 0;

void * add(){
  for (int i = 0 ; i<50; i++){
    global++;
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]){

  pthread_t *threads_array = calloc(argc - 1, sizeof(pthread_t));
  if (threads_array  == NULL){
      perror("Fallo al reservar memoria para el array de threads");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i<2; i++){
        pthread_create(&threads_array[i], NULL, (void*) add, NULL);
      }
  for (int i = 0; i<2; i++){
      pthread_join(threads_array[i], NULL);
    }

    printf("%d\n",global );

  return 0;
}
