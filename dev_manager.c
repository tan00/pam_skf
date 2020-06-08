//
// Created by a8398 on 2020/5/28.
//

#include "include/dev_manager.h"
#include "include/SKFAPI.h"
#include "include/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char *gAppName = "appAuth";
//const char *gAppName = "myapp";
const char *gFileName = "fileAuth";
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

void DEVMANAGER_free(DEVMANAGER* devman){
    if (devman==NULL){
        return;
    }
    if(devman->happlication){
        SKF_CloseApplication(devman->happlication);
        devman->happlication = NULL;
    }
    if(devman->devhandle){
        SKF_CloseHandle(devman->devhandle);
        devman->devhandle = NULL;
    }
    free(devman);
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
    if ( strlen(ptr->devName) != 0 ){
        return 0;
    }

    ULONG ulReval = SAR_FAIL;
    char nameList[1024];
    memset(nameList, 0, sizeof nameList);
    ULONG ulBufSize = sizeof nameList;

    //枚举设备
    ulReval = SKF_EnumDev(TRUE, nameList, &ulBufSize);
    if(ulReval!=0){
        printf("no ukey found \n");
        return ulReval;
    }

    int devCnt = 0;
    char devList[32][128];
    memset(devList, 0, sizeof devList);

    //获取设备数量
    singleListToMultiList(nameList, devList, &devCnt);

    //如果只有一个设备
    if (devCnt == 1) {
        strcpy(ptr->devName, devList[0]);
    } else if(devCnt > 1){//有多个设备， 提示用户输入
        printf("found %d usb device \n",devCnt);
        int i= 0;
        for(;i<devCnt;i++){
            printf("%d. %s\n",(i+1),devList[i]);
        }
        printf("input the device num : ");
        int choice = 0;
        scanf("%d",&choice);
        choice -= 0x30;//转换为数字
        choice -= 1;
        if(choice<0 || choice>=devCnt){
            printf("error choice  \n");
            return -1;
        }
        strcpy(ptr->devName, devList[choice]);
    }else{
        printf("no ukey dev found \n");
        return -1;
    }
    return 0;
}

//DEVMANAGER_OpenDev 获取设备句柄
//如果只有一个设备，则打开该设备  如果有多个则询问用户选择设置
int DEVMANAGER_OpenDev(DEVMANAGER *ptr) {
    if (ptr->devhandle != NULL){
        return 0;
    }
    ULONG ulReval = SAR_FAIL;
    ulReval = DEVMANAGE_PromtDevname(ptr);
    if (ulReval!=SAR_OK){
        return ulReval;
    }


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
    ULONG reCnt = 0;
    ULONG ulReval = SKF_VerifyPIN(ptr->happlication,USER_TYPE ,ptr->userPIN,&reCnt);
    if (ulReval != SAR_OK) {
        ShowErrInfo(ulReval);
        return ulReval;
    }
    return 0;
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



//设备认证
int DEVMANAGER_DevAuth(DEVMANAGER *devman){
    ULONG ret = 0 ;
    char* errmsg = NULL;
    DEVINFO devInfo;

    ret = DEVMANAGE_PromtDevname(devman);
    if(ret!=0){
        return ret;
    }
    DEVMANAGER_OpenDev(devman);

    BYTE random[16] = {0};
    ret = SKF_GetDevInfo(devman->devhandle, &devInfo);
    if (ret != SAR_OK){
        errmsg = "SKF_GetDevInfo";
        goto END;
    }

    ret = SKF_GenRandom(devman->devhandle, random, 8);
    if (ret != SAR_OK){
        errmsg = "SKF_GenRandom";
        goto END;
    }

    BYTE devKey[16] = {0};
    memcpy(devKey, (BYTE*)"1234567812345678", 16);
    HANDLE SessionKey = NULL;
    ret = SKF_SetSymmKey(devman->devhandle, devKey, devInfo.DevAuthAlgId, &SessionKey);
    if (ret != SAR_OK){
        errmsg = "SKF_SetSymmKey";
        goto END;
    }

    BLOCKCIPHERPARAM param;
    memset(&param, 0 ,sizeof param);
    ret = SKF_EncryptInit(SessionKey, param);
    if (ret != SAR_OK){
        errmsg = "SKF_EncryptInit";
        goto END;
    }

    BYTE devkeyenc[16] = {0};
    DWORD dwResultLen = 16;
    ret = SKF_Encrypt(SessionKey, random, 16, devkeyenc, &dwResultLen);
    if (ret != SAR_OK){
        errmsg = "SKF_Encrypt";
        goto END;
    }

    ret = SKF_DevAuth(devman->devhandle, devkeyenc, 16);
    if (ret != SAR_OK){
        errmsg = "SKF_DevAuth";
        goto END;
    }

    END:
    if (errmsg){ printf("%s \t",errmsg);}
    ShowErrInfo(ret);
    return  ret;
}
