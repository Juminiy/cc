#include "../ctrlib/ctr_rbtree.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	// rbTreeSetElemSet(rb, elem_int_set);

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
		printf("%2ld ", get_elem_i64(rbn->_data));
	}
    printf("]\n");
	freeBIter(bi);
}


void test_trav(int *arr, int sz, int delval, rbTreeTrav travfn) {
	if (travfn == NULL)
		travfn = rbTreeMidTrav;

	elem_t elem_val; 
	rb_tree *rb = makeRBTree(elem_int_cmp);
	// rbTreeSetElemSet(rb, elem_int_set);

	printf("befor delete[%2d]: ", delval);
	for (int i=0;i<sz;i++){
		setup_elem_i64(elem_val, arr[i]); 
		rbTreeInsertData(rb, elem_val);
	}
	print_blist(travfn(rb));

	printf("after delete[%2d]: ", delval);
	setup_elem_i64(elem_val, delval); rbTreeDeleteData(rb, elem_val);
	print_blist(travfn(rb));

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
	test_trav(arr,6,2,NULL); // leaf
	test_trav(arr,6,4,NULL); // leaf
	test_trav(arr,6,9,NULL); // leaf

	int arr2[10]={5,3,8,2,9,10};
	test_trav(arr2,6,3,NULL); // only left
	test_trav(arr2,6,9,NULL); // only right

	int arr3[10]={5,3,8,2,9,7,4};
	test_trav(arr3,7,8,NULL); // lr-r
	test_trav(arr3,7,3,NULL); // lr-l
	test_trav(arr3,7,5,NULL); // lr root

	int arr4[10]={1,2,3,4,5,6,7,8,9,10};
	for(int i=1;i<=10;i++){
		test_trav(arr4, 10, i,NULL);
	}
}

void test_del_trav2() {
	int arr[15]={18,13,22,5,16,29,25,33,4,17,24,27,26,28};
	for(int idx=0;idx<14;idx++){
		test_trav(arr, 14, arr[idx], NULL);
	}

	// Foung bug
	// test_trav(arr, 14, 13, NULL);
}

typedef struct kvp{
	char *key;
	char *val;
} kvp;

kvp* newkvp(char *key,char*val) {
	kvp *kv = (kvp*)malloc(sizeof(kvp));
	kv->key=key;
	kv->val=val;
	return kv;
}

int elem_kvp_cmp(elem_t e0, elem_t e1) {
	kvp *kvp0 = (kvp*)get_elem_ptr(e0);
	kvp *kvp1 = (kvp*)get_elem_ptr(e1);
	return strcmp(kvp0->key, kvp1->key);
}

void elem_kvp_set(elem_t _old, elem_t _new) {
	kvp *oldkvp = (kvp*)get_elem_ptr(_old);
	kvp *newkvp = (kvp*)get_elem_ptr(_new);
	if(newkvp->val){
		if(oldkvp->val==NULL)
			oldkvp->val=malloc(strlen(newkvp->val));
		strcpy(oldkvp->val, newkvp->val);
	} else {
		free(oldkvp->val);
		oldkvp->val=NULL;
	}
}

void print_kvnode(rb_node *nd) {
	if(nd){
		kvp *kv=(kvp*)get_elem_ptr(nd->_data);
		printf("<%s:%s>\n", kv->key,kv->val?kv->val:"null");
	} else {
		printf("NULL\n");
	}
}

void test_ssmap() {
	rb_tree *rb = makeRBTree(elem_kvp_cmp);
	// rbTreeSetElemSet(rb, elem_kvp_set);

	elem_t elem_val; 
	rb_node *nd=NULL;

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); rbTreeInsertData(rb,elem_val); 						 // put-insert

	setup_elem_ptr(elem_val, newkvp("c++",NULL)); nd = rbTreeGetData(rb, elem_val); print_kvnode(nd);	 // get-notFound		NULL
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); nd = rbTreeGetData(rb, elem_val); print_kvnode(nd);	 // get					<mysql:null>

	setup_elem_ptr(elem_val, newkvp("mysql","hachimi")); rbTreeInsertData(rb,elem_val);					 // put-update			
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); nd = rbTreeGetData(rb, elem_val); print_kvnode(nd);	 // get					<mysql:hachimi>

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); rbTreeInsertData(rb,elem_val);					     // put-update			
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); nd = rbTreeGetData(rb, elem_val); print_kvnode(nd);	 // get					<mysql:null>

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); nd = rbTreeDeleteData(rb,elem_val); print_kvnode(nd);// delete				<mysql:null>
	setup_elem_ptr(elem_val, newkvp("c++",NULL)); nd = rbTreeDeleteData(rb,elem_val); print_kvnode(nd);	 // delete-notFound		<c++:null>
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); nd = rbTreeGetData(rb, elem_val); print_kvnode(nd);  // get-notFound		NULL

	freeRBTree(rb);
}

int main() {

	test_ssmap();

	return 0;
}