#include "lpthread.h"
#include <stdio.h>

#include <unistd.h>

void fiber1()
{
    int i;
    for ( i = 0; i < 100; ++i )
    {
        printf( "Hey, I'm fiber #1: %d\n", i );
        //fiberYield();
    }
    return;
}

void fibonacchi()
{
    int i;
    int fib[2] = { 0, 1 };
    
    /*sleep( 2 ); */
    //printf( "fibonacchi(0) = 0\nfibonnachi(1) = 1\n" );
    for( i = 2; i < 100; ++ i )
    {
        int nextFib = fib[0] + fib[1];
        printf( "fibonacchi(%d) = %d\n", i, nextFib );
        fib[0] = fib[1];
        fib[1] = nextFib;
        //fiberYield();
    }
}

void squares()
{
    int i;
    
    /*sleep( 5 ); */
    for ( i = 0; i < 10000; ++ i )
    {
        printf( "%d*%d = %d\n", i, i, i*i );
        //fiberYield();
    }
}

int main()
{
    /* Initialize the fiber library */
    initLPthreads();
    
    /* Go fibers! */
    Lthread_create( &fiber1, NULL);
    Lthread_create( &fibonacchi , NULL);
    //spawnFiber( &squares );

    /* Since these are nonpre-emptive, we must allow them to run */
    waitForAllLPthreads();
    
    /* The program quits */
    return 0;
}
