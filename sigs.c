#include <stdio.h> /* printf */
#include <sys/wait.h>

static void sig_int(int);

int main(int argc, char const *argv[])
{
  if (signal(SIGINT, sig_int) == SIG_ERR) {
    fprintf(stderr, "signal() error\n");
    return -1;
  }
  char buf[255];
  while (fgets(buf, 255, stdin) != NULL);
  return 0;
}

void sig_int(int no)
{

  printf("\nInterrupt signal received\nPress ^D for exit\n");
}
