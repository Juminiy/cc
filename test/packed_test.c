#include <stdio.h>

/*
 * |----|
 * |--------|
 * |-|-|--|
 */
struct nopacked {
    int i32;        // 4B
    double f64;     // 8B
    char i8_1,i8_2; // 2B 
    short i16_1;    // 2B
};

// BigEndian
/*
 * |----|--------|-|-|--|
 */
struct __attribute__((packed)) packed {
    int i32;
    double f64;
    char i8_1,i8_2;
    short i16_1;
};

struct __attribute__((aligned(1))) aligned1 {
    int i32;
    double f64;
    char i8_1,i8_2;
    short i16_1;
};

struct __attribute__((aligned(2))) aligned2 {
    int i32;
    double f64;
    char i8_1,i8_2;
    short i16_1;
};

struct __attribute__((aligned(4))) aligned4 {
    int i32;
    double f64;
    char i8_1,i8_2;
    short i16_1;
};

struct __attribute__((aligned(8))) aligned8 {
    int i32;
    double f64;
    char i8_1,i8_2;
    short i16_1;
};

int main() {

    printf("nopacked: %ld\n", sizeof(struct nopacked));
    printf("packed: %ld\n", sizeof(struct packed));
    printf("aligned1: %ld\n", sizeof(struct aligned1));
    printf("aligned2: %ld\n", sizeof(struct aligned2));
    printf("aligned4: %ld\n", sizeof(struct aligned4));
    printf("aligned8: %ld\n", sizeof(struct aligned8));

    puts("packed");
    struct packed pval = {.f64=0.33,.i16_1=222,.i8_1=3,.i8_2=5,.i32=44};
    printf("addr(i32) = %p\n",   (&pval.i32));
    printf("addr(f64) = %p\n",   (&pval.f64));
    printf("addr(i8_1) = %p\n",  (&pval.i8_1));
    printf("addr(i8_2) = %p\n",  (&pval.i8_2));
    printf("addr(i16_1) = %p\n", (&pval.i16_1));

    return 0;
}