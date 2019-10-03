Lthread_create(void (*func)(void)){

    ucontext_t child, parent;
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
    makecontext( &child, &func, 0 );

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
