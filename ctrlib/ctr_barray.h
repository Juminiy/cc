#ifndef CTR_BARRAY_H
#define CTR_BARRAY_H

#include "ctr_elemt.h"

typedef struct barray {
    elem_t *_arr;
    size_t _siz;
    size_t _cap;
} barray;

#endif