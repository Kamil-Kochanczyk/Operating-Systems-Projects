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

int main(int argc, char* argv[])
{
	srand(time(0));
	
	if (argc != 2)
	{
		printf("Program needs 1 console argument - number of critical sections\n");
		exit(1);
	}
	
	int critical_sections = atoi(argv[1]);
	if (critical_sections <= 0)
	{
		printf("Number of critical sections must a positive integer\n");
		exit(2);
	}
	
	sem_t* sem_pointer = open_semaphore(SEMAPHORENAME);
	
	int semaphore_value;
	int iteration = 1;
	FILE* number_txt;
	char string_number[3];
	int number;
	
	do
	{
		get_value_from_semaphore(sem_pointer, &semaphore_value);
		printf("PID = %d, semaphore = %d, BEFORE critical section %d\n\n\n", getpid(), semaphore_value, iteration);
		
		sleep(rand() % 3);
		
		// ==================================== Sekcja krytyczna ====================================
		wait_for_semaphore(sem_pointer);
		
		number_txt = fopen(FILENAME, "r");
		if (number_txt == NULL)
		{
			printf("Problem with reading from file\n");
			exit(3);
		}
		fgets(string_number, 3, number_txt);
		fclose(number_txt);
		
		printf("\tNumber in file: %s\n", string_number);
		
		number = atoi(string_number);
		number++;
		sprintf(string_number, "%d", number);
		
		sleep(rand() % 3);
		
		number_txt = fopen(FILENAME, "w");
		if (number_txt == NULL)
		{
			printf("Problem with writing to file\n");
			exit(4);
		}
		fputs(string_number, number_txt);
		fclose(number_txt);
		
		get_value_from_semaphore(sem_pointer, &semaphore_value);
		printf("\tPID = %d, semaphore = %d, INSIDE critical section %d\n\n\n", getpid(), semaphore_value, iteration);
		
		release_semaphore(sem_pointer);
		// ==================================== Koniec sekcji krytycznej ====================================
		
		get_value_from_semaphore(sem_pointer, &semaphore_value);
		printf("PID = %d, semaphore = %d, AFTER critical section %d\n\n\n", getpid(), semaphore_value, iteration);
		
		memset(string_number, '\0', sizeof(string_number));
		iteration++;
	} while (iteration <= critical_sections);
	
	return 0;
}

