```c
#include <stdio.h>
#include <wchar.h>
/**
 *@param int mode - try to orient{+/-/0}
 *@return - +: ascii, -: wide, 0: unoriented
 */
int fwide(FILE *fp, int mode);

```

#### Standard IO streams
`<stdio.h>`
 - stdin  (STDIN_FILENO)
 - stdout (STDOUT_FILENO)
 - stderr (STDERR_FILENO)

 #### Buferization
 Minimize read/write calls
 Types:
  - full
  - byrow(\n)
  - no buferization

```c
#include <stdio.h>
/**
 *@param buf - buf[BUFSIZ]:full/byrow_for_tty or NULL:nobuf
 */
void setbuf(FILE *restrict fp, char *restrict buf );
/**
 *@param mode:
 *  _IOFBF - full
 *  _IOLBF - byrow
 *  _IONBF - nobuf
 *@param buf - pointer to buf(NULL for auto alloc BUFSIZ sized buf)
 *@param size - sizeof buf
 *@return 0 if success
 */
int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
```

```c
#include <stdio.h>
// flush buffers
int fflush(FILE *fp);
```

```c
#include <stdio.h>
FILE *fopen(const char *restrict pathname, const char *restrict type);
FILE *freopen(const char *restrict pathname, const char *restrict type,
FILE *restrict fp);
FILE *fdopen(int filedes, const char * type);

int fclose(FILE *fp);
```
Type param(b for binary mode, ignored by UNIX):
 - `r/rb` - read only
 - `w/wb` - write only(also create/truncate)
 - `a/ab` - appendwrite(or create)
 - `r+/r+b/rb+` - rw
 - `w+/w+b/wb+` - rw(create/truncate)
 - `a+/a+b/ab+` - appendrw
 `fdopen` does not truncate file

 for + modes:
  - we can't write after read without `fflush`/`fseek`/`fsetpos`/`rewind`
  - we can't read after write without `fseek`/`fsetpos`/`rewind`/read op which get EOF

### Read/Write streams
  - char by char
  ```c
    #include <stdio.h>
    int getc(FILE *fp);
    int fgetc(FILE *fp);//can't be macro(we have address and can operate with pointer)
    int getchar(void); // getc(stdin);
    //all returns char or EOF - (int)unsigned char

    //Errors handling
    int ferror(FILE *fp);
    int feof(FILE *fp);
    void clearerr(FILE *fp);

    //put char back to stream
    int ungetc(int c, FILE *fp);

    int putc(int c, FILE * fp);
    int fputc(int c, FILE * fp);
    int putchar(int c); // putc(c, stdout);
  ```
  - row by row
  ```c
    #include <stdio.h>
    char *fgets(char *restrict buf, int n, FILE *restrict fp);
    char *gets(char * buf);// buffer overflow risk!!
    //returns buf or NULL(EOF or error)

    // \0 termimated str
    int fputs(const char *restrict str, FILE *restrict fp);
    int puts(const char * str);//adds \n

  ```
  - direct(binary) `fread`/`fwrite`
  ```c
    #include <stdio.h>
    size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    size_t fwrite(const void *restrict ptr, size_t size, size_t nobj,
    FILE *restrict fp);  
  ```

### Stream positioning
 - `ftell`/`fseek`
 - `ftello`/`fseeko`
 - `fsetpos`/`fgetpos` (`fops_t` struct)

```c
  #include <stdio.h>
  long ftell(FILE *fp);
  int fseek(FILE *fp, long offset, int whence);
  void rewind(FILE *fp);

  off_t ftello(FILE *fp);
  int fseeko(FILE *fp, off_t offset, int whence);

  int fgetpos(FILE *restrict fp, fpos_t *restrict pos);
  int fsetpos(FILE *fp, const fpos_t *pos);
```

### Format print

```c
#include <stdio.h>
int printf(const char *restrict format, ...);
int fprintf(FILE *restrict fp, const char *restrict format, ...);
int sprintf(char *restrict buf, const char *restrict format, ...);
int snprintf(char *restrict buf, size_t n, const char *restrict format, ...);
```

Format specification:  
`%[flags][fldwidth][precision][lenmodifier]convtype`  
 - flags
    - `-` - `align: left`
    - `+` - show sign
    - ` ` - space if no sign
    - `#` - alt.form ( 0x123f )
    - `0` - zerofill
 - fldwidth - min-width `\d+|\*` (* - get from arg(before value))
 - precision - min-decimal-float `.(?:\d+|\*)`
 - len
    - `hh` - char
    - `h`  - short
    - `l`  - long|wide
    - `j`  - long long
    - `z`  - size_t
    - `t`  - ptrdiff_t
    - `L`  - long double
 - convtype
    - `d`, `i` - signed decimal
    - `u` - unsigned decimal
    - `o` - unsigned octal
    - `x`, `X` - unsigned hexadecimal
    - `f`, `F` - double
    - `e`, `E` - double exp-form 4.2e13
    - `g`, `G` - `f`, `e` depended of value
    - `a`, `A` - double exp-hex-form
    - `c` - char, wide_t
    - `s` - char[], wide_t[]
    - `p` - void*
    - `n` - int* printed_chars
    - `%` - % char
    - `C` - wide_t
    - `S` - wide_t[]

Vararg variation:  
```c
  #include <stdarg.h>
  #include <stdio.h>

  int vprintf(const char *restrict format, va_list arg);
  int vfprintf(FILE *restrict fp, const char *restrict format, va_list arg);
  int vsprintf(char *restrict buf, const char *restrict format, va_list arg);
  int vsnprintf(char *restrict buf, size_t n, const char *restrict format, va_list arg);
```

#### Format Input:
```c
  #include <stdio.h>
  int scanf(const char *restrict format, ...);
  int fscanf(FILE *restrict fp, const char *restrict format, ...);
  int sscanf(const char *restrict buf, const char *restrict format, ...);
```
`%[*][fldwidth][lenmodifier]convtype`

Vararg variation:  
```c
  #include <stdarg.h>
  #include <stdio.h>
  int vscanf(const char *restrict format, va_list arg);
  int vfscanf(FILE *restrict fp, const char *restrict format, va_list arg);
  int vsscanf(const char *restrict buf, const char *restrict format, va_list arg);
```

___________
```c
  #include <stdio.h>
  int fileno(FILE *fp);//get fd number

  char *tmpnam(char *ptr); // uniq filename
  char *tempnam(const char *directory, const char *prefix);
  FILE *tmpfile(void); // temp file
  
  #include <stdlib.h>
  int mkstemp(char *template);  
```
