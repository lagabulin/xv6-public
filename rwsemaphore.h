struct rwsemaphore
{
	struct spinlock lk;
	int sema_value;
};
