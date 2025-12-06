#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

void alloc_test(int numn) {
    int *malloc_arr = (int*)malloc(numn * sizeof(int));
    int *calloc_arr = (int*)calloc(numn, sizeof(int));

    printf("malloc_arr:%p, calloc_arr:%p\n", malloc_arr, calloc_arr);
    printf("malloc_arr[0]:%d, calloc_arr[0]:%d\n", malloc_arr[0], calloc_arr[0]);
    for(size_t idx=0;idx<numn;idx++){
        printf("malloc_arr[%ld]=%d, calloc_arr[%ld]=%d\n", 
            idx, malloc_arr[idx], idx, calloc_arr[idx]);
        malloc_arr[idx] = rand(), calloc_arr[idx] = rand();
    }

    free(malloc_arr);
    free(calloc_arr);
}

void test_mv() {
    char buf[50]="abcde mnopq rsyoa xyzrt\0";
    memcpy(buf+6, buf, 15);
    printf("%s\n",buf);

    char buf1[50]="abcde mnopq rsyoa xyzrt\0";
    memmove(buf1+6, buf1, 15);
    printf("%s\n", buf1);
}

void test_mset() {
    char buf[32]="\0";
    memset(buf, 'a', 32);
    printf("%s, %c\n",buf, buf[0]);

    int arr[10]={0};
    memset(arr, 'a', 10*4);
    for(int i=0;i<10;i++) printf("%0x ", arr[i]);
    printf("\n");
}

void int_overflow(int numa, int numb) {
    // signed int overflow: numa+numb<numa && numa+numb<numb
    printf("signed int: %d+%d=%d\n",numa,numb,numa+numb);
    // unsigned int overflow
    size_t snuma=numa, snumb=numb;
    printf("unsigned long: %zu+%zu=%zu\n",snuma,snumb,snuma+snumb);
}

void test_relloac(size_t oldsz, size_t newsz) {
    int *arr = (int*)calloc(oldsz, sizeof(int)); // alloc1
    for(int i=0;i<oldsz;i++)
        arr[i]=i+1;
    
    if(newsz > oldsz){
        arr = realloc(arr, sizeof(int)*newsz); // free1 alloc2 free2 alloc3
        memset(arr+oldsz, 0, sizeof(int)*(newsz-oldsz));
        for(int i=0;i<newsz;i++)
            printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr); // free3
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int numa=1,numb=2;
    if(argc>=2){
        numa = strtol(argv[1], NULL, 10);
    }
    if(argc>=3){
        numb = strtol(argv[2], NULL, 10);
    }
    // alloc_test(numa);
    
    // test_mv();

    // test_mset();

    test_relloac(numa, numb);

    return 0;
}