#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../ctrlib/ctr_hash.h"
#include "strstrpair.h"

int elem_i64_hash(elem_t em) {
    return get_elem_i64(em);
}

#define __add(ht, __val) \
    do { \
        elem_t em; \
        setup_elem_i64(em, __val); \
        hashPut(ht, em); \
    } while(0)
#define __con(ht, __val) \
    do { \
        elem_t em; \
        setup_elem_i64(em, __val); \
        printf("%s\n", valid_elem_t(hashGet(ht, em))?"True":"False"); \
    } while(0)
#define __rmv(ht, __val) \
    do { \
        elem_t em; \
        setup_elem_i64(em, __val); \
        hashDel(ht, em); \
    } while(0)

void __inspect(hashtable *ht) {
    #ifdef IDEBUG
    hash_inspect hi = hashInspect(ht);
    DEBUGF("size = %zu\nbkt_siz = %zu\nbkt_cap = %zu\nbkt_ll = %zu\nbkt_tr = %zu",
       hi._size, hi._bkt_siz, hi._bkt_cap, hi._bkt_ll, hi._bkt_tr
    );
    for(size_t len=0;len<HASHBUCKET_LIST_MAXSZ;len++){
        DEBUGF("ll_cnt[%zu]: %zu, %.5f%%", len, hi._bkt_ll_siz[len], hi._bkt_ll_siz[len]*100.00/hi._bkt_cap);
    }
    DEBUGF("tree_max_siz = %zu", hi._bkt_tr_max_sz);
    #endif
}


void test1() {
    hashtable *ht = makeHash(__elem_cmp_int, elem_i64_hash);

    __add(ht, 1);
    __add(ht, 2);
    __con(ht, 1); // True
    __con(ht, 3); // False
    __add(ht, 2);
    __con(ht, 2); // True
    __rmv(ht, 2);
    __con(ht, 2); // False

    freeHash(ht);
}

bool print_i64(elem_t em){
    printf("%ld ", get_elem_i64(em));
    return true;
}

// 1,000,000 * int
// 8,514,485 allocs, 8,514,485 frees, 396,787,720 bytes allocated
void test2(int tot) {
    hashtable *ht = makeHash(__elem_cmp_int, elem_i64_hash);

    int *arr = (int*)malloc(sizeof(int)*tot);

    for(int i=0;i<tot;i++){
        arr[i] = rand();
        __add(ht, arr[i]);
    }
    printf("siz=%zu, bkt_siz=%zu, bkt_cap=%zu\n", hashLen(ht), ht->_bkt_siz, ht->_bkt_cap);

    // for(int i=0;i<tot;i++){
    //     __con(ht, arr[i]);
    // }

    __inspect(ht);

    for(int i=0;i<tot;i++){
        __rmv(ht, arr[i]);
    }

    // hashIter(ht, print_i64);
    printf("siz=%zu, bkt_siz=%zu, bkt_cap=%zu\n", hashLen(ht), ht->_bkt_siz, ht->_bkt_cap);

    freeHash(ht);
    free(arr);
}

void test3(int tot) {
    kv *kvs = makekvs(tot);
    hashtable *ht = makeHash(__elem_cmp_strstrpair, __elem_hash_strstrpair);

    for(int i=0;i<tot;i++) {
        elem_t em; setup_elem_ptr(em, kvs+i);
        hashPut(ht, em);
    }

    __inspect(ht);

    freeHash(ht);
    freekvs(kvs, tot);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int tot=0;
    if(argc>1)
        tot = strtol(argv[1], NULL, 10);

    // test1();
    // test2(tot);
    test3(tot);

    return 0;
}