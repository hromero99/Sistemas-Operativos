#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>

void Factorial(int number){
  int factorial = 1;
  for (int i=1; i<=number;i++){
    factorial = factorial * i;
  }
  printf("Soy el hijo %d, mi padre %d el factorial de %d es %d\n", getpid(), getppid(), number,factorial);
}
int main(int argc, char **argv) {
  pid_t pid, child_pid, pid2;
  int status;
  char* calculator = "gnome-calculator";
  char **arguments = NULL;
  if (argc != 2){
    printf("Error, must give me a number to calculate factorial\n");
    exit(EXIT_FAILURE);
  }
  switch (pid=fork()) {
    case -1:
      printf("Error creating proccess\n Errno value: %d",errno);
      exit(EXIT_FAILURE);
    case 0:
      //printf("Hello, mi pid is %d and my father is %d\n", getpid(),getppid());
      Factorial(atoi(argv[1]));
      exit(EXIT_SUCCESS);
  }
  switch (pid2=fork()) {
    case -1:
      printf("Error creating proccess\n Errno value: %d",errno);
      exit(EXIT_FAILURE);
    case 0:
      printf("Hello, mi pid is %d and my father is %d and I'm going to execture Gnome-Calculator\n", getpid(),getppid());
      execvp(calculator, arguments);
      exit(EXIT_SUCCESS);
  }
  for (int Nproccess = 0; Nproccess<2;Nproccess++){
    child_pid = wait(&status);
    if (child_pid > 0) {
      if WIFEXITED(status){
        printf("Proccess with pid %d exited with code %d\n",child_pid, status);
      }
      else if (WIFSIGNALED(status)){
      printf("El hijo %d ha sido eliminado mediante una señal %i\n", child_pid, WTERMSIG(status));
      }
      else if (WIFSTOPPED(status)){
          printf("El hijo %d ha sido eliminado mediante una señal\n", child_pid, WSTOPSIG(status));
      }
    }
  }
  return 0;
}
