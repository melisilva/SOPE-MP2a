#ifndef UTILS_H_
#define UTILS_H_

#include <unistd.h>
#include <pthread.h>

typedef enum oper {
    IWANT,
    RECVD,
    TSKEX,
    TSKDN,
    GOTRS,
    _2LATE,
    CLOSD,
    GAVUP,
    FAILD
} oper_t;

typedef struct message {
    int i;
    int t;
    pid_t pid;
    pthread_t tid;
    int res;
} message_t;

void message_builder(message_t *message,int i,int t, int res);
int log_operation(message_t *message, oper_t oper);

/**
 * TO DO
 -> Function that sets oper for log_operation;
 -> Funct
 */

#endif  // UTILS_H_
