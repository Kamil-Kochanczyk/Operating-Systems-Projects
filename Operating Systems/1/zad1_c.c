/*
(c) Jak w (b), tylko przy użyciu funkcji sleep (nie używać funkcji wait) sprawiać, by procesy potomne były adoptowane przez proces init lub systemd; poleceniem pstree -p z poziomu powłoki wyświetlić drzewo procesów w danym systemie i zidentyfikować proces adoptujący osierocone procesy.

------------------------------------
Kamil Kochańczyk, Kraków, 12.03.2022
------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DisplayInfo.h"

/*
W każdej iteracji pętli proces macierzysty tworzy jeden proces potomny, który jest od razu usypiany.
Kolejne procesy potomne są usypiane na coraz dłuższy czas (czas uśpienia zwiększa się za każdym razem o 1 sekundę).
Proces macierzysty kończy działanie i dopiero wtedy procesy potomne budzą się, jeden po drugim.
Po wypisaniu informacji na temat każdego procesu wyświetla się drzewo procesów.
Pod drzewem procesów wyświetla się proces adoptujący osierocone procesy (wyróżniony kolorem i znaleziony przy pomocy PPID).
*/
int main()
{
	for (int i = 1; i <= NUM; i++)
	{
		int id = fork();
		
		if (id == -1)
		{
			perror("fork error");
			exit(1);
		}
		else if (id == 0)
		{	
			sleep(i);
			
			if (i == 1)
			{
				printf("\n");	// Żeby wyniki na ekranie ładniej wyglądały
			}
			
			printf("Child %d\n", i);
			DisplayInfo();
			printf("\n\n\n");
			
			if (i == NUM)
			{
				printf("Tree\n");
				
				if(system("pstree -p") == -1)	// Wypisywanie drzewa procesów
				{
					perror("pstree -p error:");
				}
				
				printf("\n");
				printf("New parent process is a process with PID %d\n", getppid());
				
				char command[100];	// Komeda ta pozwoli na zmianę koloru tekstu i wyszukanie procesu adoptującego osierocone procesy
				sprintf(command, "echo \"\033[0;31m\"; pstree -p | grep \"%d\"", getppid());
				
				if(system(command) == -1)	// Zmienianie koloru tekstu i szukanie procesu adoptującego osierocone procesy
				{
					perror("echo or grep error:");
				}
				
				if (system("echo \"\033[0m\"") == -1)	// Resetowanie koloru
				{
					perror("echo error:");
				}
				
				printf("Press Enter to continue\n");
			}
			
			exit(0);
		}
	}
	
	printf("Parent\n");
	DisplayInfo();

	return 0;
}
