#include "../../libjson/json.h"

void output_valid(const char *_path) {
    roSBuf buf = readFileAll(_path);

    bool ok = json_nvalid(roSBufStr(buf), roSBufSiz(buf));

    printf("%s", ok?"y":"n");

    freeROSBuf(buf);
}

int main(int argc, char**argv){

    if (argc<2){
        putchar('?');
        return 0;
    }

    output_valid(argv[1]);

    return 0;
}