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

// Porcja danych producenta
#define PRODUCERBYTES 7

int main(int argc, char* argv[])
{
	srand(time(0));
	
	if (argc != 3)
	{
		printf("Producer requires 2 console arguments - path to a FIFO pipe and name of textfile\n");
		exit(1);
	}
	
	// Otwieranie potoku tylko do zapisu
	int descriptor;
	if ((descriptor = open(argv[1], O_WRONLY)) == -1)
	{
		perror("open error (producer)");
		exit(2);
	}

	// Tworzenie deskryptora pliku, z którego producent będzie przesyłał dane do potoku
	int input_file = open(argv[2], O_RDONLY, 0644);
	if (input_file == -1)
	{
		printf("Problem with input file");
		exit(3);
	}
	
	int bytes;
	char data[PRODUCERBYTES + 1];
	char message[100];
	
	while ((bytes = read(input_file, data, PRODUCERBYTES)) != 0)
	{			
		if (bytes == -1)
		{
			perror("read error");
			exit(4);
		}
		
		// Wypisywanie liczby bajtów odczytanych przez funkcję read
		sprintf(message, "Producer bytes: %d\n", bytes);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 5");
			exit(5);
		}
		
		// Wypisywanie danych odczytanych z pliku i przesyłanych do potoku
		sprintf(message, "Producer data: %s\n\n\n", data);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 6");
			exit(6);
		}
		
		// Przesyłanie danych do potoku
		if (write(descriptor, data, strlen(data)) == -1)
		{
			perror("write error 7");
			exit(7);
		}
		
		// Czyszczenie zmiennej tekstowej
		memset(data, '\0', sizeof(data));
		
		// Symulowanie różnych szybkości działania programu
		sleep(rand() % 3);
	}
	
	printf("End of writing to FIFO pipe\n\n\n");
	
	if (close(descriptor) == -1)
	{
		perror("close error (producer)");
		exit(8);
	}
	
	return 0;
}
