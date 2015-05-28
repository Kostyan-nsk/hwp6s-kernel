/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : AdsNdInterface.h
  版 本 号   : 初稿
  作    者   : 鲁琳/l60609
  生成日期   : 2011年12月6日
  最近修改   :
  功能描述   : ADS和NDCLIENT之间的接口
  函数列表   :
  修改历史   :
  1.日    期   : 2011年12月6日
    作    者   : 鲁琳/l60609
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ADSNDINTERFACE_H__
#define __ADSNDINTERFACE_H__

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)


/*****************************************************************************
  2 宏定义
*****************************************************************************/
/* !!!!!!!!!!!暂时定义，最终的值由北京确定 */
#define ADS_NDCLIENT_MSG_HDR            (0x00)
#define NDCLIENT_ADS_MSG_HDR            (0x00)


/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
枚举名    : ADS_NDCLIENT_ERR_TYPE_ENUM
结构说明  : ADS和NDCLIENT之间的消息

  1.日    期   : 2011年12月6日
    作    者   : 鲁琳/l60609
    修改内容   : 定义ADS和NDCLIENT之间的
*****************************************************************************/
enum ADS_NDCLIENT_MSG_ID_ENUM
{
    ID_ADS_NDCLIENT_DATA_IND                  =   ADS_NDCLIENT_MSG_HDR + 0x00,  /* ADS->NDCLIENT DATA IND */
    ID_ADS_NDCLIENT_ERR_IND                   =   ADS_NDCLIENT_MSG_HDR + 0x01,  /* ADS->NDCLIENT ERR IND */
    ID_ADS_NDCLIENT_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32  ADS_NDCLIENT_MSG_ID_ENUM_UINT32;

/*****************************************************************************
枚举名    : ADS_NDCLIENT_ERR_TYPE_ENUM
结构说明  : ADS和NDCLIENT之间的错误类型

  1.日    期   : 2011年12月6日
    作    者   : 鲁琳/l60609
    修改内容   : 定义ADS和NDCLIENT的之间的错误类型
*****************************************************************************/
enum ADS_NDCLIENT_ERR_TYPE_ENUM
{
    ADS_NDCLIENT_ERR_TYPE_MTU_OVERLONG      = 0x00,                             /* 数据包长度大于MTU */
    ADS_NDCLIENT_ERR_TYPE_BUTT
};
typedef VOS_UINT8 ADS_NDCLIENT_ERR_TYPE_ENUM_UINT8;


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
/*****************************************************************************
结构名    : ADS_NDCLIENT_ERR_IND_STRU
结构说明  : ADS和NDCLIENT之间的错误指示消息结构
            ADS会对下行数据包做MTU的合法性检测，如果当前是IPv6数据包，
            且数据包长度超出MTU值，则ADS会给ND CLIENT发送此原语，
            由ND CLIENT组包通知对端相应错误。

  1.日    期   : 2011年12月6日
    作    者   : 鲁琳/l60609
    修改内容   : 定义ADS和NDCLIENT之间的错误指示消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    ADS_NDCLIENT_MSG_ID_ENUM_UINT32     enMsgId;                                /*_H2ASN_Skip*/
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT8                           ucRabId;                                /* RAB标识，取值范围:[5,15] */
    ADS_NDCLIENT_ERR_TYPE_ENUM_UINT8    enErrType;                              /*错误类型*/
    VOS_UINT16                          usLen;                                  /*数据包长度*/
    VOS_UINT8                           aucData[2];                             /*数据包内容*/ 
}ADS_NDCLIENT_ERR_IND_STRU;

/*****************************************************************************
结构名    : ADS_NDCLIENT_DATA_IND_STRU
结构说明  : ADS和NDCLIENT之间的数据消息结构
            核心网发送给NDCLIENT的RA包及ECHO REQUEST等报文经下行IPF过滤到A CPU后，
            由ADS通过该接口返回给ND CLIENT。

  1.日    期   : 2011年12月6日
    作    者   : 鲁琳/l60609
    修改内容   : 定义ADS和NDCLIENT之间的数据消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    ADS_NDCLIENT_MSG_ID_ENUM_UINT32     enMsgId;                                /*_H2ASN_Skip*/
    MODEM_ID_ENUM_UINT16                enModemId;
    VOS_UINT8                           ucRabId;                                /* RAB标识，取值范围:[5,15] */
    VOS_UINT8                           aucRsv[1];                              /*保留位*/
    VOS_UINT16                          usLen;                                  /*数据包长度*/
    VOS_UINT8                           aucData[2];                             /*数据包内容*/ 
}ADS_NDCLIENT_DATA_IND_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  10 函数声明
*****************************************************************************/






#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

