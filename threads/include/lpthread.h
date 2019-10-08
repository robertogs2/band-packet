#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <sched.h> /* For clone */
#include <signal.h> /* For SIGCHLD */
#include <sys/types.h> /* For pid_t */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* For getpid */
#include <time.h>

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
} lpthread_t;


// Public functions for use
int Lthread_create(void (*func)(void), void* arg);
int Lthread_end();
int Lthread_yield();
int Lthread_join();
int Lthread_detach();
int Lmutex_init();
int Lmutex_destroy();
int Lmutex_unlock();
int Lmutex_trylock();

// Functions that should not be used 
void initLPthreads();
int waitForAllLPthreads();
int find_empty_context();
void start_context();