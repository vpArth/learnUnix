#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void fail(char * const msg) {
  printf("%s failed\n", msg);
  exit(-1);
}

void daemonize(const char* cmd) {

  umask(0);

  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) fail("getrlimit()");

  pid_t pid;
  if ((pid = fork())<0) fail("fork()");
  else if (pid) exit(0);
  setsid();

  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) fail("SIGHUP ignoring");
  if ((pid = fork()) < 0) fail("fork()");
  else if (pid) exit(0);

  int i;
  if (rl.rlim_max == RLIM_INFINITY) rl.rlim_max = 1024;
  for (i = 0; i < rl.rlim_max; i++) close(i);

  int fd0, fd1, fd2;
  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_ERR, "Wrong descriptors %d %d %d", fd0, fd1, fd2);
    exit(1);
  }

}
