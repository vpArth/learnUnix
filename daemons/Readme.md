####Rules:  
 - `umask(0);`
 - `fork`  and exit from parent
 - `setsid`
 - `chdir` to root `/`
 - close excess descriptors (getrlimit/open_max)
 - [open 0, 1, 2 descriptors on /dev/null]


####Error logs  
```c
#include <syslog.h>
void openlog(const char * ident, int option, int facility);
void syslog(int priority, const char * format, ...);
void closelog(void);
int setlogmask(int maskpri);

#include <syslog.h>
#include <stdarg.h>
void vsyslog(int priority, const char *format, va_list arg);
```

####Conventions:
 - pidfile - /var/run/named.pid
 - config - /etc/name.conf
 - service - /etc/init.d/name
 - update config by SIGHUP

