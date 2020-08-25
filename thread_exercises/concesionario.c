/*
Simular un concesionario con N vehículos. C compradores y V vendedores
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t sem;


typedef struct{
  int* modelos;
  int size;
} bufferParams;


void *consumidoresAction(void *buffer_void){
    bufferParams* buffer = buffer_void;
    int modelo = rand()%(buffer->size);
    int cantidad = rand()%30;
    printf("Modelo %d decrementado %d\n", modelo,cantidad);
    pthread_mutex_lock(&sem);
    buffer->modelos[modelo] -=  cantidad;
    pthread_mutex_unlock(&sem);
    pthread_exit(NULL);
}

void *productoresAction(void *buffer_void){
    bufferParams* buffer = buffer_void;
    int modelo = rand()%(buffer->size);
    int cantidad = rand()%30;
    printf("Modelo %d incrementado %d\n", modelo,cantidad);
    pthread_mutex_lock(&sem);
    buffer->modelos[modelo] += cantidad;
    pthread_mutex_unlock(&sem);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
  if (argc != 3){
    printf("Error: ./a.out Productores consumidores \n");
    exit(EXIT_FAILURE);
  }
  // For generate random values
  srand(time(NULL));
  int productores = atoi(argv[1]);
  int consumidores = atoi(argv[2]);
  int buffer[atoi(argv[1])];
  // Create Arrays for threads
  pthread_t ThreadsProductores[productores];
  pthread_t ThreadsConsumidor[consumidores];
  //Fill buffer of random values
  printf("\n");
  for (int i = 0; i<productores; i++){
    buffer[i] = rand()%100;
    printf(" %d ",buffer[i]);
  }
  printf("\n");
  // Init semaphore
  pthread_mutex_init(&sem, 0);
  bufferParams ThreadParams;
  ThreadParams.modelos = buffer;
  ThreadParams.size = productores;
  // Thread Creation
  for(int Nthread = 0; Nthread <productores; Nthread++){
    pthread_create(&ThreadsProductores[Nthread], NULL, productoresAction, &ThreadParams);
  }
  for(int Nthread = 0; Nthread <consumidores; Nthread++){
    pthread_create(&ThreadsConsumidor[Nthread], NULL, consumidoresAction, &ThreadParams);
  }
  // Thread Execution
  for(int Nthread = 0; Nthread <productores; Nthread++){
    pthread_join(ThreadsProductores[Nthread], NULL);
  }
  for(int Nthread = 0; Nthread <consumidores; Nthread++){
    pthread_join(ThreadsConsumidor[Nthread], NULL);
  }
  printf("Final array: \n");
  for(int i = 0; i <productores; i++){
    printf(" %d ",  buffer[i]);
  }
  return 0;
}
