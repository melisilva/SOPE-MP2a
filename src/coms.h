#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

static bool send = false;
extern FILE* fd;

int send(char* FIFO);

int receive(char* FIFO);