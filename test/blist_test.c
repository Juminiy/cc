#include "../ctrlib/ctr_blist.h"

#include <stdio.h>

void print_blist(blist *bl) {
    printf("LIS:blist=[ ");
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi))
        printf("%ld ", bNodeData(bn).i64);
    printf("]\n");

    printf("REV:blist=[ ");
    bi = makeBIter(bl, BLIST_ITER_BAKWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi))
        printf("%ld ", bNodeData(bn).i64);
    printf("]\n");
}

void print_bnode(bnode *bn) {
    if (bn) 
        printf("bnode=%ld\n", bNodeData(bn).i64);
    else
        printf("bnode=NULL\n");
}

void range_add_blist(blist *bl, int _lr, int _rr) {
    for(int i=_lr;i<_rr;i++) {
        elem_t eleval = {.tag = 0};
        eleval.uni.i64 = i;
        bListAddTail(bl, eleval);
    }
}

void test_head() {
    elem_t eleval;
    setup_elem_t(eleval, 0, i64, 0);

    blist *bl = makeBList();
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 5);
    bListAddHead(bl, eleval);
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 6);
    bListAddHead(bl, eleval);
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 7);
    bListAddHead(bl, eleval);
    print_blist(bl);

    bnode *nd=bListDelHead(bl);
    print_blist(bl); print_bnode(nd);

    nd=bListDelHead(bl);
    print_blist(bl); print_bnode(nd);

    nd=bListDelHead(bl);
    print_blist(bl); print_bnode(nd);

    nd=bListDelHead(bl);
    print_blist(bl); print_bnode(nd);

    nd=bListDelHead(bl);
    print_blist(bl); print_bnode(nd);

    freeBList(bl);
}

void test_tail() {
    elem_t eleval;
    setup_elem_t(eleval, 0, i64, 0);

    blist *bl = makeBList();
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 5);
    bListAddTail(bl, eleval);
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 6);
    bListAddTail(bl, eleval);
    print_blist(bl);

    setup_elem_t(eleval, 0, i64, 7);
    bListAddTail(bl, eleval);
    print_blist(bl);

    bnode *nd=bListDelTail(bl);         // delete 7
    print_blist(bl); print_bnode(nd); 

    nd=bListDelTail(bl);                // delete 6
    print_blist(bl); print_bnode(nd);   

    nd=bListDelTail(bl);                // delete 5
    print_blist(bl); print_bnode(nd);   

    nd=bListDelTail(bl);
    print_blist(bl); print_bnode(nd);

    nd=bListDelTail(bl);
    print_blist(bl); print_bnode(nd);

    freeBList(bl);
}

void test_splice() {
    blist *l0=makeBList();
    range_add_blist(l0, 10, 15);
    print_blist(l0);

    blist *l1=makeBList();
    range_add_blist(l1, 18, 23);
    print_blist(l1);

    blist *l2=bListSplice(l0, l1);
    print_blist(l2);

    blist *l3=bListSplice(l2, makeBList());
    print_blist(l3);

    blist *l4=bListSplice(makeBList(), l2);
    print_blist(l4);

    blist *l5=bListSplice(makeBList(), makeBList());
    print_blist(l5);
}

void test_copy() {
    blist *l0=makeBList();
    range_add_blist(l0, 0, 0);
    print_blist(copyBList(l0));

    blist *l1=makeBList();
    range_add_blist(l1, 0, 1);
    print_blist(copyBList(l1));

    blist *l2=makeBList();
    range_add_blist(l2, 0, 2);
    print_blist(copyBList(l2));

    blist *l3=makeBList();
    range_add_blist(l3, 19, 25);
    print_blist(copyBList(l3));
}

int cmp_int(elem_t e0, elem_t e1) {
    return e0.uni.i64-e1.uni.i64;
}

void test_search_index() {
    blist *l1=makeBList();
    range_add_blist(l1, 10, 20);
    bListSetElemCmp(l1, cmp_int);

    // 10 11 12 13 14 15 16 16 18 19
    elem_t elem_val; setup_elem_t(elem_val, 0, i64, 1);
    bnode *bn = bListSearch(l1, elem_val); print_bnode(bn); // NULL

    setup_elem_t(elem_val, 0, i64, 11);
    bn = bListSearch(l1, elem_val); print_bnode(bn); // 11

    bn = bListIndex(l1, 0); print_bnode(bn);    // 10

    bn = bListIndex(l1, 5); print_bnode(bn);    // 15

    bn = bListIndex(l1, -1); print_bnode(bn);   // 19

    bn = bListIndex(l1, 22); print_bnode(bn);   // NULL
}

int main() {

    // test_tail();
    // test_head();
    // test_splice();
    // test_copy();

    test_search_index();
    return 0;
}