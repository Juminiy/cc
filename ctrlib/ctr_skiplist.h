#ifndef CTR_SKIPLIST_H
#define CTR_SKIPLIST_H

#define SKIP_NODE_P       4 // p=1/4
#define SKIP_NODE_P_SHIFT 2 // 4=1<<2
#define SKIP_NODE_L_MAX   32

#include "ctr_elemt.h"
#include "ctr_barray.h"

typedef struct skipnode {
    barray _next; // barray<skipnode*>
    elem_t _data;
} skipnode;

typedef struct skiplist {
    skipnode *_head,*_tail;

    size_t _size;
    size_t _level;
    elem_t_cmp _cmp;
    elem_t_free _free;
    elem_t_merge _merge;
} skiplist;

#define SKIP_NODE_OPT_NONE        ELEM_OPT_NONE
#define SKIP_NODE_INSERT_CREATED  ELEM_INSERT_CREATED
#define SKIP_NODE_INSERT_REPLACED ELEM_INSERT_REPLACED
#define SKIP_NODE_DELETE_SUCCESS  ELEM_DELETE_REMOVED
#define SKIP_NODE_DELETE_NOTFOUND ELEM_DELETE_NOTFOUND
#define SKIP_NODE_GET_SUCCESS     ELEM_GET_SUCCESS
#define SKIP_NODE_GET_NOTFOUND    ELEM_GET_NOTFOUND

#define __sl_data_free(sl, _dt) \
    do { if(sl->_free) {sl->_free(_dt);} } while(0)
#define __sl_data_merge(sl, _dest, _src) \
    do { if(sl->_merge) {_dest = sl->_merge(_dest, _src);} } while(0)

skiplist* makeSkipList(elem_t_cmp _cmp);
void freeSkipList(skiplist *_sl);
int skipListInsertData(skiplist *_sl, elem_t _dt);
int skipListDeleteData(skiplist *_sl, elem_t _dt);
elem_t skipListGetData(skiplist *_sl, elem_t _dt);

void skipListInsertNode(skiplist *_sl, _node_value *_val);
void skipListDeleteNode(skiplist *_sl, _node_value *_val);
skipnode* skipListGetNode(skiplist *_sl, _node_value *_val);

skipnode* makeSkipNode(elem_t _dt);
void freeSkipNode(skipnode *_sn, skiplist *_sl);

#endif