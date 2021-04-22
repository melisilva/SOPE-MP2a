#ifndef SRC_THREADS_H_
#define SRC_THREADS_H_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "./utils.h"

extern pthread_mutex_t LOCK_IDENTIFIER;
extern pthread_mutex_t LOCK_PUBLIC_FIFO;
extern pthread_mutex_t LOCK_RAND;
extern unsigned int RAND_R_SEED;

typedef struct args {
    message_t message;
    char *private_fifo_path;
    int *fd_private_fifo;
} args_t;

int closed;
int get_i(int *i);
int get_rand(int *res);

void* thread_entry(void *arg);
int comunicate_with_server_public_fifo(int fd_public_fifo, message_t message);


#endif  // SRC_THREADS_H_
