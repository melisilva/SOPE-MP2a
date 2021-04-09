#include "coms.h"

FILE *fd;

int send(char* FIFO){
    fd = open(FIFO, O_WRONLY);

    //get whatever we need to send, lol
    char *dummy = "dummy\n";
    char *read[100];

    //Sending == writing.
    write(fd, dummy, strlen(dummy)+1);
    //LOG: IWANT, I believe
    close(fd);
}

int receive(char* FIFO){
    //Receiving == reading.
    fd = open(FIFO, O_RDONLY);
    read(fd, read, sizeof(read));
    //LOG: RECVD
    close(fd);
}