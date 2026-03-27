#include <stdlib.h>
#include <string.h>

#include "json.h"

/*
 * JSON VALUE API
 */
json_value __json_value_true = {
    .typ = JSON_TRUE,
};
json_value __json_value_false = {
    .typ = JSON_FALSE,
};
json_value __json_value_null = {
    .typ = JSON_NULL,
};
json_value __json_value_null_object = {
    .typ = JSON_OBJECT,
    .val = {.ptr=0},
};
json_value __json_value_null_array = {
    .typ = JSON_ARRAY,
    .val = {.ptr=0},
};
json_value* const json_value_true = &__json_value_true;
json_value* const json_value_false = &__json_value_false;
json_value* const json_value_null = &__json_value_null;
json_value* const json_value_null_object = &__json_value_null_object;
json_value* const json_value_null_array = &__json_value_null_array;

json_value *new_json_value_str(const char *_str) {
    MALLOC_TYPE(json_value,val);
    val->typ = _str?JSON_STRING:JSON_NULL;
    val->val.ptr = __strdup(_str);
    return val;
}
json_value *new_json_value_str_shallow(char *_str) {
    MALLOC_TYPE(json_value,val);
    val->typ = _str?JSON_STRING:JSON_NULL;
    val->val.ptr = _str;
    return val;
}

json_value *new_json_value_int(const int64_t _int) {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_INTEGER;
    val->val.i64 = _int;
    return val;
}

json_value *new_json_value_uint(const uint64_t _uint) {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_INTEGER_UINT;
    val->val.u64 = _uint;
    return val;
}

json_value *new_json_value_num(const double _num) {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_NUMBER;
    val->val.f64 = _num;
    return val;
}

json_value *new_json_value_obj(json_object* _obj) {
    MALLOC_TYPE(json_value,val);
    val->typ = _obj?JSON_OBJECT:JSON_NULL;
    val->val.ptr = _obj;
    return val;
}

json_value *new_json_value_arr(json_array* _arr) {
    MALLOC_TYPE(json_value,val);
    val->typ = _arr?JSON_ARRAY:JSON_NULL;
    val->val.ptr = _arr;
    return val;
}

void free_json_value(json_value* _val) {
    switch (_val->typ) {
        case JSON_STRING:
            free(_val->val.ptr);
            free(_val);
        break;

        case JSON_NONE:case JSON_INTEGER:case JSON_INTEGER_UINT:case JSON_NUMBER:
            free(_val);
        break;

        case JSON_OBJECT:
            if(_val!=json_value_null_object){
                free_json_object((json_object*)(_val->val.ptr));
                free(_val);
            }
        break;

        case JSON_ARRAY:
            if(_val!=json_value_null_array){
                free_json_array((json_array*)(_val->val.ptr));
                free(_val);
            }
        break;
    }
}

json_value* new_json_value() {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_NONE;
    return val;
}

json_value* new_json_value_copy(const json_value* _val) {
    MALLOC_TYPE(json_value,val);
    val->typ = _val->typ;
    val->val = _val->val;
    return val;
}

/*
 * JSON OBJECT API 
 */
#define json_object_pair_setup(_pr, _name, _value) \
    do { _pr.name=_name; _pr.value=_value; } while(0)

int json_object_pair_cmp(elem_t _e0, elem_t _e1) {
    json_object_pair *_pair0 = get_elem_ptr(_e0);
    json_object_pair *_pair1 = get_elem_ptr(_e1);
    return __strcmp(_pair0->name, _pair1->name);
}

json_object_pair* make_json_object_pair(const char *_name, json_value *_value) {
    MALLOC_TYPE(json_object_pair, _pr);
    _pr->name = __strdup(_name);
    _pr->value = _value;
    return _pr;
}
json_object_pair* make_json_object_pair_shallow_name(char *_name, json_value *_value) {
    MALLOC_TYPE(json_object_pair, _pr);
    _pr->name = _name;
    _pr->value = _value;
    return _pr;
}

