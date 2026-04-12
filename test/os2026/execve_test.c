#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/fcntl.h>

#include "../../libctr/ctr_color.h"

int test_execve(int argc, char *argv[], char *envp[]) {
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
    return EXIT_SUCCESS;
}

void test_execl() {
    char *argv[]={"ls", "-l", NULL};
    char *envp[]={"/usr/bin:/bin:/usr/local/bin:/usr/local/sbin", NULL};
    
    execl("ls", "ls", "-l", NULL);
    execle("ls", "ls", "-l", NULL, envp);
    execlp("ls", "ls", "-l", NULL);

    execv("ls", argv);
    execve("ls", argv, envp);
    execvp("ls", argv);
}

void print_strs(char *__strs[]) {
    for(char **_s=__strs;_s&&*_s;_s++){
        printf("%s\n", *_s);
    }
}

bool cmp_strs(char *_s1[], char *_s2[]){
    for(;_s1&&_s2&&*_s1&&*_s2;_s1++,_s2++){
        if(__strcmp(*_s1,*_s2)){
            fprintf(stderr, "%s != %s\n", *_s1, *_s2);
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[], char *envp[]) {
    // puts(CSTRGREEN("envp"));
    // print_strs(envp);

    // puts(CSTRGREEN("environ"));
    // print_strs(__environ);

    puts(CSTRGREEN("envp ?= environ"));
    printf("%d\n", cmp_strs(envp, __environ));

    return 0;
}