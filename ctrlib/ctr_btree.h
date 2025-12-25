#ifndef CTR_BTREE_H
#define CTR_BTREE_H

#include <stddef.h>
#include "ctr_elemt.h"
#include "ctr_barray.h"

typedef struct btree_node {
    btree_node **_child; // []<btree_node*>
    barray        _data; // barray<elem>
} btree_node;

typedef struct btree {
    size_t      _degree;
    size_t      _height;
    size_t      _size;
    btree_node *_root;

    elem_t_cmp _cmp;
    elem_t_free _free;

    int __opt_tag;
} btree;

typedef struct btree_node_split {
    btree_node *lnd, *rnd;
    elem_t     mnd_data;
} btree_node_split;

#define bTreeSetFree(tr, f) (tr->_free=f)
#define bTreeHeight(tr) (tr->_height)
#define bTreeSize(tr) (tr->_size)

#define __btree_data_free(tr, dt) \
    do { if(tr->_free) { tr->_free(dt); } } while(0)

#define bNodeElemLen(nd) (bArrayLen(nd->_data))
#define bNodeChildLen(nd) (bArrayLen(nd->data)+1)

btree* makeBTree(size_t _d, elem_t_cmp _cmp);
void freeBTree(btree *_tr);

int bTreeInsert(btree *_tr, elem_t _dt);
int bTreeDelete(btree *_tr, elem_t _dt);
elem_t bTreeGet(btree *_tr, elem_t _dt);
void bTreeIter(btree *_tr, elem_t_vis _f);

#endif