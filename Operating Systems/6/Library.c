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

sem_t* create_semaphore(char* name, int value)
{
	sem_t* pointer = sem_open(name, O_CREAT | O_EXCL, 0644, value);
	
	if (pointer == SEM_FAILED)
	{
		perror("sem_open error");
		exit(-1);
	}
	
	return pointer;
}

sem_t* open_semaphore(char* name)
{
	sem_t* pointer = sem_open(name, O_RDWR);
	
	if (pointer == SEM_FAILED)
	{
		perror("sem_open error");
		exit(-2);
	}
	
	return pointer;
}

void close_semaphore(sem_t* pointer)
{
	if (sem_close(pointer) == -1)
	{
		perror("sem_close error");
		exit(-3);
	}
}

void unlink_semaphore(char* name)
{
	if (sem_unlink(name) == -1)
	{
		perror("sem_unlink error");
		exit(-4);
	}
}

void get_value_from_semaphore(sem_t* sem_pointer, int* int_pointer)
{
	if (sem_getvalue(sem_pointer, int_pointer) == -1)
	{
		perror("sem_getvalue error");
		exit(-5);
	}
}

void wait_for_semaphore(sem_t* pointer)
{
	if (sem_wait(pointer) == -1)
	{
		perror("sem_wait error");
		exit(-6);
	}
}

void release_semaphore(sem_t* pointer)
{
	if (sem_post(pointer) == -1)
	{
		perror("sem_post error");
		exit(-7);
	}
}

