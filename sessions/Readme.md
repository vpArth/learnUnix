

####Process groups  
```c
#include <unistd.h>
pid_t getpgrp(void);      //get my pgrp
pid_t getpgid(pid_t pid); //get pgrp by pid
// getpgid(0); === getpgrp();
int setpgid(pid_t pid, pid_t pgid); // add pid process to pgid group
 // if pid == pgid: pid process becomes a leader
 // if pid == 0: pid=calling_process_id
 // if pgid == 0: pgid=pid
```

Usually after fork, both parent and child calls setpgid(parent for child and child for self), this helps to be independent from what receive execution after fork first...  


####Sessions  
```sh
proc1 | proc2 &
proc3 | proc4 | proc5
```  
There are 3 process groups in one session:  
 - command shell
 - first group(proc1, proc2)
 - second group(3,4,5)

Session id = session leader pid = process group leader's pgid  

```c
#include <unistd.h>
pid_t setsid(void);
pid_t getsid(pid_t pid); //pid==0 means get my sid
```

`setsid`:
If process `A` is not a group leader, creates new session:
 - `A` becomes a session leader  
 - `A` becomes a new process group(with pgid=pid) leader
 - `A` losts control terminal
Else `setsid` fails

####Control Terminal(CT)  
 - Session can has only one `CT`
 - Session leader, connected to `CT` - Control Process
 - Session groups: one foreground and any background groups(leader can be in bg group)
 - Commands like ^C received by foreground group processes
 - Disconnect signals received by session leader
When std streams redirected, we can link with terminal with `open` `/dev/tty`  

```c
#include <unistd.h>
pid_t tcgetpgrp(int fd); //foreground pgid for fd terminal
int tcsetpgrp(int fd, pid_t pgrpid); //send pgrpid group to foreground

#include <termios.h>
pid_t tcgetsid(int fd); //get session leader pgid(equals to session_id)
```

####Jobs(not Stieve)
 - `vi main.c` - one foreground job
 - `(pr *.c | lpr &) || make all &` - two background jobs




