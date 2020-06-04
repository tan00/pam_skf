//
// Created by a8398 on 2020/5/28.
//

#ifndef PAM_ZCWKEY_DEV_MANAGER_H
#define PAM_ZCWKEY_DEV_MANAGER_H
#include "SKFAPI.h"

//设备认证数据
typedef struct _DEVAUTHDATA{
    char       devName[128];
    DEVHANDLE  devhandle;
    HAPPLICATION happlication;
    char appName[128];
    char fileName[128];
    char adminPIN[32];
    char userPIN[32];
}DEVMANAGER;

DEVMANAGER* DEVMANAGER_new();

//获取设备名，如果只有一个设备，则使用该设备名，如果有多个设备，则由用户输入
int DEVMANAGE_PromtDevname(DEVMANAGER* ptr);
//打开设备
int DEVMANAGER_OpenDev(DEVMANAGER* ptr);
//打开应用
int DEVMANAGER_OpenApp(DEVMANAGER* ptr);

//认证pin
int DEVMANAGER_VerifyPIN(DEVMANAGER* ptr);

//恢复出厂设置 清空key内app
int DEVMANAGER_FactoryReset();
//创建默认应用和文件
int DEVMANAGER_FactoryInit();
//提示用户输入管理员pin码
int DEVMANAGE_PromtAdminPin(DEVMANAGER* ptr);

#endif //PAM_ZCWKEY_DEV_MANAGER_H
