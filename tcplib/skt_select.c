#include "tcp_lib.h"
#include "skt_fdset.h"

#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

void fd_nonblock(int fd) {
    int fd_flag = fcntl(fd, F_GETFL);
    fcntl(fd, fd_flag | O_NONBLOCK);
}

int accept_clients_by_select(
    int srv_skt_fd, // server_fd
    int cli_max_sz, 
    int buf_max_sz
) {
    fd_nonblock(srv_skt_fd);
    optfdset *fdlis = create_optfdset(cli_max_sz + 1); // srv_skt_fd + cli_skt_fds
    optfdset_add(fdlis, srv_skt_fd);

    char *cli_buf = (char*)malloc(sizeof(char)*buf_max_sz);

    for(;;) {
        fd_set skt_fd_set; 
        FD_ZERO(&skt_fd_set); // init 
        FD_SET(srv_skt_fd, &skt_fd_set); // put server_fd listen read

        for(int idx=0;idx<fdlis->cap;idx++){
            if (optfdset_valid(fdlis->arr[idx])){
                FD_SET(fdlis->arr[idx], &skt_fd_set); // put client_fd already listen
            }
        }

        // fprintf(stdout, "[DEBUG] select to fd\n");
        
        int ready_fd_cnt = select(fdlis->max+1, &skt_fd_set, NULL, NULL, NULL);

        // fprintf(stdout, "[DEBUG] ready_fd_cnt = %d\n", ready_fd_cnt);

        if (ready_fd_cnt == -1) {
            ERRF(select fds);
            return 1;
        } else if (ready_fd_cnt > 0) {
        
            for(int idx=0;idx<fdlis->cap;idx++) {
                int curfd = fdlis->arr[idx];
                if (optfdset_valid(curfd) && FD_ISSET(curfd, &skt_fd_set)) {
                    
                    if (curfd == srv_skt_fd) { // server socket accept read 
                        int cli_skt_fd = accept_client(srv_skt_fd);
                        int add_ret = optfdset_add(fdlis, cli_skt_fd);
                        if (add_ret == SET_FULL) {
                            close(cli_skt_fd);
                            fprintf(stdout, "[WARN ] Client[client_fd:%d] connect exceed cli_max_sz:%d, rejected\n", cli_skt_fd, cli_max_sz);
                        } else {
                            fd_nonblock(cli_skt_fd);
                            fprintf(stdout, "[INFO ] Client[client_fd:%d] success enqueue the connections\n", cli_skt_fd);
                        }
                    } else { // client socket buffer read
                        int rd_res = read_client(curfd, cli_buf, buf_max_sz);
                        if (rd_res == TCP_CLIENT_EXIT || rd_res == TCP_CLIENT_ERR) {
                            int del_cli_res = optfdset_del(fdlis, curfd);
                            if (del_cli_res != OPT_OK) {
                                fprintf(stderr, "[ERROR] delete client Client[client_fd:%d]\n", curfd);
                            }
                        }
                    }

                }
            }

        } else { // never occur

        }

    }

    delete_optfdset(fdlis);
    free(cli_buf);
    
    return 0;
}
