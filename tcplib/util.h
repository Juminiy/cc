#ifndef UITL_H
#define UTIL_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FATAL(msg) \
    do { fprintf(stderr, "[FATAL] %s, sys_error: %s\n", msg, strerror(errno)); exit(EXIT_FAILURE); } while(0)
#define FATALF(__content_template__, ...) \
    do { fprintf(stderr, "[FATAL] "__content_template__", sys_error: %s\n" __VA_OPT__(,) __VA_ARGS__, strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define ERR(msg) \
    do { fprintf(stderr, "[ERROR] %s\n", msg); } while(0)
#define ERRF(__content_template__, ...) \
    do { fprintf(stderr, "[ERROR] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#define WARN(msg) \
    do { fprintf(stderr, "[ WARN] %s\n", msg); } while(0)
#define WARNF(__content_template__, ...) \
    do { fprintf(stderr, "[ WARN] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#define INFO(msg) \
    do { fprintf(stdout, "[ INFO] %s\n", msg); } while(0)
#define INFOF(__content_template__, ...) \
    do { fprintf(stdout, "[ INFO] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#define DEBUG(msg) \
    do { fprintf(stdout, "[DEBUG] %s\n", msg); } while(0)
#define DEBUGF(__content_template__, ...) \
    do { fprintf(stdout, "[DEBUG] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

void str_trunc(char *_str, int _len);

#endif