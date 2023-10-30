// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "stat.h"
#include "user.h"

#define N  1000
int pass;
/*
void
printf(int fd, const char *s, ...)
{
  write(fd, s, strlen(s));
}
*/
void
forkexectest(void)
{
  int n, pid;

  for(n=0; n<N; n++){
    pid = fork();
    if(pid < 0)
    {
      if(n<61){
        printf(1, "fork failed\n");
        pass = 0;
      }
      break;
    }
    if(pid == 0){
      char *argv[2];
      argv[0] = "test";
      argv[1] = 0;
      exec("./test",argv);
      printf(1, "exec failed\n");
    }
  }

  if(n == N){
    pass = 0;
    printf(1, "fork claimed to work N times!\n", N);
    exit();
  }

  for(; n > 0; n--){
    if(wait() < 0){
      pass = 0;
      printf(1, "wait stopped early\n");
      exit();
    }
  }

  if(wait() != -1){
    pass = 0;
    printf(1, "wait got too many\n");
    exit();
  }

}

int
main(void)
{
  pass = 1;
  printf(1, "fork exec test\n");
  
  int i;
  for(i=0;i<1000;i++)
  {	
  	forkexectest();
  }

  if(pass == 1) printf(1, "fork exec test OK\n");
  else printf(1, "fork exec test failed\n");

  exit();
}
