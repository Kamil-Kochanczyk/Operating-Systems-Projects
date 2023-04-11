// Kamil Kochańczyk, Kraków, 22.05.2022

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
#include "SemaphoreLibrary.h"
#include "SharedMemoryLibrary.h"

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("3 console arguments required:\n");
		printf("1. Name of producer's semaphore\n");
		printf("2. Name of consumer's semaphore\n");
		printf("3. Name of shared memory\n");
		exit(1);
	}
	
	// Tworzenie pliku końcowego
	int output_file = open("Output", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_file == -1)
	{
		printf("Problem with output file");
		exit(2);
	}
	
	// Uzyskiwanie dostępu do semafora producenta
	sem_t* producer_semaphore = open_semaphore(argv[1]);
	printf("(Consumer) Producer semaphore address: %p\n", (void*)producer_semaphore);
	
	// Uzyskiwanie dostępu do semafora konsumenta
	sem_t* consumer_semaphore = open_semaphore(argv[2]);
	printf("(Consumer) Consumer semaphore address: %p\n", (void*)consumer_semaphore);
	
	// Otwieranie pamięci dzielonej
	int shared_memory = open_shared_memory(argv[3], O_RDWR);
	printf("(Consumer) Shared memory descriptor: %d\n", shared_memory);
	
	// Uzyskiwanie dostępu do pamięci dzielonej
	long unsigned int size = sizeof(SharedMemorySegment);
	SharedMemorySegment* pointer = (SharedMemorySegment*)map_shared_memory(size, PROT_READ | PROT_WRITE, shared_memory);
	
	// Zmienne pomocnicze
	int value;	// Wartość jednego z semaforów
	int bytes;	// Liczba bajtów odbieranych z pamięci dzielonej
	char data[ELEMENTSIZE];	// Bufor pomocniczy
	char message[100];	// Wiadomość wypisywana na ekran
	
	while (1)
	{
		// ========================= Sekcja krytyczna =========================
		wait_for_semaphore(consumer_semaphore);
		
		// Wypisywanie wartości semafora konsumenta
		get_value_from_semaphore(consumer_semaphore, &value);
		printf("(Consumer) Consumer semaphore value: %d\n", value);
		
		// Wypisywanie indeksu bufora, z którego konsument odczytuje dane
		printf("(Consumer) Buffer index: %d\n", pointer->read);
		
		// Odczytywanie danych z pamięci dzielonej
		strcpy(data, pointer->buffer[pointer->read]);
		
		// Bufor cykliczny powinien wrócić na początek po dojściu do końca
		pointer->read = ((pointer->read) + 1) % BUFFERSIZE;
		
		release_semaphore(producer_semaphore);
		// ========================= Koniec sekcji krytycznej =========================
		
		// Jeżeli konsument odczyta wartość '\0', to powinien zakończyć pracę
		if (strcmp(data, "") == 0)
		{
			break;
		}
		
		// Przekazywanie danych do pliku końcowego
		if ((bytes = write(output_file, data, strlen(data))) == -1)
		{
			perror("write error 3");
			exit(3);
		}
		
		// Wypisywanie liczby bajtów przekazanych do pliku końcowego
		sprintf(message, "(Consumer) bytes: %d\n", bytes);		
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 4");
			exit(4);
		}
		
		// Wypisywanie danych przekazanych do pliku końcowego
		sprintf(message, "(Consumer) data: %s\n", data);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 5");
			exit(5);
		}
		
		memset(data, '\0', sizeof(data));
	}
	
	// Porządki
	if (close(output_file) == -1)
	{
		perror("close error (output file)");
		exit(6);
	}
	
	unmap_shared_memory(pointer, size);
	close_shared_memory(shared_memory);
	
	return 0;
}

