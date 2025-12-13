#include "ctr_lru.h"

lru_t* makeLRUT(elem_t_cmp _cmp, size_t _cap) {
    lru_t *_t = (lru_t*)malloc(sizeof(lru_t));
    _t->_map = makeBNodeMap(_cmp);
    _t->_bl = makeBList();
    _t->_cap = _cap;
    _t->_cmp = _cmp;
    _t->_free = NULL;
    _t->_merge = NULL;
    return _t;
}

void freeLRUT(lru_t *_t) {
    freeBList(_t->_bl);
    freeBNodeMap(_t->_map);
    free(_t);
}

elem_t lruTGet(lru_t *_t, elem_t _dt) {
    bnode *nd = bNodeMapGetNode(_t->_map, _dt);
    elem_t em={.tag=ELEM_T_INVALID};
    if(nd) {
        nd = bListDeleteNode(_t->_bl, nd);
        nd = bListLinkHead(_t->_bl, nd);
        em = nd->_data;
    }
    return em;
}

void lruTPut(lru_t *_t, elem_t _dt) {
    bnode *nd = bNodeMapGetNode(_t->_map, _dt);
    if(nd) {
        nd = bListDeleteNode(_t->_bl, nd);
        nd = bListLinkHead(_t->_bl, nd);
        __lrut_free(_t, nd->_data);
    } else {
        nd = bListAddHead(_t->_bl, _dt);
    }
    nd->_data = _dt;
    bNodeMapPut(_t->_map, nd);

    if(lruTSize(_t) > _t->_cap) {
        nd = bListUnlinkTail(_t->_bl);
        bNodeMapDel(_t->_map, nd->_data);
        freeBNode(nd, _t->_bl);
    }
}

typedef struct _bnode_wrap {
    bnode *_nd;
    elem_t_cmp _cmp; // raw data cmp
} _bnode_wrap;

_bnode_wrap* make_bnode_wrap(bnode_map _map, bnode *_bn) {
    _bnode_wrap* _w = (_bnode_wrap*)malloc(sizeof(_bnode_wrap));
    _w->_nd = _bn;
    _w->_cmp = _map._cmp;
    return _w;
}

int __elem_bnode_wrap_cmp(elem_t e0, elem_t e1) {
    _bnode_wrap *w0 = (_bnode_wrap*)get_elem_ptr(e0);
    _bnode_wrap *w1 = (_bnode_wrap*)get_elem_ptr(e1);
    return w0->_cmp(w0->_nd->_data, w1->_nd->_data);
}

void __elem_bnode_wrap_free(elem_t e0) {
    _bnode_wrap *w0 = (_bnode_wrap*)get_elem_ptr(e0);
    free(w0);
}

bnode_map makeBNodeMap(elem_t_cmp _cmp) {
    bnode_map _map;
    _map._tr = makeRBTree(__elem_bnode_wrap_cmp);
    _map._cmp = _cmp;
    setRBTreeNodeType(_map._tr, TREE_NODE_TYPE_AVL);
    setRBTreeDataFree(_map._tr, __elem_bnode_wrap_free);
    return _map;
}

void freeBNodeMap(bnode_map _map) {
    freeRBTree(_map._tr);
}

void bNodeMapPut(bnode_map _map, bnode* _nd) {
    elem_t em; 
    setup_elem_ptr(em, make_bnode_wrap(_map, _nd));
    rbTreeInsertData(_map._tr, em);
}

void bNodeMapDel(bnode_map _map, elem_t _dt) {
    bnode bn={._data=_dt,._prev=NULL,._next=NULL};
    _bnode_wrap _wp = {._cmp=_map._cmp,._nd=&bn};
    elem_t em; setup_elem_ptr(em, &_wp);
    rbTreeDeleteData(_map._tr, em);
}

bnode* bNodeMapGetNode(bnode_map _map, elem_t _dt) {
    bnode bn={._data=_dt,._prev=NULL,._next=NULL};
    _bnode_wrap _wp = {._cmp=_map._cmp,._nd=&bn};
    elem_t em; setup_elem_ptr(em, &_wp);
    elem_t em_x=rbTreeGetData(_map._tr, em);
    if(valid_elem_t(em_x)){
        _bnode_wrap *_wp_x = (_bnode_wrap*)get_elem_ptr(em_x);
        return _wp_x->_nd;
    }
    return NULL;
}

elem_t bNodeMapGetData(bnode_map _map, elem_t _dt) {
    elem_t em={.tag=ELEM_T_INVALID};
    bnode *nd = bNodeMapGetNode(_map, _dt);
    return nd?nd->_data:em;
}