#include <unistd.h>
void _start() {
    syscall(1,1,"\033[01;31mHello,World\033[0m\n",25);
    syscall(60,0);
}