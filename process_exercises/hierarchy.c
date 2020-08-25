#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <errno.h>

int main(int argc, char **argv) {
  if (argc != 2){
    printf("Error: You must give number of process\n");
    exit(-1);
  }
  int Nproccess = atoi(argv[1]);
  int status;
  pid_t pid;
  for (int i = 0; i < Nproccess; i++) {
    switch (fork()) {
      case -1:
        printf("Error creating proccess: %d\n", errno);
        return -1;
      case 0:
        printf("I'm %d, my father %d\n", getpid(), getppid());
        sleep(3);
        break;
      default:
        pid = wait(&status);
        if (pid > 0){
          if (WIFEXITED(status)){
            printf("Proccess %d exited with coide %d\n", pid,WEXITSTATUS(status));
          }
          return 0;
        }
        else{
          printf("Wait error %d\n", errno);
          return -1;
        }
      }
    }

  return 0;
}
