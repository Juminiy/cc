#include "ctr_rbtree.h"

#include "ctr_bstack.h"
#include "ctr_bqueue.h"

#include <stdlib.h>

rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data) {
	rb_node *nd = (rb_node*)malloc(sizeof(rb_node));
	__link_left(nd, _left);
	__link_right(nd, _right);
	nd->_data = _data;
	nd->_color = RB_NODE_CLR_RED;
	nd->_size = 1;
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

rb_node* bsNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr) {
	if(!_rt) {
		return _nd;
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _nd->_data);
	if(cmp_res==0) {
		// _tr->_elem_set(_rt->_data, _nd->_data); 
		_rt->_data = _nd->_data; // set new to old
	} else if (cmp_res < 0) {
		rb_node *rres = bsNodeInsertNode(_rt->_right, _nd, _tr);
		__link_right(_rt, rres);
	} else {
		rb_node *lres = bsNodeInsertNode(_rt->_left, _nd, _tr);
		__link_left(_rt, lres);
	}
	return _rt;
}

rb_node* bsNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	if(_rt == NULL) {
		return NULL;
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _nd->_data);
	if(cmp_res==0) {
		return _rt;
	} else if (cmp_res < 0) {
		return bsNodeGetNode(_rt->_right, _nd, _tr);
	} else {
		return bsNodeGetNode(_rt->_left, _nd, _tr);
	}
}

rb_node* bsNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	if(_rt==NULL) {
		return NULL;
	}
	
	int cmp_res = _tr->_elem_cmp(_rt->_data, _nd->_data);
	if (cmp_res < 0) {
		rb_node *rres = bsNodeDeleteNode(_rt->_right, _nd, _tr);
		__link_right(_rt, rres);
	} else if (cmp_res > 0) {
		rb_node *lres = bsNodeDeleteNode(_rt->_left, _nd, _tr);
		__link_left(_rt, lres);
	} else {
		// _tr->_elem_set(_nd->_data, _rt->_data); 
		_nd->_data = _rt->_data; // set old to new

		rb_node *rpl = NULL;
		if (__is_leaf(_rt)) {
			// do nothing
		} else if(_rt->_left&&_rt->_right==NULL) {
			rpl = _rt->_left;
		} else if(_rt->_right&&_rt->_left==NULL) {
			rpl = _rt->_right;
		} else {
			rpl = rbNodeMaxNode(_rt->_left, _tr); // may be leaf, must child<=1
			if(rpl != _rt->_left){
				__link_left(rpl, _rt->_left);
			}
			__link_right(rpl, _rt->_right);
			__unlink_parent(rpl);
		}
		
		rb_node *rt_parent = _rt->_parent;
		if(__is_left(_rt)){
			__link_left(rt_parent, rpl);
		} else if(__is_right(_rt)){
			__link_right(rt_parent, rpl);
		}
		__unlink_parent(_rt);
		return rpl;
	}
	return _rt;
}

// deprecated:
// rbNodeDeleteNode
// rb_node *rpl_nd = NULL;
// if(del_nd->_left) {
// 	__unlink_left(del_nd);
// 	rpl_nd = rbNodeMaxNode(del_nd->_left, _tr); // must be a leaf
// 	if(rpl_nd) {
// 		__unlink_parent(rpl_nd);
// 		__link_right(del_nd->_parent, rpl_nd);
// 		if(rpl_nd!=del_nd->_left)
// 			__link_left(rpl_nd, del_nd->_left);
// 		__link_right(rpl_nd, del_nd->_right);
// 	}
// }
// if(del_nd->_right) {
// 	__unlink_right(del_nd);
// 	if(rpl_nd==NULL) {
// 		rpl_nd = rbNodeMinNode(del_nd->_right, _tr); // must be a leaf
// 		if(rpl_nd) {
// 			__unlink_parent(rpl_nd);
// 			__link_right(del_nd->_parent, rpl_nd);
// 			if(rpl_nd!=del_nd->_right)
// 				__link_right(rpl_nd, del_nd->_right);
// 			__link_left(rpl_nd, del_nd->_left);
// 		}
// 	}
// }

rb_tree* makeRBTree(elem_t_cmp _elem_cmp) {
	rb_tree *_tr = (rb_tree*)malloc(sizeof(rb_tree));
	_tr->_root = NULL;
	_tr->_size = 0;
	_tr->_elem_cmp = _elem_cmp;
	return _tr;
}

void freeRBNodeRec(rb_node *_nd) {
	if(_nd==NULL)
		return;
	freeRBNodeRec(_nd->_left);
	freeRBNodeRec(_nd->_right);
	freeRBNode(_nd);
}

void freeRBTree(rb_tree* _tr) {
	// free them recursively
	freeRBNodeRec(_tr->_root);
	free(_tr);
}

rb_node* rbTreeInsertData(rb_tree* _tr, elem_t _dt) {
	return rbTreeInsertNode(_tr, makeRBNode(NULL, NULL, _dt));
}

/*
 * @return @param <_nd>: an inserted new node
*/
rb_node* rbTreeInsertNode(rb_tree* _tr, rb_node *_nd) {
	_tr->_root = rbNodeInsertNode(_tr->_root, _nd, _tr);
	return _nd;
}

rb_node* rbTreeDeleteData(rb_tree* _tr, elem_t _dt) {
	return rbTreeDeleteNode(_tr, makeRBNode(NULL, NULL, _dt));
}

/*
 * @return @param <_nd>: an deleted node
*/
rb_node* rbTreeDeleteNode(rb_tree* _tr, rb_node *_nd) {
	_tr->_root = rbNodeDeleteNode(_tr->_root, _nd, _tr);
	return _nd;
}

rb_node* rbTreeGetData(rb_tree* _tr, elem_t _dt) {
	return rbTreeGetNode(_tr, makeRBNode(NULL, NULL, _dt));
}

rb_node* rbTreeGetNode(rb_tree* _tr, rb_node *_nd) {
	return rbNodeGetNode(_tr->_root, _nd, _tr);
}

blist* rbTreeMidTrav(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; // <rb_node*>
	blist *bl = makeBList();			// list<rb_node*>
	bstack *bstk = makeBStack();		// stack<rb_node*>
	rb_node *nd = _tr->_root;

	// setup_elem_ptr(_el, nd); bStackPush(bstk, _el);
	while(!bStackEmpty(bstk) || nd) {
		while(nd) {
			setup_elem_ptr(_el, nd); bStackPush(bstk, _el);
			nd = nd->_left;
		}
		if(!bStackEmpty(bstk)) {
			_el = bStackPop(bstk); 
			bListAddTail(bl, _el);
			nd = (rb_node*)get_elem_ptr(_el);
			nd = nd->_right;
		}
	}

	freeBStack(bstk);
	return bl;
}

blist* rbTreeLelTrav(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; 
	blist *bl = makeBList();
	bqueue *bq = makeBQueue();
	rb_node *nd = _tr->_root;
	setup_elem_ptr(_el, nd); bQueuePush(bq, _el);

	while(!bQueueEmpty(bq)) {
		_el = bQueuePop(bq);
		bListAddTail(bl, _el);
		nd = (rb_node*)get_elem_ptr(_el);
		if(nd->_left) {
			setup_elem_ptr(_el, nd->_left); bQueuePush(bq,_el);
		}
		if(nd->_right) {
			setup_elem_ptr(_el, nd->_right); bQueuePush(bq,_el);
		}
	}

	freeBQueue(bq);
	return bl;
}