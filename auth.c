//
// Created by a8398 on 2020/5/28.
//

#include "auth.h"
#include "include/dev_manager.h"
#include "include/util.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include "include/cJSON.h"
#include "sm3.h"

extern const char *gAppName;
extern const char *gFileName;
extern const char *gAdminPIN;
extern const char *gUserPIN;

///////////////////////////////////////////////////////
//AUTHFILE_InitWriteFile 初始化写入认证凭据
int AUTHFILE_InitWriteFile(AUTHFILE *authfile) {
    int ret = 0;
    if ((ret = AUTHFILE_CheckArgs(authfile)) != 0) {
        return ret;
    }

    if (authfile->inited != 0) {
        return ERR_INITED_FLAG;
    }

    DEVMANAGER *devman = DEVMANAGER_new();
    DEVMANAGE_PromtDevname(devman);

    ret = DEVMANAGER_FactoryReset(devman);
    if (ret != 0) {

    }
    ret = DEVMANAGER_FactoryInit(devman);

    printf("InitWriteFile:: init authfile write byte : %d \n", ret);

    return 0;
}


int AUTHFILE_CheckArgs(AUTHFILE *authfile) {
    if (authfile == NULL) {
        return ERR_NULL_PTR;
    }
    if (strlen(authfile->user) == 0) {
        return ERR_USER;
    }
    if (strlen(authfile->enc_passwd) == 0) {
        return ERR_PASSWD;
    }
    return 0;
}


int AUTHFILE_Read(AUTHFILE *authfile) {
    int ret = 0;
    DEVMANAGER *devman = DEVMANAGER_new();
    DEVMANAGE_PromtDevname(devman);
    DEVMANAGER_OpenDev(devman);
    DEVMANAGER_OpenApp(devman);

    ret = DEVMANAGER_VerifyPIN(devman);
    if (ret != SAR_OK) {
        SOFT_LOG_ERR("DEVMANAGER_VerifyPIN error  ret= %x\n", ret);
        return ret;
    }

    unsigned char jsonStr[1024];
    memset(jsonStr, 0, sizeof jsonStr);
    int size = sizeof jsonStr;
    //获取文件有效数据大小
    ret = SKF_ReadFile(devman->happlication, devman->fileName, 0, sizeof jsonStr, NULL, &size);
    if (ret != SAR_OK) {
        SOFT_LOG_ERR("SKF_ReadFile ret = %d \n", ret);
        return ret;
    }

    ret = SKF_ReadFile(devman->happlication, devman->fileName, 0, size, jsonStr, &size);
    if (ret != SAR_OK) {
        SOFT_LOG_ERR("SKF_ReadFile ret = %d \n", ret);
        return ret;
    }
    SOFT_LOG_DEBUG("SKF_ReadFile readlen = %d \n  jsonStr = %s \n", size, jsonStr);

    AUTHFILE_Unmarshal(authfile, jsonStr);

    return ret;
}


int AUTHFILE_Write(AUTHFILE *authfile) {
    int ret = 0;
    DEVMANAGER *devman = DEVMANAGER_new();
    DEVMANAGE_PromtDevname(devman);
    DEVMANAGER_OpenDev(devman);
    DEVMANAGER_OpenApp(devman);

    ret = DEVMANAGER_VerifyPIN(devman);
    if (ret != SAR_OK) {
        SOFT_LOG_ERR("DEVMANAGER_VerifyPIN error  ret= %x\n", ret);
        return ret;
    }

    unsigned char jsonStr[1024];
    int size = sizeof jsonStr;

    AUTHFILE_EncryptPwd(authfile,NULL);
    AUTHFILE_Marshal(authfile, jsonStr, &size);

    ret = SKF_WriteFile(devman->happlication, devman->fileName, 0, jsonStr, size);
    if (ret != SAR_OK) {
        SOFT_LOG_ERR("SKF_WriteFile ret = %d \n", ret);
        return ret;
    }
    SOFT_LOG_DEBUG("SKF_WriteFile ret = %d \n", ret);

    return 0;
}

//AUTHFILE_EncryptPwd 计算pwd的摘要
int AUTHFILE_EncryptPwd(AUTHFILE *authfile, char *pwd) {
    char *tpwd = NULL;
    unsigned  char digest[33];
    memset(digest,0,sizeof digest);

    if (pwd != NULL) {
        tpwd = pwd;
    } else if (strlen(authfile->enc_passwd) != 0) {
        tpwd = authfile->enc_passwd;
    }else{
        return -1;//没有可用的密码
    }
    sm3(tpwd, strlen(tpwd),digest);

    BufToHex(digest,32,authfile->enc_passwd);
    return 0;
}

//AUTHFILE结构体导出为json字符串
int AUTHFILE_Marshal(AUTHFILE *authptr, BYTE *buf, int *len) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "inited", cJSON_CreateNumber(authptr->inited));
    cJSON_AddItemToObject(root, "user", cJSON_CreateString(authptr->user));
    cJSON_AddItemToObject(root, "enc_passwd", cJSON_CreateString(authptr->enc_passwd));

    char *dumpstr = cJSON_Print(root);
    if (buf == NULL) {
        *len = strlen(dumpstr);
        return 0;
    }
    if (*len <= strlen(dumpstr)) {
        return -1;
    }
    strcpy(buf, dumpstr);
    *len = strlen(dumpstr);
    free(dumpstr);
    cJSON_Delete(root);
    return 0;
}

//json字符串导入为AUTHFILE结构体
int AUTHFILE_Unmarshal(AUTHFILE *authptr, BYTE *serial) {
    cJSON *root = cJSON_Parse(serial);

    authptr->inited = cJSON_GetObjectItem(root, "inited")->valueint;
    strcpy(authptr->user, cJSON_GetObjectItem(root, "user")->valuestring);
    strcpy(authptr->enc_passwd, cJSON_GetObjectItem(root, "enc_passwd")->valuestring);

    return 0;
}

