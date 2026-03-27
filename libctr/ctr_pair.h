#ifndef CTR_PAIR_H
#define CTR_PAIR_H

#include "ctr_elemt.h"

typedef struct pair {
    elem_t _first,_second;
} pair;

#define varPair(_g0, _g1) \
({ pair pr; \
    setup_elem_u64(pr._first, _g0); \
    setup_elem_u64(pr._second, _g1); \
    pr; })
#define pairGet0(pr, typ) (cast_elem_typ(pr._first, typ))
#define pairGet1(pr, typ) (cast_elem_typ(pr._second, typ))

#endif