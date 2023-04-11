/*
(a) Napisać program wypisujący identyfikatory UID, GID, PID, PPID i PGID dla danego procesu.

------------------------------------
Kamil Kochańczyk, Kraków, 12.03.2022
------------------------------------
*/

#include <stdio.h>
#include "DisplayInfo.h"

int main()
{
	printf("Process info\n");
	DisplayInfo();
		
	return 0;
}
