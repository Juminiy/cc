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
    
}

btree_node* bTreeNodeInsert(btree_node *_nd, elem_t _dt, btree *_tr) {
    if(!_nd) {
        _nd = makeBTreeNode(_tr);
        return _nd;
    }
}

int bTreeDelete(btree *_tr, elem_t _dt) {

}

elem_t bTreeGet(btree *_tr, elem_t _dt) {

}

btree_node* makeBTreeNode(btree *_tr) {
    btree_node *nd = (btree_node*)malloc(sizeof(btree_node));
    nd->_child = NULL;
    nd->_data = (elem_t*)calloc(_tr->_degree<<1, sizeof(elem_t));
    nd->_dt_sz = 0;
    return nd;
}

// leafNode to internalNode
void bTreeNodeUpgrade(btree_node *_nd, btree *_tr) {
    _nd->_child = (btree_node**)calloc(_tr->_degree<<1, sizeof(btree_node*));
}

void freeBTreeNode(btree_node *_nd, btree *_tr) {
    free(_nd->_child);
    for(size_t _i=0;_i<_nd->_dt_sz;_i++){
        __btree_data_free(_tr, _nd->_data[_i]);
    }
    free(_nd->_data);
    free(_nd);
}

// _degree=t, _dt_sz in [1...t-1,2t-1]
void freeBTreeNodeRec(btree_node *_nd, btree *_tr) {
    for(size_t _i=0;_i<_nd->_dt_sz+1;_i++) {
        freeBTreeNodeRec(_nd->_child[_i], _tr);
    }
    freeBTreeNode(_nd, _tr);
}

typedef struct _btree_node_split {
    btree_node *lnd, *rnd;
    elem_t       mnd_data;
} _btree_node_split;

_btree_node_split bTreeNodeSplit(btree_node *_nd, btree *_tr) {
    _btree_node_split _splt={.lnd=NULL,.rnd=NULL};
    if(_nd->_dt_sz == (_tr->_degree<<1)) { // 6
        size_t mid_i = _nd->_dt_sz/2; // 3
        size_t lsz = mid_i, rsz = _nd->_dt_sz-mid_i-1; // 3, 2 
        _splt.lnd = makeBTreeNode(_tr);
        memcpy(_splt.lnd->_data, _nd->_data, sizeof(elem_t)*lsz);
        _splt.rnd = makeBTreeNode(_tr);
        memcpy(_splt.rnd->_data, _nd->_data+mid_i+1, sizeof(elem_t)*rsz);
        _splt.mnd_data = _nd->_data[mid_i];
    }
    return _splt;
}

// p0|N|p1|W|p2
//       |
// P|Q|R|S|T|U|V
_btree_node_split bTreeNodeInsert(btree_node *_nd, elem_t _dt, btree *_tr) {
    for(size_t _i=0;_i<_nd->_dt_sz;_i++) {
        int cmp_res = _tr->_cmp(_nd->_data[_i], _dt);
        if(cmp_res==0) {
            __btree_data_free(_tr, _nd->_data[_i]);
            _nd->_data[_i] = _dt;
        } else if (cmp_res > 0){
            if(_nd->_child) { // internal node
                bTreeNodeInsert(_nd->_child[_i], _dt, _tr);
            } else { // leaf node
                for(size_t _j=_nd->_dt_sz;_j>_i;_j--) {
                    _nd->_data[_j] = _nd->_data[_j-1];
                }
                _nd->_dt_sz++;
                _nd->_data[_i] = _dt;
            }
            
        }
    }
}

elem_t bTreeNodeMerge() {

}