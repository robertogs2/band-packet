#define _GNU_SOURCE
#include <sched.h> /* For clone */

#include <stdio.h>

#include <signal.h> /* For SIGCHLD */
#include <stdlib.h>
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

//#include "lpthread.h"
/* The LPthread Structure
*  Contains the information about individual lpthreads.
*/
typedef struct{
	pid_t pid; /* The pid of the child thread as returned by clone */
	void* stack; /* The stack pointer */
} lpthread;

/* The lpthread "queue" */
static lpthread lpthreadList[MAX_FIBERS];
/* The pid of the parent process */
static pid_t parentPid;
/* The number of active lpthreads */
static int numLPthreads = 0;

/* Initialize the lpthreads to null */
void initLPthreads(){
	int i;
	for ( i = 0; i < MAX_FIBERS; ++ i ){
		lpthreadList[i].pid = 0;
		lpthreadList[i].stack = 0;
	}
	parentPid = getpid();
}

int map_pid_index(pid_t id){
	for(int i = 0; i < MAX_FIBERS; ++i){
		if(lpthreadList[i].pid == id){
			return i;
		}
	}
	return -1;
}

/* Call the sched_yield system call which moves the current process to the
end of the process queue. */
void Lthread_yield(){
	sched_yield();
}

/* Standard C99 does not permit void* to point to functions. This exists to
work around that in a standards compliant fashion. */
struct LPthreadArguments {
	void (*function)();
	void* arg;
};

/* Exists to give the proper function type to clone. */
static int lpthreadStart( void* fun ){
	struct LPthreadArguments* arguments = (struct LPthreadArguments*) fun;
	void (*function)() = arguments->function;
	void* arg = arguments->arg;
	free( arguments );
	arguments = NULL;

	printf( "Child created and calling function = %p\n", fun );
	function(arg);
	return 0;
}

int Lthread_create( void (*func)(void), void* arg){
	struct LPthreadArguments* arguments = NULL;
	if ( numLPthreads == MAX_FIBERS ) return LF_MAXFIBERS;

	/* Allocate the stack */
	lpthreadList[numLPthreads].stack = malloc( FIBER_STACK );
	if ( lpthreadList[numLPthreads].stack == 0 ){
		printf( "Error: Could not allocate stack." );
		return LF_MALLOCERROR;
	}

	/* Create the arguments structure. */
	arguments = (struct LPthreadArguments*) malloc( sizeof(*arguments) );
	if ( arguments == 0 ) {
		free( lpthreadList[numLPthreads].stack );
		printf( "Error: Could not allocate lpthread arguments." );
		return LF_MALLOCERROR;
	}
	arguments->function = func;
	arguments->arg = arg;

	/* Call the clone system call to create the child thread */
	lpthreadList[numLPthreads].pid = clone( &lpthreadStart, (char*) lpthreadList[numLPthreads].stack + FIBER_STACK,
		SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, arguments );
	printf("Started thread with pid/tid %d\n", lpthreadList[numLPthreads].pid);
	if ( lpthreadList[numLPthreads].pid == -1 ){
		free( lpthreadList[numLPthreads].stack );
		free( arguments );
		printf( "Error: clone system call failed." );
		return LF_CLONEERROR;
	}
	
	numLPthreads ++;
	
	return LF_NOERROR;
}

int waitForAllLPthreads(){
	printf("%s\n", "Calling wait");
	pid_t pid;
	int i;
	int lpthreadsRemaining = 0;
		
	/* Check to see if we are in a lpthread, since we don't get signals in the child threads */
	pid = getpid();
	if ( pid != parentPid ) return LF_INFIBER;			
		
	/* Wait for the lpthreads to quit, then free the stacks */
	while ( numLPthreads > lpthreadsRemaining ){
		pid = wait( 0 ); //key here
		if ( pid == -1 ){
			printf( "Error: wait system call failed.\n" );
			exit( 1 );
		}
		
		/* Find the lpthread, free the stack, and swap it with the last one */
		for ( i = 0; i < numLPthreads; ++ i ){
			if ( lpthreadList[i].pid == pid ){
				printf( "Child lpthread pid = %d exited\n", pid );
				numLPthreads --;
				
				free( lpthreadList[i].stack );
				if ( i != numLPthreads ){
					lpthreadList[i] = lpthreadList[numLPthreads];
				}
				
				i = -1;
				break;
			}
		}
		if ( i == numLPthreads ){
			printf( "Did not find child pid = %d in the lpthread list\n", pid ); 
		}
	}
	
	return LF_NOERROR;
}

