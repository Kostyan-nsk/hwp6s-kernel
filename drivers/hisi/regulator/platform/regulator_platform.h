/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : regulator_platform.h
  版 本 号   : 初稿
  作    者   : 王振伟 176398
  生成日期   : 2012年12月19日
  最近修改   :
  功能描述   : regulator_platform.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月19日
    作    者   : 王振伟 176398
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include  <regulator_info.h>

#ifndef __REGULATOR_PLATFORM_H__
#define __REGULATOR_PLATFORM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define NUM_OF_REGULATORS     REGULATORS_TOTAL_NUM

/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/


/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/
struct regulator_platform_data{
        struct regulator_dev    *rdev[NUM_OF_REGULATORS];
        struct timeval lastoff_time[NUM_OF_REGULATORS];
        struct regulator_consumer_supply *consumer_supplies;
        int num_consumer_supplies;
};

/*****************************************************************************
  7 UNION定义
*****************************************************************************/


/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of regulator_platform.h */
