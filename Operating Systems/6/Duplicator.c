// Kamil Kochańczyk, Kraków, 02.05.2022

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
#include "Library.h"

// Funkcja do obsługi sygnału SIGINT
void signal_handler(int signal_number);

// Funkcja do usuwania semafora nazwanego, wywoływana na końcu programu
void end(void);

// Wskaźnik do semafora nazwanego
sem_t* sem_pointer;

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("3 console arguments required:\n");
		printf("1. Number of child processes\n");
		printf("2. Name of executable (mutual exclusion program)\n");
		printf("3. Number of critical sections of each child process\n");
		exit(1);
	}
	
	// Obsługa sygnału SIGINT
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal function error");
	}
	
	// Tworzenie pliku tekstowego i wpisywanie do niego liczby 0
	FILE* number_txt = fopen(FILENAME, "w");
	if (number_txt == NULL)
	{
		printf("Problem with writing to file\n");
		exit(2);
	}
	fputs("0", number_txt);
	fclose(number_txt);
	
	// Tworzenie semafora nazwanego i odczytywanie jego wartości
	sem_pointer = create_semaphore(SEMAPHORENAME, 1);
	
	int value;
	get_value_from_semaphore(sem_pointer, &value);
	
	printf("Address of created semaphore: %p\n", (void*)sem_pointer);
	printf("Value of semaphore: %d\n\n\n", value);
	
	// Tworzenie procesów potomnych i uruchamianie w nich programu do wzajemnego wykluczania
	int id;
	for (int i = 1; i <= atoi(argv[1]); i++)
	{
		id = fork();
		
		if (id == -1)
		{
			perror("fork error");
			exit(3);
		}
		else if (id == 0)
		{
			char program[25];
			sprintf(program, "./%s", argv[2]);
			
			execlp(program, argv[2], argv[3], NULL);
			
			perror("execlp error");
			_exit(4);
		}
		else
		{
			continue;
		}
	}
	
	// Czekanie na wszystkie procesy potomne
	while (wait(NULL) > 0);
	
	// Odczytywanie ostatecznej wartości z pliku i wypisywanie jej na ekran
	char string_number[3];
	int number;
	
	number_txt = fopen(FILENAME, "r");
	if (number_txt == NULL)
	{
		printf("Problem with reading from file\n");
		exit(5);
	}
	fgets(string_number, 3, number_txt);
	fclose(number_txt);
	
	number = atoi(string_number);	
	printf("Final value of number in file: %d\n", number);
	if ((atoi(argv[1]) * atoi(argv[3])) == number)
	{
		printf("Correct value in file\n");
	}
	else
	{
		printf("Incorrect value in file\n");
	}
	
	// Wypisywanie ostatecznej wartości semafora nazwanego
	get_value_from_semaphore(sem_pointer, &value);
	printf("Final value of semaphore: %d\n", value);
	
	// Rejestrowanie funkcji do usuwania semafora
	if (atexit(end) != 0)
	{
		perror("atexit error");
		exit(6);
	}
	
	return 0;
}

void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	end();
	exit(0);
}

void end()
{
	unlink_semaphore(SEMAPHORENAME);
}

