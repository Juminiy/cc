#ifndef CTR_HASH_H
#define CTR_HASH_H

#include "ctr_barray.h"
#include "ctr_blist.h"
#include "ctr_rbtree.h"

#define HASHBUCKET_TYPE_NONE 0
#define HASHBUCKET_TYPE_LIST 1
#define HASHBUCKET_TYPE_TREE 2

typedef struct hashbucket {
    void *_bkt;
    int   _typ;
} hashbucket;

typedef struct hashtable {
    hashbucket *_bkt_ll;
    size_t      _bkt_sz;

    size_t      _size;
    float       _load;
} hashtable;

#endif