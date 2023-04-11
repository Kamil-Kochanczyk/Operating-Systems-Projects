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
	if (argc != 5)
	{
		printf("4 console arguments required:\n");
		printf("1. Name of producer's semaphore\n");
		printf("2. Name of consumer's semaphore\n");
		printf("3. Name of shared memory\n");
		printf("4. Name of textfile\n");
		exit(1);
	}

	// Otwieranie pliku tekstowego w trybie tylko do odczytu
	int input_file = open(argv[4], O_RDONLY, 0644);
	if (input_file == -1)
	{
		printf("Problem with input file");
		exit(2);
	}
	
	// Uzyskiwanie dostępu do semafora producenta i wypisywanie jego adresu
	sem_t* producer_semaphore = open_semaphore(argv[1]);
	printf("(Producer) Producer semaphore address: %p\n", (void*)producer_semaphore);
	
	// Uzyskiwanie dostępu do semafora konsumenta i wypisywanie jego adresu
	sem_t* consumer_semaphore = open_semaphore(argv[2]);
	printf("(Producer) Consumer semaphore address: %p\n", (void*)consumer_semaphore);
	
	// Otwieranie pamięci dzielonej i wypisywanie jej deskryptora
	int shared_memory = open_shared_memory(argv[3], O_RDWR);
	printf("(Producer) Shared memory descriptor: %d\n\n\n", shared_memory);
	
	// Mapowanie
	long unsigned int size = sizeof(SharedMemorySegment);
	SharedMemorySegment* pointer = (SharedMemorySegment*)map_shared_memory(size, PROT_READ | PROT_WRITE, shared_memory);
	
	// Zmienne pomocnicze
	int value;	// Wartość jednego z semaforów
	int bytes;	// Liczba bajtów przekazywanych do pamięci dzielonej
	char data[ELEMENTSIZE];	// Bufor pomocniczy
	char message[100];	// Wiadomość wypisywana na ekran
	
	// Pętla wykonująca się dopóty, dopóki nie trafi się na koniec pliku
	while ((bytes = read(input_file, data, ELEMENTSIZE - 1)) != 0)
	{			
		if (bytes == -1)
		{
			perror("read error");
			exit(3);
		}

		// Wypisywanie liczby bajtów przekazywanych do pamięci dzielonej
		sprintf(message, "(Producer) Bytes: %d\n", bytes);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 4");
			exit(4);
		}
		
		// Wypisywanie danych przekazywanych do pamięci dzielonej
		sprintf(message, "(Producer) Data: %s\n", data);
		if (write(STDOUT_FILENO, message, strlen(message)) == -1)
		{
			perror("write error 5");
			exit(5);
		}
		
		// ========================= Sekcja krytyczna =========================
		wait_for_semaphore(producer_semaphore);
		
		// Wypisywanie wartości semafora producenta
		get_value_from_semaphore(producer_semaphore, &value);
		printf("(Producer) Producer semaphore value: %d\n", value);
		
		// Wypisywanie do którego indeksu bufora przekazywane są dane
		printf("(Producer) Buffer index: %d\n", pointer->write);
		
		// Przekazywanie danych do pamięci dzielonej
		strcpy(pointer->buffer[pointer->write], data);
		
		// Bufor cykliczny powinien wrócić na początek po dojściu do końca
		pointer->write = ((pointer->write) + 1) % BUFFERSIZE;
		
		release_semaphore(consumer_semaphore);
		// ========================= Koniec sekcji krytycznej =========================
		
		memset(data, '\0', sizeof(data));
	}
	
	// Producent przekazuje wartość '\0' do bufora, aby konsument wiedział kiedy zakończyć pracę
	wait_for_semaphore(producer_semaphore);
	pointer->buffer[pointer->write][0] = '\0';
	pointer->write = ((pointer->write) + 1) % BUFFERSIZE;
	release_semaphore(consumer_semaphore);
	
	// Porządki
	if (close(input_file) == -1)
	{
		perror("close error (input file)");
		exit(6);
	}
	
	unmap_shared_memory(pointer, size);
	close_shared_memory(shared_memory);
	
	return 0;
}

