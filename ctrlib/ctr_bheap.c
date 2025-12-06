#include "ctr_bheap.h"

bheap makeBHeap(elem_t_cmp _cmp) {
	bheap bh;
	bh._ll = makeBArray(0, 0);
	bArraySetElemCmp(bh._ll, _cmp);
	return bh;
}

void freeBHeap(bheap _bh) {
	freeBArray(_bh._ll);
}

bheap bHeapAdjustDown2Top(bheap _bh, size_t _idx) {
	size_t _p = (_idx-1)>>1;
	while(_p>=0&&_idx>0) {
		bool _stop = true;
		size_t plidx = _p<<1+1, pridx = _p<<1+2;
		if(_bh._ll._cmp(_bh._ll._arr[plidx], _bh._ll._arr[_p]) > 0){
			__swap_(elem_t, _bh._ll._arr[plidx], _bh._ll._arr[_p]);
			_stop = false;
		}
		if(pridx < bHeapLen(_bh)) {
			if(_bh._ll._cmp(_bh._ll._arr[pridx], _bh._ll._arr[_p]) > 0){
				__swap_(elem_t, _bh._ll._arr[pridx], _bh._ll._arr[_p]);
				_stop = false;
			}
		}
		if(_stop) {
			break;
		} else {
			_idx = _p;
			_p = (_p-1) >> 1;
		}
	}
	return _bh;
}

bheap bHeapPush(bheap _bh, elem_t _em) {
	_bh._ll = bArrayAppend(_bh._ll, _em);
	return bHeapAdjustDown2Top(_bh, bHeapLen(_bh)-1);
}

elem_t bHeapTop(bheap _bh) {
	elem_t em = {.tag=ELEM_T_INVALID};
	if(!bHeapEmpty(_bh)) {
		em = bArrayAt(_bh._ll, 0);
	}
	return em;
}

bheap bHeapAdjustTop2Down(bheap _bh, size_t _idx) {
	size_t _p = _idx;
	while(_p < bHeapLen(_bh)) {
		bool _stop = true;
		size_t plidx = _p<<1+1, pridx=_p<<1+2;
		if(plidx < bHeapLen(_bh)) {
			if(_bh._ll._cmp(_bh._ll._arr[plidx], _bh._ll._arr[_p]) < 0) {
				__swap_(elem_t, _bh._ll._arr[plidx], _bh._ll._arr[_p]);
				_idx = plidx; _stop = false;
			}
		}
		if(pridx < bHeapLen(_bh)) {
			if(_bh._ll._cmp(_bh._ll._arr[pridx], _bh._ll._arr[_p]) < 0) {
				__swap_(elem_t, _bh._ll._arr[pridx], _bh._ll._arr[_p]);
				_idx = pridx; _stop = false;
			}
		}
		if(_stop) {
			break;
		}
		_p = _idx;
	}
	return _bh;
}

bheap bHeapPop(bheap _bh) {
	size_t _hsz = bHeapLen(_bh);
	__swap_(elem_t, _bh._ll._arr[0], _bh._ll._arr[_hsz-1]);
	_bh._ll._siz--;
	return bHeapAdjustTop2Down(_bh, 0);
}

bool bHeapEmpty(bheap _bh) {
	return bHeapLen(_bh) == 0;
}