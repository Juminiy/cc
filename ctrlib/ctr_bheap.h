#ifndef CTR_BHEAP_H
#define CTR_BHEAP_H

#include "ctr_barray.h"

// bheap use value receiver
// currently BUGs
typedef struct bheap {
	barray _ll;

	elem_t_cmp _cmp;
	elem_t_swap _swap;
} bheap;

#define bHeapLen(_bh) (_bh._ll._siz)

bheap makeBHeap(elem_t_cmp _cmp, elem_t_swap _swap);
void freeBHeap(bheap _bh);

bheap bHeapPush(bheap _bh, elem_t _em);
elem_t bHeapTop(bheap _bh);
bheap bHeapPop(bheap _bh);
bool bHeapEmpty(bheap _bh);
elem_t bHeapAt(bheap _bh, size_t _idx);
bheap bHeapRemove(bheap _bh, size_t _idx);
bool bHeapCheck(bheap _bh);

#endif