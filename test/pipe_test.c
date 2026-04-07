#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

    int fds[2];
    if(pipe(fds)==-1){ // read from fds[0], write to fds[1]
        perror("create pipe error");
        return EXIT_FAILURE;
    }

    printf("read_pipe: %d, write_pipe: %d\n", fds[0], fds[1]);

    // char msg[256];
    // scanf("%s", msg);

    __pid_t chpid = fork();
    if (chpid==-1){
        perror("fork error");
        return EXIT_FAILURE;
    } else if(chpid==0){ // child process write
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);
        char *exec_argv[] = { "strace", "ls", NULL, };
        char *exec_envp[] = { "PATH=/bin", NULL, };
        execve("strace",          exec_argv, exec_envp);
        // perror("execve");
    } else { // parent process read
        close(fds[1]);
        dup2(fds[0],STDIN_FILENO);
        close(fds[0]);
        waitpid(chpid,NULL,0);
        char *exec_argv[] = { "wc", "-l", NULL, };
        char *exec_envp[] = { "PATH=/bin", NULL, };
        execve("wc",          exec_argv, exec_envp);
    }

    return 0;
}