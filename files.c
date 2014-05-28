#include <stdio.h> /* printf */
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int write_test()
{
  int fd;
  char buf[] = "abcdefghijklmnopqrstuvwxyz";
  int len = sizeof(buf)-1;
  if ((fd = open("test.tmp", O_RDWR | O_CREAT | O_TRUNC, S_IWRITE | S_IREAD))<0) {
    fprintf(stderr, "open() error\n");
    return 1;
  }
  if (write(fd, buf, len) != len) {
    fprintf(stderr, "write() error\n");
    return 1;
  }
  if (lseek(fd, 16384, SEEK_SET) == -1) {
    fprintf(stderr, "lseek() error\n");
    return 1;
  }
  if (write(fd, buf, len) != len) {
    fprintf(stderr, "write() error\n");
    return 1;
  }
  close(fd);
  return 0;
}

int read_test()
{
  int fd;
  if ((fd = open("test.tmp", O_RDONLY, S_IREAD))<0) {
    fprintf(stderr, "open() error: %s\n", strerror(errno));
    return 1;
  }

  int len = 256;
  char buf[len];
  ssize_t n;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(fileno(stdout), buf, n) != n) {
      fprintf(stderr, "write() error\n");
      return 1;
    }
  }
  if (n < 0) {
    fprintf(stderr, "read() error\n");
    return 1;
  }
  close(fd);
  return 0;
}

int main(int argc, char const *argv[])
{
  int status;
  if (status = write_test()) {
    return status;
  }
  if (status = read_test()) {
    return status;
  }
  printf("\n");
  unlink("test.tmp");
  return 0;
}
