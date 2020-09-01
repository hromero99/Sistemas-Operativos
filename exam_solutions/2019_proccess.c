#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>

void addition(){
  pid_t child_pid;
  int status;
  while(child_pid = wait(&status) > 0){
    if (WIFEXITED(status)){
      printf("Proceso %d exited with status %d\n", getpid(), WEXITSTATUS(status));
    }
    if (WIFSTOPPED(status)){
      printf("Proceso %d stopped with signal %d\n", child_pid, WSTOPSIG(status));
    }
    if (WIFSIGNALED(status)){
      printf("Proceso %d signaled with signal %d\n", child_pid, WTERMSIG(status));
    }
  }
//  printf("Proceso %d \t parametro recibido %d \t estado del anterior %d\n", getpid(),WEXITSTATUS(status) + getpid()%10,WEXITSTATUS(status));
//  printf("Valor para retornar %d \n",WEXITSTATUS(status) + getpid()%10));
  exit(WEXITSTATUS(status) + getpid()%10);
}

int main(int argc, char const *argv[]) {
  if( argc != 2){
    printf("Usage: ./a.out Nproccess\n");
    exit(-1);
  }
  int Nproccess = atoi(argv[1]);
  pid_t child;
  pid_t child_pid;
  int status;
  for (int i=0;i<Nproccess; i++){
    child = fork();
    switch (child) {
      case -1:
        printf("Error creando el proceso hijo: %d\n", errno);
        exit(EXIT_FAILURE);
      case 0:
        printf("Soy el proceso %d y mi padre es %d\n", getpid(),getppid());
        sleep(3);
        break;
      default:
        addition();
    }
  }
  exit(getpid()%10);
}
