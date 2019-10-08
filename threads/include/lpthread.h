#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

// Public functions for use
int Lthread_create(void (*func)(void), void* arg);
int Lthread_end();
int Lthread_yield();
int Lthread_join();
int Lthread_detach();
int Lmutex_init();
int Lmutex_destroy();
int Lmutex_unlock();
int Lmutex_trylock();

// Functions that should not be used 
void initLPthreads();
void waitForAllLPthreads();
int find_empty_context();
void start_context();