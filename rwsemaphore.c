#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "rwsemaphore.h"

void initrwsema(struct rwsemaphore *lk)
{
	initlock(&lk->lk, "rwsemaphore");
	lk->reader_head = (struct proc*) 0;
	lk->writer_head = (struct proc*) 0;
	lk->reading = 0;
	lk->writing = 0;
	lk->numFlush = 0;
}

void downreadsema(struct rwsemaphore *lk)
{
	struct proc *my_proc = myproc();
	struct proc **curr;

	acquire(&lk->lk);

	my_proc->next = (struct proc*) 0;
	for(curr = &(lk->reader_head); *curr; curr = &(*curr)->next)
		;

	*curr = my_proc;
	
	while(lk->reader_head != my_proc || lk->writing != 0 || lk->writer_head != 0){
		sleep(&(my_proc->next), &lk->lk);
		if(lk->numFlush != 0){
			lk->numFlush--;
			break;
		}
	}

	lk->reading++;
	lk->reader_head = lk->reader_head->next;

	if(lk->reader_head != 0){
		wakeup(&(lk->reader_head->next));
	}

	my_proc->next = (struct proc*) 0;
	release(&lk->lk);
}

void upreadsema(struct rwsemaphore *lk)
{
	acquire(&lk->lk);

	lk->reading--;
	if(lk->reading == 0 && lk->numFlush == 0 && lk->writer_head != 0){
		wakeup(&(lk->writer_head->next));
	} 

	release(&lk->lk);
}

void downwritesema(struct rwsemaphore *lk)
{
	struct proc *my_proc = myproc();
	struct proc **curr;

	acquire(&lk->lk);

	my_proc->next = (struct proc*) 0;
	for(curr = &(lk->writer_head); *curr; curr = &(*curr)->next)
		;

	*curr = my_proc;
	
	while(lk->writer_head != my_proc || lk->reading != 0 || lk->writing != 0){
		sleep(&(my_proc->next), &lk->lk);
	}

	lk->writing = 1;
	lk->writer_head = lk->writer_head->next;

	my_proc->next = (struct proc*) 0;
	release(&lk->lk);
}

void upwritesema(struct rwsemaphore *lk)
{
	acquire(&lk->lk);
	
	lk->writing = 0;
	
	if(lk->reader_head != 0){
		struct proc **curr;	
		for(curr = &(lk->reader_head); *curr; curr = &(*curr)->next){
			lk->numFlush++;
		}
		wakeup(&(lk->reader_head->next));
	} 
	else if (lk->writer_head != 0) {
		wakeup(&(lk->writer_head->next));
	} 

	release(&lk->lk);
}
