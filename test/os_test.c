#include <unistd.h>

#include <stdio.h>
int main() {

    __pid_t cur_pid = getpid();

    __pid_t ch_pid = fork();

    fprintf(stdout, "cur_pid:%d, child_pid:%d\n", cur_pid, ch_pid);

    // for(;;){

    // }

    return 0;
}