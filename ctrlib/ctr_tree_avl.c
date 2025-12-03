#include "ctr_rbtree.h"

#define __link_(_nd, _ch, _dir) \
	do { \
		if(_dir==RB_NODE_DIR_LEFT){ \
			__link_left(_nd, _ch); \
		} else if(_dir==RB_NODE_DIR_RIGHT){ \
			__link_right(_nd, _ch); \
		} \
	} while(0)

//		A				B	
//   B		Ar	->  Bl	   A
// Bl Br		  C		 Br	 Ar
// C 
rb_node* rotate_right(rb_node *_nd) {
	rb_node *_pa=_nd->_parent; 
	bool _is_lf=__is_left(_nd), _is_right=__is_right(_nd);
	rb_node *_lf=_nd->_left;
	rb_node *_lf_right=_lf?_lf->_right:NULL;
	__link_left(_nd, _lf_right);
	__link_right(_lf, _nd);
	if(_is_lf){
		__link_left(_pa, _lf);
	} else if(_is_right) {
		__link_right(_pa, _lf);
	} else { // _nd is _root
		_lf->_parent = _pa;
	}
	__update_size_height(_nd);
	__update_size_height(_lf);
	return _lf;
}

// 		A						B
// 	 Al		B		->	 A			Br
//		  Bl   Br	  Al	Bl			C
//			    C
rb_node* rotate_left(rb_node *_nd) {
	rb_node *_pa=_nd->_parent;
	bool _is_lf=__is_left(_nd), _is_right=__is_right(_nd);
	rb_node *_ri=_nd->_right;
	rb_node *_ri_left=_ri?_ri->_left:NULL;
	__link_right(_nd, _ri_left);
	__link_left(_ri, _nd);
	if(_is_lf){
		__link_left(_pa, _ri);
	} else if(_is_right) {
		__link_right(_pa, _ri);
	} else { // _nd is _root
		_ri->_parent = _pa;
	}
	__update_size_height(_nd);
	__update_size_height(_ri);
	return _ri;
}

// bf(nd) < 0, called left-heavy;  if bf(nd)==-2, rotate_r(nd)
// bf(nd) > 0, called right-heavy; if bf(nd)==+2, rotate_l(nd)
// bf(nd) = 0, called balanced

// bf(A)==2, bf(B)==1
//         A
//     Al     B
//          Bl   Br
//               C
rb_node* rotate_ll(rb_node *_nd){
    return rotate_left(_nd);
}

// bf(A)==2, bf(B)==-1
//         A
//     Al     B
//          Bl   Br
//          C
rb_node* rotate_rl(rb_node *_nd){
    _nd->_right = rotate_right(_nd->_right);
    return rotate_left(_nd);
}

// bf(A)==-2, bf(B)==-1
//         A
//      B     Ar
//   Bl   Br
//   C
rb_node* rotate_rr(rb_node *_nd){
    return rotate_right(_nd);
}

// bf(A)==-2, bf(B)=1
//         A
//      B     Ar
//   Bl   Br
//        C
rb_node* rotate_lr(rb_node *_nd){
    _nd->_left = rotate_left(_nd->_left);
    return rotate_right(_nd);
}

rb_node* avlNodeAdjust(rb_node *_rt, rb_node *_nd) {
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

/* @return @param<_rt>
 * 1. if found(cmp=0), copy data, return _rt
 * 2. else insert, adjust, return _rt
 * 3. return _rt because _root will change in every rotation
*/ 
rb_node* avlNodeInsertNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	// 1). insert _nd
	rb_node *_cur = _rt, *_p=NULL;
	int _dir = RB_NODE_DIR_NONE;
	while(_cur){
		_p = _cur;
		int cmp_res = _tr->_elem_cmp(_cur->_data, _val->src);
		if(cmp_res == 0){			
			__free_data(_tr, _cur->_data);
			_cur->_data = _val->src;	
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
	_rt = avlNodeAdjust(_rt, _nd);

	return _rt;
}

rb_node* avlNodeGetNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	return bsNodeGetNode(_rt, _val, _tr);
}

rb_node* avlNodeDeleteNode(rb_node *_rt, _node_value *_val, rb_tree *_tr) {
	rb_node *del_nd = avlNodeGetNode(_rt, _val, _tr);
	if(del_nd==NULL){
		_val->retcode = RB_NODE_DELETE_NOTFOUND;
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
		
		_node_value rpl_val; init_node_value(rpl_val, rpl->_data);
		rb_node *lres = avlNodeDeleteNode(del_left, &rpl_val, _tr);

		__link_left(del_nd, lres);
		_rt = avlNodeAdjust(_rt, del_nd);
	} else {
		rb_node *_p = del_nd->_parent;
		int _dir = __node_dir(del_nd);
		rb_node *rpl = del_nd->_left?del_nd->_left:del_nd->_right;
		__link_(_p, rpl, _dir);

		if(_p){
			_rt = avlNodeAdjust(_rt, _p);
		} else {
			_rt = rpl;
		}
		
		freeRBNode(del_nd, _tr);
	}

	return _rt;
}