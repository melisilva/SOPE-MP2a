#ifndef LOG_H_
#define LOG_H_


typedef enum oper {
    IWANT,
    RECVD,
    TSKEX,
    TSKDN,
    GOTRS,
    _2LATE,
    CLOSD,
    GAVUP,
    FAILD
} oper_t;


void log_operation(int i, int t, int res, oper_t oper);


#endif  // LOG_H_
