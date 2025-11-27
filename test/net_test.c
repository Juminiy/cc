#include <stdio.h>

#include "../tcplib/net_util.h"

int main() {

    blist *bl = get_intf_ipv4();
    biter *bi = makeBIter(bl,BLIST_ITER_FORWARD);
    for(bnode *bn=bListNext(bi);bn;bn=bListNext(bi)){
        intfip *intfipval = bNodeData(bn).ptr;
        printf("interface name: %s, ip: %s\n", 
            intfipval->_name, intfipval->_ip);
    }

    return 0;
}