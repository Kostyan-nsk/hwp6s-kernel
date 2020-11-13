/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : AtOamInterface.h
  版 本 号   : 初稿
  作    者   : f62575
  生成日期   : 2011年10月11日
  最近修改   :
  功能描述   : AtOamInterface.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年10月11日
    作    者   : f62575
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ATOAMINTERFACE_H__
#define __ATOAMINTERFACE_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "v_msg.h"
#include "siapppih.h"
#include "siapppb.h"
#include "siappstk.h"
#include "MnClient.h"

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
 枚举名    : AT_OAM_MSG_TYPE_ENUM
 结构说明  : TAF与OAM消息接口枚举
*****************************************************************************/
enum AT_OAM_MSG_TYPE_ENUM
{
    /* STK -> AT */
    STK_AT_DATAPRINT_CNF,/* MN_APP_STK_AT_DATAPRINT_STRU */
    STK_AT_EVENT_CNF,   /* _H2ASN_MsgChoice MN_APP_STK_AT_CNF_STRU */

    /* PIH -> AT */
    PIH_AT_EVENT_CNF,   /* _H2ASN_MsgChoice MN_APP_PIH_AT_CNF_STRU */

    /* PB -> AT */
    PB_AT_EVENT_CNF,   /* _H2ASN_MsgChoice MN_APP_PB_AT_CNF_STRU */

    AT_OAM_MSG_TYPE_BUTT
};
typedef VOS_UINT32 AT_OAM_MSG_TYPE_ENUM_UINT32;


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/*****************************************************************************
 结构名    : MN_APP_PIH_AT_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : PIH给AT上报的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    SI_PIH_EVENT_INFO_STRU              stPIHAtEvent;
} MN_APP_PIH_AT_CNF_STRU;

/*****************************************************************************
 结构名    : MN_APP_PB_AT_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : PIH给AT上报的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    SI_PB_EVENT_INFO_STRU               stPBAtEvent;
} MN_APP_PB_AT_CNF_STRU;

/*****************************************************************************
 结构名    : MN_APP_STK_AT_DATAIND_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : STK给AT上报的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    SI_STK_PRINTDATA_CNF_STRU           stSTKAtPrint;
} MN_APP_STK_AT_DATAPRINT_STRU;


/*****************************************************************************
 结构名    : MN_APP_STK_AT_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : STK给AT上报的消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    SI_STK_EVENT_INFO_STRU              stSTKAtCnf;
} MN_APP_STK_AT_CNF_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/

extern VOS_INT32 At_sprintf(VOS_INT32 MaxLength,VOS_CHAR *headaddr,VOS_CHAR *curraddr,const VOS_CHAR *fmt,...);
extern VOS_UINT32 MMA_VerifyOperatorLockPwd(VOS_UINT8 *pucPwd);
extern VOS_UINT8 MMA_GetCurrentPhoneMode(VOS_VOID);
extern VOS_UINT32 MN_MSG_Decode_UsimMsg(
    VOS_UINT8                           *pucData,
    VOS_UINT32                          ulLen,
    MN_MSG_SUBMIT_LONG_STRU             *pstLongSubmit
);

extern VOS_VOID  MN_CALL_GetCallInfoList(
    TAF_UINT8                           *pucNumOfCalls,
    MN_CALL_INFO_STRU                   *pstCallInfos
);



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

#endif /* end of AtOamInterface.h */

