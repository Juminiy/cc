#include <pthread.h>

#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>

typedef struct task {
    pthread_t tid;
    int idx;
    char *params;
    int result;
} task;

void* task_run(void *arg) {
    task *tsk = arg;
    tsk->result = strtol(tsk->params, NULL, 10);
    return NULL;
}

#define EXITERR(msg) \
    do { fprintf(stderr, "msg:%s, err:%s\n", msg, strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define DEBUG(msg) \
    do { fprintf(stdout, msg"\n"); } while(0)

int main(int argc, char **argv) {

    int tsk_cnt = argc-1;
    task tsks[tsk_cnt];
    pthread_attr_t tattr;
    DEBUG("initing attr");
    if(pthread_attr_init(&tattr)) {
       EXITERR("attr_init");
    }

    DEBUG("creating threads");
    // for each thread get start
    for(int idx=0;idx<tsk_cnt;idx++){
        tsks[idx].tid = idx+1;
        tsks[idx].idx = idx;
        tsks[idx].params = argv[idx+1];
        if (pthread_create(&tsks[idx].tid, &tattr, task_run, &tsks[idx])) {
            EXITERR("create_thread");
        }
    }
    
    DEBUG("destroying attr");
    if(pthread_attr_destroy(&tattr)){
        EXITERR("destroy_attr");
    }

    DEBUG("joining threads");
    // for each thread get result
    for(int idx=0;idx<tsk_cnt;idx++){
        void * tsk_res;
        if(pthread_join(tsks[idx].tid, &tsk_res)){
            EXITERR("join_thread");
        }
        free(tsk_res);
        printf("%s->%d\n", tsks[idx].params, tsks[idx].result);
    }

    return 0;
}