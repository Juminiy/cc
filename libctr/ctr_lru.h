#ifndef CTR_LRU_H
#define CTR_LRU_H

#include "ctr_rbtree.h"
#include "ctr_blist.h"

// internal api
typedef struct bnode_map {
    rb_tree *_tr;     // rb_tree<bnode*>
    elem_t_cmp _cmp;  // raw data cmp
} bnode_map;

bnode_map makeBNodeMap(elem_t_cmp _cmp);
void freeBNodeMap(bnode_map _map);
void bNodeMapPut(bnode_map _map, bnode* _nd);
void bNodeMapDel(bnode_map _map, elem_t _dt);
bnode* bNodeMapGetNode(bnode_map _map, elem_t _dt);
elem_t bNodeMapGetData(bnode_map _map, elem_t _dt);

// lru api
typedef struct lru_t {
    bnode_map _map; // map<bnode*>
    blist   *_bl;   // blist<_data>

    size_t _cap;    // max accommodation
    elem_t_cmp _cmp; // raw data cmp
    elem_t_free _free;
    elem_t_merge _merge;
} lru_t;

#define lruTSize(_t) (bListSize(_t->_bl))
#define lruTCap(_t) (_t->_cap)
#define lruTSetElemFree(_t, _f) \
    do { _t->_free=_f; bListSetElemFree(_t->_bl, _f); } while(0)
#define lruTSetElemMerge(_t, _f) (_t->_merge=_f) 
#define __lrut_free(_t, _dt) \
    do { if(_t->_free) { _t->_free(_dt); } } while(0)
#define __lrut_merge(_t, _dest, _src) \
    do { if(_t->_merge) { _dest = _t->_merge(_dest, _src); } } while(0)

lru_t* makeLRUT(elem_t_cmp _cmp, size_t _cap);
void freeLRUT(lru_t *_t);
elem_t lruTGet(lru_t *_t, elem_t _dt);
void lruTPut(lru_t *_t, elem_t _dt);

#endif