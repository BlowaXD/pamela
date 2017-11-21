#include <security/pam_modules.h>
#include "pamela.h"

/** PAM placeholder function: our module doesn't handle this */
PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
				int argc, const char **argv)
{
  pam_log(flags, "Ignoring account management\n");
  return PAM_IGNORE;
}

/** PAM placeholder function: our module doesn't handle this */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags,
				   int argc, const char **argv)
{
  pam_log(flags, "Ignoring authentication\n");
  return PAM_IGNORE;
}

/** PAM placeholder function: our module doesn't handle this */
PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags,
			      int argc, const char **argv)
{
  pam_log(flags, "Ignoring credentials settings\n");
  return PAM_IGNORE;
}

/** PAM handler for password changes */
PAM_EXTERN int pam_sm_chauthtok(pam_handle_t *pamh, int flags,
				int argc, const char **argv)
{
  pam_log(flags, "Handling password changes\n");
  return PAM_IGNORE;
}

/** PAM handler for user session opening */
PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags,
				   int argc, const char **argv)
{
  pam_log(flags, "Handling session opening\n");
  return PAM_IGNORE;
}

/** PAM handler for user session closing */
PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags,
				    int argc, const char **argv)
{
  pam_log(flags, "Handling session closing\n");
  return PAM_IGNORE;
}
