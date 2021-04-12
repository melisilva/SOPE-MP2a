#ifndef SRC_THREADS_H_
#define SRC_THREADS_H_

#include <pthread.h>
#include "./utils.h"

extern pthread_mutex_t LOCK_IDENTIFIER;
extern pthread_mutex_t LOCK_PUBLIC_FIFO;
extern pthread_mutex_t LOCK_RAND;
extern RAND_R_SEED;


int closed;
int get_i(int *i);
int get_rand(int *res);

void* thread_entry(void *arg);
int comunicate_with_server_public_fifo(int fd_public_fifo, message_t message);


#endif  // SRC_THREADS_H_
