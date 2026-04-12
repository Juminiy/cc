#include <unistd.h>
int main(){
    write(STDOUT_FILENO,"\033[01;31mHello,World\033[0m\n",25);
    return 0;
}