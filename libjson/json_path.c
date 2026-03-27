#include "json.h"

json_value* json_get_value(json_value *_val, char *_path[], size_t _n) {
    for(size_t i=0;i<_n;i++){
        if(_val==NULL){
            return NULL;
        }
        if(_val->typ==JSON_OBJECT){
            _val = json_object_get((json_object*)(_val->val.ptr),_path[i]);
        } else if (_val->typ==JSON_ARRAY){
            int idx = strtol(_path[i],NULL,10);
            if(idx>=0){
                _val = json_array_get((json_array*)(_val->val.ptr),idx);
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    }
    return _val;
}

// allow typ: val
// JSON_TRUE
// JSON_INTEGER: 1
// JSON_INTEGER_UINT: 1
// JSON_STRING: true,True,1
bool json_get_true(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return false;
    }
    bool i_ok = (_val->typ==JSON_TRUE)||
        (_val->typ==JSON_INTEGER&&_val->val.i64==1)||
        (_val->typ==JSON_INTEGER_UINT&&_val->val.u64==1);
    
    bool str_ok = false;
    if(_val->typ==JSON_STRING){
        char *_s = (char*)(_val->val.ptr);
        str_ok = __strcmp("1",_s)||__strcmp("true",_s)||__strcmp("True",_s);
    }

    return i_ok || str_ok;
}

// allow typ: val
// JSON_FALSE
// JSON_INTEGER: 0
// JSON_INTEGER_UINT: 0
// JSON_STRING: false,False,0
bool json_get_false(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return false;
    }
    bool i_ok = (_val->typ==JSON_FALSE)||
        (_val->typ==JSON_INTEGER&&_val->val.i64==0)||
        (_val->typ==JSON_INTEGER_UINT&&_val->val.u64==0);
    
    bool str_ok = false;
    if(_val->typ==JSON_STRING){
        char *_s = (char*)(_val->val.ptr);
        str_ok = __strcmp("0",_s)||__strcmp("false",_s)||__strcmp("False",_s);
    }

    return i_ok || str_ok;
}

bool json_get_null(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return false;
    }
    return _val->typ==JSON_NULL;
}

// allow typ
// 
char* json_get_str(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return NULL;
    }
    
    char stkbuf[1000];
    switch(_val->typ){
        case JSON_TRUE:
            return __strdup("true");
        case JSON_FALSE:
            return __strdup("false");
        case JSON_NULL:
            return __strdup("null");
        case JSON_STRING:
            return __strdup((char*)_val->val.ptr);
        case JSON_INTEGER:
            sprintf(stkbuf,"%ld",_val->val.i64);
            return __strdup(stkbuf);
        case JSON_INTEGER_UINT:
            sprintf(stkbuf,"%lu",_val->val.u64);
            return __strdup(stkbuf);
        case JSON_NUMBER:
            sprintf(stkbuf,"%lf",_val->val.f64);
            return __strdup(stkbuf);
        default:
            return NULL;
    }
    
}

int64_t json_get_int(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return 0;
    }
    if(_val->typ==JSON_INTEGER){
        return _val->val.i64;
    } else if(_val->typ==JSON_INTEGER_UINT&&
        (double)_val->val.u64<=JSON_INT_MAX){
        return _val->val.u64;
    }
    
    return 0;
}

uint64_t json_get_uint(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return 0;
    }
    if(_val->typ==JSON_INTEGER_UINT){
        return _val->val.u64;
    } else if(_val->typ==JSON_INTEGER&&_val->val.i64>=0){
        return _val->val.i64;
    }
    
    return 0;
}

double json_get_num(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val==NULL){
        return 0;
    }
    if(_val->typ==JSON_NUMBER) {
        return _val->val.f64;
    } else if (_val->typ==JSON_INTEGER){
        return (double)_val->val.i64;
    } else if (_val->typ==JSON_INTEGER_UINT){
        return (double)_val->val.u64;
    }

    return 0;

}

json_object* json_get_object(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val&&_val->typ==JSON_OBJECT){
        return (json_object*)(_val->val.ptr);
    }
    return NULL;
}

json_array* json_get_array(json_value *_val, char *_path[], size_t _n) {
    _val = json_get_value(_val, _path, _n);
    if(_val&&_val->typ==JSON_ARRAY){
        return (json_array*)(_val->val.ptr);
    }
    return NULL;
}