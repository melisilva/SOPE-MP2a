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
    size_t size_tids = 1000;
    pthread_t *tids = malloc(size_tids * sizeof(pthread_t));
    size_t i = 0;
    while (time(NULL) < end_time && !closed) {


        if (i == size_tids - 1) {
            size_tids += 1000;
            tids = realloc(tids, size_tids * sizeof(pthread_t));
        }

        // create thread
        if (pthread_create(&tids[i++], NULL, thread_entry,
                            (void*)&fd_public_fifo) != 0) {
            return 1;
        }

        // wait x ms to send another request
        int rand_num;
        if (get_rand(&rand_num) != 0)
            return 1;

        if (usleep((100+rand_num%10)*1000) == -1) {
            /*tried with rand()%10 +1 but the intervals where very lil
            for nsecs=2-->rand()%10 + 1 produced 169 requests 
            for nsecs=2-->10+rand()%5 produced 106 requests*/
            return 1;
        }
    }

    if (time(NULL) >= end_time) { /* "após o prazo de funcionamento especificado pelo utilizador, o Cliente deve terminar
                                      a execução fazendo com que os threads em espera de resposta desistam mas não
                                      sem antes garantir que todos os recursos tomados ao sistema são libertados."
                                    */
       
        for (size_t j = 0; j < i; j++) {
            pthread_cancel(tids[j]);
        }
    }
    
    // needs to call _join also for the canceled because: "Cancel THREAD immediately or at the next possibility."
    
    for (size_t j = 0; j < i; j++) {
        if (pthread_join(tids[j], NULL) != 0) {
            // maybe better not to return on error since all threads must join
        }
    }

    
    free(tids);
    return 0;
}

int input_check(int argc, char *argv[], int *nsecs, int *fd_public_fifo,time_t start_time,time_t *end_time) {
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "-t")) {
        // id argv[1] != "-t"
        fprintf(stderr, "Expected -t parameter not found.\n");
        return 1;
    }

    char *end;
    *nsecs = strtol(argv[2], &end, 10);
    if (argv[2] == end) {
        fprintf(stderr, "Invalid number of seconds.\n");
        return 1;
    }

    int opened=1;

    if ((*fd_public_fifo = open(argv[3], O_WRONLY)) == -1) {
        time_t *end_time = &start_time + *nsecs;
        opened=0;
        while(start_time<*end_time && !opened){
            if ((*fd_public_fifo = open(argv[3], O_WRONLY)) != -1){
                opened=1;
            }
        }
    }
    

    if(opened==0){
        fprintf(stderr, "No public pipe found with given path.\n");
        return 1;
    }
    

    return 0;
}


int init_mutexs() {

    if (pthread_mutex_init(&LOCK_IDENTIFIER, NULL) != 0) {
        // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;    
    }

    if (pthread_mutex_init(&LOCK_PUBLIC_FIFO, NULL) != 0) {
        // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;
    }

    if (pthread_mutex_init(&LOCK_RAND, NULL) != 0) {
        // TODO check if mutexattr should not be NULL!!
        perror("");
        return 1;
    }

    return 0;
}


int destroy_mutexs() {

    if (pthread_mutex_destroy(&LOCK_IDENTIFIER) != 0) {
        perror("");
        return 1;
    }

    if (pthread_mutex_destroy(&LOCK_PUBLIC_FIFO) != 0) {
        perror("");
        return 1;
    }

    if (pthread_mutex_destroy(&LOCK_RAND) != 0) {
        perror("");
        return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {
    time_t start_time = time(NULL);
    closed=0;

    RAND_R_SEED = start_time;
    int nsecs;
    int fd_public_fifo;
    time_t end_time;
    
    if (input_check(argc, argv, &nsecs, &fd_public_fifo,start_time,&end_time) != 0) {
        return 1;
    }

    if (init_mutexs() != 0) {
        return 1;
    }

    //time_t end_time = start_time + nsecs;
    if (main_cycle(end_time, fd_public_fifo) != 0) {
        return 1;
    }

    if (destroy_mutexs() != 0) {
        return 1;
    }

    close(fd_public_fifo);
    //printf("we're closed\n");

    return 0;
}