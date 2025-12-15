#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>

#include "../ctrlib/ctr_barray.h"

void print_intarr(barray arr) {
    printf("[ ");
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%ld ", get_elem_i64(bArrayAt(arr, i)));
    }
    printf("]\n");
}

void test_append_len_cap(int sz){
    barray arr = makeBArray(0, 8);
    bArraySetElemCmp(arr, __elem_cmp_int);

    elem_t em;
    for(int i=0;i<sz;i++){
        setup_elem_i64(em,i); arr = bArrayAppend(arr, em);
        // printf("size=%zu, cap=%zu\n", bArrayLen(arr), bArrayCap(arr));
    }

    print_intarr(arr);

    printf("size=%zu, cap=%zu\n", bArrayLen(arr), bArrayCap(arr));
    freeBArray(arr);
}

void test_array_modify() {
    barray arr = makeBArray(0, 8);
    bArraySetElemCmp(arr, __elem_cmp_int);

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
    freeBArray(arr);
}

void test_cmpof() {
    barray arr = makeBArray(0, 8);
    bArraySetElemCmp(arr, __elem_cmp_int);

    elem_t em;
    for(int i=0;i<11;i++){
        setup_elem_i64(em, i); arr = bArrayAppend(arr, em);
    }
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%zu,%zu ", i, bArrayIndexOf(arr, bArrayAt(arr, i), 0));
    }
    freeBArray(arr);
    printf("\n");
}

typedef struct _ipairsort {
    int k,v,m,n;
} _ipairsort;

_ipairsort* makei(int k, ...) {
    _ipairsort *_i=(_ipairsort*)malloc(sizeof(_ipairsort));
    va_list _args;
    va_start(_args, 3);
    _i->k=k;
    _i->v=va_arg(_args, int);
    _i->m=va_arg(_args, int);
    _i->n=va_arg(_args, int);
    va_end(_args);
    return _i;
}

int __compar_fn_elem_i (const void * e0, const void * e1) {
    elem_t _e0 = *(elem_t*)e0, _e1 = *(elem_t*)e1;
    _ipairsort *p0=(_ipairsort*)get_elem_ptr(_e0);
    _ipairsort *p1=(_ipairsort*)get_elem_ptr(_e1);
    return p0->k - p1->k;
}

void test_sortof(int n) {
    barray arr = makeBArray(0, n);
    elem_t em;
    _ipairsort *ptrarr[n];
    for(int i=0;i<n;i++){
        _ipairsort *curptr=makei(rand(), 1, 0, 0);
        setup_elem_ptr(em, curptr);
        arr = bArrayAppend(arr, em);
        ptrarr[i]=curptr;
    }

    qsort(arr._arr, n, sizeof(elem_t), __compar_fn_elem_i);
    int _cmpb = 0x80000000;
    for(int i=0;i<n;i++){
        _ipairsort *curptr = (_ipairsort*)get_elem_ptr(bArrayAt(arr, i));
        assert(_cmpb <= curptr->k);
        _cmpb = curptr->k;
    }
    freeBArray(arr);
    for(int i=0;i<n;i++)
        free(ptrarr[i]);
}

void test_array_index(int n) {
    barray ba = makeBArray(0, 0);
    elem_t em;
    for(int i=0;i<n;i++) {
        setup_elem_i64(em, i); 
        ba = bArrayAppend(ba, em);
    }

    for(int i=-1;i>=-n;i--){
        elem_t _iem = bArrayAt(ba, i);
        elem_t iem = bArrayAt(ba, bArrayLen(ba)+i); 
        printf("at:[%d]: %ld; at:[%zu]: %ld\n", 
            i, get_elem_i64(_iem), 
            bArrayLen(ba)+i, get_elem_i64(iem)
        );
    }

    for(int i=-1;i>=-n;i--){
        setup_elem_i64(em, i);
        bArraySet(ba, i, em);
    }

    for(int i=-1;i>=-n;i--){
        elem_t _iem = bArrayAt(ba, i);
        elem_t iem = bArrayAt(ba, bArrayLen(ba)+i); 
        printf("at:[%d]: %ld; at:[%zu]: %ld\n", 
            i, get_elem_i64(_iem), 
            bArrayLen(ba)+i, get_elem_i64(iem)
        );
    }

    bArrayReverse(ba);
    print_intarr(ba);

    freeBArray(ba);
}

void test_array_insert_delete(int n) {
    barray ba = makeBArray(0,0);
    elem_t em; 
    for(int i=0;i<n;i++){
        setup_elem_i64(em, i);
        ba = bArrayInsert(ba, 0, em);
        // 2 1 0 1 2 0
        // ba = bArrayInsert(ba, bArrayLen(ba)-1, em);
    }
    print_intarr(ba);

    // for(int i=0;i<n;i++) {
    //     ba = bArrayDeleteIndex(ba, 0);
    // }
    // print_intarr(ba);

    barray ba2 = makeBArray(0,0);
    for(int i=0;i<n;i++){
        setup_elem_i64(em, i);
        ba2 = bArrayAppend(ba2, em);
    }
    ba = bArrayExtend(ba, ba2);
    print_intarr(ba2);
    print_intarr(ba);

    freeBArray(ba2);
    freeBArray(ba);
}

void test_array_indexof(int n ) {
    int iarr[11]={1,3,5,7,9, 1,3,5,7,7,9};
    barray ba= makeBArray(0,0);
    bArraySetElemCmp(ba, __elem_cmp_int);

    elem_t em;
    for(int i=0;i<11;i++){
        setup_elem_i64(em, iarr[i]);
        ba = bArrayAppend(ba, em);
    }

    for(int i=0;i<11;i++){
        size_t idx=0;
        setup_elem_i64(em, iarr[i]);
        printf("%d ", iarr[i]);
        idx = bArrayIndexOf(ba, em, idx);
        printf("in:[%zu] \n",idx);
        // printf("%s\n", cnt>0?"":"NOTFOUND");
    }

    printf("------\n");

    for(int i=-1;i>=-11;i--){
        em = bArrayAt(ba, i);
        printf("%ld ", get_elem_i64(em));
        size_t idx = bArrayIndexOf(ba, em, -1);
        printf("in:[%ld] \n",idx!=-1?idx:-1);
        // printf("%s\n", cnt>0?"":"NOTFOUND");
    }

    freeBArray(ba);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int sz=1;
    if(argc>=2){
        sz = strtol(argv[1], NULL, 10);
    }
    
    test_append_len_cap(sz);

    test_array_modify();

    test_cmpof();

    test_sortof(sz);

    test_array_index(sz);

    test_array_insert_delete(sz);

    test_array_indexof(sz);

    return 0;
}