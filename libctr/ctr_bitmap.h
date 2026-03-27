#ifndef CTR_BITMAP
#define CTR_BITMAP

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct bitmap{
    uint64_t _span;
    int64_t _llim, _ulim;
    size_t _size;
    uint64_t *_bits;
} bitmap;

bitmap* makeBitMap(uint64_t _span);
void freeBitMap(bitmap *_bm);
int bitMapInsert(bitmap *_bm, int64_t _v);
int bitMapDelete(bitmap *_bm, int64_t _v);
bool bitMapGet(bitmap *_bm, int64_t _v);
typedef bool(*bitMapIterFunc)(int64_t _v);
void bitMapIter(bitmap *_bm, bitMapIterFunc _fn);

#endif