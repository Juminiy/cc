#include <stdio.h>
#include <getopt.h>

int main(int argc, char **argv){
    char const * const short_opts = "pnVh";
    const struct option long_opts[] = {
        {"show-pids", no_argument, NULL, 'p'},
        {"numeric-sort", no_argument, NULL, 'n'},
        {"version", no_argument, NULL, 'V'},
        {"help", no_argument, NULL, 'h'}
    };

    #define HELP() \
        do { \
            fprintf(stdout, "pstree [-p|--show-pids] [-n|--numeric-sort] [--V|--version] [-h|--help]\n"); \
        } while(0)
    
    return 0;
}