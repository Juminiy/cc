#include "ctr_blist.h"
#include <stdlib.h>

#define __link_bnode(_n0, _n1) \
    do { if(_n0) {_n0->_next=_n1;} if(_n1) {_n1->_prev=_n0;} } while(0)
#define __unlink_prev(_n0) \
    do { if(_n0 && _n0->_prev) {_n0->_prev->_next=NULL; _n0->_prev=NULL;} } while(0)
#define __unlink_next(_n0) \
    do { if(_n0 && _n0->_next) {_n0->_next->_prev=NULL; _n0->_next=NULL;} } while(0)

bnode *makeBNode(bnode *_prev, elem_t _data, bnode *_next) {
    bnode *_bn = (bnode*)malloc(sizeof(bnode));
    __link_bnode(_prev, _bn);
    _bn->_data = _data;
    __link_bnode(_bn, _next);
    return _bn;
}

void freeBNode(bnode *_bn, blist *_bl) {
    // __unlink_prev(_bn);
    // __unlink_next(_bn);
    __blist_data_free(_bl, _bn->_data);
    free(_bn);
}

blist *makeBList() {
    blist *_bl = (blist*)malloc(sizeof(blist));
    _bl->_head = NULL;
    _bl->_tail = NULL;
    _bl->_size = 0;
    _bl->_elem_cmp = NULL;
    _bl->_elem_free = NULL;
    _bl->_elem_merge = NULL;
    return _bl;
}

void freeBList(blist *_bl) {
    for(bnode *_bn=_bl->_head; _bn&&_bl->_size>0; ) {
        bnode *_nxt = _bn->_next;
        freeBNode(_bn, _bl);_bl->_size--;
        _bn = _nxt;
    }
    free(_bl);
}

bnode *bListAddHead(blist *_bl, elem_t _dt) {
    return bListLinkHead(_bl, makeBNode(NULL,_dt,NULL));
}

elem_t bListDelHead(blist *_bl) {
    elem_t _em={.tag=ELEM_T_INVALID};
    bnode *bn = bListUnlinkHead(_bl);
    if(bn){
        _em = bn->_data;
    }
    freeBNode(bn, _bl);
    return _em;
}

bnode *bListAddTail(blist *_bl, elem_t _dt) {
    return bListLinkTail(_bl, makeBNode(NULL,_dt,NULL));
}

elem_t bListDelTail(blist *_bl) {
    elem_t _em={.tag=ELEM_T_INVALID};
    bnode *bn = bListUnlinkTail(_bl);
    if(bn){
        _em = bn->_data;
    }
    freeBNode(bn, _bl);
    return _em;
}


bnode *bListLinkHead(blist *_bl, bnode *_bn) {
    __link_bnode(_bn, _bl->_head);
    _bl->_head = _bn;
    if(_bl->_size==0) {
        _bl->_tail = _bn;
    }
    _bl->_size++;
    return _bn;
}

bnode *bListUnlinkHead(blist *_bl) {
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

bnode *bListLinkTail(blist *_bl, bnode *_bn) {
    __link_bnode(_bl->_tail, _bn);
    _bl->_tail = _bn;
    if(_bl->_size==0) {
        _bl->_head = _bn;
    }
    _bl->_size++;
    return _bn;
}

bnode *bListUnlinkTail(blist *_bl) {
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

// @attention returned must be freed
blist *bListSplice(blist *_bl0, blist *_bl1) {
    _bl0 = copyBList(_bl0), _bl1 = copyBList(_bl1);
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
    _bl->_elem_cmp = _bl0->_elem_cmp ? _bl0->_elem_cmp:_bl1->_elem_cmp;
    free(_bl0), free(_bl1);
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

// @attention returned must be freed
blist *copyBList(blist *_bl) {
    blist *cpy=makeBList();
    biter *itr=makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *nd=bListNext(itr);nd;nd=bListNext(itr)) {
        bListAddTail(cpy, nd->_data);
    }
    cpy->_elem_cmp = _bl->_elem_cmp;
    freeBIter(itr);
    return cpy;
}

bnode *bListSearch(blist *_bl, elem_t _dt) {
    biter *bi=makeBIter(_bl, BLIST_ITER_FORWARD);
    bnode *ret = NULL;
    for(bnode *bn=bListNext(bi);bn;bn=bListNext(bi)) {
        if(_bl->_elem_cmp(bn->_data, _dt)==0){
            ret = bn; break;
        }
    }
    freeBIter(bi);
    return ret;
}

// _idx >= 0
//  0 -> 0
//  1 -> 1
// _idx <= 0
// -1 -> 0
// -2 -> 1
bnode *bListIndex(blist *_bl, int _idx) {
    biter *bi=makeBIter(_bl, _idx>=0 ? BLIST_ITER_FORWARD:BLIST_ITER_BAKWARD);
    _idx = _idx >= 0 ? _idx : -_idx-1;
    bnode *ret = NULL;
    for(bnode *bn=bListNext(bi);bn&&_idx>=0;bn=bListNext(bi)) {
        if(_idx==0){
            ret = bn; break;
        }
        _idx--;
    }
    freeBIter(bi);
    return ret;
}

// _bl[_lr, _rr)
blist *bListRange(blist *_bl, size_t _lr, size_t _rr) {
    if(_lr>=bListSize(_bl)||_rr>bListSize(_bl)||_lr>=_rr) {
        return NULL;
    }
    blist *rg = makeBList();
    biter *bi = makeBIter(_bl, BLIST_ITER_FORWARD);

    size_t _i=0;
    for(bnode *cur=bListNext(bi);
        cur&&_lr<_rr;
        cur=bListNext(bi),_i++){
        if(_i>=_lr) {
            bListAddTail(rg, cur->_data);
            _lr++;
        }
    }
    rg->_elem_cmp = _bl->_elem_cmp;

    freeBIter(bi);
    return rg;
}

bnode *bListInsertBefore(blist *_bl, bnode *_bn, bnode *_next) {
    if(_next==_bl->_head) {
        return bListLinkHead(_bl, _bn);
    }
    bnode *_nextprev = _next->_prev;
    __link_bnode(_nextprev, _bn);
    __link_bnode(_bn, _next);
    _bl->_size++;
    return _bn;
}

bnode *bListInsertAfter(blist *_bl, bnode *_bn, bnode *_prev) {
    if(_prev==_bl->_tail) {
        return bListLinkTail(_bl, _bn);
    }
    bnode *_prevnext = _prev->_next;
    __link_bnode(_prev, _bn);
    __link_bnode(_bn, _prevnext);
    _bl->_size++;
    return _bn;
}

bnode *bListDeleteNode(blist *_bl, bnode *_bn) {
    if(_bn==_bl->_head) {
        return bListUnlinkHead(_bl);
    } else if(_bn==_bl->_tail) {
        return bListUnlinkTail(_bl);
    }
    bnode *_prev=_bn->_prev;
    bnode *_next=_bn->_next;
    __unlink_prev(_bn); 
    __unlink_next(_bn);
    __link_bnode(_prev,_next);
    _bl->_size--;
    return _bn;
}