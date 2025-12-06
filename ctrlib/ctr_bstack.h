#ifndef CTR_BSTACK_H
#define CTR_BSTACK_H

#include "ctr_blist.h"

typedef struct bstack {
    blist *_bl;
} bstack;

bstack makeBStack();
void freeBStack(bstack _bstk);
void bStackPush(bstack _bstk, elem_t _el);
bool bStackEmpty(bstack _bstk);
elem_t bStackTop(bstack _bstk);
elem_t bStackPop(bstack _bstk);

#endif