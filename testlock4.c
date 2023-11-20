#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	int i, pid;

	testlock();

	for (i = 0; i<61; i++){
		pid = fork();
		
		if(pid == 0){
		    if(10 <= i && i < 20){
				sleep(3000 - i*100);
				exit();
			}
			else
				while(1){}
		}
	}

	for (i = 0; i<10; i++) {
		int ret;
		if((ret = wait()) < 0){
			printf(1, "Process removal caused error\n");
		}
		pid = fork();
		if (pid) {
			printf(1, "process %d is created\n", i);
			sleep(100);
		}
		else{
			break;
		}
	}

	if (pid) {
		sleep(1000);
		testlock();
		for (i = 0; i<10; i++)
			wait();
	}
	else {
		testlock();
		printf(1, "%d have acquired lock\n", i);
		testlock();
	}

	exit();
}
