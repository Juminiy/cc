#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char *s1 = malloc(25), *s2=malloc(25);
    for(int cn=scanf("%s%s", s1, s2);
        cn > 0;
        cn=scanf("%s%s", s1, s2)){
        fprintf(stdout, "%s %d %s\n", s1, strcmp(s1, s2), s2);
    }
    

    return 0;
}