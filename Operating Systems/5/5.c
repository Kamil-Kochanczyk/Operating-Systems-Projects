/*
Kamil Kochańczyk, Kraków, 24.04.2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Ścieżka do potoku FIFO
#define PATH "/tmp/Path"

// Funkcja do obsługi sygnału SIGINT
void signal_handler(int signal_number);

// Funkcja usuwająca potok FIFO
void end(void);

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("3 console arguments required\n");
		printf("First argument - name of executable (producer)\n");
		printf("Second argument - name of executable (consumer)\n");
		printf("Third argument - name of textfile with some data\n");
		exit(1);
	}
	
	// Ustawienie obsługi sygnału SIGINT
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal function error");
	}
	
	// Tworzenie potoku FIFO, 0644 = rw-r--r--
	if (mkfifo(PATH, 0644) == -1)
	{
		perror("mkfifo error");
		exit(2);
	}
	
	// Argumenty dla execlp
	char producer[25];
	char producer_exec[27];
	char consumer[25];
	char consumer_exec[27];
	char textfile[25];
	
	strcpy(producer, argv[1]);
	sprintf(producer_exec, "./%s", producer);
	strcpy(consumer, argv[2]);
	sprintf(consumer_exec, "./%s", consumer);
	strcpy(textfile, argv[3]);
	
	// Uruchamianie producenta
	int id1;
	id1 = fork();
	
	switch (id1)
	{
		case -1:
			perror("fork error");
			exit(3);
			break;
		case 0:
			execlp(producer_exec, producer, PATH, textfile, NULL);
			perror("execlp error 4");
			_exit(4);
			break;
	}
	
	// Uruchamianie konsumenta
	int id2;
	id2 = fork();
	
	switch (id2)
	{
		case -1:
			perror("fork error");
			exit(5);
			break;
		case 0:
			execlp(consumer_exec, consumer, PATH, NULL);
			perror("execlp error 6");
			_exit(6);
			break;
	}
	
	// Czekanie na producenta i konsumenta
	while (wait(NULL) > 0);
	
	// Rejestrowanie funkcji do usuwania potoku
	if (atexit(end) != 0)
	{
		perror("atexit error");
		exit(7);
	}
	
	return 0;
}

void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	end();
	exit(0);
}

void end(void)
{
	if (unlink(PATH) == -1)
	{
		perror("unlink error");
	}
}
