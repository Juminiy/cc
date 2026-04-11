#include "../libctr/ctr_util.h"
#include "../libjson/json.h"

#include <unistd.h>

int main(){

    size_t bufsz = 512; char buf[bufsz];

    json_object *obj = make_json_object();
    json_object_put(obj, "cwd", new_json_value_str(getcwd(buf, bufsz)));
    json_object_put(obj, "uid", new_json_value_uint(getuid()));
    json_object_put(obj, "gid", new_json_value_uint(getgid()));
    json_object_put(obj, "login_username", new_json_value_str(getlogin()));
    json_object_put(obj, "euid", new_json_value_uint(geteuid()));
    json_object_put(obj, "egid", new_json_value_uint(getegid()));
    json_object_put(obj, "hostid", new_json_value_int(gethostid()));
    json_object_put(obj, "pagesize", new_json_value_int(getpagesize()));
    // json_object_put(obj, "tablesize", new_json_value_int(gettablesize()));
    
    json_value *val = new_json_value_obj(obj);
    char * infos = json_stringify_indent(val,2);

    puts(infos);

    free_json_value(val); free(infos);

    return 0;
}