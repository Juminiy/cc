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

#define ELEM_T_INVALID 0
#define ELEM_T_VALID   1
#define valid_elem_t(elem) (elem.tag!=ELEM_T_INVALID)

#define setup_elem_t(elem, elem_tag, uni_field, elem_val) \
    do { elem.tag = elem_tag; elem.uni.uni_field = elem_val; } while(0)

#define setup_elem_u64(_elem, _u64) \
    setup_elem_t(_elem, ELEM_T_VALID, u64, _u64)
#define setup_elem_i64(_elem, _i64) \
    setup_elem_t(_elem, ELEM_T_VALID, i64, _i64)
#define setup_elem_f64(_elem, _f64) \
    setup_elem_t(_elem, ELEM_T_VALID, f64, _f64)
#define setup_elem_ptr(_elem, _ptr) \
    setup_elem_t(_elem, ELEM_T_VALID, ptr, _ptr)

#define get_elem_u64(_elem) (_elem.uni.u64)
#define get_elem_i64(_elem) (_elem.uni.i64)
#define get_elem_f64(_elem) (_elem.uni.f64)
#define get_elem_ptr(_elem) (_elem.uni.ptr)

// func must return `val`:
// _e0 = _e1: val===0
// _e0 < _e1: any val<0
// _e0 > _e1: any val>0
typedef int(*elem_t_cmp)(elem_t _e0, elem_t _e1);

#define __swap_(_type, _v0, _v1) \
    do { _type _tmp=_v0; _v0=_v1; _v1=_tmp; } while(0)

#endif