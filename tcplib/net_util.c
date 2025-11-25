#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

// each interface IP
char **get_avaliable_inet4() {
    struct ifaddrs *addrs;
    char buf[128] ;
    getifaddrs(&addrs);
    for(struct ifaddrs * intf=addrs; intf != NULL; intf=intf->ifa_next) {
        if(intf->ifa_addr && (intf->ifa_flags & IFF_UP && intf->ifa_addr->sa_family==AF_INET)) {
            struct sockaddr_in *sa_in = intf->ifa_addr;
            inet_ntop(intf->ifa_addr->sa_family, &(sa_in->sin_addr), buf, strlen(buf));
        }
    }
    freeifaddrs(addrs);
    return NULL;
}

char **get_avaliable_inet6() {
    return NULL;
}