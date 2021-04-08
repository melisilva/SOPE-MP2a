#include "log.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


static char * OPERATIONS[] = {"IWANT", "RECVD", "TSKEX", "TSKDN", "GOTRS", "2LATE", "CLOSD", "GAVUP", "FAILD"};


void log_operation(int i, int t, int res, oper_t oper) {
    printf("%ld ; %d ; %d ; %d ; %ld ; %d ; %s\n", time(NULL), i, t, getpid(), pthread_self(), res, OPERATIONS[oper]);
}
