#include <stdio.h>

#include "../ctrlib/ctr_map.h"

bool print_sspair(char *_key, char *_val){
    printf("<%s:%s>\n", _key, _val);
    return true;
}

void inputTest(){
    strstrmap smap = makestrstrmap();

    char *rr = NULL;

    rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // get-notFound

    strstrmapDel(smap, "hachi"); rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // del-notFound

    strstrmapPut(smap, "cpp", "well-down"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // put

    strstrmapDel(smap, "cpp"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // del

    // now empty
    printf("input N and input N pairs string key-value\n");
    int n =0; scanf("%d",&n);
    for(int idx=0;idx<n;idx++){
        char skey[100],sval[100];
        scanf("%s%s", skey, sval);
        strstrmapPut(smap, skey, sval);
    }

    strstrmapIter(smap, print_sspair);

    freestrstrmap(smap);
}

void case_test() {
    #define put_print(__key,__val) \
        do { \
            strstrmapPut(smap, __key, __val); \
            char *__var = strstrmapGet(smap, __key); \
            printf("PUT<%s:%s>, GET:<%s:%s>\n", __key, __val?__val:"@NULL", __key, __var?__var:"@NULL"); \
        } while(0)

    #define get_print(__key) \
        do { \
            char *__var = strstrmapGet(smap, __key); \
            printf("GET:<%s:%s>\n", __key, __var?__var:"@NULL"); \
        } while(0)

    #define del_print(__key) \
        do { \
            strstrmapDel(smap, __key); \
            char *__var = strstrmapGet(smap, __key); \
            printf("DEL<%s>, GET:<%s:%s>\n", __key, __key, __var?__var:"@NULL"); \
        } while(0)

    strstrmap smap = makestrstrmap();

    put_print("k1", "v1"); // insert
    put_print("k2", "v2"); // insert
    put_print("k3", "v3"); // insert

    put_print("k1", "v4"); // replace
    del_print("k2");       // delete

    put_print("k4", "v4"); // insert
    put_print("k5", "v5"); // insert

    del_print("k8");       // delete-notfound
    put_print("k8", "v8"); // get-notfound

    del_print("k8");       // delete
    put_print("k8", "v8"); // get-notfound
    
    get_print("k2");

    strstrmapIter(smap, print_sspair);

    freestrstrmap(smap);
}

int main() {

    
    

    return 0;
}