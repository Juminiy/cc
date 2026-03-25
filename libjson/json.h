#ifndef LIB_JSON_H
#define LIB_JSON_H

#include <stdint.h>
#include "../ctrlib/ctr_rbtree.h"
#include "../ctrlib/ctr_barray.h"
#include "../ctrlib/ctr_util.h"
#include "../ctrlib/ctr_elemt.h"

// JSON TYPE
#define JSON_TRUE    (int64_t)(1<<0)
#define JSON_FALSE   (int64_t)(1<<1)
#define JSON_NULL    (int64_t)(1<<2)
#define JSON_STRING  (int64_t)(1<<3)
#define JSON_INTEGER (int64_t)(1<<4)
#define JSON_INTEGER_SINT JSON_INTEGER
#define JSON_INTEGER_UINT (int64_t)((1<<4)|(1<<1))
#define JSON_NUMBER  (int64_t)(1<<5)
#define JSON_OBJECT  (int64_t)(1<<6)
#define JSON_ARRAY   (int64_t)(1<<7)

// JSON NUM
#define JSON_INT_MAX  (double)INT64_MAX
#define JSON_INT_MIN  (double)INT64_MIN
#define JSON_UINT_MAX (double)UINT64_MAX
#define JSON_NUM_MAX_SIZE (size_t)10000

typedef struct json_value {
    int64_t  typ;
    elem_uni val;
} json_value;

typedef struct json_object {
    rb_tree *_tr; // <json_object_pair*>
} json_object;
typedef struct json_object_pair {
    char        *name;
    json_value *value;
} json_object_pair;

typedef struct json_array {
    barray _arr; // <json_value*>
} json_array;

// JSON VALUE API
extern json_value* const json_value_true;        // true
extern json_value* const json_value_false;       // false
extern json_value* const json_value_null;        // null
extern json_value* const json_value_null_object; // {}
extern json_value* const json_value_null_array;  // []
json_value* new_json_value_str(const char *_str);
json_value *new_json_value_str_shallow(char *_str);
json_value* new_json_value_int(const int64_t _int);
json_value *new_json_value_uint(const uint64_t _uint);
json_value* new_json_value_num(const double _num);
json_value* new_json_value_obj(json_object* _obj);
json_value* new_json_value_arr(json_array* _arr);
void free_json_value(json_value* _val);
json_value* new_json_value();

// JSON OBJECT API
json_object* make_json_object();
void free_json_object(json_object *_obj);
void json_object_put(json_object *_obj, const char *_name, json_value *_value);
void json_object_put_shallow_name(json_object *_obj, char *_name, json_value *_value);
void json_object_del(json_object *_obj, char *_name);
json_value* json_object_get(json_object *_obj, char *_name);
typedef bool(*json_object_iter_func)(char *_name, json_value *_value);
void json_object_iter(json_object *_obj,json_object_iter_func _fn);
int json_object_size(const json_object *_obj);

// JSON ARRAY API
json_array* make_json_array();
void json_array_append(json_array *_arr, json_value *_val);
void free_json_array(json_array *_arr);
int json_array_size(const json_array *_arr);

// JSON API
bool json_valid(const char * _str);
// javasript_like API
json_value* json_parse(const char *_str);
char* json_stringify(const json_value *_val);
// python_like API
void* json_loads(const char *_str);
char* json_dumps(void *_obj);
// golang_like API
char* json_unmarshal(const char *_str, void *_elem);
char* json_marshal(void *_elem);

#endif