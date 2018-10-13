// first test case for third part of assignment (Priority Scheduler)

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread3.h"

void t_func(void)
{
  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x\n", current_thread->name, (int) current_thread);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement, frees position
  
  current_thread->state = FREE;
  thread_schedule();
}


int main(int argc, char *argv[]) 
{
  thread_init();

  thread_create("T-1",t_func,1);
  thread_create("T-2",t_func,6);
  thread_create("T-3",t_func,3);
  thread_create("T-4",t_func,25);
  thread_create("T-5",t_func,15);

  thread_schedule();
  return 0;
}

