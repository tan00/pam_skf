//
// Created by a8398 on 2020/5/28.
//

#ifndef PAM_ZCWKEY_AUTH_H
#define PAM_ZCWKEY_AUTH_H
#include "SKFAPI.h"
//函数返回0表示成功，其他表示失败


//AUTHFILE 写入key中的数据
typedef struct _AUTHFILE{
    int  inited;  //json:inited
    char user[32+1]; //json:user
    char enc_passwd[64+1];//json:enc_passwd
}AUTHFILE ;

int  AUTHFILE_Marshal(AUTHFILE* authptr, BYTE* serial, int* len);
int   AUTHFILE_Unmarshal(AUTHFILE* authptr,  BYTE* serial);


//AUTHFILE_InitWriteFile 初始化写入认证凭据
int AUTHFILE_InitWriteFile(AUTHFILE* authfile);

//AUTHFILE_Read 读取key内已有的认证凭据
int AUTHFILE_Read(AUTHFILE* authfile);

//AUTHFILE_Write 写入认证凭据
int AUTHFILE_Write(AUTHFILE* authfile);

//AUTHFILE_EncryptPwd 计算pwd的摘要,如果pwd为空，则使用 authfile内的 enc_passwd
int AUTHFILE_EncryptPwd(AUTHFILE* authfile,char* pwd);

//AUTHFILE_CheckArgs 检查内部数据
int AUTHFILE_CheckArgs(AUTHFILE* authfile );






/////////////////////错误码定义////////////////////////
#define ERR_BASE         1000
#define ERR_INITED_FLAG  ERR_BASE+1
#define ERR_USER         ERR_BASE+2
#define ERR_PASSWD       ERR_BASE+3
#define ERR_NULL_PTR     ERR_BASE+100

#endif //PAM_ZCWKEY_AUTH_H