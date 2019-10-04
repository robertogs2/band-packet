#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

#include <lpflags.h>

struct lpthread_pool{
	ucontext_t contexts[CONTEXTS_AMOUNT];
	int in_use[CONTEXTS_AMOUNT];
};
static ucontext_t lp_main_context;
static struct lpthread_pool thread_pool; 
static lp_init = 0;

// Public functions for use
int Lthread_create(void (*func)(void));
int Lthread_end();
int Lthread_yield();
int Lthread_join();
int Lthread_detach();
int Lmutex_init();
int Lmutex_destroy();
int Lmutex_unlock();
int Lmutex_trylock();

// Functions that should not be used 

int find_empty_context();
void start_context();