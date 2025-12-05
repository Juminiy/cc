#ifndef CTR_BARRAY_H
#define CTR_BARRAY_H

#include "ctr_elemt.h"

typedef struct barray {
    elem_t *_arr;
    size_t _siz;
    size_t _cap;

    elem_t_cmp _cmp;
} barray;

#define bArrayLen(_ba) (_ba->_siz)
#define bArrayCap(_ba) (_ba->_cap)

barray makeBArray(size_t _siz, size_t _cap);
void freeBArray(barray _ba);

elem_t bArrayAt(barray _ba, size_t _idx);
void bArraySet(barray _ba, size_t _idx, elem_t _dt);
void bArrayAppend(barray _ba, elem_t _dt);
void bArrayInsert(barray _ba, size_t _idx, elem_t _dt);
void bArrayDeleteIndex(barray _ba, size_t _idx);
void bArrayExtend(barray _dst, barray _src);
void bArrayReverse(barray _ba);
barray bArraySlice(barray _ba, size_t _lrg, size_t _rrg); // [_lrg, _rrg)

typedef bool(*bArrayIterFunc)(size_t _idx, elem_t _dt);
void bArrayIter(barray _ba, bArrayIterFunc _fn);

size_t bArrayIndexOf(barray _ba, elem_t _dt);
void bArrayDeleteValue(barray _ba, elem_t _dt);
void bArraySort(barray _ba);

#endif