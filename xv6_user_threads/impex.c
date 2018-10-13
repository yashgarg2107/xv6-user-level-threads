#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread.h"

extern void thread_switch(thread_p, thread_p);
static thread_t all_thread[MAX_THREAD];

thread_p  current_thread;
thread_p  next_thread;


bool 
thread_create(const char* name, void (*func)())
{
  thread_p t;
  int free = 0;

  for (t = all_thread; t < all_thread + MAX_THREAD; t++) 
  {
    if (t->state == FREE) 
    	break;
    free+=1;
  }

  if(free==4)
  	return false;

  else
  {
	  t->sp = (int) (t->stack + STACK_SIZE);   // set sp to the top of the stack
	  t->sp -= 4;                              // space for return address
	  * (int *) (t->sp) = (int)func;   // check for int or general        // push return address on stack
	  t->sp -= 32;                             // space for registers that thread_switch will push
	  t->state = RUNNABLE;
	  return true;
  }
}


static void
thread_schedule(void)
{
	thread_p t;
	// Find another runnable thread.
	for (t=all_thread; t<all_thread + MAX_THREAD; t++) 
	{
		if(t->state == RUNNABLE && t!= current_thread) 
		{
			next_thread = t ;
			break ;
		}
	}

	if (current_thread != next_thread) 
  	{         /* switch threads?  */
    	next_thread->state = RUNNING;
    	thread_switch(current_thread, next_thread);
  	} 
	// Some other stuff
}

void 
thread_yield(void)
{
  current_thread->state = RUNNABLE;
  thread_schedule();
}

static void 
mythread(void)
{
  int i;
  printf(1, "my thread running\n");
  for (i = 0; i < 100; i++) 
  {
    printf(1, "my thread 0x%x\n", (int) current_thread);
    thread_yield();
  }
  
  printf(1, "my thread: exit\n");
  current_thread->state = FREE;
  thread_schedule();
}

int 
main(int argc, char *argv[]) 
{
  // thread_init();
  thread_create("1",mythread);
  thread_create("2",mythread);
  // thread_create(mythread);
  thread_schedule();
  return 0;
}
