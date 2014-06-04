
```c
#include <signal.h>
```

####Sources  
 - Terminal generated - ^C, ^D etc.
 - Hardware errors - Division by zero, access violations etc.
 - `kill` function - sends signal to any process or process group
 - `kill` shell command
 - software generated - some events

####Actions  
  - handle
  - ignore  / SIG_IGN
  - default / SIG_DFL

####Signals  
 
  name  | description | default action | remarks  
  --- | --- | --- | ---
  SIGABRT | abort | exit+core                          | abort() 
  SIGALRM | timer reached | exit                       | alarm(), setitimer()
  SIGHUP | disconnect | exit                           | can reaches bg processes
  SIGINFO  | tty status request | ignore               |
  SIGINT   | tty interrupt(^C) | exit                  |
  SYGTSTP  | tty stop | stop                           |
  SYGTTIN  | tty read from bg | stop                   |
  SIGTTOU  | tty write from bg | stop                  |
  SIGWINCH | tty resize | ignore                       |
  SIGQUIT  | tty quit (^\)| exit+core                  |
  SIGBUS  | hwerror | exit+core                        | 
  SIGEMT  | hwerror | exit+core                        |
  SYGTRAP | hwerror | exit+core                        |
  SIGIOT  | hwerror | exit+core                        |
  SIGFPE    | FPU error | exit+core                    | div 0, float overflow, etc.
  SIGSTKFLT | FPU stack error | exit                   | 
  SIGCHLD | change child status | ignore               |
  SIGCONT | resume process | resume/ignore             |
  SIGFREEZE | breakpoint | ignore                      |
  SIGILL | wrong instruction | exit+core               |
  SIGIO | async io | exit/ignore                       |
  SIGKILL | exit | exit                                |
  SIGPIPE | write to alone pipe | exit                 |
  SIGPOLL | `poll` event | exit                        |
  SIGPROF | profile `seti-timer` event | exit          |
  SIGPWR | powerdown/restart | exit/ignore             |
  SIGSEGV | mem access error | exit+core               |
  SIGSTOP | stop process | stop                        |
  SYGSYS | wrong syscall | exit+core                   |
  SYGTERM | terminate | exit                           |
  SYGTHAW | release breakpoint | ignore                |
  SIGURG | urgent event | ignore                       |
  SIGUSR1 | user event | exit                          |
  SIGUSR2 | user event | exit                          |
  SIGVTALRM | virtual `set-timer` event | exit         |
  SIGXCPU | cpu limit reached | exit+core/ignore       |
  SIGXFSZ | filesize limit reached | exit+core/ignore  |
  SIGXRES | resource limit reached | ignore            |
  SIGLWP     | for `threads` | ignore         |
  SIGWAITING | for `threads` | ignore         |
  SIGCANCEL  | for `threads` | ignore         |

```c
#include <signal.h>
//void (*signal(int signum, void ( *func)(int)))(int);
/*
#define SIG_ERR (void (*)()) 1
#define SIG_DFL (void (*)())0
#define SIG_IGN (void (*)())1
*/
   typedef void (*sighandler_t)(int);
   sighandler_t signal(int signum, sighandler_t handler);
   //returns prev handler or SIG_ERR
```
We can't to know current handler without change it with signal function  

Child process inherits handlers after `fork`  

####Poor signals(old):  
 - Signals lost sometimes:  
```c
int sig_int();
signal(SIGINT, sig_int); // set
sig_int()
{
  //next signal can be lost here!
  signal(SIGINT, sig_int); // set to wait next
```
 - Signals not able to temporary disable signals
```c
int sig_int_flag;
main()
{
  int sig_int();
  signal(SIGINT, sig_int); //set
  while (sig_int_flag == 0) {
    //if we receive signal here - we skip it :(
    pause(); //wait first signal
  }
}
sig_int()
{
  signal(SIGINT, sig_int); // set to wait next
  sig_int_flag = 1; set flag to check
}
```
"set to wait next" need, because while handler works handler temporary sets to SIG_DFL

####Interrupted system calls  

Slow system calls:  
 - read operation, which can lock forever, if no data(ttys, pipes, network devices)
 - write operation, as above, if data can't be sent immediately
 - open operation, which can wait some conditions(wait modem connection for ex.)
 - `pause` and `wait` functions
 - some `ioctl` operations
 - some crossprocess operations
temporary locks(disk io queueing) aren't slow operations  

