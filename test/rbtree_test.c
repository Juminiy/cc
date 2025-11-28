#include "../ctrlib/ctr_rbtree.h"

#include <stdio.h>

int elem_int_cmp(elem_t _e0, elem_t _e1) {
	return _e0.uni.i64-_e1.uni.i64;
}

void elem_int_set(elem_t _old, elem_t _new) {
	return ;
}

#define test_found(rb, ival) \
	do { \
		elem_t elem_val; \
		setup_elem_t(elem_val, 0, i64, ival); \
		if(rbTreeGetData(rb, elem_val)==NULL) { \
			printf("%d Not Found\n", ival); \
		} else { \
			printf("%d Found\n", ival); \
		} \
	} while(0)

// 		5
//	1		18
// 0 4	  9		22

void test_insert_found() {
	rb_tree *rb = makeRBTree(elem_int_cmp);
	rbTreeSetElemSet(rb, elem_int_set);

	printf("put data\n");

	elem_t elem_val; 
	setup_elem_t(elem_val, 0, i64, 5); rbTreeInsertData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 1); rbTreeInsertData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 18); rbTreeInsertData(rb, elem_val);

	test_found(rb, 3);
	test_found(rb, 5);
	test_found(rb, 7);
	test_found(rb, 1);
	test_found(rb, 18);

	printf("\nafter delete\n");

	setup_elem_t(elem_val, 0, i64, 5); rbTreeDeleteData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 1); rbTreeDeleteData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 18); rbTreeDeleteData(rb, elem_val);

	test_found(rb, 3);
	test_found(rb, 5);
	test_found(rb, 7);
	test_found(rb, 1);
	test_found(rb, 18);
}

void print_blist(blist *bl) {
    printf("LIS:blist=[ ");
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi)){
		rb_node *rbn = bNodeData(bn).ptr;
		printf("%ld ", get_elem_i64(rbn->_data));
	}
    printf("]\n");
}


void test_trav(int *arr, int sz, int delval) {
	elem_t elem_val; 
	rb_tree *rb = makeRBTree(elem_int_cmp);
	rbTreeSetElemSet(rb, elem_int_set);

	printf("before delete: ");
	for (int i=0;i<sz;i++){
		setup_elem_i64(elem_val, arr[i]); 
		rbTreeInsertData(rb, elem_val);
	}
	print_blist(rbTreeLelTrav(rb));

	printf("after delete[%d]: ", delval);
	setup_elem_i64(elem_val, delval); rbTreeDeleteData(rb, elem_val);
	print_blist(rbTreeLelTrav(rb));

	freeRBTree(rb);
}

void test_link_unlink() {
	elem_t emval={.tag=0};
	rb_node * n0 = makeRBNode(NULL,NULL,emval);
	rb_node * n1 = makeRBNode(NULL,NULL,emval);
	rb_node * n2 = makeRBNode(NULL,NULL,emval);
}

void test_del_trav() {
	int arr[10]={5,3,8,2,4,9};
	test_trav(arr,6,2); // leaf
	test_trav(arr,6,4); // leaf
	test_trav(arr,6,9); // leaf

	int arr2[10]={5,3,8,2,9,10};
	test_trav(arr2,6,3); // only left
	test_trav(arr2,6,9); // only right

	int arr3[10]={5,3,8,2,9,7,4};
	test_trav(arr3,7,8); // lr-r
	test_trav(arr3,7,3); // lr-l
	test_trav(arr3,7,5); // lr root

	int arr4[10]={1,2,3,4,5,6,7,8,9,10};
	for(int i=1;i<=10;i++){
		test_trav(arr4, 10, i);
	}
}

void test_bfs() {

}

int main() {

	test_del_trav();

	return 0;
}