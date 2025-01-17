	.text

/* Switch from current_thread to next_thread. Make next_thread
 * the current_thread, and set next_thread to 0.
 * Use eax as a temporary register; it is caller saved.
 */
	.globl thread_switch
thread_switch:
	/* YOUR CODE HERE */
	pushal
	movl current_thread, %eax
	movl %esp, (%eax)

	movl next_thread, %eax
	movl (%eax), %esp
	movl %eax, current_thread
	movl $0x0, next_thread
	
	popal
	ret				/* pop return address from stack */

	.globl usertrapret
usertrapret:
	popal
	popl %gs
	popl %fs
	popl %es
	popl %ds
	addl $0x8, %esp
	iret

	.globl new_scheduler
new_scheduler:
	push %eax
	call thread_yield
	pop %eax
	ret
