#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFONAME_SIZE 200
int i;

void* send_request(void *arg){
}

int c(int duration, char* FIFO){
    srand(time(NULL));
    //char *fifoname = (char *) malloc(FIFONAME_SIZE);
    //strcpy(fifoname,argv[2]);
    pthread_t tid;

    time_t t0 = time(NULL);
    
    while (time(NULL) < t0 + duration)
    {
        //should we have mutex here?

        //create thread 
        if (pthread_create(&tid, NULL, send_request, (void*) fifoname) != 0) {
            return 1;
        }

        if (pthread_detach(tid) != 0) {
            return 1;
        }

        //wait x ms to send another request
        if (usleep((10 + rand() % 5)*1000) == -1) {
            /*tried with rand()%10 +1 but the intervals where very lil
            for nsecs=2-->rand()%10 + 1 produced 169 requests 
            for nsecs=2-->10+rand()%5 produced 106 requests*/
            return 1;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {  //c <-t nsecs> fifoname
        printf("Not enough arguments\n");
        return 1;
    }

    int nsecs = atoi(argv[1]);
    c(nsecs, argv[2]);

    //sample
    message_t *m_1 = (message_t *) malloc(sizeof(message_t));
    message_builder(m_1, 27, 2, -1);
    log_operation(m_1, FAILD);
    free(m_1);
    //free(fifoname);
    return 0;
}
