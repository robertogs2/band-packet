#define _GNU_SOURCE
#include "lpthread.h"
#include <stdio.h>

#include <unistd.h>

int fiber1(void* arg){
    printf("%s\n", "enter");
    int a = *((int*)arg);
    int i;
    for ( i = 0; i < 100; ++i ){
        printf( "Hey, I'm fiber #1: %d asdasd %d\n", i ,a);
        //fiberYield();
    }
    printf("%s\n", "end");
    return 0;
}

int fibonacchi()
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
    return 0;
}

int squares(){
    int i;
    
    /*sleep( 5 ); */
    for ( i = 0; i < 100; ++ i ){
        printf( "%d*%d = %d\n", i, i, i*i );
        //fiberYield();
    }
    return 0;
}

int main(){
    
    int a = 10;
    void* ap = (void*)&a;
    // lpthread_t* thread = (lpthread_t*)malloc(sizeof(lpthread_t));
    // lpthread_t* thread2 = (lpthread_t*)malloc(sizeof(lpthread_t));
    // lpthread_t* thread3 = (lpthread_t*)malloc(sizeof(lpthread_t));

    lpthread_t thread;
    lpthread_t thread2;
    lpthread_t thread3;

    /* Go fibers! */
    Lthread_create(&thread, NULL, &fiber1, ap);
    Lthread_create(&thread2, NULL, &fibonacchi , NULL);
    Lthread_create(&thread3, NULL, &squares , NULL);
    
    //spawnFiber( &squares );
    Lthread_join(thread, NULL);
    Lthread_join(thread2, NULL);
    Lthread_join(thread3, NULL);

    /* Since these are nonpre-emptive, we must allow them to run */
    //waitForAllLPthreads();
    
    /* The program quits */
    return 0;
}
