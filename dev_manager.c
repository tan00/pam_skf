//
// Created by a8398 on 2020/5/28.
//

#include "include/dev_manager.h"
#include "include/SKFAPI.h"
#include "include/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//const char *gAppName = "appNameLoginAuth";
const char *gAppName = "myapp";
const char *gFileName = "fileNameLoginAuth";
const char *gAdminPIN = "zcwadminpin";
//const char *gUserPIN = "zcwuserpin";
const char *gUserPIN = "123456";

const int gMaxRetryCnt = 6;
const int gFileLen = 128;

DEVMANAGER *DEVMANAGER_new() {
    DEVMANAGER *ptr = calloc(sizeof(DEVMANAGER), 1);
    strcpy(ptr->appName, gAppName);
    strcpy(ptr->fileName, gFileName);
    strcpy(ptr->adminPIN, gAdminPIN);
    strcpy(ptr->userPIN, gUserPIN);
    return ptr;
}


int singleListToMultiList(char *list, char newlist[32][128], int *rownum) {
    char *tmpnameList = list;
    int devCnt = 0;
    //获取设备数量
    while (*tmpnameList != '\0') {
        strcpy(newlist[devCnt], tmpnameList);
        printf("get device name : %s\n", tmpnameList);
        tmpnameList += strlen((const char *) tmpnameList) + 1;
        ++devCnt;
    }
    *rownum = devCnt;
    return 0;
}


int DEVMANAGE_PromtDevname(DEVMANAGER *ptr) {
    ULONG ulReval = SAR_FAIL;
    char nameList[1024];
    memset(nameList, 0, sizeof nameList);
    ULONG ulBufSize = sizeof nameList;

    //枚举设备
    ulReval = SKF_EnumDev(TRUE, nameList, &ulBufSize);

    int devCnt = 0;
    char devList[32][128];
    memset(devList, 0, sizeof devList);

    //获取设备数量
    singleListToMultiList(nameList, devList, &devCnt);

    //如果只有一个设备
    if (devCnt == 1) {
        strcpy(ptr->devName, devList[0]);
    } else {//有多个设备， 提示用户输入
        //todo
    }
    return 0;
}

//DEVMANAGER_OpenDev 获取设备句柄
//如果只有一个设备，则打开该设备  如果有多个则询问用户选择设置
int DEVMANAGER_OpenDev(DEVMANAGER *ptr) {
    ULONG ulReval = SAR_FAIL;
    ulReval = SKF_ConnectDev((LPSTR) ptr->devName, &ptr->devhandle);
    if (ulReval != SAR_OK) {
        ShowErrInfo(ulReval);
        return ulReval;
    }
    return 0;
}

//获取应用句柄
int DEVMANAGER_OpenApp(DEVMANAGER *ptr) {
    ULONG ulReval = SKF_OpenApplication(ptr->devhandle, ptr->appName, &ptr->happlication);
    if (ulReval != SAR_OK) {
        ShowErrInfo(ulReval);
        return ulReval;
    }
    return 0;
}

//认证pin
int DEVMANAGER_VerifyPIN(DEVMANAGER* ptr){
    int reCnt = 0;
    ULONG ulReval = SKF_VerifyPIN(ptr->happlication,USER_TYPE ,ptr->userPIN,&reCnt);
    if (ulReval != SAR_OK) {
        ShowErrInfo(ulReval);
        return ulReval;
    }
    return 0;
}

//恢复出厂设置 清空key内app
int DEVMANAGER_FactoryReset() {
    int ret = 0;
    int MaxRetryCnt = 0;
    ULONG ulReval = SAR_FAIL;
    char nameList[1024];
    memset(nameList, 0, sizeof nameList);
    ULONG ulBufSize = sizeof nameList;
    int appCnt = 0;
    char appList[32][128];
    memset(appList, 0, sizeof appList);

    DEVMANAGER *devman = DEVMANAGER_new();
    DEVMANAGE_PromtDevname(devman);
    DEVMANAGER_OpenDev(devman);

    DEVMANAGE_PromtAdminPin(devman);


    printf("get admin pin : %s \n" , devman->adminPIN);

    //todo 只能解锁用户pin ， 如果遇到管理员PIN也锁住了 该怎么办？
    ret = SKF_UnblockPIN(devman->happlication, devman->adminPIN, devman->userPIN, &MaxRetryCnt);
    if (ret != SAR_OK) {
        ShowErrInfo(ulReval);
        return ret;
    }

    ret = SKF_EnumApplication(devman->devhandle, nameList, &ulBufSize);
    if (ret != SAR_OK) {
        ShowErrInfo(ulReval);
        return ret;
    }

    singleListToMultiList(nameList, appList, &appCnt);

    int i = 0;
    for (; i < appCnt; i++) {
        SKF_DeleteApplication(devman->devhandle, appList[i]);
    }

    SKF_CloseHandle(devman->devhandle);
    devman->devhandle = NULL;
    return 0;
}

//创建默认应用和文件
int DEVMANAGER_FactoryInit() {
    int ret = 0;
    DEVMANAGER *devman = DEVMANAGER_new();
    DEVMANAGE_PromtDevname(devman);
    DEVMANAGER_OpenDev(devman);

    ret = SKF_CreateApplication(devman->devhandle, devman->appName, \
            devman->adminPIN, gMaxRetryCnt, devman->userPIN, gMaxRetryCnt, \
            SECURE_USER_ACCOUNT, &devman->happlication);
    if (ret != SAR_OK) {
        ShowErrInfo(ret);
        return ret;
    }

    ret = SKF_CreateFile(devman->happlication, devman->fileName, gFileLen, SECURE_USER_ACCOUNT, SECURE_USER_ACCOUNT);
    if (ret != SAR_OK) {
        ShowErrInfo(ret);
        return ret;
    }
    return ret;
}


//提示用户输入管理员pin码
int DEVMANAGE_PromtAdminPin(DEVMANAGER *ptr) {
    char adminPin[1024];

    int i = 0;
    for (; i < 3; i++) {
        memset(adminPin, 0, sizeof adminPin);
        FlushStdin();
        printf("input admin PIN:");
        scanf("%s", adminPin);
        if (1 == Isdigits(adminPin)) {
            strcpy(ptr->adminPIN, adminPin);
            return 0;
        }
        printf("your input is not nums ");
    }
    return -1;
}