#include "ctr_rbtree.h"

rb_node* mavlNodeAdjust(rb_node *_rt, rb_node *_nd) {
	if(!_nd){
		return NULL;
	}
	for(rb_node *_cur = _nd; _cur; _cur=_cur->_parent) {
		__update_size_height(_cur);
		int cur_bf = __node_bf(_cur);
		if(cur_bf==2) {				// right-heavy
			int right_bf = __node_bf(_cur->_right);
			if(right_bf>=0){
				_cur = rotate_ll(_cur);
			} else if(right_bf<0){
				_cur = rotate_rl(_cur);
			}

		} else if(cur_bf==-2){		// left-heavy
			int left_bf = __node_bf(_cur->_left);
			if(left_bf<=0){
				_cur = rotate_rr(_cur);
			} else if(left_bf>0){
				_cur = rotate_lr(_cur);
			}
		}
		_rt = _cur;
	}
	return _rt;
}

#define __update_psize(_cur, _inc) \
    do { \
        for(rb_node *_p=_cur->_parent;_p;_p=_p->_parent){ \
            _p->_size += _inc; \
        } \
    } while(0)

/* @return @param<_rt>
 * 1. if found(cmp=0), copy data, return _rt
 * 2. else insert, adjust, return _rt
 * 3. return _rt because _root will change in every rotation
*/ 
rb_node* mavlNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	// 1). insert _nd
	rb_node *_cur = _rt, *_p=NULL;
	int _dir = RB_NODE_DIR_NONE;
	while(_cur){
		_p = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _val->src);
		if(cmp_res == 0){			
			__free_data(_tr, _cur->_data);
			_cur->_data = _val->src;	
            __update_cnt(_cur, +1);
            __update_psize(_cur, +1);
			_val->retcode = RB_NODE_INSERT_REPLACED;
			return _rt;
		} else if(cmp_res < 0){
			_cur = _cur->_right;
			_dir = RB_NODE_DIR_RIGHT;
		} else {
			_cur = _cur->_left;
			_dir = RB_NODE_DIR_LEFT;
		}
	}

	rb_node *_nd = makeRBNode(NULL,NULL,_val->src);
	_val->retcode = RB_NODE_INSERT_CREATED;
	if(!_cur){
		if(_dir==RB_NODE_DIR_LEFT){
			__link_left(_p, _nd);
		} else if(_dir==RB_NODE_DIR_RIGHT){
			__link_right(_p, _nd);
		} else {
			return _nd;				// root is null, insert return
		}
	}


	// 2). 
	// search _nd._p._p._p... ancestors for loop, rectify
	// update size and height
	_rt = mavlNodeAdjust(_rt, _nd);

	return _rt;
}

rb_node* mavlNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	return bsNodeGetNode(_rt, _val, _tr);
}

rb_node* mavlNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	rb_node *del_nd = mavlNodeGetNode(_rt, _val, _tr);
	if(del_nd==NULL){
		_val->retcode = RB_NODE_DELETE_NOTFOUND;
		return _rt;
	}
    if(del_nd->_cnt>0){
		__update_cnt(del_nd, -1);
        __update_psize(del_nd, -1);
		if(del_nd->_cnt>0)
        	return _rt;
    }

	_val->dst = del_nd->_data;
	_val->retcode = RB_NODE_DELETE_SUCCESS;

	if (del_nd->_left&&del_nd->_right){ // left and right
		//			A
		//		Al		Ar
		//	..	   ..   ...
		//			rpl

		// left subtree
		rb_node *del_left=del_nd->_left;
		__unlink_left(del_nd);
		
		rb_node *rpl = bsNodeMaxNode(del_left);
		__swap_(elem_t, del_nd->_data, rpl->_data);
		__swap_(size_t, del_nd->_cnt, rpl->_cnt);
		
		_node_value rpl_val; init_node_value(rpl_val, rpl->_data);
		rb_node *lres = mavlNodeDeleteNode(del_left, &rpl_val, _tr);

		__link_left(del_nd, lres);
		_rt = mavlNodeAdjust(_rt, del_nd);
	} else {
		rb_node *_p = del_nd->_parent;
		int _dir = __node_dir(del_nd);
		rb_node *rpl = del_nd->_left?del_nd->_left:del_nd->_right;
		__link_(_p, rpl, _dir);

		if(_p){
			_rt = mavlNodeAdjust(_rt, _p);
		} else {
			_rt = rpl;
		}
		
		freeRBNode(del_nd, _tr);
	}

	return _rt;
}