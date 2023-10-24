#include "types.h"
#include "user.h"

int main(int args, char *argv[]){
	int pid = fork();

	if(pid > 0){
		printf(1,"parent: child=%d\n", pid);
		pid = wait();
		printf(1,"child %d is done\n",pid);
	} else if(pid == 0){
		printf(1,"child existing\n");
		exit();
	} else {
		printf(1,"fork error\n");
	}

	exit();
}
