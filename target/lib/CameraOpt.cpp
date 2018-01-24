#include "CCamearOpt.h"
#include <iostream>
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>

#include <math.h>
#include <fstream>

#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "HttpRequest.h"

#define MYLIBAPI extern "C"

using namespace std;

inline BYTE toHex(const BYTE &x);
inline BYTE fromHex(const BYTE &x);
inline string URLEncode(const string &sIn);
inline string Encode(char* Data,int DataByte);
static void GetUrlFun(string sCode,string m_szCameraIp,char* pathpic,string index);
static int CreateDir(const char *sPathName);
static int GBKToUTF8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen);
string ReadFile(char *pathpic, int &pic_len);
inline void *fun(char * const str);
void *fun(char * const str)
{
    int i,j;
    for (i = 0; str[i];) {
        if (str[i]=='\n'||str[i]=='\r' || str[i]=='\t' || str[i]==' ') {
        for (j=i; str[j]; j++) {
            str[j]=str[j+1];
            }
        }
        else i++;
    }
    return str;
}
static int CreateDir(const char *sPathName)
{
    char DirName[256];
    strcpy(DirName, sPathName);
    int i, len = strlen(DirName);
    if (DirName[len-1] != '/') strcat(DirName, "/");
    len = strlen(DirName);
    for (i=1; i<len; i++)
    {
        if (DirName[i] == '/')
        {
            DirName[i] = 0;
            if (access(DirName, NULL) != 0)
            {
                if (mkdir(DirName, 0755) == -1) return -1;
            }
            DirName[i] = '/';
        }
    }
    return 0;
}

 CCameraOpt::CCameraOpt()
 {
     memset(m_szCameraIp, 0, 32);
     m_hCameraHandle = NULL;
     m_fOpen = false;

     m_fRecordOpen = false;
     m_fH264Open = false;
     m_fJpegOpen = false;
     m_iFrames = 0;
 }

CCameraOpt::  ~CCameraOpt()
{
        CLoseCamera();
        m_fOpen = false;
        m_fRecordOpen = false;
        m_fH264Open = false;
        m_fJpegOpen = false;
}

bool  CCameraOpt::OpenCamera(char *szAddr)
{
    if ( szAddr == NULL )
    return false;
    CLoseCamera();

    strncpy(m_szCameraIp, szAddr, 32);
    m_hCameraHandle = HVAPI_OpenEx(m_szCameraIp, NULL );
    if ( m_hCameraHandle != NULL )
    {
        m_fOpen = true;
        return true;
    }
    m_fOpen = false;
    return false;
}

bool  CCameraOpt::CLoseCamera()
{
     if ( m_fOpen && m_hCameraHandle != NULL )
    {
        HVAPI_CloseEx(m_hCameraHandle);
        m_hCameraHandle = NULL;
        m_fOpen = false;
    }
    return true;
}

bool CCameraOpt::OpenRecordStream(char* szCommand)
{
    if ( m_hCameraHandle == NULL )
    return false;

    if ( m_fRecordOpen )
    {
       CloseRecordStream();
       m_fRecordOpen = false;
    }

    HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordBeginCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOBEGIN , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordEndCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOEND , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordBigImageCallBack, this, 0, CALLBACK_TYPE_RECORD_BIGIMAGE , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordSmallImageCallBack, this, 0, CALLBACK_TYPE_RECORD_SMALLIMAGE , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordBinaryImageCallBack, this, 0, CALLBACK_TYPE_RECORD_BINARYIMAGE , NULL );

    if ( S_OK == HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::RecordPlateNoCallBack, this, 0, CALLBACK_TYPE_RECORD_PLATE , NULL ) )
    {
        m_fRecordOpen = true;
    }
    else
    {
        m_fRecordOpen = false;
    }
    return  m_fRecordOpen;
}

