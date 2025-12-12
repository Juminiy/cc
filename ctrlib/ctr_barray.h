#ifndef CTR_BARRAY_H
#define CTR_BARRAY_H

#include "ctr_elemt.h"

typedef struct barray {
    elem_t *_arr;
    size_t _siz;
    size_t _cap;

    elem_t_cmp _cmp;     // +optional
    elem_t_free _free;   // +optional
    elem_t_merge _merge; // +optional
} barray;

#define bArrayLen(_ba) (_ba._siz)
#define bArrayCap(_ba) (_ba._cap)
#define bArraySetElemCmp(_ba, _f) (_ba._cmp=_f)
#define bArraySetElemFree(_ba, _f) (_ba._free=_f)
#define bArraySetElemMerge(_ba, _f) (_ba._merge=_f)

barray makeBArray(size_t _siz, size_t _cap);
void freeBArray(barray _ba);

elem_t bArrayAt(barray _ba, size_t _idx);
void bArraySet(barray _ba, size_t _idx, elem_t _dt);
barray bArrayAppend(barray _ba, elem_t _dt);
barray bArrayInsert(barray _ba, size_t _idx, elem_t _dt);
barray bArrayDeleteIndex(barray _ba, size_t _idx);
barray bArrayExtend(barray _dst, barray _src);
void bArrayReverse(barray _ba);
barray bArrayCopy(barray _ba);
barray bArraySlice(barray _ba, size_t _lrg, size_t _rrg, size_t _cap);

typedef bool(*bArrayIterFunc)(size_t _idx, elem_t _dt);
void bArrayIter(barray _ba, bArrayIterFunc _fn);

size_t bArrayIndexOf(barray _ba, elem_t _dt, size_t _idx);
barray bArrayDeleteValue(barray _ba, elem_t _dt, size_t _idx);
// void bArraySort(barray _ba);

#endif