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
	char detached; // Detached or not
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

/*
Lthread_create: Creates a new thread
lpthread_t* thread: Pointer to thread structure
const lpthread_attr_t* attr: pointer to thread attribute structure
const lpthread_attr_t* attr: pointer to thread attribute structure
int (*start_routine)(void*): Pointer to routine to start
void* arg: Argument for the routine
*/

int Lthread_create(lpthread_t* thread, const lpthread_attr_t *attr, int (*start_routine)(void*), void* arg);

/*
Lthread_exit: Kills a thread
lpthread_t thread: Thread structure
*/

int Lthread_exit(lpthread_t thread);

/*
Lthread_yield: Yields a thread
*/

int Lthread_yield();

/*
Lthread_join: Joins a thread and waits for it to finish
lpthread_t thread: Thread structure
void** retval: return value of the function
*/

int Lthread_join(lpthread_t thread, void **retval);

/*
Lthread_detachs: Detachs a thread and makes it unjoinable
lpthread_t thread: Thread structure
*/

int Lthread_detach(lpthread_t thread);

/*
Lmutex_init: Initializes a mutex strcuture
lpthread_mutex_t* restrict mutex: Pointer to the structure
const lpthread_mutexattr_t *restrict attr: Pointer to attribute 
*/

int Lmutex_init(lpthread_mutex_t* restrict mutex, const lpthread_mutexattr_t *restrict attr);

/*
Lmutex_destroy: Destroys a mutex strcuture
lpthread_mutex_t* mutex: Pointer to the structure
*/

int Lmutex_destroy(lpthread_mutex_t *mutex);

/*
Lmutex_unlock: Unlock a mutex strcuture
lpthread_mutex_t* mutex: Pointer to the structure
*/

int Lmutex_unlock(lpthread_mutex_t *mutex);

/*
Lmutex_trylock: Tries to lock a mutex strcuture
lpthread_mutex_t* mutex: Pointer to the structure
*/

int Lmutex_trylock(lpthread_mutex_t *mutex);

/*
Lmutex_lock: Locks a mutex strcuture
lpthread_mutex_t* mutex: Pointer to the structure
*/

int Lmutex_lock(lpthread_mutex_t *mutex);

/*
Lmutex_end: Ends a thread called from itself
*/

void Lthread_end();

/*
init_threads: Starts the threads system
*/

void init_threads();

/*
map_pid_index: Maps a pid to an index in the list
pid_t id: pid to search
*/

int map_pid_index(pid_t id);

/*
waitForAllLPthreads: Waits for all the pthreads to end
*/

int waitForAllLPthreads();

/*
sync_printf: Printf blocked with lpthread mutex
char* format: Format for the string
*/

void sync_printf(char* format,...);

/*
convert: Converts number to a base 
unsigned int num: number input
int base: base input
*/


char* convert(unsigned int num, int base);

#endif