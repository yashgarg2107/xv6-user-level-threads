

#    .text
#
# void swtch(struct context **old, struct context *new);
#
# Switch from current_thread to next_thread. Make next_thread
# the current_thread, and set next_thread to 0.
# Use eax as a temporary register, which should be caller saved.
#
.globl thread_switch
thread_switch:
    # /* YOUR CODE HERE */

  movl 4(%esp), %eax
  movl 8(%esp), %edx

  # Save old callee-save registers
  pushl %ebp
  pushl %ebx
  pushl %esi
  pushl %edi

  # Switch stacks
  movl %esp, (%eax)
  movl %edx, %esp

  # Load new callee-save registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp

  ret               #/* pop return address from stack */