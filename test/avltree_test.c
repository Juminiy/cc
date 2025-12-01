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
		rb_node *rbn = bNodeData(bn).ptr;   int64_t _cur_i64 = get_elem_i64(rbn->_data);
        rb_node *rbn_parent = rbn->_parent; int64_t _par_i64 = rbn_parent?get_elem_i64(rbn_parent->_data):-1;
		printf("%2ld(p=%2ld)", _cur_i64, _par_i64);//(size=%2ld,height=%2ld) rbn->_size, rbn->_height
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
    setTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    for(int i=0;i<arr_sz;i++){
        setup_elem_i64(_em, arr[i]); rbTreeInsertData(rb, _em);
    }   
    // printf("befor: ");
    print_blist(rbTreeLelTrav(rb));

}

void test_avl_sorted(int tot_cnt) {
    srand(time(NULL));
    elem_t _em;
    rb_tree *rb = makeRBTree(elem_int_cmp);
    setTreeNodeType(rb, TREE_NODE_TYPE_AVL);
    for(int idx=0;idx<tot_cnt;idx++){
        setup_elem_i64(_em, rand()); rbTreeInsertData(rb, _em);
    }
    printf("insert: %d, tree_size: %ld, tree_height: %ld, log_2(%d)=%.2f\n", 
        tot_cnt, __tree_size(rb), __tree_height(rb), tot_cnt, log2(tot_cnt*1.0));

    // check order
    // check_order(rbTreeMidTrav(rb));

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

    return 0;
}