#include "ctr_hash.h"
#include "ctr_blist.h"
#include "ctr_rbtree.h"

hashtable* makeHash(elem_t_cmp _cmp, elem_t_hash _hash) {
    hashtable *ht = (hashtable*)malloc(sizeof(hashtable));
    ht->_bkt_siz = 0;
    ht->_bkt_cap = HASHTABLE_BUKT_INITSZ;
    ht->_bkt_ll = (hashbucket*)calloc(ht->_bkt_cap, sizeof(hashbucket));

    ht->_hash = _hash;
    ht->_load = HASHTABLE_LOAD_FACTOR;
    ht->_size = 0;
    return ht;
}

void freeHash(hashtable *_ht) {
    for(size_t idx=0;idx<_ht->_bkt_cap;idx++){
        hashbucket bkt=_ht->_bkt_ll[idx];
        switch (bkt._typ) {
            case HASHBUCKET_TYPE_TREE:
                rb_tree *_tr = (rb_tree*)bkt._ptr;
                setRBTreeDataFree(_tr, _ht->_free);
                freeRBTree(_tr);
            case HASHBUCKET_TYPE_LIST:
                blist *_bl = (blist*)bkt._ptr;
                bListSetElemFree(_bl, _ht->_free);
                freeBList(_bl);
        }
    }
    free(_ht->_bkt_ll);
    free(_ht);
}

#define __bktidx(_ht, _dt) \
    ({ int hash_val = _ht->_hash(_dt); \
        hash_val = hash_val ^ (hash_val>>16); \
        hash_val & (_ht->_bkt_cap - 1); })

rb_tree* __blist2_rbtree(blist *_bl, hashtable *_ht) {
    biter *bi = makeBIter(_bl, BLIST_ITER_FORWARD);
    rb_tree *tr = makeRBTree(_ht->_cmp);
    setRBTreeNodeType(tr, TREE_NODE_TYPE_AVL);

    for(rb_node *nd=bListNext(bi);nd;nd=bListNext(bi)) {
        rbTreeInsertData(tr, nd->_data);
    }

    freeBIter(bi);
    freeBList(_bl);
    return tr;
}

elem_t hashGet(hashtable *_ht, elem_t _dt) {
    int bkt_idx = __bktidx(_ht, _dt);
    hashbucket _bkt = _ht->_bkt_ll[bkt_idx];
    elem_t _em = {.tag=ELEM_T_INVALID};
    switch (_bkt._typ) {
        case HASHBUCKET_TYPE_TREE:
            rb_tree *_tr = (rb_tree*)(_bkt._ptr);
            _em = rbTreeGetData(_tr, _dt);

        case HASHBUCKET_TYPE_LIST:
            blist *_bl = (blist*)(_bkt._ptr);
            bnode *_bn = bListSearch(_bl, _dt);
            _em = _bn ? _bn->_data : _em;
        
        default: // none
    }
    return _em;
}

int __hashtable_put(hashtable *_ht, elem_t _dt) {
    int _ret_code = ELEM_OPT_NONE;
    int bkt_idx = __bktidx(_ht, _dt);
    hashbucket _bkt = _ht->_bkt_ll[bkt_idx]; // read from bucketArray
    switch (_bkt._typ) {
        case HASHBUCKET_TYPE_TREE:
            rb_tree *_tr = (rb_tree*)(_bkt._ptr);
            _ret_code = rbTreeInsertData(_tr, _dt);

        case HASHBUCKET_TYPE_LIST:
            blist *_bl = (blist*)(_bkt._ptr);
            bnode *_bn = bListSearch(_bl, _dt);
            if(!_bn) {
                _ret_code = ELEM_INSERT_CREATED;
                bListAddHead(_bl, _dt);
            } else {
                __hash_free(_ht, _bn->_data);
                _bn->_data = _dt;
            }
            if(bListSize(_bl) >= HASHBUCKET_LIST_MAXSZ &&
                _ht->_bkt_cap >= HASHBUCKET_BUKT_2TREE_MINSZ) {
                _bkt._typ = HASHBUCKET_TYPE_TREE;
                _bkt._ptr = __blist2_rbtree(_bl, _ht);
            }
        
        default: // none
            blist *_bl = makeBList();
            bListSetElemCmp(_bl, _ht->_cmp);
            
            _ht->_bkt_siz++;
            bListAddHead(_bl, _dt);
            _ret_code = ELEM_INSERT_CREATED;
            _bkt._typ = HASHBUCKET_TYPE_LIST;
            _bkt._ptr = _bl;
    }
    _ht->_bkt_ll[bkt_idx] = _bkt; // write back to bucketArray
    return _ret_code;
}

void __hashtable_enlarge_rehash(hashtable *_ht) {
    // buckets enlarge
    hashbucket *old_bkts = _ht->_bkt_ll;
    size_t old_cap = _ht->_bkt_cap;

    _ht->_bkt_cap <<= 1;
    _ht->_bkt_ll = (hashbucket*)calloc(_ht->_bkt_cap, sizeof(hashbucket));

    // elems rehash
    for(size_t idx=0;idx<old_cap;idx++) {
        hashbucket old_bkt=old_bkts[idx];
        switch (old_bkt._typ) {
            // case HASHBUCKET_TYPE_TREE:
            //     rbTreeIter((rb_tree*)old_bkt._ptr, _fn);
            // case HASHBUCKET_TYPE_LIST:
            //     bListIter((blist*)old_bkt._ptr, _fn);
        }
    }
}

void hashPut(hashtable *_ht, elem_t _dt) {
    if(_ht->_bkt_siz > (size_t)(_ht->_bkt_cap * _ht->_load)) {
        __hashtable_enlarge_rehash(_ht);
    }

    _ht->_size = __hashtable_put(_ht, _dt) == ELEM_INSERT_CREATED ? 
        _ht->_size : _ht->_size + 1;
}

void hashDel(hashtable *_ht, elem_t _dt) {

}

void hashIter(hashtable *_ht, elem_t_vis _fn) {
    for(size_t idx=0;idx<_ht->_bkt_cap;idx++){
        hashbucket bkt=_ht->_bkt_ll[idx];
        switch (bkt._typ) {
            case HASHBUCKET_TYPE_TREE:
                rbTreeIter((rb_tree*)bkt._ptr, _fn);
            case HASHBUCKET_TYPE_LIST:
                bListIter((blist*)bkt._ptr, _fn);
        }
    }
}