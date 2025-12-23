#include <stdio.h>
#include <stdlib.h>

#include "../ctrlib/ctr_bitmap.h"

bool printnum(int64_t vi){
    // printf("%ld ", vi);
    return true;
}

int main(int argc, char **argv) {
    int cnt=0;
    if(argc>1)
        cnt = strtol(argv[1], NULL, 10);

// 0/64  = 0, 0%64  = 0
// 33/64 = 0, 33%64 = 33
// 64/64 = 1, 64%64 = 0
// 88/64 = 1, 88%64 = 24

    bitmap *bm = makeBitMap(cnt);
    for(int i=0;i<cnt;i++)
        bitMapInsert(bm, i);
    
    // for(int idx=0;idx<bm->_size;idx++)
    //     printf("%lx ", bm->_bits[idx]);
    // puts("");

    bitMapIter(bm, printnum);
    puts("");

    freeBitMap(bm);

    return 0;
}