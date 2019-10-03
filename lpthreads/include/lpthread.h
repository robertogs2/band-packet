#include <malloc.h>
#include <ucontext.h>
#include <stdio.h>
// 64kB stack
#define FIBER_STACK 1024*64
Lthread_create(void (*func)(void));
Lthread_end();
Lthread_yield();
Lthread_join();
Lthread_detach();
Lmutex_init();
Lmutex_destroy();
Lmutex_unlock();
Lmutex_trylock();