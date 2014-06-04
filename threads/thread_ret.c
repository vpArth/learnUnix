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

  void * thr_fn1(void *arg)
  {
    printids("Child1");
    return((void *)1);
  }
  void * thr_fn2(void *arg)
  {
    printids("Child2");
    pthread_exit((void *)2);
  }

int main(void)
{
  pthread_t tid1, tid2;
  void* tret;
  int err;
  err = pthread_create(&tid1, NULL, thr_fn1, NULL);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    return -1;
  }
  err = pthread_create(&tid2, NULL, thr_fn2, NULL);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    return -1;
  }
  printids("Main");

  err = pthread_join(tid1, &tret);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    return -1;
  }
  printf("Thread1 returns %d\n", (int)tret);

  err = pthread_join(tid2, &tret);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    return -1;
  }
  printf("Thread2 returns %d\n", (int)tret);

  sleep(1);
  return 0;
}
