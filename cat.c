#include <stdio.h> /* printf */

#define BUFSIZE 1024

#define E_SUCCESS 0
#define E_WRITE_ERROR 1
#define E_READ_ERROR  2

int cat(FILE* in, FILE* out, FILE* err)
{
  int n;
  char buf[BUFSIZE];
  while ((n = read(fileno(in), buf, BUFSIZE)) > 0) {
    if (write(fileno(out), buf, n) != n) {
      fprintf(err, "write() error\n");
      return E_WRITE_ERROR;
    }
  }
  if (n < 0) {
    fprintf(err, "read() error\n");
    return E_READ_ERROR;
  }
  return E_SUCCESS;
}

int catb(FILE* in, FILE* out, FILE* err)
{
  char c;
  while ((c = getc(in)) != EOF) {
    if (putc(c, out) == EOF) {
      fprintf(err, "putc() error\n");
      return E_WRITE_ERROR;
    }
  }
  if (ferror(in)) {
    fprintf(err, "getc() error\n");
    return E_READ_ERROR;
  }
  return E_SUCCESS;
}

int main(int argc, char const *argv[])
{
  return catb(stdin, stdout, stderr);
}
