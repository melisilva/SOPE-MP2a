#ifndef THREADS_H_
#define THREADS_H_

#include <pthread.h>

extern pthread_mutex_t LOCK_IDENTIFIER;

void get_i(int *i);

void* thread_entry(void *arg);


#endif  // THREADS_H_
