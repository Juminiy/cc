#include "../json.h"
#include "../../libctr/ctr_barray.h"
#include <stdlib.h>
#include <string.h>

#define json_object_pair_setup(_pr, _name, _value) \
    do { _pr.name = _name; _pr.value = _value; } while(0)

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

void free_json_object_pair(json_object_pair *_pr) {
    free(_pr->name);
    free_json_value(_pr->value);
    free(_pr);
}

json_object* make_json_object() {
    MALLOC_TYPE(json_object, _obj);
    _obj->_underlying_type = JSON_OBJECT_BARRAY;
    _obj->_tr = NULL;
    _obj->_arr = makeBArray(0, 0);
    return _obj;
}

void free_json_object(json_object *_obj) {
    for (size_t i = 0; i < bArrayLen(_obj->_arr); i++) {
        elem_t _em = bArrayAt(_obj->_arr, i);
        if (valid_elem_t(_em)) {
            json_object_pair *_pr = cast_elem_typ(_em, json_object_pair*);
            free_json_object_pair(_pr);
        }
    }
    freeBArray(_obj->_arr);
    free(_obj);
}

void json_object_put(json_object *_obj, const char *_name, json_value *_value) {
    json_object_pair *_pr = make_json_object_pair(_name, _value);
    elem_t _em;
    setup_elem_ptr(_em, _pr);
    _obj->_arr = bArrayAppend(_obj->_arr, _em);
}

void json_object_put_shallow_name(json_object *_obj, char *_name, json_value *_value) {
    json_object_pair *_pr = make_json_object_pair_shallow_name(_name, _value);
    elem_t _em;
    setup_elem_ptr(_em, _pr);
    _obj->_arr = bArrayAppend(_obj->_arr, _em);
}

void json_object_del(json_object *_obj, char *_name) {
    for (size_t i = 0; i < bArrayLen(_obj->_arr); i++) {
        elem_t _em = bArrayAt(_obj->_arr, i);
        if (valid_elem_t(_em)) {
            json_object_pair *_pr = cast_elem_typ(_em, json_object_pair*);
            if (__strcmp(_pr->name, _name) == 0) {
                free_json_object_pair(_pr);
                _obj->_arr = bArrayDeleteIndex(_obj->_arr, i);
                return;
            }
        }
    }
}

json_value* json_object_get(json_object *_obj, char *_name) {
    for (size_t i = 0; i < bArrayLen(_obj->_arr); i++) {
        elem_t _em = bArrayAt(_obj->_arr, i);
        if (valid_elem_t(_em)) {
            json_object_pair *_pr = cast_elem_typ(_em, json_object_pair*);
            if (__strcmp(_pr->name, _name) == 0) {
                return _pr->value;
            }
        }
    }
    return NULL;
}

void json_object_iter(json_object *_obj, json_object_iter_func _fn) {
    for (size_t i = 0; i < bArrayLen(_obj->_arr); i++) {
        elem_t _em = bArrayAt(_obj->_arr, i);
        if (valid_elem_t(_em)) {
            json_object_pair *_pr = cast_elem_typ(_em, json_object_pair*);
            bool _ret = _fn(_pr->name, _pr->value);
            if (!_ret) break;
        }
    }
}

int json_object_size(const json_object *_obj) {
    return (int)bArrayLen(_obj->_arr);
}
