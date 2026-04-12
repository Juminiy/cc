#include "../../libctr/ctr_lru.h"
#include "../../libctr/ctr_elemt.h"

typedef struct {
    lru_t *c;
} LRUCache;

typedef struct ii{
    int k,v;
} ii;

ii* makeii(int k,int v){
    ii *iikv=(ii*)malloc(sizeof(ii));
    iikv->k=k,iikv->v=v;
    return iikv;
}

int cmpii(elem_t e0, elem_t e1) {
    ii *i0=(ii*)get_elem_ptr(e0);
    ii *i1=(ii*)get_elem_ptr(e1);
    return i0->k-i1->k;
}

void freeii(elem_t e){
    ii *iikv=(ii*)get_elem_ptr(e);
    free(iikv);
}

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *_c = (LRUCache*)malloc(sizeof(LRUCache));
    _c->c = makeLRUT(cmpii, capacity);
    lruTSetElemFree(_c->c, freeii);
    return _c;
}

int lRUCacheGet(LRUCache* obj, int key) {
    ii __iikv; __iikv.k=key,__iikv.v=0;
    elem_t em; setup_elem_ptr(em, &__iikv);
    em = lruTGet(obj->c, em);
    if(valid_elem_t(em)){
        return cast_elem_typ(em,ii*)->v;
    } else {
        return -1;
    }
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    elem_t em; setup_elem_ptr(em, makeii(key,value)); 
    lruTPut(obj->c, em);
}

void lRUCacheFree(LRUCache* obj) {
    freeLRUT(obj->c);
    free(obj);
}   

/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);
 
 * lRUCachePut(obj, key, value);
 
 * lRUCacheFree(obj);
*/

int main(){

    LRUCache *cx = lRUCacheCreate(2);
    lRUCachePut(cx,1, 1); // 缓存是 {1=1}
    lRUCachePut(cx,2, 2); // 缓存是 {1=1, 2=2}
    printf("%d\n",lRUCacheGet(cx,1));    // 返回 1
    lRUCachePut(cx,3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    printf("%d\n",lRUCacheGet(cx,2));    // 返回 -1 (未找到)
    lRUCachePut(cx,4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    printf("%d\n",lRUCacheGet(cx,1));    // 返回 -1 (未找到)
    printf("%d\n",lRUCacheGet(cx,3));    // 返回 3
    printf("%d\n",lRUCacheGet(cx,4));    // 返回 4

    return 0;
}