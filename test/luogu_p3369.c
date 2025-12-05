#include "../ctrlib/ctr_rbtree.h"

int int_cmp(elem_t e0, elem_t e1){
	return e0.uni.i64 - e1.uni.i64;
}

int main() {

	rb_tree *tr = makeRBTree(int_cmp);
	setRBTreeNodeType(tr, TREE_NODE_TYPE_MAVL);
	elem_t em;

	int n=0;
	scanf("%d",&n);
	while(n--){
		int a,b;
		scanf("%d%d",&a,&b);
		switch(a){
		case 1: // insert(b,1)
		setup_elem_i64(em, b); rbTreeInsertData(tr, em);
		break;

		case 2: // delete(b,1)
		setup_elem_i64(em, b); rbTreeDeleteData(tr, em);
		break;

		case 3: // rank(b), maynot exist
		setup_elem_i64(em, b);
		printf("%zu\n", bsDataRank(tr, em));
		break;

		case 4: // rank_k(b)
		em = bsDataRankKData(tr, b);
		printf("%ld\n", get_elem_i64(em));
		break;

		case 5: // prev(b), maynot exist
		setup_elem_i64(em, b); em = bsDataPrevData(tr, em);
		printf("%ld\n", get_elem_i64(em));
		break;

		case 6: // next(b), maynot exist
		setup_elem_i64(em, b); em = bsDataNextData(tr, em);
		printf("%ld\n", get_elem_i64(em));
		break;
		}
	}

	freeRBTree(tr);
	
	return 0;
}