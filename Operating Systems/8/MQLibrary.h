#ifndef MQLIBRARY_GUARD
#define MQLIBRARY_GUARD

#define SERVER_MQ_NAME "/SomeNiceServerMQ"
#define AUXILIARY 50
#define MAXMSG 10
#define MSGSIZE 100

mqd_t open_mq(char* name, int flags);

mqd_t open_create_mq(char* name, int flags, mode_t mode, struct mq_attr* attributes);

void close_mq(mqd_t descriptor);

void unlink_mq(char* name);

void get_mq_attributes(mqd_t descriptor, struct mq_attr* attributes);

void set_mq_attributes(mqd_t descriptor, struct mq_attr* new_attributes, struct mq_attr* old_attributes);

void send_mq(mqd_t descriptor, char* message, size_t message_length, unsigned int priority);

void receive_mq(mqd_t descriptor, char* message, size_t message_length, unsigned int* priority);

#endif

