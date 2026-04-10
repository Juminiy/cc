#ifndef CTR_UNION_H
#define CTR_UNION_H

#include <stdint.h>
#include <stddef.h>

#define UNION_SET_NOTFOUND ((int32_t)-1)

typedef struct union_set {
    int32_t *_parent;
    int32_t *_rank;
    size_t  _cnt,_siz;
} union_set;

#define unionSetCnt(_set) (_set->_cnt)
union_set* makeUnionSet(size_t _n);
void freeUnionSet(union_set *_set);
int32_t unionSetPut(union_set *_set, int32_t _x);
int32_t unionSetGet(union_set *_set, int32_t _x);
int32_t unionSetMerge(union_set *_set, int32_t _x, int32_t _y);

#endif // CTR_UNION_H