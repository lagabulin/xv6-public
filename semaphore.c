#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "semaphore.h"

void initsema(struct semaphore *lk, int count)
{
	initlock(&lk->lk, "semaphore");
	lk->count = count;
	lk->head = (struct proc*) 0;
}

int downsema(struct semaphore *lk)
{
	int ret;
	struct proc *my_proc = myproc();
	struct proc **curr;

	acquire(&lk->lk);

	my_proc->next = (struct proc*) 0;
	for(curr = &(lk->head); *curr; curr = &(*curr)->next)
		;

	*curr = my_proc;

	while(lk->count == 0 || lk->head != my_proc){
		sleep(&(my_proc->next), &lk->lk);
	}
	
	ret = --(lk->count);
	lk->head = lk->head->next;

	my_proc->next = (struct proc*) 0;
	release(&lk->lk);

	return ret;
}

int upsema(struct semaphore *lk)
{
	int ret;
	acquire(&lk->lk);
	ret = ++(lk->count);

	if(lk->head){
		wakeup(&(lk->head->next));
	}

	release(&lk->lk);
	return ret; 
}
