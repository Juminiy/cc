#ifndef CTR_MAP_H
#define CTR_MAP_H

#include "ctr_elemt.h"
#include "ctr_rbtree.h"

typedef struct strstrmap {
    rb_tree *_tr;
} strstrmap;

typedef struct strstrpair {
    char *_key;
    char *_val;
} strstrpair;

typedef bool(*strstrmapIterFunc)(char *_key, char *_val);

strstrmap makestrstrmap();
void freestrstrmap(strstrmap _map);
void strstrmapPut(strstrmap _map, char *_key, char *_val);
char* strstrmapGet(strstrmap _map, char *_key);
void strstrmapDel(strstrmap _map, char *_key);
void strstrmapIter(strstrmap _map, strstrmapIterFunc _fn);

#endif