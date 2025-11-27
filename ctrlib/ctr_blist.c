#include "ctr_blist.h"
#include <stdlib.h>

#define __link_bnode(_n0, _n1) \
    do { if(_n0) {_n0->_next=_n1;} if(_n1) {_n1->_prev=_n0;} } while(0)
#define __unlink_prev(_n0) \
    do { if(_n0->_prev) {_n0->_prev->_next=NULL; _n0->_prev=NULL;} } while(0)
#define __unlink_next(_n0) \
    do { if(_n0->_next) {_n0->_next->_prev=NULL; _n0->_next=NULL;} } while(0)

bnode *makeBNode(bnode *_prev, elem_t _data, bnode *_next) {
    bnode *_bn = (bnode*)malloc(sizeof(bnode));
    __link_bnode(_prev, _bn);
    _bn->_data = _data;
    __link_bnode(_bn, _next);
    return _bn;
}

void freeBNode(bnode *_bn) {
    __unlink_prev(_bn);
    __unlink_next(_bn);
    free(_bn);
}

blist *makeBList() {
    blist *_bl = (blist*)malloc(sizeof(blist));
    _bl->_head = NULL;
    _bl->_tail = NULL;
    _bl->_size = 0;
    return _bl;
}

void freeBList(blist *_bl) {
    for(bnode *_bn=_bl->_head; _bn; ) {
        bnode *_nxt = _bn->_next;
        freeBNode(_bn);
        _bn = _nxt;
    }
    free(_bl);
}

blist *bListLinkHead(blist *_bl, bnode *_bn) {
    __link_bnode(_bn, _bl->_head);
    _bl->_head = _bn;
    if(_bl->_size==0) {
        _bl->_tail = _bn;
    }
    _bl->_size++;
    return _bl;
}

blist *bListAddHead(blist *_bl, elem_t _dt) {
    return bListLinkHead(_bl, makeBNode(NULL,_dt,NULL));
}

bnode *bListDelHead(blist *_bl) {
    bnode *_bn = _bl->_head;
    if(_bn) {
        _bl->_head = _bn->_next;
        __unlink_next(_bn);
        _bl->_size--;
    }
    if(_bl->_size==0){
        _bl->_tail = NULL;
    }
    return _bn;
}

blist *bListLinkTail(blist *_bl, bnode *_bn) {
    __link_bnode(_bl->_tail, _bn);
    _bl->_tail = _bn;
    if(_bl->_size==0) {
        _bl->_head = _bn;
    }
    _bl->_size++;
    return _bl;
}

blist *bListAddTail(blist *_bl, elem_t _dt) {
    return bListLinkTail(_bl, makeBNode(NULL,_dt,NULL));
}

bnode *bListDelTail(blist *_bl) {
    bnode *_bn = _bl->_tail;
    if(_bn) {
        _bl->_tail = _bn->_prev;
        __unlink_prev(_bn);
        _bl->_size--;
    }
    if(_bl->_size==0){
        _bl->_head = NULL;
    }
    return _bn;
}

blist *bListSplice(blist *_bl0, blist *_bl1) {
    bnode *_bn0=NULL, *_bn1=NULL;
    if(_bl0 && _bl0->_tail)
        _bn0 = _bl0->_tail;
    if(_bl1 && _bl1->_head)
        _bn1 = _bl1->_head;
    __link_bnode(_bn0, _bn1);
    blist *_bl = makeBList();
    _bl->_head = _bn0 ? _bl0->_head:_bl1->_head;
    _bl->_tail = _bn1 ? _bl1->_tail:_bl0->_tail;
    _bl->_size = _bl0->_size+_bl1->_size;
    return _bl;
}

biter *makeBIter(blist *_bl, int _dire) {
    biter * _bi = (biter*)malloc(sizeof(biter));
    _bi->_next = _dire == BLIST_ITER_FORWARD ? 
        _bl->_head : _bl->_tail;
    _bi->_dire = _dire;
    return _bi;
}

void freeBIter(biter *_bi) {
    free(_bi);
}

bnode *bListNext(biter *_bi) {
    bnode *cur = _bi->_next;
    if (cur)
        _bi->_next = _bi->_dire == BLIST_ITER_FORWARD ?
            cur->_next : cur->_prev;
    return cur;
}

blist *copyBList(blist *_bl) {
    blist *cpy=makeBList();
    biter *itr=makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *nd=bListNext(itr);nd;nd=bListNext(itr)) {
        bListAddTail(cpy, nd->_data);
    }
    freeBIter(itr);
    return cpy;
}

bnode *bListSearch(blist *_bl, elem_t _dt) {
    biter *bi=makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *bn=bListNext(bi);bn;bn=bListNext(bi)) {
        if(_bl->_elem_cmp(bn->_data, _dt)==0){
            return bn;
        }
    }
    freeBIter(bi);
    return NULL;
}

// -1 -> 0
// -2 -> 1
bnode *bListIndex(blist *_bl, int _idx) {
    biter *bi=makeBIter(_bl, _idx>=0 ? BLIST_ITER_FORWARD:BLIST_ITER_BAKWARD);
    _idx = _idx >= 0 ? _idx : -_idx-1;
    for(bnode *bn=bListNext(bi);bn&&_idx>=0;bn=bListNext(bi)) {
        if(_idx==0){
            return bn;
        }
        _idx--;
    }
    freeBIter(bi);
    return NULL;
}