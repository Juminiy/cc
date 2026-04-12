#include <unistd.h>
#include <wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libctr/ctr_util.h"

typedef struct cmd{
    char *path;
    char *argv[8];
    int argc;
} cmd;

char *resolve_filepath(char *filename, char **__envp) {
    if(__strhasprefix(filename,"/")){
        return __strdup(filename);
    }

    // 首先搜索PATH环境变量
    char *env_paths = getenv("PATH");
    if (env_paths) {
        char *paths = __strdup(env_paths);
        char *saveptr = NULL;
        char *dir = strtok_r(paths, ":", &saveptr);
        while (dir) {
            char *_paths[3] = {dir, filename, NULL};
            char *abs_path = __strjoin(_paths, "/");
            if (access(abs_path, X_OK) == 0) {
                free(paths);
                return abs_path;
            }
            free(abs_path);
            dir = strtok_r(NULL, ":", &saveptr);
        }
        free(paths);
    }

    // 然后搜索__envp提供的额外路径
    for(; __envp && *__envp; __envp++){
        char *_paths[3] = {*__envp, filename, NULL};
        char *abs_path = __strjoin(_paths, "/");
        if(access(abs_path, X_OK) == 0){
            return abs_path;
        }
        free(abs_path);
    }

    return NULL;
}

int exec_one(cmd cmdx) {
    pid_t chpid = fork();

    if(chpid==-1){
        perror("fork child");
        exit(EXIT_FAILURE);
    }

    if(chpid==0){
        // char *__filepath = resolve_filepath(cmds[i].path, menvp);
        // if(__filepath==NULL){
        //     fprintf(stderr, "filepath: %s not found\n", cmds[i].path);
        //     exit(EXIT_FAILURE);
        // }
        // execve(__filepath, cmds[i].argv, menvp);
        execvp(cmdx.path, cmdx.argv);
        perror("execve");   
        exit(EXIT_FAILURE);
    } else {
        int parent_status;
        waitpid(chpid, &parent_status, 0);
        if(WIFEXITED(parent_status)){
            return WEXITSTATUS(parent_status);
        }
        return -1;
    }
}


// ls -l | grep "lib" | grep -v "tag" | wc -l
int main(int margc, char *margv[], char *menvp[]){

    size_t cmdsz = 4;
    cmd cmds[4] = {
        {.path="ls",.argc=2,.argv={"ls","-l",NULL}},
        {.path="grep",.argc=2,.argv={"grep","lib",NULL}},
        {.path="grep",.argc=3,.argv={"grep","-v","tag",NULL}},
        {.path="wc",.argc=2,.argv={"wc","-l",NULL}},
    };

    // 对于n个命令，需要n-1个管道
    int pipefds[3][2];  // 3个管道连接4个命令
    for(size_t i=0;i<cmdsz-1;i++){
        if(pipe(pipefds[i]) == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pids[4];
    for(size_t i=0;i<cmdsz;i++){
        pid_t chpid = fork();
        if(chpid==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(chpid==0){  // 子进程
            // 连接输入：如果不是第一个命令，从上一个管道读取
            if(i>0){
                dup2(pipefds[i-1][0], STDIN_FILENO);
            }

            // 连接输出：如果不是最后一个命令，写到下一个管道
            if(i<cmdsz-1){
                dup2(pipefds[i][1], STDOUT_FILENO);
            }

            // 关闭所有不需要的管道文件描述符
            for(size_t j=0;j<cmdsz-1;j++){
                // 关闭读端：如果不是当前命令的输入管道
                if(i == 0 || j != i-1){
                    close(pipefds[j][0]);
                }
                // 关闭写端：如果不是当前命令的输出管道
                if(i == cmdsz-1 || j != i){
                    close(pipefds[j][1]);
                }
            }

            // 执行命令
            execvp(cmds[i].path, cmds[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {  // 父进程
            pids[i] = chpid;
            // 父进程关闭当前命令已经使用过的管道端
            if(i>0){
                close(pipefds[i-1][0]);  // 前一个管道的读端已经给了子进程
            }
            if(i<cmdsz-1){
                close(pipefds[i][1]);    // 当前命令的输出管道写端已经给了子进程
            }
        }
    }

    // 父进程等待所有子进程结束
    int status;
    for(size_t i=0;i<cmdsz;i++){
        waitpid(pids[i], &status, 0);
    }

    return 0;
}