# Impelemented using given directions @ https://pdos.csail.mit.edu/6.828/2012/homework/xv6-uthread.html
#
#   void thread_switch(struct thread* current_thread, struct thread* next_thread)
#

.globl thread_switch

thread_switch:

    movl %esp, %ecx     # arguments stored in ecx // caller save registers

    movl 4(%ecx), %eax  # current thread address stored in eax
    movl 8(%ecx), %edx  # next thread address stored in edx

    pushal

    movl %esp, (%eax)   # current thread stack pointer points to esp register
    movl (%edx), %esp   # esp go to next thread sp, so that registers can be loaded from there
    
    movl %edx, current_thread  # making next thread the current thread
    
    # Load new callee-save registers
    # getting next thread registers

    popal

    movl $0, next_thread  # set next thread to 0 (given in problem statement : @ https://pdos.csail.mit.edu/6.828/2012/homework/xv6-uthread.html)

    ret
