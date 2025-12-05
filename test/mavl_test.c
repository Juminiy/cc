#include <stdio.h>

#include "../ctrlib/ctr_rbtree.h"

typedef struct strli64{
    char *key;
    int *arr;
}strli64;

strli64* maket(char *_key, int *_arr){
    strli64 *t=malloc(sizeof(strli64));
    t->key = __strdup(_key);
    t->arr = _arr;
}

strli64 freet(strli64* t){
    free(t->key);
    free(t->arr);
    free(t);
}

int cmp_t(elem_t e0, elem_t e1){
    strli64 *t0 = (strli64 *)get_elem_ptr(e0);
    strli64 *t1 = (strli64 *)get_elem_ptr(e1);
    return __strcmp(t0->key, t1->key);
}

elem_t mrg_t(elem_t e0, elem_t e1) {
    strli64 *t0 = (strli64 *)get_elem_ptr(e0);
    strli64 *t1 = (strli64 *)get_elem_ptr(e1);
    return e0;
}

void free_t(elem_t e0) {
    freet((strli64*)get_elem_ptr(e0));
}

int main() {

    rb_tree *rb=makeRBTree(cmp_t);
    setRBTreeDataFree(rb, free_t);
    setRBTreeNodeType(rb, TREE_NODE_TYPE_MAVL);

    
    return 0;
}