bool CCameraOpt::CloseRecordStream()
{
    if ( m_hCameraHandle == NULL )
    return true;

    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_RECORD_INFOBEGIN , NULL ) ;
    HVAPI_SetCallBackEx(m_hCameraHandle,NULL, NULL , 0, CALLBACK_TYPE_RECORD_INFOBEGIN , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_RECORD_INFOEND , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_RECORD_BIGIMAGE , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_RECORD_SMALLIMAGE , NULL );
    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_RECORD_BINARYIMAGE , NULL );
    m_fRecordOpen = false;
    return true;
}


bool CCameraOpt::OpenH264Stream(char* szCommand)
{
    if ( m_hCameraHandle == NULL )
    return false;

    if  ( m_fH264Open )
    {
        CloseH264Stream();
        m_fH264Open = false;
    }

    if ( HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::DeviceH264StreamCallBack, this, 0, CALLBACK_TYPE_H264_VIDEO , NULL ) == S_OK )
    {
        m_fH264Open = true;
    }
    else
    {
        m_fH264Open = false;
    }
    return m_fH264Open;
}


bool CCameraOpt::CloseH264Stream()
{
    if ( m_hCameraHandle == NULL )
    return false;

    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0, CALLBACK_TYPE_H264_VIDEO , NULL ) ;
    m_fH264Open = false;

    return true;
}

bool CCameraOpt::OpenImageStream(char* szCommand)
{
    if ( m_hCameraHandle == NULL )
    return false;

    if ( m_fJpegOpen )
    {
        CloseImageStream();
        m_fJpegOpen = false;
    }

    if ( S_OK == HVAPI_SetCallBackEx(m_hCameraHandle, (void*)CCameraOpt::DeviceJpegStreamCallBack, this, 0, CALLBACK_TYPE_JPEG_FRAME , NULL ) )
    {
        m_fJpegOpen = true;
    }
    else
    {
        m_fJpegOpen = false;
    }
    return m_fJpegOpen;
}
bool CCameraOpt::CloseImageStream()
{
    if ( m_hCameraHandle == NULL )
    return false;

    HVAPI_SetCallBackEx(m_hCameraHandle, NULL, NULL, 0,CALLBACK_TYPE_JPEG_FRAME , NULL );
    m_fJpegOpen = false;
    return true;
}

  bool CCameraOpt::GetLinkInfo(int SatusType, DWORD &linkStatus, DWORD &iTryTime)
  {
      if ( m_hCameraHandle == NULL )
        return false;

    DWORD dwTemp = 0;
   HVAPI_GetConnStatusEx(m_hCameraHandle, SatusType,  &dwTemp);
    linkStatus = dwTemp;
    HVAPI_GetReConnectTimesEx(m_hCameraHandle, SatusType, &dwTemp, false);
    iTryTime = dwTemp;
    return true;
  }

///call back
int   CCameraOpt::RecordBegin(DWORD dwCarID)
{
    return 0;
}
int   CCameraOpt::RecordEnd(DWORD dwCarID)
{
    return 0;
}

int    CCameraOpt::RecordBigImage(DWORD dwCarID,
                                                                     WORD wImageTYpe,
                                                                     WORD wWidth,
                                                                     WORD wHeight,
                                                                     PBYTE pbPIcData,
                                                                     DWORD dwImgDataLen,
                                                                     DWORD dwRecordType,
                                                                     DWORD64 dw64TimeMs)
{
    int iLen = sizeof(dw64TimeMs);
    printf("Len:%d --%I64u\n", iLen, dw64TimeMs );

    time_t mytime_t = (time_t)(dw64TimeMs / 1000);
    struct tm *mytm = localtime(&mytime_t);
    char chTimeFmt[100] = {0};
    strftime(chTimeFmt, 100, "%Y%m%d%H%M%S", mytm);
    printf("%s\n ", chTimeFmt);

    char filePath[1024] = { 0 };
    sprintf(filePath, "./result/%s/", m_szCameraIp);

    CreateDir(filePath);

    if ( pbPIcData!= NULL  && dwImgDataLen != 0 )
    {
        char filename[256] = {0};
        //sprintf(filename, "%s%s_%d_%d.jpg", filePath, chTimeFmt, dwCarID, wImageTYpe);
        sprintf(filename, "%s%d.jpg", filePath, dwCarID);

        int tfdjpg = 0;
        tfdjpg = open(filename, O_WRONLY | O_CREAT, S_IRUSR |S_IWUSR);
        if (tfdjpg)
        {
            write(tfdjpg, pbPIcData, dwImgDataLen);
            close(tfdjpg);
        }
    }

    return 0;
}

