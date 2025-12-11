#ifndef CTR_BHEAP_H
#define CTR_BHEAP_H

#include "ctr_barray.h"

typedef struct bheap {
	barray _ll;
} bheap;

#define bHeapLen(_bh) (_bh._ll._siz)

bheap makeBHeap(elem_t_cmp _cmp);
void freeBHeap(bheap _bh);

bheap bHeapPush(bheap _bh, elem_t _em);
elem_t bHeapTop(bheap _bh);
bheap bHeapPop(bheap _bh);
bool bHeapEmpty(bheap _bh);
bheap bHeapRemove(bheap _bh, size_t _idx);

#endif