#include <stdio.h>

#include "../../libctr/ctr_pair.h"

int main() {

    pair pr = varPair(1,true);
    printf("pair: <%d,%s>\n", pairGet0(pr,int), pairGet1(pr,bool)?"True":"False");

    elem_t em = varElemI64(18);
    printf("elem: %d\n", cast_elem_typ(em, int));

    return 0;
}