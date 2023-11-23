/*
To look for deadlock
*/
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
void peekwaiters2()
{
	int i;
	int size = 10;
	int waiter[10] = {0,};
	int len = peeklock2(waiter, size);

	printf(1,"list2:");
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
		int j =0;
		for(j= 0;j<10;j++){
		  testlock();
		  peekwaiters();
		  testlock();
		  testlock2();
		  peekwaiters2();
		 // sleep(10);
		  testlock2();
		}

		testlock();
		printf(1, "%d have acquired lock2\n", i);
		testlock();


	}

	exit();
}
