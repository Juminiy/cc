#include "util.h"

void str_trunc(char *_str, int _len) {
    size_t _sz = strlen(_str);
    if(_sz>_len){
        _str[_len]='\0';
    }
}