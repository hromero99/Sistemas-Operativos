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

## Para controlar la salida de los procesos

* WIFEXITED(wstatus): Retorna _true_ si el hijo termina correctamente.
	* WEXITSTATUS(wstatus): Retorna el código de salida el hijo.
* WIFSIGNALED(wstatus): Retorna _true_ si el hijo ha sido terminado por una señal.
	* WTERMSIG(wstatus): Retorna el número de la señal que ha caussado que el hijo termine.
* WIFSOTTED(wstatus): Retorna _true_ si el hijo ha sido parado por una señal.
	* WSTOPSIG(wstatus): Retorna el número de la señal que ha caussado que el hijo se pare.
