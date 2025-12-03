#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>

#include <sys/socket.h>

#include "../tcplib/tcp_lib.h"
#include "../tcplib/net_util.h"

#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include <pthread.h>

// int glb_skt_fd = -1;
// void __attribute__((destructor)) __inexit() {
//     INFO("clean socket");
//     if (glb_skt_fd != -1) {
//         close(glb_skt_fd);
//     }
// }

typedef struct {
    int skt_fd;
    char *buf;
    int buf_sz;
} io_srv;

void* read_server(void *arg) {
    io_srv *rd_srv = arg;
    for (;;) {
        ssize_t rd_sz = read(rd_srv->skt_fd, rd_srv->buf, rd_srv->buf_sz);
        if (rd_sz==-1) {
            ERRF("read from server error");
        } else if(rd_sz==0){
            FATAL("server disconnected");
        } else {
            str_trunc(rd_srv->buf, rd_sz);
            fprintf(stdout, "\r[OUPUT] > %s\n", rd_srv->buf);
            fprintf(stdout, "\r[INPUT] < ");
        }
    }
    return NULL;
}

void* write_server(void *arg) {
    io_srv *wr_srv = arg;
    for (;;) {
        fprintf(stdout, "\r[INPUT] < ");
        int cinsz = fscanf(stdin, "%s", wr_srv->buf);
        if (cinsz == -1) {
            FATAL("CIN Scan Bytes");
        }
        ssize_t wr_sz = write(wr_srv->skt_fd, wr_srv->buf, wr_srv->buf_sz);
        if (wr_sz == 0){
            INFOF("server disconnected");
            break;
        } else if (wr_sz == -1) {
            FATAL("Client Write");
            break;
        } else if (strcmp(wr_srv->buf, TCP_CLI_EXIT_MSG) == 0) {
            INFOF("client disconnected");
            break;
        }
    }
}

int main(int argc, char** argv) {

    int addr_family = AF_INET;
    char srv_addr[128] = {'\0'};
    int srv_port = 3054;
    int cli_send_max_sz = 4096; // 4KB

    const char *shrt_opts = "h46a:p:b:s";
    const struct option long_opts[] = 
    {
        {"help", no_argument, NULL, 'h'},
        {"ipv4", no_argument, NULL, '4'},       
        {"ipv6", no_argument, NULL, '6'},
        {"addr", required_argument, NULL, 'a'},
        {"port", optional_argument, NULL, 'p'},
        {"bsiz", optional_argument, NULL, 'b'},
        {"show-intf", no_argument, NULL, 's'}
    };
    #define HELP_ARGS() \
        do { \
            fprintf(stdout, "Usage:\n"); \
            fprintf(stdout, "Option: [-4] | [-6] | [--ipv4] | [--ipv6], ip version, (default:ipv4)\n"); \
            fprintf(stdout, "Option: [-a] | [--addr], bind address\n"); \
            fprintf(stdout, "Option: [-p] | [--port], bind port, (default:3054)\n"); \
            fprintf(stdout, "Option: [-b] | [--bsiz], client max buffer size byte, (default:4096)\n"); \
            fprintf(stdout, "Option: [-s] | [--show-intf], show net interface and ip\n"); \
        } while(0)

    int optcase = 0;
    while((optcase = getopt_long(argc, argv, shrt_opts, long_opts, NULL)) != -1)
    {
        switch(optcase) {
            // AF_INET:IPv4; AF_INET6:IPv6
            case '4': 
            addr_family = AF_INET;
            break;

            case '6':
            addr_family = AF_INET6;
            break;

            case 'a':
            strcpy(srv_addr, optarg);
            break;

            case 'p':
            srv_port = strtol(optarg, NULL, 10);
            break;

            case 'b':
            cli_send_max_sz = strtol(optarg, NULL, 10);
            break;

            case 's':
            SHOW_INTFIP(INFOF);
            return 0;

            case 'h':
            HELP_ARGS();
            return 0;

            default: 
            HELP_ARGS();
            return 0;
        }
    }

    if (strlen(srv_addr) == 0) {
        ERRF("bad address:%s(len=%ld) with adfamily:%d", srv_addr, strlen(srv_addr), addr_family);
        return 1;
    }
    if (srv_port<=0 || srv_port>65535) {
        ERRF("bad port:%d", srv_port);
        return 1;
    }
    if (cli_send_max_sz<=0) {
        ERRF("bad bsiz:%d", cli_send_max_sz);
        return 1;
    }
    
    int skt_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (skt_fd == -1){
        FATAL("Create Socket");
    }
    DEBUGF("Create Socket success, [client_fd:%d]", skt_fd);
    // glb_skt_fd = skt_fd;

    struct sockaddr_in skt_addr;
    sktaddrin_set(skt_addr, AF_INET, srv_port, srv_addr);
    int conn_err = connect(skt_fd, (struct sockaddr*) &skt_addr, sizeof(skt_addr));
    if (conn_err == -1) {
        ERRF("Connect to host %s:%d", srv_addr, srv_port);
        return 1;
    }
    INFOF("Connect to host %s:%d success", srv_addr, srv_port);

    pthread_attr_t tattr;
    pthread_t rd_thread_num = 1, wr_thread_num = 2;
    io_srv rd_srv = {
        .buf_sz = cli_send_max_sz,
        .buf = (char*)malloc(cli_send_max_sz),
        .skt_fd = skt_fd
    };
    io_srv wr_srv = {
        .buf_sz = cli_send_max_sz,
        .buf = (char*)malloc(cli_send_max_sz),
        .skt_fd = skt_fd
    };
    void *rd_thread_ret=NULL, *wr_thread_ret=NULL;
    if(pthread_attr_init(&tattr)) {
        FATAL("threads initing attr");
    }
    if(pthread_create(&rd_thread_num, &tattr, read_server, &rd_srv)) {
        FATAL("read_server thread creating");
    }
    if(pthread_create(&wr_thread_num, &tattr, write_server, &wr_srv)) {
        FATAL("write_server thread creating");
    }
    if(pthread_attr_destroy(&tattr)) {
        FATAL("threads destroying attr");
    }
    if(pthread_join(rd_thread_num, &rd_thread_ret)) {
        FATAL("read_server thread joining");
    }
    if(pthread_join(wr_thread_num, &wr_thread_ret)) {
        FATAL("write_server thread joining");
    }

    close(skt_fd);
    return 0;
}

