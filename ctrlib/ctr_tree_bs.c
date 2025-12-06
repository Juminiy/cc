#include "ctr_rbtree.h"

#include "ctr_bstack.h"
#include "ctr_bqueue.h"

#include <stdlib.h>

/*==============================================================================
 * MARK: - Tree (Implementation)
 *============================================================================*/

rb_tree* makeRBTree(elem_t_cmp _elem_cmp) {
	rb_tree *_tr = (rb_tree*)malloc(sizeof(rb_tree));
	_tr->_root = NULL;
	_tr->_size = 0;
	_tr->_elem_cmp = _elem_cmp;
	_tr->_elem_free = NULL;
	_tr->_elem_merge = NULL;
	_tr->_node_type = TREE_NODE_TYPE_BS;
	return _tr;
}

void freeRBTree(rb_tree* _tr) {
	// free them recursively
	freeRBNodeRec(_tr->_root, _tr);
	free(_tr);
}

int rbTreeInsertData(rb_tree* _tr, elem_t _dt) {
	_node_value _val; init_node_value(_val, _dt);
	rbTreeInsertNode(_tr, &_val);
	return _val.retcode;
}

int rbTreeDeleteData(rb_tree* _tr, elem_t _dt) {
	_node_value _val; init_node_value(_val, _dt);
	rbTreeDeleteNode(_tr, &_val);
	return _val.retcode;
}

elem_t rbTreeGetData(rb_tree* _tr, elem_t _dt) {
	_node_value _val; init_node_value(_val, _dt);
	rbTreeGetNode(_tr, &_val);
	return _val.dst;
}

elem_t rbTreeMaxData(rb_tree *_tr) {
	elem_t _em={.tag=ELEM_T_INVALID};
	rb_node *_nd = bsNodeMaxNode(_tr->_root);
	return _nd ? _nd->_data : _em;
}

elem_t rbTreeMinData(rb_tree *_tr) {
	elem_t _em={.tag=ELEM_T_INVALID};
	rb_node *_nd = bsNodeMinNode(_tr->_root);
	return _nd ? _nd->_data : _em;
}

/*
 * @return void
*/
void rbTreeInsertNode(rb_tree *_tr, _node_value *_val) {
	switch (_tr->_node_type) {
		case TREE_NODE_TYPE_MAVL:
		_tr->_root = mavlNodeInsertNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_AVL:
		_tr->_root = avlNodeInsertNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_RB:
		_tr->_root = rbNodeInsertNode(_tr->_root, _val, _tr);
		_tr->_root->_color = RB_NODE_CLR_BLK; // set root BLACK
		break;

		default: // TREE_NODE_TYPE_BS, or other
		_tr->_root = bsNodeInsertNode(_tr->_root, _val, _tr);
		break;
	}
}

/*
 * @return void
*/
void rbTreeDeleteNode(rb_tree *_tr, _node_value *_val) {
	switch (_tr->_node_type) {
		case TREE_NODE_TYPE_MAVL:
		_tr->_root = mavlNodeDeleteNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_AVL:
		_tr->_root = avlNodeDeleteNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_RB:
		_tr->_root = rbNodeDeleteNode(_tr->_root, _val, _tr);
		break;

		default: // TREE_NODE_TYPE_BS, or other
		_tr->_root = bsNodeDeleteNode(_tr->_root, _val, _tr);
		break;
	}
}

/*
 * @return node in tree with links
*/
rb_node* rbTreeGetNode(rb_tree *_tr, _node_value *_val) {
	rb_node *_nd;
	switch (_tr->_node_type) {
		case TREE_NODE_TYPE_MAVL:
		_nd = mavlNodeGetNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_AVL:
		_nd = avlNodeGetNode(_tr->_root, _val, _tr);
		break;

		case TREE_NODE_TYPE_RB:
		_nd = rbNodeGetNode(_tr->_root, _val, _tr);
		break;

		default: // TREE_NODE_TYPE_BS, or other
		_nd = bsNodeGetNode(_tr->_root, _val, _tr);
		break;
	}
	return _nd;
}



