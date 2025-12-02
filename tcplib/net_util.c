#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

#include "util.h"
#include "net_util.h"

#define add_intfip_list(addr_blist, intf) \
    do { \
        struct sockaddr_in *sa_in = intf->ifa_addr; \
        intfip *intfipaddr = (intfip*)malloc(sizeof(intfip)); \
        strcpy(intfipaddr->_ip, inet_ntoa(sa_in->sin_addr)); \
        intfipaddr->_name = intf->ifa_name; \
        elem_t elem_addr; \
        setup_elem_ptr(elem_addr, intfipaddr); \
        bListAddTail(addr_blist, elem_addr); \
    } while(0)

// @attention returned must be freed
// each interface IPv4
blist* get_intf_ipv4() {
    struct ifaddrs *addrs;
    blist *addrlist = makeBList();
    if(getifaddrs(&addrs)==-1) {
        ERRF("get interface");
        return NULL;
    }
    for(struct ifaddrs * intf=addrs; intf != NULL; intf=intf->ifa_next) {
        if(intf->ifa_addr && 
            intf->ifa_flags & IFF_UP &&  
            intf->ifa_addr->sa_family==AF_INET && 
            !(intf->ifa_flags & IFF_LOOPBACK)
        ) {
            add_intfip_list(addrlist, intf);
            // inet_ntop(intf->ifa_addr->sa_family, &(sa_in->sin_addr), buf, strlen(buf));
        }
    }
    freeifaddrs(addrs);
    return addrlist;
}

// @attention returned must be freed
// each interface IPv6
blist* get_intf_ipv6() {
    struct ifaddrs *addrs;
    blist *addrlist = makeBList();
    if(getifaddrs(&addrs)==-1) {
        ERRF("get interface");
        return NULL;
    }
    for(struct ifaddrs * intf=addrs; intf != NULL; intf=intf->ifa_next) {
        if(intf->ifa_addr && 
            intf->ifa_flags & IFF_UP &&  
            intf->ifa_addr->sa_family==AF_INET6 && 
            !(intf->ifa_flags & IFF_LOOPBACK)
        ) {
            add_intfip_list(addrlist, intf);
            // inet_ntop(intf->ifa_addr->sa_family, &(sa_in->sin_addr), buf, strlen(buf));
        }
    }
    freeifaddrs(addrs);
    return addrlist;
}