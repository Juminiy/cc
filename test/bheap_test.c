#include "../ctrlib/ctr_bheap.h"

void print_intarr(barray arr) {
    printf("[ ");
    for(size_t i=0;i<bArrayLen(arr);i++){
        printf("%ld ", get_elem_i64(bArrayAt(arr, i)));
    }
    printf("]\n");
}

int main(){
	elem_t em;
	bheap bh = makeBHeap(__elem_cmp_int);

	int arr[14]={-1, 9, 6, 4, 3,2,5, 1, 18, 22, 3, 75,99,-2};
	for(int i=0;i<14;i++){
		setup_elem_i64(em, arr[i]); bh = bHeapPush(bh, em);
	}

	while(!bHeapEmpty(bh)){
		printf("%ld \n", get_elem_i64(bHeapTop(bh)));
		bh = bHeapPop(bh);
	}

	freeBHeap(bh);

	return 0;
}