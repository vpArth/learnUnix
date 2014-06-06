#include <stdio.h> /* printf */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> /* getpid, pause */
#include <stdint.h>
#include <time.h>       /* time */

typedef struct params_t {
  pthread_t thread;
  int id;
  int secs;
  int ret;
} params_t;

void * sleepy(void *arg)
{
  params_t *self = (params_t *) arg;
  printf("\tLet %d sleep %d secs...\n", self->id, self->secs);
  sleep(self->secs);
  printf("\tWaked up %d after %d secs...\n", self->id, self->secs);
  self->ret = rand() % 6 + 1;
  return (void *)(intptr_t)0;
}

void makeSleepy(params_t* params) {
  int err;
  printf("\tTry to run %d to %d secs...\n", params->id, params->secs);
  err = pthread_create(&params->thread, NULL, sleepy, params);
  if (err != 0) {
    fprintf(stderr, "pthread_create() error\n");
    exit(1);
  }
}

void runNumSleepy(int num) {
  params_t params[num];
  void* res;
  int i = 0;
  for (i=0; i<num; i++ ) {
    params[i].id = i+1;
    params[i].secs = (i+1)/2;
    makeSleepy(&params[i]);
  }
  for (i=0;i<num;i++) {
    pthread_join(params[i].thread, &res);
  }
  for (i=0;i<num;i++) {
    printf("%d: %d\n", params[i].id, params[i].ret);
  }

}

int main(/*int argc, char const *argv[]*/)
{
  srand (time(NULL));
  runNumSleepy(3);
  return 0;
}
