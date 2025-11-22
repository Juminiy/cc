#include <sys/select.h>
#include <sys/time.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    // max_fd < FD_SETSIZE=1024
    fd_set rd_fd_set; FD_ZERO(&rd_fd_set);
    fd_set wr_fd_set; FD_ZERO(&wr_fd_set);
    FD_SET(0, &rd_fd_set); // stdin
    // FD_SET(1, &wr_fd_set); // stdout
    FD_SET(2, &wr_fd_set); // stderr

    struct timeval timev; 
    timev.tv_sec = 3;
    timev.tv_usec = 0;

    int resval = select(3, &rd_fd_set, &wr_fd_set, NULL, &timev); 
    if (resval == -1){
        fprintf(stderr, "select error\n");
    } else if (resval > 0) {
        fprintf(stdout, "%d, %d\n", 
            FD_ISSET(0, &rd_fd_set),
            // FD_ISSET(1, &wr_fd_set),
            FD_ISSET(2, &wr_fd_set)
        );
    } else {
        fprintf(stdout, "%ld second timeout\n", timev.tv_sec);
    }

}