void free_json_object_pair(elem_t _e) {
    json_object_pair *_pr = cast_elem_typ(_e, json_object_pair*);
    free(_pr->name);
    free_json_value(_pr->value);
    free(_pr);
}

json_object* make_json_object() {
    MALLOC_TYPE(json_object, _obj);
    _obj->_tr = makeRBTree(json_object_pair_cmp);
    setRBTreeDataFree(_obj->_tr, free_json_object_pair);
    setRBTreeNodeType(_obj->_tr, TREE_NODE_TYPE_AVL);
    return _obj;
}

void free_json_object(json_object *_obj) {
    freeRBTree(_obj->_tr);
    free(_obj);
}

void json_object_put(json_object *_obj, const char *_name, json_value *_value) {
    json_object_pair *_pr = make_json_object_pair(_name, _value);
    elem_t _em; setup_elem_ptr(_em, _pr);
    rbTreeInsertData(_obj->_tr, _em);
}

void json_object_put_shallow_name(json_object *_obj, char *_name, json_value *_value) {
    json_object_pair *_pr = make_json_object_pair_shallow_name(_name, _value);
    elem_t _em; setup_elem_ptr(_em, _pr);
    rbTreeInsertData(_obj->_tr, _em);
}

void json_object_del(json_object *_obj, char *_name) {
    json_object_pair _pr; json_object_pair_setup(_pr, _name, NULL);
    elem_t _em; setup_elem_ptr(_em, &_pr);
    rbTreeDeleteData(_obj->_tr, _em);
}

json_value* json_object_get(json_object *_obj, char *_name) {
    json_object_pair _pr; json_object_pair_setup(_pr, _name, NULL);
    elem_t _em; setup_elem_ptr(_em, &_pr);
    _em = rbTreeGetData(_obj->_tr, _em);
    if(valid_elem_t(_em)){
        _pr = *cast_elem_typ(_em, json_object_pair*);
        return _pr.value;
    }
    return NULL;
}

void json_object_iter(json_object *_obj,json_object_iter_func _fn) {
    blist *_bl = rbTreeMidTrav(_obj->_tr);
    biter *_it = makeBIter(_bl, BLIST_ITER_FORWARD);
    for(bnode *_bn=bListNext(_it);
        _bn;
        _bn=bListNext(_it)
    ){
        rb_node *_rbnd = get_elem_ptr(_bn->_data);
        json_object_pair *_pair = get_elem_ptr(_rbnd->_data);
        bool _ret = _fn(_pair->name, _pair->value);
        if(!_ret){
            break;
        }
    }
    freeBIter(_it);
    freeBList(_bl);
}

int json_object_size(const json_object *_obj) {
    return __tree_size(_obj->_tr);
}

/*
 * JSON ARRAY
 */
json_array* make_json_array() {
    MALLOC_TYPE(json_array, _arr);
    _arr->_arr = makeBArray(0,0);
    return _arr;
}

void json_array_append(json_array *_arr, json_value *_val) {
    elem_t _em; setup_elem_ptr(_em, _val);
    _arr->_arr = bArrayAppend(_arr->_arr, _em);
}

json_value* json_array_get(json_array *_arr, int _idx) {
    elem_t _em = bArrayAt(_arr->_arr,_idx);
    if(valid_elem_t(_em)){
        return cast_elem_typ(_em, json_value*);
    }
    return NULL;
}

bool free_json_array_elem(size_t _idx, elem_t _dt){
    json_value *val = cast_elem_typ(_dt, json_value*);
    free_json_value(val);
}
void free_json_array(json_array *_arr) {
    bArrayIter(_arr->_arr, free_json_array_elem);
    freeBArray(_arr->_arr);
    free(_arr);
}

int json_array_size(const json_array *_arr) {
    return bArrayLen(_arr->_arr);
}

bool json_valid(const char * _str) {
    json_value *val = json_parse(_str);
    bool ok = val?true:false;
    if(val){
        free_json_value(val);
    }
    return ok;
}