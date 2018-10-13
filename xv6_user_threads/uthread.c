// fourth test case for third part of assignment (Priority Scheduler)

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread3.h"

mutex_p lock1;
mutex_p lock2;


void t_func1(void)
{
  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  lock_busy_wait_acquire(lock1);    // first critical section starts

  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x", current_thread->name, (int) current_thread);
    printf(1," with Priority = %d\n", current_thread->priority);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement, frees position
  
  current_thread->state = FREE;

  lock_busy_wait_release(lock1);    // first critical section ends

  if(p_donation_flag==1) 
    restore_priority(lock1);

  lock_busy_wait_acquire(lock2);    // second critical section starts

  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x", current_thread->name, (int) current_thread);
    printf(1," with Priority = %d\n", current_thread->priority);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   
  
  current_thread->state = FREE;

  lock_busy_wait_release(lock2);    // second critical section ends

  thread_schedule();
}




void t_func2(void)
{
  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  lock_busy_wait_acquire(lock1);        // first critical section starts

  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x", current_thread->name, (int) current_thread);
    printf(1," with Priority = %d\n", current_thread->priority);
    thread_yield();
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   
  
  current_thread->state = FREE;

  lock_busy_wait_release(lock1);        // first critical section ends

  if(p_donation_flag==1) 
    restore_priority(lock1);
  thread_schedule();
}


int main(int argc, char *argv[]) 
{
  thread_init();

  mutex_t m_1,m_2;
  
  lock1=&m_1;
  lock2=&m_2;

  init_lock(lock1);
  init_lock(lock2);

  print_iter_flag = 1;
  p_donation_flag=1;

  // thread_create("T-1",t_func2,1);
  // thread_create("T-2",t_func2,5);
  // thread_create("T-3",t_func2,20);

  thread_create("T-1",t_func1,1);
  thread_create("T-2",t_func2,5);
  thread_create("T-3",t_func2,20);


  thread_scheduler_init(); // single time schedule without priorities, so that 1 acquires lock (first one to be created)
  thread_schedule();
  return 0;
}
