#include "ctr_union.h"
#include <stdlib.h>
#include <string.h>

union_set* makeUnionSet(size_t _n) {
    union_set *_set = (union_set*)malloc(sizeof(union_set));
    _set->_cnt = 0;
    _set->_siz = _n;
    _set->_parent = (int32_t*)malloc(_n * sizeof(int32_t));
    _set->_rank = (int32_t*)calloc(_n, sizeof(int32_t));

    memset(_set->_parent, UNION_SET_NOTFOUND, sizeof(int32_t)*_n);
    return _set;
}

void freeUnionSet(union_set *_set) {
    free(_set->_parent);
    free(_set->_rank);
    free(_set);
}

int32_t unionSetPut(union_set *_set, int32_t _x) {
    if(_set->_parent[_x]==UNION_SET_NOTFOUND){
        _set->_cnt++;
    }
    _set->_parent[_x] = _x;
    _set->_rank[_x] = 1;
    return _x;
}

int32_t unionSetGet(union_set *_set, int32_t _x) {
    if(_set->_parent[_x] != _x){
        _set->_parent[_x] = unionSetGet(_set, _set->_parent[_x]);
        return _set->_parent[_x];
    }
    return _x;
}

int32_t unionSetMerge(union_set *_set, int32_t _x, int32_t _y) {
    int32_t _px = unionSetGet(_set, _x);
    int32_t _py = unionSetGet(_set, _y);
    if(_px!=_py){
        if(_set->_rank[_px]<_set->_rank[_py]){
            int32_t _tmp = _px;
            _px=_py;
            _py=_tmp;
        }
        _set->_parent[_py] = _px;
        _set->_rank[_px] += _set->_rank[_py];
        _set->_cnt--;
    }
    return _px;
}