#define _GNU_SOURCE

#include "lpthread.h"

/* The lpthread "queue" */
static lpthread_t lpthreadList[MAX_FIBERS];
/* The pid of the parent process */
static pid_t parentPid;
/* The number of active lpthreads */
static int numLPthreads = 0;

static char started_ = 0;

/* Initialize the lpthreads to null */
void initLPthreads(){
	// for (int i = 0; i < MAX_FIBERS; ++ i){
	// 	lpthreadList[i]->pid = 0;
	// 	lpthreadList[i]->stack = 0;
	// }
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
int Lthread_yield(){
	sched_yield();
	return 0;
}

int Lthread_create(lpthread_t* thread, const lpthread_attr_t *attr,
					int (*start_routine)(void*), void* arg){

	if(started_ == 0){
		initLPthreads();
		started_=1;
	}
	

	/*Allocate the stack*/
	thread->stack=malloc(FIBER_STACK);
	if(thread->stack==0){
		printf("Error: Could not allocate stack.");
		return LF_MALLOCERROR;
	}

	/* Call the clone system call to create the child thread */
	thread->pid = clone( start_routine, (char*) (thread->stack + FIBER_STACK),
		SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM |CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, arg );

	printf("Started thread with pid/tid %d\n", thread->pid);
	if(thread->pid==-1){
		free(thread->stack);
		printf("Error: clone system call failed.");
		return LF_CLONEERROR;
	}
	memcpy((void*)&lpthreadList[numLPthreads++], (void*)thread, sizeof(lpthread_t));
	return LF_NOERROR;
}

int Lthread_join(lpthread_t thread, void **retval){
	//printf("%s%p\n", "joining",thread);
	int id = thread.pid;
	printf("Thread to join id%d\n", id);
	// Stop the others threads

	for(int i = 0; i < numLPthreads; ++i){
		pid_t another_id = lpthreadList[i].pid;
		if(another_id != id && another_id > 0){
			printf("Stopping thread with id: %d\n", another_id);
			kill(another_id, SIGSTOP);
		}
	}

	printf("Joining: %d\n", thread.pid);
	int res = waitpid(thread.pid, 0, 0);
	printf("%s\n", "done");

	for(int i = 0; i < numLPthreads; ++i){
		pid_t another_id = lpthreadList[i].pid;
		if(another_id != id && another_id > 0){
			printf("Restarting thread with id: %d\n", another_id);
			kill(another_id, SIGCONT);
		}
	}

	return 0;
}

int Lthread_detach(lpthread_t thread){
	int index = map_pid_index(thread.pid);
	lpthreadList[index].detached = 1;
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

