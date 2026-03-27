#include "tcp_lib.h"
#include "skt_fdset.h"

#include <unistd.h>
#include <fcntl.h>

int accept_clients_blocking(
    int srv_skt_fd,
    int __attribute__((unused)) cli_max_sz, // assured to 1
    int buf_max_sz
) {
    int cli_skt_fd = accept_client(srv_skt_fd);
    char* cli_buf = (char*)malloc(sizeof(char)*buf_max_sz);

    int cli_res = 0;
    do {
        cli_res = read_client(cli_skt_fd, cli_buf, buf_max_sz);
    } while(cli_res == TCP_SRV_RD_OK);

    free(cli_buf);
    close(cli_skt_fd);
    return cli_res;
}

// only receive 1 client
int accept_client(int srv_skt_fd) {
    // Accept Client TCP Connection
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_sz = sizeof(cli_addr);

    int cli_skt_fd = accept(srv_skt_fd, (struct sockaddr*)&cli_addr, &cli_addr_sz); 
    if (cli_skt_fd == -1) {
        FATAL("Accepting TCP Socket");
        return FD_INVALID;
    }
    DEBUGF("Received Socket Conn: [client_fd:%d]", cli_skt_fd);
    INFOF("Client[client_fd:%d], client_ip:%s, client_port:%d", 
        cli_skt_fd, inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);

    return cli_skt_fd;
}

int read_client(int cli_skt_fd, char *cli_buf, int buf_max_sz) {
    ssize_t rd_sz = read(cli_skt_fd, cli_buf, buf_max_sz);
    if (rd_sz == -1) { // server read client error
        ERRF("Read From Client[client_fd:%d]", cli_skt_fd);
        return TCP_SRV_RD_ERR;
    }
    if (rd_sz == 0 || strcmp(cli_buf, TCP_CLI_EXIT_MSG)==0) { // client EOF or quit
        INFOF("Client[client_fd:%d] closed, caused by: client %s", cli_skt_fd, rd_sz==0 ? "EOF": "quit");
        return TCP_CLI_EXIT;
    } else { // server received legally
        INFOF("Read From Client[client_fd:%d], Msg: %s", cli_skt_fd, cli_buf);
        return TCP_SRV_RD_OK;
    }
}

int write_client(int cli_skt_fd, char *buf, int buf_sz) {
    ssize_t wr_sz = write(cli_skt_fd, buf, buf_sz);
    if (wr_sz == -1) {
        ERRF("write to client[client_fd:%d] error", cli_skt_fd);
        return TCP_SRV_WR_ERR;
    }
    return TCP_SRV_WR_OK;
}

void fd_nonblocking(int fd) {
    int fd_flag = fcntl(fd, F_GETFL);
    fcntl(fd, fd_flag | O_NONBLOCK);
}