```c
again:
  if ((n = read(fd, buf, BUFFSIZE)) < 0) {
    if (errno == EINTR) goto again; /* just interrupted syscall */
      /* other error */
  }
```
`ioctl`, `read`, `readv`, `write`, `writev`, `wait` and `waitpid` are auto rerunable  

####Reenterable functions  
We can't use any function in our sighandlers, because it can brake original workflow  
It: 
 - functions, which use some static data structures  
 - functions, which calls `malloc` or `free`  
 - functions from standard i/o library  

Here list of safe functions:  

    |     |     |     |   
--- | --- | --- | --- | ---  
accept | fchmod | lseek | sendto | stat
access | fchown | lstat | setgid | symlink
aio_error | fcntl | mkdir | setpgid | sysconf377
aio_return | fdatasync | mkfifo | setsid | tcdrain
aio_suspend | fork | open | setsockopt | tcflow
alarm | fpathconf | pathconf | setuid | tcflush
bind | fstat | pause | shutdown | tcgetattr
cfgetispeed | fsync | pipe | sigaction | tcgetpgrp
cfgetospeed | ftruncate | poll | sigaddset | tcsendbreak
cfsetispeed | getegid | posix_trace_event | sigdelset | tcsetattr
cfsetospeed | geteuid | pselect | sigemptyset | tcsetpgrp
chdir | getgid | raise | sigfillset | time
chmod | getgroups | read | sigismember | timer_getoverrun
chown | getpeername | readlink | signal | timer_gettime
clock_gettime | getpgrp | recv | sigpause | timer_settime
close | getpid | recvfrom | sigpending | times
connect | getppid | recvmsg | sigprocmask | umask
creat | getsockname | rename | sigqueue | uname
dup | getsockopt | rmdir | sigset | unlink
dup2 | getuid | select | sigsuspend | utime
execle | kill | sem_post | sleep | wait
execve | link | send | socket | waitpid
_Exit/_exit | listen | sendmsg | socketpair | write

Also we should save and restore `errno` value, if use one of functions, which can modify it  
Also we should block signals recieving on critical code parts  

  
```c
#include <signal.h>
int kill(pid_t pid, int signo);
int raise(int signo); // kill(getpid(), signo);
```
`pid`:  

 - `<-1` - to `pgid`==abs(`pid`)
 -  `0` - to `pgid`==getpid()
 - `>0` - to `pid`
 - `-1` - to all, which can

```c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
int pause(void);
```
####Signal Sets
```c
#include <signal.h>
int sigemptyset(sigset_t * set);
int sigfillset(sigset_t * set);
int sigaddset(sigset_t * set, int signo);
int sigdelset(sigset_t * set, int signo);
int sigismember(const sigset_t * set, int signo);
```

Signal Mask  
```c
#include <signal.h>
int sigprocmask(int how, const sigset_t * restrict set, sigset_t * restrict oset); // for one_threaded only
```
if `oset` is not null pointer it sets to current mask  
 `how`:
  - `SIG_BLOCK` - mask |= `set`
  - `SIG_UNBLOCK` - mask &- `set`
  - `SIG_SETMASK` - mask = `set`

```c
#include <signal.h>
int sigpending(sigset_t *set); 
// return set locked and triggered signals set
```

####sigaction  
```c
#include <signal.h>
int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);
struct sigaction {
  void (*sa_handler)(int); // handler
  sigset_t sa_mask; //additional locked signals(while handler works)
  int sa_flags; // flags
  void (*sa_sigaction)(int, siginfo_t *, void *);//alternative handler
};
```
flags:  
 - `SA_INTERRUPT` - don't autorecall interrupted syscall 
 - `SA_NOCLDSTOP` - ignore child stop/resumes
 - `SA_NOCLDWAIT` - no zombies, ignore child exits
 - `SA_NODEFER`   - no autolock self(sig) while handle
 - `SA_ONSTACK`   - On altern. stack delivery(`sigaltstack`)
 - `SA_RESETHAND` - while handle: set `SIG_DFL`, reset `SA_SIGINFO`
 - `SA_RESTART`   - autorecall interrupted syscall
 - `SA_SIGINFO`   - use `void handler(int signo, siginfo_t *info, void *context);`

```
struct siginfo {
  int si_signo;
  int si_errno;
  int si_code;
  pid_t si_pid;
  uid_t si_uid;
  void *si_addr;
  int si_status;
  long si_band;  /* SIGPOLL dir and prior */
};
```
`si_code`:
 - @todo - add...

