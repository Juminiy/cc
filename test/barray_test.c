#include <stdio.h>
#include <stdlib.h>

#include "../ctrlib/ctr_barray.h"

int main(int argc, char **argv) {
    int sz=1;
    if(argc>=2){
        sz = strtol(argv[1], NULL, 10);
    }

    barray arr = makeBArray(5, 10);
    bArraySetElemCmp(arr, __elem_cmp_int);
    elem_t em;

    for(int i=0;i<sz;i++){
        setup_elem_i64(em,i); 
        arr = bArrayAppend(arr, em);
        printf("size=%zu, cap=%zu\n", bArrayLen(arr), bArrayCap(arr));
    }

    freeBArray(arr);

    return 0;
}