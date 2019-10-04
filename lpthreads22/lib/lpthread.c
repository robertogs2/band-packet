int Lthread_create(void (*func)(void), void* arg){

    // Initilize pool if no thread was created
    if (lp_init == LP_NOT_STARTED){
        lp_init = LP_STARTED;
        getcontext(&main_context); // Stores the first main context
        for(int i = 0; i < CONTEXTS_AMOUNT; ++i){
            lpthread_pool.in_use[i] = LP_NOT_IN_USE;
        }
    }

    // Finds a context not in use
    int k = find_empty_context();
    if(k==-1){ //All in use
        perror("All contexts in use");
    }

    // Get the calling context
    getcontext(&lpthread_pool.contexts[k]);

    // Modify the context to a new stack
    lpthread_pool.contexts[k].uc_link = 0;
    lpthread_pool.contexts[k].uc_stack.ss_sp = malloc( FIBER_STACK );
    lpthread_pool.contexts[k].uc_stack.ss_size = FIBER_STACK;
    lpthread_pool.contexts[k].uc_stack.ss_flags = 0;        
    if (lpthread_pool.contexts[k].uc_stack.ss_sp == 0){
        perror( "malloc: Could not allocate stack" );
        return LP_CREATE_ERROR;
        //exit( 1 );
    }

    // Create the new context
    printf("Creating child in context %d\n", k);

    // Generates the context with the start function with the function pointer, arguments, and id
    makecontext(&lpthread_pool.contexts[k], (void (*)(void)) &start_context, 3, func, arg, k);
    printf("%s\n", "Created child");

    // // Execute the child context
    // printf( "Switching to child fiber\n" );
    // swapcontext( &parent, &child );
    // printf( "Switching to child fiber again\n" );
    // swapcontext( &parent, &child );

    // // Free the stack
    // free( child.uc_stack.ss_sp );

    // printf( "Child fiber returned and stack freed\n" );

    return k;

}

// Find an empty context
int find_empty_context(){
    int k = -1;
    for(int i = 0; i < CONTEXTS_AMOUNT; ++i){
        if(lpthread_pool.in_use[i] == LP_NOT_IN_USE){ // Find a context not in use
            k = i;
            break;
        }
    }
}

void start_context(void (*func)(void), void* arg, int k){
    lpthread_pool[k].in_use = LP_IN_USE;
    func();
    fiberList[currentFiber].active = LP_NOT_IN_USE;
    
    /* Yield control, but because active == 0, this will free the fiber */
    //fiberYield();
}