struct semaphore{
  uint locked;       // Is the lock held?
  struct spinlock lk; // spinlock protecting this sleep lock
  struct proc *head;  
  int count;
};
