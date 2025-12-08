#ifndef CTR_RBTREE_H
#define CTR_RBTREE_H

#include "ctr_elemt.h"
#include "ctr_blist.h"
#include "ctr_util.h"

#define TREE_NODE_TYPE_BS   0
#define TREE_NODE_TYPE_RB   1
#define TREE_NODE_TYPE_AVL  2
#define TREE_NODE_TYPE_MAVL 3

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
	size_t _cnt;	  						  // 4B
} rb_node;

typedef struct rb_tree {
    elem_t_cmp _elem_cmp; 						// 8B, +required, compare _data
	elem_t_free _elem_free;						// 8B, +optional, free _data
	elem_t_merge _elem_merge;					// 8B, +optional, merge _data
	rb_node *_root;								// 8B
	size_t   _size;							    // 4B
	int 	 _node_type;						// 4B, +optional, default=TREE_NODE_TYPE_BS
} rb_tree;

#define setRBTreeNodeType(_rb, _type) (_rb->_node_type=_type)
#define setRBTreeDataFree(_rb, _free) (_rb->_elem_free=_free)
#define setRBTreeDataMerge(_rb, _merge) (_rb->_elem_merge=_merge)

// please use the `*link*, *unlink*` MACRO carefully, because the `link` and `unlink` operation is bidirectional

#define __link_left(nd, left) \
	do { if(nd) {nd->_left=left;} if(left) {left->_parent=nd;} } while(0)

#define __link_right(nd, right) \
	do { if(nd) {nd->_right=right;} if(right) {right->_parent=nd;} } while(0)

#define __link_(_nd, _ch, _dir) \
	do { \
		if(_dir==RB_NODE_DIR_LEFT){ \
			__link_left(_nd, _ch); \
		} else if(_dir==RB_NODE_DIR_RIGHT){ \
			__link_right(_nd, _ch); \
		} else { \
			if(_ch) {_ch->_parent = NULL;} \
		} \
	} while(0)

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
#define __node_cnt(nd) (nd?nd->_cnt:0)
#define __node_bf(nd) (__node_height(nd->_right)-__node_height(nd->_left))
#define __tree_size(tr) (tr&&tr->_root?tr->_root->_size:0)
#define __tree_height(tr) (tr?__node_height(tr->_root):0)
#define __update_size_height(_nd) \
	do { \
		_nd->_size=__node_size(_nd->_left)+__node_size(_nd->_right)+__node_cnt(_nd); \
		_nd->_height=__max_(__node_height(_nd->_left),__node_height(_nd->_right))+1; \
	} while(0)
#define __update_cnt(_nd, _inc) \
	do { _nd->_cnt += _inc; _nd->_size += _inc;} while(0)
#define __free_data(tr, _dt) \
	do { if(tr->_elem_free) {tr->_elem_free(_dt);} } while(0)
#define __merge_data(tr, _dst, _src) \
	do { if(tr->_elem_merge) { _dst = tr->_elem_merge(_dst, _src); } } while(0)
#define __child_(_nd, _dir) ((_dir)==RB_NODE_DIR_LEFT?_nd->_left:_nd->_right)
#define __rotate_(_nd, _dir) \
	do { \
		if((_dir)==RB_NODE_DIR_LEFT) rotate_left(_nd); \
		else if((_dir)==RB_NODE_DIR_RIGHT) rotate_right(_nd); \
	} while(0)

#define RB_NODE_OPT_NONE	 	ELEM_OPT_NONE
#define RB_NODE_INSERT_CREATED  ELEM_INSERT_CREATED
#define RB_NODE_INSERT_REPLACED ELEM_INSERT_REPLACED
#define RB_NODE_DELETE_SUCCESS  ELEM_DELETE_REMOVED
#define RB_NODE_DELETE_NOTFOUND ELEM_DELETE_NOTFOUND
#define RB_NODE_GET_SUCCESS	    ELEM_GET_SUCCESS
#define RB_NODE_GET_NOTFOUND    ELEM_GET_NOTFOUND

// Tree high level API
rb_tree* makeRBTree(elem_t_cmp _elem_cmp);
void freeRBTree(rb_tree* _tr);
int rbTreeInsertData(rb_tree *_tr, elem_t _dt);
int rbTreeDeleteData(rb_tree *_tr, elem_t _dt);
elem_t rbTreeGetData(rb_tree *_tr, elem_t _dt);
elem_t rbTreeMaxData(rb_tree *_tr);
elem_t rbTreeMinData(rb_tree *_tr);
elem_t bsDataPrevData(rb_tree *_tr, elem_t _dt);
elem_t bsDataNextData(rb_tree *_tr, elem_t _dt);
size_t bsDataRank(rb_tree *_tr, elem_t _dt);
elem_t bsDataRankKData(rb_tree *_tr, size_t _rank);

// Tree lower level API
void rbTreeInsertNode(rb_tree *_tr, _node_value *_val);
void rbTreeDeleteNode(rb_tree *_tr, _node_value *_val);
rb_node* rbTreeGetNode(rb_tree *_tr, _node_value *_val);

typedef blist*(*rbTreeTrav)(rb_tree*);
blist* rbTreeMidTravData(rb_tree* _tr);
blist* rbTreeLelTravData(rb_tree* _tr);
blist* rbTreeMidTrav(rb_tree *_tr);
blist* rbTreeLelTrav(rb_tree *_tr);

// TreeNode API
rb_node* makeRBNode(rb_node *_left, rb_node *_right, elem_t _data);
void freeRBNode(rb_node *_nd, rb_tree *_tr);
void freeRBNodeRec(rb_node *_nd, rb_tree *_tr);
rb_node* bsNodeMinNode(rb_node *_rt);
rb_node* bsNodeMaxNode(rb_node *_rt);
rb_node* bsNodePreNode(rb_node *_rt);
rb_node* bsNodeNextNode(rb_node *_rt);

rb_node* rotate_right(rb_node *_nd);
rb_node* rotate_left(rb_node *_nd);
rb_node* rotate_ll(rb_node *_nd);
rb_node* rotate_rl(rb_node *_nd);
rb_node* rotate_rr(rb_node *_nd);
rb_node* rotate_lr(rb_node *_nd);

// BST TreeNode
typedef rb_node*(*rbNodeOpt)(rb_node*, _node_value*, rb_tree*);
rb_node* bsNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* bsNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* bsNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);

// AVL TreeNode
rb_node* avlNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* avlNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* avlNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);

// RB TreeNode
rb_node* rbNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* rbNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* rbNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);

// MAVL TreeNode
rb_node* mavlNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* mavlNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);
rb_node* mavlNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr);

#endif