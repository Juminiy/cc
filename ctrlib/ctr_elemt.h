#ifndef CTR_ELEMT_H
#define CTR_ELEMT_H

#include <stddef.h>
#include <float.h>
#include <stdint.h>
#include <stdbool.h>

typedef union elem_uni {
    // 1B: bool, unsigned char
    // 2B: unsigned short, unsigned short int
    // 4B: unsigned, unsigned int
    // 4B: unsigned long, unsigned long int
    // 8B: unsigned long long, unsigned long long int
    uint64_t u64;
    // 1B: char, signed char, 
    // 2B: short, short int, signed short, signed short int
    // 4B: int, signed, signed int
    // 4B: long, long int, signed long, signed long int
    // 8B: long long, long long int, signed long long, signed long long int
    int64_t  i64; 
    // 4B: float, 8B: double
    double   f64; 
    // 8B: any pointer
    void    *ptr;
} elem_uni;

typedef struct elem_t {
    uint64_t tag; // 8B
    elem_uni uni; // 8B
} elem_t;

#define setup_elem_t(elem, elem_tag, uni_field, elem_uni) \
    do { elem.tag = elem_tag; elem.uni.uni_field = elem_uni; } while(0)

typedef int(*elem_t_cmp)(elem_t, elem_t);

#endif