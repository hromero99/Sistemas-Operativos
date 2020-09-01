#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
void proccess( int i, int factorial){
  char* const calculatorArguments = NULL;
  int finalFactorial = 1;
  if (i == 0){
    for (int i=1;i<=factorial;i++){
      finalFactorial = finalFactorial * i;
    }
    printf("Soy el hijo %d, mi padre es %d y el factorial de %d es %d \n", getpid(),getppid(), factorial, finalFactorial);
  }
  else if (i == 1){
    printf("Soy el hijo %d mi padre es %d y ejecuto la calculadora \n", getpid(),getppid());
    execvp("gnome-calculator",&calculatorArguments);
  }
}
int main(int argc, char **argv) {
  if (argc != 2){
    printf("You must give me a number for factorial\n");
    exit(EXIT_FAILURE);
  }
  pid_t pid, child_pid;
  int status;
  int factorial = atoi(argv[1]);
  int finalFactorial = 1;
  for (int i=0;i<2;i++){
    pid = fork();
    switch (pid) {
      case -1:
        printf("Error creating proccess %d\n", errno);
        exit(EXIT_FAILURE);
      case 0:
        // make a function to separate execution from control code
        proccess(i,factorial);
        exit(EXIT_SUCCESS);
    }
  }
  for (int i=0;i<2;i++){
    child_pid = wait(&status);
    if ( child_pid> 0){
      if WIFEXITED(status){
        printf("Child %d exited with code %d\n", child_pid, WEXITSTATUS(status));
      }
      if WIFSTOPPED(status){
        printf("Child %d stopped with code %d\n", child_pid, WSTOPSIG(status));
      }
      if WIFSIGNALED(status){
        printf("Child %d signaled with code %d\n", child_pid, WTERMSIG(status));
      }
    }
    else{
      printf("Error while using Wait %d \n", errno);
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
