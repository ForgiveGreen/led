#ifndef _HVCAMERATYEP_H_
#define _HVCAMERATYEP_H_

typedef enum
{
    E_NETFAIL = -1
}NET_ERROR;


typedef enum
{
    CAMERA_UNKNOW_CMD = 0xFFFF0000,
    CAMERA_GET_PARAM_CMD = 0x000F0001,
    CAMERA_SET_PARAM_CMD = 0x000F0002,
    CAMERA_SET_EXPOSURE_CMD = 0x000F0003,
    CAMERA_SET_GAIN_CMD = 0x000F0004,
    CAMERA_SET_AGCLIMIT_CMD = 0x000F0005,
    CAMERA_GET_TIME_CMD = 0x000F0006,
    CAMERA_SET_TIME_CMD = 0x000F0007,
    CAMERA_SET_RGB_GAIN_CMD = 0x000F0008,
    CAMERA_SET_ENCODE_MODE_CMD = 0x000F0009,
    CAMERA_SET_FPS_MODE_CMD = 0x000F0010,
    CAMERA_SOFT_TRIGGER_CAPTURE_CMD = 0x000F0011,
    CAMERA_SET_RAW_SHUTTER_CMD = 0x000F0012,
    CAMERA_SET_REG_CMD = 0x000F1000,
    CAMERA_GET_STATE_CMD = 0x000F2001,
    CAMERA_SET_STATE_CMD = 0x000F2002,
    CAMERA_DEBUG_SET_SOME_VALUE_CMD = 0x000F3000,
    CAMERA_XML_EXT_CMD = 0x000F4000,
    CAMERA_RESET_DEVICE_CMD = 0x000F5000,
    CAMERA_GET_RESET_REPORT_CMD = 0x000F5001,
    CAMERA_NETCOM_CMD = 0x000F6000,
    CAMERA_GET_DEBUG_INFO_CMD = 0x000F7000,
    CAMERA_UPDATE_CONTROLL_PANNEL = 0x000F8000,
    CAMERA_PROTOCAL_COMMAND = 0x000F9000,
    CAMERA_GET_NAMELIST_CMD = 0x000FB000,
    CAMERA_SET_OSD_TEXT=0x000FE000
}CAMERA_COMMAND_TYPE;


typedef enum{
    CAMERA_THROB = 0xFFFF0001,
    CAMERA_THROB_RESPONSE = 0xFFFF0002,
    CAMERA_IMAGE = 0xF0000001,
    CAMERA_VIDEO = 0xF0000002,
    CAMERA_RECORD = 0xF0010001,
    CAMERA_STRING = 0xF0010002,
    CAMERA_STATE = 0xF0020001,
    CAMERA_HISTORY_BEGIN = 0xF0030001,
    CAMERA_HISTORY_END = 0xF0030002,
    CAMERA_TFD_STRING = 0xF0030003
}CAMERA_INFO_TYPE;

typedef enum{
    CAMERA_IMAGE_JPEG = 0x0001,
    CAMERA_IMAGE_JPEG_CAPTURE = 0x0002,
    CAMERA_IMAGE_JPEG_SLAVE = 0x0003
}CAMERA_IMAGE_TYPE;

typedef enum
{
    CAMERA_VIDEO_H264 = 0x0001,
    CAMERA_FRAME_I = 0x1000,
    CAMERA_FRAME_P = 0x1001,
    CAMERA_FRAME_IP_ONE_SECOND = 0x1002,
    CAMERA_VIDEO_JPEG = 0x1003
}CAMERA_VIDEO_TYPE;

typedef enum
{
    CAMERA_RECORD_NORMAL = 0x0001,
    CAMERA_RECORD_HISTORY = 0x0002,
    CAMERA_RECORD_STRING = 0x0003,
    CAMERA_RECORD_INLEFT = 0x0004,
    CAMERA_RECORD_STATUS = 0x0005
}CAMERA_RECORD_TYPE;

typedef enum
{
    BLOCK_IMAGE_TYPE = 0xFF000001,
    BLOCK_IMAGE_WIDTH=0xFF000002,
    BLOCK_IMAGE_HEIGHT=0xFF000003,
    BLOCK_IMAGE_TIME=0xFF000004,
    BLOCK_IMAGE_EXT_INFO = 0xFF000005,
    BLOCK_IMAGE_JPEG_CR=0xFF000006,
    BLOCK_IMAGE_Y = 0xFF000007,
    BLOCK_VIDEO_TYPE=0xFF001001,
    BLOCK_FRAME_TYPE = 0xFF001002,
    BLOCK_VIDEO_TIME = 0xFF001003,
    BLOCK_IMAGE_OFFSET = 0xFF001004,
    BLOCK_VIDEO_EXT_INFO = 0xFF001005,
    BLOCK_VIDEO_Y = 0xFF001006,
    BLOCK_VIDEO_WIDTH = 0xFF001007,
    BLOCK_VIDEO_HEIGHT = 0xFF001008,
    BLOCK_VIDEO_OUTPUT_FR = 0xFF001009,
    BLOCK_XML_TYPE = 0xFF002001,
    BLOCK_RECORD_TYPE = 0xFF002002,
    BLOCK_FPGA_EXT_INFO = 0xFF003001
}CAMERA_BLOCK_NAME;

typedef enum
{
    CAMERA_IMAGE_LINK_PORT = 9900,
    CAMERA_VIDEO_LINK_PORT = 9901,
    CAMERA_RECORD_LINK_PORT = 9902,
    CAMERA_CMD_LINK_PORT = 9910
}CAMERA_LINK_PORT;

typedef struct tagImageExtInfo
{
    int iShutter;
    int iGain;
    int iGainR;
    int iGainG;
    int iGainB;
} ImageExtInfo;

typedef struct tag_VideoExtInfo
{
    int iShutter;
    int iGain;
    int iGainR;
    int iGainG;
    int iGainB;
}VideoExtInfo;


typedef struct tag_head
{
    DWORD32 dwType;
    DWORD32 dwInfoLen;
    DWORD32 dwDataLen;
}INFO_HEADER;

typedef struct tag_block
{
    DWORD32 dwID;
    DWORD32 dwLen;
}BLOCK_HEADER;

typedef struct tag_CameraCmdHeader
{
    DWORD32 dwID;
    DWORD32 dwInfoSize;
}CAMERA_CMD_HEADER;


typedef struct tag_CameraCmdRespond
{
    DWORD dwID;
    DWORD dwInfoSize;
    int dwResult;
}CAMERA_CMD_RESPOND;


typedef struct tagRect
{
    long left;
    long top;
    long right;
    long bottom;
}RECT;

typedef enum{
    RECORD_IMAGE_BEST_SNAPSHOT=0,
    RECORD_IMAGE_LAST_SNAPSHOT,
    RECORD_IMAGE_BEGIN_CAPTURE,
    RECORD_IMAGE_BEST_CAPTURE,
    RECORD_IMAGE_LAST_CAPTURE,
    RECORD_IMAGE_SMALL_IMAGE,
    RECORD_IMAGE_BIN_IMAGE
}RECORD_IMAGE_TYPE;



#endif // _HVCAMERATYEP_H_
