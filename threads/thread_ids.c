#include <stdio.h> /* printf */
#include <stdlib.h>
#include <pthread.h>


  void printids(const char *s)
  {
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    printf("%s:\tpid %u tid %u (0x%x)\n", s, (unsigned int)pid,
    (unsigned int)tid, (unsigned int)tid);
  }

  void * thr_fn(void *arg)
  {
    printids("Child");
    return((void *)0);
  }

int main(void)
{
  pthread_t ntid;
  int err;
  err = pthread_create(&ntid, NULL, thr_fn, NULL);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    return -1;
  }

  printids("Main");
  sleep(1);
  return 0;
}
