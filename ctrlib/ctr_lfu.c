#include "ctr_lfu.h"

int __elem_cmp_lfunode_priority(elem_t _e0, elem_t _e1) {
    lfu_node *l0=(lfu_node*)get_elem_ptr(_e0);
    lfu_node *l1=(lfu_node*)get_elem_ptr(_e1);
    if(l0->_refs==l1->_refs) {
        return -(l0->_ts - l1->_ts);
    }
    return -(l0->_refs - l1->_refs);
}

int __elem_cmp_lfunode_raw(elem_t _e0, elem_t _e1) {
    lfu_node *l0=(lfu_node*)get_elem_ptr(_e0);
    lfu_node *l1=(lfu_node*)get_elem_ptr(_e1);
    return l0->_cmp(l0->_data, l1->_data);
}

void __elem_swap_lfunode(elem_t *_e0, elem_t *_e1) {
    lfu_node *l0=(lfu_node*)get_elem_ptr((*_e0));
    lfu_node *l1=(lfu_node*)get_elem_ptr((*_e1));
    __swap_typed(size_t, l0->_idx, l1->_idx);
    __elem_swap_normalized(_e0, _e1);
}

lfu_t* makeLFUT(elem_t_cmp _cmp, size_t _cap) {
    lfu_t *_t = (lfu_t*)malloc(sizeof(lfu_t));
    _t->_cap = _cap;
    _t->_hp = makeBHeap(__elem_cmp_lfunode_priority, __elem_swap_lfunode);
    _t->_map = makeLFUMap(_cmp);
    _t->_ts = 0;

    _t->_cmp = _cmp;
    _t->_free = NULL;
    _t->_merge = NULL;
    return _t;
}

void freeLFUT(lfu_t *_t) {
    freeBHeap(_t->_hp);
    freeLFUMap(_t->_map);
    free(_t);
}

elem_t lfuTGet(lfu_t *_t, elem_t _dt) {
    _t->_ts++;

    elem_t em={.tag=ELEM_T_INVALID};
    lfu_node *nd = lfuMapGetNode(_t->_map, _dt);
    if(nd) {
        _t->_hp = bHeapRemove(_t->_hp, nd->_idx);
        nd->_refs++;
        nd->_ts = _t->_ts;
        setup_elem_ptr(em, nd); 
        _t->_hp = bHeapPush(_t->_hp, em);
        em = nd->_data;
    }
    return em;
}

void lfuTPut(lfu_t *_t, elem_t _dt) {
    _t->_ts++;

    lfu_node *nd = lfuMapGetNode(_t->_map, _dt);
    if(nd) {
        nd->_refs++;
        nd->_ts = _t->_ts;
        __lfut_free(_t, nd->_data);
        nd->_data = _dt;
        _t->_hp = bHeapRemove(_t->_hp, nd->_idx);
    } else {
        if (lfuTSize(_t) == _t->_cap) {
            nd = (lfu_node*)get_elem_ptr(bHeapTop(_t->_hp));
            _t->_hp = bHeapPop(_t->_hp);
            lfuMapDel(_t->_map, nd->_data);
            freeLFUNode(nd, _t);
        }
        nd = makeLFUNode(_dt, _t);
    }
    lfuMapPut(_t->_map, nd);
    elem_t em; setup_elem_ptr(em, nd); 
    _t->_hp = bHeapPush(_t->_hp, em);
}

lfu_node* makeLFUNode(elem_t _dt, lfu_t *_t) {
    lfu_node *nd = (lfu_node*)malloc(sizeof(lfu_node));
    nd->_data = _dt;
    nd->_idx = __min_(lfuTSize(_t), _t->_cap-1);
    nd->_refs = 1;
    nd->_ts = _t->_ts;
    nd->_cmp = _t->_cmp;
    return nd;
}

void freeLFUNode(lfu_node *_nd, lfu_t *_t) {
    __lfut_free(_t, _nd->_data);
    free(_nd);
}

lfu_map makeLFUMap(elem_t_cmp _cmp) {
    lfu_map _map;
    _map._tr = makeRBTree(__elem_cmp_lfunode_raw);
    _map._cmp = _cmp;
    _map._free = NULL;
    setRBTreeNodeType(_map._tr, TREE_NODE_TYPE_AVL);
    return _map;
}

void freeLFUMap(lfu_map _map) {
    barray nds = rbTree2Array(_map._tr);
    for(size_t i=0;i<bArrayLen(nds);i++) {
        lfu_node *nd=(lfu_node*)get_elem_ptr(bArrayAt(nds, i));
        if(_map._free)
            _map._free(nd->_data);
        free(nd);
    }
    freeBArray(nds);
    // setRBTreeDataFree(_map._tr, freeLFUNode);
    freeRBTree(_map._tr);
}

void lfuMapPut(lfu_map _map, lfu_node *_nd) {
    elem_t em; setup_elem_ptr(em, _nd);
    rbTreeInsertData(_map._tr, em);
}

void lfuMapDel(lfu_map _map, elem_t _dt) {
    lfu_node nd={._cmp=_map._cmp,._data=_dt};
    elem_t em; setup_elem_ptr(em, &nd);
    rbTreeDeleteData(_map._tr, em);
}

lfu_node* lfuMapGetNode(lfu_map _map, elem_t _dt) {
    lfu_node nd={._cmp=_map._cmp,._data=_dt};
    elem_t em; setup_elem_ptr(em, &nd);
    em = rbTreeGetData(_map._tr, em);
    if(valid_elem_t(em)) {
        return (lfu_node*)get_elem_ptr(em);
    }
    return NULL;
}