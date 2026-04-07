#include <regex.h>

#include "../libctr/ctr_util.h"

int main(){

    #define panice(dst_regt, src_str) \
        do { \
            int errcode = regcomp(&dst_regt, src_str, REG_EXTENDED|REG_NOTBOL|REG_NOTEOL); \
            if(errcode){ \
                char errmsg[256]; \
                regerror(errcode, &dst_regt, errmsg, 256); \
                PANICF("regex compile error: %s", errmsg); \
            } \
        } while(0)
    
    regex_t json_num_preg;
    regex_t json_str_preg;
    regex_t json_ws_preg;
    
    panice(json_num_preg, "^-?(0|[1-9][0-9]*)(.[0-9]+)?([eE][-+]?[0-9]+)?$");
    regmatch_t num_mtch;
    int mtch_res = regexec(&json_num_preg, "1.1e+3", 1, &num_mtch, 0);
    if(mtch_res==REG_NOMATCH){
        INFOF("num not match");
    }else {
        INFOF("matched: [%d,%d)", num_mtch.rm_so, num_mtch.rm_eo);
    }

    regfree(&json_num_preg);

    // panice(json_str_preg, "^\"([^\"\\\x00-\x1F]|\\([\"\\\\/bfnrt]|u[0-9A-Fa-f]{4}))*\"$");
    panice(json_ws_preg, "^[ \n\r\t]*$");
    regfree(&json_str_preg);
    regfree(&json_ws_preg);
    return 0;
}