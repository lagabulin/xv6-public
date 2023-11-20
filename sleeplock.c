// Sleeping locks

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"

void
initsleeplock(struct sleeplock *lk, char *name)
{
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
  lk->head = 0;
}

void
acquiresleep(struct sleeplock *lk)
{
  struct proc *cur_proc; // list iterator
  struct proc *my_proc;
  
  acquire(&lk->lk);
  /*
	 1. Find the tail of the waiting list protected by the spinlock.
	 2. Push process itself to the tail
	 3. Sleep until lk->locked == 0 and lk->head == myproc()
	 4. After escaping, change the head to the next one.
	 
	 * Note that the 'next' field is process-wise, not lock-wise.
	 * If the head is changed at the end of the critical section(releasesleep),
	 * the holder(head) may enter an inner critical section protected by a sleeplock.
	 * Then its 'next' will be initialized to zero at the end of the inner critical section.
	 * At this moment, the 'next' which has pointed the next waiter will be lost. 
  */

  my_proc = myproc();
  my_proc->next = 0;
  
  if(lk->head == 0){
	lk->head = my_proc;
  }
  else{
	cur_proc = lk->head;
	while(cur_proc->next != 0){
	  cur_proc = cur_proc->next;
	}
	cur_proc->next = my_proc;
  }

  while (lk->locked) {
    sleep(my_proc, &lk->lk);
  }
  lk->head = lk->head->next;
  lk->locked = 1;
  lk->pid = myproc()->pid;
  
  release(&lk->lk);
}

void
releasesleep(struct sleeplock *lk)
{
  struct proc *my_proc;
  my_proc = myproc();

  acquire(&lk->lk);
  lk->locked = 0;
  lk->pid = 0;
  my_proc->next = 0;
  wakeup(lk->head);
  release(&lk->lk);
}

int
holdingsleep(struct sleeplock *lk)
{
  int r;
  
  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}



