#include <stdio.h> /* printf */
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
  pid_t pid;
  printf("%d\tBefore fork\n", getpid());
  if ((pid = fork()) < 0) {
    fprintf(stderr, "fork() error\n");
  } else if(pid == 0) {
    //child branch
    printf("%d\tAfter fork\n", getpid());
  } else {
    //parent branch
    printf("%d\tAfter fork\n", getpid());
    printf("%d\tChild pid: %d\n", getpid(), pid);
  }
  return 0;
}
