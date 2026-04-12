#ifndef CTR_UTIL_H
#define CTR_UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "ctr_string.h"
#include "ctr_file.h"

// logger
#undef DEBUGF
#define DEBUGF(__content_template__, ...) \
    do { fprintf(stdout, "[DEBUG] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#undef INFOF
#define INFOF(__content_template__, ...) \
    do { fprintf(stdout, "[INFO] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#undef ERRORF
#define ERRORF(__content_template__, ...) \
    do { fprintf(stderr, "[ERROR] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

#undef PANICF
#define PANICF(__content_template__, ...) \
    do { \
		fprintf(stderr, "[PANIC] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while(0)

#if defined(__GNUC__)
#define __typeof_(_a) typeof(_a)
#define __max_(a, b) \
	({ __auto_type _a = (a); \
		__auto_type _b = (b); \
		_a > _b ? _a : _b; })
#define __min_(a, b) \
	({ __auto_type _a = (a); \
		__auto_type _b = (b); \
		_a < _b ? _a : _b; })
#define __in_range_(l,x,r) \
	({ __auto_type _l=(l); \
		__auto_type _x=(x); \
		__auto_type _r=(r); \
		_l<=_x && _x<=_r; })
#else
	static inline int64_t __max_(int64_t _a, int64_t _b){
		return _a > _b ? _a : _b;
	}
	static inline int64_t __min_(int64_t _a, int64_t _b){
		return _a < _b ? _a : _b;
	}
	static inline bool __in_range_(int64_t _l, int64_t _x, int64_t _r) {
		return _l <= _x && _x <= _r;
	}
#endif

// safe swap
#define __swap_typed(_type, _v0, _v1) \
	do { _type _tmp=_v0; _v0=_v1; _v1=_tmp; } while(0)
#define __swap_(_type, _v0, _v1) \
    __swap_typed(_type, _v0, _v1)

#define MALLOC_TYPE(_typ,_val) _typ *_val = (_typ*)malloc(sizeof(_typ))

#endif // CTR_UTIL_H