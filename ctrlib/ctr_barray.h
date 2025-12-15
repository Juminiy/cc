#ifndef CTR_BARRAY_H
#define CTR_BARRAY_H

#include "ctr_elemt.h"
#include <sys/types.h>

// @attention 1. barray use value receiver
// @attention 2. `bArrayAppend`, `bArrayInsert`, `bArrayDeleteIndex`, `bArrayExtend` must receive to itself
// @attention 3. the barray container will not take responsibility of _data
typedef struct barray {
    elem_t *_arr;
    size_t _siz;
    size_t _cap;

    elem_t_cmp _cmp;     // +optional
} barray;

#define bArrayLen(_ba) (_ba._siz)
#define bArrayCap(_ba) (_ba._cap)
#define bArraySetElemCmp(_ba, _f) (_ba._cmp=_f)

barray makeBArray(size_t _siz, size_t _cap);
void freeBArray(barray _ba);

elem_t bArrayAt(barray _ba, ssize_t _idx);
void bArraySet(barray _ba, ssize_t _idx, elem_t _dt);
barray bArrayAppend(barray _ba, elem_t _dt);
barray bArrayInsert(barray _ba, ssize_t _idx, elem_t _dt);
barray bArrayDeleteIndex(barray _ba, ssize_t _idx);
barray bArrayExtend(barray _dst, barray _src);
void bArrayReverse(barray _ba);
barray bArrayCopy(barray _ba);
barray bArraySlice(barray _ba, size_t _lrg, size_t _rrg, size_t _cap);

typedef bool(*bArrayIterFunc)(size_t _idx, elem_t _dt);
void bArrayIter(barray _ba, bArrayIterFunc _fn);

ssize_t bArrayIndexOf(barray _ba, elem_t _dt, ssize_t _idx);

#endif