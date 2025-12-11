#include "../ctrlib/ctr_lru.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct ii{
    int k,v;
} ii;

ii* makeii(int k,int v){
    ii *iikv=(ii*)malloc(sizeof(ii));
    iikv->k=k,iikv->v=v;
    return iikv;
}

int cmpii(elem_t e0, elem_t e1) {
    ii *i0=(ii*)get_elem_ptr(e0);
    ii *i1=(ii*)get_elem_ptr(e1);
    return i0->k-i1->k;
}

void freeii(elem_t e){
    ii *iikv=(ii*)get_elem_ptr(e);
    free(iikv);
}

int main() {

    #define get_i(_ikey) \
        do { \
            ii __iikv; __iikv.k=_ikey,__iikv.v=0; \
            setup_elem_ptr(em, &__iikv); \
            void *ptr=get_elem_ptr(lruTGet(t, em)); \
            ii *iikv=ptr?(ii*)ptr:NULL; \
            printf("%d\n", iikv?iikv->v:-1); \
        } while(0)

    elem_t em;
    lru_t *t = makeLRUT(cmpii, 2); // []
    lruTSetElemFree(t, freeii);

    setup_elem_ptr(em, makeii(1,1)); lruTPut(t, em); // [(1,1)]
    setup_elem_ptr(em, makeii(2,2)); lruTPut(t, em); // [(2,2),(1,1)]

    get_i(1); // 1

    setup_elem_ptr(em, makeii(3,3)); lruTPut(t, em); // [(3,3),(2,2)]

    get_i(2); // 2

    setup_elem_ptr(em, makeii(4,4)); lruTPut(t, em); // [(4,4),(3,3)]

    get_i(1); // -1
    get_i(2); // -1
    get_i(3); // 3
    get_i(4); // 4

    setup_elem_ptr(em, makeii(4,5)); lruTPut(t, em); // [(4,5),(3,3)]
    get_i(4); // 5

    freeLRUT(t);

    return 0;
}