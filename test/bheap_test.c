#include "../ctrlib/ctr_bheap.h"

void print_intarr(barray arr) {
    printf("[ ");
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%lld ", get_elem_i64(bArrayAt(arr, i)));
    }
    printf("]\n");
}

int main(){
	elem_t em;
	bheap bh = makeBHeap(__elem_cmp_int);

	int arr[10]={8,5,3,5,1,3,8,9,2};
	for(int i=0;i<9;i++){
		setup_elem_i64(em, arr[i]); bh = bHeapPush(bh, em);
		print_intarr(bh._ll);
	}

	while(!bHeapEmpty(bh)){
		printf("%lld \n", get_elem_i64(bHeapTop(bh)));
		bh = bHeapPop(bh);
		print_intarr(bh._ll);
	}


	freeBHeap(bh);

	return 0;
}