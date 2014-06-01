####Process lifetime  
 - `int main(int argc, char *argv[]);`  
 - end process  
    - `return` from main
    - `exit` call
    - `_exit` or `_Exit` call
    - return from last thread run function
    - `pthread_exit` from last thread
    -  `abort`
    -  signal receiving
    -  request for last thread finish

```c
#include <stdlib.h>
void exit(int status);
void _Exit(int status);

#include <unistd.h>
void _exit(int status);
```

```c
#include <stdlib.h>
int atexit(void (* func)(void));
```

`extern char **environ;`
`int main(int argc, char *argv[], char *envp[]);`

####Memory allocation  
```c
#include <stdlib.h>
void *malloc(size_t size);
void *calloc(size_t nobj, size_t size);
void *realloc(void *ptr, size_t newsize);
void free(void *ptr);
```

####Env  
```c
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str);
int setenv(const char *name, const char *value, int rewrite);
int unsetenv(const char *name);
```

####Long Goto  

```c
#include <setjmp.h>
int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);
```

longjmp does not rollback *static*, *global* and *volatile* declared variables  


####Resource limits  
```c
#include <sys/resource.h>
int getrlimit(int resource, struct rlimit *rlptr);
int setrlimit(int resource, const struct rlimit *rlptr);

struct rlimit {
rlim_t rlim_cur; /* soft limit: current limit */
rlim_t rlim_max; /* hard limit: max for rlim_cur */
//RLIM_INFINITY for infinity limit
};
```

`resouce` param:  
 - `RLIMIT_AS` - allowed memory in bytes.  
 - `RLIMIT_CORE` - core dump max size. (0 value disables core dumps)
 - `RLIMIT_CPU` - cpu time limit in seconds, on soft limit reaching process receives `SIGXFSZ` signal  
 - `RLIMIT_LOCKS` - file locks ma numbers  
 - `RLIMIT_MEMLOCK` - `mlock` function max capacity, bytes  
 - `RLIMIT_NOFILE` - open files number limit, sysconf(_SC_OPEN_MAX) changed  
 - `RLIMIT_NPROC` - child processes for uid limit, sysconf(_SC_CHILD_MAX)  
 - `RLIMIT_RSS` - virtual memory pages limit, bytes  
 - `RLIMIT_SBSIZE` - socket buffers limit, bytes  
 - `RLIMIT_STACK` - max stack size  
 - `RLIMIT_VMEM` - `RLIMIT_AS` alias  

shells have commands `ulimit`/`limit` for this  





