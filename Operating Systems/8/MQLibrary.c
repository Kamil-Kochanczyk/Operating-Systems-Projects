// Kamil Kochańczyk, Kraków, 24.05.2022

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
#include <mqueue.h>

mqd_t open_mq(char* name, int flags)
{
	mqd_t descriptor = mq_open(name, flags);
	
	if (descriptor == ((mqd_t)-1))
	{
		perror("open_mq error");
		exit(-1);
	}
	
	return descriptor;
}

mqd_t open_create_mq(char* name, int flags, mode_t mode, struct mq_attr* attributes)
{
	mqd_t descriptor = mq_open(name, flags, mode, attributes);
	
	if (descriptor == ((mqd_t)-1))
	{
		perror("open_create_mq error");
		exit(-2);
	}
	
	return descriptor;
}

void close_mq(mqd_t descriptor)
{
	int value = mq_close(descriptor);
	
	if (value == -1)
	{
		perror("mq_close error");
		exit(-3);
	}
}

void unlink_mq(char* name)
{
	int value = mq_unlink(name);
	
	if (value == -1)
	{
		perror("mq_unlink error");
		exit(-4);
	}
}

void get_mq_attributes(mqd_t descriptor, struct mq_attr* attributes)
{
	int value = mq_getattr(descriptor, attributes);
	
	if (value == -1)
	{
		perror("mq_getattr error");
		exit(-5);
	}
}

void set_mq_attributes(mqd_t descriptor, struct mq_attr* new_attributes, struct mq_attr* old_attributes)
{
	int value = mq_setattr(descriptor, new_attributes, old_attributes);
	
	if (value == -1)
	{
		perror("mq_setattr error");
		exit(-6);
	}
}

void send_mq(mqd_t descriptor, char* message, size_t message_length, unsigned int priority)
{
	int value = mq_send(descriptor, message, message_length, priority);
	
	if (value == -1)
	{
		perror("mq_send error");
		exit(-7);
	}
}

void receive_mq(mqd_t descriptor, char* message, size_t message_length, unsigned int* priority)
{
	int value = mq_receive(descriptor, message, message_length, priority);
	
	if (value == -1)
	{
		perror("mq_receive error");
		exit(-8);
	}

}

