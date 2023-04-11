/*
(b) Wywołać funkcję fork trzy razy (najlepiej w pętli for) i wypisać powyższe identyfikatory dla procesu macierzystego oraz wszystkich procesów potomnych. Przy pomocy funkcji wait sprawić, aby proces macierzysty zaczekał na zakończenie wszystkich procesów potomnych.

------------------------------------
Kamil Kochańczyk, Kraków, 12.03.2022
------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "DisplayInfo.h"

/*
W każdej iteracji pętli tworzony jest proces potomny.
Na ekran wypisywane są informacje na temat procesu potomnego.
Od razu po wypisaniu wszystkich informacji proces potomny kończy działanie.
Jednocześnie proces macierzysty natrafia na linijkę kodu, która każe mu czekać na proces potomny.
W ten sposób proces macierzysty przejdzie do następnej iteracji dopiery wtedy, gdy proces potomny utworzony w danej iteracji się zakończy.
W kolejnych iteracjach proces ten się powtarza.
Na końcu wypisywane są informacje o procesie macierzystym.
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
			printf("Child %d\n", i);
			DisplayInfo();
			printf("\n\n\n");
			exit(0);
		}
		else
		{
			if (wait(NULL) == -1)
			{
				perror("wait error");
			}
		}
	}
	
	printf("Parent\n");
	DisplayInfo();
	
	return 0;
}
