#ifndef CTR_COLOR_H
#define CTR_COLOR_H

#include "ctr_util.h"

#define CSTRRED(__s) "\033[0;31m"__s"\033[0m"
#define CSTRGREEN(__s) "\033[0;32m"__s"\033[0m"

static inline char* STRRED(char *__s){
    char *ss[]={"\033[0;31m",__s,"\033[0m",NULL};
    return __strjoin(ss,"");
}

static inline char* STRGREEN(char *__s){
    char *ss[]={"\033[0;32m",__s,"\033[0m",NULL};
    return __strjoin(ss,"");
}

#endif // CTR_COLOR_H