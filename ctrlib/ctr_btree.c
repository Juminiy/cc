#include "ctr_btree.h"

#include <stdlib.h>

btree* makeBTree(size_t _d, elem_t_cmp _cmp) {
    btree *_tr = (btree*)malloc(sizeof(btree));
    _tr->_cmp = _cmp;
    _tr->_degree = _d;
    _tr->_free = NULL;

    _tr->_height = 0;
    _tr->_size = 0;
    _tr->_root = NULL;

    return _tr;
}

void freeBTree(btree *_tr) {
    freeBTreeNodeRec(_tr->_root, _tr);
    free(_tr);
}

int bTreeInsert(btree *_tr, elem_t _dt) {
    _tr->__opt_tag = ELEM_INSERT_CREATED;

    _tr->_size += _tr->__opt_tag==ELEM_INSERT_CREATED;
}

int bTreeDelete(btree *_tr, elem_t _dt) {
    _tr->__opt_tag = ELEM_OPT_NONE;

    _tr->_size -= _tr->__opt_tag==ELEM_DELETE_REMOVED;
}

elem_t bTreeGet(btree *_tr, elem_t _dt) {

}

btree_node* makeBTreeNode(btree *_tr, barray _dt, btree_node **_ch) {
    btree_node *nd = (btree_node*)malloc(sizeof(btree_node));
    nd->_data = _dt;
    nd->_child = _ch;
    return nd;
}

void freeBTreeNode(btree_node *_nd, btree *_tr) {
    free(_nd->_child);
    freeBArray(_nd->_data);
    free(_nd);
}

void freeBTreeNodeRec(btree_node *_nd, btree *_tr) {
    for(size_t _i=0;_i<bArrayLen(_nd->_data);_i++) {
        freeBTreeNodeRec(_nd->_child[_i], _tr);
    }
    freeBTreeNode(_nd, _tr);
}

btree_node_split bTreeNodeInsert(btree_node *_nd, elem_t _dt, btree *_tr) {
    
}