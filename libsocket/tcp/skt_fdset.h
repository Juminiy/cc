#ifndef TCP_SET_H
#define TCP_SET_H

#include <stdbool.h>

#define FD_OPT_OK 0
#define FD_INVALID -1
#define FD_BAD_INDEX -2
#define FD_SET_FULL -3

// add, delete, foreach, ensured input data unique
// do not permit enlarge
typedef struct optfdset {
    int *arr; // fd: all init to `-1`
    int cap; // capacity
    int siz; // size
    int max; // max_fd
    int min; // min_fd
} optfdset;

optfdset* create_optfdset(int _sz);
int delete_optfdset(optfdset *_fdset);
int optfdset_add(optfdset *_fdset, int _fd);
int optfdset_del(optfdset *_fdset, int _fd);
int optfdset_index(optfdset *_fdset, int _fd);
bool optfdset_valid(int _fd);
int optfdset_minmax_set(optfdset *_fdset, int _fd);
int optfdset_minmax_reset(optfdset *_fdset);

#endif