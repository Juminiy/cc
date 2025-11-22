#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>

#include <sys/socket.h>

#include "../tcplib/tcp_lib.h"

#include <time.h>
#include <unistd.h>

#define ERRF(msg) \
    do { fprintf(stderr, "[Error] "#msg", error: %s\n", strerror(errno)); exit(EXIT_FAILURE); } while(0)

int main(int argc, char** argv) {

    int srv_port = 3054;
    char *srv_addr = "192.168.3.47";
    int cli_send_max_sz = 1<<5;


    int skt_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (skt_fd == -1){
        ERRF(Create Socket Error);
        return 1;
    }
    fprintf(stdout, "[DEBUG] Create Socket success, [client_fd:%d]\n", skt_fd);

    struct sockaddr_in skt_addr;
    sktaddrin_set(skt_addr, AF_INET, srv_port, srv_addr);
    int conn_err = connect(skt_fd, (struct sockaddr*) &skt_addr, sizeof(skt_addr));
    if (conn_err == -1) {
        fprintf(stderr, "connect to host %s:%d\n", srv_addr, srv_port);
        return 1;
    }
    fprintf(stdout, "[INFO ] Connect to host %s:%d success\n", srv_addr, srv_port);

    char *wrbuf = (char*)malloc(cli_send_max_sz);
    for (;;) {
        fprintf(stdout, "please write to: ");
        int cinsz = fscanf(stdin, "%s", wrbuf);
        if (cinsz == -1) {
            ERRF(CIN Scan Bytes);
            return 1;
        }
        ssize_t wr_sz = send(skt_fd, wrbuf, cli_send_max_sz, 0);
        if (wr_sz == 0){
            fprintf(stdout, "[INFO ] server disconnected\n");
            break;
        }
        if (wr_sz == -1) {
            ERRF(Client Write buffer);
            break;
        }
        sleep(1);
    }

    close(skt_fd);
    return 0;
}