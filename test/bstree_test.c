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
		elem_t _em; \
		setup_elem_i64(_em, ival); \
		_em = rbTreeGetData(rb, _em);\
		if(valid_elem_t(_em)) { \
			printf("%d Found\n", ival); \
		} else { \
			printf("%d Not Found\n", ival); \
		} \
	} while(0)

// 		5
//	1		18
// 0 4	  9		22

void test_insert_found() {
	rb_tree *rb = makeRBTree(elem_int_cmp);

	printf("put data\n");

	elem_t elem_val; 
	setup_elem_i64(elem_val, 5); rbTreeInsertData(rb, elem_val);
	setup_elem_i64(elem_val, 1); rbTreeInsertData(rb, elem_val);
	setup_elem_i64(elem_val, 18); rbTreeInsertData(rb, elem_val);

	test_found(rb, 3); // NotFound
	test_found(rb, 5); // Found
	test_found(rb, 7); // NotFound
	test_found(rb, 1); // Found
	test_found(rb, 18); // Found

	printf("\nafter delete\n");

	setup_elem_t(elem_val, 0, i64, 5); rbTreeDeleteData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 1); rbTreeDeleteData(rb, elem_val);
	setup_elem_t(elem_val, 0, i64, 18); rbTreeDeleteData(rb, elem_val);

	test_found(rb, 3);
	test_found(rb, 5);
	test_found(rb, 7);
	test_found(rb, 1);
	test_found(rb, 18);

	freeRBTree(rb);
}

void print_blist(blist *bl) {
    printf("LIS:blist=[ ");
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi)){
		rb_node *rbn = get_elem_ptr(bn->_data); 
		printf("%2ld ", get_elem_i64(rbn->_data));
	}
    printf("]\n");
	freeBIter(bi);
	freeBList(bl);
}


void test_trav(int *arr, int sz, int delval, rbTreeTrav travfn) {
	if (travfn == NULL)
		travfn = rbTreeMidTrav;

	elem_t elem_val; 
	rb_tree *rb = makeRBTree(elem_int_cmp);

	printf("befor delete[%2d]: ", delval);
	for (int i=0;i<sz;i++){
		// printf("insert elem: %d\n", arr[i]);
		setup_elem_i64(elem_val, arr[i]); 
		rbTreeInsertData(rb, elem_val);
	}
	print_blist(travfn(rb));

	printf("after delete[%2d]: ", delval);
	setup_elem_i64(elem_val, delval); rbTreeDeleteData(rb, elem_val);
	print_blist(travfn(rb));

	freeRBTree(rb);
}

// void test_link_unlink() {
// 	elem_t emval={.tag=0};
// 	rb_node * n0 = makeRBNode(NULL,NULL,emval);
// 	rb_node * n1 = makeRBNode(NULL,NULL,emval);
// 	rb_node * n2 = makeRBNode(NULL,NULL,emval);
// }

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
	for(int i=0;i<10;i++){
		test_trav(arr4, 10, arr4[i], NULL);
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
	kv->key=__strdup(key);
	kv->val=__strdup(val);
	return kv;
}

int elem_kvp_cmp(elem_t e0, elem_t e1) {
	kvp *kvp0 = (kvp*)get_elem_ptr(e0);
	kvp *kvp1 = (kvp*)get_elem_ptr(e1);
	return __strcmp(kvp0->key, kvp1->key);
}

// void elem_kvp_set(elem_t _old, elem_t _new) {
// 	kvp *oldkvp = (kvp*)get_elem_ptr(_old);
// 	kvp *newkvp = (kvp*)get_elem_ptr(_new);
// 	if(newkvp->val){
// 		if(oldkvp->val==NULL)
// 			oldkvp->val=malloc(strlen(newkvp->val));
// 		strcpy(oldkvp->val, newkvp->val);
// 	} else {
// 		free(oldkvp->val);
// 		oldkvp->val=NULL;
// 	}
// }

void elem_kvp_free(elem_t _e) {
	kvp *kv=(kvp*)(get_elem_ptr(_e));
	free(kv->key);
	free(kv->val);
	free(kv);
}

// void print_kvnode(rb_node *nd) {
// 	if(nd){
// 		kvp *kv=(kvp*)get_elem_ptr(nd->_data);
// 		printf("<%s:%s>\n", kv->key,kv->val?kv->val:"null");
// 	} else {
// 		printf("NULL\n");
// 	}
// }

