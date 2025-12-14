#include "../ctrlib/ctr_blist.h"

#include <stdio.h>

void print_blist(blist *bl) {
    printf("LIS:blist=[ ");
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi))
        printf("%ld ", get_elem_i64(bn->_data));
    printf("]\n");
    freeBIter(bi);

    printf("REV:blist=[ ");
    bi = makeBIter(bl, BLIST_ITER_BAKWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi))
        printf("%ld ", get_elem_i64(bn->_data));
    printf("]\n");
    freeBIter(bi);
}

void print_bnode(bnode *bn) {
    if (bn) 
        printf("bnode=%ld\n", get_elem_i64(bn->_data));
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

    eleval = bListDelHead(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval = bListDelHead(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval = bListDelHead(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval = bListDelHead(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval = bListDelHead(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

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

    eleval=bListDelTail(bl);                 // delete 7
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval=bListDelTail(bl);                // delete 6
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval=bListDelTail(bl);                // delete 5
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval)); 

    eleval=bListDelTail(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

    eleval=bListDelTail(bl);
    print_blist(bl); printf("%ld\n",get_elem_i64(eleval));

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

    blist *_emptyLis=makeBList();
    blist *l3=bListSplice(l2, _emptyLis);
    print_blist(l3);

    blist *l4=bListSplice(_emptyLis, l2);
    print_blist(l4);

    blist *l5=bListSplice(_emptyLis, _emptyLis);
    print_blist(l5);

    freeBList(l0); printf("l0 ok\n");
    freeBList(l1); printf("l1 ok\n");
    freeBList(_emptyLis); printf("empty ok\n");
    freeBList(l2); printf("l2 ok\n");
    freeBList(l3); printf("l3 ok\n");
    freeBList(l4); printf("l4 ok\n");
    freeBList(l5); printf("l5 ok\n");
}

void test_copy() {
    blist *l0=makeBList();
    range_add_blist(l0, 0, 0);
    blist *l0cpy=copyBList(l0);
    print_blist(l0cpy); freeBList(l0cpy);

    blist *l1=makeBList();
    range_add_blist(l1, 0, 1);
    blist *l1cpy=copyBList(l0);
    print_blist(l1cpy); freeBList(l1cpy);

    blist *l2=makeBList();
    range_add_blist(l2, 0, 2);
    blist *l2cpy=copyBList(l0);
    print_blist(l2cpy); freeBList(l2cpy);
 
    blist *l3=makeBList();
    range_add_blist(l3, 19, 25);
    blist *l3cpy=copyBList(l0);
    print_blist(l3cpy); freeBList(l3cpy);

    freeBList(l0), freeBList(l1), freeBList(l2), freeBList(l3);
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

    freeBList(l1);
}

void test_range() {
    blist *l1=makeBList();
    range_add_blist(l1, 4, 22);// idx[0,17)
    print_blist(l1); // [4,21]

    blist *l2=bListRange(l1, 3, 13); // idx[3,13)
    print_blist(l2); freeBList(l2); // [7,16]

    blist *l3=bListRange(l1, 0, 1); 
    print_blist(l3); freeBList(l3); // [4,4]

    blist *l4=bListRange(l1, 17, 18);
    print_blist(l4); freeBList(l4); // [21,21]

    freeBList(l1);
}

void test_linka(int i, int j, bool _after) {
    elem_t em;
    printf("tot=%d, insert %d, %s %d\n", j, j+i, _after?"after":"before",i);
    blist *bl = makeBList();
    bListSetElemCmp(bl, __elem_cmp_int);

    range_add_blist(bl, 0, j);
    print_blist(bl);

    setup_elem_i64(em, i+j); bnode *bn=makeBNode(NULL,em,NULL); 
    if (_after)
        bListInsertAfter(bl, bn, bListIndex(bl, i));
    else
        bListInsertBefore(bl, bn, bListIndex(bl, i));
    print_blist(bl);

    printf("-----------\n");

    freeBList(bl);
}

void test_link() {
    for(int j=1;j<=3;j++){
        for(int i=0;i<j;i++){
            test_linka(i, j, true);
            test_linka(i, j, false);
        }
    }
}

int main() {

    test_tail();
    test_head();
    test_splice();
    test_copy();

    test_search_index();
    test_range();

    test_link();

    return 0;
}