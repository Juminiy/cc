#ifndef CTR_UTIL_H
#define CTR_UTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEBUGF(__content_template__, ...) \
    do { fprintf(stdout, "[DEBUG] "__content_template__"\n" __VA_OPT__(,) __VA_ARGS__); } while(0)

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
	static inline size_t __max_(size_t _a, size_t _b){
		return _a > _b ? _a : _b;
	}
	static inline size_t __min_(size_t _a, size_t _b){
		return _a < _b ? _a : _b;
	}
	static inline bool __in_range_(size_t _l, size_t _x, size_t _r) {
		return _l <= _x && _x <= _r;
	}
#endif

// safe swap
#define __swap_typed(_type, _v0, _v1) \
	do { _type _tmp=_v0; _v0=_v1; _v1=_tmp; } while(0)
#define __swap_(_type, _v0, _v1) \
    __swap_typed(_type, _v0, _v1)

// safe strlen
#define __strlen(__s) (__s ? strlen(__s): 0)

// safe strdup
static inline char* __strdup(const char *__s) {
	if(!__s) return NULL;
	size_t __ssz = __strlen(__s);
	char *__sdup=(char*)malloc(sizeof(char)*(__ssz+1)); // `__s`+'\0'
	strncpy(__sdup, __s, __ssz);
	__sdup[__ssz] = '\0';
	return __sdup;
}

// safe strcmp
static inline int __strcmp(const char *__s1, const char *__s2) {
	if(__s1 && __s2){
		return strcmp(__s1, __s2);
	} else if(__s1){
		return __strlen(__s1);
	} else if(__s2){
		return __strlen(__s2);
	} else {
		return 0;
	}
}

#endif