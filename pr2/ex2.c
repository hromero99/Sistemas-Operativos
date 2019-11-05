#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


void * countLines(void * filename_arg){
  int * number_lines = calloc(1, sizeof(int));
  char * filename = filename_arg;
  printf("%p\n",filename_arg );
  FILE * file_stream = fopen(filename, "r");
  if (file_stream == NULL){
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }

  while(!feof(file_stream)){
    char ch = fgetc(file_stream);
    if (ch == '\n'){
      *number_lines = *number_lines + 1;
    }
  }
  printf("%i\n",*number_lines );
  pthread_exit( (void *) number_lines);
}


int main(int argc, char* argv[]){
  int total_lines = 0;

  pthread_t *threads_array = calloc(argc - 1, sizeof(pthread_t));
  if (threads_array  == NULL){
    perror("Fallo al reservar memoria para el array de threads");
    exit(EXIT_FAILURE);
  }

  int ** lines_array = calloc(argc -1  , sizeof(int*));
  if (lines_array == NULL){
    perror("Fallo al reservar memoria para el array de lineas");
    exit(EXIT_FAILURE);
  }


  for (int i = 1; i<argc; i++){
    pthread_create(&threads_array[i], NULL, (void*) countLines, (void*)argv[i]);
  }

  for (int i = 0; i<argc; i++){
    pthread_join(threads_array[i], (void **) &lines_array[i]);
  }

  for (int i = 1; i < argc ; i++){
      total_lines = total_lines + *lines_array[i];
    }
  printf("Numero total de lineas: %d\n",total_lines );
}