int   CCameraOpt::RecordSmallImage(DWORD dwCarID,WORD wWidth,
                                                                         WORD wHeight,
                                                                         PBYTE pbPIcData,
                                                                         DWORD dwImgDataLen,
                                                                         DWORD dwRecordType,
                                                                         DWORD64 dw64TimeMs)
{
    int iLen = 1024 * 1024;
    PBYTE pBuff = new BYTE[iLen];
    if (  HVAPIUTILS_SmallImageToBitmapEx(pbPIcData, wWidth, wHeight, pBuff, &iLen) == S_OK )
    {
         time_t mytime_t = dw64TimeMs / 1000;
        struct tm *mytm = localtime(&mytime_t);
        char chTimeFmt[100] = {0};
        strftime(chTimeFmt, 100, "%Y%m%d%H%M%S", mytm);


        char filePath[1024] = { 0 };
        sprintf(filePath, "./result/%s/", m_szCameraIp);

        CreateDir(filePath);


        char filename[256] = {0};
        sprintf(filename, "%s%s_%d.bmp", filePath, chTimeFmt, dwCarID);

        int tfdjpg = 0;
        tfdjpg = open(filename, O_WRONLY | O_CREAT, S_IRUSR |S_IWUSR);
        if (tfdjpg)
        {
                write(tfdjpg, pBuff, iLen);
                close(tfdjpg);
        }

    }

     if ( pBuff != NULL )
     {
         delete[] pBuff;
         pBuff = NULL;
     }
}

int   CCameraOpt::RecordBinaryImage(DWORD dwCarID,
                                                                         WORD wWidth,
                                                                         WORD wHeight,
                                                                         PBYTE pbPIcData,
                                                                        DWORD dwImgDataLen,
                                                                         DWORD dwRecordType,
                                                                         DWORD64 dw64TimeMs)
 {
    int iLen = 1024 * 500;
    PBYTE pBuff = new BYTE[iLen];
    if ( HVAPIUTILS_BinImageToBitmapEx( pbPIcData, pBuff,  &iLen) == S_OK )
    {
        time_t mytime_t = dw64TimeMs / 1000;
        struct tm *mytm = localtime(&mytime_t);
        char chTimeFmt[100] = {0};
        strftime(chTimeFmt, 100, "%Y%m%d%H%M%S", mytm);

       char filePath[1024] = { 0 };
        sprintf(filePath, "./result/%s/", m_szCameraIp);

        CreateDir(filePath);


        char filename[256] = {0};
        sprintf(filename, "%s%s_%d_bin.bmp", filePath, chTimeFmt, dwCarID);

        int tfdjpg = 0;
        tfdjpg = open(filename, O_WRONLY | O_CREAT, S_IRUSR |S_IWUSR);
        if (tfdjpg)
        {
                write(tfdjpg, pBuff, iLen);
                close(tfdjpg);
        }
    }

    if (pBuff  != NULL )
    {
        delete[] pBuff;
        pBuff = NULL;
    }
    return 0;
}

