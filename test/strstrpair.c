#include "strstrpair.h"

#define freekv(kvp) do { free(kvp.k), free(kvp.v); } while(0)

char* rand_str(size_t _sz) {
    char *_ss = (char*)malloc(sizeof(char)*(_sz+1));
    for(size_t _i=0;_i<_sz;_i++)
        _ss[_i] = '0' + rand()%72;
    _ss[_sz]='\0';
    return _ss;
}

kv* makekvs(size_t _sz) {
    kv *kvs = (kv*)malloc(sizeof(kv)*_sz);
    for(size_t i=0;i<_sz;i++){
        kvs[i].k = rand_str(rand()%15);
        kvs[i].v = rand_str(rand()%15);
    }
    return kvs;
}

void freekvs(kv *kvs, size_t _sz) {
    for(size_t i=0;i<_sz;i++){
        freekv(kvs[i]);
    }
    free(kvs);
}