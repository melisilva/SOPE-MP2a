#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>
#include "./utils.h"

extern pthread_mutex_t LOCK_IDENTIFIER;
extern pthread_mutex_t LOCK_PUBLIC_FIFO;


int get_i(int *i);

void* thread_entry(void *arg);
int comunicate_with_server_public_fifo(int fd_public_fifo, message_t message);


#endif  // THREADS_H_