void GetUrlFun(string sCode,string m_szCameraIp,char* pathpic,DWORD index)
{
                    //sCode = "�A33333";
                    //sCode = sCode.at(0);
                    HttpRequest* Http = new HttpRequest;
                	char* str = (char*)malloc(BUFSIZE);
                	memset(str, 0, BUFSIZE);
                	string pcPlateNo = "plateno="+URLEncode(sCode);
                	//string sUrl = "http://test.dev.365etc.com/car/in?in_out_park_id=30&in_out_license_plate="+pcPlateNo;
                	string sUrl = "http://test.dev.365etc.com/front/Testa/test/?sys=record&"+pcPlateNo+"&ip="+m_szCameraIp;







                	string sUrl2 = "http://test.dev.365etc.com/car/inout_photo/?sys=record&"+pcPlateNo+"&ip="+m_szCameraIp+"&index=";
                	const char* c_Url = sUrl.c_str();
                	printf("GET %s\n", c_Url);
                	if(Http->HttpGet(c_Url,str)) {
                		printf("\n->%s\n\n", str);

                         int i=0;
                            char*temp = strtok(str,"\n");
                            while(temp)
                            {
                                i++;
                                //printf("%d.\n",i);
                                if(i == 15)
                                {
                                    break;
                                }
                                temp = strtok(NULL,"\n");
                            }
                            fun(temp);
                            int code;
                            sscanf(temp,"%d",&code);
printf("\n===== API String ======\n%s\n",temp);
printf("\n===== API Int ======\n%d\n",code);
                        //int number = atoi(str);
                		if(code != -1)
                		{


                		    printf("\n%s  %s ...\n",m_szCameraIp.c_str(),sCode.c_str());

                		    HVAPI_HANDLE_EX hHandle = HVAPI_OpenEx("192.168.10.26",NULL);
                                    if(hHandle == NULL)
                                    {
                                        printf("Camera %s Open Filed\n",m_szCameraIp.c_str());
                                    }
                                    else
                                    {
                                        printf("Camera %s Open Success...\n",m_szCameraIp.c_str());
                                        HRESULT r = HVAPI_TriggerSignal(hHandle,0);
                                        if(r == S_OK)
                                        {
                                            printf("Camera %s Open Success\n",m_szCameraIp.c_str());
                                        }
                                        else
                                        {
                                            printf("Camera %s Open Filed\n",m_szCameraIp.c_str());
                                        }
                                        HVAPI_CloseEx(hHandle);
                                    }

                            sUrl2 = sUrl2 +temp;

                            int pic_len = 0;
                                                    string fileData2 = ReadFile(pathpic,pic_len);
                                                    string fileData = fileData2+"===end===";

                                                    int max = fileData.size();
                                                    printf("strlen -> %d\n",max);

                            //printf("string strlen %d\n",sizeof(fileData));
                                                    //unsigned uLen = strlen(fileData);
                                                    //printf("strlen %d\n",uLen);
                                                    //printf("%s\n",fileData.c_str());
                                                    int kuai = 20480;
                                                    double d_count = (double)max/(double)kuai;

                                                    printf("d_count strlen -> %f\n",d_count);

                                                    int run_count = ceil(d_count);

                                                    printf("run_count strlen -> %d\n",run_count);

                                                    for(int i=0; i<run_count;i++)
                                                    {
                                                        int s = i*kuai;
                                                        //int e = 10240;
                                                        //if()

                                                        string fileData2 = fileData.substr(s,kuai);
                                                        const char* c_Url2 = sUrl2.c_str();
                                                        const char* c_fileData=  fileData2.c_str();
                                                        char* str2 = (char*)malloc(BUFSIZE);
                                                        memset(str2, 0, BUFSIZE);
                                                        if(Http->HttpPost(c_Url2,c_fileData,str2)) {
                                                            printf("C++\ns=>%s\n\n", str2);
                                                        }
                                                    }
                                                    printf("HttpPost Url %s\n",sUrl2.c_str());
                                                                        printf("HttpPost File %s\n",pathpic);
                		}
                		else
                		{
                		    printf("\n%s no thoroughfare %s!\n",m_szCameraIp.c_str(),sCode.c_str());
                		}

                	} else {
                		printf("%s HttpGet Filed!\n",c_Url);
                	}



                	//int pic_len = 0;
                    //char* fileData = ReadFile(pathpic,pic_len);



                	//if(Http->HttpPost(sUrl2.c_str(),fileData,str)) {
                    //    printf("\n->%s\n\n", str);
                	//}else {
                    //       printf("%s HttpPost Filed!\n",sUrl2.c_str());
                                     	//}
            free(str);
}


