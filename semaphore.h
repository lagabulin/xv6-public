struct semaphore{
  struct spinlock lk;
  struct proc *head;  
  int count;
};
