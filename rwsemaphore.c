#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "rwsemaphore.h"

void initrwsema(struct rwsemaphore *lk)
{}

void downreadsema(struct rwsemaphore *lk)
{}

void upreadsema(struct rwsemaphore *lk)
{}

void downwritesema(struct rwsemaphore *lk)
{}

void upwritesema(struct rwsemaphore *lk)
{}
