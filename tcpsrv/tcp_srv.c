#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tcplib/tcp_lib.h"

int glb_skt_fd = -1;
void __attribute__((destructor)) __inexit() {
    fprintf(stdout, "[INFO ] clean socket\n");
    if (glb_skt_fd != -1) {
        close(glb_skt_fd);
    }
}

int main(int argc, char** argv) {

    int domain_safamily = AF_INET; // AF_INET:IPv4; AF_INET6:IPv6
    char *skt_ipv4_addr = "192.168.3.47";
    int skt_ipv4_port = 3054;
    int skt_max_conns = 2;
    int cli_buf_max_sz = 1<<8;

    // Create an `IPv4` socket
    int skt_fd = socket(domain_safamily, SOCK_STREAM, IPPROTO_TCP);
    if (skt_fd == -1) {
        ERRF(Creating TCP Socket);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Create Socket success, [server_fd:%d]\n", skt_fd);
    glb_skt_fd = skt_fd;

    // Assign socket an `IPv4:Port` Address
    struct sockaddr_in svr_addr;
    sktaddrin_set(svr_addr, domain_safamily, skt_ipv4_port, skt_ipv4_addr);
    int bind_err = bind(skt_fd, (struct sockaddr*)&svr_addr, sizeof(svr_addr));
    if (bind_err == -1) {
        ERRF(Binding TCP Socket Address);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Assign Socket Address success\n");

    // Listen to conns
    int lis_err = listen(skt_fd, skt_max_conns);
    if (bind_err == -1) {
        ERRF(Listening TCP Socket);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Listening Socket Conns in: %s:%d\n", skt_ipv4_addr, skt_ipv4_port);

    // accept_clients(skt_fd, cli_buf_max_sz); 1server-to-1client
    accept_clients_by_select(skt_fd, skt_max_conns, cli_buf_max_sz);

    if (close(skt_fd) == -1){
        ERRF(Closing TCP Socket);
        return 1;
    } else {
        fprintf(stdout, "[INFO ] server closed normalized, exit\n");
        return 0;
    }
}