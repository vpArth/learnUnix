#include <stdio.h>   /* printf perror */
#include <string.h>  /* strerror */
#include <errno.h>

int main(int argc, char const *argv[])
{
  fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
  errno = ENOENT;
  perror(argv[0]);
  return 0;
}
