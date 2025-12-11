#ifndef CTR_LFU_H
#define CTR_LFU_H

#include "ctr_rbtree.h"
#include "ctr_bheap.h"

typedef struct lfu_node {
	size_t _ts;   // last visit clock
	size_t _refs; // ref counts
	size_t _idx;  // index in heap
	elem_t _data; // raw data
	elem_t_cmp _cmp; // raw data cmp
} lfu_node;

typedef struct lfu_map {
	rb_tree *_tr; // rb_tree<lfu_node*>
} lfu_map;

typedef struct lfu_t {
	lfu_map _map;
	bheap _hp;   // bheap<lfu_node*>
	size_t _ts;  // lastes clock
	size_t _cap; // max accommodation

	elem_t_cmp _cmp;
  elem_t_free _free;
  elem_t_merge _merge;
} lfu_t;

#define lfuTSize(_t) (__tree_size(_t->_tr))
#define lfuTCap(_t) (_t->_cap)
#define lfuTSetElemFree(_t, _f) \
    do { _t->_free=_f; } while(0)
#define lfuTSetElemMerge(_t, _f) (_t->_merge=_f) 
#define __lfut_free(_t, _dt) \
    do { if(_t->_free) { _t->_free(_dt); } } while(0)
#define __lfut_merge(_t, _dest, _src) \
    do { if(_t->_merge) { _dest = _t->_merge(_dest, _src); } } while(0)

// lru api
lfu_t* makeLFUT(elem_t_cmp _cmp, size_t _cap);
void freeLFUT(lfu_t *_t);
elem_t lfuTGet(lfu_t *_t, elem_t _dt);
void lfuTPut(lfu_t *_t, elem_t _dt);

// internal api
lfu_node* makeLFUNode(lfu_t *_t);
void freeLFUNode(lfu_node *_nd);

lfu_map makeLFUMap(elem_t_cmp _cmp);
void freeLFUMap(lfu_map _map);
void lfuMapPut(lfu_map _map, lfu_node *_nd);
void lfuMapDel(lfu_map _map, elem_t _dt);
lfu_node lfuMapGetNode(lfu_map _map, elem_t _dt);

#endif