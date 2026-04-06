#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

void int_anylength() {
    int8_t i8 = INT8_MAX;
    int16_t i16 = INT16_MAX;
    int32_t i32 = INT32_MAX;
    int64_t i64 = INT64_MAX;
    uint8_t u8 = UINT8_MAX;
    uint16_t u16 = UINT16_MAX;
    uint32_t u32 = UINT32_MAX;
    uint64_t u64 = UINT64_MAX;
    uintptr_t uptr = UINTPTR_MAX;
    intptr_t iptr = INTPTR_MAX;

    printf("%"PRIx8", %"PRIX8", %"PRId8"\n", i8, i8, i8);
    printf("%"PRId16"\n", i16);
    printf("%"PRId32"\n", i32);
    printf("%"PRId64"\n", i64);
    printf("%"PRIu8"\n", u8);
    printf("%"PRIu16"\n", u16);
    printf("%"PRIu32"\n", u32);
    printf("%"PRIu64"\n", u64);
    printf("%"PRIdPTR"\n", iptr);
    printf("%"PRIuPTR"\n", uptr);
    printf("%"PRIxPTR"\n", uptr);
    printf("%"PRIXPTR"\n", uptr);
}


int main() {




    return 0;
}