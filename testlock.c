#include "types.h"
#include "user.h"
#include "fcntl.h"

int
main()
{
	int i, pid;

	testlock();

	for (i = 0; i<10; i++) {
		pid = fork();
		if (pid) {
			printf(1, "process %d is created\n", i);
			sleep(100);
		}
		else{
			sleep((10-i)*300);
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
