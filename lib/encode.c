#include "encode.h"
#include <stddef.h>
#include <iconv.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif


int GB2312ToUTF8( char const *srcStr, char* desBuff, int desBuffLength)
{
    //assert(strlen(srcStr)>0);
    unsigned int iLen = strlen(srcStr);

    iconv_t cd;
    cd = iconv_open("utf-8","gb2312");
    assert(cd != 0);
    int n=iconv(cd, (char **)&srcStr, &iLen, &desBuff, (size_t*)&desBuffLength);
    iconv_close(cd);
    return n;
}

int UTF8ToGB2312( char const *srcStr, char* desBuff, int desBuffLength)
{
   // assert(strlen(srcStr)>0);
    unsigned int iLen = strlen(srcStr);

    iconv_t cd;
    cd = iconv_open("gb2312","utf-8");
    assert(cd != 0);
    int n=iconv(cd, (char **)&srcStr, (size_t*)&iLen, &desBuff, (size_t*)&desBuffLength);
    iconv_close(cd);
    return n;
}

/*代码转换:从一种编码转为另一种编码*/
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
    iconv_t cd;
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);
    if (cd==0) return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
    iconv_close(cd);
    return 0;
}
/*UNICODE码转为GB2312码*/
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}
/*GB2312码转为UNICODE码*/
int g2u(char *inbuf, int inlen,char *outbuf,int outlen)
{
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}



#ifdef __cplusplus
}; //end of extern "C" {
#endif
