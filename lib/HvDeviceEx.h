#ifndef _HVDEVICEEX_H
#define _HVDEVICEEX_H

#include "swerror.h"
#include "HvDeviceBaseType.h"
#include "HvDeviceCommDef.h"

#define HV_API_EX extern "C"
#define HV_API extern "C"

#define HVAPI_API_VERSION_EX "2.0"
typedef PVOID HVAPI_HANDLE_EX;

//CALL BACK TYPE
#define CALLBACK_TYPE_RECORD_PLATE                  0xFFFF0001
#define CALLBACK_TYPE_RECORD_BIGIMAGE           0xFFFF0002
#define CALLBACK_TYPE_RECORD_SMALLIMAGE     0xFFFF0003
#define CALLBACK_TYPE_RECORD_BINARYIMAGE    0xFFFF0004
#define CALLBACK_TYPE_RECORD_INFOBEGIN         0xFFFF0005
#define CALLBACK_TYPE_RECORD_INFOEND             0xFFFF0006
#define CALLBACK_TYPE_STRING                                0xFFFF0007
#define CALLBACK_TYPE_JPEG_FRAME                        0xFFFF0008
#define CALLBACK_TYPE_H264_VIDEO                          0xFFFF0009
#define CALLBACK_TYPE_HISTROY_VIDEO                 0xFFFF0010


/* BIG Image  TYPE*/
#define RECORD_BIGIMG_BEST_SNAPSHOT             0x0001
#define RECORD_BIGIMG_LAST_SNAPSHOT             0x0002
#define RECORD_BIGIMG_BEGIN_CAPTURE               0x0003
#define RECORD_BIGIMG_BEST_CAPTURE                 0x0004
#define RECORD_BIGIMG_LAST_CAPTURE                 0x0005

//connect TYPE
#define CONN_TYPE_UNKNOWN                   0xffff0000
#define CONN_TYPE_IMAGE                          0xffff0001
#define CONN_TYPE_VIDEO                           0xffff0002
#define CONN_TYPE_RECORD                        0xffff0003


/* ??????????? */
#define PLATE_RECT_BEST_SNAPSHOT           0x0001        /**< ?????????????????? */
#define PLATE_RECT_LAST_SNAPSHOT           0x0002        /**< ??????????????? */
#define PLATE_RECT_BEGIN_CAPTURE           0x0003        /**< ??????????????? */
#define PLATE_RECT_BEST_CAPTURE            0x0004        /**< ?????????????????? */
#define PLATE_RECT_LAST_CAPTURE            0x0005        /**< ??????????????? */

#define FACE_RECT_BEST_SNAPSHOT            0x0006        /**< ????????????????? */
#define FACE_RECT_LAST_SNAPSHOT            0x0007        /**< ?????????????? */
#define FACE_RECT_BEGIN_CAPTURE            0x0008        /**< ?????????????? */
#define FACE_RECT_BEST_CAPTURE             0x0009        /**< ????????????????? */
#define FACE_RECT_LAST_CAPTURE             0x000A        /**< ?????????????? */

//??????????
#define STREAM_TYPE_UNKNOWN                     0xffff0000
#define STREAM_TYPE_IMAGE                            0xffff0001
#define STREAM_TYPE_VIDEO                             0xffff0002
#define STREAM_TYPE_RECORD                         0xffff0003

typedef INT (CDECL * HVAPI_CALLBACK_RECORD_INFOBEGIN)(PVOID pUserData, DWORD dwCarID);
typedef INT (CDECL * HVAPI_CALLBACK_RECORD_INFOEND)(PVOID pUserData, DWORD dwCarID);

typedef INT (CDECL *HVAPI_CALLBACK_RECORD_PLATE)(PVOID pUserData,
                                                                                            DWORD dwCarID,
                                                                                            LPCSTR pcPlateNo,
                                                                                            LPCSTR pcAppendInfo,
                                                                                            DWORD dwRecordType,
                                                                                            DWORD64 dw64TimeMs);

typedef INT (CDECL *HVAPI_CALLBACK_RECORD_BIGIMAGE)(PVOID pUserData,
                                                                                                    DWORD dwCarID,
                                                                                                    WORD wImageTYpe,
                                                                                                    WORD wWidth,
                                                                                                    WORD wHeight,
                                                                                                    PBYTE pbPIcData,
                                                                                                    DWORD dwImgDataLen,
                                                                                                    DWORD dwRecordType,
                                                                                                    DWORD64 dw64TimeMs);


typedef INT (CDECL *HVAPI_CALLBACK_RECORD_SMALLIMAGE)(PVOID pUserData,
                                                                                                    DWORD dwCarID,
                                                                                                    WORD wWidth,
                                                                                                    WORD wHeight,
                                                                                                    PBYTE pbPIcData,
                                                                                                    DWORD dwImgDataLen,
                                                                                                    DWORD dwRecordType,
                                                                                                    DWORD64 dw64TimeMs);

