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

void fd_nonblocking(int fd);
int accept_client(int srv_skt_fd);
int read_client(int cli_skt_fd, char *cli_buf, int buf_max_sz);

typedef int(*server_accept_f)(int, int, int);

int accept_clients_blocking(
    int srv_skt_fd,
    int __attribute__((unused)) cli_max_sz, // assured to 1
    int buf_max_sz
);
int accept_clients_by_select(
    int srv_skt_fd,
    int cli_max_sz, 
    int buf_max_sz
);
int accept_clients_by_epoll(
    int srv_skt_fd,
    int cli_max_sz,
    int buf_max_sz
);

// Server Code
#define TCP_SRV_RD_OK  (1<<0)
#define TCP_SRV_RD_ERR (1<<1)
#define TCP_SRV_WR_OK  (1<<2)
#define TCP_SRV_WR_ERR (1<<3)

// Client Code
#define TCP_CLI_RD_OK  (1<<4)
#define TCP_CLI_RD_ERR (1<<5)
#define TCP_CLI_WR_OK  (1<<6)
#define TCP_CLI_WR_ERR (1<<7)
#define TCP_CLI_EOF    (1<<8)
#define TCP_CLI_EXIT   (1<<9)

#define TCP_CLI_EXIT_MSG "(!q)"

#define SRV_MODE_BLOCKING 0
#define SRV_MODE_SELECT 1
#define SRV_MODE_EPOLL 2

#endif