#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sys/stat.h>
#include "../../libctr/ctr_sbuf.h"
#include "../../libctr/ctr_util.h"

int main() {

    #define _DIR "/tmp/crepl/" 
    #define _FMODE 0777
    #define _IN_EXPR 1
    #define _IN_FUNC 2

    #define __fork_execve(__seg) \
        do { \
            pid_t chpid = fork();\
            if(chpid==0){ __seg } \
        } while(0)


    if(mkdir(_DIR, _FMODE)==-1){
        // perror("mkdir "_DIR);
        // return EXIT_FAILURE;
    }
    chmod(_DIR, _FMODE);

    char *dep_h_fname =   _DIR"deps.h";
    char *funcs_c_fname = _DIR"funcs.c";
    char *eval_c_fname =  _DIR"eval.c";
    // int deps_h_fd = mkstemp(dep_h_fname);
    // int funcs_c_fd = mkstemp(funcs_c_fname);
    // int eval_c_fd = mkstemp(eval_c_fname);
    // rename(dep_h_fname,   "/tmp/crepl/""deps.h");
    // rename(funcs_c_fname, "/tmp/crepl/""funcs.c");
    // rename(eval_c_fname,  "/tmp/crepl/""eval.c");

    int deps_h_fd  = open(dep_h_fname, O_CREAT|O_APPEND|O_RDWR, _FMODE);
    int funcs_c_fd = open(funcs_c_fname, O_CREAT|O_APPEND|O_RDWR, _FMODE);
    int eval_c_fd  = open(eval_c_fname, O_CREAT|O_TRUNC|O_RDWR, _FMODE);

    char * include_str = "#include \"""/tmp/crepl/""deps.h\"\n";
    write(funcs_c_fd, include_str, __strlen(include_str));
    for(;;){
        printf("crepl> ");
        fflush(stdout);
        char buf[256];
        fgets(buf, sizeof(buf), stdin);
        
        // int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
        char *func_decl_ptr = strchr(buf, ')');
        char *func_def_ptr = strchr(buf, '}');
        if(func_def_ptr){ // funcs
            write(deps_h_fd, buf, func_decl_ptr-buf);
            write(deps_h_fd, "\n", 1);
            write(funcs_c_fd, buf, func_def_ptr-buf);
            write(funcs_c_fd, "\n", 1);
            __fork_execve(
                execlp("/usr/bin/cc", "cc", "-c", "-o", _DIR"funcs.o", _DIR"funcs.c", NULL);
                perror("cc funcs error");
            );
        } else { // expr
            __fork_execve(
                execlp("/usr/bin/cc", "cc", "-c", "-o", _DIR"funcs.o", _DIR"funcs.c", NULL);
                perror("cc funcs error");
            );
            sbuf *bf = makeSBuf(256);
            sBufWrite(bf, "#include \"""/tmp/crepl/""deps.h\"\n");
            sBufWrite(bf, "#include <stdio.h>\n");
            sBufWrite(bf, "int main(){\n");
            sBufWriteFmt(bf, "printf(\"%%d\", %s);\n", buf);
            sBufWrite(bf, " return 0;\n");
            sBufWrite(bf, "}\n");
            write(eval_c_fd, sBufStr(bf), sBufSize(bf));

            __fork_execve(
                execlp("/usr/bin/cc", "cc", "-o", _DIR"eval.d", _DIR"funcs.o", _DIR"eval.c", NULL);
                perror("cc eval error");
            );

            char *ch_args[] = {"eval.d", NULL};
            char *ch_envp[] = {"PATH="_DIR, NULL};
            __fork_execve(
                execve(_DIR"eval.d", ch_args, ch_envp);
                perror("cc eval.d error");
            );
        }
    }

    close(deps_h_fd);
    close(funcs_c_fd);
    close(eval_c_fd);
    remove(_DIR"deps.h");
    remove(_DIR"funcs.c");
    remove(_DIR"eval.c");

    return 0;
}