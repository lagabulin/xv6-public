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
  struct proc *my_proc = myproc();
  struct proc **curr;

  acquire(&lk->lk);
	
  // PUSH (Similar to the idequeue's push)
  my_proc->next = (struct proc*) 0;
  for(curr = &(lk->head); *curr; curr = &(*curr)->next)
	  ;

  *curr = my_proc;
  
  // (Holder OR Waiters) exist -> SLEEP
  if(lk->locked || lk->head != my_proc){
	sleep(&(my_proc->next), &lk->lk);
  }

  lk->locked = 1;
  lk->pid = my_proc->pid;
  lk->head = lk->head->next; // POP

  my_proc->next = (struct proc*) 0;
  release(&lk->lk);
}

void
releasesleep(struct sleeplock *lk)
{
  acquire(&lk->lk);
  lk->pid = 0;
  lk->locked = 0;
  
  if(lk->head){
	wakeup(&(lk->head->next));
  }
  
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



