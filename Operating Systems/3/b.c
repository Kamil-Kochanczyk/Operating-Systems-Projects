/*
------------------------------------
Kamil Kochańczyk, Kraków, 29.03.2022
------------------------------------

(b) Uruchomić program z (a) poprzez funkcję exec w procesie potomnym innego procesu i wysyłać do niego sygnały poprzez funkcję systemową kill z procesu macierzystego. Przed wysłaniem sygnału sprawdzić, czy proces istnieje.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Program needs two numbers as console arguments\n");
		exit(1);
	}
	
	int sig = atoi(argv[1]);
	int id = fork();
	
	if (id == -1)
	{
		perror("fork error");
		exit(2);
	}
	else if (id == 0)
	{
		// Proces potomny uruchamia program a.c
		execlp("./a", "a", argv[1], argv[2], NULL);
		
		perror("execlp error");
		exit(3);
	}
	else
	{
		sleep(1);
		
		// Sprawdzanie czy proces, do którego ma być wysłany sygnał, w ogóle istnieje
		if (kill(id, 0) == -1)
		{
			perror("kill error");
			exit(4);
		}
		
		printf("Parent process (b.c) is sending signal to child process (a.c)...\n\n");
		
		// Wysyłanie sygnału z procesu macierzystego do procesu potomnego
		if (kill(id, sig) == -1)
		{
			perror("kill error");
			exit(4);
		}
	}
	
	return 0;
}
