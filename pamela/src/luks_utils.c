#include "pamela.h"

#define err_if(x)							     \
  ({									     \
    if ((x) != 0) {							     \
      return -1;							     \
    }									     \
  })

int luks_create(const char *id, const char *container_file, const char *key)
{
  err_if(shell_run("dd if=/dev/urandom of=%s bs=1 count=0 seek=2G", container_file));
  err_if(shell_run("chown %s:%s %s", id, id, container_file));
  err_if(shell_run("echo %s | sudo cryptsetup luksFormat %s", key, container_file));
  err_if(shell_run("echo %s | sudo cryptsetup luksOpen %s container_%s", key,
		   container_file, id));
  err_if(shell_run("sudo mkfs.ext4 /dev/mapper/container_%s", id));
  err_if(shell_run("sudo cryptsetup luksClose container_%s", id));
  return 0;
}

int luks_mount(const char *id, const char *container_file,
	       const char *key, const char *mountpoint)
{
  err_if(shell_run("echo %s | sudo cryptsetup luksOpen %s container_%s",
		   key, container_file, id));
  err_if(shell_run("mkdir -p %s", mountpoint));
  err_if(shell_run("sudo mount /dev/mapper/container_%s %s", id, mountpoint));
  err_if(shell_run("chown -R %s:%s %s", id, id, mountpoint));
  return 0;
}

int luks_umount(const char *id, const char *mountpoint)
{
  err_if(shell_run("sudo umount %s", mountpoint));
  err_if(shell_run("rmdir %s", mountpoint));
  err_if(shell_run("sudo cryptsetup luksClose container_%s", id));
  return 0;
}

int luks_reencrypt(const char *id, const char *container_file,
		   const char *oldkey, const char *newkey,
		   const char *mountpoint)
{
  luks_umount(id, mountpoint);
  err_if(shell_run("echo %s | sudo cryptsetup luksOpen %s container_%s",
		   oldkey, container_file, id));
  err_if(shell_run("echo -e \"%s\n%s\" | sudo cryptsetup-reencrypt %s",
		   oldkey, newkey, container_file));
  err_if(shell_run("mkdir -p %s", mountpoint));
  err_if(shell_run("sudo mount /dev/mapper/container_%s %s", id, mountpoint));
  err_if(shell_run("chown -R %s:%s %s", id, id, mountpoint));
  return 0;
}

#undef err_if
