#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define N 4

void* partial_addition(){
    float *fn =  calloc(1, sizeof(float));
    if (fn == NULL) {
      perror("Error al reservar memoria dentro de la suma parcial");
      exit(EXIT_FAILURE);
    }
    *fn =  (float)rand() + (float)rand();
    printf("Contenido en un hijo: %f\n", *fn);
    pthread_exit( (void *) fn);
}

int main(int argc, char *argv[]){
    float final_addition = 0.0;

    //Memory allocation for all threads
    pthread_t *threads_array = calloc(N, sizeof(float));
    if (threads_array == NULL){
        perror("Error while making array of threads");
        exit(EXIT_FAILURE);
    }

    //Memory allocation to save partial results
    float **partial_array = calloc(N, sizeof(float*));

    if (partial_array == NULL){
        perror("Error allocate memory for float vectoir");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++){
        pthread_create(&threads_array[i],NULL,(void *) partial_addition, NULL);
    }

    for (int i = 0; i < N; i++){
        pthread_join(threads_array[i], (void **)&partial_array[i]);

    }

    for (int i = 0; i < N; i++){
      final_addition = final_addition + *partial_array[i];
    }

    printf("%f\n", final_addition);
    printf("Han finalizados los hilos\n");

    free(partial_array);
    free(threads_array);
    exit(EXIT_SUCCESS);
}
