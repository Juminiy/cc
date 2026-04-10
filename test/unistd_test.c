#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <wait.h>

#include "../libctr/ctr_util.h"

void test_access(char *filename) {
	int fok = access(filename, R_OK|W_OK|X_OK|F_OK);
	printf("%s : %0x\n", filename, fok);
	perror("access file");
}

int main(int argc, char *argv[], char *envp[]){

	if(argc<3){
		PANICF("lack argv[1,2]");
	}

	// brk();
	// sbrk();

	pid_t chpid = fork();
	if(chpid==0){
		chdir(argv[1]);

		char buf[256];
		printf("ch-workdir: %s\n",getcwd(buf,256));
		printf("ch-process uid:%u, gid:%u, loginname:%s\n", getuid(),getgid(),getlogin());

		char *ch_argv[]={argv[2],NULL};
		execve(argv[2], ch_argv, envp);
		perror("execve error");
	} else {
		waitpid(chpid,NULL,0);
		char buf[256];
		printf("pa-workdir: %s\n",getcwd(buf,256));
		printf("pa-process uid:%u, gid:%u, loginname:%s\n", getuid(),getgid(),getlogin());
	}


	return 0;
}