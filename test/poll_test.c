#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// usage:

// poll_testd <fname1> <fname2> <fname3> ...

// intention: 
// to verify the poll mode is a Level-Triggered EventLoop
int main(int argc, char** argv) {

    if (argc<2) {
        fprintf(stderr, "[ERROR] no filenames\n");
        return 1;
    }

    int fd_cnt = argc-1;
    int fd_live = argc-1;
    struct pollfd* fd_lis = 
        (struct pollfd*)malloc(sizeof(struct pollfd)*fd_cnt);

    for (int idx=0;idx<fd_cnt;idx++){
        int cur_fd = open(argv[idx+1], O_RDONLY);
        if (cur_fd==-1) {
            fprintf(stderr, "[ERROR] open file: %s\n", argv[idx+1]);
            return 1;
        }
        fd_lis[idx].fd = cur_fd;
        fd_lis[idx].events = POLLIN;
    }

    int rd_max_sz = 16;
    char rd_buf[rd_max_sz];
    while (fd_live > 0) {
        int poll_res = poll(fd_lis, fd_cnt, -1);
        if (poll_res == 0) {
            fprintf(stderr, "[ERROR] timeout\n");
        } else if (poll_res == -1) {
            fprintf(stderr, "[ERROR] event: %s\n", strerror(errno));
        } else { // poll_res > 0
            for(int idx=0;idx<fd_cnt;idx++){
                int eve_fd = fd_lis[idx].fd;
                short int ret_eve = fd_lis[idx].revents;
                fprintf(stdout, "[INFO ] event type: %s%s%s, event status: %s%s%s\n", 
                    ret_eve&POLLIN ?   "POLLIN":"",
                    ret_eve&POLLPRI ?  "POLLPRI":"",
                    ret_eve&POLLOUT ?  "POLLOUT":"",
                    ret_eve&POLLERR ?  "POLLERR":"",
                    ret_eve&POLLHUP ?  "POLLHUP":"",
                    ret_eve&POLLNVAL ? "POLLNVAL":""
                );
                if (ret_eve&POLLIN) {
                    int rd_sz = read(eve_fd, rd_buf, sizeof(rd_buf));
                    switch (rd_sz) {
                        case -1: {
                            fprintf(stderr, "[ERROR] read fd:%d, error\n", eve_fd);
                            break;
                        }
                        case 0: {
                            // fd_lis[idx].fd = -1; // set -1 to return 0
                            fprintf(stderr, "[INFO ] read fd:%d, EOF\n", eve_fd);
                            break;
                        }
                        default: {
                            if (rd_sz<rd_max_sz) {
                                rd_buf[rd_sz+1]='\0';
                            }
                            fprintf(stdout, "[INFO ] read from fd:%d, msg:%s\n", eve_fd, rd_buf);
                            break;
                        }
                    }

                } else {
                    fprintf(stdout, "[INFO ] closing fd:%d\n", eve_fd);
                    int cls_res = close(eve_fd);
                    if (cls_res == -1) {
                        fprintf(stderr, "[ERROR] close fd:%d error\n", eve_fd);
                    }
                    fd_live--;
                }
            }
        }
        sleep(1);
    }


    return 0;
}