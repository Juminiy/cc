#include "../libjson/json.h"
#include <stdio.h>

void test_json_encode(){
    json_object *obj = make_json_object();
    json_object_put(obj,"age",new_json_value_int(11));
    json_object_put(obj,"name",new_json_value_str("hachimi"));
    json_object_put(obj,"height",new_json_value_num(180.22));
    json_object_put(obj,"norm",json_value_true);
    json_object_put(obj,"var",json_value_false);
    json_object_put(obj,"kilob",json_value_null);

    json_object *seek = make_json_object();
    json_object_put(seek,"key", new_json_value_int(1));
    json_object_put(seek, "vale", new_json_value_str("rrb"));
    json_object_put(obj,"seek",new_json_value_obj(seek));

    json_array *tese = make_json_array();
    json_array_append(tese, new_json_value_int(1));
    json_array_append(tese, new_json_value_int(13));
    json_array_append(tese, new_json_value_str("88"));
    json_array_append(tese, json_value_null_object);
    json_object_put(obj,"tese",new_json_value_arr(tese));

    json_value *val = new_json_value_obj(obj);
    char *_jsonstr = json_stringify(val);
    printf("%s\n",_jsonstr);
    free(_jsonstr);

    free_json_value(val);
}

void test_json_decode(const char *_path){
    char * buf = __readfile(_path);

    // printf("start parse\n");
    printf("%s\n\n", buf);
    json_value *value = json_parse(buf);
    printf("%s\n", json_stringify(value));

    free(buf);
}


int main(int argc, char **argv){

    // test_json_encode();

    if (argc<2){
        fprintf(stderr, "path: argv[1] is ""\n");
        return 0;
    }
    test_json_decode(argv[1]);
    
    // char * _s = "a\t\"";
    // ch_state stt = {
    //     ._raw=_s,
    //     .rcur=0,
    //     .rsiz=__strlen(_s),
    // };
    // printf("READ: %s\n", read_json_string(&stt));

    return 0;
}