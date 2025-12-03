#include <stdio.h>

#include "../ctrlib/ctr_rbtree.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

int elem_int_cmp(elem_t _e0, elem_t _e1) {
	return _e0.uni.i64-_e1.uni.i64;
}

void print_blist(blist *bl) {
    printf("LIS:blist=[ ");
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi)){
		rb_node *rbn = bNodeData(bn).ptr;   
        int64_t _cur_i64 = get_elem_i64(rbn->_data);
        // rb_node *rbn_parent = rbn?rbn->_parent:NULL; 
        // int64_t _par_i64 = rbn_parent?get_elem_i64(rbn_parent->_data):-1;
		printf("%2ld ", _cur_i64);//(p=%2ld,size=%2ld,height=%2ld) _par_i64, rbn->_size, rbn->_height,  
        // printf("%2ld(ptr=%p) ",_cur_i64,rbn);
	}
    printf("]\n");
	freeBIter(bi);
}

void check_order(blist *bl){
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    int64_t prev_val = -1;
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi)){
		rb_node *rbn = bNodeData(bn).ptr;   
        int64_t _cur_i64 = get_elem_i64(rbn->_data);
        if(_cur_i64<prev_val){
            printf("[ERROR]: %ld < %ld\n", _cur_i64, prev_val);
            break;
        }
        prev_val=_cur_i64;
	}
}

typedef rb_node*(*rotate_fn)(rb_node*);
void test_rotate_fn(int *arr, int arr_sz, rotate_fn _fn) {
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    for(int i=0;i<arr_sz;i++){
        setup_elem_i64(_em, arr[i]); rbTreeInsertData(rb, _em);
    }   
    printf("befor: ");
    print_blist(rbTreeLelTrav(rb));

    printf("after: ");
    rb->_root = _fn(rb->_root);
    print_blist(rbTreeLelTrav(rb));

    freeRBTree(rb);
}


void test_rotate() {
    // bf(5)=2, bf(8)=1
    //      5                                   8
    //  1       8                  ->       5       15     
    //        7     15                  1     7         18
    //                  18
    int arr0[10] = {5,1,8,7,15,18};
    test_rotate_fn(arr0, 6, rotate_ll);

    // bf(5)=2, bf(8)=1
    //      5                                   5                       7
    //  1       8                  ->         1     7           ->    5     8   
    //        7     15                            6   8             1   6       15 
    //      6                                           15
    int arr1[10] = {5,1,8,7,15,6};
    test_rotate_fn(arr1, 6, rotate_rl);

    // bf(15)=-2, bf(8)=-1
    //          15                              8
    //      8       22          ->            5     15
    //   5      9                           1      9   22
    // 1                
    int arr2[10] = {15,8,22,5,9,1};
    test_rotate_fn(arr2, 6, rotate_rr);

    // bf(15)=-2, bf(8)=1
    //          15                              15                      9
    //      8       22                      9       22               8      15
    //   5      9               ->       8     13           ->     5      13    22       
    //            13                   5
    int arr3[10] = {15,8,22,5,9,13};
    test_rotate_fn(arr3, 6, rotate_lr);


    // 1
    //   2
    //     3
    int arr4[10]={1,2,3};
    test_rotate_fn(arr4, 3, rotate_ll);

    int arr5[10]={3,2,1};
    test_rotate_fn(arr5, 3, rotate_rr);

    int arr6[10]={3,1,2};
    test_rotate_fn(arr6, 3, rotate_lr);
    
    int arr7[10]={1,3,2};
    test_rotate_fn(arr7, 3, rotate_rl);
}


void test_avl_tree(int *arr, int arr_sz) {
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    setRBTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    for(int i=0;i<arr_sz;i++){
        setup_elem_i64(_em, arr[i]); rbTreeInsertData(rb, _em);
    }   
    // printf("befor: ");
    print_blist(rbTreeLelTrav(rb));

    freeRBTree(rb);
}

