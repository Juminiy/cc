#include "ctr_skiplist.h"

int skipListInsertData(skiplist *_sl, elem_t _dt) {
    _node_value _val; init_node_value(_val,_dt);
    skipListInsertNode(_sl, &_val);
    return _val.retcode;
}

int skipListDeleteData(skiplist *_sl, elem_t _dt) {
    _node_value _val; init_node_value(_val,_dt);
    skipListDeleteNode(_sl, &_val);
    return _val.retcode;
}

elem_t skipListGetData(skiplist *_sl, elem_t _dt) {
    _node_value _val; init_node_value(_val,_dt);
    skipListGetNode(_sl, &_val);
    return _val.dst;
}

int __elem_cmp_sl_decorator(elem_t_cmp _cmp, elem_t e0, elem_t e1) {
    uint64_t e0tg=e0.tag, e1tg=e1.tag;
    return e0tg==ELEM_T_MIN||e1tg==ELEM_T_MAX ? 
        -1 : (
            e0tg==ELEM_T_MAX||e1tg==ELEM_T_MIN ? +1 : _cmp(e0, e1)
        );
}

int __rand_level() {
    int lvl = 1;
    while((rand() & 0xffff) < (0xffff>>SKIP_NODE_P_SHIFT) && lvl < SKIP_NODE_L_MAX) {
        lvl ++;
    }
    return lvl;
}

#define __next_(__cur, __lvl) \
    (__cur&&__cur->_next ? __cur->_next[__lvl] : NULL)


skiplist* makeSkipList(elem_t_cmp _cmp) {
    skiplist *sl = (skiplist*)malloc(sizeof(skiplist));
    sl->_cmp = _cmp;
    sl->_free = NULL;
    sl->_merge = NULL;
    sl->_level = 1;
    sl->_size = 0;
    elem_t _e0 = {.tag=ELEM_T_MIN}; sl->_head = makeSkipNode(_e0, SKIP_NODE_L_MAX);
    elem_t _e1 = {.tag=ELEM_T_MAX}; sl->_tail = makeSkipNode(_e1, SKIP_NODE_L_MAX);
    for(size_t lvl=0;lvl<SKIP_NODE_L_MAX;lvl++){
        sl->_head->_next[lvl] = sl->_tail;
        sl->_tail->_next[lvl] = NULL;
    }
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


void skipListInsertNode(skiplist *_sl, _node_value *_val) {
    skipnode *_prev[SKIP_NODE_L_MAX];
    skipnode *cur = _sl->_head;
    for(int lvl = _sl->_level-1; lvl >= 0; lvl--) {
        for(skipnode *nxt = __next_(cur, lvl);
            nxt;
            cur = nxt, nxt = __next_(cur, lvl)
        ) {
            int cmp_res = __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src);
            if(cmp_res == 0) {
                __sl_data_free(_sl, nxt->_data);
                nxt->_data = _val->src;
                _val->retcode = SKIP_NODE_INSERT_REPLACED;
                break;
            } 
            if(cmp_res > 0){
                break;
            }
        }
        _prev[lvl] = cur;
    }

    if (_val->retcode==SKIP_NODE_INSERT_REPLACED){
        return ;
    }

    int curlvl = __rand_level();
    if(curlvl > _sl->_level) {
        for(int lvl=_sl->_level;lvl<curlvl;lvl++){
            _prev[lvl] = _sl->_head;
        }
        _sl->_level = curlvl;
    }

    _val->retcode = SKIP_NODE_INSERT_CREATED;
    skipnode *new_nd = makeSkipNode(_val->src, curlvl);
    for(int lvl=0;lvl<curlvl;lvl++){
        new_nd->_next[lvl] = _prev[lvl]->_next[lvl];
        _prev[lvl]->_next[lvl] = new_nd;
    }

    _sl->_size++;
}

void skipListDeleteNode(skiplist *_sl, _node_value *_val) {
    skipnode *_prev[SKIP_NODE_L_MAX];
    skipnode *cur = _sl->_head, *del_nd = NULL;
    for(int lvl = _sl->_level-1; lvl >= 0; lvl--) {
        for(skipnode *nxt = __next_(cur, lvl);
            nxt;
            cur = nxt, nxt = __next_(cur, lvl)
        ) {
            int cmp_res = __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src);
            if(cmp_res == 0) {
                _val->dst = nxt->_data;
                _val->retcode = SKIP_NODE_DELETE_SUCCESS;
                del_nd = nxt;
                break;
            } 
            if(cmp_res > 0){
                break;
            }
        }
        _prev[lvl] = cur;
    }

    if(!del_nd){
        _val->retcode = SKIP_NODE_DELETE_NOTFOUND;
        return;
    }

    for(int lvl=0;lvl<_sl->_level;lvl++){
        if(_prev[lvl]->_next[lvl] == del_nd) {
            _prev[lvl]->_next[lvl] = del_nd->_next[lvl];
        }
    }
    freeSkipNode(del_nd, _sl);

    while(_sl->_level>1 && _sl->_head->_next[_sl->_level-1]==_sl->_tail) {
        _sl->_level--;
    }
    _sl->_size--;

}

skipnode* skipListGetNode(skiplist *_sl, _node_value *_val) {
    skipnode *cur = _sl->_head;
    for(int lvl = _sl->_level-1; lvl >= 0; lvl--) {
        for(skipnode *nxt = __next_(cur, lvl);
            nxt;
            cur = nxt, nxt = __next_(cur, lvl)
        ) { 
            int cmp_res = __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src);
            if(cmp_res == 0) {
                _val->dst = nxt->_data;
                _val->retcode = SKIP_NODE_GET_SUCCESS;
                return nxt;
            } 
            if(cmp_res > 0){
                break;
            }
        }
    }

    // skipnode *nxt = __next_(cur, 0);
    // if(nxt && __elem_cmp_sl_decorator(_sl->_cmp, nxt->_data, _val->src)==0) {
    //     _val->retcode = SKIP_NODE_GET_SUCCESS;
    //     return nxt;
    // }

    _val->retcode = SKIP_NODE_GET_NOTFOUND;
    return NULL;
}

skipnode* makeSkipNode(elem_t _dt, size_t _level) {
    skipnode *sn = (skipnode*)malloc(sizeof(skipnode));
    sn->_data = _dt;
    sn->_level = _level;
    sn->_next = (skipnode**)malloc(sn->_level * sizeof(skipnode*));
    return sn;
}

void freeSkipNode(skipnode *_sn, skiplist *_sl) {
    __sl_data_free(_sl, _sn->_data);
    free(_sn->_next);
    free(_sn);
}

barray skipList2Array(skiplist *_sl) {
    barray _ems = makeBArray(0, _sl->_size);
    for(skipnode *cur=_sl->_head;
        ;
    ) { 
        skipnode *l0_nxt = __next_(cur, 0);
        if(l0_nxt==_sl->_tail) break;
        _ems = bArrayAppend(_ems, l0_nxt->_data);
        cur = l0_nxt;
    }
    return _ems;
}

void skipListIter(skiplist *_sl, elem_t_vis _fn) {
    for(skipnode *cur=_sl->_head;
        ;
    ) { 
        skipnode *l0_nxt = __next_(cur, 0);
        if(l0_nxt==_sl->_tail) break;
        _fn(l0_nxt->_data);
        cur = l0_nxt;
    }
}