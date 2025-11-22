#include <stdio.h>

#include <sys/queue.h>
#include <malloc.h>

typedef struct ll_i32 {
    int val;
    LIST_ENTRY(ll_i32) lli;
} ll_i32;

LIST_HEAD(ll_i32_hd, ll_i32);

int main() {

    struct ll_i32_hd hd;
    LIST_INIT(&hd);

    struct ll_i32 *l1 = malloc(sizeof(struct ll_i32)),
        *l2 = malloc(sizeof(struct ll_i32)),
        *l3 = malloc(sizeof(struct ll_i32)); 

    LIST_INSERT_HEAD(&hd, l1, lli); // l1->hd
    LIST_INSERT_BEFORE(l1, l2, lli); // l2->l1
    LIST_INSERT_AFTER(l2, l3, lli); // l2->l3->l1->hd

    struct ll_i32 *ln;
    int i=0;
    LIST_FOREACH(ln, &hd, lli)
        ln->val = ++i;
    
    LIST_REMOVE(l3, lli);
    LIST_FOREACH(ln, &hd, lli)
        printf("%d ", ln->val);
    printf("\n");

    LIST_REMOVE(l2, lli);
    LIST_FOREACH(ln, &hd, lli)
        printf("%d ", ln->val);
    printf("\n");

    return 0;
}