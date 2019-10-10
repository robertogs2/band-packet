#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <sched.h> /* For clone */
#include <signal.h> /* For SIGCHLD */
#include <sys/types.h> /* For pid_t */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* For getpid */
#include <time.h>
#include <string.h>

/* The maximum number of fibers that can be active at once. */
#define MAX_FIBERS 10
/* The size of the stack for each fiber. */
#define FIBER_STACK (1024*1024)

#define	LF_NOERROR	0
#define	LF_MAXFIBERS	1
#define LF_MALLOCERROR	2
#define LF_CLONEERROR	3
#define	LF_INFIBER	4
#define LF_SIGNALERROR	5


/* The LPthread Structure
*  Contains the information about individual lpthreads.
*/
typedef struct{
  pid_t pid; /* The pid of the child thread as returned by clone */
  void* stack; /* The stack pointer */
  char detached;
} lpthread_t;
typedef struct{
  int id;
} lpthread_attr_t;

typedef struct{
  char locked;
  pid_t pid;
} lpthread_mutex_t;
typedef struct{
  int id;
} lpthread_mutexattr_t;

// Public functions for use
int Lthread_create(lpthread_t* thread, const lpthread_attr_t* attr,
                   int (*start_routine)(void*), void* arg);			// Missing usage of attr
int Lthread_exit(lpthread_t thread);
int Lthread_yield();
int Lthread_join(lpthread_t thread, void **retval); 					// Missing usage of retval
int Lthread_detach(lpthread_t thread);

int Lmutex_init(lpthread_mutex_t* restrict mutex, const lpthread_mutexattr_t *restrict attr); // Missing usage of attr
int Lmutex_destroy(lpthread_mutex_t *mutex);
int Lmutex_unlock(lpthread_mutex_t *mutex);
int Lmutex_trylock(lpthread_mutex_t *mutex);
int Lmutex_lock(lpthread_mutex_t *mutex);

// Functions that should not be used
void initLPthreads();
int waitForAllLPthreads();
int find_empty_context();
void start_context();