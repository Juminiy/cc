#ifndef UITL_H
#define UTIL_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ERRF(msg) \
    do { fprintf(stderr, "[ERROR] "#msg", error: %s\n", strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define INFOF(msg) \
    do { fprintf(stdout, "[INFO ] "#msg"\n"); } while(0)

#define DEBUGF(msg) \
    do { fprintf(stdout, "[DEBUG] "#msg"\n"); } while(0)

#endif