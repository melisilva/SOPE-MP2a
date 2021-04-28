#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "./threads.h"
#include "./utils.h"

#define PRIVATE_PERMS 0600

pthread_mutex_t LOCK_IDENTIFIER;
pthread_mutex_t LOCK_RAND;
unsigned int RAND_R_SEED;

int get_i(int *res) {
    if (pthread_mutex_lock(&LOCK_IDENTIFIER) != 0) {
        return 1;
    }

    static int i = 0;
    *res = ++i;

    if (pthread_mutex_unlock(&LOCK_IDENTIFIER) != 0) {
        return 1;
    }

    return 0;
}


int get_rand(int *res) {
    if (pthread_mutex_lock(&LOCK_RAND) != 0) {
        return 1;
    }

    *res = rand_r(&RAND_R_SEED);

    if (pthread_mutex_unlock(&LOCK_RAND) != 0) {
        return 1;
    }

    return 0;
}


int comunicate_with_server_public_fifo(int fd_public_fifo, message_t message) {
    int n = write(fd_public_fifo, &message, sizeof(message_t));

    if (n < 0) {
        perror("Couldn't write to public FIFO\n");
        return 1;
    }

    return 0;
}


void thread_handler_clean_up(void *argsp) {
    args_t args = *(args_t *)argsp;

    if (*args.communicated == 0){
        log_operation(&args.message, GAVUP); 
    }

    if (*(args.fd_private_fifo) != 0) {
        close(*(args.fd_private_fifo));
    }

    if (args.private_fifo_path != NULL) {
        unlink(args.private_fifo_path);
    }

    if (args.private_fifo_path != NULL) {
        free(args.private_fifo_path);
    }
}


void* thread_entry(void *arg) {

    int fd_public_fifo = *(int *) arg;

    int i;
    if (get_i(&i) < 0) {
        return NULL;
    }

    int rand_num;
    if (get_rand(&rand_num) != 0) {
        return NULL;
    }

    int task_weight = (rand_num % 9) + 1;

    message_t message;
    message_builder(&message, i, task_weight, -1);

    char *private_fifo_path = NULL;
    int path_size = snprintf(private_fifo_path, 0, "/tmp/%d.%lu",
                                getpid(), pthread_self()) + 1;                         

    if (path_size == -1) {
        return NULL;
    }

    private_fifo_path = malloc(path_size);
    int fd_private_fifo = 0;
    int communicated = -1;
    
    args_t args = {.message = message, .fd_private_fifo = &fd_private_fifo, .private_fifo_path = private_fifo_path, .communicated = &communicated};
    pthread_cleanup_push(thread_handler_clean_up, (void *)&args); 

    if (snprintf(private_fifo_path, path_size, "/tmp/%d.%lu",
        getpid(), pthread_self()) < 0 ) {
        free(private_fifo_path);
        return NULL;
    }


    if (mkfifo(private_fifo_path, PRIVATE_PERMS) != 0) {
        free(private_fifo_path);
        return NULL;
    }


    if ((communicated = comunicate_with_server_public_fifo(fd_public_fifo, message)) != 0) {
        free(private_fifo_path);
        return NULL;
    }

    if (log_operation(&message, IWANT) != 0) {
        free(private_fifo_path);
        return NULL;
    }

    if ((fd_private_fifo = open(private_fifo_path, O_RDONLY)) == -1) {
        perror("Couldn't open private FIFO\n");
        free(private_fifo_path);
        return NULL;
    }

    message_t message_received;

    int n = read(fd_private_fifo, &message_received, sizeof(message_t));

    if (n < 0) {
        perror("Couldn't read private FIFO");
        close(fd_private_fifo);
        unlink(private_fifo_path);
        free(private_fifo_path);
        return NULL;
    } else if (n > 0) {
        if (message_received.tskres != -1) {
            // server's res==-1 if it's closed
            message_builder(&message, i, task_weight, message_received.tskres);
            if (log_operation(&message, GOTRS) != 0) {
                close(fd_private_fifo);
                unlink(private_fifo_path);
                free(private_fifo_path);
                return NULL;
            }
        } else {
            // we need to stop making new request threads
            closed = 1;
            if (log_operation(&message, CLOSD) != 0) {
                close(fd_private_fifo);
                unlink(private_fifo_path);
                free(private_fifo_path);
                return NULL;
            }
        }
    }
    close(fd_private_fifo);
    unlink(private_fifo_path);
    free(private_fifo_path);

    pthread_cleanup_pop(0);

    return NULL;
}