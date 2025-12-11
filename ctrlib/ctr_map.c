#include "ctr_map.h"

#include <string.h>
#include <stdlib.h>

#define setup_strstrpair(pair, key, val) \
    do { pair._key=key; pair._val=val; } while(0)

strstrpair* makestrstrpair(const char *_key, const char *_val) {
    strstrpair *_pair = malloc(sizeof(strstrpair));
    _pair->_key = __strdup(_key);
    _pair->_val = __strdup(_val);
    // DEBUGF("alloc === pair<key,val>=(%p<%s,%s>)", _pair, _pair->_key, _pair->_val);
    return _pair;
}

void freestrstrpair(elem_t _e) {
    strstrpair *_pair = (strstrpair*)get_elem_ptr(_e);
    // DEBUGF("freed === pair<key,val>=(%p<%s,%s>)", _pair, _pair->_key, _pair->_val);
    free(_pair->_key);
    free(_pair->_val);   
    free(_pair);
}

int strstrpaircmp(elem_t _e0, elem_t _e1) {
    strstrpair *_pair0 = get_elem_ptr(_e0);
    strstrpair *_pair1 = get_elem_ptr(_e1);
    return __strcmp(_pair0->_key, _pair1->_key);
}

strstrmap makestrstrmap() {
    strstrmap _map;
    _map._tr = makeRBTree(strstrpaircmp);
    setRBTreeDataFree(_map._tr, freestrstrpair);
    setRBTreeNodeType(_map._tr, TREE_NODE_TYPE_AVL);
    return _map;
}

void freestrstrmap(strstrmap _map) {
    freeRBTree(_map._tr);
}

void strstrmapPut(strstrmap _map, const char *_key, const char *_val) {
    strstrpair *_ptr = makestrstrpair(_key,_val);
    elem_t _em; setup_elem_ptr(_em, _ptr);
    rbTreeInsertData(_map._tr, _em);
}

char* strstrmapGet(strstrmap _map, const char *_key) {
    strstrpair _pair; setup_strstrpair(_pair, _key, NULL);
    elem_t _em; setup_elem_ptr(_em, &_pair);
    _em = rbTreeGetData(_map._tr, _em);
    if(valid_elem_t(_em)){
        _pair = *(strstrpair*)get_elem_ptr(_em);
        return _pair._val;
    }
    return NULL;
}

void strstrmapDel(strstrmap _map, const char *_key) {
    strstrpair _pair; setup_strstrpair(_pair, _key, NULL);
    elem_t _em; setup_elem_ptr(_em, &_pair);
    rbTreeDeleteData(_map._tr, _em);
}

size_t strstrmapSize(strstrmap _map) {
    return __tree_size(_map._tr);
}

void strstrmapIter(strstrmap _map, strstrmapIterFunc _fn) {
    blist *_bl = rbTreeMidTrav(_map._tr);
    biter *_it = makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *_bn=bListNext(_it);
        _bn;
        _bn=bListNext(_it)
    ){
        rb_node *_rbnd = get_elem_ptr(_bn->_data);
        strstrpair *_pair = get_elem_ptr(_rbnd->_data);
        bool _ret = _fn(_pair->_key, _pair->_val);
        if(!_ret){
            break;
        }
    }
    freeBIter(_it);
    freeBList(_bl);
}