#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Struktura opakowująca argumenty funkcji wywoływanej przez wątki
typedef struct
{
	int thread_id;
	int number_of_threads;
	int critical_sections;
	pthread_mutex_t* mutex_pointer;
} my_function_args;

// Funkcja, która będzie wywoływana przez wątki
void* my_function(void* arg);

// Funkcja pomocnicza zmieniająca pozycję kursora (na podstawie StartSO)
void go_to(unsigned int x, unsigned int y, int clear);

// Licznik globalny i licznik aktywnych wątków
unsigned int global = 0;
unsigned int thread_count;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Program requires 2 console arguments:\n");
		printf("1. Number of threads\n");
		printf("2. Number of critical sections per thread\n");
		exit(1);
	}
	
	int number_of_threads = atoi(argv[1]);
	int critical_sections = atoi(argv[2]);
	
	if (number_of_threads < 0 || critical_sections < 0)
	{
		printf("Console arguments must be positive integers\n");
		exit(2);
	}
	
	thread_count = number_of_threads;
	
	if (system("clear") == -1)
	{
    	perror("Problem  with clear");
    	exit(3);
    }
    
    // Ukrycie kursora
    printf("\e[?25l");
	
	pthread_mutex_t mutex;
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		perror("Problem with pthread_mutex_init");
		exit(4);
	}
	printf("Address of mutex: %p\n\n", (void*)(&mutex));
	
	pthread_t threads[number_of_threads];
	my_function_args args[number_of_threads];
	
	for (int i = 0; i < number_of_threads; i++)
	{
		args[i].thread_id = i;
		args[i].number_of_threads = number_of_threads;
		args[i].critical_sections = critical_sections;
		args[i].mutex_pointer = &mutex;
		
		if (pthread_create(&threads[i], NULL, &my_function, (void*)(&args[i])) != 0)
		{
			perror("Problem with pthread_create");
			exit(5);
		}
		
		printf("Thread %d, id %ld\n", i, threads[i]);
	}
	
	printf("\n");
	
	for (int i = 0; i < number_of_threads; i++)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			perror("Problem with pthread_join");
			exit(6);
		}
	}
	
	while (thread_count > 0);
	
	pthread_mutex_destroy(&mutex);
	
	go_to(1, (2 * number_of_threads) + 5, 1);
	
	printf("Global: %d\n", global);
	printf("Expected value of global: %d\n", number_of_threads * critical_sections);
	printf("%s\n", (global == number_of_threads * critical_sections) ? "Correct" : "Incorrect");
	
	// Pokazanie kursora
	printf("\e[?25h");
	
	return 0;
}

void* my_function(void* arg)
{
	my_function_args* args = (my_function_args*)arg;
	
	int thread_id = args->thread_id;
	int number_of_threads = args->number_of_threads;
	int critical_sections = args->critical_sections;
	pthread_mutex_t* mutex_pointer = args->mutex_pointer;
	
	// Licznik lokalny i zmienna pomagająca ustawić pozycję kursora
	int local;
	unsigned int y_origin = number_of_threads + 4;
	
	sleep(1);
	
	for (int i = 1; i <= critical_sections; i++)
	{	
		go_to(1, y_origin + thread_id, 1);
		printf("Thread %d, section %d\n", thread_id, i);
		
		sleep((rand() % 2) + 1);	
		
		// ==================================== Sekcja krytyczna ====================================
		pthread_mutex_lock(mutex_pointer);
		
		sleep(1);
		
		go_to(50, y_origin + thread_id, 1);
		printf("Thread %d, critical section %d, global %d\n", thread_id, i, global);
		
		local = global;
		sleep((rand() % 2) + 1);
		local++;
		global = local;
		
		pthread_mutex_unlock(mutex_pointer);
		// ==================================== Koniec sekcji krytycznej ====================================
		
		go_to(1, y_origin + thread_id, 1);
		printf("Thread %d, section %d\n", thread_id, i);
	}
	
	sleep(1);
	
	thread_count--;
	
	return NULL;
}

void go_to(unsigned int x, unsigned int y, int clear)
{
	if (clear)
	{
		// Ustawianie pozycji kursora i usuwanie poprzedniej zawartości wiersza
		printf("\033[%d;%dH\033[2K", y, x);
	}
	else
	{
		// Ustawianie pozycji kursora bez usuwania poprzedniej zawartości wiersza
		printf("\033[%d;%dH", y, x);
	}
}

