#include "ctr_bloom.h"
#include <stdarg.h>

bloom makeBloom(size_t _k, ...) {
    bloom bl;
    bl._bitve = makeBitMap((uint64_t)(1)<<32);
    bl._hashk = _k;
    bl._hashf = (elem_t_hash*)malloc(sizeof(elem_t_hash) * _k);
    va_list _funcs;
    va_start(_funcs, _k);
    for(size_t i=0;i<_k;i++)
        bl._hashf[i] = va_arg(_funcs, elem_t_hash);
    va_end(_funcs);
    return bl;
}

void freeBloom(bloom _bl) {
    freeBitMap(_bl._bitve);
    free(_bl._hashf);
}

void bloomInsert(bloom _bl, elem_t _dt) {
    for(size_t i=0;i<_bl._hashk;i++){
        int64_t hval = _bl._hashf[i](_dt);
        bitMapInsert(_bl._bitve, hval);
    }
}

bool bloomGet(bloom _bl, elem_t _dt) {
    for(size_t i=0;i<_bl._hashk;i++){
        int64_t hval = _bl._hashf[i](_dt);
        if(!bitMapGet(_bl._bitve, hval))
            return false;
    }
    return true;
}