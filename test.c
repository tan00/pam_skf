//
// Created by a8398 on 2020/5/29.
//
#include "auth.h"
#include <string.h>
#include <stdio.h>
#include "include/dev_manager.h"

int test_auth_marshal(){
    AUTHFILE authfile;
    authfile.inited = 1;
    strcpy(authfile.user , "lin" );
    strcpy(authfile.enc_passwd , "pwd" );
    if ( AUTHFILE_CheckArgs(&authfile)!=0){
        printf("AUTHFILE_CheckArgs err \n");
        return 0;
    }

    unsigned  char jsonstr[1024];
    int bufsize = sizeof jsonstr;

    AUTHFILE_Marshal(&authfile,jsonstr,&bufsize);
    printf("jsonstr  = %s \n", jsonstr);

    //反序列化
    memset(&authfile,0,sizeof authfile);
    unsigned char jsonstr2[1024];
    int bufsize2 = sizeof jsonstr2;
    AUTHFILE_Unmarshal(&authfile,jsonstr);
    AUTHFILE_Marshal(&authfile,jsonstr2,&bufsize2);
    printf("jsonstr2  = %s \n", jsonstr2);
}

int test_devman() {
    DEVMANAGER *devman = DEVMANAGER_new();
    //DEVMANAGER_FactoryReset(devman);
    DEVMANAGER_FactoryInit(devman);

    return 0;
}

int test_authfile() {
    AUTHFILE authfile;
    authfile.inited = 1;
    strcpy(authfile.user , "lin" );
    strcpy(authfile.enc_passwd , "pwd" );

    //AUTHFILE_Write(&authfile);

    AUTHFILE_Read(&authfile);

    return 0;
}

#if 0
int main()
{
    //test_auth_marshal();

    //test_devman();

    test_authfile();
    return 0;
}
#endif

#if 1
/*
  This program was contributed by Shane Watts
  [modifications by AGM and kukuk]

  You need to add the following (or equivalent) to the
  /etc/pam.d/check_user file:
  # check authorization
  auth       required     pam_unix.so
  account    required     pam_unix.so
 */

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>

static struct pam_conv conv = {
        misc_conv,
        NULL
};

int main(int argc, char *argv[])
{
    pam_handle_t *pamh=NULL;
    int retval;
    const char *user="nobody";

    if(argc == 2) {
        user = argv[1];
    }

    if(argc > 2) {
        fprintf(stderr, "Usage: check_user [username]\n");
        exit(1);
    }

    retval = pam_start("check_user", user, &conv, &pamh);

    if (retval == PAM_SUCCESS)
        retval = pam_authenticate(pamh, 0);    /* is user really user? */

//    if (retval == PAM_SUCCESS)
//        retval = pam_acct_mgmt(pamh, 0);       /* permitted access? */

    /* This is where we have been authorized or not. */
    if (retval == PAM_SUCCESS) {
        fprintf(stdout, "Authenticated\n");
    } else {
        fprintf(stdout, "Not Authenticated\n");
    }

    if (pam_end(pamh,retval) != PAM_SUCCESS) {     /* close Linux-PAM */
        pamh = NULL;
        fprintf(stderr, "check_user: failed to release authenticator\n");
        exit(1);
    }

    return ( retval == PAM_SUCCESS ? 0:1 );       /* indicate success */
}

#endif