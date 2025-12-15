#include "ctr_barray.h"

#include <stdlib.h>
#include <string.h>

barray makeBArray(size_t _siz, size_t _cap) {
    barray _ba;
    _ba._siz = _siz > 0 ? _siz : 0;
    _ba._cap = _cap > _siz ? _cap : _siz;
    _ba._arr = _ba._cap > 0 ? 
        (elem_t*)calloc(_ba._cap, sizeof(elem_t)) : NULL;
    _ba._cmp = NULL;
    return _ba;
}

// @attention 
// freeBArray will not free each _data in the array container
void freeBArray(barray _ba) {
    free(_ba._arr);
}

#define __index(_ba, _idx) ((_ba._siz+_idx)%(_ba._siz))

// @attention support both positive index and negative index
// @example [0] and [-_siz] refer to [0]
// @example [_siz-1] and [-1] refer to [_siz-1]
elem_t bArrayAt(barray _ba, ssize_t _idx) {
    return _ba._arr[__index(_ba,_idx)];
}

// @attention support both positive index and negative index
// @example [0] and [-_siz] refer to [0]
// @example [_siz-1] and [-1] refer to [_siz-1]
void bArraySet(barray _ba, ssize_t _idx, elem_t _dt) {
    _ba._arr[__index(_ba,_idx)] = _dt;
}

#define __barray_grow(_ba, _num) \
    do { \
        size_t oldsiz = _ba._siz, oldcap = _ba._cap; \
        size_t newsiz = oldsiz+_num, newcap = oldcap; \
        if(newsiz <= newcap) \
            goto __end_tag; \
        size_t d2cap = newcap << 1; \
        if(newsiz > d2cap) { \
            newcap = newsiz; \
            goto __alloc_tag; \
        } \
        const size_t threshold = 256; \
        if(oldcap < threshold) { \
            newcap = d2cap; \
            goto __alloc_tag; \
        } \
        do { \
            newcap += (newcap + 3*threshold) >> 2; \
        } while(newcap < newsiz); \
        newcap = newcap <= 0 ? newsiz : newcap; \
    __alloc_tag: \
        _ba._arr = realloc(_ba._arr, sizeof(elem_t)*newcap); \
        memset(_ba._arr + newsiz, 0, sizeof(elem_t)*(newcap-newsiz)); \
    __end_tag: \
        _ba._siz = newsiz; \
        _ba._cap = newcap; \
    }while(0)

// @attention
// ba = bArrayAppend(ba, em); ✅
// bArrayAppend(ba, em); ❌
barray bArrayAppend(barray _ba, elem_t _dt) {
    size_t _pos = _ba._siz;
    __barray_grow(_ba, 1);
    _ba._arr[_pos] = _dt;
    return _ba;
}

// @attention
// ba = bArrayInsert(ba, idx, em); ✅
// bArrayInsert(ba, idx, em); ❌
barray bArrayInsert(barray _ba, ssize_t _idx, elem_t _dt) {
    ssize_t _pos = _ba._siz;
    __barray_grow(_ba, 1);
    ssize_t _ati = __index(_ba, _idx);
    for(ssize_t _i=_pos;_i>_ati;_i--){
        _ba._arr[_i] = _ba._arr[_i-1];
    }
    _ba._arr[_ati] = _dt;
    return _ba;
}

// @attention
// ba = bArrayDeleteIndex(ba, idx); ✅
// bArrayDeleteIndex(ba, idx); ❌
barray bArrayDeleteIndex(barray _ba, ssize_t _idx) {
    ssize_t _ati = __index(_ba, _idx);
    for(ssize_t _i=_ati;_i<_ba._siz-1;_i++){
        _ba._arr[_i] = _ba._arr[_i+1];
    }   
    _ba._siz--;
    return _ba;
}

// @attention
// ba = bArrayExtend(ba, ba2); ✅
// bArrayExtend(ba, ba2); ❌
barray bArrayExtend(barray _dst, barray _src) {
    size_t oldsiz = _dst._siz;
    __barray_grow(_dst, _src._siz);
    memcpy(_dst._arr+oldsiz, _src._arr, sizeof(elem_t)*_src._siz);
    return _dst;
}

void bArrayReverse(barray _ba) {
    for(size_t _i=0;_i<(_ba._siz>>1);_i++){
        __swap_(elem_t, _ba._arr[_i], _ba._arr[_ba._siz-_i-1]);
    }
}

barray bArrayCopy(barray _ba) {
    barray _nba = makeBArray(_ba._siz, _ba._cap);
    memcpy(_nba._arr, _ba._arr, sizeof(elem_t)*_ba._siz);
    _nba._cmp = _ba._cmp;
    return _nba;
}

// [_lrg, _rrg)
barray bArraySlice(barray _ba, size_t _lrg, size_t _rrg, size_t _cap) {
    barray _nba = makeBArray(_rrg-_lrg, _cap);
    memcpy(_nba._arr, _ba._arr+_lrg, sizeof(elem_t)*(_rrg-_lrg));
    _nba._cmp = _ba._cmp;
    return _nba;
}

void bArrayIter(barray _ba, bArrayIterFunc _fn) {
    for(size_t _idx=0;_idx<_ba._siz;_idx++){
        if(!_fn(_idx, _ba._arr[_idx])){
            return;
        }
    }
}

// _idx >= 0, forward
// _idx < 0, backward
ssize_t bArrayIndexOf(barray _ba, elem_t _dt, ssize_t _idx) {
    for(ssize_t _iat = __index(_ba, _idx);
        _idx>=0?(_iat<_ba._siz):(_iat>=0);
        _iat += (_idx>=0? +1 : -1)    
    ) {
        if(_ba._cmp(_dt, _ba._arr[_iat]) == 0){
            return _iat;
        }
    }
    return -1;
}