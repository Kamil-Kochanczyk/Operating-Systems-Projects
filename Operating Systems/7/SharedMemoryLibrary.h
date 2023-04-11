#ifndef SHARED_MEMORY_GUARD
#define SHARED_MEMORY_GUARD

#define SHAREDMEMORYNAME "/SomeNiceSharedMemory"
#define BUFFERSIZE 7
#define ELEMENTSIZE 4

typedef struct
{
	char buffer[BUFFERSIZE][ELEMENTSIZE];
	int write;
	int read;
} SharedMemorySegment;

int open_shared_memory(char* name, int flags);

void set_size(int descriptor, off_t size);

void close_shared_memory(int descriptor);

void unlink_shared_memory(char* name);

void* map_shared_memory(size_t length, int protection, int descriptor);

void unmap_shared_memory(void* address, size_t length);

#endif

