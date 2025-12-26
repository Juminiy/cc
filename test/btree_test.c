#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../ctrlib/ctr_btree.h"
#include "strstrpair.h"

// 1,000,000 * int
// total heap usage: 1,408,211 allocs, 1,408,211 frees, 122,573,400 bytes allocated = 116MB
void test_sort(int tot) {
    btree *tr = makeBTree(3, __elem_cmp_int);

    for(int i=0;i<tot;i++)
        bTreeInsert(tr, varElemI64(rand()));
    
    int prv=-1;
    bool print_i32(elem_t em) {
        int cur = cast_elem_typ(em, int);
        if(cur<prv){
            printf("%d < %d",cur,prv);
            return false;
        }
        prv=cur;
        // printf("%d\n", cur);
        return true;
    };

    bTreeIter(tr, print_i32);
    puts("");

    printf("tot=%d size=%zu height=%zu\n", tot, bTreeSize(tr), bTreeHeight(tr));

    freeBTree(tr);
}

void test_strstrpair(int tot) {
    btree *tr = makeBTree(3, __elem_cmp_strstrpair);

    kv *kvs = makekvs(tot);
    for(int i=0;i<tot;i++){
        bTreeInsert(tr, varElemPtr(kvs+i));
    }

    freekvs(kvs, tot);
    freeBTree(tr);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int tot=0;
    if(argc>1)
        tot = strtol(argv[1],NULL,10);

    // test_sort(tot);

    test_strstrpair(tot);

    return 0;
}