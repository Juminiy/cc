#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../libctr/ctr_util.h"

void * 
my_memcpy(void *__dest, const void *__src,size_t __n) {
    unsigned char *_d=__dest;
    unsigned char *_s=__src;

    size_t _i=0;
    for(_i=0;_i<__n;_i+=4) {
        for(size_t _j=_i;_j<_i+4;_j++) {
            _d[_j] = _s[_j];
        }
    }
    for(;_i<__n;_i++){
        _d[_i] = _s[_i];
    }
    return __dest;
}

void test_memcpy(char *_s) {
    size_t ssz = __strlen(_s);
    char *d = malloc(sizeof(char)*(ssz+1));
    my_memcpy(d, _s, ssz);
    d[ssz]='\0';
    printf("[%10s] <- [%10s], %3s\n", d, _s, __strcmp(d, _s)==0?"ok":"false");
}

int main() {
    test_memcpy(NULL);
    test_memcpy("");
    test_memcpy("a");
    test_memcpy("ab");
    test_memcpy("abc");
    test_memcpy("abcd");
    test_memcpy("abcde");
    test_memcpy("abcdef");
    test_memcpy("abcdefg");
    test_memcpy("abcdefgh");

    return 0;
}