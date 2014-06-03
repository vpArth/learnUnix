#include <stdio.h> /* printf */
#include <signal.h>
static void handler(int);

#define SUCCESS 0
#define ERROR -1
#define MAXINPUT 255

int addSigHandlers()
{
  if (signal(  SIGINT, handler) == SIG_ERR) return ERROR;
  if (signal( SIGUSR1, handler) == SIG_ERR) return ERROR;
  if (signal( SIGUSR2, handler) == SIG_ERR) return ERROR;

  // set handler only if it not ignored earlier:
  // if (signal(SIGINT, SIG_IGN) != SIG_IGN) signal(SIGINT, sig_int);
  // we can't to know current handler without change it with signal function

  return SUCCESS;
}

int main(int argc, char const *argv[])
{
  printf("Process %d started\n", getpid());
  if(addSigHandlers() != SUCCESS) return ERROR;
  char buf[MAXINPUT];
  for(;;) pause();
  return SUCCESS;
}

int counter = 0;
void handler(int no)
{
//  if (signal(no, handler) == SIG_ERR) return;
  printf("\nSignal %d received, %d\n", no, ++counter);
}