void test_avl_sorted(int tot_cnt) {
    srand(time(NULL));
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    setRBTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    int arr[tot_cnt+1]; int arr_sz=0;
    for(int idx=0;idx<tot_cnt;idx++){
        int genrand = rand();
        setup_elem_i64(_em, genrand); 
        if(rbTreeInsertData(rb, _em)==RB_NODE_INSERT_CREATED){
            arr[arr_sz++]=genrand;
        }
    }
    printf("insert: %d, tree_size: %ld, tree_height: %ld, log_2(%d)=%.2f\n", 
        tot_cnt, __tree_size(rb), __tree_height(rb), tot_cnt, log2(tot_cnt*1.0));

    // check order
    check_order(rbTreeMidTrav(rb));
    
    int szof=__tree_size(rb);
    for(int idx=0;idx<tot_cnt/2;idx++){
        szof--;
        setup_elem_i64(_em, arr[idx]); rbTreeDeleteData(rb, _em);
        if(__tree_size(rb)!=szof){
            printf("ERROR size:%zu, real=%d\n", __tree_size(rb),szof);
            exit(1);
        }
    }
    printf("after delete, tree_size: %ld, tree_height: %ld, log_2(%d)=%.2f\n", 
        __tree_size(rb), __tree_height(rb), tot_cnt/2, log2(tot_cnt*1.0/2.0));
    // check order
    check_order(rbTreeMidTrav(rb));
    

    freeRBTree(rb);
}

void test_avl_delete(int *arr, size_t arr_sz, int del_val) {
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    // printf("tree=%p(root=%p)\n", rb, rb->_root);
    setRBTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    for(int i=0;i<arr_sz;i++){
        // printf("insert: %d\n", arr[i]);
        setup_elem_i64(_em, arr[i]); rbTreeInsertData(rb, _em);
    }   
    printf("befor del[%2d]: ", del_val);
    print_blist(rbTreeMidTrav(rb));
    // print_blist(rbTreeLelTrav(rb));

    setup_elem_i64(_em, del_val); rbTreeDeleteData(rb, _em);
    printf("after del[%2d]: ", del_val);
    print_blist(rbTreeMidTrav(rb));
    // print_blist(rbTreeLelTrav(rb));

    freeRBTree(rb);
}

void valid_delete(int *arr, size_t arr_sz, int del_val) {
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    setRBTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    for(int i=0;i<arr_sz;i++){
        setup_elem_i64(_em, arr[i]); rbTreeInsertData(rb, _em);
    }       

    size_t before_sz = __tree_size(rb);
    setup_elem_i64(_em, del_val); rbTreeDeleteData(rb, _em);
    setup_elem_i64(_em, del_val); _em = rbTreeGetData(rb, _em); 
    printf("[%d] %s, ", del_val, valid_elem_t(_em)?"delete fail":"delete ok");
    size_t after_sz = __tree_size(rb);
    if(before_sz-after_sz!=1){
        printf("ERROR\n");
        exit(1);
    }
    printf("size=%zu -> size=%zu, sub:%ld\n", before_sz, after_sz, before_sz-after_sz);
    freeRBTree(rb);
}

int main(int argc, char **argv) {

    // report: after rotate nd->_parent is bug
    // same bug as: rb_tree insert 1,2,3,4,5 bug
    // caused by: nd->_parent ERROR bug, if _nd is _root, set returned value _parent=NULL;
    // test_rotate();

    // int arr0[10]={1,2,3,4,5,6};
    // test_avl_tree(arr0, 6);

    // int arr1[10]={6,5,4,3,2,1};
    // test_avl_tree(arr1, 6);

    int t_sz = 100;
    if(argc>=2){
        t_sz = strtol(argv[1], NULL, 10);
    }
    test_avl_sorted(t_sz);

    //      8
    //    5   15
    //  1  7     18
    // int arr[10]={1,5,7,8,15,18};
    // for(int idx=0;idx<6;idx++) {
    //     test_avl_delete(arr, 6, arr[idx]); // bug report: function stack memory pointer to heap location reused, same address no freed, memleak! 
    // }

    //     int arr1[7]= {10, 5, 15, 3, 7, 12, 18};
    // int arr2[12] = {30, 20, 40, 10, 25, 35, 50, 5, 15, 28, 45, 60};
    // int arr3[15] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    // int arr4[9] = {9, 5, 10, 0, 6, 11, -1, 1, 2};
    // for(int i=0;i<7;i++)
    //     valid_delete(arr1, 7, arr1[i]);
    // for(int i=0;i<12;i++)
    //     valid_delete(arr2, 12, arr2[i]);
    // for(int i=0;i<15;i++)
    //     valid_delete(arr3, 15, arr3[i]);
    // for(int i=0;i<9;i++)
    //     valid_delete(arr4, 9, arr4[i]);

    // test_avl_delete(arr4, 9, 1);

    return 0;
}