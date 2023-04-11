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
#include "SharedMemoryLibrary.h"

int open_shared_memory(char* name, int flags)
{
	int descriptor = shm_open(name, flags, 0644);
	
	if (descriptor == -1)
	{
		perror("shm_open error");
		exit(-1);
	}
	
	return descriptor;
}

void set_size(int descriptor, off_t size)
{
	int value = ftruncate(descriptor, size);
	
	if (value == -1)
	{
		perror("ftruncate error");
		exit(-2);
	}
}

void close_shared_memory(int descriptor)
{
	int value = close(descriptor);
	
	if (value == -1)
	{
		perror("close error");
		exit(-3);
	}
}

void unlink_shared_memory(char* name)
{
	int value = shm_unlink(name);
	
	if (value == -1)
	{
		perror("shm_unlink error");
		exit(-4);
	}
}

void* map_shared_memory(size_t length, int protection, int descriptor)
{
	void* pointer = mmap(NULL, length, protection, MAP_SHARED, descriptor, 0);
	
	if (pointer == MAP_FAILED)
	{
		perror("mmap error");
		exit(-5);	
	}
	
	return pointer;
}

void unmap_shared_memory(void* address, size_t length)
{
	int value = munmap(address, length);
	
	if (value == -1)
	{
		perror("munmap error");
		exit(-6);
	}
}

