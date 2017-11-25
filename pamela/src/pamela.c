#include <unistd.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <string.h>
#include "pamela.h"

/** PAM placeholder function: our module doesn't handle this */
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int pam_flags,
				int argc, const char **argv)
{
  return PAM_IGNORE;
}

/** PAM placeholder function: our module doesn't handle this */
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int pam_flags,
			      int argc, const char **argv)
{
  return PAM_IGNORE;
}

#define MOUNTPOINT_DIR	"/secure_data-rw"
#define CONTAINER_FILE	"/.encrypted_container"

static inline void get_mountpoint_path(char *out, const char *home)
{
  strcpy(stpcpy(out, home), MOUNTPOINT_DIR);
}

static inline void get_container_path(char *out, const char *home)
{
  strcpy(stpcpy(out, home), CONTAINER_FILE);
}

#define check_user(pamh, user)						     \
  if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS) {			     \
    return PAM_IGNORE;							     \
  }									     \
									     \
  log("Got user name '%s'\n", user);					     \
  if (strcmp(user, "root") == 0) {					     \
    return PAM_IGNORE;							     \
  }

/* #define get_homedir(user)						     */

/**
 * PAM handler for authentication
 * Our module doesn't validate authentication, however it needs
 * to retrieve the auth token.
 */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int pam_flags,
				   int argc, const char **argv)
{
  const char *user;
  const char *tok;

  check_user(pamh, user);

  if (pam_get_item(pamh, PAM_AUTHTOK, (const void **)&tok) != PAM_SUCCESS) {
    return PAM_IGNORE;
  }
  log("Saving auth token '%s'\n", tok);
  pam_set_data(pamh, "authtok", strdup(tok), NULL);
  return PAM_IGNORE;
}

/**
 * PAM handler for password changes
 *
 * Our module doesn't validate password changes, however it needs
 * to retrieve the new auth token in order
 */
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int pam_flags,
				int argc, const char **argv)
{
  char homedir[4096];
  char container[4096];
  char mountpoint[4096];
  const char *user;
  const char *oldtok;
  const char *tok;

  check_user(pamh, user);

  if (pam_get_item(pamh, PAM_AUTHTOK, (const void **)&tok) != PAM_SUCCESS || !tok)
    return PAM_IGNORE;
  log("Got token '%s'\n", tok);

  if (pam_get_authtok(pamh, PAM_OLDAUTHTOK, &oldtok, "Old password: ") != PAM_SUCCESS
      || !oldtok)
    return PAM_IGNORE;
  log("Got old token '%s'\n", oldtok);

  if (get_home_dir(homedir, user) == -1)
    return PAM_IGNORE;
  log("Got home directory '%s'\n", homedir);

  get_container_path(container, homedir);
  get_mountpoint_path(mountpoint, homedir);
  luks_reencrypt(user, container, oldtok, tok, mountpoint);
  return PAM_IGNORE;
}

/* static size_t nblogins = 0; */

/** Retrieve a previously stored auth token or ask for password if none is found */
static int get_pass_or_ask(pam_handle_t *pamh, const char **tok)
{
  if (pam_get_data(pamh, "authtok", (const void **)tok) == PAM_SUCCESS && *tok)
    return PAM_SUCCESS;
  return pam_get_authtok(pamh, PAM_AUTHTOK, tok, NULL);
}

/**
 * PAM handler for user session opening
 *
 * We create the LUKS container if it doesn't exist yet, then open it
 * and mount it in the user's home directory
 */
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int pam_flags,
				   int argc, const char **argv)
{
  char homedir[4096];
  char container[4096];
  char mountpoint[4096];
  const char *user;
  const char *tok;

  check_user(pamh, user);

  if (get_home_dir(homedir, user) == -1)
    return PAM_IGNORE;
  log("Got home directory '%s'\n", homedir);

  if (get_pass_or_ask(pamh, &tok) != PAM_SUCCESS)
    return PAM_IGNORE;
  log("Got auth token '%s'\n", tok);

  get_container_path(container, homedir);
  get_mountpoint_path(mountpoint, homedir);
  if (access(container, F_OK) != 0) {
    if (luks_create(user, container, tok) == -1) {
      return PAM_SYSTEM_ERR;
    }
  }
  luks_mount(user, container, tok, mountpoint);
  /* nblogins++; */
  return PAM_IGNORE;
}

/**
 * PAM handler for user session closing
 *
 * We unmount and close the LUKS container
 */
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int pam_flags,
				    int argc, const char **argv)
{
  const char *user;
  char mountpoint[4096];

  check_user(pamh, user);

  if (get_home_dir(mountpoint, user) == -1) {
    return PAM_IGNORE;
  }
  strcat(mountpoint, MOUNTPOINT_DIR);
  log("Got mountpoint '%s'\n", mountpoint);
  /* nblogins--; */
  /* if (nblogins == 0) */
  luks_umount(user, mountpoint);
  return PAM_IGNORE;
}
