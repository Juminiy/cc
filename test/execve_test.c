#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>

int main(int argc, char *argv[], char *envp[]) {
    if(argc<2){
        fprintf(stderr, "argc too small\n");
        return EXIT_FAILURE;
    }

    int ffd = open("data/test/strace.txt", O_CREAT|O_WRONLY|O_TRUNC, 0666);
    // dup2(ffd, STDOUT_FILENO);
    dup2(ffd, STDERR_FILENO);

    if(execve(argv[1], argv+1, envp)==-1) {
        perror("execve");
    }

    close(ffd);
    return 0;
}