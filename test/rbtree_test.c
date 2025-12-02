#include <stdio.h>

#include "../ctrlib/ctr_rbtree.h"

int elem_int_cmp(elem_t _e0, elem_t _e1) {
	return _e0.uni.i64-_e1.uni.i64;
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

void test_rotate() {
	elem_t _em;
	rb_tree *tr=makeRBTree(elem_int_cmp);
	int arr[14]={18,13,22,5,16,29,25,33,4,17,24,27,26,28};
	
	for(int i=0;i<14;i++){
		setup_elem_i64(_em, arr[i]);
		rbTreeInsertData(tr, _em);
	}
	print_blist(rbTreeLelTrav(tr));

	tr->_root = rotate_right(tr->_root);
	print_blist(rbTreeLelTrav(tr));

	freeRBTree(tr);
}

void test_rb_node() {
	elem_t _em;
	rb_tree *tr=makeRBTree(elem_int_cmp);
	setTreeNodeType(tr, TREE_NODE_TYPE_RB);
	int arr[14]={18,13,22,5,16,29,25,33,4,17,24,27,26,28};
	
	for(int i=0;i<14;i++){
		setup_elem_i64(_em, arr[i]); rbTreeInsertData(tr, _em);
	}
	
	printf("rbtree: size=%zu, height:%zu\n", __tree_size(tr), __tree_height(tr));
	print_blist(rbTreeMidTrav(tr));
	print_blist(rbTreeLelTrav(tr));

	freeRBTree(tr);
}

int main() {

	test_rb_node();
	
	return 0;
}