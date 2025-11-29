#include "ctr_map.h"

#include <string.h>
#include <stdlib.h>

int __elem_t_str_str_map_cmp(elem_t _e0, elem_t _e1) {
    strstrpair *_pair0 = get_elem_ptr(_e0);
    strstrpair *_pair1 = get_elem_ptr(_e1);
    return strcmp(_pair0->_key, _pair1->_key);
}

strstrmap makestrstrmap() {
    strstrmap _map;
    _map._tr = makeRBTree(__elem_t_str_str_map_cmp);
    return _map;
}

void freestrstrmap(strstrmap _map) {
    freeRBTree(_map._tr);
}

strstrpair* makestrstrpair(char *_key, char *_val) {
    strstrpair *_pair = malloc(sizeof(strstrpair));
    if(_key){
        _pair->_key = malloc(strlen(_key)); strcpy(_pair->_key, _key);
    }
    if(_val){
        _pair->_val = malloc(strlen(_val)); strcpy(_pair->_val, _val);
    }
    return _pair;
}

void strstrmapPut(strstrmap _map, char *_key, char *_val) {
    elem_t _em; 
    setup_elem_ptr(_em, makestrstrpair(_key,_val)); // inserted pair
    rbTreeInsertData(_map._tr, _em);
}

char* strstrmapGet(strstrmap _map, char *_key) {
    elem_t _em;
    strstrpair *_pair = makestrstrpair(_key, NULL); // created pair
    setup_elem_ptr(_em, _pair);
    rb_node *_nd = rbTreeGetData(_map._tr, _em);
    free(_pair); // created pair
    if(!_nd || !valid_elem_t(_nd->_data)){
        return NULL;
    }
    _em = _nd->_data;
    _pair = (strstrpair*)get_elem_ptr(_em);
    char *_ret = _pair->_val;
    return _ret;
}

void strstrmapDel(strstrmap _map, char *_key) {
    elem_t _em;
    strstrpair *_pair = makestrstrpair(_key, NULL); // created pair
    setup_elem_ptr(_em, _pair);
    rb_node *_nd = rbTreeDeleteData(_map._tr, _em);
    free(_pair); // created pair // inserted pair
    // if(_nd)
    //     free(get_elem_ptr(_nd->_data)); // error: double free
}

void strstrmapIter(strstrmap _map, strstrmapIterFunc _fn) {
    blist *_bl = rbTreeMidTrav(_map._tr);
    biter *_it = makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *_bn=bListNext(_it);
        _bn;
        _bn=bListNext(_it)
    ){
        rb_node *_rbnd = bNodeData(_bn).ptr;
        strstrpair *_pair = get_elem_ptr(_rbnd->_data);
        bool _ret = _fn(_pair->_key, _pair->_val);
        if(!_ret){
            break;
        }
    }

    freeBIter(_it);
    freeBList(_bl);
}