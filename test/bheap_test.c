#include "../ctrlib/ctr_bheap.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

void print_intarr(barray arr) {
    printf("[ ");
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%ld ", get_elem_i64(bArrayAt(arr, i)));
    }
    printf("]\n");
}

int elem_int_less(elem_t e0, elem_t e1) {
	return -__elem_cmp_int(e0,e1);
}

void test_bheap(int tot, int inc) {
	elem_t em;
	bheap bh = makeBHeap(inc > 0 ?__elem_cmp_int:elem_int_less, NULL);
	// int arr[14]={-1, 9, 6, 4, 3, 2, 5, 1, 18, 22, 3, 75, 99, -2};
	for(int i=0;i<tot;i++){
		setup_elem_i64(em, rand()); bh = bHeapPush(bh, em);
		assert(bHeapCheck(bh));
	}

	int prev=inc>0?0x7fffffff:0x80000000 ,cnt=0;
	while(!bHeapEmpty(bh)){
		// printf("%ld ", get_elem_i64(bHeapTop(bh)));
		cnt++;
		int cur = get_elem_i64(bHeapTop(bh));
		// inc>0, prev>cur; inc<0, prev<cur 
		if((inc>0&&prev<cur) || (inc<0&&prev>cur)) {
			printf("ERROR, cnt:%d, prev=%d, cur=%d\n", cnt, prev, cur);
			exit(1);
		}
		prev = cur;
		bh = bHeapPop(bh);
		assert(bHeapCheck(bh));
	}

	freeBHeap(bh);
}

void test_rm() {
	elem_t em;
	bheap bh = makeBHeap(elem_int_less, NULL);
	int arr[14]={-1, 9, 6, 4, 3, 2, 5, 1, 18, 22, 3, 75, 99, -2};
	for(int i=0;i<14;i++){
		setup_elem_i64(em, arr[i]); bh = bHeapPush(bh, em);
	}

	bh = bHeapRemove(bh, 5);
}	

int main(int argc, char **argv){
	srand(time(NULL));
	int tot=10, inc=+1;
	if(argc>1){
		tot = strtol(argv[1], NULL, 10);
	}
	if(argc>2){
		inc = strtol(argv[2], NULL, 10);
	}

	test_bheap(tot, inc);

	return 0;
}