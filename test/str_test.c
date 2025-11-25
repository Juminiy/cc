#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char strcts[20];
    sprintf(strcts, "%d %s", 1, "hachimi");
    printf("%s\n", strcts);

    return 0;
}