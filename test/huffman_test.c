#include "../ctrlib/ctr_huffman.h"

#include <stdio.h>

bool printkv(char *_key, char *_val) {
    printf("<%s:%s> ", _key, _val);
    return true;
}

int main() {

    huffman_res res = huffmanEncode("ABAACDCFF");

    strstrmapIter(res.code, printkv);
    puts("");

    printf("wpl=%d\n", res.wpl);

    puts(res.dest);
    char *dec=huffmanDecode(res);
    puts(dec); free(dec);

    freeHuffmanRes(res);
    return 0;
}