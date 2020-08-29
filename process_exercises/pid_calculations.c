#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void addition(int parameter){
  int status;
  pid_t pid;
  int returnNumber;
  returnNumber = getpid()%10;
  while (pid = wait(&status)>0){
    if (WIFEXITED(status)){
        returnNumber = returnNumber + WEXITSTATUS(status);
      }
    printf("ITERACION: %d. PID: %d. PID PADRE: %d. Valor recibido: %d\n",parameter, getpid(), getppid(), WEXITSTATUS(status));
    printf("Returned Value: %d\n",returnNumber);
    exit(returnNumber);
  }
}
int main(int argc, char const *argv[]) {
  if (argc != 2){
    printf("Error, you must give me Number of process");
    exit(EXIT_FAILURE);
  }
  int Nproccess = atoi(argv[1]);
  pid_t pid;

  for (int proccess = 0; proccess < Nproccess; proccess++) {
    switch (fork()) {
      case -1:
        printf("Error creating proccess: %d\n", errno);
        exit(EXIT_FAILURE);
      case 0:
        //sleep(2);
        printf("I'm %d and my father: %d\n",getpid(),getppid());
        break;
      default:
        addition(proccess);
        break;
    }
  }
  return 0;
}
