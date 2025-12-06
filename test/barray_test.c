#include <stdio.h>
#include <stdlib.h>

#include "../ctrlib/ctr_barray.h"

void print_intarr(barray arr) {
    printf("[ ");
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%lld ", get_elem_i64(bArrayAt(arr, i)));
    }
    printf("]\n");
}

void test_append_len_cap(barray arr, int sz){
    elem_t em;
    for(int i=0;i<sz;i++){
        setup_elem_i64(em,i); arr = bArrayAppend(arr, em);
        // printf("size=%zu, cap=%zu\n", bArrayLen(arr), bArrayCap(arr));
    }

    print_intarr(arr);

    printf("size=%zu, cap=%zu\n", bArrayLen(arr), bArrayCap(arr));
}

void test_array_modify(barray arr) {
    elem_t em;

    for(int i=0;i<8;i++) {
        setup_elem_i64(em,i); arr = bArrayAppend(arr, em);
    } // 0 1 2 3 4 5 6 7

    setup_elem_i64(em, 114); bArraySet(arr, 4, em); 
    setup_elem_i64(em, 514); bArraySet(arr, 0, em); 
    setup_elem_i64(em, 1919); bArraySet(arr, 7, em); 

    setup_elem_i64(em, 810); arr = bArrayInsert(arr, 0, em);
    setup_elem_i64(em, 33); arr = bArrayInsert(arr, 5, em);
    setup_elem_i64(em, 666); arr = bArrayInsert(arr, 7, em);

    arr = bArrayDeleteIndex(arr, 3);
    arr = bArrayDeleteIndex(arr, 6); // 810 514 1 3 33 114 5 6 1919

    bArrayReverse(arr);
    print_intarr(arr); // 1919 6 5 114 33 3 1 514 810

    barray acpy = bArrayCopy(arr);
    print_intarr(acpy); // 1919 6 5 114 33 3 1 514 810
    freeBArray(acpy);

    barray aslc = bArraySlice(arr, 3, 6, 9);
    print_intarr(aslc); // 114 33 3
    arr = bArrayExtend(arr, aslc); 

    freeBArray(aslc);

    print_intarr(arr);
}

void test_cmpof(barray arr) {
    elem_t em;
    for(int i=0;i<11;i++){
        setup_elem_i64(em, i); arr = bArrayAppend(arr, em);
    }
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%zu,%zu ", i, bArrayIndexOf(arr, bArrayAt(arr, i), 0));
    }
}

int main(int argc, char **argv) {
    int sz=1;
    if(argc>=2){
        sz = strtol(argv[1], NULL, 10);
    }

    barray arr = makeBArray(0, 8);
    bArraySetElemCmp(arr, __elem_cmp_int);

    test_append_len_cap(arr, sz);

    test_array_modify(arr);

    test_cmpof(arr);

    freeBArray(arr);

    return 0;
}