string ReadFile(char *pathpic, int &pic_len){
             printf("open file2 => %s \n",pathpic);
                  filebuf *pbuf;
                    ifstream filestr;
                    long size;
                    char * buffer;
                    string file_path = pathpic;
                    // 要读入整个文件，必须采用二进制打开
                    filestr.open (pathpic, ios::binary);
                    // 获取filestr对应buffer对象的指针
                    pbuf=filestr.rdbuf();

                    // 调用buffer对象方法获取文件大小
                    size=pbuf->pubseekoff (0,ios::end,ios::in);
                    pbuf->pubseekpos (0,ios::in);

                    // 分配内存空间
                    buffer=new char[size];

                    // 获取文件内容
                    pbuf->sgetn (buffer,size);

                    filestr.close();

                    //printf("open file1 => %d \n",strlen(buffer));

                    //printf("open file2 => %d \n",size);
                    //printf("open file3 => %d \n",sizeof(buffer));

                    //printf("open => %s \n",buffer);

             string file_co = Encode(buffer,size);
                    // 输出到标准输出
                    //cout.write (buffer,size);
                 return file_co;
                         /*
                         printf("open file => %s \n",pathpic);

                          //将图片读取出来
                          FILE *fp = fopen(pathpic, "rb");     //打开文件
                          if (!fp){
                              printf("没有找到文件位置 %s",pathpic);
                              return NULL;
                          }
                          fseek(fp, 0, SEEK_END);  //一直寻找到文件尾部
                           pic_len = ftell(fp);  //得到图片的长度
                          rewind(fp);  //rewind将文件指针指向开头

                          printf("file open len %d\n",pic_len);

                          char *pic_buf = new char[pic_len + 1];  //开辟一个空间在堆上
                          memset(pic_buf, 0, pic_len + 1);  //清空文件指针
                          //读取文件内容
                          fread(pic_buf,sizeof(char),pic_len,fp);


                             printf("file open  %s\n",pic_buf);
                          fclose(fp);

                          return pic_buf;
                          */

         }
         string Encode(char* Data,int DataByte)
         {
             //编码表
             const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
             //返回值
             string strEncode;
             unsigned char Tmp[4]={0};
             int LineLength=0;
             for(int i=0;i<(int)(DataByte / 3);i++)
             {
                 Tmp[1] = *Data++;
                 Tmp[2] = *Data++;
                 Tmp[3] = *Data++;
                 strEncode+= EncodeTable[Tmp[1] >> 2];
                 strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
                 strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
                 strEncode+= EncodeTable[Tmp[3] & 0x3F];
                 if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
             }
             //对剩余数据进行编码
             int Mod=DataByte % 3;
             if(Mod==1)
             {
                 Tmp[1] = *Data++;
                 strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
                 strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
                 strEncode+= "==";
             }
             else if(Mod==2)
             {
                 Tmp[1] = *Data++;
                 Tmp[2] = *Data++;
                 strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
                 strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
                 strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
                 strEncode+= "=";
             }

             return strEncode;
         }
  int   CCameraOpt::RecordPlateNo(DWORD dwCarID,LPCSTR pcPlateNo, LPCSTR pcAppendInfo,DWORD dwRecordType,DWORD64 dw64TimeMs)
  {
     time_t mytime_t = dw64TimeMs / 1000;
    struct tm *mytm = localtime(&mytime_t);
    char chTimeFmt[100] = {0};
    strftime(chTimeFmt, 100, "%Y-%m-%d %H:%M:%S", mytm);

    char filePath[1024] = { 0 };
    sprintf(filePath, "./result/%s/", m_szCameraIp);
    CreateDir(filePath);

     char filename[256] = {0};
    sprintf(filename, "%sRecord.txt", filePath);


        printf("ͨ�� %s ʶ�𵽳��� %s!\n",m_szCameraIp,pcPlateNo);





        //printf("ͨ�� %s ʶ�𵽳��� %s!\n",m_szCameraIp,lpUTF8Str);




        char filePath2[1024] = { 0 };
        sprintf(filePath2, "./result/%s/", m_szCameraIp);
        char filename2[256] = {0};
        sprintf(filename2, "%s%d.jpg", filePath2, dwCarID);
        //string index = dwCarID;
        GetUrlFun(pcPlateNo,m_szCameraIp,filename2,dwCarID);

        /*
        HttpRequest* Http = new HttpRequest;
    	char* str = (char*)malloc(BUFSIZE);
    	memset(str, 0, BUFSIZE);
    	string sUrl = "http://www.baidu.com?k="+pcPlateNo;

        printf("GET %s\n", sUrl);

    	const char* c_Url = sUrl.c_str();

    	if(Http->HttpGet(c_Url, str)) {
    		printf("%s\n", str);
    	} else {
    		printf("%s HttpGet ����ʧ��!\n",sUrl);
    	}
    	free(str);
    	*/



    FILE *myfile = NULL;
    myfile = fopen(filename, "a+");
    if (myfile)
    {
        fprintf(myfile, "%s\n",chTimeFmt);
        fprintf(myfile, "%s\n", pcPlateNo);
        fprintf(myfile, "%s\n\n", pcAppendInfo);
        fclose(myfile);
        myfile = NULL;
    }
    return 0;
  }

