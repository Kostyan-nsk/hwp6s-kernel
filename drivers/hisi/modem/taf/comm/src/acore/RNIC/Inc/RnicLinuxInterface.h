/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : RnicLinuxInterface.h
  版 本 号   : 初稿
  作    者   : 范晶
  生成日期   : 2011年12月20日
  最近修改   :
  功能描述   : RNIC的Linux接口文件
  函数列表   :
  修改历史   :
  1.日    期   : 2011年12月20日
    作    者   : 范晶/00179208
    修改内容   : 创建文件

******************************************************************************/

#ifndef __LINUXINTERFACE_H_
#define __LINUXINTERFACE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#if (VOS_OS_VER == VOS_LINUX)
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <init.h>
#include <if.h>
#include <kernel.h>
#include <skbuff.h>
#include <netdevice.h>
#include <asm-generic/errno-base.h>
#include <delay.h>
#include <gfp.h>
#include <linux/netlink.h>
#else
#include "LinuxStub.h"
#endif

/******************************************************************************
  2 宏定义
******************************************************************************/
#ifdef __UT_CENTER__
#define ACPU_FID_RNIC    (0)
#endif

/******************************************************************************
  3 枚举定义
******************************************************************************/


/******************************************************************************
  4 全局变量声明
******************************************************************************/


/******************************************************************************
  5 消息头定义
******************************************************************************/


/******************************************************************************
  6 消息定义
******************************************************************************/


/******************************************************************************
  7 STRUCT定义
******************************************************************************/
#pragma pack(4)



#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


/******************************************************************************
  8 UNION定义
******************************************************************************/


/******************************************************************************
  9 OTHERS定义
******************************************************************************/




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* LINUXInterface.h */






