#include "../tcplib/util.h"

int main() {

    int i32val=114514;
    double f64val=1919.810;

    ERRF("hachimi");
    ERRF("intval:%d", -1);
    ERRF("intval:%d, strval:%s", 1, "rr");
    ERRF("intval:%d, strval:%s, charval:%c, ptrval:%p", 1, "rr", 49, &i32val);

    WARNF("hachimi");
    WARNF("intval:%d", -1);
    WARNF("intval:%d, strval:%s", 1, "rr");
    WARNF("intval:%d, strval:%s, charval:%c, ptrval:%p", 1, "rr", 49, &i32val);

    INFOF("hachimi");
    INFOF("intval:%d", -1);
    INFOF("intval:%d, strval:%s", 1, "rr");
    INFOF("intval:%d, strval:%s, charval:%c, ptrval:%p", 1, "rr", 49, &i32val);

    DEBUGF("hachimi");
    DEBUGF("intval:%d", -1);
    DEBUGF("intval:%d, strval:%s", 1, "rr");
    DEBUGF("intval:%d, strval:%s, charval:%c, ptrval:%p", 1, "rr", 49, &i32val);

    // FATALF("hachimi");
    // FATALF("intval:%d", -1);
    // FATALF("intval:%d, strval:%s", 1, "rr");
    FATALF("intval:%d, strval:%s, charval:%c, ptrval:%p", 1, "rr", 49, &i32val);

    return 0;
}