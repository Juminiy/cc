// will not use it anymore
#define T smap_int, int, int
#include <stc/sortedmap.h>

int main() {
    
    smap_int m={0};
    for(int i=0;i<10;i++){
        smap_int_insert(&smap, i, 10-i);
    }

    for(c_each(itr, smap_int, m)){
        printf("%d, %d\n", itr.ref->first, itr.ref->second);
    }

    return 0;
}