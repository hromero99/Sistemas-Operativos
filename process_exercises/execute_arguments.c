// program to execute gnome-calculator and gedit with all files that are given by arguments
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_program(char* program, char** arguments){
  pid_t child;
  switch (child = fork()){
    case -1:
      printf("Error al crear el hijo Errno value :%d\n", errno);
      exit(EXIT_FAILURE);
    case 0:
      printf("Soy el proceso %d, con padre %d\n", getpid(),getppid());
      if ( execvp(program, arguments) < 0 ){
        printf("Error en la ejecucion del programa %s, errno %d\n", program,errno);
        exit(EXIT_FAILURE);
      }
      exit(0);
  }
}

int main(int argc, char **argv) {
  if (argc <= 2){
    printf("Error: ./a.out calculadora gedit file1 file2");
    exit(-1);
  }
  int status;
  pid_t child_pid;
  char** calculatorArguments = NULL;
  execute_program(argv[1],calculatorArguments);
  execute_program(argv[2], &argv[2]);
  for (int i=0;i<2;i++){
    child_pid = wait(&status);
    if (child_pid > 0){
      if (WIFEXITED(status)){
        printf("Proceso %d finalizado con codigo %d\n", child_pid, WEXITSTATUS(status));
      }
      if(WIFSIGNALED(status)){
        printf("Proceso %d ha recibido la señal %d", child_pid, WTERMSIG(status));
      }
      if(WIFSTOPPED(status)){
        printf("Proceso %d ha recibido la señal %d", child_pid, WSTOPSIG(status));

      }
    }
    else{
      printf("Error invocando wait: %d\n", errno);
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}
