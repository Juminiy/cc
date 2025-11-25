#include "skt_fdset.h"
#include <stdlib.h>
#include <sys/select.h>
#include <sys/param.h>

// select specific 

optfdset* create_optfdset(int _sz) {
    optfdset* _st = (optfdset*)malloc(sizeof(optfdset));
    _st->arr = (int*)malloc(sizeof(int)*_sz);
    _st->cap = _sz;
    _st->siz = 0;
    _st->max = FD_INVALID;
    _st->min = FD_INVALID;
    for(int idx=0;idx<_st->cap;idx++){
        _st->arr[idx] = FD_INVALID;
    }
    return _st;
}

int delete_optfdset(optfdset *_fdset) {
    free(_fdset->arr);
    free(_fdset);
    return FD_OPT_OK;
}

int optfdset_add(optfdset *_fdset, int _fd) {
    if (!optfdset_valid(_fd)) {
        return FD_INVALID;
    }
    int exist_idx = optfdset_index(_fdset, _fd);
    if (exist_idx!=FD_BAD_INDEX){
        return exist_idx;
    }
    int empty_idx = optfdset_index(_fdset, FD_INVALID);
    if (empty_idx!=FD_BAD_INDEX){
        _fdset->siz++;
        _fdset->arr[empty_idx] = _fd;
        optfdset_minmax_set(_fdset, _fd);
        return empty_idx;
    }
    return FD_SET_FULL;
}

int optfdset_del(optfdset *_fdset, int _fd) {
    if (!optfdset_valid(_fd)) {
        return FD_INVALID;
    }
    int exist_idx = optfdset_index(_fdset, _fd);
    if (exist_idx!=FD_BAD_INDEX) {
        _fdset->siz--;
        _fdset->arr[exist_idx] = FD_INVALID;
        optfdset_minmax_reset(_fdset);
    }
    return FD_OPT_OK;
}

int optfdset_index(optfdset *_fdset, int _fd) {
    for (int idx=0;idx<_fdset->cap;idx++){
        if (_fdset->arr[idx]==_fd) {
            return idx;
        }
    }
    return FD_BAD_INDEX;
}

bool optfdset_valid(int _fd) {
    return _fd >=0 && _fd < FD_SETSIZE;
}

int optfdset_minmax_set(optfdset *_fdset, int _fd) {
    if (_fdset->min==FD_INVALID) {
        _fdset->min=_fd;
    } else {
        _fdset->min=MIN(_fdset->min, _fd);
    }
    if (_fdset->max==FD_INVALID) {
        _fdset->max=_fd;
    } else {
        _fdset->max=MAX(_fdset->max, _fd);
    }
    return FD_OPT_OK;
}

int optfdset_minmax_reset(optfdset *_fdset) {
    _fdset->min = FD_INVALID;
    _fdset->max = FD_INVALID;
    for(int idx=0;idx<_fdset->cap;idx++){
        if(optfdset_valid(_fdset->arr[idx])) {
            optfdset_minmax_set(_fdset, _fdset->arr[idx]);
        }
    }
}