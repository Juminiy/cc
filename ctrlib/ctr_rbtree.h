#ifndef CTR_RBTREE_H
#define CTR_RBTREE_H

#include "ctr_elemt.h"
#include "ctr_blist.h"

#define TREE_NODE_TYPE_BS  0
#define TREE_NODE_TYPE_RB  1
#define TREE_NODE_TYPE_AVL 2

#define RB_NODE_CLR_BLK 0
#define RB_NODE_CLR_RED 1

#define RB_NODE_DIR_NONE -1
#define RB_NODE_DIR_LEFT 0
#define RB_NODE_DIR_RIGHT 1

typedef struct rb_node {
    struct rb_node *_left, *_right, *_parent; // 24B
    int _color;								  // 4B
	size_t _size;							  // 4B
    elem_t _data;							  // 16B
	size_t _height;							  // 4B
	size_t __attribute__((unused)) __align;	  // 4B
} rb_node;

typedef struct rb_tree {
    rb_node *_root;
    elem_t_cmp _elem_cmp; 						 // +required, compare _data
    // void (*_elem_set)(elem_t _old, elem_t _new); // +optional, set _old to _new

	size_t   _size;
	int 	 _tree_node_type;					// +optional, default=TREE_NODE_TYPE_BS
} rb_tree;

#define setTreeNodeType(_rb, _type) (_rb->_tree_node_type=_type)
// #define rbTreeSetElemSet(_tr, _f) ((_tr)->_elem_set = (_f))

// please use the MACRO carefully, because the `link` and `unlink` operation is bidirectional

#define __link_left(nd, left) \
	do { if(nd) {nd->_left=left;} if(left) {left->_parent=nd;} } while(0)

#define __link_right(nd, right) \
	do { if(nd) {nd->_right=right;} if(right) {right->_parent=nd;} } while(0)

// __unlink is the BUGGY maker, do not use it!
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
#define __is_red(nd) (nd&&nd->_color==RB_NODE_CLR_RED)
#define __is_black(nd) (!nd||(nd&&nd->_color==RB_NODE_CLR_BLK))
#define __node_size(nd) (nd?nd->_size:0)
#define __node_height(nd) (nd?nd->_height:0)
#define __node_bf(nd) (__node_height(nd->_right)-__node_height(nd->_left))
#define __tree_size(tr) (tr&&tr->_root?tr->_root->_size:0)
#define __tree_height(tr) (tr?__node_height(tr->_root):0)
#define __update_size_height(_nd) \
	do { \
		_nd->_size=__node_size(_nd->_left)+__node_size(_nd->_right)+1; \
		_nd->_height=__max_(__node_height(_nd->_left),__node_height(_nd->_right))+1; \
	} while(0)

static inline size_t __max_(size_t _a, size_t _b){
	return _a > _b ? _a : _b;
}

static inline size_t __min_(size_t _a, size_t _b){
	return _a < _b ? _a : _b;
}


// tree: bst,rbtree,avltree
rb_tree* makeRBTree(elem_t_cmp _elem_cmp);
void freeRBTree(rb_tree* _tr);

// rbtree: red black tree 
rb_node* rbTreeInsertData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeInsertNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeDeleteData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeDeleteNode(rb_tree* _tr, rb_node *_nd);
rb_node* rbTreeGetData(rb_tree* _tr, elem_t _dt);
rb_node* rbTreeGetNode(rb_tree* _tr, rb_node *_nd);

// treeNode: (bst,rbtree,avltree)'s node
rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data);
void freeRBNode(rb_node* _nd);

// bsnode: binary search tree node
rb_node* bsNodeMinNode(rb_node* _rt, rb_tree *_tr);
rb_node* bsNodeMaxNode(rb_node* _rt, rb_tree *_tr);
rb_node* bsNodePreNode(rb_node* _rt, rb_tree *_tr);
rb_node* bsNodeNextNode(rb_node* _rt, rb_tree *_tr);

typedef rb_node*(*rbNodeOpt)(rb_node*, rb_node*, rb_tree*);
rb_node* bsNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr);
rb_node* bsNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* bsNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);

// tree: traverse
typedef blist*(*rbTreeTrav)(rb_tree*);
blist* rbTreeMidTrav(rb_tree* _tr);
blist* rbTreeLelTrav(rb_tree* _tr);

// avltree: balance tree node
rb_node* rotate_right(rb_node* _nd);
rb_node* rotate_left(rb_node *_nd);
rb_node* rotate_ll(rb_node *_nd);
rb_node* rotate_rl(rb_node *_nd);
rb_node* rotate_rr(rb_node *_nd);
rb_node* rotate_lr(rb_node *_nd);

// avltree: AVL tree node
rb_node* avlNodeInsertNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* avlNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* avlNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);

// rbtree: red black tree node
rb_node* rbNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr);
rb_node* rbNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);
rb_node* rbNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr);

#endif