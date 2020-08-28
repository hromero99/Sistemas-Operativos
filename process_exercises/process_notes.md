# Notes

## Para esperar a que todos los procesos finalicen

```c
pid_t child_pid;
int status;

while(wait(status) > 0){
	//Control proccess exit
}

```
Wait suspende la ejecución del _thread_ que lo llama hasta que han terminado todos los hijos. 
