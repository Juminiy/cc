#ifndef NET_UTIL_H
#define NET_UTIL_H

#include "../ctrlib/ctr_blist.h"

typedef struct intfip {
    char _ip[135]; // size = IPv4:12+3, IPv6:128+7
    char *_name;   // interface name
} intfip;

blist* get_intf_ipv4();

blist* get_intf_ipv6();

#endif 