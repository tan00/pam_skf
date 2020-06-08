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
    printf(">>>> pam_sm_setcred\n ");
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate( pam_handle_t *pamh, int flags,int argc, const char **argv ) {
    const char *user;
    char  *password;
    int pam_err, retry;
    int ret = 0;

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
    memset(&authfile,0,sizeof authfile);
    ret = AUTHFILE_Read(&authfile);
    if (ret!=0){
        pam_prompt(pamh , PAM_ERROR_MSG,  NULL,"read form ukey failed！does ukey inited? \n");
        return PAM_CRED_INSUFFICIENT;
    }

    //文件存在，但是没有内容
    if ( authfile.inited != 1){
        pam_prompt(pamh , PAM_TEXT_INFO,  NULL,"writing token to ukey\n");
        authfile.inited = 1;
        strcpy(authfile.user , user);
        strcpy(authfile.enc_passwd , password);
        ret = AUTHFILE_Write(&authfile);
        if(ret!=0){
            pam_prompt(pamh , PAM_TEXT_INFO,  NULL,"failed write token to ukey\n");
        }
    }

    //比较用户名密码
    if (memcmp(user,authfile.user,strlen(user)) != 0){
       return PAM_AUTH_ERR;
    }
    AUTHFILE authFromUser;
    memset(&authFromUser,0,sizeof authFromUser);
    AUTHFILE_EncryptPwd(&authFromUser,password);
    if (memcmp(authFromUser.enc_passwd ,authfile.enc_passwd,strlen(authFromUser.enc_passwd)) != 0){
        return PAM_AUTH_ERR;
    }

    return (PAM_SUCCESS);
}


