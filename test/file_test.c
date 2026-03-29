#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc < 3) {
        printf("Usage: <filename>:argv[1] <openmode>:argv[2]\n");
        return 1;
    }
    FILE *fd = fopen(argv[1], argv[2]);
    if(fd == NULL) {
        perror("Error opening file");
        return 1;
    }

    for(;;){}

    fclose(fd);
    return 0;
}