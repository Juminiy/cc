#ifndef CTR_BLOOM_H
#define CTR_BLOOM_H

#include "ctr_elemt.h"
#include "ctr_bitmap.h"

typedef struct bloom {
    bitmap      *_bitve;
    size_t       _hashk;
    elem_t_hash *_hashf;
} bloom;

bloom makeBloom(size_t _k, ...);
void freeBloom(bloom _bl);
void bloomInsert(bloom _bl, elem_t _dt);
bool bloomGet(bloom _bl, elem_t _dt);

#endif // CTR_BLOOM_H