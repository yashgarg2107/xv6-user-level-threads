// third test case for third part of assignment (Priority Scheduler)

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread3.h"

mutex_p lock1;

void t_func(void)
{
  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  lock_busy_wait_acquire(lock1);

  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x\n", current_thread->name, (int) current_thread);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement, frees position
  
  current_thread->state = FREE;

  lock_busy_wait_release(lock1); 
  thread_schedule();
}


int main(int argc, char *argv[]) 
{
  thread_init();

  mutex_t m_1;
  lock1=&m_1;

  ageing_flag = 1;
  print_iter_flag = 1;

  init_lock(lock1);

  thread_create("T-1",t_func,1);
  thread_create("T-2",t_func,2);
  thread_create("T-3",t_func,20);

  thread_scheduler_init(); // single time schedule without priorities, so that 1 acquires lock (first one to be created)
  thread_schedule();

  return 0;
}


