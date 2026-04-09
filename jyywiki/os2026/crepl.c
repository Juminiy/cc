#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include <sys/wait.h>
#include "../../libctr/ctr_sbuf.h"
#include "../../libctr/ctr_util.h"
#include "../../libctr/ctr_color.h"


#define _DIR "/tmp/crepl/" 
#define _deps_h_path _DIR"deps.h"
#define _funcs_c_path _DIR"funcs.c"
#define _funcs_c_dup_path _DIR"funcs_dup.c"
#define _eval_c_path _DIR"eval.c"
#define _include_hdr "#include \""_deps_h_path"\"\n"

#define _FMODE 0777
#define _IN_EXPR 1
#define _IN_FUNC 2

#define __clear_screen() \
    do { puts("\033[2J\033[H"); fflush(stdout); } while(0)

#define __trim_str(__buf) \
    do { \
        for(;(*__buf)!='\0'&&isspace(*__buf);__buf++){} \
        __buf[strcspn(__buf,"\n")]='\0'; \
    } while(0)

#define __fork_catch_err(chp_do, err_do, ok_do) \
    do { \
        int fderr[2]; pipe(fderr); \
        pid_t ch_pid = fork(); \
        if(ch_pid==0){ \
            close(fderr[0]); dup2(fderr[1],STDERR_FILENO); close(fderr[1]); \
            chp_do \
            exit(EXIT_FAILURE); \
        } else { \
            waitpid(ch_pid, NULL, 0); \
            close(fderr[1]); roSBuf errbuf = readFdAll(fderr[0]); close(fderr[0]); \
            if(roSBufSiz(errbuf)>0){ \
                err_do \
            } else { \
                ok_do \
            } \
            freeROSBuf(errbuf); \
        } \
    } while(0)

void execute_eval(char * buf) {
    // execute eval.d
    int fdout[2]; pipe(fdout);
    int fderr[2]; pipe(fderr);
    pid_t evald_exe_pid = fork();
    if(evald_exe_pid==0){
        close(fdout[0]); dup2(fdout[1], STDOUT_FILENO); close(fdout[1]);
        close(fderr[0]); dup2(fderr[1], STDERR_FILENO); close(fderr[1]);
        execlp(_DIR"eval.d", "eval.d", NULL);
        perror("execute eval.d error");
        exit(EXIT_FAILURE);
    } else {
        waitpid(evald_exe_pid,NULL,0);
        close(fdout[1]); close(fderr[1]);
        roSBuf bufout = readFdAll(fdout[0]); close(fdout[0]);
        roSBuf buferr = readFdAll(fderr[0]); close(fderr[0]);
        if(roSBufSiz(buferr)>0){
            fprintf(stderr, "%s\n", CSTRRED("Execute Error")); freeROSBuf(buferr);
        } else {
            printf("(%s) == %s\n",buf, roSBufStr(bufout)); freeROSBuf(bufout);
        }
    }
}

void do_eval(char * buf) {
    // write eval.c
    sbuf *bf = makeSBuf(256);
    sBufWrite(bf, _include_hdr);
    sBufWrite(bf, "#include <stdio.h>\n");
    sBufWrite(bf, "int main(){\n");
    sBufWriteFmt(bf, "printf(\"%%d\", %s);\n", buf);
    sBufWrite(bf, "  return 0;\n");
    sBufWrite(bf, "}\n");
    int eval_c_fd  = open(_eval_c_path, O_CREAT|O_TRUNC|O_RDWR, _FMODE);
    write(eval_c_fd, sBufStr(bf), sBufSize(bf));
    close(eval_c_fd);

    // compile eval.c 
    __fork_catch_err(
        execlp("/usr/bin/cc", "cc", "-w", "-o", _DIR"eval.d", _DIR"funcs.o", _eval_c_path, NULL);
        perror("compile eval.d error");
        ,
        fprintf(stderr, "%s\n", CSTRRED("Compile Error"));
        ,
        // execute eval.d
        execute_eval(buf);
    );

}

// funcs: 
// int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
// expr:
// 1+1
int main() {
    if(__mkdir(_DIR, _FMODE)==-1){
        perror("mkdir "_DIR);
        return EXIT_FAILURE;
    }
    if(chmod(_DIR, _FMODE)==-1){
        perror("chmod "_DIR);
        return EXIT_FAILURE;
    }

    int deps_h_fd  = open(_deps_h_path, O_CREAT|O_TRUNC|O_RDWR, _FMODE);
    int funcs_c_fd = open(_funcs_c_path, O_CREAT|O_TRUNC|O_RDWR, _FMODE);
    write(funcs_c_fd, _include_hdr, __strlen(_include_hdr));
    int funcs_c_dup_fd = open(_funcs_c_dup_path, O_CREAT|O_TRUNC|O_RDWR, _FMODE);
    write(funcs_c_dup_fd, _include_hdr, __strlen(_include_hdr));
    
    size_t bufsz = 512;
    char *buf = (char*)malloc(sizeof(char)*bufsz);
    for(;;){
        printf("crepl> ");
        fflush(stdout);

        if (fgets(buf, bufsz, stdin)==NULL){
            break;
        }
        __trim_str(buf);
        
        if(__strlen(buf)==0){
            continue;
        } else if (!__strcmp(buf, "!q")||!__strcmp(buf, "quit")||!__strcmp(buf, "exit")){
            break;
        } else if (!__strcmp(buf, "clear")||!__strcmp(buf, "cls")) {
            __clear_screen(); 
            continue;
        }
        
        char *func_decl_ptr = strchr(buf, ')');
        char *func_def_ptr = strrchr(buf, '}');
        if(strncmp(buf, "#include", 8)==0){
            write(deps_h_fd, buf, __strlen(buf));
            write(deps_h_fd, "\n", 1);
        } else if(func_def_ptr){ // funcs
            write(deps_h_fd, buf, func_decl_ptr-buf+1);
            write(deps_h_fd, ";\n", 2);
            write(funcs_c_dup_fd, buf, func_def_ptr-buf+1);
            write(funcs_c_dup_fd, "\n", 1);
            
            // compile funcs.c
            __fork_catch_err(
                execlp("/usr/bin/cc", "cc", "-w", "-c", "-o", _DIR"funcs.o", _funcs_c_dup_path, NULL);
                perror("compile funcs.c error");
                ,
                fprintf(stderr, "%s\n", CSTRRED("Compile Error"));
                ,
                puts(CSTRGREEN("OK."));
                write(funcs_c_fd, buf, func_def_ptr-buf+1);
                write(funcs_c_fd, "\n", 1);
            );
            
        } else { // expr

            // compile funcs.c
            __fork_catch_err(
                execlp("/usr/bin/cc", "cc", "-w", "-c", "-o", _DIR"funcs.o", _funcs_c_path, NULL);
                perror("compile funcs.c error");
                ,
                fprintf(stderr, "%s\n", CSTRRED("Compile Error"));
                ,
                do_eval(buf);
            );
        }
    }

    close(deps_h_fd);
    close(funcs_c_fd);

    return 0;
}