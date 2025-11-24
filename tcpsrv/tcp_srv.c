#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <getopt.h>

#include "../tcplib/tcp_lib.h"

int glb_skt_fd = -1;
void __attribute__((destructor)) __inexit() {
    INFOF("clean socket");
    if (glb_skt_fd != -1) {
        close(glb_skt_fd);
    }
}

int main(int argc, char** argv) {

    int domain_safamily = AF_INET;
    char skt_ipv4_addr[128] = {'\0'};
    int skt_ipv4_port = 3054;
    int skt_max_conns = 1024;
    int cli_buf_max_sz = 4096; // 4KB
    int server_mode = SRV_MODE_EPOLL;
    int log_level = LOG_LEVEL_DEBUG;

    char const * const shrt_opts = "h46a:p:c:b:m:l:";
    const struct option long_opts[] = 
    {
        {"help", no_argument, NULL, 'h'},
        {"ipv4", no_argument, NULL, '4'},       
        {"ipv6", no_argument, NULL, '6'},
        {"addr", required_argument, NULL, 'a'},
        {"port", optional_argument, NULL, 'p'},
        {"conn", optional_argument, NULL, 'c'},
        {"bsiz", optional_argument, NULL, 'b'},
        {"mode", optional_argument, NULL, 'm'},
        {"log", optional_argument, NULL, 'l'}
    };
    #define HELP_ARGS() \
        do { \
            fprintf(stdout, "Usage:\n"); \
            fprintf(stdout, "Option: [-4] | [-6] | [--ipv4] | [--ipv6], ip version, (default:ipv4)\n"); \
            fprintf(stdout, "Option: [-a] | [--addr], bind address\n"); \
            fprintf(stdout, "Option: [-p] | [--port], bind port, (default:3054)\n"); \
            fprintf(stdout, "Option: [-c] | [--conn], max client connections, (default:1024)\n"); \
            fprintf(stdout, "Option: [-b] | [--bsiz], client max buffer size byte, (default:4096)\n"); \
            fprintf(stdout, "Option: [-m] | [--mode], blocking:0, select:1, epoll:2, (default:2)\n"); \
            fprintf(stdout, "Option: [-l] | [--log], debug:0, info:1, error:2, (default:0)\n"); \
        } while(0)
    
    int optcase = 0;
    while((optcase = getopt_long(argc, argv, shrt_opts, long_opts, NULL)) != -1)
    {
        switch(optcase) {
            // AF_INET:IPv4; AF_INET6:IPv6
            case '4': 
            domain_safamily = AF_INET;
            break;

            case '6':
            domain_safamily = AF_INET6;
            break;

            case 'a':
            strcpy(skt_ipv4_addr,optarg);
            break;

            case 'p':
            skt_ipv4_port = strtol(optarg, NULL, 10);
            break;

            case 'c':
            skt_max_conns = strtol(optarg, NULL, 10);
            break;

            case 'b':
            cli_buf_max_sz = strtol(optarg, NULL, 10);
            break;

            case 'm':
            server_mode = strtol(optarg, NULL, 10);
            break;

            case 'l':
            log_level = strtol(optarg, NULL, 10);
            break;

            case 'h':
            HELP_ARGS();
            return 0;

            default: 
            HELP_ARGS();
            return 0;
        }
    }

    if (strlen(skt_ipv4_addr) == 0) {
        ERRF("bad address:%s(len=%ld) with adfamily:%d", skt_ipv4_addr, strlen(skt_ipv4_addr), domain_safamily);
        return 1;
    }
    if (skt_ipv4_port<=0 || skt_ipv4_port>65535) {
        ERRF("bad port:%d", skt_ipv4_port);
        return 1;
    }
    if (skt_max_conns<=0) {
        ERRF("bad conn:%d", skt_max_conns);
        return 1;
    }
    if (cli_buf_max_sz<=0) {
        ERRF("bad bsiz:%d", cli_buf_max_sz);
        return 1;
    }
    server_accept_f accf = NULL;
    switch (server_mode) {
        case SRV_MODE_BLOCKING: {
            accf = accept_clients_blocking;
            break;
        }
        case SRV_MODE_SELECT: {
            accf = accept_clients_by_select;
            break; 
        }
        case SRV_MODE_EPOLL: {
            accf = accept_clients_by_epoll;
            break;
        }
        default: {
            ERRF("server_mode:%d is not supportted", server_mode);
            return 1;
        }
    }

    // Create an `IPv4` socket
    int skt_fd = socket(domain_safamily, SOCK_STREAM, IPPROTO_TCP);
    if (skt_fd == -1) {
        FATAL("Creating TCP Socket");
    }
    DEBUGF("Create Socket success, [server_fd:%d]", skt_fd);
    glb_skt_fd = skt_fd;

    // Assign socket an `IPv4:Port` Address
    struct sockaddr_in svr_addr;
    sktaddrin_set(svr_addr, domain_safamily, skt_ipv4_port, skt_ipv4_addr);
    int bind_err = bind(skt_fd, (struct sockaddr*)&svr_addr, sizeof(svr_addr));
    if (bind_err == -1) {
        FATAL("Binding TCP Socket Address");
    }
    DEBUGF("Assign Socket Address success");

    // Listen to conns
    int lis_err = listen(skt_fd, skt_max_conns);
    if (bind_err == -1) {
        FATAL("Listening TCP Socket");
    }
    DEBUGF("Listening Socket Conns in: %s:%d", skt_ipv4_addr, skt_ipv4_port);

    accf(skt_fd, skt_max_conns, cli_buf_max_sz);

    if (close(skt_fd) == -1){
        FATAL("Closing TCP Socket");
    } else {
        INFOF("server closed normalized, exit");
        return 0;
    }
}