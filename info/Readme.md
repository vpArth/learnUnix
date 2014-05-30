
###System info

####File `/etc/passwd`  
```
root:x:0:0:root:/root:/bin/bash
nobody:x:65534:65534:Nobody:/home:/bin/sh
```

```c  
#include <pwd.h>
struct passwd *getpwuid(uid_t uid);
struct passwd *getpwnam(const char *name);

struct passwd *getpwent(void); //get next passwd record
void setpwent(void); // rewind
void endpwent(void); //close files
```

Struct passwd fields:  
 - name - `char *pw_name`  
 - pass - `char *pw_passwd`  
 - uid  - `uid_t pw_uid`  
 - gid  - `gid_t pw_gid`  
 - remark - `char *pw_gecos`  
 - homedir - `char *pw_dir`  
 - shell - `char *pw_shell`  
 - accessclass - `char *pw_class`  
 - passexpire - `time_t *pw_change`  
 - acctexpire - `time_t *pw_expire`  

####File `/etc/shadow`  

```c
#include <shadow.h>
struct spwd *getspnam(const char *name);
struct spwd *getspent(void);
void setspent(void);
void endspent(void)
```  
Struct spwd fields:  
 - name - `char *sp_namp`  
 - pass - `char *sp_pwdp`  
 - lmod - `int sp_lstchg`  
 - minupdateperioddays - `int sp_min`  
 - maxupdateperioddays - `int sp_max`  
 - warnbefore - `int sp_warn`  
 - nouseddays - `int sp_inact`  
 - expiredate - `int sp_expire`  
 - reserved - `unsigned int sp_flag`

####File `/etc/group`  

```c
#include <grp.h>
struct group *getgrgid(gid_t gid);
struct group *getgrnam(const char * name);
struct group *getgrent(void);
void setgrent(void);
void endgrent(void);
```  

Struct group fields:  
  - name - `char *gr_name`  
  - pass - `char *gr_passwd`  
  - gid  - `int gr_gid`  
  - members - `char **gr_mem`  

Additional groups:  

```c

#include <unistd.h>
int getgroups(int gidsetsize, gid_t grouplist[]);
//fills list, returns number

#include <grp.h>/* Linux */
#include <unistd.h> /* FreeBSD, Mac OS X, Solaris */
int setgroups(int ngroups, const gid_t grouplist[]);

#include <grp.h>/* Linux, Solaris */
#include <unistd.h> /* FreeBSD, Mac OS X */
int initgroups(const char *username, gid_t basegid);
```

####Other files:  
File | Header | Struct | functions
---- | ------ | ------ | --- 
`/etc/hosts`     | `<netdb.h>` | `hostent` | `gethostbyname`, `gethostbyaddr` 
`/etc/networks`  | `<netdb.h>` | `netent`  | `getnetbyname`, `getnetbyaddr`
`/etc/protocols` | `<netdb.h>` | `protoent`| `getprotobyname`, `getprotobynumber`
`/etc/services`  | `<netdb.h>` | `servent` | `getservbyame`, `getservbyprot`

####Logins control  
 - `/var/run/utmp`  
 - `/var/run/wtmp`  

####System info  
```c
#include <sys/utsname.h>
int uname(struct utsname *name);

struct utsname {
  char sysname[];
  char nodename[];
  char release[];
  char version[];
  char machine[];
}

#include <unistd.h>
int gethostname(char *name, int namelen);

``` 

####Datetime functions  
```c
#include <time.h>
time_t time(time_t *calptr);

#include <sys/time.h>
int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
  struct timeval {
  time_t tv_sec; /* секунды */
  long tv_usec; /* микросекунды */
};

#include <time.h>
struct tm *gmtime(const time_t *calptr);
struct tm *localtime(const time_t *calptr);
time_t mktime(struct tm *tmptr);
char *asctime(const struct tm *tmptr);
char *ctime(const time_t *calptr);
size_t strftime(char *restrict buf, size_t maxsize,
        const char *restrict format,
        const struct tm *restrict tmptr);
struct tm {
int tm_sec; //0-60
int tm_min; //0-59
int tm_hour;//0-23
int tm_mday;//1-31
int tm_mon; //0-11
int tm_year;//1900-x
int tm_wday;//0-6
int tm_yday;//0-365
int tm_isdst;//<0, 0, >0
};
```  

