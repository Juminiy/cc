#include "../libjson/json.h"

void output_valid(const char *_path) {
    char *buf = __readfile(_path);

    bool ok = json_valid(buf);

    printf("%s", ok?"y":"n");

    free(buf);
}

int main(int argc, char**argv){

    if (argc<2){
        putchar('?');
        return 0;
    }

    output_valid(argv[1]);

    return 0;
}