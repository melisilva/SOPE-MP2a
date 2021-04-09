#!/bin/bash

set -m

FIFO_NAME=/tmp/foo
TIME=2

mkfifo -m 0666 $FIFO_NAME

./s -t $TIME $FIFO_NAME & ./c -t $TIME $FIFO_NAME

rm -f $FIFO_NAME


exit
