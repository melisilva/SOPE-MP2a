#!/bin/bash

set -m

FIFO_NAME=/tmp/foo
TIME=1
BUF_SIZE=1


mkfifo -m 0666 $FIFO_NAME

./s -t $TIME -l $BUF_SIZE $FIFO_NAME & ./c -t $TIME $FIFO_NAME

rm -f $FIFO_NAME


exit
