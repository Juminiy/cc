#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

#include "../../libctr/ctr_util.h"
#include "../../libctr/ctr_sbuf.h"
#include "../../libctr/ctr_rbtree.h"

#define debug_envp(__envp, __caller) \
    do { \
        DEBUGF("%s",CSTRRED(__caller)); \
        for(char **_envp=__envp;*_envp;_envp++){ \
            DEBUGF("%s",*_envp); \
        } \
    }while(0)
#undef debug_envp

// char* envp_get_path(char *envp[]) {
//     for(char **_envp=envp;*_envp;_envp++){
//         if(*_envp && strncmp(*_envp,"PATH=",5)==0){
//             return *_envp;
//         }
//     }
//     return NULL;
// }

typedef struct {
    rb_tree *_tr; // <_syscall_record*>
} _syscall_records;

typedef struct {
    char *syscall_name;
    int cnt;
    float ts;
    float *tot_ts;
} _syscall_record;

int record_cmp(elem_t r0, elem_t r1){
    _syscall_record *_r0 =cast_elem_typ(r0, _syscall_record *);
    _syscall_record *_r1 =cast_elem_typ(r1, _syscall_record *);
    int f0 = __strcmp(_r0->syscall_name,_r1->syscall_name);
    float f1 = _r0->ts-_r1->ts;
    // return f0==0?(f1>0?+1:-1):f0;
    return f0;
}

elem_t record_merge(elem_t _dest, elem_t _src) {
    _syscall_record *_r0 =cast_elem_typ(_dest, _syscall_record *);
    _syscall_record *_r1 =cast_elem_typ(_src, _syscall_record *);
    _r0->ts += _r1->ts;
    _r0->cnt++;
    return _dest;
}

void free_record(elem_t r0) {
    _syscall_record *_r0 =cast_elem_typ(r0, _syscall_record *);
    free(_r0->syscall_name);
    free(_r0);
}

bool record_vis(elem_t r0) {
    _syscall_record *_r0 =cast_elem_typ(r0, _syscall_record *);
    printf("%s (%.3f%%)\n", _r0->syscall_name, ((_r0->ts*100)/(*_r0->tot_ts)));
    return true;
}

_syscall_records make_records(){
    _syscall_records rcds;
    rcds._tr = makeRBTree(record_cmp);
    setRBTreeNodeType(rcds._tr, TREE_NODE_TYPE_MAVL);
    setRBTreeDataFree(rcds._tr, free_record);
    setRBTreeDataMerge(rcds._tr, record_merge);
}

void records_put(_syscall_records rcds, char *_name, float _ts, float *_tot_ts) {
    _syscall_record *rcd = (_syscall_record *)malloc(sizeof(_syscall_record));
    rcd->syscall_name = __strdup(_name);
    rcd->ts = _ts;
    rcd->tot_ts = _tot_ts;
    rcd->cnt=1;
    elem_t em; setup_elem_ptr(em, rcd);
    rbTreeInsertData(rcds._tr, em);
    // em = rbTreeGetData(rcds._tr, em);
    // if(valid_elem_t(em)){
    //     rcd = cast_elem_typ(em, _syscall_record *);
    //     rcd->ts += _ts;
    // } else {
    //     setup_elem_ptr(em, rcd);
    //     rbTreeInsertData(rcds._tr, em);
    // }
}

int main(int argc, char *argv[], char *envp[]) {
    if(argc<2){
        ERRORF("lacked argv[1:]");
        return EXIT_FAILURE;
    }

    int fds[2];
    if(pipe(fds)==-1){
        perror("pipe error");
        return EXIT_FAILURE;
    }

    int devnull_fd = open("/dev/null",O_RDWR,0666);

    __pid_t chpid = fork();
    if(chpid==-1){
        perror("fork error");
        return EXIT_FAILURE;
    } else if (chpid==0){ // childp write
        close(fds[0]);
        dup2(devnull_fd, STDOUT_FILENO);
        dup2(fds[1], STDERR_FILENO);
        close(fds[1]);

        char *ch_argv[argc+2];
        ch_argv[0] = "strace";
        ch_argv[1] = "-T";
        for (int i=1;i<argc;i++){
            ch_argv[i+1] = argv[i];
        }
        ch_argv[argc+1] = NULL;
        execve("/bin/strace", ch_argv, envp);
        perror("child: execve");
    } else { // parentp read
        if(waitpid(chpid, NULL, 0)==-1){ // blocking wait
            perror("parent: wait child error");
            return EXIT_FAILURE;
        }
        close(fds[1]);
        
        // int _buffd = open("data/test/psave.txt", O_CREAT|O_TRUNC|O_WRONLY, 0666);
        // dup2(_buffd, STDOUT_FILENO);

        // char *pa_argv[]={"cat", NULL};
        // execve("/bin/cat", pa_argv, envp);
        // perror("parent: execve");
        
        // close(_buffd);

        roSBuf curbuf = readFdAll(fds[0]);
        close(fds[0]);

        float tot_ts = 0; 
        _syscall_records rcds = make_records();
        char *pch = strtok(curbuf._p, "\n");
        while(pch){
            int namesz = strchr(pch, '(')-pch;
            if (namesz<=0){
                break;
            }
            char *rcd_name = __strdupn(pch, namesz);
            char *tsptr = strchr(pch, '<');
            if(!tsptr){
                break;
            }
            float rcd_ts = strtof(tsptr+1,NULL);
            tot_ts += rcd_ts;
            records_put(rcds, rcd_name, rcd_ts, &tot_ts);
            pch = strtok(NULL, "\n");
        }
        // __writetruncfile("data/test/psave.txt", roSBufStr(curbuf));
        rbTreeIter(rcds._tr, record_vis);

        freeROSBuf(curbuf);
    }

    return 0;
}