```c
//signal through sigaction implementation
Sigfunc *signal(int signo, Sigfunc *func)
{
  struct sigaction act, oact;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
  } else {
#ifdef SA_RESTART
    act.sa_flags |= SA_RESTART;
#endif
  }
  if (sigaction(signo, &act, &oact) < 0)
    return(SIG_ERR);
    return(oact.sa_handler);
  }
```

```c
#include <setjmp.h>
    int sigsetjmp(sigjmp_buf env, int savemask);
    void siglongjmp(sigjmp_buf env, int val);
```

```c
#include <signal.h>
    int sigsuspend(const sigset_t *sigmask); //set signal mask and pause
```

```c
#include <stdlib.h>
void abort(void); // raise(SIGABRT) and always exit process
```

####System()
```c
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
int system(const char *cmdstring) /* предусматривает обработку сигналов */
{
  pid_t pid;
  int status;
  struct sigaction ignore, saveintr, savequit;
  sigset_t chldmask, savemask;

  if (cmdstring == NULL) return(1); /* UNIX всегда поддерживает командный процессор */
  
  ignore.sa_handler = SIG_IGN; /* игнорировать SIGINT и SIGQUIT */
  sigemptyset(&ignore.sa_mask);
  ignore.sa_flags = 0;
  
  if (sigaction(SIGINT, &ignore, &saveintr) < 0) return( 1);
  if (sigaction(SIGQUIT, &ignore, &savequit) < 0) return( 1);
  sigemptyset(&chldmask);
  /* заблокировать SIGCHLD */
  sigaddset(&chldmask, SIGCHLD);
  if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0) return( 1);
  
  if ((pid = fork()) < 0) {
    status = 1;
    /* вероятно, превышено максимальное количество процессов */
  } else if (pid == 0) {
    /* дочерний процесс */
    /* восстановить предыдущие действия сигналов и сбросить маску */
    sigaction(SIGINT, &saveintr, NULL);
    sigaction(SIGQUIT, &savequit, NULL);
    sigprocmask(SIG_SETMASK, &savemask, NULL);
    execl("/bin/sh", "sh", " c", cmdstring, (char *)0);
    _exit(127);
    /* ошибка вызова функции exec */
  } else {
    /* родительский процесс */
    while (waitpid(pid, &status, 0) < 0)
      if (errno != EINTR) {
        status = 1;
        /* получен код ошибки, отличный от EINTR */
        break;
      }
  }
  /* восстановить предыдущие действия сигналов и сбросить маску */
  if (sigaction(SIGINT, &saveintr, NULL) < 0) return( 1);
  if (sigaction(SIGQUIT, &savequit, NULL) < 0) return( 1);
  if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0) return( 1);
  return(status);
}
```

####sleep()

```c

    #include <unistd.h>
    unsigned int sleep(unsigned int seconds);// return 0 or seconds remaining, if interrupted by some signal
```

Sample implementation:  
```c

    static void sig_alrm(int signo) {}
    unsigned int sleep(unsigned int nsecs)
    {
      struct sigaction newact, oldact;
      sigset_t newmask, oldmask, suspmask;
      unsigned int unslept;
      /* установить свой обработчик, сохранив предыдущую информацию */
      newact.sa_handler = sig_alrm;
      sigemptyset(&newact.sa_mask);
      newact.sa_flags = 0;
      sigaction(SIGALRM, &newact, &oldact);
      
      /* заблокировать сигнал SIGALRM и сохранить текущую маску сигналов */
      sigemptyset(&newmask);
      sigaddset(&newmask, SIGALRM);
      sigprocmask(SIG_BLOCK, &newmask, &oldmask);
      
      alarm(nsecs);
      
      suspmask = oldmask;
      sigdelset(&suspmask, SIGALRM);/* убедиться, что SIGALRM не заблокирован */
      sigsuspend(&suspmask); /* дождаться, пока не будет перехвачен какой либо сигнал */
      /* был перехвачен некоторый сигнал, сейчас SIGALRM заблокирован */
      unslept = alarm(0);
      sigaction(SIGALRM, &oldact, NULL); /* восстановить предыдущее действие */
      /* восстановить маску сигналов, в которой сигнал SIGALRM разблокирован */
      sigprocmask(SIG_SETMASK, &oldmask, NULL);
      return(unslept);
    }
```

####Aux  
```c
extern char *sys_siglist[]; // global array with signal names
#include <signal.h>
void psignal(int signo, const char *msg); // "str: sig_decr\n" >> stderr
#include <string.h>
char *strsignal(int signo); // returns sig decription
```

Signal names:  
```c
#include <signal.h>
int sig2str(int signo, char *str);
int str2sig(const char *str, int *signop);
```
