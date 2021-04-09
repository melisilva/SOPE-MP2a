#include "./threads.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t LOCK_IDENTIFIER;

void get_i(int *res) {
    pthread_mutex_lock(&LOCK_IDENTIFIER);

    static int i = 0;
    *res = ++i;

    pthread_mutex_unlock(&LOCK_IDENTIFIER);
}


void* thread_entry(void *arg) {
    int fd_public_fifo = *(int *)arg;
    
    int i;
    get_i(&i);

    printf("%d, %d, %ld\n", i, fd_public_fifo, pthread_self()); // just for debug
    



    return NULL;
}