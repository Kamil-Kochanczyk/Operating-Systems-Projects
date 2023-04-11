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

// Deskryptory kolejek komunikatów serwera oraz klienta
mqd_t server_mq;
mqd_t client_mq;

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
	
	// Atrybuty kolejki komunikatów serwera
	struct mq_attr attributes;
	attributes.mq_flags = 0;
	attributes.mq_maxmsg = MAXMSG;
	attributes.mq_msgsize = MSGSIZE;
	attributes.mq_curmsgs = 0;
	
	// Tworzenie kolejki komunikatów serwera
	server_mq = open_create_mq(SERVER_MQ_NAME, O_CREAT | O_EXCL | O_RDONLY, 0644, &attributes);
	
	// Wypisywanie atrybutów
	printf("mq_flags: %ld\n", attributes.mq_flags);
	printf("mq_maxmsg: %ld\n", attributes.mq_maxmsg);
	printf("mq_msgsize: %ld\n", attributes.mq_msgsize);
	printf("mq_curmsgs: %ld\n", attributes.mq_curmsgs);
	printf("Name: %s\n", SERVER_MQ_NAME);
	printf("Descriptor: %d\n\n", server_mq);
	
	char received_message[MSGSIZE + 1];
	char message_to_send[MSGSIZE];
	
	int pid;
	char client_mq_name[AUXILIARY];
	
	float first_number, second_number, result;
	char operation;
	
	while (1)
	{
		// Odbieranie komunikatu
		receive_mq(server_mq, received_message, MSGSIZE, NULL);
		
		// Czekanie
		sleep(rand() % 3);
		
		// Wydobywanie odpowiednich informacji z komunikatu
		sscanf(received_message, "/%d %f %c %f", &pid, &first_number, &operation, &second_number);
		
		// Wypisywanie odebranego komunikatu
		printf("%s", received_message);
		
		// Otwieranie kolejki komunikatów odpowiedniego klienta na podstawie jego PID
		sprintf(client_mq_name, "/%d", pid);
		client_mq = open_mq(client_mq_name, O_WRONLY);
		
		// Wykonywanie odpowiedniego działania
		switch (operation)
		{
			case '+':
				result = first_number + second_number;
				break;
			case '-':
				result = first_number - second_number;
				break;
			case '*':
				result = first_number * second_number;
				break;
			case '/':
				result = first_number / second_number;
				break;
		}
		
		// Tworzenie komunikatu z wynikiem działania
		sprintf(message_to_send, "%f %c %f = %f", first_number, operation, second_number, result);
		
		// Wypisywanie komunikatu
		printf("%s\n\n", message_to_send);
		
		// Wysyłanie komunikatu
		send_mq(client_mq, message_to_send, MSGSIZE, 0);
		
		// Zamykanie kolejki komunikatów klienta
		close_mq(client_mq);
	}
	
	return 0;
}

// Funkcja obsługująca sygnał SIGINT
void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	end();
	exit(0);
}

// Funkcja zamykająca i usuwająca kolejkę komunikatów serwera
void end()
{
	close_mq(server_mq);
	unlink_mq(SERVER_MQ_NAME);
}

