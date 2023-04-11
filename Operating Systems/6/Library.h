#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

#define SEMAPHORENAME "/SomeNiceSemaphore"
#define FILENAME "Number.txt"

sem_t* create_semaphore(char* name, int value);

sem_t* open_semaphore(char* name);

void close_semaphore(sem_t* pointer);

void unlink_semaphore(char* name);

void get_value_from_semaphore(sem_t* sem_pointer, int* int_pointer);

void wait_for_semaphore(sem_t* pointer);

void release_semaphore(sem_t* pointer);

#endif
