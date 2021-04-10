#include "./threads.h"
#include "./utils.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


pthread_mutex_t LOCK_IDENTIFIER;
pthread_mutex_t LOCK_PUBLIC_FIFO;


int get_i(int *res) {
    if (pthread_mutex_lock(&LOCK_IDENTIFIER) != 0)
        return 1;

    static int i = 0;
    *res = ++i;

    if (pthread_mutex_unlock(&LOCK_IDENTIFIER) != 0)
        return 1;
    
    return 0;
}


int comunicate_with_server_public_fifo(int fd_public_fifo, message_t message) {
    if (pthread_mutex_lock(&LOCK_PUBLIC_FIFO) != 0)
        return 1;
    
    int n=write(fd_public_fifo, &message, sizeof(message_t)); 

    if(n<0){
        perror("Couldn't write to public FIFO\n");
        return 1; //?
    }
    if (pthread_mutex_unlock(&LOCK_PUBLIC_FIFO) != 0)
        return 1;

    return 0;
}


void* thread_entry(void *arg) {
    int fd_public_fifo = *(int *)arg;
    
    int i;
    get_i(&i);
    int task_weight = (rand() % 9) + 1;

        
    char *private_fifo_path = NULL;
    int path_size = snprintf(private_fifo_path, 0, "/tmp/%d.%lu", getpid(), pthread_self()) + 1;

    private_fifo_path = malloc(path_size);

    if (snprintf(private_fifo_path, path_size, "/tmp/%d.%lu", getpid(), pthread_self()) < 0 )
    {
        free(arg);
        return NULL;
    }

    if (mkfifo(private_fifo_path, 0660) != 0) // TODO check the right perms to be "private"
    {
        free(arg);
        return NULL;
    }
    
    //printf("%d, %d, %d, %ld\n", i, task_weight, fd_public_fifo, pthread_self()); // just for debug
    printf("%s\n", private_fifo_path); // debug

    int fd_private_fifo = 0;
    message_t message;
    message_builder(&message, i, task_weight, -1); // Client res is allways -1

    if (log_operation(&message, IWANT) != 0)
        return NULL;
    
    if (comunicate_with_server_public_fifo(fd_public_fifo, message) != 0)
        return NULL;

    //printf("fg\n");

    if ((fd_private_fifo = open(private_fifo_path, O_RDONLY)) == -1){
        printf("DIDN'T OPEN\n");
        return NULL;
    }
    
    printf("OPENED.\n");
    message_t message_received;

    int n=read(fd_private_fifo, &message_received, sizeof(message_t));
    
    if(n<0){
        perror("Couldn't read private FIFO");
        return NULL;
    }    
    else if(n>0){
        if(message_received.tskres!=-1){ //server's res==-1 if it's closed
            if (log_operation(&message, GOTRS) != 0) // check if this is the right message to write
               return NULL;
        }
        else{ //we need to stop making new request threads
             closed=1;
             if (log_operation(&message, CLOSD) != 0)
               return NULL;
        }
    }
    else{
         if (log_operation(&message, GAVUP) != 0) // check if this is the right message to write
               return NULL;
    }

    close(fd_private_fifo);
    unlink(private_fifo_path);
    free(private_fifo_path);
    return NULL;
}