#ifndef STRSTRPAIR_H
#define STRSTRPAIR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../ctrlib/ctr_elemt.h"
#include <stdlib.h>

typedef struct kv {
	char *k, *v;
} kv;

char* rand_str(size_t _sz);
kv* makekvs(size_t _sz);
void freekvs(kv *kvs, size_t _sz);

static inline int __elem_cmp_strstrpair(elem_t em0, elem_t em1) {
	kv *kvp0 = cast_elem_typ(em0, kv*);
	kv *kvp1 = cast_elem_typ(em1, kv*);
	return __strcmp(kvp0->k, kvp1->k);
}

static inline int __elem_hash_strstrpair(elem_t em) {
	int ppow = 1, p = 31;
	int hval = 0, m = 1e9+7;
	for(char *_key = cast_elem_typ(em, kv*)->k;
		*_key!='\0';
		_key++
	) {
		hval = (hval + ppow * (*_key)) % m;
		ppow = (ppow * p) % m;
	}
	// DEBUGF("%s: %u", cast_elem_typ(em, kv*)->k, hval);
	return hval >= 0 ? hval : -hval;
}

#ifdef __cplusplus
}
#endif


#endif 