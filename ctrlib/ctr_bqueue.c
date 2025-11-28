#include "ctr_bqueue.h"

#include <stdlib.h>

bqueue* makeBQueue() {
    bqueue *bq=(bqueue*)malloc(sizeof(bqueue));
    bq->_bl = makeBList();
    return bq;
}

void freeBQueue(bqueue *_bq) {
    freeBList(_bq->_bl);
    free(_bq);
}

void bQueuePush(bqueue *_bq, elem_t _el) {
    bListAddTail(_bq->_bl, _el);
}

bool bQueueEmpty(bqueue *_bq) {
    return bListSize(_bq->_bl)==0;
}

elem_t bQueueFront(bqueue *_bq) {
    elem_t elem_val = {.tag = ELEM_T_INVALID};
    if(!bQueueEmpty(_bq)){
        elem_val = _bq->_bl->_head->_data;
    }
    return elem_val;
}

elem_t bQueueBack(bqueue *_bq) {
    elem_t elem_val = {.tag = ELEM_T_INVALID};
    if(!bQueueEmpty(_bq)){
        elem_val = _bq->_bl->_tail->_data;
    }
    return elem_val;
}

elem_t bQueuePop(bqueue *_bq) {
    elem_t elem_val = {.tag = ELEM_T_INVALID};
    if(!bQueueEmpty(_bq)){
        bnode *bn = bListDelHead(_bq->_bl);
        elem_val = bn->_data;
    }
    return elem_val;
}