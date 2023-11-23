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

extern int peekchan(void* chan);

void
initsleeplock(struct sleeplock *lk, char *name)
{
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
  lk->head = 0;
}

int length(struct sleeplock *lk)
{
  if(lk->head == 0) return 0;
  int cnt = 0;
  struct proc *curr = lk->head;
  for(;curr != 0;curr = curr->next)
	  cnt++;
  return cnt;
}

void
acquiresleep(struct sleeplock *lk)
{
  struct proc *my_proc = myproc();
  struct proc **curr;

  acquire(&lk->lk);
	
  // Push(Similar to the idequeue push)
  int init_len = length(lk);
  my_proc->next = (struct proc*) 0;
  for(curr = &(lk->head); *curr; curr = &(*curr)->next)
	  ;

  /* debugging */
  if(curr == 0)
	  panic("Wrong push happens");
  /* end - debugging */

  *curr = my_proc;
  int second_len = length(lk);
  if(init_len != second_len - 1)
	  panic("Loss in the list");
  
  if(lk->locked || lk->head != my_proc){
	sleep(&(my_proc->next), &lk->lk);
  }

  /* debugging */
  if(lk->locked || lk->head != my_proc)
	  panic("Fake wakeup");
  /* end - debugging */

  lk->locked = 1;
  lk->pid = my_proc->pid;
  
  init_len = length(lk);
  for(curr = &(lk->head); *curr; curr = &(*curr)->next){
  	if(*curr == my_proc){
	  /* debuging */
	  if(*curr != lk->head)
		  panic("curr is not a head.");
	  /* end - debugging */
	  *curr = my_proc->next;
	  break;
	}
  }
  second_len = length(lk);

  if(init_len != second_len + 1)
	  panic("Loss in the list");
  /* debugging */
  if(lk->head != my_proc->next)
	  panic("Wrong pop happens");
  /* end - debbuging */

  my_proc->next = (struct proc*) 0;
  release(&lk->lk);
}

void
releasesleep(struct sleeplock *lk)
{
  acquire(&lk->lk);
  lk->pid = 0;
  lk->locked = 0;
  //wakeup(lk);
  
  if(lk->head){
	// debugging
	int cnt = peekchan(&(lk->head->next));
	if(cnt != 1){
		cprintf("more than 1 target for %s\n",lk->name);
	}
	if(&(lk->head->next) == 0)
		panic("no channel exists");
    // end - debugging
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



