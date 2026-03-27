#ifndef CTR_BTREE_H
#define CTR_BTREE_H

#include <stddef.h>
#include "ctr_elemt.h"
#include "ctr_barray.h"

typedef struct btree_node {
    barray _child; // barray<btree_node*>
    barray _data;  // barray<elem>
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

#define bTreeSetFree(tr, f) (tr->_free=f)
#define bTreeHeight(tr) (tr->_height)
#define bTreeSize(tr) (tr->_size)

#define __btree_data_free(tr, dt) \
    do { if(tr->_free) { tr->_free(dt); } } while(0)

#define bNodeElemLen(nd) (bArrayLen(nd->_data))
#define bNodeElemAt(nd, idx) bArrayAt(nd->_data, idx)
#define bNodeElemInsert(nd, idx, dt) \
    do { nd->_data = bArrayInsert(nd->_data, idx, dt); } while(0)
#define bNodeElemAppend(nd, dt) \
    do { nd->_data = bArrayAppend(nd->_data, dt); } while(0)
#define bNodeChildLen(nd) (bArrayLen(nd->_child))
#define bNodeChildAt(nd, idx) cast_elem_typ(bArrayAt(nd->_child, idx), btree_node*)
#define bNodeChildInsert(nd, idx, chnd) \
    do { nd->_child = bArrayInsert(nd->_child, idx, varElemPtr(chnd)); } while(0)
#define bNodeChildAppend(nd, chnd) \
    do { nd->_child = bArrayAppend(nd->_child, varElemPtr(chnd)); } while(0)

btree* makeBTree(size_t _d, elem_t_cmp _cmp);
void freeBTree(btree *_tr);

int bTreeInsert(btree *_tr, elem_t _dt);
int bTreeDelete(btree *_tr, elem_t _dt);
elem_t bTreeGet(btree *_tr, elem_t _dt);
void bTreeIter(btree *_tr, elem_t_vis _f);

typedef struct btree_node_split {
    btree_node *lnd, *rnd;
    elem_t     mdata;
    bool       valid;
} btree_node_split;

#define setup_btree_node_split(_lnd,_rnd,_mdata,_valid) \
    ({ btree_node_split _split; \
        _split.lnd=_lnd,_split.rnd=_rnd,_split.mdata=_mdata; \
        _split.valid=_valid; \
        _split; })
#define varBTreeNodeSplit(_lnd,_rnd,_mdata) setup_btree_node_split(_lnd,_rnd,_mdata,true)
#define varBTreeNodeSplitNone() setup_btree_node_split(NULL,NULL,varElemNone(),false)

#endif