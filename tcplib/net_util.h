#ifndef NET_UTIL_H
#define NET_UTIL_H

#include "../ctrlib/ctr_blist.h"

typedef struct intfip {
    char _ip[135]; // size = IPv4:12+3, IPv6:128+7
    char *_name;   // interface name
} intfip;

blist* get_intf_ipv4();

blist* get_intf_ipv6();

#define SHOW_INTFIP(__printf) \
    do { \
        blist *bl = get_intf_ipv4(); \
        biter *bi = makeBIter(bl,BLIST_ITER_FORWARD); \
        for(bnode *bn=bListNext(bi);bn;bn=bListNext(bi)){ \
            intfip *intfipval = bNodeData(bn).ptr; \
            __printf("interface name: %s, ip: %s\n",  \
                intfipval->_name, intfipval->_ip); \
        } \
        freeBIter(bi); \
        freeBList(bl); \
    }while(0)

#endif 