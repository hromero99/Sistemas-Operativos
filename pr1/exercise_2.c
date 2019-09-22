#include <sys/types.h> //Para tipo pid_t 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Para fork()
#include <errno.h>
#include <sys/wait.h>

int main(void){
    pid_t pid, child_pid;
    int status;

    pid = fork();
    if (pid == -1){
        perror("Error al crear el hijo");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        printf("Este es un proceso huerfano");
    }
    else{
        sleep(20);
        child_pid = wait(&status);
          
        if (child_pid > 0){
            if (WIFEXITED(status)){
                printf("child %d killed (signal %d)\n", child_pid, WTERMSIG(status));
            }
            else if (WIFSIGNALED(status)){
                printf("child %d killed (signal %d)\n", child_pid, WTERMSIG(status));
            } 
            else if (WIFSTOPPED(status)) {
                printf("child %d stopped (signal %d)\n", child_pid, WSTOPSIG(status));
            } 
        }
        else {
            printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
            exit(EXIT_FAILURE);
        } 
    }
    exit(EXIT_SUCCESS);
}