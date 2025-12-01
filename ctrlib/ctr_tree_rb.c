#include "ctr_rbtree.h"

#define __child_(_nd, _dir) ((_dir)==RB_NODE_DIR_LEFT?_nd->_left:_nd->_right)
#define __rotate_(_nd, _dir) \
	do { \
		if((_dir)==RB_NODE_DIR_LEFT) rotate_left(_nd); \
		else if((_dir)==RB_NODE_DIR_RIGHT) rotate_right(_nd); \
	} while(0)

void rbNodeInsertAdjust(rb_node *_nd) {
	
	for(rb_node *_p = _nd->_parent;__is_red(_p);_p = _nd->_parent){
		int p_dir = __node_dir(_p);
		rb_node *_g=_p->_parent; 
		rb_node *_u=__child_(_g, !p_dir);
		if(__is_red(_u)){					// case1: u is RED
			_p->_color = _u->_color = RB_NODE_CLR_BLK;
			_g->_color = RB_NODE_CLR_RED;
			_nd = _g;
			continue;
		} 
		if(p_dir!=__node_dir(_nd)){ 		// case2: u is BLACK, p_dir!=n_dir
			__rotate_(_p, p_dir);
			__swap_(rb_node*, _nd, _p);
		} 						
		// case3: u is BLACK, p_dir=n_dir
		_p->_color = RB_NODE_CLR_BLK;
		_g->_color = RB_NODE_CLR_RED;
		__rotate_(_g, !p_dir);
	}
}

// @return @param _rt: _tr->_root itself
rb_node* rbNodeInsertNode(rb_node* _rt, rb_node* _nd, rb_tree *_tr) {
	rb_node *_cur = _rt, *_cur_parent=NULL;
	int _dir = RB_NODE_DIR_NONE;
	
	// find insert or update position
	while(_cur) {
		_cur_parent = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _nd->_data);
		if(cmp_res==0){ // find update
			_cur->_data = _nd->_data;
			return _rt;
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
		} else { // _cur_parent == NULL
			return _nd;
		}
	}

	// node _sizeAdd
	for(rb_node *_curnd=_cur_parent;_curnd;_curnd=_curnd->_parent){
		__update_size_height(_curnd); 
		// _curnd->_size++; // if height is not used, optimized to this line
	}

	// rotate & _colorChange
	rbNodeInsertAdjust(_nd);

	return _rt;
}

// @return if found: node, else: NULL
rb_node* rbNodeGetNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	return avlNodeGetNode(_rt, _nd, _tr);
}

void rbNodeDeleteAdjust(rb_node *_nd, rb_tree *_tr) {
	int n_dir = __node_dir(_nd);
	rb_node *_p = _nd->_parent;
	if(!_p){
		if(_tr->_root) {
			_tr->_root->_color = RB_NODE_CLR_BLK;
		}
		return;
	} else {
		rb_node *_s = __child_(_p, n_dir);
		if(_s){
			_s->_color = RB_NODE_CLR_BLK;
			return;
		}
	}

	while(_p && !__is_red(_nd)) {
		rb_node *_s = __child_(_p, !n_dir);
		if(__is_red(_s)){
			_s->_color=RB_NODE_CLR_BLK, _p->_color=RB_NODE_CLR_RED;
			__rotate_(_p, n_dir);
			_s = __child_(_p, !n_dir);
		}

		rb_node *_c=__child_(_s, n_dir), *_d=__child_(_s, !n_dir);
		if(!__is_red(_c) && !__is_red(_d)){
			_s->_color = RB_NODE_CLR_RED;
			_nd = _p;
			goto __endup_tag;
		}

		if(!__is_red(_d)){
			_c->_color=RB_NODE_CLR_BLK, _s->_color=RB_NODE_CLR_RED;
			__rotate_(_s, !n_dir);
			_s = __child_(_p, !n_dir), _c = __child_(_s, n_dir), _d = __child_(_s, !n_dir);
		}

		_s->_color = _p->_color, _p->_color = _d->_color = RB_NODE_CLR_BLK;
		__rotate_(_p, n_dir); _nd = _tr->_root;
	__endup_tag:
		_p=_nd->_parent;
		if(!_p) break;
		n_dir = __node_dir(_nd);
	}
	_nd->_color = RB_NODE_CLR_BLK;
}

// @return @param<_rt>: _tr->_root itself
rb_node* rbNodeDeleteNode(rb_node *_rt, rb_node *_nd, rb_tree *_tr) {
	rb_node *del_nd = rbNodeGetNode(_rt, _nd, _tr);
	if(del_nd==NULL){
		return _rt;
	}

	_nd->_data = del_nd->_data;
	_nd = del_nd;
	rb_node *_res=NULL;
	if(_nd->_left&&_nd->_right){
		rb_node *_suc = bsNodeMinNode(_nd->_right,_tr);
		__swap_(elem_t, _suc->_data, _nd->_data);
		_res = _nd, _nd = _suc;
	} else {
		_res = bsNodeNextNode(_nd, _tr);
	}

	int n_dir=__node_dir(_nd);
	rb_node *_p=_nd->_parent, *_s=_nd->_left?_nd->_left:_nd->_right;
	if(_s){
		_s->_parent=_p;
	}
	if(!_p){
		_tr->_root = _s;
	} else {
		if(n_dir==RB_NODE_DIR_LEFT){
			_p->_left=_s;
		} else if(n_dir==RB_NODE_DIR_RIGHT){
			_p->_right=_s;
		}
		for(rb_node *_curnd=_p;_curnd;_curnd=_curnd->_parent){
			_curnd->_size--;
		}
	}
	
	rbNodeDeleteAdjust(_nd, _tr);

	freeRBNode(del_nd);
	return _rt;
}
