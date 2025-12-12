#include "../ctrlib/ctr_lfu.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct ii{
    int k,v;
} ii;

ii* makeii(int k,int v){
    ii *iikv=(ii*)malloc(sizeof(ii));
    iikv->k=k,iikv->v=v;
    return iikv;
}

int cmpii(elem_t e0, elem_t e1) {
    ii *i0=(ii*)get_elem_ptr(e0);
    ii *i1=(ii*)get_elem_ptr(e1);
    return i0->k-i1->k;
}

void freeii(elem_t e){
    ii *iikv=(ii*)get_elem_ptr(e);
    free(iikv);
}

#define put_i(_ikey, _ival) \
    do { \
        elem_t em; \
        setup_elem_ptr(em, makeii(_ikey, _ival)); \
        lfuTPut(t, em); \
    } while(0)

#define get_i(_ikey) \
    do { \
        elem_t em; \
        ii __iikv; __iikv.k=_ikey,__iikv.v=0; \
        setup_elem_ptr(em, &__iikv); \
        void *ptr=get_elem_ptr(lfuTGet(t, em)); \
        ii *iikv=ptr?(ii*)ptr:NULL; \
        printf("%d\n", iikv?iikv->v:-1); \
    } while(0)

void printlfunodes(lfu_t *_t) {
    blist *ls = lfuT2List(_t);
    biter *bi = makeBIter(ls, BLIST_ITER_FORWARD);
    printf("[ ");
    for(bnode *bn=bListNext(bi);
        bn;
        bn=bListNext(bi)
    ) {
        lfu_node* lfun=(lfu_node*)get_elem_ptr(bn->_data);
        ii* iikv = (ii*)get_elem_ptr(lfun->_data);
        printf("(%zu,%zu,<%d,%d>) ",lfun->_refs,lfun->_ts,iikv->k,iikv->v);   
    }
    printf("]\n");

    freeBIter(bi);
    freeBList(ls);
}

void test_case1() {
    lfu_t *t = makeLFUT(cmpii, 2); 
    lfuTSetElemFree(t, freeii);

    put_i(1,1); printlfunodes(t);
    put_i(2,2); printlfunodes(t);
    get_i(1);   printlfunodes(t); // 1
    put_i(3,3); printlfunodes(t);
    get_i(2);   printlfunodes(t); // -1
    get_i(3);   printlfunodes(t); // 3
    put_i(4,4); printlfunodes(t); 
    get_i(1);   printlfunodes(t); // -1
    get_i(3);   printlfunodes(t); // 3
    get_i(4);   printlfunodes(t); // 4

    freeLFUT(t);
}

void test_case2() {
    lfu_t *t = makeLFUT(cmpii, 2); 
    lfuTSetElemFree(t, freeii);

    put_i(2, 1);
    put_i(2, 2);
    get_i(2);
    put_i(1, 1);
    put_i(4, 1);
    get_i(2); 

    freeLFUT(t);
}

void test_case3() {
    lfu_t *t = makeLFUT(cmpii, 3); 
    lfuTSetElemFree(t, freeii);

    put_i(1,1);
    put_i(2,2);
    put_i(3,3);
    put_i(4,4);
    get_i(4);
    get_i(3);
    get_i(2);
    get_i(1);
    put_i(5,5);
    get_i(1);
    get_i(2);
    get_i(3);
    get_i(4);
    get_i(5);

    freeLFUT(t);
}

int main() {

    test_case1();
    printf("----\n");
    test_case2();
    printf("----\n");
    test_case3();

    return 0;
}