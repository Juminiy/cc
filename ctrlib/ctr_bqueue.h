#ifndef CTR_BQUEUE_H
#define CTR_BQUEUE_H

#include "ctr_blist.h"

typedef struct bqueue {
    blist *_bl;
} bqueue;

bqueue makeBQueue();
void freeBQueue(bqueue _bq);
void bQueuePush(bqueue _bq, elem_t _el);
bool bQueueEmpty(bqueue _bq);
elem_t bQueueFront(bqueue _bq);
elem_t bQueueBack(bqueue _bq);
elem_t bQueuePop(bqueue _bq);

#endif