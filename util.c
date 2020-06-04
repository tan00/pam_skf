//
// Created by a8398 on 2020/5/28.
//

#include "include/util.h"
#include <ctype.h>
#include <stdio.h>

void ShowErrInfo(DWORD dwErrInfo)
{
    printf("\n");
    switch(dwErrInfo)
    {
        case SAR_OK						    :  printf("succeed	\n");	break;
        case SAR_FAIL					    :  printf("failed               \n");	break;
        case SAR_UNKNOWNERR				    :  printf("unexpected error            \n");	break;
        case SAR_NOTSUPPORTYETERR		    :  printf("unsupport service        \n");	break;
        case SAR_FILEERR					:  printf("file error        \n");	break;
        case SAR_INVALIDHANDLEERR		    :  printf("invalid handle          \n");	break;
        case SAR_INVALIDPARAMERR			:  printf("invalid params          \n");	break;
        case SAR_READFILEERR				:  printf("read file error          \n");	break;
        case SAR_WRITEFILEERR			    :  printf("write file error          \n");	break;
        case SAR_NAMELENERR				    :  printf("invalid file name length        \n");	break;
        case SAR_KEYUSAGEERR				:  printf("key usage error        \n");	break;
        case SAR_MODULUSLENERR			    :  printf("modulus len error        \n");	break;
        case SAR_NOTINITIALIZEERR		    :  printf("not initialize            \n");	break;
        case SAR_OBJERR					    :  printf("object error            \n");	break;
        case SAR_MEMORYERR				    :  printf("memory error            \n");	break;
        case SAR_TIMEOUTERR				    :  printf("time out                \n");	break;
        case SAR_INDATALENERR			    :  printf("invalid inputdata len    \n");	break;
        case SAR_INDATAERR				    :  printf("invalid inputdata       \n");	break;
        case SAR_GENRANDERR				    :  printf("gen rand error      \n");	break;
        case SAR_HASHOBJERR				    :  printf("hash object error          \n");	break;
        case SAR_HASHERR					:  printf("hash failed        \n");	break;
        case SAR_GENRSAKEYERR			    :  printf("gen rsa key pair failed       \n");	break;
        case SAR_RSAMODULUSLENERR		    :  printf("invalid rsa modulus len     \n");	break;
        case SAR_CSPIMPRTPUBKEYERR		    :  printf("CSP service import key failed \n");	break;
        case SAR_RSAENCERR				    :  printf("RSA encrypt failed         \n");	break;
        case SAR_RSADECERR				    :  printf("RSA decrypt failed         \n");	break;
        case SAR_HASHNOTEQUALERR			:  printf("HASH value not equal        \n");	break;
        case SAR_KEYNOTFOUNTERR			    :  printf("not found key          \n");	break;
        case SAR_CERTNOTFOUNTERR			:  printf("not found cert          \n");	break;
        case SAR_NOTEXPORTERR			    :  printf("export object failed          \n");	break;
        case SAR_DECRYPTPADERR			    :  printf("padding of decrypt error    \n");	break;
        case SAR_MACLENERR				    :  printf("invalid MAC len         \n");	break;
        case SAR_BUFFER_TOO_SMALL		    :  printf("buffer too small          \n");	break;
        case SAR_KEYINFOTYPEERR			    :  printf("invalid key type        \n");	break;
        case SAR_NOT_EVENTERR			    :  printf("no event     \n");	break;
        case SAR_DEVICE_REMOVED			    :  printf("device removed          \n");	break;
        case SAR_PIN_INCORRECT			    :  printf("PIN incorrect           \n");	break;
        case SAR_PIN_LOCKED				    :  printf("PIN locked           \n");	break;
        case SAR_PIN_INVALID				:  printf("PIN invalid             \n");	break;
        case SAR_PIN_LEN_RANGE			    :  printf("PIN len range         \n");	break;
        case SAR_USER_ALREADY_LOGGED_IN	    :  printf("user already logged in        \n");	break;
        case SAR_USER_PIN_NOT_INITIALIZED   :  printf("pin not initialized  \n");	break;
        case SAR_USER_TYPE_INVALID		    :  printf("invalid PIN type         \n");	break;
        case SAR_APPLICATION_NAME_INVALID   :  printf("invalid application name        \n");	break;
        case SAR_APPLICATION_EXISTS		    :  printf("application exists        \n");	break;
        case SAR_USER_NOT_LOGGED_IN		    :  printf("user not logged in        \n");	break;
        case SAR_APPLICATION_NOT_EXISTS	    :  printf("application not exists          \n");	break;
        case SAR_FILE_ALREADY_EXIST		    :  printf("file already exist        \n");	break;
        case SAR_FILE_NOT_EXIST				:  printf("file not exist			 \n");  break;
        case SAR_NO_ROOM					:  printf("no enough space            \n");	break;
        default								:  printf("unknown error        \n");  	break;
    }
}

void FlushStdin(){
//    int c = 0;
//    while ((c = getchar()) != EOF && c != '\n');
    setbuf(stdin,NULL);
}

int  Isdigits(char* str){
    while (*str != '\n' && *str != '\0'){
        if ( isdigit(*str) == 0 ){
            return 0;
        }
        ++str;
    }
    return 1;
}




static char hexlowtoasc(int xxc)
{
    xxc&=0x0f;
    if (xxc<0x0a) xxc+='0';
    else xxc+=0x37;
    return (char)xxc;
}
static char hexhightoasc(int xxc)
{
    xxc&=0xf0;
    xxc = xxc>>4;
    if (xxc<0x0a) xxc+='0';
    else xxc+=0x37;
    return (char)xxc;
}
static char asctohex(char ch1,char ch2)
{
    char ch;
    if (ch1>='A') ch=(char )((ch1-0x37)<<4);
    else ch=(char)((ch1-'0')<<4);
    if (ch2>='A') ch|=ch2-0x37;
    else ch|=ch2-'0';
    return ch;
}


int BufToHex(const char* buf,int len,char* hex)
{
    int i;

    for (i=0;i<len;i++)
    {
        hex[2*i]   = hexhightoasc(buf[i]);
        hex[2*i+1] = hexlowtoasc(buf[i]);
    }

    return(len*2);
}

int HexToBuf(const char* hex,int len,char* asc)
{
    int i,j;
    if (len % 2 == 0)
        j = len / 2;
    else
        j = len / 2 + 1;

    for (i = 0; i < j; i++)
        asc[i] = asctohex(hex[2*i],hex[2*i+1]);

    return(j);
}