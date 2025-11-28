#include "ctr_bstack.h"

#include <stdlib.h>

bstack* makeBStack() {
    bstack *_bstk = (bstack*)malloc(sizeof(bstack));
    _bstk->_bl = makeBList();
    return _bstk;
}

void freeBStack(bstack *_bstk) {
    freeBList(_bstk->_bl);
    free(_bstk);
}

void bStackPush(bstack *_bstk, elem_t _el) {
    bListAddTail(_bstk->_bl, _el);
}

bool bStackEmpty(bstack *_bstk) {
    return bListSize(_bstk->_bl)==0;
}

elem_t bStackTop(bstack *_bstk) {
    elem_t elem_val = {.tag = ELEM_T_INVALID};
    if(!bStackEmpty(_bstk)){
        elem_val = _bstk->_bl->_tail->_data;
    }
    return elem_val;
}

elem_t bStackPop(bstack *_bstk) {
    elem_t elem_val = {.tag = ELEM_T_INVALID};
    if(!bStackEmpty(_bstk)){
        bnode *bn = bListDelTail(_bstk->_bl);
        elem_val = bn->_data;
    }
    return elem_val;
}