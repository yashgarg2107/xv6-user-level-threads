// second test case for second part of assignment (Synchronization)

#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread2.h"

mutex_p lock1;
mutex_p lock2;


void t_func1(void)
{

  lock_acquire(lock1);              // critical section begins

  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x\n", current_thread->name, (int) current_thread);
    thread_yield();                 // yielding in critical section
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement, frees position
  
  current_thread->state = FREE;

  lock_release(lock1);              // critical section ends
  thread_schedule();
}



void t_func2(void)
{
  lock_acquire(lock2);              // critical section begins

  int i;
  printf(1, "Thread \'%s\' started running\n",current_thread->name);
  
  for (i = 0; i < 10; i++) 
  {
    printf(1, "Thread \'%s\' at 0x%x\n", current_thread->name, (int) current_thread);
    thread_yield();                 // yielding in critical section
  }
  
  printf(1, "Thread \'%s\' finished: exit\n",current_thread->name);   // last statement, frees position
  
  current_thread->state = FREE;

  lock_release(lock2);              // critical section ends

  thread_schedule();
}


int main(int argc, char *argv[]) 
{
  thread_init();

  mutex_t m_1, m_2;

  lock1 = &m_1;
  lock2 = &m_2;

  init_lock(lock1);   // need to init locks in this case as queue is used
  init_lock(lock2);   // as so the queue position variables should have correct values

  thread_create("T-1",t_func1);
  thread_create("T-2",t_func1);
  thread_create("T-3",t_func2);
  thread_create("T-4",t_func2);

  thread_schedule();
  return 0;
}

