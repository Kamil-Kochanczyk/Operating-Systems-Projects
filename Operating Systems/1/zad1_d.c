/*
(d) Jak w (b), tylko wstawić funkcję sleep w takich miejscach programu, aby procesy pojawiały się na ekranie grupowane pokoleniami od najstarszego do najmłodszego, a proces macierzysty kończył się dopiero po procesach potomnych (nie używać funkcji wait). Na podstawie wyników programu sporządzić (w pliku tekstowym) ,,drzewo genealogiczne” tworzonych procesów z zaznaczonymi identyfikatorami PID, PPID i PGID.

------------------------------------
Kamil Kochańczyk, Kraków, 13.03.2022
------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DisplayInfo.h"

/*
Na początku proces macierzysty tworzy i otwiera plik "Tree" i wypisuje w nim informacje o sobie, rozpoczynając w ten sposób drzewo genealogiczne.
Później w każdej iteracji pętli tworzony jest proces potomny, który również otwiera plik "Tree" i dopisuje do drzewa genealogicznego informacje o sobie.
Proces macierzysty jest usypiany na odpowiednio długi czas i nie przechodzi do następnej iteracji.
Proces potomny przechodzi do następnej iteracji, staje się procesem macierzystym dla kolejnego procesu potomnego i zostaje uśpiony. Jego proces potomny z kolei przechodzi do następnej iteracji, staje się procesem macierzystym i tak dalej.
Procesy macierzyste od razu po przebudzeniu kończą działanie.
W międzyczasie na ekran wypisywane są informacje o danym procesie.
*/
int main()
{
	FILE* pointer;
	pointer = fopen("Tree", "w");
	
	if (pointer != NULL)	// Proces macierzysty dopisuje informacje o sobie do drzewa genealogicznego
	{
		fprintf(pointer, "PID = %d\n"
						 "PPID = %d\n"
						 "PGID = %d\n"
						 "|\n|\n|\n" , getpid(), getppid(), getpgid(0));
		fclose(pointer);
	}
	else
	{
		perror("File pointer error:");
		exit(1);
	}
	
	DisplayInfo();
	printf("\n\n\n");

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
			pointer = fopen("Tree", "a");
			
			if (pointer != NULL)	// Proces potomny dopisuje informacje o sobie do drzewa genealogicznego
			{
				fprintf(pointer, "PID = %d\n"
								 "PPID = %d\n"
								 "PGID = %d\n"
								 "|\n|\n|\n" , getpid(), getppid(), getpgid(0));
				fclose(pointer);
			}
			else
			{
				perror("File pointer error:");
				exit(1);
			}

			DisplayInfo();
			printf("\n\n\n");
		}
		else
		{
			sleep(NUM - i + 1);	// Proces utworzony na początku musi zostać uśpiony na dłużej niż proces utworzony na końcu, bo musi zaczekać na większą liczbę procesów
			exit(0);
		}
	}
	
	return 0;
}
