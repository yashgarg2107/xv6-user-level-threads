// first test case for first part of assignment (User Level Threads)

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread1.h"

void t_func(void)
{
  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x\n", current_thread->name, (int) current_thread);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement to execute
  
  current_thread->state = FREE;   // mark space as free for further processes
  thread_schedule();
}


int main(int argc, char *argv[]) 
{
  thread_init();
  
  thread_create("T-1",t_func);
  thread_create("T-2",t_func);
  thread_create("T-3",t_func);
  thread_create("T-4",t_func);
  thread_create("T-5",t_func);

  thread_schedule();
  return 0;
}
