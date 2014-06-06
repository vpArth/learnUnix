#include <pthread.h>
#include <syslog.h>
sigset_t mask;
extern int already_running(void);
void reread(void);
void daemonize(const char*);


void * thr_fn(void *arg) {
  int err, signo;
  for (;;) {
    err = sigwait(&mask, &signo);
    if (err != 0) {
    syslog(LOG_ERR, "ошибка вызова функции sigwait");
    exit(1);
  }
  switch (signo) {
    case SIGHUP:
      syslog(LOG_INFO, "Чтение конфигурационного файла");
      reread();
      break;
    case SIGTERM:
      syslog(LOG_INFO, "получен сигнал SIGTERM; выход");
      exit(0);
    default:
      syslog(LOG_INFO, "получен непредвиденный сигнал %d\n", signo);
    }
  }
  return(0);
}
void bythread() {
  sa.sa_handler = SIG_DFL;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) syslog("%s: невозможно восстановить действие SIG_DFL для SIGHUP", argv[0]);
  sigfillset(&mask);
  if (pthread_sigmask(SIG_BLOCK, &mask, NULL)) syslog("ошибка выполнения операции SIG_BLOCK");
  if (pthread_create(&tid, NULL, thr_fn, 0)) syslog("невозможно создать поток");
}


void sigterm(int signo) {
  syslog(LOG_INFO, "получен сигнал SIGTERM; выход");
  exit(0);
}
void sighup(int signo) {
  syslog(LOG_INFO, "Чтение конфигурационного файла");
  reread();
}
void bysig() {
  sa.sa_handler = sigterm;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGHUP);
  sa.sa_flags = 0;
  if (sigaction(SIGTERM, &sa, NULL) < 0) {
    syslog(LOG_ERR, "невозможно перехватить сигнал SIGTERM: %s",
    strerror(errno));
    exit(1);
  }

  sa.sa_handler = sighup;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGTERM);
  sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
    syslog(LOG_ERR, "невозможно перехватить сигнал SIGHUP: %s",
    strerror(errno));
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  int err;
  pthread_t tid;
  char *cmd;
  struct sigaction sa;

  if ((cmd = strrchr(argv[0], ’/’)) == NULL)
    cmd = argv[0];
  else cmd++;

  daemonize(cmd);

  if (already_running()) {
    syslog(LOG_ERR, "демон уже запущен");
    exit(1);
  }
  // bythread();
  bysig();
  /*
  .....
  */
  exit(0);
}
