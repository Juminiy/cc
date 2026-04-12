#include <stdio.h>

int main(){

    int dst,src;
    scanf("%d %d",&dst,&src);

    __asm__ (//"mov %1, %0\n\t" // mov src, dst 
        "add %0, %1"          // add src, dst
        : "=r" (dst) // %0
        : "r" (src)  // %1
    );

    printf("dst= %d, src= %d\n",dst, src);

    return 0;
}