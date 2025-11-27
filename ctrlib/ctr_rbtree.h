#ifndef CTR_RBTREE_H
#define CTR_RBTREE_H

#include "ctr_elemt.h"

#define RB_NODE_BLK 1
#define RB_NODE_RED 0

typedef struct rb_node {
    struct rb_node *_left, *_right, *_parent;
    int _color;
    elem_t _data;
} rb_node;

typedef struct rb_tree {
    rb_node *_root;
    size_t   _size;
    elem_t_cmp _elem_cmp; // +required
    void (*_elem_set)(elem_t _old, elem_t _new);
} rb_tree;

#define rbTreeSetElemSet(_tr, _f) ((_tr)->_elem_set = (_f))

rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data);
void freeRBNode(rb_node* _nd);

rb_tree* makeRBTree(elem_t_cmp _elem_cmp);
void freeRBTree(rb_tree* _tr);
rb_tree* rbTreeInsertData(rb_tree* _tr, elem_t _dt);
rb_tree* rbTreeInsertNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeDeleteData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeDeleteNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeGetData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeGetNode(rb_tree* _tr, rb_node *_nd);

// todo: iterater and rb_tree

#endif