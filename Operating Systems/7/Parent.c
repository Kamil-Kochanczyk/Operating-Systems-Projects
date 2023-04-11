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

// Funkcja obsługująca sygnal SIGINT
void signal_handler(int signal_number);

// Funkcja usuwająca semafory i pamięć dzieloną, wykonywana na końcu programu
void end(void);

// Semafory nazwane odpowiednio dla producenta i konsumenta
sem_t* producer_semaphore;
sem_t* consumer_semaphore;

int main()
{
	// Ustawienie obsługi sygnału SIGINT
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal function error");
	}
	
	// Rejestrowanie funkcji usuwającej semafory i pamięć dzieloną
	if (atexit(end) != 0)
	{
		perror("atexit error");
		exit(1);
	}
	
	// Zmienna pomocnicza przechowująca wartość jednego z semaforów
	int value;
	
	// Tworzenie semafora nazwanego dla producenta i wypisywanie jego początkowej wartości
	producer_semaphore = create_semaphore(PRODUCERSEMAPHORE, BUFFERSIZE);
	printf("(Parent) Producer semaphore address: %p\n", (void*)producer_semaphore);
	
	get_value_from_semaphore(producer_semaphore, &value);
	printf("(Parent) Producer semaphore value: %d\n", value);
	
	// Tworzenie semafora nazwanego dla konsumenta i wypisywanie jego początkowej wartości
	consumer_semaphore = create_semaphore(CONSUMERSEMAPHORE, 0);
	printf("(Parent) Consumer semaphore address: %p\n", (void*)consumer_semaphore);
	
	get_value_from_semaphore(consumer_semaphore, &value);
	printf("(Parent) Consumer semaphore value: %d\n", value);

	// Tworzenie pamięci dzielonej i wypisywanie jej deskryptora
	int descriptor = open_shared_memory(SHAREDMEMORYNAME, O_RDWR | O_CREAT | O_EXCL);
	printf("(Parent) Shared memory descriptor: %d\n", descriptor);
	
	// Ustawienie rozmiaru pamięci dzielonej
	long unsigned int size = sizeof(SharedMemorySegment);
	set_size(descriptor, size);
	printf("(Parent) Shared memory size (bytes): %ld\n", size);
	
	// Mapowanie i ustawianie indeksów producenta i konsumenta na 0
	SharedMemorySegment* pointer = (SharedMemorySegment*)map_shared_memory(size, PROT_READ | PROT_WRITE, descriptor);
	pointer->write = 0;
	pointer->read = 0;
	
	// Usunięcie mapowania i zamknięcie deskryptora
	unmap_shared_memory(pointer, size);
	close_shared_memory(descriptor);
	
	printf("\n");
	
	// Uruchamianie producenta w procesie potomnym
	int id1;
	id1 = fork();
	switch (id1)
	{
		case -1:
			perror("fork error");
			exit(2);
			break;
		case 0:
			execlp("./Producer", "Producer", PRODUCERSEMAPHORE, CONSUMERSEMAPHORE, SHAREDMEMORYNAME, FILENAME, NULL);
			perror("execlp error 3");
			_exit(3);
			break;
	}
	
	// Uruchamianie konsumenta w procesie potomnym
	int id2;
	id2 = fork();
	switch (id2)
	{
		case -1:
			perror("fork error");
			exit(4);
			break;
		case 0:
			execlp("./Consumer", "Consumer", PRODUCERSEMAPHORE, CONSUMERSEMAPHORE, SHAREDMEMORYNAME, NULL);
			perror("execlp error 5");
			_exit(5);
			break;
	}
	
	// Czekanie na procesy potomne
	while (wait(NULL) > 0);
	
	return 0;
}

// Uruchamianie funkcji usuwającej semafory i pamięć dzieloną w razie pojawienia się sygnału SIGINT
void signal_handler(int signal_number)
{
	printf("This is a custom signal handler\n");
	end();
	exit(0);
}

// Usuwanie semaforów i pamięci dzielonej
void end()
{
	unlink_semaphore(PRODUCERSEMAPHORE);
	unlink_semaphore(CONSUMERSEMAPHORE);
	unlink_shared_memory(SHAREDMEMORYNAME);
}

