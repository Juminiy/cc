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

int main() {
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


	return 0;
}