typedef INT (CDECL *HVAPI_CALLBACK_RECORD_BINARYIMAGE)(PVOID pUserData,
                                                                                                        DWORD dwCarID,
                                                                                                        WORD wWidth,
                                                                                                        WORD wHeight,
                                                                                                        PBYTE pbPIcData,
                                                                                                        DWORD dwImgDataLen,
                                                                                                        DWORD dwRecordType,
                                                                                                        DWORD64 dw64TimeMs);

typedef INT(CDECL *HVAPI_CALLBACK_STRING)(PVOID pUserData, LPCSTR pString, DWORD dwStrLen);

typedef INT(CDECL *HVAPI_CALLBACK_JPEG)(PVOID pUserData,PBYTE pbImageData,DWORD dwImageDataLen,
DWORD dwImageType, LPCSTR szImageExtInfo);

typedef INT(CDECL *HVAPI_CALLBACK_H264)( PVOID pUserData,
                                                                        PBYTE pbVideoData,
                                                                        DWORD dwVIdeoDataLen,
                                                                        DWORD dwVIdeoType,
                                                                        LPCSTR szVideoExtInfo);

typedef INT(CDECL *HVAPI_CALLBACK_HISTORY_VIDEO)( PVOID pUserData,
                                                                                            PBYTE pbVideoData,
                                                                                            DWORD dwVIdeoDataLen,
                                                                                            DWORD dwVIdeoType,
                                                                                            LPCSTR szVideoExtInfo);


HV_API_EX HRESULT CDECL HVAPI_SearchDeviceCount(DWORD32 *pdwDeviceCount);
HV_API_EX HRESULT CDECL HVAPI_GetDeviceAddr(DWORD32 dwIndex, DWORD64 *dw64MacAddr, DWORD32 *dwIP, DWORD32 *dwMask, DWORD32 *dwGateWay);
HV_API_EX HRESULT CDECL HVAPI_SetIPByMacAddr(DWORD64 dw64MacAddr, DWORD32 dwIP, DWORD32 dwMask, DWORD32 dwGateWay);
HV_API_EX HRESULT CDECL HVAPI_GetDeviceInfoEx(int iIndex, LPSTR  lpDevTypeInfo, int iBufLen);
HV_API_EX HRESULT CDECL HVAPI_GetDevTypeEx(PSTR pcIP, int *iDeviceType);
HV_API_EX HVAPI_HANDLE_EX CDECL HVAPI_OpenEx(LPCSTR szIp, LPCSTR szApiVer);
HV_API_EX HRESULT CDECL HVAPI_CloseEx(HVAPI_HANDLE_EX hHandle);
//HV_API_EX HRESULT CDECL HVAPI_GetXmlVersionEx(HVAPI_HANDLE_EX hHandle, bool *pfIsNewProtol);

HV_API_EX HRESULT  CDECL HVAPI_GetXmlVersionEx(HVAPI_HANDLE_EX hHandle, PROTOCOL_VERSION* pemProtocolVersion);

HV_API_EX HRESULT CDECL HVAPI_GetConnStatusEx(HVAPI_HANDLE_EX hHandle, int nStreamType, DWORD *pdwConStatus);
HV_API_EX HRESULT CDECL HVAPI_GetReConnectTimesEx(HVAPI_HANDLE_EX hHandle, INT nStreamType, DWORD *pdwReConnectTimes, BOOL fIsReset);
HV_API_EX HRESULT CDECL HVAPI_ExecCmdEx(HVAPI_HANDLE_EX hHandle, LPCSTR szCmd,LPSTR szRetBuf, INT nBufLen, INT* pnRetLen);
HV_API_EX HRESULT CDECL HVAPI_GetParamEx(HVAPI_HANDLE_EX hHandle,LPSTR szParamDoc,INT nBufLen, INT *pnRetLen);
HV_API_EX HRESULT CDECL HVAPI_SetParamEx(HVAPI_HANDLE_EX hHandle, LPCSTR szParamDoc);
HV_API_EX HRESULT CDECL HVAPI_SendControllPannelUpDataFileEx(HVAPI_HANDLE_EX hHandle, PBYTE pUpDateFileBuffer, DWORD dwFileSize);
HV_API_EX HRESULT CDECL HVAPI_SetCallBackEx(HVAPI_HANDLE_EX hHandle, PVOID pFunc, PVOID pUserData, INT iVideoID,INT iCallBackType, LPCSTR szConnCmd);
HV_API_EX HRESULT CDECL HVAPI_SetEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle,DWORD dwEnhanceStyle, INT iBrightness,INT iHubThreshold,INT iCompressRate);
HV_API_EX HRESULT CDECL HVAPI_SetHistoryVideoEnhanceRedLightFlagEx(HVAPI_HANDLE_EX hHandle, DWORD dwEnhanceStyle,INT IBrightness,INT iHubThreshold,INT iCompressRate);
HV_API_EX HRESULT CDECL HVAPI_GetExtensionInfoEx(HVAPI_HANDLE_EX hHandle, DWORD dwType, LPVOID pRetData, INT* iBufLen);


