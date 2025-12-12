#include "ctr_bheap.h"
#include <assert.h>

bheap makeBHeap(elem_t_cmp _cmp, elem_t_swap _swap) {
	bheap bh;
	bh._ll = makeBArray(0, 0);
	bh._ll._cmp = _cmp;
	bh._cmp = _cmp;
	bh._swap = _swap ? _swap : __elem_swap_normalized;
	return bh;
}

void freeBHeap(bheap _bh) {
	freeBArray(_bh._ll);
}

#define __bheap_cmp(_i0, _i1) \
	(_bh._cmp(_bh._ll._arr[_i0], _bh._ll._arr[_i1]))
#define __bheap_swap(_i0, _i1) \
	(_bh._swap(_bh._ll._arr+_i0, _bh._ll._arr+_i1))

void bHeapAdjustDown2Top(bheap _bh, size_t _idx) {
	while(_idx > 0) {
		bool _stop = true;
		size_t _p = (_idx-1)>>1;
		size_t plidx = (_p<<1)+1, pridx = (_p<<1)+2;
		// assert(__in_range_(0,_p,bHeapLen(_bh)) &&
		// 		__in_range_(0,plidx,bHeapLen(_bh)) &&
		// 		__in_range_(0,pridx,bHeapLen(_bh))	);
		if(__bheap_cmp(plidx, _p) > 0){
			__bheap_swap(plidx, _p);
			_stop = false;
		}
		if(pridx < bHeapLen(_bh)) {
			if(__bheap_cmp(pridx, _p) > 0){
				__bheap_swap(pridx, _p);
				_stop = false;
			}
		}
		if(_stop) {
			break;
		}
		_idx = _p;
	}
}

bheap bHeapPush(bheap _bh, elem_t _em) {
	_bh._ll = bArrayAppend(_bh._ll, _em);
	bHeapAdjustDown2Top(_bh, bHeapLen(_bh)-1);
	return _bh;
}

elem_t bHeapTop(bheap _bh) {
	elem_t em = {.tag=ELEM_T_INVALID};
	if(!bHeapEmpty(_bh)) {
		em = bArrayAt(_bh._ll, 0);
	}
	return em;
}

bool bHeapAdjustTop2Down(bheap _bh, size_t _idx, size_t _n) {
	size_t _p = _idx;
	while(_p < _n) {
		size_t plidx = (_p<<1)+1;
		if(plidx >= _n) { // OR plidx overflow
			break;
		}
		size_t pchs = plidx, pridx = plidx+1;
		if(pridx < _n && __bheap_cmp(pridx, plidx)>0) {
			pchs = pridx;
		}
		if(__bheap_cmp(pchs, _p) <= 0) {
			break;
		}
		__bheap_swap(pchs, _p);
		_p = pchs;
	}
	return _p > _idx;
}

bheap bHeapPop(bheap _bh) {
	size_t _hsz = bHeapLen(_bh);
	if(_hsz > 0) {
		__bheap_swap(0, _hsz-1);
		bHeapAdjustTop2Down(_bh, 0, _hsz-1);
		_bh._ll._siz--;
	}
	return _bh;
}

bool bHeapEmpty(bheap _bh) {
	return bHeapLen(_bh) == 0;
}

elem_t bHeapAt(bheap _bh, size_t _idx) {
	elem_t em={.tag=ELEM_T_INVALID};
	if(_idx < bHeapLen(_bh)) {
		em = bArrayAt(_bh._ll, _idx);
	}
	return em;
}

bheap bHeapRemove(bheap _bh, size_t _idx) {
	size_t _hsz = bHeapLen(_bh);
	if(_hsz > 0 && _hsz-1!=_idx) {
		__bheap_swap(_idx, _hsz-1);
		if (!bHeapAdjustTop2Down(_bh, _idx, _hsz-1)){
			bHeapAdjustDown2Top(_bh, _idx);
		}
	}
	return bHeapPop(_bh);
}

bool bHeapCheck(bheap _bh) {
	for(size_t p=0;p<bHeapLen(_bh);p++){
		size_t pl = (p<<1)+1, pr = (p<<1)+2;
		if(pl < bHeapLen(_bh)){
			if(__bheap_cmp(pl, p)>0){
				return false;
			}
		}
		if(pr < bHeapLen(_bh)) {
			if(__bheap_cmp(pr, p)>0){
				return false;
			}
		}
	}
	return true;
}