void print_kvp_elem(elem_t _e) {
	if(valid_elem_t(_e)){
		kvp *kv=(kvp*)get_elem_ptr(_e);
		printf("<%s:%s>\n", kv->key?kv->key:"null",kv->val?kv->val:"null");
	} else {
		printf("NULL\n");
	}
}

void test_ssmap() {
	rb_tree *rb = makeRBTree(elem_kvp_cmp);
	setRBTreeNodeType(rb, TREE_NODE_TYPE_BS);
	setRBTreeDataFree(rb, elem_kvp_free);

	elem_t elem_val; 
	rb_node *nd=NULL;
	#define get_print() \
		do {elem_val=rbTreeGetData(rb, elem_val); print_kvp_elem(elem_val);} while(0)

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); rbTreeInsertData(rb,elem_val); 				// put-insert

	setup_elem_ptr(elem_val, newkvp("c++",NULL)); get_print();									// get-notFound			NULL
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); get_print();	 							// get					<mysql:null>

	setup_elem_ptr(elem_val, newkvp("mysql","hachimi")); rbTreeInsertData(rb,elem_val);			// put-update			
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); get_print();	 							// get					<mysql:hachimi>

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); rbTreeInsertData(rb,elem_val);				// put-update			
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); get_print();	 							// get					<mysql:null>

	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); rbTreeDeleteData(rb,elem_val); get_print();	// delete				NULL
	setup_elem_ptr(elem_val, newkvp("c++",NULL)); rbTreeDeleteData(rb,elem_val); get_print();	// delete-notFound		NULL
	setup_elem_ptr(elem_val, newkvp("mysql",NULL)); get_print();  								// get-notFound			NULL

	setup_elem_ptr(elem_val, newkvp("",NULL)); rbTreeInsertData(rb,elem_val); get_print();
	setup_elem_ptr(elem_val, newkvp(NULL,NULL)); rbTreeInsertData(rb,elem_val); get_print();

	freeRBTree(rb);
}

void test_del_trav_bug() {
	int arr4[10]={1,2,3,4,5,6,7,8,9,10};
	for(int i=0;i<10;i++){
		test_trav(arr4, 10, arr4[i], NULL);
	}
}

#include <time.h>
#include <stdlib.h>
#include <math.h>

void check_order(blist *bl){
    biter *bi = makeBIter(bl, BLIST_ITER_FORWARD);
    int64_t prev_val = -1;
    for(bnode *bn = bListNext(bi); bn; bn = bListNext(bi)){
		rb_node *rbn = get_elem_ptr(bn->_data); 
        int64_t _cur_i64 = get_elem_i64(rbn->_data);
        if(_cur_i64<prev_val){
            printf("[ERROR]: %ld < %ld\n", _cur_i64, prev_val);
            break;
        }
        prev_val=_cur_i64;
	}
	freeBIter(bi);
	freeBList(bl);
}

void test_bstree_delete(int tot_sz) {
	elem_t elem_val; 
	rb_tree *rb = makeRBTree(elem_int_cmp);
	setRBTreeNodeType(rb, TREE_NODE_TYPE_BS);
	int arr[tot_sz+1];
	for (int i=0;i<tot_sz;i++){
		arr[i]=rand();
		setup_elem_i64(elem_val, arr[i]); rbTreeInsertData(rb, elem_val);
	}

	printf("insert: %d, tree_size: %ld, tree_height: %ld, log_2(%d)=%.2f\n", 
        tot_sz, __tree_size(rb), __tree_height(rb), tot_sz, log2(tot_sz*1.0));

	for (int i=0;i<tot_sz;i++){
		setup_elem_i64(elem_val, arr[i]); rbTreeDeleteData(rb, elem_val);
		check_order(rbTreeMidTrav(rb));
	}
	
	freeRBTree(rb);
}

void test_del_trav_mem_bug() {
	int arr[10]={5,3,8,2,4,9};
	test_trav(arr,6,2,NULL); // leaf
	test_trav(arr,6,4,NULL); // leaf
	test_trav(arr,6,9,NULL); // leaf
}

int main(int argc, char **argv) {	
	srand(time(NULL));
	int tot_sz = 10;
	if(argc>=2){
		tot_sz = strtol(argv[1], NULL, 10);
	}
	test_bstree_delete(tot_sz);

	test_insert_found();
	test_del_trav();
	test_del_trav2();
	test_del_trav_bug();
	test_del_trav_mem_bug();

	test_ssmap();

	return 0;
}