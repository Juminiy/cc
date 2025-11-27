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
    elem_t_cmp _elem_cmp; // +optional, if equals return 0, else return !0
} blist;

typedef struct biter{
    bnode *_next;
    int _dire;
} biter;

#define bNodeData(_bn) ((_bn)->_data.uni)
#define bListSize(_bl) ((_bl)->_size)
#define bListSetElemCmp(_bl, _fn) ((_bl)->_elem_cmp = (_fn))

bnode *makeBNode(bnode *_prev, elem_t _data, bnode *_next);
void freeBNode(bnode *_bn);

blist *makeBList();
void freeBList(blist *_bl);
blist *copyBList(blist *_bl);
blist *bListLinkHead(blist *_bl, bnode *_bn);
blist *bListAddHead(blist *_bl, elem_t _dt);
bnode *bListDelHead(blist *_bl);
blist *bListLinkTail(blist *_bl, bnode *_bn);
blist *bListAddTail(blist *_bl, elem_t _dt);
bnode *bListDelTail(blist *_bl);
blist *bListSplice(blist *_bl0, blist *_bl1);

biter *makeBIter(blist *_bl, int _dire);
void freeBIter(biter *_bi);
bnode *bListNext(biter *_bi);

bnode *bListSearch(blist *_bl, elem_t _dt);
bnode *bListIndex(blist *_bl, int _idx);

#define BLIST_ITER_FORWARD +1
#define BLIST_ITER_BAKWARD -1

#endif