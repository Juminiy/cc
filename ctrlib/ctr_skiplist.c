#include "ctr_skiplist.h"

int __elem_cmp_sl_decorator(elem_t_cmp _cmp, elem_t e0, elem_t e1) {
    uint64_t e0tg=e0.tag, e1tg=e1.tag;
    return e0tg==ELEM_T_MIN||e1tg==ELEM_T_MAX ? 
        -1 : (
            e0tg==ELEM_T_MAX||e1tg==ELEM_T_MIN ? +1 : _cmp(e0, e1)
        );
}

size_t __rand_level() {
    size_t lvl = 1;
    while((rand() & 0xffff) < (0xffff>>SKIP_NODE_P_SHIFT) && lvl < SKIP_NODE_L_MAX) {
        lvl ++;
    }
    return lvl;
}

#define __next_(__cur, __lvl) \
    (bArrayLen(__cur->_next) > 0 ? (skipnode*)get_elem_ptr(bArrayAt(__cur->_next, __lvl)) : NULL)

skiplist* makeSkipList(elem_t_cmp _cmp) {
    skiplist *sl = (skiplist*)malloc(sizeof(skiplist));
    sl->_cmp = _cmp;
    sl->_free = NULL;
    sl->_merge = NULL;
    sl->_level = 1;
    sl->_size = 0;
    elem_t _e0 = {.tag=ELEM_T_MIN}; sl->_head = makeSkipNode(_e0);
    elem_t _e1 = {.tag=ELEM_T_MAX}; sl->_tail = makeSkipNode(_e1);
    return sl;
}

void freeSkipList(skiplist *_sl) {
    for(skipnode *cur=_sl->_head;
        cur;
    ) { 
        skipnode *l0_nxt = __next_(cur, 0);
        freeSkipNode(cur, _sl);
        cur = l0_nxt;
    }
    free(_sl);
}

// head -> tail
// insert:10 
// head -> 10 -> tail

void skipListInsertNode(skiplist *_sl, _node_value *_val) {
    barray _prev = makeBArray(_sl->_level, _sl->_level); // barray<skipnode*>
    skipnode *cur = _sl->_head;
    for(size_t lvl = _sl->_level-1; lvl >= 0; lvl--) {
        do {
            skipnode *nxt = __next_(cur, lvl);
            int cmp_res = __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src);
            if(cmp_res == 0) {
                __sl_data_free(_sl, nxt->_data);
                nxt->_data = _val->src;
                _val->retcode = SKIP_NODE_INSERT_REPLACED;
            } else if(cmp_res > 0){
                break;
            }
            cur = nxt;
        } while(true);
        elem_t _em; setup_elem_ptr(_em, cur);
        _prev = bArrayAppend(_prev, _em);
    }

    size_t lvl = __rand_level();
    if(lvl > _sl->_level) {
        
    }

    _val->retcode = SKIP_NODE_INSERT_CREATED;
    skipnode *new_nd = makeSkipNode(_val->src);
    
    freeBArray(_prev);

    _sl->_size++;
}

void skipListDeleteNode(skiplist *_sl, _node_value *_val) {

}

// 85
// head - - - -> 50 - - - - - - -  ->tail
// head - - - -> 50 - -> 70 - - -  ->tail
// head->30->40->50->60->70->80->90->tail
skipnode* skipListGetNode(skiplist *_sl, _node_value *_val) {
    skipnode *cur = _sl->_head;
    for(size_t lvl = _sl->_level-1; lvl >= 0; lvl--) {
        do {
            skipnode *nxt = __next_(cur, lvl);
            int cmp_res = __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src);
            if(cmp_res == 0) {
                _val->retcode = SKIP_NODE_GET_SUCCESS;
                return nxt;
            } else if(cmp_res > 0){
                break;
            }
            cur = nxt;
        } while(true);
    }

    // skipnode *nxt = __next_(cur, 0);
    // if(nxt && __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src)==0) {
    //     _val->retcode = SKIP_NODE_GET_SUCCESS;
    //     return nxt;
    // }

    return NULL;
}

// @param _prev
// type: barray<skipnode*>
// elem: [l-1, l-2, ..., 1, 0]
skipnode* makeSkipNode(elem_t _dt) {
    skipnode *sn = (skipnode*)malloc(sizeof(skipnode));
    sn->_data = _dt;
    sn->_next = makeBArray(0, 0);
    return sn;
}

void freeSkipNode(skipnode *_sn, skiplist *_sl) {
    if(_sl->_free) {
        _sl->_free(_sn->_data);
    }
    freeBArray(_sn->_next);
    free(_sn);
}