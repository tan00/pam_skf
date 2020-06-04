//
// Created by a8398 on 2020/5/27.
//

#ifndef PAM_ZCWKEY_PAM_ZCWKEY_H
#define PAM_ZCWKEY_PAM_ZCWKEY_H

#include <security/pam_modules.h>
#include <security/pam_appl.h>
PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags,int argc, const char *argv[]);
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv );

#endif //PAM_ZCWKEY_PAM_ZCWKEY_H
