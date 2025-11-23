#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>


#include "tcp_lib.h"

int accept_clients_by_epoll(
    int srv_skt_fd,
    int cli_max_sz,
    int buf_max_sz
) {

    char *cli_buf = (char*)malloc(buf_max_sz);

    int epoll_fd = epoll_create1(0); // EPOLL_CLOEXEC
    if (epoll_fd == -1) {
        fprintf(stderr, "[ERROR] epoll_create1 create epoll_fd\n");
        return 1;
    }

    fd_nonblocking(srv_skt_fd);
    struct epoll_event srv_ev;
    srv_ev.events = EPOLLIN;
    srv_ev.data.fd = srv_skt_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv_skt_fd, &srv_ev) == -1) {
        fprintf(stderr, "[ERROR] epoll add server_fd:%d into interest\n", srv_skt_fd);
        return 1;
    }

    int tot_sz = cli_max_sz + 1;
    struct epoll_event tot_evs[tot_sz];
    for(;;) {
        int fd_cnt = epoll_wait(epoll_fd, tot_evs, tot_sz, -1);
        if (fd_cnt == -1){
            fprintf(stderr, "[ERROR] epoll wait all events\n");
            return 1;
        }

        for(int idx=0; idx<fd_cnt; idx++) {
            int cur_fd = tot_evs[idx].data.fd;
            if(cur_fd==srv_skt_fd) {
                int cli_fd = accept_client(srv_skt_fd);
                fd_nonblocking(cli_fd);

                struct epoll_event cli_ev;
                cli_ev.events = EPOLLIN | EPOLLET;
                cli_ev.data.fd = cli_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_fd, &cli_ev) == -1) {
                    fprintf(stderr, "[ERROR] epoll add client_fd:%d into interest\n", cur_fd);
                    return 1;
                }
            } else {
                int rd_res = read_client(cur_fd, cli_buf, buf_max_sz);
                if (rd_res == TCP_CLIENT_EXIT || rd_res == TCP_CLIENT_ERR) {
                    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cur_fd, NULL)==-1) {
                        fprintf(stderr, "[ERROR] epoll delete client_fd:%d from interest\n", cur_fd);
                    }
                    close(cur_fd);
                }
            }
        }

    } 
}