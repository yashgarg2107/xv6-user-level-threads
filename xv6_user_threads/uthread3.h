// header file for third part of assignment (Priority Scheduler)

#define true 1
#define false 0
#define boolean _Bool

/* Possible states of a thread; */
#define FREE        0
#define RUNNING     1
#define RUNNABLE    2
#define WAITING     3

#define STACK_SIZE  8192
#define MAX_THREAD  5       // need to take decisions regarding these

struct thread 
{
  int sp;                 // saved stack pointer
  char stack[STACK_SIZE]; // the threads stack
  int state;              // running, runnable, waiting, free(automatically initialised to 0)
  char *name;             // name of the thread
  int priority;
};


typedef struct thread thread_t, *thread_p;

struct mutex
{
  boolean acquired;
  thread_p queue[MAX_THREAD];
  int front,back,filled;
  thread_p curt;
  thread_p next;
};

typedef struct mutex mutex_t, *mutex_p;
  
static thread_t all_thread[MAX_THREAD];

thread_p current_thread;
thread_p next_thread;

extern void thread_switch(thread_p, thread_p);

int ageing_flag = 0;    // if 1 ageing will happen otherwise not 
int p_donation_flag=0;  // if 1 priority donation scheme will happen, otherwise not
int num_iter = 0;       // for priority inversion part - to show how many times scheduler is called
int print_iter_flag = 0;    // whether print no. of iterations or not

boolean 
thread_create(char* name, void *func, int priority)   // is void necessary here
{
  thread_p t;
  boolean free = false;

  if(priority<1 || priority>20)
  {
    printf(2,"Thread \'%s\' creation unsuccessful. Priority Invalid (valid values : 1-20) \n",name);
    return false;    // no space for threads available, max_threads limit reached
  }

  for (t = all_thread; t < all_thread + MAX_THREAD; t++) 
  {
    if(t->state == FREE) 
    {
      free=true;
      break;
    }
  }

  if(free==false)   
  {
    printf(2,"Thread \'%s\'creation unsuccessful. MAX_THREAD limit reached \n",name);
    return false;    // no space for threads available, max_threads limit reached
  }

  else
  {
    t->sp = (int) (t->stack + STACK_SIZE);   // set sp to the top of the stack
    t->sp -= 4;                              // space for return address
    * (int *) (t->sp) = (int)func;           // push return address on stack
    t->sp -= 32;                             // space for registers that thread_switch will push
    t->state = RUNNABLE;
    t->name = name;
    t->priority = priority;     // priority allowed only between 1 to 20
    printf(2,"Thread \'%s\' successfully created with p = %d\n",t->name,t->priority);
    return true;
  }
}



void 
age_priorities(void)
{
  thread_p t = all_thread;                       // at any thread switch ageing happens
  while(t < all_thread + MAX_THREAD)      
  {
    if (t->state == RUNNABLE || t->state == RUNNING)    
    {
      if(t->priority<20)
      t->priority+=1;    
    }
    t++;
  }
}


void 
thread_schedule(void)   // priority scheduling
{
  thread_p t;
  int maxp = 0;
  int curp = current_thread->priority;

  t = all_thread;
  while(t < all_thread + MAX_THREAD)      
  {
    if (t->state == RUNNABLE && t->priority > maxp)    
    {
        next_thread = t;
        maxp = t->priority;    
    }
    t++;
  }

  if(maxp==0)
  {
    printf(2, "thread_schedule: no more runnable threads;\n");
    if(print_iter_flag==1)
      printf(2,"Number of sheduler iterations = %d\n",num_iter);
    exit();
  }
  else if(maxp==curp)       // to treat same priority threads in round robin fashion
  {
    boolean found = false;

    t = current_thread;
    while(t < all_thread + MAX_THREAD)      
    {
      if (t->state == RUNNABLE && t->priority == maxp && t!=current_thread)    
      {
        next_thread = t;
        found = true;
        break;
      }
      t++;
    }

    if(found==false)
    {
      t = all_thread;
      while(t!=current_thread)
      {
        if (t->state == RUNNABLE && t->priority == maxp) 
        {
          next_thread = t;
          found = true;
          break;
        }
        t++;
      }
    }

    if(found==false)
      next_thread = t;

    next_thread->state = RUNNING;
    thread_switch(current_thread, next_thread);
  }
  else
  {
    next_thread->state = RUNNING;
    thread_switch(current_thread, next_thread);
  }

  if(ageing_flag==1)
    age_priorities();

  num_iter++;
}


void 
thread_yield(void)
{
  current_thread->state = RUNNABLE;
  thread_schedule();
}


void 
thread_init(void)
{
  current_thread = &all_thread[0];
  current_thread->state = RUNNING;
}


void
restore_priority(mutex_p lock)
{
  int p = lock->curt->priority;
  lock->curt->priority = lock->next->priority;
  lock->next->priority=p;
}

void 
swap_priorities(thread_p a, thread_p b)
{
  int p = a->priority;
  a->priority = b->priority;
  b->priority = p;
}



void
lock_busy_wait_acquire(mutex_p lock)
{
  while(lock->acquired == true)
  {
    if(p_donation_flag==1){
    if(current_thread->priority > lock->curt->priority)
    {
      if(lock->next!=0)
        restore_priority(lock);

      swap_priorities(current_thread, lock->curt);
      lock->next = current_thread;
    }}
    thread_yield();
  }
  lock->acquired = true;
  lock->curt=current_thread;
}

void
lock_busy_wait_release(mutex_p lock)
{
  lock->acquired = false;
}

void
lock_acquire(mutex_p lock)
{
  if(lock->acquired == false)
  {
    lock->acquired = true;
    lock->curt=current_thread;
  }
  else
  {
    current_thread->state = WAITING;
    lock->queue[(lock->back+1)%MAX_THREAD] = current_thread;
    lock->back = (lock->back+1)%MAX_THREAD;
    lock->filled++;
    thread_schedule();
  }
}

void
lock_release(mutex_p lock)
{
  lock->acquired = false;
  if(lock->filled != 0)
  {
    lock->queue[lock->front]->state = RUNNABLE;
    lock->front = (lock->front+1)%MAX_THREAD;
    lock->filled--;
  }
}


void
init_lock(mutex_p lock)
{
  lock->acquired=false;
  lock->filled=0;
  lock->back=-1;
  lock->front=0;
}


void 
thread_scheduler_init(void)   // to initialise locks so that lower process able to get lock
{
  thread_p t;
  boolean found = false;

  t = current_thread;
  while(t < all_thread + MAX_THREAD)        
  {
    if (t->state == RUNNABLE && t != current_thread)    
    {
      next_thread = t;
      found = true;
      break;
    }
    t++;
  }

  if(found==false)                          
  {
    t = all_thread;
    while(t!=current_thread)
    {
      if (t->state == RUNNABLE) 
      {
        next_thread = t;
        found = true;
        break;
      }
      t++;
    }
  }

  if(current_thread->state==RUNNABLE)       
    next_thread = t;

  if (next_thread == 0)                     
  {
    printf(2, "thread_schedule: no more runnable threads;\n");
    exit();
  }

  if (current_thread != next_thread)       
  {        
    next_thread->state = RUNNING;
    thread_switch(current_thread, next_thread);
  } 
  else
    next_thread = 0;

  num_iter++;
}