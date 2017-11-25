#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

int shell_run(const char *fmt, ...)
{
  char buff[4096];
  va_list ap;

  va_start(ap, fmt);
  vsnprintf(buff, 4096, fmt, ap);
  va_end(ap);
  return system(buff);
}
