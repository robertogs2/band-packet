#define _GNU_SOURCE
#include "lpthread.h"
#include <stdio.h>

#include <unistd.h>

lpthread_mutex_t mutex1;

int fiber1(void* arg){
    Lmutex_lock(&mutex1);
    printf("%s\n", "enter fiber1");
    int a = *((int*)arg);
    int i;
    for ( i = 0; 1; ++i ){
        printf( "Hey, I'm fiber #1: %d asdasd %d\n", i ,a);
        //fiberYield();
    }
    printf("%s\n", "end fiber1");
    Lmutex_unlock(&mutex1);
    return 0;
}

int fibonacchi(){
    Lmutex_lock(&mutex1);
    int i;
    int fib[2] = { 0, 1 };
    printf("%s\n", "enter fiber2");
    /*sleep( 2 ); */
    //printf( "fibonacchi(0) = 0\nfibonnachi(1) = 1\n" );
    for( i = 2; i < 10000000; ++ i )
    {
        int nextFib = fib[0] + fib[1];
        printf( "fibonacchi(%d) = %d\n", i, nextFib );
        fib[0] = fib[1];
        fib[1] = nextFib;
        //fiberYield();
    }
    printf("%s\n", "end fiber2");
    Lmutex_unlock(&mutex1);
    return 0;
}

int squares(){
    int i;
    //Lmutex_lock(&mutex1);
    /*sleep( 5 ); */
    printf("%s\n", "enter fiber3");
    for ( i = 0; i < 10000; ++ i ){
        printf( "%d*%d = %d\n", i, i, i*i );
        //fiberYield();
    }
    //Lmutex_unlock(&mutex1);
    printf("%s\n", "end fiber3");
    return 0;
}

int main(){
    Lmutex_init(&mutex1, NULL);

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
    //Lthread_join(thread, NULL);
    //spawnFiber( &squares );
    //Lthread_exit(thread3);
    // Lthread_detach(thread2);
    // Lthread_join(thread2, NULL);
    // Lthread_join(thread3, NULL);


    /* Since these are nonpre-emptive, we must allow them to run */
    //waitForAllLPthreads();
    
    /* The program quits */
    return 0;
}
