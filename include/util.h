//
// Created by a8398 on 2020/5/28.
//

#ifndef PAM_ZCWKEY_UTIL_H
#define PAM_ZCWKEY_UTIL_H
#include "SKFAPI.h"

void ShowErrInfo(DWORD dwErrInfo);
void FlushStdin();
int  Isdigits(char* str);
int BufToHex(const char* buf,int len,char* hex);
int HexToBuf(const char* hex,int len,char* asc);

#endif //PAM_ZCWKEY_UTIL_H




#ifndef __HARD_COMMON_H
#define __HARD_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif



#define SOFT_LOG_ENABLE 1

#if SOFT_LOG_ENABLE
#define SOFT_LOG_EMERGE(fmt, ARGS...)	printf("[EMERGE][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_ALERT(fmt, ARGS...)	printf("[ALERT][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_CRIT(fmt, ARGS...)		printf("[CRIT][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_ERR(fmt, ARGS...)		printf("[ERR][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_WAR(fmt, ARGS...)		printf("[WAR][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_NOTICE(fmt, ARGS...)	printf("[NOTICE][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_INF(fmt, ARGS...)		printf("[INF][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_DEBUG(fmt, ARGS...)	printf("[DEBUG][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#else
#define SOFT_LOG_EMERGE(fmt, ARGS...)	({(void)0, 0;})
#define SOFT_LOG_ALERT(fmt, ARGS...)	({(void)0, 0;})
#define SOFT_LOG_CRIT(fmt, ARGS...)		({(void)0, 0;})
#define SOFT_LOG_ERR(fmt, ARGS...)		printf("[ERR][F:%s][L:%d]: "fmt"\n",__FUNCTION__,__LINE__, ##ARGS)
#define SOFT_LOG_WAR(fmt, ARGS...)		({(void)0, 0;})
#define SOFT_LOG_NOTICE(fmt, ARGS...)	({(void)0, 0;})
#define SOFT_LOG_INF(fmt, ARGS...)		({(void)0, 0;})
#define SOFT_LOG_DEBUG(fmt, ARGS...)	({(void)0, 0;})
#endif





#ifdef __cplusplus
}
#endif
#endif

