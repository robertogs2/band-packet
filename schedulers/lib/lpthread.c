#define _GNU_SOURCE

#include "../include/lpthread.h"

// List of threads
static lpthread_t lpthreadList[MAX_FIBERS];
// Required pids
static pid_t parent_pid;
static pid_t group_pid;
// Counter for threads
static int numLPthreads = 0;
// Variable to check if it already started
static char started_ = 0;

int Lthread_create(lpthread_t* thread, const lpthread_attr_t *attr, int (*start_routine)(void*), void* arg){
	if(started_ == 0){ // Singleton for the system
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
	if(thread->pid==-1){ // Error in clone
		free(thread->stack);
		printf("Error: clone system call failed.\n");
		return LF_CLONEERROR;
	}
	// Copies thread to list
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
	int index = map_pid_index(thread.pid);

	if(lpthreadList[index].detached==0){
		waitpid(thread.pid, 0, 0); // Key is here, wait for it to end
		printf("%s\n", "done join");

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
	mutex->locked=0; // Set the mutex as unlocked
	mutex->pid=0;
	return 0;
}
int Lmutex_destroy(lpthread_mutex_t *mutex){
	mutex->locked = 0; // Set the mutex as unlocked
	mutex->pid = 0;
	return 0;
}
int Lmutex_unlock(lpthread_mutex_t *mutex){
	mutex->locked = 0; // Set the mutex as unlocked
	mutex->pid = 0;
	return 0;
}
int Lmutex_trylock(lpthread_mutex_t *mutex){
	if(mutex->locked==0){ // If mutex is not locked, lock it
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

void Lthread_end(){
	// Kills the thread
	killpg(getpgrp(), SIGKILL);
}
void init_threads(){
	// Initialices
	for (int i = 0; i < MAX_FIBERS; ++ i){
		lpthreadList[i].pid = 0;
		lpthreadList[i].stack = 0;
	}
	// Signal from terminal
	signal(SIGINT, Lthread_end);
	atexit(Lthread_end); // When parent ends
	// Sets the required pids
	group_pid = getpgrp();
	parent_pid = getpid();

}
int map_pid_index(pid_t id){
	// Search for that pid
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