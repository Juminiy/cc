#include "tcp_lib.h"
#include "skt_fdset.h"
#include <unistd.h>

int accept_clients(int srv_skt_fd, int buf_max_sz) {
    int cli_skt_fd = accept_client(srv_skt_fd);
    return read_client_infinite(
        cli_skt_fd, 
        (char*)malloc(sizeof(char)*buf_max_sz), 
        buf_max_sz
    );
}

// only receive 1 client
int accept_client(int srv_skt_fd) {
    // Accept Client TCP Connection
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_sz = sizeof(cli_addr);

    int cli_skt_fd = accept(srv_skt_fd, (struct sockaddr*)&cli_addr, &cli_addr_sz); 
    if (cli_skt_fd == -1) {
        ERRF(Accepting TCP Socket);
        return FD_INVALID;
    }
    fprintf(stdout, "[DEBUG] Received Socket Conn: [client_fd:%d]\n", cli_skt_fd); 
    fprintf(stdout, "[INFO ] Client[client_fd:%d], client_ip:%s, client_port:%d\n",
        cli_skt_fd, inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);  

    return cli_skt_fd;
}

int read_client_infinite(int cli_skt_fd, char *cli_buf, int buf_max_sz) {
    // Read From Client Data

    int cli_res = 0;
    do {
        cli_res = read_client(cli_skt_fd, cli_buf, buf_max_sz);
        switch (cli_res) {
            case TCP_CLIENT_ERR: {
                break;
            }
            case TCP_CLIENT_EXIT: {
                break;
            }
        }
    } while(cli_res == TCP_CLIENT_SEND);

    close(cli_skt_fd);
    return 0;
}

int read_client(int cli_skt_fd, char *cli_buf, int buf_max_sz) {
    ssize_t rd_sz = read(cli_skt_fd, cli_buf, buf_max_sz);
    if (rd_sz == -1) { // client <-> server error
        fprintf(stderr, "[ERROR] Read From Client[client_fd:%d]\n", cli_skt_fd);
        return TCP_CLIENT_ERR;
    }
    if (rd_sz == 0 || strcmp(cli_buf, TCP_CLIENT_EXIT_STR)==0) { // client say legal exit
        fprintf(stdout, "[INFO ] Client[client_fd:%d] closed normalized\n", cli_skt_fd);  
        return TCP_CLIENT_EXIT; 
    } else { // server legal received
        fprintf(stdout, "[INFO ] Read From Client[client_fd:%d], Msg: %s\n", cli_skt_fd, cli_buf);
        return TCP_CLIENT_SEND;
    }
}