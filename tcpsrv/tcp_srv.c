#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <malloc.h>
#include <stdlib.h>

#include <sys/select.h>
#include <sys/time.h>

#include "../tcplib/tcp_lib.h"
#include "../tcplib/tcp_srv.h"

#define ERRF(msg) \
    do { fprintf(stderr, "[Error] "#msg", error: %s\n", strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define INFOF(msg) \
    do { fprintf(stdout, "[INFO ] "#msg"\n"); } while(0)

#define DEBUGF(msg) \
    do { fprintf(stdout, "[Debug] "#msg"\n"); } while(0)


int main(int argc, char** argv) {

    int domain_safamily = AF_INET; // AF_INET:IPv4; AF_INET6:IPv6
    char *skt_ipv4_addr = "192.168.3.47";
    int skt_ipv4_port = 3054;
    int skt_max_conns = 3;
    int cli_buf_max_sz = 1<<11;

    // Create an `IPv4` socket
    int skt_fd = socket(domain_safamily, SOCK_STREAM, IPPROTO_TCP);
    if (skt_fd == -1) {
        ERRF(Creating TCP Socket);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Create Socket success, [server_fd:%d]\n", skt_fd);
    
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

    accept_clients(skt_fd, cli_buf_max_sz);

    if (close(skt_fd) == -1){
        ERRF(Closing TCP Socket);
        return 1;
    }

    return 0;
}

int accept_clients(int srv_skt_fd, int buf_max_sz) {
    return accept_client(srv_skt_fd, buf_max_sz);
}

int accept_clients_by_select(
    int srv_skt_fd,
    int max_cli_sz, 
    int buf_max_sz
) {

    int cur_cli_sz = 0;
    int *rd_fd = (int*)malloc(sizeof(int)*max_cli_sz); 

    for(;;) {
        // accept to max
        while (cur_cli_sz < max_cli_sz) {
            struct sockaddr_in cli_addr;
            socklen_t peer_addr_sz = sizeof(cli_addr);

            int cli_fd = accept(srv_skt_fd, (struct sockaddr*)&cli_addr, &peer_addr_sz); 
            if (cli_fd == -1) {
                ERRF(Accepting TCP Socket);
                return 1;
            }
            fprintf(stdout, "[DEBUG] Received Socket Conn: [client_fd:%d]\n", cli_fd); 
            fprintf(stdout, "[INFO ] Client[client_fd:%d], client_ip:%s, client_port:%d\n",
                cli_fd, inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port); 
        }

        // read to by queue
        // fd_set monitor_rd_fd = 
    }

    

    return 0;
}

// only receive 1 client
int accept_client(int srv_skt_fd, int buf_max_sz) {
    // Accept Client TCP Connection
    struct sockaddr_in cli_addr;
    socklen_t peer_addr_sz = sizeof(cli_addr);

    int cli_fd = accept(srv_skt_fd, (struct sockaddr*)&cli_addr, &peer_addr_sz); 
    if (cli_fd == -1) {
        ERRF(Accepting TCP Socket);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Received Socket Conn: [client_fd:%d]\n", cli_fd); 

    // fprintf Client Info
    fprintf(stdout, "[INFO ] Client[client_fd:%d], client_ip:%s, client_port:%d\n",
        cli_fd, inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);  

    // Read From Client Data
    char *cli_buf = (char*)malloc(sizeof(char)*buf_max_sz);
    for(ssize_t rd_sz = read(cli_fd, cli_buf, buf_max_sz); // read operator is a `BlockingIO`
        rd_sz != 0; // Client EOF
        rd_sz = read(cli_fd, cli_buf, buf_max_sz)
    ) {
        if (rd_sz == -1) { // client <-> server error
            fprintf(stderr, "[ERROR] Read From Client[client_fd:%d]\n", cli_fd);
            break;
        }
        if (strcmp(cli_buf, "000\0")==0) { // client say legal exit
            fprintf(stdout, "[INFO ] Read From Client[client_fd:%d], Says Bye\n", cli_fd);   
            close(cli_fd);
            break;
        } else { // server legal received
            fprintf(stdout, "[INFO ] Read From Client[client_fd:%d], Msg: %s\n", cli_fd, cli_buf);
        }
    }

    return 0;
}

// each interface IP
char **get_avaliable_inet4() {
    
}

char **get_avaliable_inet6() {

}