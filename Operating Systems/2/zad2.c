/*
(b) Wywołać funkcję fork trzy razy (najlepiej w pętli for) i wypisać powyższe identyfikatory dla procesu macierzystego oraz wszystkich procesów potomnych. Przy pomocy funkcji wait sprawić, aby proces macierzysty zaczekał na zakończenie wszystkich procesów potomnych.

Zmodyfikować program z ćwiczenia 1b, tak aby komunikaty procesów potomnych były wypisywane przez program uruchamiany przez funkcję execlp. Nazwę programu do uruchomienia przekazywać przez argumenty programu procesu macierzystego.

------------------------------------
Kamil Kochańczyk, Kraków, 19.03.2022
------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
W każdej iteracji pętli z procesu macierzystego tworzony jest jeden proces potomny.
Informacje na temat procesu potomnego są wypisywane za pomocą programu DisplayInfo, którego nazwa powinna być przekazana jako argument konsoli.
Proces macierzysty czeka z przejściem do następnej iteracji pętli i utworzeniem kolejnego procesu potomnego aż obecny proces potomny skończy działanie.
*/
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Please provide a correct console argument - the name of the program that will output information about processes (UID, GID, PID, PPID, PGID).\n");
		exit (-1);
	}
	
	printf("Parent\n");
	
	printf("------------------------------------\n");
	printf("------------------------------------\n");
	
	int uid = getuid();
	int gid = getgid();
	int pid = getpid();
	int ppid = getppid();
	int pgid = getpgid(0);
	
	printf("UID = %d\n", uid);
	printf("GID = %d\n", gid);
	printf("PID = %d\n", pid);
	printf("PPID = %d\n", ppid);
	
	if (pgid == -1)
	{
		perror("PGID error");
	}
	else
	{
		printf("PGID = %d\n", pgid);
	}

	printf("------------------------------------\n");
	printf("------------------------------------\n");
	
	printf("\n\n\n");

	for (int i = 1; i <= 3; i++)
	{
		int id = fork();
		
		if (id == -1)
		{
			perror("fork error");
			exit(1);
		}
		else if (id == 0)
		{	
			printf("Child %d\n", i);
			
			char path[100];
			sprintf(path, "./%s", argv[1]);
			
			execlp(path, argv[1], NULL);
			
			perror("exexlp error");
			exit(2);
		}
		else
		{
			if (wait(NULL) == -1)
			{
				perror("wait error");
			}
		}
	}
	
	return 0;
}
