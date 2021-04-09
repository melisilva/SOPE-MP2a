#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


static char * OPERATIONS[] = {"IWANT", "RECVD", "TSKEX", "TSKDN", "GOTRS", "2LATE", "CLOSD", "GAVUP", "FAILD"};


void message_builder(message_t *message, int i,int t, int res) {

    message->i = i;
    message->t = t;
    message->pid = getpid();
    message->tid = pthread_self();
    message->res=res;
}

int log_operation(message_t *message, oper_t oper) {
    if(printf("%ld ; %d ; %d ; %d ; %ld ; %d ; %s\n", time(NULL), message->i, message->t, message->pid, message->tid, message->res, OPERATIONS[oper])<0){
        return 1;
    }
    return 0;
}