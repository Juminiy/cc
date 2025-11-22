#include <stdio.h>
#include "../tcplib/skt_fdset.h"

void usage_test() {
    optfdset *s = create_optfdset(6);

    printf("--------------------------------\n");
    optfdset_add(s, 1);
    optfdset_add(s, 2);
    optfdset_add(s, 3);

    for(int idx=0;idx<s->cap;idx++){
        if (optfdset_valid(s->arr[idx])) {
            printf("%d ", s->arr[idx]);
        }
    }
    printf(" siz=%d, min=%d, max=%d\n", s->siz, s->min, s->max);

    printf("--------------------------------\n");
    optfdset_add(s, 1);
    optfdset_add(s, 2);
    optfdset_add(s, 3);
    optfdset_del(s, -1);
    optfdset_del(s, -5);

    for(int idx=0;idx<s->cap;idx++){
        if (optfdset_valid(s->arr[idx])) {
            printf("%d ", s->arr[idx]);
        }
    }
    printf(" siz=%d, min=%d, max=%d\n", s->siz, s->min, s->max);

    printf("--------------------------------\n");
    optfdset_del(s, 1);
    optfdset_del(s, 2);

    for(int idx=0;idx<s->cap;idx++){
        if (optfdset_valid(s->arr[idx])) {
            printf("%d ", s->arr[idx]);
        }
    }
    printf(" siz=%d, min=%d, max=%d\n", s->siz, s->min, s->max);

    printf("--------------------------------\n");
    optfdset_del(s, 1);
    optfdset_del(s, 2);
    optfdset_del(s, 3);
    optfdset_del(s, -1);
    optfdset_del(s, -5);

    for(int idx=0;idx<s->cap;idx++){
        if (optfdset_valid(s->arr[idx])) {
            printf("%d ", s->arr[idx]);
        }
    }
    printf(" siz=%d, min=%d, max=%d\n", s->siz, s->min, s->max);
}

void boundary_test( ) {
    optfdset *s = create_optfdset(5);

    for(int i=1;i<=10;i++){
        int res = optfdset_add(s, i);
        printf("%d\n", res);
    }

    
} 

int main() {

    boundary_test();

    return 0;
}