typedef unsigned char BYTE;

    inline BYTE toHex(const BYTE &x)
    {
        return x > 9 ? x -10 + 'A': x + '0';
    }

    inline BYTE fromHex(const BYTE &x)
    {
        return isdigit(x) ? x-'0' : x-'A'+10;
    }

    inline string URLEncode(const string &sIn)
    {
        string sOut;
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {
            BYTE buf[4];
            memset( buf, 0, 4 );
            if( isalnum( (BYTE)sIn[ix] ) )
            {
                buf[0] = sIn[ix];
            }
            //else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
            //{
            //    buf[0] = '+';
            //}
            else
            {
                buf[0] = '%';
                buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
                buf[2] = toHex( (BYTE)sIn[ix] % 16);
            }
            sOut += (char *)buf;
        }
        return sOut;
    };

int GBKToUTF8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen)
{

}




  int   CCameraOpt::DeviceStatus(LPCSTR pString, DWORD dwStrLen)
  {
      return 0;
  }

  int   CCameraOpt::DeviceJpegStream(PBYTE pbImageData,DWORD dwImageDataLen,DWORD dwImageType, LPCSTR szImageExtInfo)
  {
      return 0;
  }

  int    CCameraOpt::DeviceH264Stream(PBYTE pbVideoData,DWORD dwVIdeoDataLen,DWORD dwVIdeoType,LPCSTR szVideoExtInfo)
  {
      time_t  tt  = time(NULL);
    struct tm *mytm = localtime(&tt);
    char chTimeFmt[100] = {0};
    char chFileTime[100] = { 0 };
    strftime(chTimeFmt, 100, "%Y%m%d%H%M", mytm);
    strftime(chFileTime, 100, "%Y%m%d%H%M%S", mytm);


    char filePath[1024] = { 0 };
    sprintf(filePath, "./result/%s/VideoH264/%s/", m_szCameraIp, chTimeFmt);

    CreateDir(filePath);

    if ( pbVideoData != NULL  && dwVIdeoDataLen != 0 )
    {
        char filename[256] = {0};
        sprintf(filename, "%s%s_%d_%d.H264", filePath, chFileTime, m_iFrames++);

        int tfdjpg = 0;
        tfdjpg = open(filename, O_WRONLY | O_CREAT, S_IRUSR |S_IWUSR);
        if (tfdjpg)
        {
            write(tfdjpg, pbVideoData, dwVIdeoDataLen);
            close(tfdjpg);
        }
    }
  }



