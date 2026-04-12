#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

#include "../../libctr/ctr_color.h"

int pipe_commu(int _chffd, int _pffd) {
    int fds[2];
    if(pipe(fds)==-1){ // read from fds[0], write to fds[1]
        perror("create pipe error");
        return EXIT_FAILURE;
    }

    printf("read_pipe: %d, write_pipe: %d\n", fds[0], fds[1]);

    __pid_t chpid = fork();
    if (chpid==-1){
        perror("fork error");
        return EXIT_FAILURE;
    } else if(chpid==0){ // child process write
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        dup2(_chffd, STDOUT_FILENO);
        // close(fds[1]);

        char *exec_argv[] = { "strace", "ls", "-l", NULL, };
        char *exec_envp[] = { "PATH=/bin", NULL, };
        execve("/bin/strace",          exec_argv, exec_envp);
        perror(CSTRRED("execve child"));
    } else { // parent process read
        close(fds[1]);
        dup2(fds[0],STDIN_FILENO);
        dup2(_pffd, STDOUT_FILENO);
        // close(fds[0]);
        
        char *exec_argv[] = { "wc", "-c", NULL, };
        char *exec_envp[] = { "PATH=/bin", NULL, };
        execve("/bin/wc",          exec_argv, exec_envp);
        perror(CSTRRED("execve parent"));

        waitpid(chpid,NULL,0);
    }

    return EXIT_SUCCESS;
}

int main(){
    // int _ffd = open("data/test/pipe.txt", O_WRONLY|O_APPEND|O_CREAT,0666);
    // if(_ffd==-1){
    //     perror("open file error:");
    // }

    // dup2(_ffd, STDOUT_FILENO);
    // dup2(_ffd, STDERR_FILENO);

    // fprintf(stdout, "this is stdout desc\n");
    // fprintf(stderr, "this is stderr desc\n");


    int _chffd = open("data/test/pipe_ch.txt", O_WRONLY|O_TRUNC|O_CREAT,0666);
    if(_chffd==-1){
        perror("open file error:");
    }
    int _pffd = open("data/test/pipe_pa.txt", O_WRONLY|O_TRUNC|O_CREAT,0666);
    if(_pffd==-1){
        perror("open file error:");
    }
    if(pipe_commu(_chffd,_pffd)){
        perror("commu error:");
    }
    close(_chffd);
    close(_pffd);
    

    return 0;
}