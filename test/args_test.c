#include <stdio.h>
#include <unistd.h>

#include <getopt.h>


int main(int argc, char **argv) {

    char const * const shrt_opts = "ac:f:";
    const struct option long_opts[] = 
    {
        {"aaa", no_argument, NULL, 'a'},
        {"ccc", required_argument, NULL, 'b'},
        {"fff", optional_argument, NULL, 'f'},
    };

    int optcase = 0;
    while((optcase = getopt_long(argc, argv, shrt_opts, long_opts, NULL)) != -1) 
    {
        switch (optcase) {
            case 'a': {
                fprintf(stdout, "[INFO] arg: aaa,a input\n");
                break;
            }

            case 'c': {
                fprintf(stdout, "[INFO] arg: ccc,c input, value: %s\n", optarg);
                break;
            }

            case 'f': {
                fprintf(stdout, "[INFO] arg: fff,f input, value: %s\n", optarg);
                break;
            }

            // case ':': {
            //     fprintf(stdout, "[INFO] option need a value\n");
            //     break;
            // }

            // case '?': {
            //     fprintf(stdout, "[INFO] unknown option: %c\n", optopt);
            //     break;
            // }

            default: {
                fprintf(stdout, "Usage:\n");
                fprintf(stdout, "Option: [-a] | [--aaa], Desc: if give some an item\n");
                fprintf(stdout, "Option: [-c] | [--ccc], Desc: give some an item name\n");
                fprintf(stdout, "Option: [-f] | [--fff], Desc: if give it again\n");
                break;
            }

        }
    }



    return 0;
}