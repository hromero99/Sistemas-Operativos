/*
* Simple program to calculate the factorial of a number given by command line
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if (argc != 2){
        perror("Usage: ./factorial <numero>");
        exit(EXIT_FAILURE);
    }
    int numberToCalculate = atoi(argv[1]);
    int result = 1;
    printf("Calculando factorial de %s\n", argv[1]);
    for (int i = 1; i<=numberToCalculate; i++){
        result = result * i;
        printf("%d\n",result);
        sleep(1);
    }
    printf("Resultado factorial: %d\n", result);
    return result;
}