/*==============================================================================
 * MARK: - Tree Node (Implementation)
 *============================================================================*/

rb_node* makeRBNode(rb_node* _left, rb_node* _right, elem_t _data) {
	rb_node *nd = (rb_node*)malloc(sizeof(rb_node));
	__link_left(nd, _left);
	__link_right(nd, _right);
	nd->_parent = NULL;
	nd->_data = _data;
	nd->_color = RB_NODE_CLR_RED;
	nd->_size = 1;
	nd->_height = 1;
	nd->_cnt = 1;
	return nd;
}

void freeRBNode(rb_node* _nd, rb_tree* _tr) {
	__free_data(_tr, _nd->_data);
	// __unlink_left(_nd);
	// __unlink_right(_nd);
	// __unlink_parent(_nd);
	free(_nd);
}

void freeRBNodeRec(rb_node *_nd, rb_tree *_tr) {
	if(_nd==NULL)
		return;
	freeRBNodeRec(_nd->_left, _tr);
	freeRBNodeRec(_nd->_right, _tr);
	freeRBNode(_nd, _tr);
}

// Minimum
rb_node* bsNodeMinNode(rb_node* _rt) {
	rb_node *_cur = _rt;
	while(_cur && _cur->_left) {
		_cur = _cur->_left;
	}
	return _cur;
}

// Maximum
rb_node* bsNodeMaxNode(rb_node* _rt) {
	rb_node *_cur = _rt;
	while(_cur && _cur->_right) {
		_cur = _cur->_right;
	}
	return _cur;
}

// Predecessor
rb_node* bsNodePreNode(rb_node* _rt) { 
	if(_rt->_left){
		return bsNodeMaxNode(_rt->_left);
	}
	rb_node *_p = _rt->_parent;
	while(_p&&_p->_left==_rt){
		_rt = _p, _p = _p->_parent;
	}
	return _p;
}

// Successor
rb_node* bsNodeNextNode(rb_node* _rt) {
	if(_rt->_right){
		return bsNodeMinNode(_rt->_right);
	}
	rb_node *_p = _rt->_parent;
	while(_p&&_p->_right==_rt){
		_rt = _p, _p = _p->_parent;
	}
	return _p;
}

// Predecessor data MayNotExist
elem_t bsDataPrevData(rb_tree *_tr, elem_t _dt) { 
	rb_node *_cur = _tr->_root, *_p=NULL;
	int _dir = RB_NODE_DIR_NONE;
	while(_cur){
		_p = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _dt);
		if(cmp_res==0){
			break;
		} else if(cmp_res > 0) {
			_cur = _cur->_left;
			_dir = RB_NODE_DIR_LEFT;
		} else {
			_cur = _cur->_right;
			_dir = RB_NODE_DIR_RIGHT;
		}
	}
	rb_node *_prev=_cur?bsNodePreNode(_cur):
		(_p?(_dir==RB_NODE_DIR_LEFT?bsNodePreNode(_p):_p):NULL);
	elem_t _em={.tag=ELEM_T_INVALID};
	return _prev?_prev->_data:_em;
}

// Successor data MayNotExist
elem_t bsDataNextData(rb_tree *_tr, elem_t _dt) {
	rb_node *_cur = _tr->_root, *_p=NULL;
	int _dir = RB_NODE_DIR_NONE;
	while(_cur){
		_p = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _dt);
		if(cmp_res==0){
			break;
		} else if(cmp_res > 0) {
			_cur = _cur->_left;
			_dir = RB_NODE_DIR_LEFT;
		} else {
			_cur = _cur->_right;
			_dir = RB_NODE_DIR_RIGHT;
		}
	}
	rb_node *_next=_cur?bsNodeNextNode(_cur):
		(_p?(_dir==RB_NODE_DIR_RIGHT?bsNodeNextNode(_p):_p):NULL);
	elem_t _em={.tag=ELEM_T_INVALID};
	return _next?_next->_data:_em;
}

