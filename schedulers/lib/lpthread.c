#define _GNU_SOURCE

#include "../include/lpthread.h"

/* The lpthread "queue" */
static lpthread_t lpthreadList[MAX_FIBERS];
/* The pid of the parent process */
static pid_t parent_pid;
static pid_t group_pid;
/* The number of active lpthreads */
static int numLPthreads = 0;

static char started_ = 0;

int Lthread_create(lpthread_t* thread, const lpthread_attr_t *attr, int (*start_routine)(void*), void* arg){
	printf("%s\n", "Creating new thread");
	if(started_ == 0){
		init_threads();
		started_=1;
	}
	
	/*Allocate the stack*/
	thread->stack=malloc(FIBER_STACK);
	if(thread->stack==0){
		printf("Error: Could not allocate stack.\n");
		return LF_MALLOCERROR;
	}

	/* Call the clone system call to create the child thread */
	thread->pid = clone(start_routine, (char*) (thread->stack + FIBER_STACK),
						SIGCHLD|
						CLONE_FS|
						CLONE_FILES|
						CLONE_SIGHAND|
						CLONE_VM|
						CLONE_PARENT_SETTID|
						CLONE_CHILD_CLEARTID|
						CLONE_PTRACE, arg);
	//printf("Started thread with pid/tid %d\n", thread->pid);
	if(thread->pid==-1){
		free(thread->stack);
		printf("Error: clone system call failed.\n");
		return LF_CLONEERROR;
	}
	thread->detached=0;
	memcpy((void*)&lpthreadList[numLPthreads++], (void*)thread, sizeof(lpthread_t));
	return LF_NOERROR;
}
int Lthread_exit(lpthread_t thread){
	kill(thread.pid, SIGKILL);
	return 0;
}
int Lthread_yield(){
	/* Call the sched_yield system call which moves the current process to the
	end of the process queue. */
	sched_yield();
	return 0;
}
int Lthread_join(lpthread_t thread, void **retval){
	//printf("%s%p\n", "joining",thread);
	//int id = thread.pid;
	int index = map_pid_index(thread.pid);
	//printf("Thread to join id%d\n", id);
	// Stop the others threads

	if(lpthreadList[index].detached==0){
		// for(int i = 0; i < numLPthreads; ++i){
		// 	pid_t another_id = lpthreadList[i].pid;
		// 	if(another_id != id && another_id > 0){
		// 		printf("Stopping thread with id: %d\n", another_id);
		// 		kill(another_id, SIGSTOP);
		// 	}
		// }

		//printf("Joining: %d\n", thread.pid);
		waitpid(thread.pid, 0, 0);
		printf("%s\n", "done join");

		// for(int i = 0; i < numLPthreads; ++i){
		// 	pid_t another_id = lpthreadList[i].pid;
		// 	if(another_id != id && another_id > 0){
		// 		printf("Restarting thread with id: %d\n", another_id);
		// 		kill(another_id, SIGCONT);
		// 	}
		// }

		return 0;
	}
	else{
		return 1;
	}
}
int Lthread_detach(lpthread_t thread){
	int index = map_pid_index(thread.pid);
	lpthreadList[index].detached = 1;
	return 0;
}

int Lmutex_init(lpthread_mutex_t* restrict mutex, const lpthread_mutexattr_t *restrict attr){
	mutex->locked=0;
	mutex->pid=0;
	return 0;
}
int Lmutex_destroy(lpthread_mutex_t *mutex){
	mutex->locked = 0;
	mutex->pid = 0;
	return 0;
}
int Lmutex_unlock(lpthread_mutex_t *mutex){
	mutex->locked = 0;
	mutex->pid = 0;
	return 0;
}
int Lmutex_trylock(lpthread_mutex_t *mutex){
	if(mutex->locked==0){
		mutex->locked=1;
		mutex->pid = getpid();
		return 0;
	}
	return 1;
}
int Lmutex_lock(lpthread_mutex_t *mutex){
	LOOP: while(mutex->locked); // Race condition !!!!!!!!! Wait for mutex to unlock
	pid_t id = getpid();
	mutex->locked= 1;
	mutex->pid = id;
	if(mutex->pid != id){ // Method one to eliminate race condition
		goto LOOP;
	}
	return 0;
}

/* Initialize the lpthreads to null */
void Lthread_end(){
	killpg(getpgrp(), SIGKILL);
}
void init_threads(){
	for (int i = 0; i < MAX_FIBERS; ++ i){
		lpthreadList[i].pid = 0;
		lpthreadList[i].stack = 0;
	}
	signal(SIGINT, Lthread_end);
	atexit(Lthread_end);
	group_pid = getpgrp();
	parent_pid = getpid();

}
int map_pid_index(pid_t id){
	for(int i = 0; i < MAX_FIBERS; ++i){
		if(lpthreadList[i].pid == id){
			return i;
		}
	}
	return -1;
}
int wait_all(){
	printf("%s\n", "Calling wait");
	pid_t pid;
	int i;
	int lpthreadsRemaining = 0;
		
	/* Check to see if we are in a lpthread, since we don't get signals in the child threads */
	pid = getpid();
	if ( pid != parent_pid ) return LF_INFIBER;			
		
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
void sync_printf(char* format,...) { 
	static lpthread_mutex_t lock = {0,0};
	Lmutex_lock(&lock);
	printf("%s\n", "Locked");
	va_list arg; 
	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg); 
	Lmutex_unlock(&lock);
	printf("%s\n", "Unlocked");
}
char* convert(unsigned int num, int base) { 
	static char Representation[]= "0123456789ABCDEF";
	static char buffer[50]; 
	char *ptr; 
	
	ptr = &buffer[49]; 
	*ptr = '\0'; 
	
	do 
	{ 
		*--ptr = Representation[num%base]; 
		num /= base; 
	}while(num != 0); 
	
	return(ptr); 
}