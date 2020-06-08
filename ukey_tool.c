//
// Created by a8398 on 2020/6/4.
//
#include "auth.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "include/dev_manager.h"
#include "include/util.h"

const char* usage = "usage: ukey_tool {reset | init} \n\treset: delete app in ukey \n\tinit: create app and file in ukey";
extern const int gMaxRetryCnt;
extern const int gFileLen;


//删除ukey内的app
int DelApp(){
    ULONG ret = 0 ;
    char* errmsg = NULL;
    DEVMANAGER *devman = DEVMANAGER_new();

    ret = DEVMANAGER_DevAuth(devman);
    if (ret != SAR_OK){
        errmsg = "DEVMANAGER_DevAuth";
        goto END;
    }

    char nameList[1024]; memset(nameList,0,sizeof nameList);
    ULONG  ulBufSize = sizeof nameList;

    ret = SKF_EnumApplication(devman->devhandle, nameList, &ulBufSize);
    if (ret != SAR_OK){
        errmsg = "SKF_EnumApplication";
        goto END;
    }

    if( strlen(nameList) == 0 ){
        errmsg = "no app in ukey ";
        goto END;
    }

    ret = SKF_DeleteApplication(devman->devhandle, nameList);
    if (ret != SAR_OK){
        errmsg = "SKF_DeleteApplication";
        goto END;
    }

    END:
    DEVMANAGER_free(devman);
    if (errmsg){ printf("%s \t",errmsg);}
    ShowErrInfo(ret);
    return  ret;
}


//创建默认应用和文件
int FactoryInit() {
    int ret = 0;
    char* errmsg = NULL;

    DEVMANAGER *devman = DEVMANAGER_new();
    ret = DEVMANAGE_PromtDevname(devman);
    if (ret!=SAR_OK){
        goto END;
    }

    DEVMANAGER_OpenDev(devman);

    ret = DEVMANAGER_DevAuth(devman);
    if (ret != SAR_OK){
        errmsg = "DEVMANAGER_DevAuth";
        goto END;
    }

    ret = SKF_CreateApplication(devman->devhandle, devman->appName,\
                                    devman->adminPIN, gMaxRetryCnt,devman->userPIN , gMaxRetryCnt, \
                                    SECURE_USER_ACCOUNT,&devman->happlication);
    if (ret != SAR_OK){
        errmsg = "SKF_CreateApplication";
        goto END;
    }

    ULONG ulRetryCount;
    ret = SKF_VerifyPIN(devman->happlication, USER_TYPE, devman->userPIN, &ulRetryCount);
    if (ret != SAR_OK){
        errmsg = "SKF_VerifyPIN";
        goto END;
    }

    ret = SKF_CreateFile(devman->happlication, (LPSTR)devman->fileName, gFileLen, SECURE_USER_ACCOUNT , SECURE_USER_ACCOUNT);
    if (ret != SAR_OK){
        errmsg = "SKF_VerifyPIN";
        goto END;
    }

    END:
    DEVMANAGER_free(devman);
    if (errmsg){ printf("%s \t",errmsg);}
    ShowErrInfo(ret);
    return  ret;
}


int main(int argc, char* argv[])
{
    if (argc!=2){
        printf("%s \n",usage);
        return 0;
    }
    int ret = 0;

    if ( 0 == strcmp(argv[1], "init") ){
        if ( (ret = FactoryInit())!=0) goto END;
    }else if ( 0 == strcmp(argv[1], "reset") ){
        if ( (ret = DelApp())!=0) goto END;
    }

    END:
    ShowErrInfo(ret);
    return ret;
}


