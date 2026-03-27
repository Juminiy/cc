#ifndef CTR_HASH_H
#define CTR_HASH_H

#include "ctr_elemt.h"

#define HASHTABLE_LOAD_FACTOR 0.75
#define HASHTABLE_BUKT_INITSZ 16

#define HASHBUCKET_TYPE_NONE 0
#define HASHBUCKET_TYPE_LIST 1
#define HASHBUCKET_TYPE_TREE 2
#define HASHBUCKET_TYPE_SKIP 3

#define HASHBUCKET_LIST_MAXSZ 8
#define HASHBUCKET_TREE_MINSZ 6
#define HASHBUCKET_BUKT_2TREE_MINSZ 64

typedef struct hashbucket {
    void *_ptr;
    int   _typ;
} hashbucket;

typedef struct hashtable {
    hashbucket *_bkt_ll;
    size_t      _bkt_siz;
    size_t      _bkt_cap;

    size_t      _size;
    float       _load;

    elem_t_hash _hash;
    elem_t_cmp  _cmp;
    elem_t_free _free;
    elem_t_merge _merge;
} hashtable;

#define hashLen(_ht) (_ht->_size)
#define hashSetElemLoad(_ht, _lf) (_ht->_load=_lf)
#define hashSetElemFree(_ht, _fn) (_ht->_free=_fn)
#define hashSetElemMerge(_ht, _fn) (_ht->_merge=_fn)
#define __hash_free(_ht, _dt) \
    do { if(_ht->_free) { _ht->_free(_dt); } } while(0)
#define __hash_merge(_ht, _dest, _src) \
    do { if(_ht->_merge) { _dest = _ht->_merge(_dest, _src); } } while(0)

hashtable* makeHash(elem_t_cmp _cmp, elem_t_hash _hash);
void freeHash(hashtable *_ht);
elem_t hashGet(hashtable *_ht, elem_t _dt);
void hashPut(hashtable *_ht, elem_t _dt);
void hashDel(hashtable *_ht, elem_t _dt);
void hashIter(hashtable *_ht, elem_t_vis _fn);

#ifdef IDEBUG
typedef struct hash_inspect {
    size_t _size;
    size_t _bkt_siz;
    size_t _bkt_cap;
    size_t _bkt_ll;
    size_t _bkt_tr;
    size_t _bkt_ll_siz[HASHBUCKET_LIST_MAXSZ];
    size_t _bkt_tr_max_sz;
} hash_inspect;
hash_inspect hashInspect(hashtable *_ht);
#endif

#endif