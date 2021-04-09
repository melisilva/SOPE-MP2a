#include <stdio.h>
#include <stdlib.h>

#include "utils.h"



int main() {
    
    //sample
    message_t* m_1 = (message_t*) malloc(sizeof(message_t));
    message_builder(m_1,27,2,-1);
    log_operation(m_1,FAILD);

    return 0;
}
