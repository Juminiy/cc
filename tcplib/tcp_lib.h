#ifndef TCP_LIB_H
#define TCP_LIB_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include "util.h"

#define sktaddrin_set(sktaddrin, sfamily, sport, saddr) \
    do { \
        memset(&sktaddrin, 0, sizeof(sktaddrin)); \
        sktaddrin.sin_family = sfamily; \
        sktaddrin.sin_port = htons(sport); \
        sktaddrin.sin_addr.s_addr = inet_addr(saddr); \
    } while(0)

int accept_clients(int, int);
int accept_client(int);
int read_client(int, char*, int);
int read_client_infinite(int, char*, int);
int accept_clients_by_select(
    int srv_skt_fd,
    int cli_max_sz, 
    int buf_max_sz
);

#define TCP_CLIENT_EXIT_STR "000"
#define TCP_CLIENT_EXIT 111
#define TCP_CLIENT_SEND 000
#define TCP_CLIENT_ERR  010

#endif