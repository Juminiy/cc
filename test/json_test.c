#include "../libjson/json.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../ctrlib/ctr_sbuf.h"

#define print_time_ms(__block, __desc) \
    do { \
        clock_t st_ts=clock(); \
        __block \
        clock_t ed_ts=clock(); \
        printf("%s: %.4fms\n", __desc, (double)(ed_ts-st_ts)/(CLOCKS_PER_SEC)*1000); \
    }while(0)

void opt_json_value(json_value* value, 
    bool _encode, bool _print, bool _free_value 
) {
    if (_encode){
        char *_s = NULL;
        print_time_ms(
            _s = json_stringify(value); ,
            "json encode time"
        );
        if(_print){
            printf("%s\n", _s);
        }
        free(_s);
    }
    if(_free_value){
        free_json_value(value);
    }
}

void test_json_encode(){
    json_object *obj = make_json_object();
    json_object_put(obj,"age",new_json_value_int(11));
    json_object_put(obj,"name",new_json_value_str("和意为"));
    json_object_put(obj,"height",new_json_value_num(180.22));
    json_object_put(obj,"norm",json_value_true);
    json_object_put(obj,"var",json_value_false);
    json_object_put(obj,"kilob",json_value_null);

    json_object *seek = make_json_object();
    json_object_put(seek,"key", new_json_value_int(1));
    json_object_put(seek, "vale", new_json_value_str("rrb"));
    json_object_put(obj,"seek",new_json_value_obj(seek));

    json_array *tese = make_json_array();
    json_array_append(tese, new_json_value_num(0.1));
    json_array_append(tese, new_json_value_int(13));
    json_array_append(tese, new_json_value_str("88"));
    json_array_append(tese, json_value_null_object);
    json_object_put(obj,"tese",new_json_value_arr(tese));

    json_value *val = new_json_value_obj(obj);
    opt_json_value(val,true,true,true);
}

void test_json_decode(const char *_path){
    char * buf = __readfile(_path);
    
    printf("bssize = %ldB\n", __strlen(buf));

    if (buf!=NULL){
        json_value *val;
        print_time_ms(
            val = json_parse(buf);,
            "json decode time"
        );

        opt_json_value(val,true,false,true);
    } else {
        printf("path:%s no buffer\n",_path);
    }
    
    free(buf);
}

void output_valid(const char *_path) {
    char *buf = __readfile(_path);

    bool ok = json_valid(buf);

    printf("%s", ok?"y":"n");

    free(buf);
}

void inspect_char(const char *_path){
    char *buf = __readfile(_path);

    printf("%s\n", buf);

    for(int i =0;i<__strlen(buf);i++){
        printf("%c\n",buf[i]);
    }

    free(buf);
}

char* json_value_strfmt(json_value *val) {
    sbuf *bf = makeSBuf(100);
    switch(val->typ) {
        case JSON_NONE:
        sBufWrite(bf,"NONE");
        break;

        case JSON_TRUE:
        sBufWrite(bf,"true");
        break;

        case JSON_FALSE:
        sBufWrite(bf,"false");
        break;

        case JSON_NULL:
        sBufWrite(bf,"null");
        break;

        case JSON_STRING:
        sBufWriteFmt(bf,"\"%s\"",(char*)val->val.ptr);
        break;

        case JSON_INTEGER:
        sBufWriteFmt(bf,"%ld",val->val.i64);
        break;

        case JSON_INTEGER_UINT:
        sBufWriteFmt(bf,"%lu",val->val.u64);
        break;

        case JSON_NUMBER:
        sBufWriteFmt(bf,"%.6f",val->val.f64);
        break;

        case JSON_OBJECT:
        sBufWriteFmt(bf,"OBJECT<%p>",val->val.ptr);
        break;

        case JSON_ARRAY:
        sBufWriteFmt(bf,"ARRAY<%p>",val->val.ptr);
        break;

    }

    return sBufStr(bf);
}

#define COLOR_RED "\033[0;31"
#define COLOR_GREEN "\033[0;32"
#define COLOR_CLEAR "\033[0m"

// buf="$number y"
// buf="$number n"
void test_parse_num_line(char *buf) {
    char *_sptr = strrchr(buf, ' ');
    size_t _off = _sptr-buf;
    char *_ln = __substr(buf,0,_off);
    json_value *val = new_json_value();
    parse_json_number(_ln,val);
    if((buf[_off+1]=='n'&&val->typ==JSON_NONE)||  // should fail, parse fail
        (buf[_off+1]=='y'&&val->typ!=JSON_NONE)){ // should sucs, parse sucs
        printf("OK: src[%s] dst[%s]\n",
            _ln,json_value_strfmt(val));
    } else {
        printf("ERROR: src[%s] dst[%s]\n",
            _ln,json_value_strfmt(val));
    }
    json_value_strfmt(val);
    free_json_value(val);
}

void test_parse_num1(){
    FILE *fd = fopen("data/json/num_test_parsing.txt", "r");
    if(fd==NULL){return;}
    char buf[256];
    while(fgets(buf, 256, fd)){
        test_parse_num_line(buf);
    }
}

void test_parse_num2() {
    char *buf[] = {
        "1 000.0 n",
    };
    for(int i=0;i<1;i++){
        test_parse_num_line(buf[i]);
    }
}

// buf="$string y"
// buf="$string n"
void test_parse_str_line(char *buf) {
    char *_sptr = strrchr(buf, ' ');
    if (_sptr==NULL){
        // printf("between \" and \" has a '\\000'\n");
        return;
    }
    size_t _off = _sptr-buf;
    char *_ln = __substr(buf,0,_off);
    json_value *val = new_json_value();
    parse_json_string(_ln,val);
    if((buf[_off+1]=='n'&&val->typ==JSON_NONE)||  // should fail, parse fail
        (buf[_off+1]=='y'&&val->typ!=JSON_NONE)){ // should sucs, parse sucs
        // printf("OK: src[%s] dst[%s]\n",
        //     _ln,json_value_strfmt(val));
    } else {
        printf("ERROR: src[%s] dst[%s]\n",
            _ln,json_value_strfmt(val));
    }
    json_value_strfmt(val);
    free_json_value(val);
}

void test_parse_str1(){
    FILE *fd = fopen("data/json/str_test_parsing.txt", "r");
    if(fd==NULL){return;}
    char buf[256];
    while(fgets(buf, 256, fd)){
        // printf("RUN: %s\n", buf);
        test_parse_str_line(buf);
    }
}

/*
"\
"a
"a
*/
void test_parse_str2() {
    char *buf[] = {
    };
    for(int i=0;i<1;i++){
        test_parse_num_line(buf[i]);
    }
}

int main(int argc, char **argv){
    if (argc<2){
        fprintf(stderr, "path: argv[1] not found\n");
        return 0;
    }
    // test_json_decode(argv[1]);

    output_valid(argv[1]);

    return 0;
}