#ifndef PAMELA_H
#define PAMELA_H

#include <stdio.h>

#ifdef DEBUG
#define log(fmt, ...)		printf(fmt, ##__VA_ARGS__)
#else
#define log(fmt, ...)
#endif

int get_home_dir(char *out, const char *username);

int shell_run(const char *fmt, ...);

int luks_create(const char *user, const char *containerFile, const char *key);

int luks_mount(const char *user, const char *containerFile,
	       const char *key, const char *mountpoint);

int luks_umount(const char *user, const char *mountpoint);

int luks_reencrypt(const char *id, const char *container_file,
		   const char *oldkey, const char *newkey,
		   const char *mountpoint);

#endif /* !PAMELA_H */
