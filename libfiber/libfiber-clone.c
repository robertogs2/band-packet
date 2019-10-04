#define _GNU_SOURCE
#include <sched.h> /* For clone */

#include <stdio.h>

#include <signal.h> /* For SIGCHLD */
#include <stdlib.h>
#include <sys/types.h> /* For pid_t */
#include <sys/wait.h> /* For wait */
#include <unistd.h> /* For getpid */
#include <time.h>

#include "libfiber.h"
/* The Fiber Structure
*  Contains the information about individual fibers.
*/
typedef struct{
	pid_t pid; /* The pid of the child thread as returned by clone */
	void* stack; /* The stack pointer */
} fiber;

/* The fiber "queue" */
static fiber fiberList[MAX_FIBERS];
/* The pid of the parent process */
static pid_t parentPid;
/* The number of active fibers */
static int numFibers = 0;

/* Initialize the fibers to null */
void initFibers()
{
	int i;
	for ( i = 0; i < MAX_FIBERS; ++ i )
	{
		fiberList[i].pid = 0;
		fiberList[i].stack = 0;
	}
	
	parentPid = getpid();
}

int map_pid_index(pid_t id){
	for(int i = 0; i < MAX_FIBERS; ++i){
		if(fiberList[i].pid == id){
			return i;
		}
	}
	return -1;
}

/* Call the sched_yield system call which moves the current process to the
end of the process queue. */
void fiberYield(){
	// pid_t id = getpid();
	// map_pid_index(id);
	// struct sched_param pp; 
	// sched_getparam(id, &pp);
	// //printf("%d\n", pp.__ss_un);
	// struct timespec wait_time = {0, 1000*500*2*2*2};
	// nanosleep(&wait_time, NULL);
	sched_yield();
}

/* Standard C99 does not permit void* to point to functions. This exists to
work around that in a standards compliant fashion. */
struct FiberArguments {
	void (*function)();
};

/* Exists to give the proper function type to clone. */
static int fiberStart( void* arg ){
	struct FiberArguments* arguments = (struct FiberArguments*) arg;
	void (*function)() = arguments->function;
	free( arguments );
	arguments = NULL;

	printf( "Child created and calling function = %p\n", arg );
	function();
	return 0;
}

int spawnFiber( void (*func)(void) ){
	struct FiberArguments* arguments = NULL;
	if ( numFibers == MAX_FIBERS ) return LF_MAXFIBERS;

	/* Allocate the stack */
	fiberList[numFibers].stack = malloc( FIBER_STACK );
	if ( fiberList[numFibers].stack == 0 ){
		printf( "Error: Could not allocate stack." );
		return LF_MALLOCERROR;
	}

	/* Create the arguments structure. */
	arguments = (struct FiberArguments*) malloc( sizeof(*arguments) );
	if ( arguments == 0 ) {
		free( fiberList[numFibers].stack );
		printf( "Error: Could not allocate fiber arguments." );
		return LF_MALLOCERROR;
	}
	arguments->function = func;

	/* Call the clone system call to create the child thread */
	fiberList[numFibers].pid = clone( &fiberStart, (char*) fiberList[numFibers].stack + FIBER_STACK,
		SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, arguments );
	printf("Started thread with pid/tid %d\n", fiberList[numFibers].pid);
	if ( fiberList[numFibers].pid == -1 ){
		free( fiberList[numFibers].stack );
		free( arguments );
		printf( "Error: clone system call failed." );
		return LF_CLONEERROR;
	}
	
	numFibers ++;
	
	return LF_NOERROR;
}

int waitForAllFibers(){
	printf("%s\n", "Calling wait");
	pid_t pid;
	int i;
	int fibersRemaining = 0;
		
	/* Check to see if we are in a fiber, since we don't get signals in the child threads */
	pid = getpid();
	if ( pid != parentPid ) return LF_INFIBER;			
		
	/* Wait for the fibers to quit, then free the stacks */
	while ( numFibers > fibersRemaining ){
		pid = wait( 0 ); //key here
		if ( pid == -1 ){
			printf( "Error: wait system call failed.\n" );
			exit( 1 );
		}
		
		/* Find the fiber, free the stack, and swap it with the last one */
		for ( i = 0; i < numFibers; ++ i ){
			if ( fiberList[i].pid == pid ){
				printf( "Child fiber pid = %d exited\n", pid );
				numFibers --;
				
				free( fiberList[i].stack );
				if ( i != numFibers ){
					fiberList[i] = fiberList[numFibers];
				}
				
				i = -1;
				break;
			}
		}
		if ( i == numFibers ){
			printf( "Did not find child pid = %d in the fiber list\n", pid ); 
		}
	}
	
	return LF_NOERROR;
}

