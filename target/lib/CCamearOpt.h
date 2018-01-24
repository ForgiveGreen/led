#ifndef _CAMERAOPT_H_
#define _CAMERAOPT_H_

#include "HvDeviceEx.h"

class CCameraOpt
{
    public:
    CCameraOpt();
    ~CCameraOpt();


    static int RecordBeginCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
    }
    static int RecordEndCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
    }
    static int  RecordBigImageCallBack(PVOID pUserData,
                                                                        DWORD dwCarID,
                                                                        WORD wImageTYpe,
                                                                        WORD wWidth,
                                                                        WORD wHeight,
                                                                        PBYTE pbPIcData,
                                                                        DWORD dwImgDataLen,
                                                                        DWORD dwRecordType,
                                                                        DWORD64 dw64TimeMs)
     {
         if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
        return pThis->RecordBigImage(dwCarID, wImageTYpe, wWidth, wHeight, pbPIcData, dwImgDataLen, dwRecordType, dw64TimeMs);
      }

    static int RecordSmallImageCallBack(PVOID pUserData,
                                                                            DWORD dwCarID,
                                                                            WORD wWidth,
                                                                            WORD wHeight,
                                                                            PBYTE pbPIcData,
                                                                            DWORD dwImgDataLen,
                                                                            DWORD dwRecordType,
                                                                            DWORD64 dw64TimeMs)
     {
         if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;

        return pThis->RecordSmallImage(dwCarID,wWidth,wHeight,pbPIcData,dwImgDataLen,dwRecordType,dw64TimeMs);
      }

    static int RecordBinaryImageCallBack(PVOID pUserData,
                                                                              DWORD dwCarID,
                                                                              WORD wWidth,
                                                                              WORD wHeight,
                                                                              PBYTE pbPIcData,
                                                                              DWORD dwImgDataLen,
                                                                              DWORD dwRecordType,
                                                                              DWORD64 dw64TimeMs)
     {
         if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
        return pThis->RecordBinaryImage( dwCarID,wWidth,wHeight,pbPIcData,dwImgDataLen,dwRecordType,dw64TimeMs);
      }


  static int RecordPlateNoCallBack(PVOID pUserData,
                                                                   DWORD dwCarID,
                                                                   LPCSTR pcPlateNo,
                                                                   LPCSTR pcAppendInfo,
                                                                   DWORD dwRecordType,
                                                                   DWORD64 dw64TimeMs)
  {
      if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
        return pThis->RecordPlateNo(dwCarID, pcPlateNo,pcAppendInfo,dwRecordType,dw64TimeMs);
  }

  static int DeviceStatusCallBack(PVOID pUserData, LPCSTR pString, DWORD dwStrLen)
  {
      if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;
        return pThis->DeviceStatus(pString,  dwStrLen);
  }

  static int DeviceJpegStreamCallBack(PVOID pUserData,PBYTE pbImageData,DWORD dwImageDataLen,
DWORD dwImageType, LPCSTR szImageExtInfo)
{
    if ( pUserData == NULL )
        return 0;

    CCameraOpt *pThis = (CCameraOpt *)pUserData;
    return pThis->DeviceJpegStream(pbImageData,dwImageDataLen, dwImageType, szImageExtInfo);
}

  static int  DeviceH264StreamCallBack( PVOID pUserData,
                                                                        PBYTE pbVideoData,
                                                                        DWORD dwVIdeoDataLen,
                                                                        DWORD dwVIdeoType,
                                                                        LPCSTR szVideoExtInfo)
   {
       if ( pUserData == NULL )
        return 0;

        CCameraOpt *pThis = (CCameraOpt *)pUserData;

        return pThis->DeviceH264Stream( pbVideoData,dwVIdeoDataLen,dwVIdeoType,szVideoExtInfo);
    }

    /////////////////////////////

     int RecordBegin(DWORD dwCarID);
     int RecordEnd(DWORD dwCarID);
     int  RecordBigImage(DWORD dwCarID,
                                             WORD wImageTYpe,
                                             WORD wWidth,
                                             WORD wHeight,
                                             PBYTE pbPIcData,
                                             DWORD dwImgDataLen,
                                             DWORD dwRecordType,
                                             DWORD64 dw64TimeMs);

    int RecordSmallImage(DWORD dwCarID,WORD wWidth,
                                                 WORD wHeight,
                                                 PBYTE pbPIcData,
                                                 DWORD dwImgDataLen,
                                                 DWORD dwRecordType,
                                                 DWORD64 dw64TimeMs);

    int RecordBinaryImage(DWORD dwCarID,
                                                 WORD wWidth,
                                                 WORD wHeight,
                                                 PBYTE pbPIcData,
                                                DWORD dwImgDataLen,
                                                 DWORD dwRecordType,
                                                 DWORD64 dw64TimeMs);


  int RecordPlateNo(DWORD dwCarID,LPCSTR pcPlateNo, LPCSTR pcAppendInfo,DWORD dwRecordType,DWORD64 dw64TimeMs);

  int DeviceStatus(LPCSTR pString, DWORD dwStrLen);

  int DeviceJpegStream(PBYTE pbImageData,DWORD dwImageDataLen,DWORD dwImageType, LPCSTR szImageExtInfo);

  int  DeviceH264Stream(PBYTE pbVideoData,DWORD dwVIdeoDataLen,DWORD dwVIdeoType,LPCSTR szVideoExtInfo);

  bool OpenCamera(char *szAddr);
  bool CLoseCamera();

  bool OpenRecordStream(char* szCommand);
  bool CloseRecordStream();
  bool OpenH264Stream(char* szCommand);
  bool CloseH264Stream();
  bool OpenImageStream(char* szCommand);
  bool CloseImageStream();

  bool GetLinkInfo(int SatusType, DWORD &linkStatus, DWORD &iTryTime);

  char *GetDeviceIp()
  {
      return m_szCameraIp;
  }


private:
    char m_szCameraIp[32];
    HVAPI_HANDLE_EX m_hCameraHandle;

    bool m_fOpen;
    bool m_fRecordOpen;
    bool m_fH264Open;
    bool m_fJpegOpen;
    int m_iFrames;
};
#endif