size_t bsDataRankRec(rb_node *_rt, elem_t _dt, rb_tree *_tr) {
	if(!_rt) {
		return 0;
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _dt);
	if(cmp_res==0){
		return __node_size(_rt->_left) + 1;
	} else if (cmp_res>0){
		return bsDataRankRec(_rt->_left, _dt, _tr);
	} else {
		return __node_size(_rt->_left) + __node_cnt(_rt) + bsDataRankRec(_rt->_right, _dt, _tr);
	}
}

// data rank MayNotExist
size_t bsDataRank(rb_tree *_tr, elem_t _dt) {
	_node_value nval; init_node_value(nval, _dt);
	if(_tr->_root==NULL){
		return 0;
	}
	rb_node *nd = bsNodeGetNode(_tr->_root, &nval, _tr);
	if(nd){
		return bsDataRankRec(_tr->_root, _dt, _tr);
	} else {
		elem_t _tr_max=bsNodeMaxNode(_tr->_root)->_data;
		elem_t _tr_min=bsNodeMinNode(_tr->_root)->_data;
		if(_tr->_elem_cmp(_tr_max,_dt)<0){
			return __tree_size(_tr) + 1;
		} else if(_tr->_elem_cmp(_tr_min,_dt)>0){
			return 0;
		}
		return bsDataRankRec(_tr->_root, bsDataNextData(_tr, _dt), _tr);
	}
	
}

elem_t bsDataRankKDataRec(rb_node *_rt, size_t _rank, rb_tree *_tr) {
	if(!_rt){
		elem_t _em={.tag=ELEM_T_INVALID};
		return _em;
	}
	if(_rt->_left){
		if(__node_size(_rt->_left) >= _rank)
			return bsDataRankKDataRec(_rt->_left, _rank, _tr);
		if(__node_size(_rt->_left) + __node_cnt(_rt) >= _rank)
			return _rt->_data;
	} else {
		if(__node_cnt(_rt) >= _rank)
			return _rt->_data;
	}

	return bsDataRankKDataRec(_rt->_right, _rank-__node_size(_rt->_left)-__node_cnt(_rt), _tr);
}

// rank_k data
elem_t bsDataRankKData(rb_tree *_tr, size_t _rank) {
	return bsDataRankKDataRec(_tr->_root, _rank, _tr);
}



/*==============================================================================
 * MARK: - BST TreeNode (Implementation)
 *============================================================================*/

rb_node* bsNodeInsertNode(rb_node* _rt, _node_value *_val, rb_tree *_tr) {
	if(!_rt) {
		_val->retcode = RB_NODE_INSERT_CREATED;
		return makeRBNode(NULL, NULL, _val->src);
	}
	int cmp_res = _tr->_elem_cmp(_rt->_data, _val->src);
	if(cmp_res==0) {
		__free_data(_tr, _rt->_data);
		_rt->_data = _val->src;
		_val->retcode = RB_NODE_INSERT_REPLACED;
	} else if (cmp_res < 0) {
		rb_node *rres = bsNodeInsertNode(_rt->_right, _val, _tr);
		__link_right(_rt, rres);
	} else {
		rb_node *lres = bsNodeInsertNode(_rt->_left, _val, _tr);
		__link_left(_rt, lres);
	}
	return _rt;
}

// @return @param<_nd>: copy data to _nd and return _nd
rb_node* bsNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	rb_node *_cur = _rt;
	while(_cur){
		int cmp_res = _tr->_elem_cmp(_cur->_data, _val->src);
		if(cmp_res == 0){
			_val->dst = _cur->_data;
			_val->retcode = RB_NODE_GET_SUCCESS;
			return _cur;
		} else if(cmp_res < 0){
			_cur = _cur->_right;
		} else {
			_cur = _cur->_left;
		}
	}
	_val->retcode = RB_NODE_GET_NOTFOUND;
	return NULL;
}

