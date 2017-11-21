#ifndef PAMELA_H
#define PAMELA_H

void pam_do_log(const char *fmt, ...);

#define pam_log(flags, fmt, ...)					     \
  ({									     \
    if (!(flags & PAM_SILENT))						     \
      pam_do_log(fmt, #__VA_ARGS__);					     \
  })

#endif /* !PAMELA_H */
