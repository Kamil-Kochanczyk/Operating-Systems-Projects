/*
------------------------------------
Kamil Kochańczyk, Kraków, 29.03.2022
------------------------------------

(c) W procesie macierzystym utworzyć proces potomny i sprawić, aby stał się liderem nowej grupy procesów (funkcja setpgid), a następnie uruchomić w nim kilka procesów potomnych wykonujących program do obsługi sygnałów. Z pierwszego procesu macierzystego wysyłać sygnały do całej grupy procesów potomnych po uprzednim sprawdzeniu jej istnienia. Identyfikator tej grupy procesów uzyskać przy pomocy funkcji getpgid. Proces będący liderem grupy procesów niech ignoruje sygnały, a na końcu czeka na zakończenie wszystkich swoich procesów potomnych i wypisuję ich status zakończenia zwracany przez funkcję wait.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Stała mówiąca ile procesów potomnych utworzyć z lidera grupy
#define CHILDREN 3

int main(int argc, char* argv[])
{
	printf("Parent process: PID = %d\n", getpid());

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
		if (setpgid(0, 0) == -1)	// Robienie z procesu potomnego lidera grupy
		{
			perror("setpgid error");
			exit(3);
		}
		
		for (int i = 1; i <= CHILDREN; i++)	// Tworzenie kilku procesów potomnych z lidera grupy
		{
			int another_id = fork();
			
			if (another_id == -1)
			{
				perror("fork error");
				exit(2);
			}
			else if (another_id == 0)
			{
				break;	// Nowo utworzony proces potomny od razu wychodzi z pętli i nie tworzy swoich własnych procesów potomnych
			}
			else
			{
				sleep(1);	// Lider grupy śpi 1 sekundę, aby był większy porządek i lepsza synchronizacja między procesami
				continue;	// Po przebudzeniu lider grupy kontynuuje wykonywanie pętli i tworzy tyle procesów potomnych, ile wynosi stała CHILDREN
			}
		}
		
		if (getpid() != getpgid(0))	// Procesy potomne lidera grupy uruchamiają program do obsługi sygnałów i czekają na sygnał
		{
			printf("\nHello from child process\n");
			execlp("./a", "a", argv[1], argv[2], NULL);
			
			perror("execlp error");
			exit(4);
		}
		else	// Lider grupy ignoruje sygnał, który jest przesyłany do grupy, a potem czeka na swoje procesy potomne i wypisuje, co się z nimi stało
		{
			printf("\nHello from group leader: PID = %d\n\n", getpid());
			
			if (signal(sig, SIG_IGN) == SIG_ERR)
			{
				perror("signal function error");
			}
			
			int status = 0;
			while (wait(&status) > 0)
			{
				if (WIFEXITED(status))
				{
					printf("Child process of group leader exited normally, exit status = %d\n", WEXITSTATUS(status));
				}

				if (WIFSIGNALED(status))
				{
					printf("Child process of group leader exited due to signal with number %d\n", WTERMSIG(status));
				}
			}
			
			printf("\n");
		}
	}
	else
	{
		sleep(CHILDREN + 1);	// Spanie w celu porządku i synchronizacji między procesami
		
		int group = getpgid(id);
		
		if (kill((-1) * group, 0) == -1)	// Sprawdzanie istnienia procesów przed wysłaniem sygnału
		{
			perror("kill error");
			exit(6);
		}
		
		printf("Parent process in program c.c is sending signal with number %d...\n\n", sig);
		
		if (kill((-1) * group, sig) == -1)	// Wysyłanie sygnału do grupy
		{
			perror("kill error");
			exit(6);
		}
		
		waitpid(id, NULL, 0);	// Czekanie na lidera grupy
	}
	
	return 0;
}
