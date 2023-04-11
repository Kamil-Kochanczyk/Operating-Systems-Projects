#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PRODUCERBYTES 8	// Porcja danych przesyłanych do potoku przez producenta
#define CONSUMERBYTES 6	// Porcja danych odbieranych z potoku przez konsumenta

int main(int argc, char* argv[])
{
	srand(time(0));
	
	if (argc != 3)
	{
		printf("Program requires two console arguments:\n");
		printf("1. Name of the file with input data\n");
		printf("2. Name of the file with output data\n");
		return 1;
	}
	
	int input_file = open(argv[1], O_RDONLY, 0644);
	if (input_file == -1)
	{
		perror("Problem with input file");
		return 2;
	}
	
	int output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_file == -1)
	{
		perror("Problem with output file");
		return 3;
	}
	
	int fd[2];	// Deskryptory plików, fd[0] - odczyt z potoku, fd[1] - zapis do potoku
	if (pipe(fd) == -1)
	{
		perror("pipe error");
		return 4;
	}
	
	int id = fork();
	
	if (id == -1)
	{
		perror("fork error");
		return 5;
	}
	else if (id == 0)	// Proces potomny - producent
	{
		close(fd[0]);
		
		int bytes;
		char data[PRODUCERBYTES + 1];
		char message[100];
		
		while ((bytes = read(input_file, data, PRODUCERBYTES)) != 0)
		{
			if (bytes == -1)
			{
				perror("read error");
				return 6;
			}
			
			// Wypisywanie liczby bajtów przesyłanych do potoku
			sprintf(message, "Producer bytes: %d\n", bytes);
			if (write(STDOUT_FILENO, message, strlen(message)) == -1)
			{
				perror("write error 7");
				return 7;
			}
			
			// Wypisywanie danych przesyłanych do potoku
			sprintf(message, "Producer data: %s\n\n\n", data);
			if (write(STDOUT_FILENO, message, strlen(message)) == -1)
			{
				perror("write error 8");
				return 8;
			}
			
			// Przesyłanie danych do potoku
			if (write(fd[1], data, strlen(data)) == -1)
			{
				perror("write error 9");
				return 9;
			}
			
			// Czyszczenie zmiennej tekstowej
			memset(data, '\0', sizeof(data));
			
			// Symulowanie różnych szybkości działania programu
			sleep(rand() % 3);
		}
		
		if (close(fd[1]) == -1)
		{
			perror("close error");
			return 9;
		}
		
		printf("End of writing to the pipe\n\n\n");
	}
	else	// Proces macierzysty - konsument
	{
		close(fd[1]);
		
		int bytes;
		char data[CONSUMERBYTES + 1];
		char message[100];
		
		while ((bytes = read(fd[0], data, CONSUMERBYTES)) != 0)
		{
			if (bytes == -1)
			{
				perror("read error");
				return 10;
			}
			
			// Wypisywanie liczby bajtów przesyłanych do pliku końcowego
			sprintf(message, "Consumer bytes: %d\n", bytes);
			if (write(STDOUT_FILENO, message, strlen(message)) == -1)
			{
				perror("write error 11");
				return 11;
			}
			
			// Wypisywanie danych przesyłanych do pliku końcowego
			sprintf(message, "Consumer data: %s\n\n\n", data);
			if (write(STDOUT_FILENO, message, strlen(message)) == -1)
			{
				perror("write error 12");
				return 12;
			}
			
			// Przesyłanie danych do pliku końcowego
			if (write(output_file, data, strlen(data)) == -1)
			{
				perror("write error 13");
				return 13;
			}
			
			// Czyszczenie zmiennej tekstowej
			memset(data, '\0', sizeof(data));
			
			// Symulowanie różnych szybkości działania programu
			sleep(rand() % 3);
		}
		
		if (close(fd[0]) == -1)
		{
			perror("close error");
			return 14;
		}
		
		printf("End of reading from the pipe\n\n\n");
		
		wait(NULL);
	}
	
	return 0;
}
