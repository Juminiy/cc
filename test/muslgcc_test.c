#include <stdio.h>
#include <stdlib.h>
int main(){

    // printf("Hello,Debug\n");

    // // %[$][flags][width][.precision][length modifier]conversion
    // printf("%c, %d, %0*d\n", 'a', 114514, 6, 111);

    void *ptr = malloc(18);
    free(ptr);

    return 0;
}