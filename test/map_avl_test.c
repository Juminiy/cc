// #include <stdio.h>

// #include "../ctrlib/ctr_map.h"

// bool print_sspair(char *_key, char *_val){
//     printf("<%s:%s>\n", _key, _val);
//     return true;
// }

// void inputTest(){
//     strstrmap smap = makestrstrmap();

//     char *rr = NULL;

//     rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // get-notFound

//     strstrmapDel(smap, "hachi"); rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // del-notFound

//     strstrmapPut(smap, "cpp", "well-down"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // put

//     strstrmapDel(smap, "cpp"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // del

//     // now empty
//     printf("input N and input N pairs string key-value\n");
//     int n =0; scanf("%d",&n);
//     for(int idx=0;idx<n;idx++){
//         char skey[100],sval[100];
//         scanf("%s%s", skey, sval);
//         strstrmapPut(smap, skey, sval);
//     }

//     strstrmapIter(smap, print_sspair);

//     freestrstrmap(smap);
// }

// void case_test() {
//     #define put_print(__key,__val) \
//         do { \
//             strstrmapPut(smap, __key, __val); \
//             char *__var = strstrmapGet(smap, __key); \
//             printf("PUT<%s:%s>, GET:<%s:%s>\n", __key, __val?__val:"@NULL", __key, __var?__var:"@NULL"); \
//         } while(0)

//     #define get_print(__key) \
//         do { \
//             char *__var = strstrmapGet(smap, __key); \
//             printf("GET:<%s:%s>\n", __key, __var?__var:"@NULL"); \
//         } while(0)

//     #define del_print(__key) \
//         do { \
//             strstrmapDel(smap, __key); \
//             char *__var = strstrmapGet(smap, __key); \
//             printf("DEL<%s>, GET:<%s:%s>\n", __key, __key, __var?__var:"@NULL"); \
//         } while(0)

//     strstrmap smap = makestrstrmap();

//     put_print("k1", "v1"); // insert
//     put_print("k2", "v2"); // insert
//     put_print("k3", "v3"); // insert

//     put_print("k1", "v4"); // replace
//     del_print("k2");       // delete

//     put_print("k4", "v4"); // insert
//     put_print("k5", "v5"); // insert

//     del_print("k8");       // delete-notfound
//     put_print("k8", "v8"); // get-notfound

//     del_print("k8");       // delete
//     put_print("k8", "v8"); // get-notfound
    
//     get_print("k2");

//     strstrmapIter(smap, print_sspair);

//     freestrstrmap(smap);
// }

// int main() {


//     return 0;
// }


#include "../ctrlib/ctr_map.h"
#include "strstrpair.h"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    int opt_cnt = 0;
    if(argc>=2){
        opt_cnt = strtol(argv[1], NULL, 10);
    }

    strstrmap _avlmap = makestrstrmap();

    kv *kvs = makekvs(opt_cnt);
    for(int i=0;i<opt_cnt;i++){  
        strstrmapPut(_avlmap, kvs[i].k, kvs[i].v);
    }
    
    int getops=0,delops=0,putops=0;
    for(int i=0;i<opt_cnt;i++){
        int opt_code = rand() % 10;
        // printf("opt_code=%d\n", opt_code);
        if(opt_code>=0&&opt_code<5){ //put
            putops++;
            int idx = rand()%opt_cnt;
            strstrmapPut(_avlmap, kvs[idx].k, kvs[idx].v);
        } else if (opt_code>=5&&opt_code<=7){ //del
            delops++;
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
                strstrmapDel(_avlmap, kvs[idx].k);
            } else {
                char *_tmps=rand_str(rand()%15);
                strstrmapDel(_avlmap, _tmps);
                free(_tmps);
            }
        } else {
            getops++;
            if(rand()%2==0){
                int idx = rand()%opt_cnt;
                strstrmapGet(_avlmap, kvs[idx].k);
            } else {
                char *_tmps=rand_str(rand()%15);
                strstrmapGet(_avlmap, _tmps);
                free(_tmps);
            }
        }

    }

    freekvs(kvs, opt_cnt);
    freestrstrmap(_avlmap);

    printf(
        "tot_siz=%d, ops_cnt=%d, get_ops=%d, del_ops=%d, put_ops=%d\n", 
        opt_cnt, opt_cnt, getops, delops, putops 
    );

    return 0;
}