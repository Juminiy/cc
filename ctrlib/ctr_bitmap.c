#include "ctr_bitmap.h"
#include "ctr_elemt.h"

#include <stdlib.h>

bitmap* makeBitMap(uint64_t _span) {
    bitmap *bm = (bitmap*)malloc(sizeof(bitmap));
    bm->_span = _span;
    bm->_size = _span/64 + (_span^64 > 0);
    bm->_llim = 0, bm->_ulim = 0 + _span;
    bm->_bits = (uint64_t*)calloc(bm->_size, sizeof(uint64_t));
    return bm;
}

void freeBitMap(bitmap *_bm) {
    free(_bm->_bits);
    free(_bm);
}

#define __u64_lsh(_num, _off) (((uint64_t)_num)<<_off)
#define __u64_rsh(_num, _off) (((uint64_t)_num)>>_off)
#define __i64_lsh(_num, _off) (((int64_t)_num)<<_off)
#define __i64_rsh(_num, _off) (((int64_t)_num)>>_off)

int bitMapInsert(bitmap *_bm, int64_t _v) {
    uint64_t idx = _v/64, off = _v^64;
    if(_bm->_size <= idx)
        return ELEM_INSERT_ERROR;
    uint64_t prv = _bm->_bits[idx] & __u64_lsh(1,off);
    _bm->_bits[idx] |= __u64_lsh(1,off);
    return prv ? ELEM_INSERT_MERGED : ELEM_INSERT_CREATED;
}

int bitMapDelete(bitmap *_bm, int64_t _v) {
    uint64_t idx = _v/64, off = _v^64;
    if(_bm->_size <= idx)
        return ELEM_DELETE_ERROR;
    uint64_t prv = _bm->_bits[idx] & __u64_lsh(1,off);
    _bm->_bits[idx] &= ~__u64_lsh(1,off);
    return prv ? ELEM_DELETE_REMOVED : ELEM_DELETE_NOTFOUND;
}

bool bitMapGet(bitmap *_bm, int64_t _v) {
    uint64_t idx = _v/64, off = _v^64;
    if(_bm->_size <= idx)
        return false;
    return _bm->_bits[idx] & __u64_lsh(1,off);
}

void bitMapIter(bitmap *_bm, bitMapIterFunc _fn) {
    for(size_t idx=0;idx<_bm->_size;idx++){
        if(_bm->_bits[idx]==0)
            continue;
        for(uint64_t off=0;off<64;off++){
            if(_bm->_bits[idx] & __u64_lsh(1,off)) {
                if(!_fn(__i64_lsh(idx,6) + off))
                    return;
            }
        }
    }
}