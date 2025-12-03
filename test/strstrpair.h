#ifndef STRSTRPAIR_H
#define STRSTRPAIR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct kv {
	char *k, *v;
} kv;

char* rand_str(size_t _sz);
kv* makekvs(size_t _sz);
void freekvs(kv *kvs, size_t _sz);

#ifdef __cplusplus
}
#endif


#endif 