HV_API_EX HRESULT CDECL HVAPIUTILS_GetRecordInfoFromAppendStringEx(LPCSTR szAppened, LPCSTR szInfoName, LPSTR szRetInfo, int iRetInfoBufLen);
HV_API_EX HRESULT CDECL HVAPIUTILS_ParsePlateXmlStringEx(LPCSTR pszXmlPlateInfo, LPSTR pszPlateInfoBuf, int iPlateInfoBufLen );
HV_API_EX HRESULT CDECL HVAPIUTILS_SmallImageToBitmapEx(PBYTE pbSmallImageData, INT nSmallImageWidth, INT nSmallImageHeight, PBYTE pbBitmapData, INT* pnBitmapDataLen );
HV_API_EX HRESULT CDECL HVAPIUTILS_BinImageToBitmapEx(PBYTE pbBinImageData, PBYTE pbBitmapData,
INT *pnBitmapDataLen);
HV_API_EX HRESULT CDECL HVAPIUTILS_GetExeCmdRetInfoEx(BOOL fIsNewXmlProtocol, LPCSTR pszRetXmlStr, LPCSTR pszCmdName, LPCSTR pszInfoName, LPSTR pszInfoValue );



/**
* @brief		????????????????
* @param[in]	hHandle	        ????õô????§¹???
*@param[in]   iVideo		   ??????
* @return		?????S_OK??????E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_TriggerSignal(HVAPI_HANDLE_EX hHandle, INT iVideoID);

/**
* @brief		??????????
* @param[in]	hHandle	        ????õô????§¹???
* @param[in]	szWhileNameList	 ???????§Ò?,??????(????-??§¹??)??$????????????:??A11111-2013010101$??A2222-2013010202$??A33333-2013020202   ???????NULL,????????¨¢?
* @param[in]	iWhileListLen		?????????????
* @param[in]	szBlackNameList		??????????? ??????(????-??§¹??)??$????????????:??A11111-2013010101$??A2222-2013010202$??A33333-2013020202   ??????????????
* @param[in]	iListLen		?????????????
* @return		?????S_OK??????E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPI_InportNameList(HVAPI_HANDLE_EX hHandle, CHAR* szWhileNaneList, INT iWhileListLen, CHAR* szBlackNameList, INT iBlackListLen);


/**
* @brief		??????????
* @param[in]	hHandle	        ????õô????§¹???
* @param[out]	szWhileNameList		??????????????????? ??????(????-??§¹??)??$????????????:??A11111-2013010101$??A2222-2013010202$??A33333-2013020202
* @param[in/out]	iWhileListLen		????????????? ????????????
@param[out]	szBlackNameList		??????????????????? ??????(????-??§¹??)??$????????????:??A11111-2013010101$??A2222-2013010202$??A33333-2013020202
* @param[in/out]	iBlackListLen		????????????? ????????????

* @return		?????S_OK??????E_FAIL
*/

HV_API_EX HRESULT CDECL HVAPI_GetNameList(HVAPI_HANDLE_EX hHandle, CHAR* szWhileNameList, INT *iWhileListLen, CHAR* szBlackNameList, INT *iBlackListLen);


/**
* @brief		??????????????
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	fOSDEnable		0??????1????
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId ,BOOL fOSDEnable);
/**
* @brief		???¨®?????????????
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	fOSDEnable		0??????1????
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDPlateEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId ,BOOL fOSDEnable);

/**
* @brief		???????????-?????????
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	fEnable			???????????????????¦¶??0??????1????
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDTimeEnable(HVAPI_HANDLE_EX hHandle, INT nStreamId, BOOL fEnable);

/**
* @brief		???????????¦Ë??
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	nPosX			????¦Ë??X??????¦¶: 0~????
* @param[in]	nPosY			????¦Ë??Y??????¦¶??0~????
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDPos( HVAPI_HANDLE_EX hHandle,INT nStreamId, INT nPosX ,INT nPosY );

/**
* @brief		???????????????
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	nFontSize		?????§³????¦¶??16~32
* @param[in]	nColorR			???????R?????¦¶0??255
* @param[in]	nColorG			???????G?????¦¶0??255
* @param[in]	nColorB			???????B?????¦¶0??255
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDFont(HVAPI_HANDLE_EX hHandle, INT nStreamId ,INT nFontSize ,
										 INT nColorR ,INT nColorG,INT nColorB);
/**
* @brief		????????????????
* @param[in]	hHandle			????õô????§¹???
* @param[in]	nStreamId		?????ID??0??H264,1:MJPEG
* @param[in]	szText			????????? ?????¦¶??0??255
* @return		?????S_OK??????E_FAIL  ???????????S_FALSE
*/
HV_API_EX HRESULT CDECL HVAPI_SetOSDText(HVAPI_HANDLE_EX hHandle, INT nStreamId,CHAR* szText  );


/**
* @brief			????????????§Ý????????
* @param[in]		szAppened			??????????
* @param[in]		szInfoName			???????
* @param[out]		szRetInfo			??????
* @param[in]		iRetInfoBufLen		?????????H??????
* @return			?????S_OK??????E_FAIL
*/
HV_API_EX HRESULT CDECL HVAPIUTILS_GetRecordInfoFromAppenedStringEx(LPCSTR szAppened, LPCSTR szInfoName, LPSTR szRetInfo, INT iRetInfoBufLen);










#endif // _HVDEVICEEX_H
