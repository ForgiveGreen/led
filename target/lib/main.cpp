#include "main.h"
#include <iostream>
#include <iconv.h>
#include<stdio.h>
#include<string.h>
#include <fstream>
#include <math.h>
#include "HvDeviceEx.h"
#include "CCamearOpt.h"
#include "HttpRequest.h"

#define MYLIBAPI extern "C"



using namespace std;

MYLIBAPI void OpenTriggerFun();
MYLIBAPI string GetUrlApi(string sUrl);
MYLIBAPI string GetUrlIn(string sUrl);
MYLIBAPI string GetUrlOut(string sUrl);
MYLIBAPI string GetUrlHttp(string sUrl);
string Encode(char* Data,int DataByte);
inline BYTE toHex(const BYTE &x);
inline BYTE fromHex(const BYTE &x);
inline string URLEncode(const string &sIn);
inline string ReadFile2(char *pathpic, int &pic_len);
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
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
    char *outbuf, size_t outlen);
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
    char *outbuf, size_t outlen) {
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset, from_charset);
    if (cd == 0)
        return -1;
    memset(outbuf, 0, outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
        return -1;
    iconv_close(cd);
    *pout = '\0';

    return 0;
}

int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
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
string ReadFile2(char *pathpic, int &pic_len){

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
MYLIBAPI void ConnectDevicesFun(char* szAddr);

int FindEmptyPos();
void ShowDeviceStatus();
void CloseDevices();

const int MAX_CAMERA_COUNT = 100;
static DWORD64 g_DeviceMac[100] = { 0 };
static CCameraOpt *g_CameraArray[100] = {NULL};

int FindEmptyPos()
{
    int index = -1;
    for ( int i=0; i<MAX_CAMERA_COUNT; i++ )
    {
        if (g_CameraArray[i] == NULL )
        {
            index = i;
            break;
        }
    }
    return index;
}

bool TestExistObj(char* szAddr)
{
    bool bRet = false;
    for ( int i=0; i<MAX_CAMERA_COUNT; i++)
    {
        if (g_CameraArray[i] == NULL )
        continue ;

        char *szTemp = g_CameraArray[i]->GetDeviceIp();
        if ( strcmp(szTemp, szAddr) == 0 )
        {
            bRet = true;
            break;
        }
    }
    return bRet;
}



void ShowAllDevice();
 void MyGetIPDWORD( char *chTempIP, DWORD32 &dwIP)
    {
        DWORD32 temp[4] = {0};
        sscanf(chTempIP, "%d.%d.%d.%d", temp+3, temp+2, temp+1, temp);
        dwIP = (temp[3]<<24) | (temp[2]<<16) | (temp[1]<<8) | temp[0];
        return ;
    }

    void MyGetIPString(DWORD32 dwIP, char *chIp)
    {
        unsigned char * pByte = (unsigned char*)&dwIP;
        sprintf(chIp, "%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
        return ;
    }

    void MyGetMacString(DWORD64 dwMac, char *chMac)
    {
        if (chMac == NULL)
            return ;
        unsigned  char *pByte = (unsigned char *)&dwMac;
        sprintf(chMac, "%02X-%02X-%02X-%02X-%02X-%02X", pByte[0], pByte[1],pByte[2], pByte[3], pByte[4],pByte[5]);
        return ;
    }


int main()
{
    while(true)
    {




        printf("Select Function\n");
        printf("1---�������������������ͷ\n");
        printf("2---����IP��ַ��������ͷ������Ƶ���ص�\n");
        printf("3---��ʾ�豸״̬\n");
        printf("4---�������\n");
        printf("5---�˳�\n");
        int iSelect = -1;
        scanf("%d", &iSelect);
        switch(iSelect)
        {
            case 1:
            {
                 ShowAllDevice();
            }
            break ;
            case 2:
            {
                GetUrlIn("http://test.dev.365etc.com/front/Testa/test/?sys=record&plateno=��A13333&ip=123");
                //ConnectDevicesFun("172.16.2.100");
            }
            break;
            case 3:
            {
                ShowDeviceStatus();
            }
            break;
            case 4:
            {

                 HVAPI_HANDLE_EX hHandle = HVAPI_OpenEx("192.168.10.26",NULL);
        if(hHandle == NULL)
        {
            printf("Camera 192.168.10.26 Open Filed\n\n");
        }
        else
        {
            printf("Camera 192.168.10.26 Open Success\n\n");
            HRESULT r = HVAPI_TriggerSignal(hHandle,0);
            if(r == S_OK)
            {
                printf("Camera 192.168.10.26 Open Passageway Success\n\n");
            }
            else
            {
                printf("Camera 192.168.10.26 Open Passageway Filed\n\n");
            }
        }
                 //CloseDevices();
            }
            break ;
            case 5:
                goto Quit;
            default:
            break;
        }
    }
Quit:
    return 0;
}
void OpenTriggerFun()
{
        HVAPI_HANDLE_EX hHandle = HVAPI_OpenEx("192.168.10.26",NULL);
        if(hHandle == NULL)
        {
            printf("Camera2 192.168.10.26 Open Filed\n\n");
        }
        else
        {
            printf("Camera2 192.168.10.26 Open Success\n\n");
            HRESULT r = HVAPI_TriggerSignal(hHandle,0);
            if(r == S_OK)
            {
                printf("Camera 192.168.10.26 Open Passageway Success\n\n");
            }
            else
            {
                printf("Camera 192.168.10.26 Open Passageway Filed\n\n");
            }
            HVAPI_CloseEx(hHandle);
        }
}
string GetUrlApi(string sUrl)
{
     string sUrl1 = "http://test.dev.365etc.com/front/Testa/test/?sys=Init";
     return GetUrlHttp(sUrl1);
}
string GetUrlIn(string sUrl)
{

    string sTemp = URLEncode("��A23333");
    string sUrl1 = "http://test.dev.365etc.com/front/Testa/test/?sys=record&plateno="+sTemp+"&ip=192.168.10.99";
    return GetUrlHttp(sUrl1);
}
string GetUrlOut(string sUrl)
{
    string sTemp = URLEncode("��A33333");
    string sUrl1 = "http://test.dev.365etc.com/car/out/?in_out_license_plate="+sTemp+"&in_out_park_id=30";
    return GetUrlHttp(sUrl1);
}

string GetUrlHttp(string sUrl)
{
                        HttpRequest* Http = new HttpRequest;
                    	char* str = (char*)malloc(BUFSIZE);
                    	memset(str, 0, BUFSIZE);
                    	//string pcPlateNo = "PlateNo=123";
                    	//string sUrl = "http://test.dev.365etc.com/front/Testa/test/?sys=Init";
                    	const char* c_Url = sUrl.c_str();
                    	printf("C++ GET %s\n", c_Url);
                    	if(Http->HttpGet(c_Url,str)) {


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





printf("\n===========\n%s\n===========\n",temp);
printf("\n===========\n%d\n===========\n",code);
if(code == -1)
{
    printf("\n===========ERROR===========\n");
}


                            if(strstr(str,",s=1,") != NULL)
                            {
                                printf("Success!\n");
                            }
                            else{
                            printf("Filed!\n");
                            }


                    		printf("C++\ns=>%s\n\n", str);
                    		//printf("C++\nd=>%d\n\n", number);
                    	}
                    	else
                    	{
                    		printf("C++\n\n%s HttpGet Request Filed!\n",c_Url);
                    	}
                    	//free(str);



                        printf("===================================\n");

                    	char* pathpic = "./result/192.168.10.99/10.jpg";
                    	string sUrl2 = "http://test.dev.365etc.com/car/inout_photo/?sys=record&filename=test1";
                    	int pic_len = 0;
                        string fileData2 = ReadFile2(pathpic,pic_len);
                        string fileData = fileData2+"===end===";

                        int max = fileData.size();
                        printf("strlen -> %d\n",max);


                        int kuai = 20480;
                        double d_count = (double)max/(double)kuai;

                        printf("d_count strlen -> %f\n",d_count);

                        int run_count = ceil(d_count);

                        printf("run_count strlen -> %d\n",run_count);

                        for(int i=0; i<run_count;i++)
                        {
                            int s = i*kuai;


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
                        printf("HttpPost File %s %d\n",pathpic,pic_len);

                         
                    	return str;
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
            //else if ( isspace( (BYTE)sIn[ix] ) ) //ò�ưѿո�����%20����+������
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

void ShowAllDevice()
{
    //
    DWORD32  iDeviceCount = 0;
    HVAPI_SearchDeviceCount(&iDeviceCount);
    printf("Search DeviceCount : %d\n", iDeviceCount);

    DWORD64 dwMacAddr = 0;
    DWORD32 dwIP=0, dwMask = 0, dwGateWay = 0;
    char szMac[124] = {0};
    char szAddr[64] = {0};
    char szMask[64] = {0};
    char szGateWay[64] = { 0 };
    char szDeveTypeInfo[64] = { 0 };
    for (int i=0; i<(int)iDeviceCount; i++)
    {
        memset(szMac, 0, 124);
        memset(szAddr, 0, 64);
        memset(szMask, 0, 64);
        memset(szGateWay, 0, 64);
        memset(szDeveTypeInfo, 0, 64);
        if ( HVAPI_GetDeviceAddr((DWORD32)i, &dwMacAddr, &dwIP,&dwMask, &dwGateWay)  == S_OK )
        {
            g_DeviceMac[i] = dwMacAddr;
            MyGetMacString(dwMacAddr, szMac);
            MyGetIPString(dwIP, szAddr);
            MyGetIPString(dwMask, szMask);
            MyGetIPString(dwGateWay, szGateWay);
            printf("%d---mac:%s addr:%s  mask:%s, gateway:%s\n", i, szMac, szAddr, szMask, szGateWay);
        }

        if ( HVAPI_GetDeviceInfoEx((DWORD32)i, szDeveTypeInfo, 64 ) == S_OK )
        {
            printf("DeviceTypeInfo:%s\n\n",szDeveTypeInfo);

        }
    }

    if ( iDeviceCount == 0 )
        return ;
    while(true)
    {
         printf("1---Modify NetAddr Info\n");
        printf("2--- return\n ");
        int iSelect = -1;
        //
        scanf("%d" ,&iSelect);

        switch(iSelect)
        {
            case 1:
            {
                int index = -1;
                printf("InPut index:");
                scanf("%d", &index);
                if ( index < 0 || index > iDeviceCount -1)
                {
                    printf("Input error\n");
                    break;
                }

                printf("Mac:%u64d\n", g_DeviceMac[index] );

                memset(szAddr, 0, 64);
                memset(szMask, 0, 64);
                memset(szGateWay, 0, 64);

                printf("Input Device IP:");
                scanf("%s", szAddr);
                printf("Input Device Mask:");
                scanf("%s", szMask);
                printf("Input Device GateWay:");
                scanf("%s", szGateWay);

                DWORD32 dwIP, dwMask, dwGateWay;
                MyGetIPDWORD(szAddr, dwIP);
                MyGetIPDWORD(szMask, dwMask);
                MyGetIPDWORD(szGateWay, dwGateWay);

                if ( HVAPI_SetIPByMacAddr(g_DeviceMac[index], dwIP, dwMask,dwGateWay) == S_OK )
                {
                    printf("Setting Success\n");
                     HVAPI_SearchDeviceCount(&iDeviceCount);
                }
                else
                {
                    printf("Setting Failt\n");
                }
            }
            break;
            case 2:
                return ;
        }
    }

    return ;
}

void ConnectDevicesFun(char* szAddr)
{
    //char szAddr[64] = { 0 };
    //szAddr = inet_addr("172.16.2.100");
    //char* szAddr = sIp.c_str();//"192.168.10.100";
    //char* szAddr = "172.16.2.50";

    printf("摄像头IP:%s\n",szAddr);

    if ( TestExistObj(szAddr) )
    {
        printf("Exit Link\n");
        return ;
    }
    int iPos = FindEmptyPos();
    if ( iPos == -1)
     {
         printf("连接摄像头IP:%s 失败！\n",szAddr);
         return ;
     }
    g_CameraArray[iPos] = new CCameraOpt();
    if ( g_CameraArray[iPos] == NULL )
    {
        return ;
    }
   bool bRet = g_CameraArray[iPos]->OpenCamera(szAddr);
   if ( !bRet )
   {
       printf("摄像头IP:%s 打开失败！\n",szAddr);
       return ;
   }
      if ( g_CameraArray[iPos]->OpenRecordStream(NULL) )
      {
          printf("摄像头IP:%s 打开回调成功！\n",szAddr);
      }
      else
      {
           printf("摄像头IP:%s 打开回调失败！\n",szAddr);
      }
      /*
      if ( g_CameraArray[iPos]->OpenH264Stream(NULL) )
      {

          printf("Open H264 video Stream Success,Out\n");
      }
      else
      {

            printf("Opening H264 video stream failed,Out\n");
      }
       */
    return ;
}



void ShowDeviceStatus()
{
    for ( int i=0; i<MAX_CAMERA_COUNT; i++)
    {
        if ( g_CameraArray[i] == NULL )
            continue ;

        printf("%s Satus Info\n", g_CameraArray[i]->GetDeviceIp());

        DWORD linkStatus = 0, iTryTime = 0;
        if ( g_CameraArray[i]->GetLinkInfo(CONN_TYPE_RECORD , linkStatus, iTryTime)  )
        {
            if (  linkStatus ==  CONN_STATUS_NORMAL )
                printf(" Record Link Status:  connect\n" );
            else
             printf(" Record Link Status:  disconnect\n");

        }
        else
        {
            printf("Record Link Unkwon\n");
        }
        //
         if ( g_CameraArray[i]->GetLinkInfo(CONN_TYPE_VIDEO , linkStatus, iTryTime)  )
        {
            if (  linkStatus ==  CONN_STATUS_NORMAL )
                printf(" Video Link Status:  connect\n" );
            else
             printf(" Video Link Status:  disconnect\n");

        }
        else
        {
            printf("Record Link Unkwon\n");
        }
    }
}

void CloseDevices()
{
     for ( int i=0; i<MAX_CAMERA_COUNT; i++)
    {
        if ( g_CameraArray[i] == NULL )
            continue ;

        g_CameraArray[i]->CLoseCamera();
        delete g_CameraArray[i];
        g_CameraArray[i]  = NULL;
    }
}
