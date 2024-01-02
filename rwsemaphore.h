struct rwsemaphore{
	struct spinlock lk;
	struct proc *reader_head;
	struct proc *writer_head;
	uint reading;
	uint writing;
	uint numFlush;
};
