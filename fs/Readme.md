
```c
#include <sys/stat.h>
int stat(const char *restrict pathname, struct stat *restrict buf);
int fstat(int fd, struct stat * buf);
int lstat(const char *restrict pathname, struct stat restrict buf);
```

`stat` get stat struct of pathname file  
`fstat` get stat struct of fd file  
`lstat` get stat struct of fd file(about symlink also)  

```c
  struct stat {
    mode_t st_mode; /* file type and access rights */
    ino_t st_ino;   /* inode */
    dev_t st_dev;   /* device(fs) (major(st_rdev), minor(st_rdev))*/
    dev_t st_rdev;  /* device for spec files (major(st_rdev), minor(st_rdev))*/
    nlink_t st_nlink; /* symlinks number */
    uid_t st_uid;    /* owner uid */
    gid_t st_gid;    /* owner gid */
    off_t st_size;   /* size in bytes */
    time_t st_atime; /* last access */
    time_t st_mtime; /* last modify */
    time_t st_ctime; /* last state flags modify */
    blksize_t st_blksize; /* optimal io blocksize */
    blkcnt_t st_blocks; /* occupied drive blocks number */
  }
```

### st_mode
 `st_mode & S_IFMT`  
 type: macros(st_mode) from sys/stat.h
 - Regular: S_ISREG()
 - Directory: S_ISDIR()
 - Block file: S_ISBLK()
 - Character(tty): S_ISCHR()
 - FIFO (named channel): S_ISFIFO
 - Socket: S_ISSOCK()
 - Symlink: S_ISLNK()  

 - MQueue: S_TYPEISMQ()
 - Semaphor: S_TYPEISSEM()
 - Shared Memory: S_TYPEISSHM()
Macro example:  
`#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)`  

`set-uid`: `st_mode & S_ISUID`  
`set-gid`: `st_mode & S_ISGID`  

Access rights:
  `S_IRUSR`, `S_IWUSR`, `S_IXUSR`: `S_IRWXU`  
  `S_IRGRP`, `S_IWGRP`, `S_IXGRP`: `S_IRWXG`  
  `S_IROTH`, `S_IWOTH`, `S_IXOTH`: `S_IRWXO`  

Test access rights of real uid/gid(not effective)  
```c
#include <unistd.h>
int access(const char * pathname, int mode);
```
`mode`: `R_OK`, `W_OK`, `X_OK`, `F_OK`(test file_exists)  

Set mask of create file mode for process, get previous:  
```c
#include <sys/stat.h>
mode_t umask(mode_t cmask); // S_IRUSR etc
```

Change access rights modificators  
```c
#include <sys/stat.h>
int chmod(const char * pathname, mode_t mode);
int fchmod(int filedes, mode_t mode);
```

`S_IRWXU` and others, `S_ISUID`, `S_ISGID`, `S_ISVTX`  
`S_ISVTX` is saved-text(sticky) bit  

```c
#include <unistd.h>
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
```
set owner/group to -1 for leave unchanged  
resets suid/sgid bits if not by superuser called..

```c
#include <unistd.h>
int truncate(const char *pathname, off_t length);
int ftruncate(int fd, off_t length);
```

```c
#include <unistd.h>
int link(const char *existingpath, const char *newpath);
int unlink(const char *pathname);
int symlink(const char *actualpath, const char *sympath);
ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsize);

```

```c
#include <stdio.h>
int remove(const char *pathname);// unlink/rmdir
int rename(const char *oldname, const char *newname);
```


Last modify time  
```c
#include <utime.h>
int utime(const char *pathname, const struct utimbuf *times);

struct utimbuf {
  time_t actime;
  time_t modtime;
}
```

```c
#include <sys/stat.h>
int mkdir(const char *pathname, mode_t mode);
```

```c
#include <unistd.h>
int rmdir(const char *pathname);
```

### Directory reading
```c
#include <dirent.h>
DIR *opendir(const char *pathname);
struct dirent *readdir(DIR *dp);
void rewinddir(DIR *dp);
int closedir(DIR *dp);
long telldir(DIR *dp);
void seekdir(DIR *dp, long loc);

struct dirent {
  ino_t d_ino;
  char d_name[NAME_MAX + 1];
}
```

```c
#include <unistd.h>
int chdir(const char *pathname);
int fchdir(int fd);
char *getcwd(char *buf, size_t size);
```
