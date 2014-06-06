####Limits  
 - `PTHREAD_DESTRUCTOR_ITERATIONS`
 - `PTHREAD_KEYS_MAX`
 - `PTHREAD_STACK_MIN`
 - `PTHREAD_STACK_MAX`
 
####Thread Attributes

```c
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
```
 - `detachstate`
 - `guardsize` - guard against stack overflow
 - `stackaddr` - low address
 - `stacksize` - bytes

Detached:  

```c
#include <pthread.h>

int pthread_attr_getdetachstate(const pthread_attr_t *restrict attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate); // PTHREAD_CREATE_DETACHED/PTHREAD_CREATE_JOINABLE

int pthread_attr_getstack(const pthread_attr_t *restrict attr, void **restrict stackaddr, size_t *restrict stacksize); 
int pthread_attr_setstack(const pthread_attr_t *attr, void *stackaddr, size_t *stacksize); 

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

int pthread_attr_getguardsize(const pthread_attr_t *restrict attr, size_t *restrict guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);

int pthread_getconcurrency(void);
int pthread_setconcurrency(int level);

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr, int *restrict pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);

int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr, int *restrict type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr, int *restrict pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);

int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getpshared(const pthread_condattr_t *restrict attr, int *restrict pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

int pthread_key_create(pthread_key_t *keyp, void (* destructor)(void *));
int pthread_key_delete(pthread_key_t *key);

pthread_once_t initflag = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *initflag, void (*initfn)(void));

void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void * value);

int pthread_setcancelstate(int state, int * oldstate);
void pthread_testcancel(void);
int pthread_setcanceltype(int type, int * oldtype);

#include <signal.h>
int pthread_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
int sigwait(const sigset_t *restrict set, int *restrict signop);
int pthread_kill(pthread_t thread, int signo);

#include <pthread.h>
int pthread_atfork(void (* prepare)(void), void (* parent)(void), void (* child)(void));


```

