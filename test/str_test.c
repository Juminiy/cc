#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ctrlib/ctr_util.h"

size_t __i_strlen (const char *__s) {
    size_t _l=0;
    for(char *_p=__s;_p&&(*_p!='\0');_l++,_p++);
    return _l;
}

int main() {

    // // 1. test '\0' truncated str's strlen
    // char chls[10] = {'\0'};
    // printf("chrs: %s, size: %ld", chls, strlen(chls));

    // // 2. test strcmp output, equal or not equal
    // char *s1 = malloc(25), *s2=malloc(25);
    // for(int cn=scanf("%s%s", s1, s2);
    //     cn > 0;
    //     cn=scanf("%s%s", s1, s2)){
    //     fprintf(stdout, "%s %d %s\n", s1, strcmp(s1, s2), s2);
    // }

    // 3. test ssprintf
    // char strcts[20];
    // sprintf(strcts, "%d %s", 1, "hachimi");
    // printf("%s\n", strcts);

    // 4. test cpy
    char *s0=__strdup(NULL),*s1=__strdup(""),*s2=__strdup("hachimi");
    printf("s0=%s,size=%zu\n", s0?s0:"NULL", __strlen(s0));
    printf("s1=%s,size=%zu\n", s1?s1:"NULL", __strlen(s1));
    printf("s2=%s,size=%zu\n", s2?s2:"NULL", __strlen(s2));
    printf("glibc:%zu, i:%zu\n", __strlen(s0), __i_strlen(s0));
    printf("glibc:%zu, i:%zu\n", __strlen(s1), __i_strlen(s1));
    printf("glibc:%zu, i:%zu\n", __strlen(s2), __i_strlen(s2));
    free(s0),free(s1),free(s2);

    return 0;
}