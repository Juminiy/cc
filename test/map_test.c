#include <stdio.h>

#include "../ctrlib/ctr_map.h"

bool print_sspair(char *_key, char *_val){
    printf("<%s:%s>\n", _key, _val);
    return true;
}

int main() {

    strstrmap smap = makestrstrmap();

    char *rr = NULL;

    rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // get-notFound

    strstrmapDel(smap, "hachi"); rr = strstrmapGet(smap, "hachi"); printf("%s\n", rr?rr:"null"); // del-notFound

    strstrmapPut(smap, "cpp", "well-down"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // put

    strstrmapDel(smap, "cpp"); rr = strstrmapGet(smap, "cpp"); printf("%s\n", rr?rr:"null"); // del

    // now empty

    int n =0; scanf("%d",&n);
    for(int idx=0;idx<n;idx++){
        char skey[100],sval[100];
        scanf("%s%s", skey, sval);
        strstrmapPut(smap, skey, sval);
    }

    strstrmapIter(smap, print_sspair);

    freestrstrmap(smap);

    return 0;
}