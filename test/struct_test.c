#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct typa{
    int a,b;
    float f1,f2;
    char *x,*y;
} typa;

int main(){

    typa va;
    printf("%lu\n", ((typa*)NULL-&(((typa*)NULL)->a)));
    printf("%lu\n", offsetof(typa, a));

    return 0;
}