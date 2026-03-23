#include <stdlib.h>
#include <string.h>

#include "json.h"

/*
 * JSON VALUE API
 */
const json_value json_value_true = {
    .typ = JSON_TRUE,
};
const json_value json_value_false = {
    .typ = JSON_FALSE,
};
const json_value json_value_null = {
    .typ = JSON_NULL,
};
const json_value json_value_null_object = {
    .typ = JSON_OBJECT,
    .val = {.ptr=0},
};
const json_value json_value_null_array = {
    .typ = JSON_ARRAY,
    .val = {.ptr=0},
};

json_value *new_json_value_str(const char *_str) {
    MALLOC_TYPE(json_value,val);
    val->typ = _str?JSON_STRING:JSON_NULL;
    val->val.ptr = __strdup(_str);
    return val;
}

json_value *new_json_value_int(const int64_t _int) {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_INTEGER;
    val->val.i64 = _int;
    return val;
}

json_value *new_json_value_num(const double _num) {
    MALLOC_TYPE(json_value,val);
    val->typ = JSON_NUMBER;
    val->val.f64 = _num;
    return val;
}

json_value *new_json_value_obj(const json_object* _obj) {
    MALLOC_TYPE(json_value,val);
    val->typ = _obj?JSON_OBJECT:JSON_NULL;
    val->val.ptr = _obj;
    return val;
}

json_value *new_json_value_arr(const json_array* _arr) {
    MALLOC_TYPE(json_value,val);
    val->typ = _arr?JSON_ARRAY:JSON_NULL;
    val->val.ptr = _arr;
    return val;
}

void free_json_value(json_value *_val) {
    switch (_val->typ) {
        case JSON_STRING:
            free(_val->val.ptr);
        break;

        case JSON_OBJECT:
            free_json_object((json_object*)(_val->val.ptr));
        break;

        case JSON_ARRAY:
            free_json_array((json_array*)(_val->val.ptr));
        break;
    }
    free(_val);
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

void json_object_del(json_object *_obj, const char *_name) {
    json_object_pair _pr; json_object_pair_setup(_pr, _name, NULL);
    elem_t _em; setup_elem_ptr(_em, &_pr);
    rbTreeDeleteData(_obj->_tr, _em);
}

json_value* json_object_get(json_object *_obj, const char *_name) {
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

char* json_object_encode(const json_object *_obj) {
    char *buf = NULL;
    int cnt = 0, siz = json_object_size(_obj);
    bool encode_obj_fn(char *_name, json_value *_value) {
        cnt++;
        buf = __strcat(buf, "\"");
        buf = __strcat(buf, _name);
        buf = __strcat(buf, "\":");
        buf = __strcat(buf, json_stringify(_value));
        if (cnt<siz) {
            buf = __strcat(buf, ",");
        }
        return true;
    };
    json_object_iter(_obj, encode_obj_fn);
    return buf;
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

void free_json_array(json_array *_arr) {
    freeBArray(_arr->_arr);
    free(_arr);
}

int json_array_size(const json_array *_arr) {
    return bArrayLen(_arr->_arr);
}

char* json_array_encode(const json_array *_arr) {
    char *buf = NULL;
    int cnt = 0, siz = json_array_size(_arr);
    bool encode_arr_fn(size_t _idx, elem_t _dt) {
        cnt++;
        json_value *val = cast_elem_typ(_dt, json_value*);
        buf = __strcat(buf, json_stringify(val));
        if (cnt<siz) {
            buf = __strcat(buf, ",");
        }
        return true;
    };
    bArrayIter(_arr->_arr, encode_arr_fn);
    return buf;
}

/*
 * JSON API
 */
char* json_stringify(const json_value *_val) {
    
    char * buf = NULL;
    switch (_val->typ) {
        case JSON_TRUE:
        buf = __strcat(buf,"true");
        break;

        case JSON_FALSE:
        buf = __strcat(buf,"false");
        break;

        case JSON_NULL:
        buf = __strcat(buf,"null");
        break;

        case JSON_STRING:
        buf = __strcat(buf, "\"");
        buf = __strcat(buf,(char*)(_val->val.ptr));
        buf = __strcat(buf, "\"");
        break;

        case JSON_INTEGER:
        char ibuf[256];
        sprintf(ibuf,"%ld\0",_val->val.i64);
        buf = __strcat(buf,ibuf);
        break;

        case JSON_NUMBER:
        char fbuf[256];
        sprintf(fbuf,"%.6f\0",_val->val.f64);
        buf = __strcat(buf,fbuf);
        break;

        case JSON_OBJECT:
        buf = __strcat(buf, "{");
        if (_val->val.ptr!=0){
            buf = __strcat(buf,json_object_encode((json_object*)(_val->val.ptr)));
        }
        buf = __strcat(buf, "}");
        break;

        case JSON_ARRAY:
        buf = __strcat(buf, "[");
        if(_val->val.ptr!=0){
            buf = __strcat(buf,json_array_encode((json_array*)(_val->val.ptr)));
        }
        buf = __strcat(buf, "]");
        break;
    }
    return buf;
}

