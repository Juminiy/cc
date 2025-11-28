#include "../ctrlib/ctr_bstack.h"
#include "../ctrlib/ctr_bqueue.h"

#include <stdio.h>

void test_bstack() {
    elem_t elem_val = {.tag=1};
    bstack *bstk=makeBStack();

    setup_elem_t(elem_val,1,i64,10); bStackPush(bstk,elem_val);
    elem_val = bStackTop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);
    
    setup_elem_t(elem_val,1,i64,12); bStackPush(bstk,elem_val);
    elem_val = bStackTop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);
    elem_val = bStackTop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);

    elem_val = bStackPop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);
    elem_val = bStackPop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);
    elem_val = bStackPop(bstk); printf("top=(%ld,%ld)\n", elem_val.tag, elem_val.uni.i64);

    freeBStack(bstk);
}

void test_bqueue() {
    elem_t elem_val = {.tag=0};
    bqueue *bq = makeBQueue();

    setup_elem_t(elem_val,1,i64,114514); bQueuePush(bq, elem_val);
    setup_elem_t(elem_val,1,i64,1919810); bQueuePush(bq, elem_val);
    setup_elem_t(elem_val,1,i64,334555); bQueuePush(bq, elem_val);

    for(elem_val=bQueuePop(bq);
        valid_elem_t(elem_val);
        elem_val=bQueuePop(bq)) 
    {
        printf("%ld\n", elem_val.uni.i64);
    }

    freeBQueue(bq);
}

int main() {

    // test_bstack();

    test_bqueue();

    return 0;
}