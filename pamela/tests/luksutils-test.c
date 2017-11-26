#include "pamela.h"

#define fail_if(x)							     \
  ({									     \
    if ((x) != 0) {							     \
      return 1;								     \
    }									     \
  })

int main(int ac, char **av)
{
  fail_if(ac < 2);
  fail_if(luks_create(av[1], "/tmp/somecontainer", "s3cr3tk3y"));
  fail_if(luks_mount(av[1], "/tmp/somecontainer", "s3cr3tk3y", "/tmp/mnt"));
  fail_if(shell_run("echo -n aaa > /tmp/mnt/file"));
  fail_if(luks_umount(av[1], "/tmp/mnt"));
  fail_if(luks_mount(av[1], "/tmp/somecontainer", "s3cr3tk3y", "/tmp/mnt"));
  fail_if(shell_run("[ \"$(cat /tmp/mnt/file)\" = \"aaa\" ]"));
  fail_if(luks_umount(av[1], "/tmp/mnt"));
  fail_if(shell_run("rm /tmp/somecontainer"));
  return 0;
}
