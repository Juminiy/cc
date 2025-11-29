#include "ctr_rbtree.h"

rb_node* rotate_right(rb_node *_nd) {
	rb_node *_pa=_nd->_parent; 
	bool _is_lf=__is_left(_nd), _is_right=__is_right(_nd);
	rb_node *_lf=_nd->_left;
	rb_node *_lf_right=_lf?_lf->_right:NULL;
	__link_right(_lf, _nd);
	__link_left(_nd, _lf_right);
	if(_is_lf){
		__link_left(_pa, _lf);
	} else if(_is_right) {
		__link_right(_pa, _lf);
	}
	return _lf;
}

rb_node* rotate_left(rb_node *_nd) {
	rb_node *_pa=_nd->_parent;
	bool _is_lf=__is_left(_nd), _is_right=__is_right(_nd);
	rb_node *_ri=_nd->_right;
	rb_node *_ri_left=_ri?_ri->_left:NULL;
	__link_left(_ri, _nd);
	__link_right(_nd, _ri_left);
	if(_is_lf){
		__link_left(_pa, _ri);
	} else if(_is_right) {
		__link_right(_pa, _ri);
	}
	return _ri;
}

// 			_g
// 		_p     _u
//   _nd
rb_node* rbNodeAdjust(rb_node *_p, rb_node *_nd, int _dir) {
	
	for(_p = _nd->_parent;_p->_color==RB_NODE_RED;_p = _nd->_parent){
		int p_dir = __node_dir(_p);
		rb_node *_g=_p->_parent;
		rb_node *_u=(p_dir==RB_NODE_DIR_LEFT?_g->_right:_g->_left);
		if(_u->_color==RB_NODE_RED){		// case1: u is RED
			_p->_color = _u->_color = RB_NODE_BLK;
			_g->_color = RB_NODE_RED;
			_nd = _g;
			continue;
		} else if(p_dir!=__node_dir(_nd)){ 	// case2: u is BLACK, p_dir!=n_dir
			if(p_dir==RB_NODE_DIR_LEFT){
				rotate_left(_p);
			} else if(p_dir==RB_NODE_DIR_RIGHT){
				rotate_right(_p);
			}
			__swap_(rb_node*, _nd, _p);
		} else {							// case3: u is BLACK, p_dir=n_dir
			_p->_color = RB_NODE_BLK;
			_g->_color = RB_NODE_RED;
			if(p_dir==RB_NODE_DIR_LEFT) {
				rotate_right(_g);
			} else if(p_dir==RB_NODE_DIR_RIGHT) {
				rotate_left(_g);
			}
		}
	}
}

rb_node* rbNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr) {
	rb_node *_cur = _rt, *_cur_parent=NULL;
	int _dir = RB_NODE_DIR_NONE;
	while(_cur) {
		_cur_parent = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _nd->_data);
		if(cmp_res==0){
			_cur->_data = _nd->_data;
			break;
		} else if(cmp_res<0){
			_cur = _cur->_right;
			_dir = RB_NODE_DIR_RIGHT;
		} else {
			_cur = _cur->_left;
			_dir = RB_NODE_DIR_LEFT;
		}
	}
	if(!_cur){
		if(_dir==RB_NODE_DIR_LEFT){
			__link_left(_cur_parent, _nd);
		} else if(_dir==RB_NODE_DIR_RIGHT){
			__link_right(_cur_parent, _nd);
		} else {
			_rt = _nd;
		}
	}
	return _rt;
}

rb_node* rbNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	rb_node *_cur = _rt;
	while(_cur) {
		int cmp_res = _tr->_elem_cmp(_cur->_data, _nd->_data);
		if(cmp_res==0){
			return _cur;
		} else if(cmp_res<0){
			_cur = _cur->_right;
		} else {
			_cur = _cur->_left;
		}
	}
	return NULL;
}

rb_node* rbNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	return _rt;
}
