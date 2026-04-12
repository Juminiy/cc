#include "../../libctr/ctr_union.h"
#include <stdio.h>

#define put(_st, _l,_r) \
    do { \
        for(int i=_l;i<_r;i++){ \
            unionSetPut(_st, i); \
        } \
    } while(0)

#define put_merge(_st, _l,_r) \
    do { \
        for(int i=_l;i<_r;i++){ \
            unionSetPut(_st, i); \
        } \
        for(int i=_l+1;i<_r;i++){ \
            unionSetMerge(_st, i, i-1); \
        } \
    } while(0)

#define uset_print(_st) \
    do { \
        for(int i=0;i<_st->_siz;i++){ \
            printf("idx=[%d], parent=[%d], rank=[%d]\n", i, _st->_parent[i], _st->_rank[i]);\
        } \
    } while(0)

void test_set1() {
    size_t sz = 10;
    union_set *_st = makeUnionSet(sz);

    put(_st,0,10);
    printf("size=%ld, count=%ld\n", sz, unionSetCnt(_st));
    put_merge(_st, 0, 10);
    printf("size=%ld, count=%ld\n", sz, unionSetCnt(_st));

    freeUnionSet(_st);
}

void test_set3() {
    size_t sz = 20;
    union_set *_st = makeUnionSet(sz);

    put(_st,0, 10);
    put(_st,15, 17);
    put(_st,18, 20);
    printf("size=%ld, count=%ld\n", sz, unionSetCnt(_st));
    put_merge(_st, 0, 10);  // [0,9] 10
    put_merge(_st, 15, 17); // [15,16] 2
    put_merge(_st, 18, 20); // [18,19] 2
    printf("size=%ld, count=%ld\n", sz, unionSetCnt(_st));

    freeUnionSet(_st);
}

int main() {

    test_set1();
    puts("");
    test_set3();

    return 0;
}