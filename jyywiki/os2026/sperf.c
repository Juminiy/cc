#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "../../test/test.h"
#include "../../libctr/ctr_util.h"

char* envp_get_path(char *envp[]) {
    for(char **_envp=envp;*_envp;_envp++){
        if(*_envp && strncmp(*_envp,"PATH=",5)==0){
            return *_envp;
        }
    }
    return NULL;
}

int main(int argc, char *argv[], char *envp[]) {
    #define debug_envp(__envp, __caller) \
        do { \
            DEBUGF("%s",CSTRRED(__caller)); \
            for(char **_envp=__envp;*_envp;_envp++){ \
                DEBUGF("%s",*_envp); \
            } \
        }while(0)
    
    if(argc<2){
        ERRORF("lacked argv[1:]");
        return EXIT_FAILURE;
    }

    __pid_t chpid = fork();

    if(chpid==-1){
        perror("fork error");
        return EXIT_FAILURE;
    } else if (chpid==0){
        // debug_envp(envp, "child");
        char *_envp_ch[] = {envp_get_path(envp),NULL};
        char *_argv_ch[argc+1]; _argv_ch[0] = "strace";
        for(int i=1;i<argc;i++){
            _argv_ch[i] = argv[i];
        }                       _argv_ch[argc] = NULL;
        // char *_argv_ch[] = {"strace", "ls", NULL};
        // char *_envp_ch[] = {"PATH=/usr/bin",NULL};
        execve("strace", _argv_ch, _envp_ch);
        
    } else {
        // debug_envp(envp, "parent");
        if(waitpid(chpid, NULL, 0)==-1){
            perror("waitpid error");
        } else { // blocking wait
            char stdbuf[256];
            read(STDOUT_FILENO, stdbuf, 256);
            puts(stdbuf);
        }
    }
    #undef debug_envp

    return 0;
}