#include <signal.h>
#include <termios.h>
#include <unistd.h>

int main(){

    char *__filepath="";
    char *__argv[]={"",NULL};
    char *__envp[]={"/usr/bin","/bin",NULL};

    execl(__filepath, __argv[0]);
    execle(__filepath, __argv[0]);
    execlp(__filepath, __argv[0], NULL, __envp);

    execv(__filepath, __argv);
    execvp(__filepath, __argv);
    execve(__filepath, __argv, __envp);

    return 0;
}