#include <string.h>
#include <pwd.h>

int get_home_dir(char *out, const char *username)
{
  struct passwd passwd;
  struct passwd *result;
  char buf[16384];
  int ret = getpwnam_r(username, &passwd, buf, 16384, &result);

  if (!result || ret != 0)
    return -1;
  strcpy(out, passwd.pw_dir);
  return 0;
}
