/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : AdsFcInterface.h
  版 本 号   : 初稿
  作    者   : l60609
  生成日期   : 2012年1月10日
  最近修改   :
  功能描述   : ADS提供给流控模块的接口
  函数列表   :
  修改历史   :
  1.日    期   : 2012年1月10日
    作    者   : l60609
    修改内容   : 创建文件

******************************************************************************/

#ifndef __ADSFCINTERFACE_H__
#define __ADSFCINTERFACE_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include  "vos.h"


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
  7 STRUCT定义
*****************************************************************************/

typedef VOS_VOID (*ADS_DRV_ASSEM_FUNC)(VOS_VOID);

/*****************************************************************************
 结构名   : FC_ADS_DRV_ASSEM_STRU
 结构说明 : 底软动态组包参数
*****************************************************************************/
typedef struct
{
    VOS_UINT8               ucEnableMask;
    VOS_UINT8               aucRsv[3];
    VOS_UINT32              ulDlRateUpLev;
    VOS_UINT32              ulDlRateDownLev;
    VOS_UINT32              ulExpireTmrLen;
    ADS_DRV_ASSEM_FUNC      pDrvAssemFunc;
}FC_ADS_DRV_ASSEM_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/
extern VOS_UINT32 ADS_GetCurrentRate
(
    VOS_UINT32                         *pulUlBpsRate,
    VOS_UINT32                         *pulDlBpsRate
);

/*****************************************************************************
 函 数 名  : ADS_DL_RegDrvAssemFunc
 功能描述  : 下行流控注册函数, 用于解决下行突发流量, FC无法及时调整流程参数,
             FC向ADS注册流控的回调函数, 以及触发流控阈值参数
 输入参数  : pstDrvAssemParam - 触发流控阈值参数
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :
*****************************************************************************/
extern VOS_VOID ADS_DL_RegDrvAssemFunc(FC_ADS_DRV_ASSEM_STRU *pstDrvAssemParam);


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

#endif /* end of AtFcInterface.h */

