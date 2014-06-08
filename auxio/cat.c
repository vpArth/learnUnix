#include <stdio.h> /* printf */
#include <errno.h>
#include <fcntl.h>

void set_fl(int fd, int flags)
{
  int   val;

  if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    fprintf(stderr, "fcntl F_GETFL error\n");

  val |= flags;   /* turn on flags */

  if (fcntl(fd, F_SETFL, val) < 0)
    fprintf(stderr, "fcntl F_SETFL error\n");
}

void clr_fl(int fd, int flags)
{
  int   val;

  if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    fprintf(stderr, "fcntl F_GETFL error\n");

  val &= ~flags;    /* turn flags off */

  if (fcntl(fd, F_SETFL, val) < 0)
    fprintf(stderr, "fcntl F_SETFL error\n");
}


char buf[500000];
int main()
{
  int ntowrite, nwrite;
  char *ptr;
  ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
  fprintf(stderr, "прочитано %d байт\n", ntowrite);

  set_fl(STDOUT_FILENO, O_NONBLOCK);
  ptr = buf;
  while (ntowrite > 0) {
    errno = 0;
    nwrite = write(STDOUT_FILENO, ptr, ntowrite);
    fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);
    if (nwrite > 0) {
      ptr += nwrite;
      ntowrite = nwrite;
    }
  }
  clr_fl(STDOUT_FILENO, O_NONBLOCK);

  exit(0);
}
