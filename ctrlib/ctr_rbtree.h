#ifndef CTR_RBTREE_H
#define CTR_RBTREE_H

#include "ctr_elemt.h"
#include "ctr_blist.h"

#define RB_NODE_BLK 0
#define RB_NODE_RED 1

#define RB_NODE_DIR_NONE -1
#define RB_NODE_DIR_LEFT 0
#define RB_NODE_DIR_RIGHT 1

typedef struct rb_node {
    struct rb_node *_left, *_right, *_parent;
    int _color;
    elem_t _data;
} rb_node;

typedef struct rb_tree {
    rb_node *_root;
    size_t   _size;
    elem_t_cmp _elem_cmp; 						 // +required, compare _data
    // void (*_elem_set)(elem_t _old, elem_t _new); // +optional, set _old to _new 
} rb_tree;

// #define rbTreeSetElemSet(_tr, _f) ((_tr)->_elem_set = (_f))

// please use the MACRO carefully, because the `link` and `unlink` operation is bidirectional

#define __link_left(nd, left) \
	do { if(nd) {nd->_left=left;} if(left) {left->_parent=nd;} } while(0)

#define __link_right(nd, right) \
	do { if(nd) {nd->_right=right;} if(right) {right->_parent=nd;} } while(0)

#define __unlink_left(nd) \
	do { if(nd && nd->_left) {nd->_left->_parent=NULL;nd->_left=NULL;} } while(0) 

#define __unlink_right(nd) \
	do { if(nd && nd->_right) {nd->_right->_parent=NULL;nd->_right=NULL;} } while(0) 

#define __unlink_parent(nd) \
	do { \
		if(nd && nd->_parent) { \
			if(nd->_parent->_left==nd) {nd->_parent->_left=NULL; nd->_parent=NULL;} \
			else if(nd->_parent->_right==nd) {nd->_parent->_right=NULL; nd->_parent=NULL;} \
		} \
	} while(0)

#define __is_leaf(nd) (nd->_left==NULL&&nd->_right==NULL)
#define __is_root(nd) (nd->_parent==NULL)
#define __is_left(nd) (nd->_parent&&nd->_parent->_left==nd)
#define __is_right(nd) (nd->_parent&&nd->_parent->_right==nd) 
#define __node_dir(nd) (__is_left(nd)?RB_NODE_DIR_LEFT:(__is_right(nd)?RB_NODE_DIR_RIGHT:RB_NODE_DIR_NONE))

rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data);
void freeRBNode(rb_node* _nd);

rb_tree* makeRBTree(elem_t_cmp _elem_cmp);
void freeRBTree(rb_tree* _tr);

// bsnode: binary tree node
rb_node* rbNodeMinNode(rb_node* _rt, rb_tree *_tr);
rb_node* rbNodeMaxNode(rb_node* _rt, rb_tree *_tr);
rb_node* bsNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr);
rb_node* bsNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* bsNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);

// bstree: binary search tree
rb_node* rbTreeInsertData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeInsertNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeDeleteData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeDeleteNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeGetData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeGetNode(rb_tree* _tr, rb_node *_nd);

// traverse
typedef blist*(*rbTreeTrav)(rb_tree*);
blist* rbTreeMidTrav(rb_tree* _tr);
blist* rbTreeLelTrav(rb_tree* _tr);

// avltree: balance tree
rb_node* rotate_right(rb_node* _nd);
rb_node* rotate_left(rb_node *_nd);

// rbtree: red black tree
rb_node* rbNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr);
rb_node* rbNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* rbNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);


#endif