#include <stdarg.h>
#include <stdio.h>

void pam_do_log(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
}
