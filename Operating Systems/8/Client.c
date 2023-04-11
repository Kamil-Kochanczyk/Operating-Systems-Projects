// Kamil Kochańczyk, Kraków, 24.05.2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <mqueue.h>
#include "MQLibrary.h"

// Funkcja do obsługi sygnału SIGINT
void signal_handler(int signal_number);

// Funkcja do zarejestrowania przez atexit
void end(void);

// Zmienna pomocnicza i deskryptory kolejek komunikatów
char client_mq_name[AUXILIARY];
mqd_t client_mq;
mqd_t server_mq;

int main()
{	
	// Obsługa sygnału SIGINT
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal function error");
	}
	
	// Zarejestrowanie odpowiedniej funkcji przez atexit
	if (atexit(end) != 0)
	{
		perror("atexit error");
		exit(1);
	}
	
	// Atrybuty kolejki komunikatów klienta
	struct mq_attr attributes;
	attributes.mq_flags = 0;
	attributes.mq_maxmsg = MAXMSG;
	attributes.mq_msgsize = MSGSIZE;
	attributes.mq_curmsgs = 0;
	
	// Tworzenie nazwy kolejki komunikatów klienta na podstawie jego PID
	int pid = getpid();
	sprintf(client_mq_name, "/%d", pid);
	
	// Tworzenie kolejki komunikatów klienta
	client_mq = open_create_mq(client_mq_name, O_CREAT | O_EXCL | O_RDONLY, 0644, &attributes);
	
	// Otwieranie kolejki komunikatów serwera
	server_mq = open_mq(SERVER_MQ_NAME, O_WRONLY);
	
	// Wypisywanie atrubutów
	printf("mq_flags: %ld\n", attributes.mq_flags);
	printf("mq_maxmsg: %ld\n", attributes.mq_maxmsg);
	printf("mq_msgsize: %ld\n", attributes.mq_msgsize);
	printf("mq_curmsgs: %ld\n", attributes.mq_curmsgs);
	printf("Name: %s\n", client_mq_name);
	printf("Descriptor: %d\n\n", client_mq);
	
	// Zmienne pomocnicze
	char expression[AUXILIARY];
	char message_to_send[MSGSIZE];
	char received_message[MSGSIZE + 1];
	
	printf("Enter an expression in the form \"[number] [operation] [number]\", e.g. 3 + 5\n");
	printf("Available operations: +, -, *, /\n");
	printf("To quit the loop, press Ctrl-D\n\n");
	
	while (fgets(expression, MSGSIZE, stdin) != NULL)
	{
		// Tworzenie komunikatu dla serwera
		sprintf(message_to_send, "/%d %s", pid, expression);
		
		// Wysyłanie komunikatu do serwera
		send_mq(server_mq, message_to_send, MSGSIZE, 0);
		
		// Czekanie
		sleep(rand() % 3);
		
		// Odbieranie komunikatu zwrotnego od serwera
		receive_mq(client_mq, received_message, MSGSIZE, NULL);
		
		// Wypisywanie odebranego komunikatu
		printf("%s\n\n", received_message);
	}
	
	printf("Finished!\n");
	
	return 0;
}

// Funkcja obsługująca sygnał SIGINT
void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	end();
	exit(0);
}

// Zamykanie i usuwanie
void end()
{
	close_mq(client_mq);
	unlink_mq(client_mq_name);
	
	close_mq(server_mq);
}

