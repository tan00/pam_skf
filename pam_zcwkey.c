#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <sys/types.h>
#include <pwd.h>
#include "include/dev_manager.h"
#include "auth.h"



/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
    printf(">>>>>pam_sm_setcred \n ");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    printf(">>>> pam_sm_acct_mgmt\n");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {

    const char *user;
    char  *password;
    int pam_err, retry;
    int ret = 0;
    char *resposemsg = NULL;

    // get user
    if ((pam_err = pam_get_user(pamh, &user, NULL)) != PAM_SUCCESS)
        return (pam_err);

    // get password
    for (retry = 0; retry < 3; ++retry) {
        pam_err = pam_get_authtok(pamh, PAM_AUTHTOK,
                                  (const char **)&password, "ukey PIN");
        if (pam_err == PAM_SUCCESS)
            break;
    }

    if (pam_err == PAM_CONV_ERR)
        return (pam_err);

    if (pam_err != PAM_SUCCESS)
        return (PAM_AUTH_ERR);


    AUTHFILE authfile;
    ret = AUTHFILE_Read(&authfile);
    if (ret!=0){
        pam_prompt(pamh , PAM_ERROR_MSG,  &resposemsg,"pam_sm_authenticate  read from usbkey user =%s  password =%s \n",authfile.user,authfile.enc_passwd);
        pam_prompt(pamh , PAM_ERROR_MSG,  &resposemsg,"read form ukey failed ret = %x \n",ret);
        return PAM_CRED_INSUFFICIENT;
    }

    //如果未初始化，写入key
    if ( authfile.inited != 1){
        AUTHFILE_EncryptPwd(&authfile,NULL);
        AUTHFILE_Write(&authfile);
        return PAM_SUCCESS;
    }

    //如果已初始化，对比用户名和密码

    pam_prompt(pamh , PAM_TEXT_INFO,  &resposemsg,"pam_sm_authenticate get from promot  user =%s  password =%s \n" ,user,password);
    pam_prompt(pamh , PAM_TEXT_INFO,  &resposemsg,"pam_sm_authenticate  read from usbkey user =%s  password =%s \n",authfile.user,authfile.enc_passwd);

    //比较用户名密码
    if (memcmp(user,authfile.user,strlen(user)) != 0){
       //return PAM_AUTH_ERR;
       return PAM_SUCCESS;
    }

    AUTHFILE authFromUser;
    AUTHFILE_EncryptPwd(&authFromUser,password);
    if (memcmp(authFromUser.enc_passwd ,authfile.enc_passwd,strlen(authFromUser.enc_passwd)) != 0){
        //return PAM_AUTH_ERR;
        return PAM_SUCCESS;
    }

    return (PAM_SUCCESS);
}


