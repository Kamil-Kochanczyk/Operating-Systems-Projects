/*
------------------------------------
Kamil Kochańczyk, Kraków, 29.03.2022
------------------------------------

(a) Napisać program do obsługi sygnałów z możliwościami: (1) wykonania operacji domyślnej, (2) ignorowania oraz (3) przechwycenia i własnej obsługi sygnału. Do oczekiwania na sygnał użyć funkcji pause. Uruchomić program i wysyłać do niego sygnały przy pomocy sekwencji klawiszy oraz przy pomocy polecenia kill z poziomu powłoki.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

extern const char* const sys_siglist[];

void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	printf("Signal number: %d\n", signal_number);
	printf("Signal name: %s\n", sys_siglist[signal_number]);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Program needs two console arguments:\n");
		printf("- number of a signal\n");
		printf("- number specyfying how to handle the signal (1 - default handling, 2 - ignore signal, 3 - custom handling)\n");
		
		exit(1);
	}
	
	printf("Program a.c: PID = %d\n", getpid());
	
	int sig = atoi(argv[1]);
	int handling_option = atoi(argv[2]);
	
	switch (handling_option)
	{
		case 1:	// Domyślna obsługa sygnału
		if (signal(sig, SIG_DFL) == SIG_ERR)
		{
			perror("signal function error");
		}
		break;
			
		case 2:	// Ignorowanie sygnału
		if (signal(sig, SIG_IGN) == SIG_ERR)
		{
			perror("signal function error");
		}
		break;
			
		case 3:	// Własna obsługa sygnału
		if (signal(sig, &signal_handler) == SIG_ERR)
		{
			perror("signal function error");
		}
		break;
			
		default:
		printf("Wrong console argument\n");
		printf("First console argument should be the number of a signal\n");
		printf("Second console argument should be the number specyfying how to handle the signal (1, 2 or 3)\n");
		exit(2);
		break;
	}
	
	printf("Program a.c is waiting for the signal with number %d...\n\n", sig);
	
	if (pause() == -1)
	{
		perror("pause");
	}
	
	return 0;
}
