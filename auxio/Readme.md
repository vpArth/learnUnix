####Non block operations
Slow operations(which can "always lock"):  
 - read, when no data
 - write, when cannot write immediately
 - open, while any conditions matched
 - file read/writes, with record locks
 - any `ioctl` operations
 - any crossprocess functions

`O_NONBLOCK` mode for `open`/`read`/`write` cause immediately error instead lock
 flag `O_NONBLOCK` can be set up:
 - in `open` operations
 - `fcntl`

see `cat.c`...

####Records locks
 Record is a bytes range in a file  
Lock forms:  
System | Recomend | Force | fcntl | lockf | flock
---|---|---|---|---|---
SUS     | + | - | + | XSI | -
FreeBSD | + | - | + | + | +
Linux   | + | + | + | + | +
MacOSX  | + | - | + | + | +
Solaris | + | + | + | + | +

`fcntl`-based locks  
```c
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* struct flock *flockptr */ );
struct flock {
  short l_type; // F_RDLCK/F_WRLCK/F_UNLCK
  off_t l_start; // start offset from whence in bytes
  short l_whence; // SEEK_SET/SEEK_CUR/SEEK_END
  off_t l_len; // area length, bytes (0 means to eof)
  pid_t l_pid; //for F_GETLK, lock owner's pid
};
```
`cmd`: 
 - `F_GETLK` - get current lock; non atomic!
 - `F_SETLK` - errno = EAGAIN/EACCESS, if fails
 - `F_SETLKW` - wait if fails

Locks related to process and file  



####STREAM
####IO Multiplex
####Async IO
####Functions readv/writev
####Functions readn/writen
####Mapped memory IO operations

