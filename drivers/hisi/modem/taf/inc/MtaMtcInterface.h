

#ifndef _MTCMTAINTERFACE_H_
#define _MTCMTAINTERFACE_H_

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "vos.h"
#include "PsTypeDef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/

/*****************************************************************************
  3 枚举定义
*****************************************************************************/

enum MTA_MTC_MSG_ID_ENUM
{
    /* 消息名称 */                         /* 消息ID */                         /* 备注 */
    /* MTA --> MTC */

    /* MTC --> MTA */
    ID_MTC_MTA_PS_TRANSFER_IND          = 0x0002,                              /* _H2ASN_MsgChoice MTC_MTA_PS_TRANSFER_IND_STRU */
    ID_MTC_MTA_MIPICLK_INFO_IND         = 0x0004,                              /* _H2ASN_MsgChoice MTC_MTA_MIPICLK_INFO_IND_STRU*/

    ID_MTA_MTC_MSG_ID_BUTT
};
typedef VOS_UINT32  MTA_MTC_MSG_ID_ENUM_UINT32;


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/

typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    VOS_UINT8                           ucCause;                                /* PS域切换原因值 */
    VOS_UINT8                           aucReserved[3];                         /*保留位*/
} MTC_MTA_PS_TRANSFER_IND_STRU;
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;                            /*_H2ASN_Skip*/
    VOS_UINT16                          usMipiCLk;                              /* 可选MIPICLK值集合 */
    VOS_UINT8                           aucReserved[2];                         /*保留位*/
} MTC_MTA_MIPICLK_INFO_IND_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  H2ASN顶级消息结构定义
*****************************************************************************/
typedef struct
{
    MTA_MTC_MSG_ID_ENUM_UINT32          enMsgID;                                /*_H2ASN_MsgChoice_Export MTA_MTC_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MTA_MTC_MSG_ID_ENUM_UINT32
    ****************************************************************************/
} MTA_MTC_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    MTA_MTC_MSG_DATA                    stMsgData;
} MtaMtcInterface_MSG;

/*****************************************************************************
  10 函数声明
*****************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MtaMtcInterface.h */

