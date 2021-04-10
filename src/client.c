#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include "./utils.h"
#include "./threads.h"


int main_cycle(time_t end_time, int fd_public_fifo) {

    while (time(NULL) < end_time) {
        pthread_t tid;
        //should we have mutex here?
   /*if (pthread_mutex_lock(&LOCK_PUBLIC_FIFO) != 0) { // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;    
    }*/
        //create thread 
        if(pthread_create(&tid, NULL, thread_entry, (void*)&fd_public_fifo) != 0) {
            return 1;
        }

        /*if(pthread_detach(tid)!=0) { // TODO with this uncommented it's called allways with the same thread
                                       // maybe save all the tid an after this while loop, loop over the tid with .join()
            return 1;
        }*/

       /*if (pthread_mutex_unlock(&LOCK_PUBLIC_FIFO) != 0) {
        perror("");
        return 1;  
       }*/
    

        

        //wait x ms to send another request
        if (usleep(( 60000)*1000) == -1) { 
            /*tried with rand()%10 +1 but the intervals where very lil
                                                                        for nsecs=2-->rand()%10 + 1 produced 169 requests 
                                                                        for nsecs=2-->10+rand()%5 produced 106 requests*/
            return 1;
        }
    }

        

    return 0;
}

int input_check(int argc, char *argv[], int *nsecs, int *fd_public_fifo) {
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "-t")) { // id argv[1] != "-t"
        fprintf(stderr, "Expected -t parameter not found.\n");
        return 1;
    }

    char *end;
    *nsecs = strtol(argv[2], &end, 10);
    if (argv[2] == end) {
        fprintf(stderr, "Invalid number of seconds.\n");
        return 1;
    }

    if ((*fd_public_fifo = open(argv[3], O_WRONLY)) == -1) {
        fprintf(stderr, "No public pipe found with given path.\n");
        return 1;
    }
    //close(*fd_public_fifo);
    


    return 0;
}


int main(int argc, char *argv[]) {
    time_t start_time = time(NULL);

    srand(time(NULL));
    int nsecs;
    int fd_public_fifo;

    if (input_check(argc, argv, &nsecs, &fd_public_fifo) != 0)
        return 1;
    

    // TODO maybe create a function to init all the mutex we will need
    // and other to destroy them
    /*if (pthread_mutex_init(&LOCK_IDENTIFIER, NULL) != 0) { // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;    
    }

    if (pthread_mutex_init(&LOCK_PUBLIC_FIFO, NULL) != 0) { // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;    
    }
*/
    time_t end_time = start_time + nsecs;
    if (main_cycle(end_time, fd_public_fifo) != 0)
        return 1;


   /* if (pthread_mutex_destroy(&LOCK_IDENTIFIER) != 0) {
        perror("");
        return 1;    
    }

    if (pthread_mutex_destroy(&LOCK_PUBLIC_FIFO) != 0) {
        perror("");
        return 1;    
    }*/

    close(fd_public_fifo);
    printf("we're closed\n");

    return 0;
}
