#include "json.h"
#include "../libctr/ctr_sbuf.h"

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
        if(_val->val.ptr!=0){
            sBufWriteFmt(buf,"\"%s\"",(char*)(_val->val.ptr));
        }
        break;

        case JSON_INTEGER:
        sBufWriteFmt(buf,"%ld",_val->val.i64);
        break;

        case JSON_INTEGER_UINT:
        sBufWriteFmt(buf,"%lu",_val->val.u64);
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

typedef struct json_encode_config {
    size_t _indent;
} json_encode_config;
#define JSON_INDENT_MAX 16

void json_object_encode_indent(json_object *_obj, sbuf *buf, size_t _c, size_t _x);
void json_array_encode_indent(json_array *_arr, sbuf *buf, size_t _c, size_t _x);
void json_value_encode_indent(const json_value *_val, sbuf *buf, size_t _c, size_t _x);
#define sbuf_writenspace(__buf,__n) \
    do { \ 
        for(int i=0;i<__n;i++) { sBufWriteChar(__buf,' '); } \
    } while(0)
#define sbuf_writelf(__buf) \
    do { sBufWriteChar(__buf,'\n'); } while(0)

void json_object_encode_indent(json_object *_obj, sbuf *buf, size_t _c, size_t _x) {
    int cnt = 0, siz = json_object_size(_obj);
    bool encode_obj_fn(char *_name, json_value *_value) {
        cnt++;
        sbuf_writenspace(buf,_c);
        sBufWriteFmt(buf,"\"%s\":",_name);
        if (_value->typ == JSON_OBJECT || _value->typ == JSON_ARRAY) {
            // 对象或数组值：冒号后空格，换行，然后缩进编码
            sBufWriteChar(buf, ' ');
            sbuf_writelf(buf);
            json_value_encode_indent(_value, buf, _c + _x, _x);
        } else {
            // 简单值：冒号后空格，直接编码（不添加前导空格）
            sBufWriteChar(buf, ' ');
            json_value_encode_indent(_value, buf, 0, _x);
        }
        if (cnt<siz) {
            sBufWriteChar(buf,',');
            sbuf_writelf(buf);
        }
        return true;
    };
    json_object_iter(_obj, encode_obj_fn);
}

void json_array_encode_indent(json_array *_arr, sbuf *buf, size_t _c, size_t _x) {
    int cnt = 0, siz = json_array_size(_arr);
    bool encode_arr_fn(size_t _idx, elem_t _dt) {
        cnt++;
        sbuf_writenspace(buf,_c);
        json_value *val = cast_elem_typ(_dt, json_value*);
        json_value_encode_indent(val,buf,1,_x);
        if (cnt<siz) {
            sBufWriteChar(buf,',');
            sbuf_writelf(buf);
        }
        return true;
    };
    bArrayIter(_arr->_arr, encode_arr_fn);
}

void json_value_encode_indent(const json_value *_val, sbuf *buf, size_t _c, size_t _x) {
    switch (_val->typ) {
        case JSON_TRUE:
        sbuf_writenspace(buf, _c);
        sBufWrite(buf,"true");
        break;

        case JSON_FALSE:
        sbuf_writenspace(buf, _c);
        sBufWrite(buf,"false");
        break;

        case JSON_NULL:
        sbuf_writenspace(buf, _c);
        sBufWrite(buf,"null");
        break;

        case JSON_STRING:
        if(_val->val.ptr!=0){
            sbuf_writenspace(buf, _c);
            sBufWriteFmt(buf,"\"%s\"",(char*)(_val->val.ptr));
        }
        break;

        case JSON_INTEGER:
        sbuf_writenspace(buf, _c);
        sBufWriteFmt(buf,"%ld",_val->val.i64);
        break;

        case JSON_INTEGER_UINT:
        sbuf_writenspace(buf, _c);
        sBufWriteFmt(buf,"%lu",_val->val.u64);
        break;

        case JSON_NUMBER:
        sbuf_writenspace(buf, _c);
        sBufWriteFmt(buf,"%.6f",_val->val.f64);
        break;

        case JSON_OBJECT:
        sbuf_writenspace(buf,_c>0?1:0);
        sBufWriteChar(buf,'{');
        sbuf_writelf(buf);
        if (_val->val.ptr!=0){
            json_object_encode_indent((json_object*)(_val->val.ptr),buf,_c+_x,_x);
        }
        sbuf_writelf(buf);
        sbuf_writenspace(buf, _c);
        sBufWriteChar(buf,'}');
        break;

        case JSON_ARRAY:
        sbuf_writenspace(buf,_c>0?1:0);
        sBufWriteChar(buf,'[');
        sbuf_writelf(buf);
        if(_val->val.ptr!=0){
            json_array_encode_indent((json_array*)(_val->val.ptr),buf,_c+_x,_x);
        }
        sbuf_writelf(buf);
        sbuf_writenspace(buf, _c);
        sBufWriteChar(buf,']');
        break;
    }
}

char* json_stringify_indent(const json_value *_val, size_t _x) {
    sbuf *buf = makeSBuf(0);
    if(_x>JSON_INDENT_MAX){
        _x = JSON_INDENT_MAX;
    }
    json_value_encode_indent(_val, buf, 0, _x);
    char *buf_str = sBufStr(buf);
    free(buf);
    return buf_str;
}