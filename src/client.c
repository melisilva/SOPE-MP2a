#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

pthread_t tid[INT_MAX];
FILE* fd;

int c(int duration, char *FIFO) {
    mkfifo(FIFO, 0666);  // Starting FIFO file + give the file all permissions needed.

    t0 = time(NULL)
    while (t < t0 + duration) {
        //new_interval = random number no intervalo pedido
        usleep(rand() % 2001);  //só como dummy
        //executar pedido de comunicação

        //Aqui vai ter de haver alguma espécie de distinção entre quando se envia e quando se recebe - um if qualquer.
        //send(FIFO);
        //receive(FIFO);       
    }

    //LOG: GAVUP - Tempo de execução do cliente foi-se.
}

int main(int argc, char *argv[]) {
    //argv[0] é c, [1] o nsecs e [2] o fifoname.
    //client(argv[1], argv[2])
    //sample
    message_t *m_1 = (message_t *)malloc(sizeof(message_t));
    message_builder(m_1, 27, 2, -1);
    log_operation(m_1, FAILD);

    return 0;
}
