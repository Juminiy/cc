#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "../../libctr/ctr_util.h"

int my_execvp(char *__path, char *__argv[]) {
    if(!__strhasprefix(__path,"/")){
        char * env_path_val = getenv("PATH");
        while(1) {
            char *vptr = strchr(env_path_val, ':');
            char *pref = __substr(env_path_val, 0, vptr-env_path_val);
            char *seps[] = {pref,__path,NULL};
            char *vp = __strjoin(seps, "/");
            if(access(vp, X_OK)==0){
                __path = vp;
                break;
            }
            env_path_val = vptr+1;
        }
    }
    __argv[0] = __path;
    #if defined(__USE_GNU) || defined(__MUSL__)
    execve(__path, __argv, environ);
    #else
    execve(__path, __argv, __environ);
    #endif
    
    perror("execve error");
}

int main(int __argc, char *__argv[], char *__envp[]) {
    if(__argc<2){
        ERRORF("lack argv[1:]");
        return EXIT_FAILURE;
    }
    my_execvp("strace", __argv);

    return 0;
}