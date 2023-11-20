#include "types.h"
#include "user.h"
#include "fcntl.h"

void peekwaiters()
{
	int i;
	int size = 10;
	int waiter[10] = {0,};
	int len = peeklock(waiter, size);

	printf(1,"list:");
	for (i=0; i<len; i++){
		printf(1, "->%d", waiter[i]);

	}
	printf(1, "\n");
}

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
			peekwaiters();
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
		peekwaiters();
		testlock();
	}

	exit();
}
