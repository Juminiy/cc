#include <stdlib.h>

int main(){

    void *mem1k = malloc(1<<10);
    free(mem1k);

    return 0;
}