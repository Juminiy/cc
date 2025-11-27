#ifndef CTR_RBTREE_H
#define CTR_RBTREE_H

#include "ctr_elemt.h"

typedef struct rb_node {
    struct rb_node *_left, *_right, *_parent;
    int _color;
    elem_t _data;
} rb_node;

typedef struct rb_tree {
    rb_node *_root;
    size_t   _size;
    elem_t_cmp _elem_cmp; // +required
} rb_tree;

rb_node* makeRBNode(rb_node* _left, rb_node* _right, rb_node* _parent, elem_t _data);
void freeRBNode(rb_node* _nd);

rb_tree* makeRBTree(elem_t_cmp _elem_cmp);
void freeRBTree(rb_tree* _tr);

#endif