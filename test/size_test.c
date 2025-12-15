#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <stddef.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#include "../ctrlib/ctr_elemt.h"

void print_size() {
    #define ptsz(typ) printf("%22s %ld\n", #typ, sizeof(typ))
                                                                                        // x86_64 GNU/Linux
    ptsz(bool);                                                                         // 1B
    ptsz(unsigned char);          ptsz(char); ptsz(signed char);                        // 1B
    ptsz(unsigned short);         ptsz(short); ptsz(short int);                         // 2B
    ptsz(unsigned short int);     ptsz(signed short); ptsz(signed short int);           // 2B
    ptsz(unsigned);               ptsz(int); ptsz(signed);                              // 4B
    ptsz(unsigned int);           ptsz(signed int);                                     // 4B
    ptsz(unsigned long);          ptsz(long); ptsz(long int);                           // 8B
    ptsz(unsigned long int);      ptsz(signed long); ptsz(signed long int);             // 8B
    ptsz(unsigned long long);     ptsz(long long); ptsz(long long int);                 // 8B
    ptsz(unsigned long long int); ptsz(signed long long); ptsz(signed long long int);   // 8B
    ptsz(float);                                                                        // 4B
    ptsz(double);                                                                       // 8B
    ptsz(long double);                                                                  // 16B
    ptsz(void*);                                                                        // 8B

    ptsz(size_t); ptsz(ssize_t);                                                        // 8B

    ptsz(int8_t); ptsz(uint8_t);                                                        // 1B
    ptsz(int16_t); ptsz(uint16_t);                                                      // 2B
    ptsz(int32_t); ptsz(uint32_t);                                                      // 4B
    ptsz(int64_t); ptsz(uint64_t);                                                      // 8B

}

void print_fmt(int64_t val) {
    #define ptcast(fmt,typ) printf("%s "fmt"\n", #typ, cast_elem_typ(em, typ))

    elem_t em; setup_elem_i64(em, val);

    // signed: -2^(b-1),2^(b-1)-1; unsigned: 0,2^(b)-1
    ptcast("%d", bool);         
    ptcast("%hhd", char);       ptcast("%hhu", unsigned char); 
    ptcast("%hd", short);       ptcast("%hu", unsigned short);
    ptcast("%d", int);          ptcast("%u", unsigned int);
    ptcast("%ld", long);        ptcast("%lu", long);
    ptcast("%lld", long long);  ptcast("%llu", long long);
    ptcast("%f", float);        
    ptcast("%f", double);       
    ptcast("%p", void*);   
    ptcast("%zu", size_t);
    ptcast("%zd", ssize_t);
}

int main(int argc, char **argv) {
    int64_t val=0;
    if(argc>1)
        val=strtoll(argv[1], NULL, 10);

    // print_size();
    print_fmt(val);

    return 0;
}