// @return @param<_rt>: copy data to _nd and return _rt
rb_node* bsNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	if(_rt==NULL) {
		_val->retcode = RB_NODE_DELETE_NOTFOUND;
		return NULL;
	}

	int cmp_res = _tr->_elem_cmp(_rt->_data, _val->src);
	if (cmp_res < 0) {
		rb_node *rres = bsNodeDeleteNode(_rt->_right, _val, _tr);
		__link_right(_rt, rres);
	} else if (cmp_res > 0) {
		rb_node *lres = bsNodeDeleteNode(_rt->_left, _val, _tr);
		__link_left(_rt, lres);
	} else {
		// _tr->_elem_set(_nd->_data, _rt->_data); 
		_val->dst = _rt->_data; // set old to new
		_val->retcode = RB_NODE_DELETE_SUCCESS;

		if(_rt->_right==NULL) {
			rb_node *_tmp=_rt->_left;
			freeRBNode(_rt, _tr);
			return _tmp;
		} else if(_rt->_left==NULL) {
			rb_node *_tmp=_rt->_right;
			freeRBNode(_rt, _tr);
			return _tmp;
		} else {
			rb_node *rpl = bsNodeMaxNode(_rt->_left); // may be leaf, must child<=1
			_rt->_data = rpl->_data;
			_node_value rpl_val; init_node_value(rpl_val, rpl->_data);
			rb_node *lres = bsNodeDeleteNode(_rt->_left, &rpl_val, _tr);
			__link_left(_rt, lres);
		}
	}
	return _rt;
}

// @deprecated
// rbNodeDeleteNode
// rb_node *rpl_nd = NULL;
// if(del_nd->_left) {
// 	__unlink_left(del_nd);
// 	rpl_nd = bsNodeMaxNode(del_nd->_left, _tr); // must be a leaf
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
// 		rpl_nd = bsNodeMinNode(del_nd->_right, _tr); // must be a leaf
// 		if(rpl_nd) {
// 			__unlink_parent(rpl_nd);
// 			__link_right(del_nd->_parent, rpl_nd);
// 			if(rpl_nd!=del_nd->_right)
// 				__link_right(rpl_nd, del_nd->_right);
// 			__link_left(rpl_nd, del_nd->_left);
// 		}
// 	}
// }



/*==============================================================================
 * MARK: - Tree Traversal (Implementation)
 *============================================================================*/

// @attention returned must be freed
blist* rbTreeMidTravData(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; // <*rb_node>
	blist *bl = makeBList();			// blist<*rb_node->_data>
	bstack bstk = makeBStack();			// bstack<*rb_node>
	rb_node *nd = _tr->_root;

	// setup_elem_ptr(_el, nd); bStackPush(bstk, _el);
	while(!bStackEmpty(bstk) || nd) {
		while(nd) {
			setup_elem_ptr(_el, nd); bStackPush(bstk, _el);
			nd = nd->_left;
		}
		if(!bStackEmpty(bstk)) {
			_el = bStackPop(bstk); 
			nd = (rb_node*)get_elem_ptr(_el);
			bListAddTail(bl, nd->_data);
			nd = nd->_right;
		}
	}

	freeBStack(bstk);
	return bl;
}

// @attention returned must be freed
blist* rbTreeLelTravData(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; // *rb_node
	blist *bl = makeBList();			// blist<*rb_node->data>
	bqueue bq = makeBQueue();			// bqueue<*rb_node>
	rb_node *nd = _tr->_root;
	if(nd){
		setup_elem_ptr(_el, nd); bQueuePush(bq, _el);
	}

	while(!bQueueEmpty(bq)) {
		_el = bQueuePop(bq);
		nd = (rb_node*)get_elem_ptr(_el);
		bListAddTail(bl, nd->_data);
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

// @attention returned must be freed
blist* rbTreeMidTrav(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; // <rb_node*>
	blist *bl = makeBList();			// blist<rb_node*>
	bstack bstk = makeBStack();			// bstack<rb_node*>
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

// @attention returned must be freed
blist* rbTreeLelTrav(rb_tree* _tr) {
	elem_t _el = {.tag=ELEM_T_INVALID}; // <rb_node*>
	blist *bl = makeBList();			// blist<rb_node*>
	bqueue bq = makeBQueue();			// bqueue<rb_node*>
	rb_node *nd = _tr->_root;
	if(nd){
		setup_elem_ptr(_el, nd); bQueuePush(bq, _el);
	}

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