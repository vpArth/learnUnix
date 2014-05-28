#include <stdio.h> /* printf */
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int test(int fd)
{
  int val;
  if ((val = fcntl(fd, F_GETFL, 0)) < 0 ) {
    fprintf(stderr, "fcntl() error for %d\n", fd);
    return -1;
  }
  switch (val & O_ACCMODE) {
    case O_RDONLY: printf("Read only\n"); break;
    case O_WRONLY: printf("Write only\n"); break;
    case O_RDWR:   printf("Read/write\n"); break;
    default:
      fprintf(stderr, "Unknown access mode %d for %d\n", val, fd);
      return -1;
  }
  if (val & O_APPEND)   printf("append mode\n");
  if (val & O_NONBLOCK) printf("nonblock mode\n");
#ifdef O_SYNC
  if (val & O_SYNC)     printf("sync write mode\n");
#endif

  return 0;
}

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s fdnum\n", argv[0]);
    return -1;
  }
  int status;
  status = test(atoi(argv[1]));
  if(status) {
    return status;
  }
  return 0;
}
