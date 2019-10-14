#ifndef LPTHREAD 
#define LPTHREAD 

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
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h> // For call to gettid

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

int Lthread_create(lpthread_t* thread, const lpthread_attr_t *attr, int (*start_routine)(void*), void* arg);
int Lthread_exit(lpthread_t thread);
int Lthread_yield();
int Lthread_join(lpthread_t thread, void **retval);
int Lthread_detach(lpthread_t thread);

int Lmutex_init(lpthread_mutex_t* restrict mutex, const lpthread_mutexattr_t *restrict attr);
int Lmutex_destroy(lpthread_mutex_t *mutex);
int Lmutex_unlock(lpthread_mutex_t *mutex);
int Lmutex_trylock(lpthread_mutex_t *mutex);
int Lmutex_lock(lpthread_mutex_t *mutex);

/* Initialize the lpthreads to null */
void Lthread_end();
void init_threads();
int map_pid_index(pid_t id);
int waitForAllLPthreads();
void sync_printf(char* format,...);
char* convert(unsigned int num, int base);

#endif