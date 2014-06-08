#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>

#define read_lock(fd, offset, whence, len) \
lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
  struct flock lock;
  lock.l_type = type;
  lock.l_start = offset;
  lock.l_whence = whence;
  lock.l_len = len;
  return fcntl(fd, cmd, &lock);
}

#define is_read_lockable(fd, offset, whence, len) \
(lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
(lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
  struct flock lock;
  lock.l_type = type; /* F_RDLCK/F_WRLCK */
  lock.l_start = offset;
  lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
  lock.l_len = len; /*bytenum, 0 - to eof*/
  if (fcntl(fd, F_GETLK, &lock) < 0) {
    fprintf(stderr, "fcntl error\n");
    exit(-1);
  }
  if (lock.l_type == F_UNLCK) return 0; //false
  return(lock.l_pid); //true
}

int lockfile(int fd)
{
  //write_lock(fd, 0, SEEK_SET, 0);
  struct flock fl;
  fl.l_type = F_WRLCK;
  fl.l_start = 0;
  fl.l_whence = SEEK_SET;
  fl.l_len = 0;
  return(fcntl(fd, F_SETLK, &fl));
}

static int  pfd1[2], pfd2[2];

void TELL_WAIT(void)
{
  if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
    fprintf(stderr, "pipe error");
    exit(-1);
  }
}

void TELL_PARENT(pid_t pid)
{
  if (write(pfd2[1], "c", 1) != 1) {
    fprintf(stderr, "write error");
    exit(-1);
  }
}

void WAIT_PARENT(void)
{
  char  c;

  if (read(pfd1[0], &c, 1) != 1) {
    fprintf(stderr, "read error");
    exit(-1);
  }

  if (c != 'p') {
    fprintf(stderr, "WAIT_PARENT: incorrect data");
    exit(-1);
  }
}

void TELL_CHILD(pid_t pid)
{
  if (write(pfd1[1], "p", 1) != 1) {
    fprintf(stderr, "write error");
    exit(-1);
  }
}

void WAIT_CHILD(void)
{
  char  c;

  if (read(pfd2[0], &c, 1) != 1) {
    fprintf(stderr, "read error");
    exit(-1);
  }

  if (c != 'c') {
    fprintf(stderr, "WAIT_CHILD: incorrect data");
    exit(-1);
  }
}



#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void lockabyte(const char*, int, off_t);

void deadlock() {
  pid_t pid;
  int fd;
  /*
  * Создать файл и записать в него два байта.
  */
  if ((fd = creat("templock", FILE_MODE)) < 0){
    fprintf(stderr, "creat error");
    exit(-1);
  }
  if (write(fd, "ab", 2) != 2){
    fprintf(stderr, "write error");
    exit(-1);
  }
  TELL_WAIT();
  if ((pid = fork()) < 0) {
    fprintf(stderr, "ошибка вызова функции fork");
    exit(-1);
  } else if (pid == 0) {
    /* потомок */
    lockabyte ("потомок", fd, 0);
    TELL_PARENT(getppid());
    WAIT_PARENT();
    lockabyte("потомок", fd, 1);
  } else {
    /* родитель */
    lockabyte("родитель", fd, 1);
    TELL_CHILD(pid);
    WAIT_CHILD();
    lockabyte("родитель", fd, 0);
  }
  exit(0);
}


static void lockabyte(const char *name, int fd, off_t offset)
{
  if (writew_lock(fd, offset, SEEK_SET, 1) < 0) {
    fprintf(stderr, "%s: ошибка вызова функции writew_lock: %s\n", name, strerror(errno));
    exit(-1);
  }
  printf("%s: установлена блокировка на байт %ld\n", name, offset);
}

int main(void)
{
  deadlock();
  return 0;
}
