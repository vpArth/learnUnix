
Check:  
 - `#ifdef _POSIX_THREADS` for compile time
 - `sysconf(_SC_THREADS)` for runtime

####Identification  

```c

#include <pthread.h>
int pthread_equal(pthread_t tid1, pthread_t tid2); // compare thread ids
pthread_t pthread_self(void); // get self thread id   | getpid
```

####Create  
```c

#include <pthread.h>
int pthread_create(
  pthread_t *restrict tidp, 
  const pthread_attr_t *restrict attr, 
  void *(*start_rtn)(void), 
  void *restrict arg
); //    | fork
```
 - `tidp` - pointer to memory area for thread_id
 - `attr` - attributes or `NULL` for default
 - `start_rtn` - `void* foo(void *arg);` new thread start execution from this function
 - `arg` - pointer to `start_rtn` argument


####Stop thread  
 - return from `start_rtn`
 - `pthread_exit`
 - stop from other thread (PTHREAD_CANCELED)

```c
#include <pthread.h>
void pthread_exit(void *rval_ptr); // rval_ptr - returned value | exit
int pthread_join(pthread_t thread, void **rval_ptr); // wait for thread | waitpid
int pthread_cancel(pthread_t tid); // force stop     | abort
``` 

####Cleanup handlers  
```c
#include <pthread.h>
void pthread_cleanup_push(void ( *rtn)(void *), void *arg); //reg `void*rtn(void* arg);` handler
void pthread_cleanup_pop(int execute);  //  | atexit

int pthread_detach(pthread_t tid); // clear mem onexit, we can't join after this
```


####Sync  

#####Mutexes  
```c
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t * mutex);

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

//read-write locks
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

// Condition vars
int pthread_cond_init(pthread_cond_t *restrict cond, pthread_condattr_t *restrict attr);
int pthread_cond_destroy(pthread_cond_t *cond);

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict timeout);
struct timespec {
  time_t tv_sec; //sec
  long tv_nsec; //nanosec
};
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```




Deadlocks  
A -> r1  
A -> r1  

A -> r1    B -> r2
A -> r2    B -> r1
