#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
// 64kB stack
#define FIBER_STACK 1024*64
ucontext_t child, parent;


void threadFunction(void* arg){
	while(1){
		printf( "Child fiber yielding to parent %d\n", *(int*) arg);
		swapcontext( &child, &parent );
		printf( "Child thread exiting %d\n", *(int*) arg);
		swapcontext( &child, &parent );
	}
	
}

int Lthread_create(void (*func)(void), void* arg){


    // Get the current execution context
    getcontext( &child );

    // Modify the context to a new stack
    child.uc_link = 0;
    child.uc_stack.ss_sp = malloc( FIBER_STACK );
    child.uc_stack.ss_size = FIBER_STACK;
    child.uc_stack.ss_flags = 0;        
    if ( child.uc_stack.ss_sp == 0 ){
        perror( "malloc: Could not allocate stack" );
        exit( 1 );
    }

    // Create the new context
    printf( "Creating child fiber\n" );
    makecontext( &child, func, 1, arg);

    // Execute the child context
    printf( "Switching to child fiber\n" );
    swapcontext( &parent, &child );
    printf( "Switching to child fiber again\n" );
    swapcontext( &parent, &child );

    // Free the stack
    free( child.uc_stack.ss_sp );

    printf( "Child fiber returned and stack freed\n" );

    return 0;

}




int main(){
	int a = 4;
	void* ap = &a;
	Lthread_create((void (*)(void)) &threadFunction, ap);

	int b = 5;
	void* bp = &b;
	Lthread_create((void (*)(void)) &threadFunction, bp);
	return 0;
}