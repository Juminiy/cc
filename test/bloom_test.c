#include <stdio.h>
#include <stdlib.h>

#include "../ctrlib/ctr_bloom.h"
#include "strstrpair.h"

int main(int argc, char **argv) {
    int tot=0;
    if(argc>1)
        tot=strtol(argv[1], NULL, 10);

    bloom bl = makeBloom(1, __elem_hash_strstrpair);
    kv *kvs = makekvs(tot);
    for(int i=0;i<tot;i++){
        elem_t em; setup_elem_ptr(em, kvs+i);
        bloomInsert(bl, em);
    }

    for(int i=0;i<tot;i++) {
        elem_t em; setup_elem_ptr(em, kvs+i);
        bool ok = bloomGet(bl, em);
        // printf("%s: %s\n", kvs[i].k, ok?"True":"False");
        if(!ok)
            exit(1);
    }

    free(kvs);
    freeBloom(bl);

    
    return 0;
}