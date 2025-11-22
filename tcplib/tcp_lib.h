#ifndef TCP_LIBH
#define TCP_LIBH

#include <arpa/inet.h>
#include <netinet/in.h>

#define sktaddrin_set(sktaddrin, sfamily, sport, saddr) \
    do { \
        memset(&sktaddrin, 0, sizeof(sktaddrin)); \
        sktaddrin.sin_family = sfamily; \
        sktaddrin.sin_port = htons(sport); \
        sktaddrin.sin_addr.s_addr = inet_addr(saddr); \
    } while(0)

    
#endif