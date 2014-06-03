#include <stdio.h> /* printf */
#include <setjmp.h> /*longjmp, jmp_buf, setjmp*/
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

void hsleep0(int no) {
  //
}
int sleep0(unsigned int secs) {
  if (signal( SIGALRM, hsleep0) == SIG_ERR) return ERROR;//we lost old disposition
  alarm(secs);// we lost also old timer if was set
  //here we have "race state", when alarm triggered before pause
  pause();// each other signal also wake us up
  return SUCCESS;
}

static jmp_buf env_alrm;
static void hsleep1(int no) {
  longjmp(env_alrm, 1);// brake other signal disposition
}
int sleep1(unsigned int secs) {
  if (signal( SIGALRM, hsleep1) == SIG_ERR) return ERROR;//we lost old disposition
  if (setjmp(env_alrm) == 0) {
    alarm(secs);// we lost also old timer if was set
    pause();// each other signal also wake us up
  }
  return SUCCESS;
}

int main(int argc, char const *argv[])
{
  if (argc<2) {
    printf("Using: %s secs\n", argv[0]);
    return ERROR;
  }
  if(addSigHandlers() != SUCCESS) return ERROR;
  unsigned int secs = atoi(argv[1]);
  return sleep1(secs);
}

int counter = 0;
void handler(int no)
{
//  if (signal(no, handler) == SIG_ERR) return;
  printf("\nSignal %d received, %d\n", no, ++counter);
}

