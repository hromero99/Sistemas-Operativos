#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

typedef struct{
  int * modelos;
  int size;
} buffer;

pthread_mutex_t sem;

void* consume (void* params){
  buffer * params_buffer = (buffer*)params;
  int modelo = rand()%params_buffer->size;
  int value = rand()%10;
  printf("D: buffer[%d] %d\n", modelo,value);
  pthread_mutex_lock(&sem);
  params_buffer->modelos[modelo] =   params_buffer->modelos[modelo] - value;
  pthread_mutex_unlock(&sem);
  pthread_exit(NULL);
}

void* provide (void* params){
  buffer * params_buffer = (buffer*)params;
  int modelo = rand()%params_buffer->size;
  int value = rand()%10;
  printf("I: buffer[%d] %d\n", modelo,value);
  pthread_mutex_lock(&sem);
  params_buffer->modelos[modelo] =   params_buffer->modelos[modelo] + value;
  pthread_mutex_unlock(&sem);
  pthread_exit(NULL);
}

int main(int argc, char**argv){
  if (argc != 3){
    printf("Error: ./a.out proveedores consumidores\n");
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));
  pthread_mutex_init(&sem,0);
  int nProveedores = atoi(argv[1]);
  int nConsumidores = atoi(argv[2]);
  //Create empty buffer
  int *bufferEmpty = calloc(sizeof(int), nProveedores);
  if (bufferEmpty == NULL){
    printf("Fallo al reservar memoria\n");
    exit(EXIT_FAILURE);
  }
  buffer sharedBuffer;
  sharedBuffer.modelos = bufferEmpty;
  sharedBuffer.size =nProveedores;

  pthread_t consumidores[nConsumidores];
  pthread_t proveedores[nProveedores];

  for (int i= 0; i< nProveedores;i++){
    if (pthread_create(&proveedores[i], NULL, provide, (void*)&sharedBuffer) != 0){
      printf("Error creando procesos de proveedores\n");
    }
  }
  for (int i= 0; i< nConsumidores;i++){
    if (pthread_create(&consumidores[i], NULL, consume, (void*) &sharedBuffer) != 0){
      printf("Error creando procesos de proveedores\n");
    }
  }
  for (int i= 0; i< nProveedores;i++){
    if (pthread_join(proveedores[i], NULL) != 0){
      printf("Error creando procesos de proveedores\n");
    }
  }
  for (int i= 0; i< nConsumidores;i++){
    if (pthread_join(consumidores[i], NULL) != 0){
      printf("Error creando procesos de proveedores\n");
    }
  }
  printf("\n -- Final Buffer -- \n");
  for (int i= 0; i< nProveedores;i++){
    printf("%d\t", sharedBuffer.modelos[i]);
  }
  return 0;
}
