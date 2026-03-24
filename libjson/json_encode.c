#include "json.h"
#include "../ctrlib/ctr_sbuf.h"

/*
 * JSON Encode API
 */
void json_object_encode(json_object *_obj, sbuf *buf);
void json_array_encode(json_array *_arr, sbuf *buf);
void json_value_encode(const json_value *_val, sbuf *buf);

void json_object_encode(json_object *_obj, sbuf *buf) {
    int cnt = 0, siz = json_object_size(_obj);
    bool encode_obj_fn(char *_name, json_value *_value) {
        cnt++;
        sBufWriteFmt(buf,"\"%s\":",_name);
        json_value_encode(_value,buf);
        if (cnt<siz) {
            sBufWriteChar(buf,',');
        }
        return true;
    };
    json_object_iter(_obj, encode_obj_fn);
}

void json_array_encode(json_array *_arr, sbuf *buf) {
    int cnt = 0, siz = json_array_size(_arr);
    bool encode_arr_fn(size_t _idx, elem_t _dt) {
        cnt++;
        json_value *val = cast_elem_typ(_dt, json_value*);
        json_value_encode(val,buf);
        if (cnt<siz) {
            sBufWriteChar(buf,',');
        }
        return true;
    };
    bArrayIter(_arr->_arr, encode_arr_fn);
}

void json_value_encode(const json_value *_val, sbuf *buf) {
    switch (_val->typ) {
        case JSON_TRUE:
        sBufWrite(buf,"true");
        break;

        case JSON_FALSE:
        sBufWrite(buf,"false");
        break;

        case JSON_NULL:
        sBufWrite(buf,"null");
        break;

        case JSON_STRING:
        sBufWriteFmt(buf,"\"%s\"",(char*)(_val->val.ptr));
        break;

        case JSON_INTEGER:
        sBufWriteFmt(buf,"%ld",_val->val.i64);
        break;

        case JSON_NUMBER:
        sBufWriteFmt(buf,"%.6f",_val->val.f64);
        break;

        case JSON_OBJECT:
        sBufWriteChar(buf,'{');
        if (_val->val.ptr!=0){
            json_object_encode((json_object*)(_val->val.ptr),buf);
        }
        sBufWriteChar(buf,'}');
        break;

        case JSON_ARRAY:
        sBufWriteChar(buf,'[');
        if(_val->val.ptr!=0){
            json_array_encode((json_array*)(_val->val.ptr),buf);
        }
        sBufWriteChar(buf,']');
        break;
    }
}

char* json_stringify(const json_value *_val) {
    sbuf *buf = makeSBuf(0);
    json_value_encode(_val, buf);
    char *buf_str = sBufStr(buf);
    free(buf);
    return buf_str;
}