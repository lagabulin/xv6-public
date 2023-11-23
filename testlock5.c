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
  int pid, i;

  printf(1, "testlock5\n");

  pid = fork();
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }

  unsigned int x = (pid ? 1 : 97);
  for(i = 0; i < 100; i++){
    x = x * 1103515245 + 12345;
    if((x % 3) == 0){
      testlock();
	  testlock2();
	  peekwaiters();
	  peekwaiters2();
	  testlock2();
	  testlock();

	  
	  testlock();
	  testlock2();
	  peekwaiters();
	  peekwaiters2();
	  testlock2();
	  testlock();
    } else if((x % 3) == 1){
      testlock();
	  testlock2();
	  peekwaiters();
	  peekwaiters2();
	  testlock2();
	  testlock();
	} else {
      testlock();
	  testlock2();
	  peekwaiters();
	  peekwaiters2();
	  testlock2();
	  testlock();
	}
  }

  if(pid)
    wait();
  else
    exit();

  printf(1, "testlock5 ok\n");
  exit();
}
