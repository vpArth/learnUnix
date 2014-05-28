#include <stdio.h>   /* printf */
#include <unistd.h>  /* execlp */
#include <string.h>  /* strlen */
#include <sys/wait.h>

#define MAXLINE 512

int main(int argc, char const *argv[])
{
  char buf[MAXLINE];
  pid_t pid;
  int status;

  printf("> ");
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf)-1] == '\n')
      buf[strlen(buf)-1] = '\0';
    if ((pid = fork()) < 0) {
      fprintf(stderr, "fork() error\n");
    } else if(pid == 0) {
      //child branch
      execlp(buf, buf, (char*)0);
      fprintf(stderr, "Command '%s' failed\n", buf);
      return 127;
    } else {
      //parent branch
      if ((pid = waitpid(pid, &status, 0)) < 0) {
        fprintf(stderr, "waitpid() error\n");
        return 1;
      }
      printf("> ");
    }
  }
  return 0;
}
