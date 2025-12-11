#ifndef CTR_BLIST_H
#define CTR_BLIST_H

// bidirectional linked list

#include <stddef.h>
#include "ctr_elemt.h"

typedef struct bnode{
    struct bnode *_prev, *_next;
    elem_t _data;
} bnode;

typedef struct blist{
    bnode *_head, *_tail;
    size_t _size;

    elem_t_cmp _elem_cmp;     // +optional
    elem_t_free _elem_free;   // +optional
    elem_t_merge _elem_merge; // +optional
} blist;

#define bListSize(_bl) ((_bl)->_size)
#define bListSetElemCmp(_bl, _fn) ((_bl)->_elem_cmp = (_fn))
#define bListSetElemFree(_bl, _fn) ((_bl)->_elem_free = (_fn))
#define bListSetElemMerge(_bl, _fn) ((_bl)->_elem_merge = (_fn))
#define __blist_data_free(_bl, _dt) \
    do { if(_bl->_elem_free) { _bl->_elem_free(_dt); } } while(0)
#define __blist_data_merge(_bl, _dest, _src) \
    do { if(_bl->_elem_merge) { _dest = _bl->_elem_merge(_dest, _src); } } while(0)
#define __blist_copy_fn(_dest, _src) \
    do { _dest->_elem_cmp=_src->_elem_cmp; \
        _dest->_elem_free=_src->_elem_free; \
        _dest->_elem_merge=_src->_elem_merge; \
    } while(0)

blist *makeBList();
void freeBList(blist *_bl);
blist *copyBList(blist *_bl);
blist *bListSplice(blist *_bl0, blist *_bl1);
blist *bListRange(blist *_bl, size_t _lr, size_t _rr);
// via _elem_cmp
bnode *bListSearch(blist *_bl, elem_t _dt);
bnode *bListIndex(blist *_bl, int _idx);

// safe api
bnode *bListAddHead(blist *_bl, elem_t _dt);
elem_t bListDelHead(blist *_bl);
bnode *bListAddTail(blist *_bl, elem_t _dt);
elem_t bListDelTail(blist *_bl);

// unsafe api
// blist api
bnode *bListLinkHead(blist *_bl, bnode *_bn);
bnode *bListUnlinkHead(blist *_bl);
bnode *bListLinkTail(blist *_bl, bnode *_bn);
bnode *bListUnlinkTail(blist *_bl);
// bnode api
bnode *makeBNode(bnode *_prev, elem_t _data, bnode *_next);
void freeBNode(bnode *_bn, blist *_bl);
bnode *bListInsertBefore(blist *_bl, bnode *_bn, bnode *_next);
bnode *bListInsertAfter(blist *_bl, bnode *_bn, bnode *_prev);
bnode *bListDeleteNode(blist *_bl, bnode *_bn);

typedef struct biter{
    bnode *_next;
    int _dire;
} biter;

// iterator api
biter *makeBIter(blist *_bl, int _dire);
void freeBIter(biter *_bi);
bnode *bListNext(biter *_bi);

#define BLIST_ITER_FORWARD +1
#define BLIST_ITER_BAKWARD -1

#endif