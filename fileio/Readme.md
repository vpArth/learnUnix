## Unbuffered IO

### Functions:
 - int open(const char *pathname, int oflag, ... /*mode_t mode*/);  
    Include lib `fcntl.h`  
    `pathname` - file to open name  
    `oflag` - followed flags OR combination  
     - `O_RDONLY`  
     - `O_WRONLY`  
     - `O_RDWR`  
     - `O_APPEND` - open in append mode  
     - `O_CREATE` - create if not exists; `mode` - access rights level  
     - `O_EXCL`   - exception if `O_CREAT` and file exists  
     - `O_TRUNC`  - clear existed file after open(w or rw)  
     - `O_NOCTTY` - no command terminal  
     - `O_NONBLOCK` - non block io mode  
     Synchronization:  
     - `O_SYNC`  - wait while physical write  
     - `O_DSYNC` - as `O_SYNC`, except attributes updating  
     - `O_RSYNC` - read ops wait for writes flushed  
    returns new file descriptor or -1 on error  
 - ssizet read(int fd, void* buf, size_t nbytes);  
    Include lib `unistd.h`  
    returns readbytes number/0 - EOF/-1 error
 - ssize_t write(int fd, const void * buf, size_t nbytes);  
    Include lib `unistd.h`  
    returns numofwrittenbytes or -1

 - off_t lseek(int fd, off_t offset, int whence);  
    Include lib `unistd.h`  
    `whence`  
      - SEEK_SET - offset counts from filestart  
      - SEEK_CUR - offset counts from current pos(can be negative)  
      - SEEK_END - offset counts from fileend(can be negative)  
    returns current cursor position or -1  
    ```c  
      offt cur;  
      cur = lseek(fd, 0, SEEK_CUR); // Get current position.  
    ```  
 - int close(int fd);  
    Include lib `unistd.h`  
    returns:  
      - 0 - success  
      - -1 - fail  

### Concurrent file access  

### Other:  
 - int creat(const char *pathname, mode_t mode);  
    Include lib `fcntl.h`  
    `open(pathname, O_WRONLY | O_CREATE | O_TRUNC, mode)`  
    returns new file descriptor or -1 on error  
 - ssize_t pread(int fd, void * buf, size_t nbytes, off_t offset);  
    Include lib `unistd.h`  
    like write, but does not change current position
 - ssize_t pwrite(int ав, const void * buf, size_t nbytes, off_t offset);  
    Include lib `unistd.h`  
    like write, but does not change current position
 - int dup(int fd);  
    Include lib `unistd.h`  
    get descriptor duplicate  
    fcntl(filedes, F_DUPFD, 0);
 - int dup2(int fd, int fd2);  
    Include lib `unistd.h`  
    get descriptor duplicate on fd2  
    close(fd2);fcntl(filedes, F_DUPFD, fd2);  
 - int fcntl(int fd, int cmd, .../*int arg*/)  
    Include lib `fcntl.h`  
    returns -1 on error
    cmd: 
    - `F_DUPFD`: duplicate descriptor
    - `F_GETFD`, `F_SETFD` - set/get descriptor flags (FD_CLOEXEC)
    - `F_GETFL`, `F_SETFL` - set/get file state flags
        - `O_RDONLY` - read
        - `O_WRONLY` - write
        - `O_RDWR` - read-write
        - `O_APPEND` - Append mode
        - `O_NONBLOCK` - Non-Block mode
        - `O_SYNC` - Sync writes(data and attrs)
        - `O_DSYNC` - Sync data writes
        - `O_RSYNC` - Sync read
        - `O_FSYNC` - Wait write finish(FreeBSD/MacOSX only)
        - `O_ASYNC` - async mode(FreeBSD/MacOSX only)
        Mask O_ACCMODE for first three flags
    - `F_GETOWN`, `F_SETOWN` - set/get owner for aio
      get pid which handles SIGURG/SIGIO signals now(if negative sets process group)  
    - `F_GETLK`, `F_SETLK`, `F_SETLKW` - get/set file record lock

 - void sync(void);  
    Include lib `unistd.h`  
    Push all changed buffers to write queue  
 - int fsync(int fd)  
    Include lib `unistd.h`  
    Push all changed buffers of file to write queue and wait it  
 - int fdatasync(int fd)  
    Include lib `unistd.h`  
    like fsync, except file attributes  
 - int ioctl(int fd, int request, ...);  
    Include lib `unistd.h`, `sys/ioctl.h`, `stropts.h`  

### Other remarks
 - folder `/dev/fd/` contans all opened file descriptors
 - `/dev/stdin` == `/dev/fd/0`, `/dev/stdout` == `/dev/fd/1`, `/dev/stderr` == `/dev/fd/2`
