/*
Kamil Kochańczyk, 24.04.2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Porcja danych konsumenta
#define CONSUMERBYTES 6

int main(int argc, char* argv[])
{
	srand(time(0));
	
	if (argc != 2)
	{
		printf("Consumer requires 1 console argument - path to a FIFO pipe\n");
		exit(1);
	}
	
	// Otwieranie potoku tylko do odczytu
	int descriptor;
	if ((descriptor = open(argv[1], O_RDONLY)) == -1)
	{
		perror("open error (consumer)");
		exit(2);
	}
	
	// Tworzenie deksryptora pliku, do którego konsument będzie przesyłał dane z potoku
	int output_file = open("Output", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_file == -1)
	{
		printf("Problem with output file");
		exit(3);
	}
	
	int bytes;
	char data[CONSUMERBYTES + 1];
	char message[100];
	
	while ((bytes = read(descriptor, data, CONSUMERBYTES)) != 0)
	{
		if (bytes == -1)
		{
			perror("read error");
			exit(4);
		}

		// Wypisywanie liczby bajtów odczytanych przez funkcję read	
		sprintf(message, "Consumer bytes: %d\n", bytes);		
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 5");
			exit(5);
		}
		
		// Wypisywanie danych odczytanych z potoku i przesyłanych do pliku końcowego
		sprintf(message, "Consumer data: %s\n\n\n", data);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 6");
			exit(6);
		}
		
		// Przesyłanie danych do pliku końcowego
		if (write(output_file, data, strlen(data)) == -1)
		{
			perror("write error 7");
			exit(7);
		}
		
		// Czyszczenie zmiennej tekstowej
		memset(data, '\0', sizeof(data));
		
		// Symulowanie różnych szybkości działania programu
		sleep(rand() % 3);
	}
	
	printf("End of reading from FIFO pipe\n\n\n");
	
	if (close(descriptor) == -1)
	{
		perror("close error (consumer)");
		exit(8);
	}
	
	return 0;
}
