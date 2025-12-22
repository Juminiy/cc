#include <stdio.h>
#include <stdlib.h>

// nested functions
int main(int argc, char **argv) {
    int tot=0;

    void fn0(char *_argv1) {
        tot = strtol(_argv1, NULL, 10);   
    };

    if(argc>1)
        fn0(argv[1]);
    
    printf("%d\n", tot);

    return 0;
}