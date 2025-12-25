#include "../ctrlib/ctr_skiplist.h"
#include "../ctrlib/ctr_rbtree.h"

#include <time.h>
#include <stdlib.h>

// 1,000,000 * int
// 1,999,557 allocs, 1,999,557 frees, 58,651,072 bytes allocated
void test_sl(int tot) {
    skiplist *sl=makeSkipList(__elem_cmp_int);
    elem_t em;

    // int arr[10]={8,5,7,3,6,1,2,8,4,1};
    for(int i=0;i<tot;i++){
        setup_elem_i64(em,rand());
        skipListInsertData(sl, em);
    }

    // barray elems = skipList2Array(sl);
    // int prev_val = 0x80000000;
    // for(size_t i=0;i<bArrayLen(elems);i++){
    //     // printf("%zu, %ld\n", i, get_elem_i64(bArrayAt(elems, i)));
    //     int curval = get_elem_i64(bArrayAt(elems, i));
    //     if(curval < prev_val){
    //         fprintf(stderr, "curlval: %d < prevval: %d\n", curval, prev_val);
    //         exit(1);
    //     }
    // }

    printf("len=%zu, level=%d\n", __sl_len(sl), __sl_level(sl));

    // freeBArray(elems);
    freeSkipList(sl);
} 

// 1,000,000 * int
// 1,999,550 allocs, 1,999,550 frees, 119,973,864 bytes allocated
void test_avl(int tot) {
    rb_tree *tr=makeRBTree(__elem_cmp_int);
    setRBTreeNodeType(tr, TREE_NODE_TYPE_AVL);
    elem_t em;

    // int arr[10]={8,5,7,3,6,1,2,8,4,1};
    for(int i=0;i<tot;i++){
        setup_elem_i64(em,rand());
        rbTreeInsertData(tr, em);
    }

    // barray elems = rbTree2Array(tr);
    // int prev_val = 0x80000000;
    // for(size_t i=0;i<bArrayLen(elems);i++){
    //     // printf("%zu, %ld\n", i, get_elem_i64(bArrayAt(elems, i)));
    //     int curval = get_elem_i64(bArrayAt(elems, i));
    //     if(curval < prev_val){
    //         fprintf(stderr, "curlval: %d < prevval: %d\n", curval, prev_val);
    //         exit(1);
    //     }
    // }

    printf("len=%zu, level=%zu\n", __tree_size(tr), __tree_height(tr));

    // freeBArray(elems);
    freeRBTree(tr);
}

void test_sl_curd(int tot) {
    skiplist *sl=makeSkipList(__elem_cmp_int);
    elem_t em;

    int arr[10]={8,5,7,3,6,1,2,8,4,1};
    for(int i=0;i<10;i++){
        setup_elem_i64(em,arr[i]);
        skipListInsertData(sl, em);
    }

    barray elems = skipList2Array(sl);
    int prev_val = 0x80000000;
    for(size_t i=0;i<bArrayLen(elems);i++){
        printf("%zu, %ld\n", i, get_elem_i64(bArrayAt(elems, i)));
        // int curval = get_elem_i64(bArrayAt(elems, i));
        // if(curval < prev_val){
        //     fprintf(stderr, "curlval: %d < prevval: %d\n", curval, prev_val);
        //     exit(1);
        // }
    }
    printf("len=%zu, level=%d\n", __sl_len(sl), __sl_level(sl));

    for(int i=0;i<10;i++){
        setup_elem_i64(em,arr[i]);
        skipListDeleteData(sl, em);
    }
    setup_elem_i64(em,100); skipListDeleteData(sl, em);
    setup_elem_i64(em,-1); skipListDeleteData(sl, em);
    printf("len=%zu, level=%d\n", __sl_len(sl), __sl_level(sl));

    freeBArray(elems);
    freeSkipList(sl);
} 

int main(int argc, char **argv) {
    srand(time(NULL));
    int typ=1,tot=0;
    if(argc>=2){
        typ = __strcmp(argv[1], "avl") == 0 ? 0 : 1;
    }
    if(argc>=3){
        tot = strtol(argv[2], NULL, 10);
    }

    if(typ==0){
        printf("test AVLTree\n");
        test_avl(tot);
    } else {
        printf("test SkipList\n");
        test_sl(tot);
    }

    // test_sl_curd(tot);

    return 0;
}