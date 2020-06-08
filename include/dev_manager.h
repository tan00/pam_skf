//
// Created by a8398 on 2020/5/28.
//

#ifndef PAM_ZCWKEY_DEV_MANAGER_H
#define PAM_ZCWKEY_DEV_MANAGER_H
#include "SKFAPI.h"

//设备认证数据
typedef struct _DEVAUTHDATA{
    char       devName[33];
    DEVHANDLE  devhandle;
    HAPPLICATION happlication;
    char appName[33];
    char fileName[33];
    char adminPIN[33];
    char userPIN[33];
}DEVMANAGER;

DEVMANAGER* DEVMANAGER_new();
void DEVMANAGER_free(DEVMANAGER* devman);

//获取设备名，如果只有一个设备，则使用该设备名，如果有多个设备，则由用户输入
int DEVMANAGE_PromtDevname(DEVMANAGER* ptr);
//打开设备
int DEVMANAGER_OpenDev(DEVMANAGER* ptr);
//打开应用
int DEVMANAGER_OpenApp(DEVMANAGER* ptr);

//认证pin
int DEVMANAGER_VerifyPIN(DEVMANAGER* ptr);

//提示用户输入管理员pin码
int DEVMANAGE_PromtAdminPin(DEVMANAGER* ptr);

//设备认证
int DEVMANAGER_DevAuth(DEVMANAGER *devman);

#endif //PAM_ZCWKEY_DEV_MANAGER_H
