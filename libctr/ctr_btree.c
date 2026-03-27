#include "ctr_btree.h"
#include "ctr_pair.h"

#include <stdlib.h>

btree_node* makeBTreeNode(btree *_tr, barray _dt, barray _ch);
void freeBTreeNode(btree_node *_nd, btree *_tr);
void freeBTreeNodeRec(btree_node *_nd, btree *_tr);
btree_node_split bTreeNodeInsert(btree_node *_nd, elem_t _dt, btree *_tr);
pair bTreeNodeGetGEIndex(btree_node *_nd, elem_t _dt, btree *_tr);
void bTreeNodeIter(btree_node *nd, elem_t_vis _f);

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
    btree_node_split _split = bTreeNodeInsert(_tr->_root, _dt, _tr);
    if(_split.valid) {
        btree_node *new_root = makeBTreeNode(_tr, makeBArray(0,_tr->_degree), makeBArray(0,_tr->_degree));
        bNodeElemAppend(new_root, _split.mdata);
        if(_split.lnd&&_split.rnd) {
            bNodeChildAppend(new_root, _split.lnd);
            bNodeChildAppend(new_root, _split.rnd);
        }
        freeBTreeNode(_tr->_root, _tr);
        _tr->_root = new_root;
        _tr->_height++;
    }
    _tr->_size += _tr->__opt_tag==ELEM_INSERT_CREATED;
}

// todo: fix
int bTreeDelete(btree *_tr, elem_t _dt) {
    _tr->__opt_tag = ELEM_OPT_NONE;

    _tr->_size -= _tr->__opt_tag==ELEM_DELETE_REMOVED;
}

elem_t bTreeGet(btree *_tr, elem_t _dt) {
    elem_t em={.tag=ELEM_T_INVALID};
    for(btree_node *cur=_tr->_root;cur;) {
        pair pr = bTreeNodeGetGEIndex(cur, _dt, _tr);
        if(pairGet1(pr,bool)) {
            return bNodeElemAt(cur, pairGet0(pr,int));
        } else if(bNodeChildLen(cur)) {
            cur = bNodeChildAt(cur, pairGet0(pr,int));
        } else {
            return em;
        }
    }
    return em;
}

void bTreeIter(btree *_tr, elem_t_vis _f) {
    bTreeNodeIter(_tr->_root, _f);
}

btree_node* makeBTreeNode(btree *_tr, barray _dt, barray _ch) {
    btree_node *nd = (btree_node*)malloc(sizeof(btree_node));
    nd->_data = _dt;
    nd->_child = _ch;
    return nd;
}

void freeBTreeNode(btree_node *_nd, btree *_tr) {
    if(_nd){
        freeBArray(_nd->_child);
        for(size_t i=0;i<bNodeElemLen(_nd);i++){
            __btree_data_free(_tr, bNodeElemAt(_nd, i));
        }
        freeBArray(_nd->_data);
        free(_nd);
    }
}

void freeBTreeNodeRec(btree_node *_nd, btree *_tr) {
    if(!_nd)
        return;
    for(size_t _i=0;_i<bNodeChildLen(_nd);_i++) {
        freeBTreeNodeRec(bNodeChildAt(_nd, _i), _tr);
    }
    freeBTreeNode(_nd, _tr);
}

btree_node_split bTreeNodeInsert(btree_node *_nd, elem_t _dt, btree *_tr) {
    if(!_nd) {
        return varBTreeNodeSplit(NULL,NULL,_dt);
    }

    pair pr = bTreeNodeGetGEIndex(_nd, _dt, _tr);
    int gtIndex = pairGet0(pr,int);
    if(pairGet1(pr,bool)) {
        __btree_data_free(_tr, bNodeElemAt(_nd, gtIndex));
        bArraySet(_nd->_data, gtIndex, _dt);
        _tr->__opt_tag = ELEM_INSERT_REPLACED;
        return varBTreeNodeSplitNone();
    }

    if(bNodeChildLen(_nd)==0) {
        bNodeElemInsert(_nd, gtIndex, _dt);
    } else {
        btree_node *gtNode = bNodeChildAt(_nd, gtIndex);
        btree_node_split _split = bTreeNodeInsert(gtNode, _dt, _tr);
        if(_split.valid) {
            bNodeChildInsert(_nd, gtIndex, _split.lnd);
            bArraySet(_nd->_child, gtIndex+1, varElemPtr(_split.rnd));
            bNodeElemInsert(_nd, gtIndex, _split.mdata);
            freeBTreeNode(gtNode, _tr);
        }
    }

    if(bArrayLen(_nd->_data)==(_tr->_degree<<1)) {
        barray ldt, rdt; size_t dtsz=bArrayLen(_nd->_data);
        ldt = bArraySlice(_nd->_data, 0, _tr->_degree, _tr->_degree<<1);
        rdt = bArraySlice(_nd->_data, _tr->_degree+1, dtsz, _tr->_degree<<1);

        barray lch, rch; size_t chsz=0;
        if((chsz=bNodeChildLen(_nd)) > 0){
            lch = bArraySlice(_nd->_child, 0, _tr->_degree+1, _tr->_degree<<1);
            rch = bArraySlice(_nd->_child, _tr->_degree+1, chsz, _tr->_degree<<1);
        } else {
            lch = makeBArray(0,0);
            rch = makeBArray(0,0);
        }
        // freeBTreeNode(_nd, _tr);
        return varBTreeNodeSplit(
            makeBTreeNode(_tr,ldt,lch),
            makeBTreeNode(_tr,rdt,rch),
            bNodeElemAt(_nd, _tr->_degree)
        );
    }

    return varBTreeNodeSplitNone();
}

// @return <geIndex,equal>
pair bTreeNodeGetGEIndex(btree_node *_nd, elem_t _dt, btree *_tr) {
    for(int i=0;i<bNodeElemLen(_nd);i++){
        int cmp_res = _tr->_cmp(bNodeElemAt(_nd, i), _dt);
        if(cmp_res == 0) {
            return varPair(i,true);
        } else if (cmp_res > 0) {
            return varPair(i,false);
        }
    }
    return varPair(bNodeElemLen(_nd),false);
}

void bTreeNodeIter(btree_node *nd, elem_t_vis _f) {
    if(!nd)
        return;
    if(bNodeChildLen(nd)>0){
        for(size_t idx=0;idx<bNodeElemLen(nd);idx++){
            bTreeNodeIter(bNodeChildAt(nd, idx),_f);
            if(!_f(bNodeElemAt(nd,idx))){
                return;
            }
        }
        bTreeNodeIter(bNodeChildAt(nd, bNodeElemLen(nd)),_f);
    } else {
        for(size_t idx=0;idx<bNodeElemLen(nd);idx++){
            if(!_f(bNodeElemAt(nd,idx))){
                return;
            }
        }
    }
}