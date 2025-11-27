#include "ctr_rbtree.h"

#include <stdlib.h>

#define __link_left(nd, left) \
	do { if(nd) {nd->_left=left;} if(left) {left->_parent=nd;} } while(0)

#define __link_right(nd, right) \
	do { if(nd) {nd->_right=right;} if(right) {right->_parent=nd;} } while(0)

#define __unlink_left(nd) \
	do { if(nd->_left) {nd->_left->_parent=NULL;} if(nd) {nd->_left=NULL;} } while(0) 

#define __unlink_right(nd) \
	do { if(nd->_right) {nd->_right->_parent=NULL;} if(nd) {nd->_right=NULL;} } while(0) 

#define __unlink_parent(nd) \
	do { \
		if(nd->_parent) { \
			if(nd->_parent->_left==nd) __unlink_left(nd->_parent); \
			if(nd->_parent->_right==nd) __unlink_right(nd->_parent); \
		} \
	} while(0)

#define __is_leaf(nd) (nd->_left==NULL&&nd->_right==NULL)

rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data) {
	rb_node *nd = (rb_node*)malloc(sizeof(rb_node));
	__link_left(nd, _left);
	__link_right(nd, _right);
	nd->_data = _data;
	return nd;
}

void freeRBNode(rb_node* _nd) {
	__unlink_left(_nd);
	__unlink_right(_nd);
	free(_nd);
}

rb_node* rbNodeMinNode(rb_node* _rt, rb_tree *_tr) {
	rb_node *_cur = _rt;
	while(_cur && _cur->_left) {
		_cur = _cur->_left;
	}
	return _cur;
}

rb_node* rbNodeMaxNode(rb_node* _rt, rb_tree *_tr) {
	rb_node *_cur = _rt;
	while(_cur && _cur->_right) {
		_cur = _cur->_right;
	}
	return _cur;
}

rb_node* rbNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr) {
	if(!_rt) {
		return _nd;
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _nd->_data);
	if(cmp_res==0) {
		_tr->_elem_set(_rt->_data, _nd->_data);
	} else if (cmp_res < 0) {
		rb_node *rres = rbNodeInsertNode(_rt->_right, _nd, _tr);
		__link_right(_rt, rres);
	} else {
		rb_node *lres = rbNodeInsertNode(_rt->_left, _nd, _tr);
		__link_left(_rt, lres);
	}
	return _rt;
}

rb_node* rbNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	if(_rt == NULL) {
		return NULL;
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _nd->_data);
	if(cmp_res==0) {
		return _rt;
	} else if (cmp_res < 0) {
		return rbNodeGetNode(_rt->_right, _nd, _tr);
	} else {
		return rbNodeGetNode(_rt->_left, _nd, _tr);
	}
}

rb_node* rbNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	rb_node *del_nd = rbNodeGetNode(_rt, _nd, _tr);
	if(!del_nd) {
		return NULL;
	}
	if(!__is_leaf(del_nd)) {
		rb_node *rpl_nd = NULL;
		if(del_nd->_left) {
			__unlink_left(del_nd);
			rpl_nd = rbNodeMaxNode(del_nd->_left, _tr); // must be a leaf
			if(rpl_nd) {
				__unlink_parent(rpl_nd);
				__link_right(del_nd->_parent, rpl_nd);
				if(rpl_nd!=del_nd->_left)
					__link_left(rpl_nd, del_nd->_left);
				__link_right(rpl_nd, del_nd->_right);
			}
		}
		if(del_nd->_right) {
			__unlink_right(del_nd);
			if(rpl_nd==NULL) {
				rpl_nd = rbNodeMinNode(del_nd->_right, _tr);
				if(rpl_nd) {
					__unlink_parent(rpl_nd);
					__link_right(del_nd->_parent, rpl_nd);
					if(rpl_nd!=del_nd->_right)
						__link_right(rpl_nd, del_nd->_right);
					__link_left(rpl_nd, del_nd->_left);
				}
			}
		}
	}
	__unlink_parent(del_nd);
	return del_nd;
}

rb_tree* makeRBTree(elem_t_cmp _elem_cmp) {
	rb_tree *_tr = (rb_tree*)malloc(sizeof(rb_tree));
	_tr->_root = NULL;
	_tr->_size = 0;
	_tr->_elem_cmp = _elem_cmp;
	return _tr;
}

void freeRBTree(rb_tree* _tr) {
	// free them recursively
	free(_tr);
}

rb_tree* rbTreeInsertData(rb_tree* _tr, elem_t _dt) {
	return rbTreeInsertNode(_tr, makeRBNode(NULL, NULL, _dt));
}

rb_tree* rbTreeInsertNode(rb_tree* _tr, rb_node *_nd) {
	_tr->_root = rbNodeInsertNode(_tr->_root, _nd, _tr);
	return _tr;
}

rb_node* rbTreeDeleteData(rb_tree* _tr, elem_t _dt) {
	return rbTreeDeleteNode(_tr, makeRBNode(NULL, NULL, _dt));
}

rb_node* rbTreeDeleteNode(rb_tree* _tr, rb_node *_nd) {
	rb_node *del_nd = rbNodeDeleteNode(_tr->_root, _nd, _tr);
	if(del_nd==_tr->_root) {
		_tr->_root = NULL;
	}
	return del_nd;
}

rb_node* rbTreeGetData(rb_tree* _tr, elem_t _dt) {
	return rbTreeGetNode(_tr, makeRBNode(NULL, NULL, _dt));
}

rb_node* rbTreeGetNode(rb_tree* _tr, rb_node *_nd) {
	return rbNodeGetNode(_tr->_root, _nd, _tr);
}

