/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hifi_loader_interface.h
  版 本 号   : 初稿
  作    者   : 刘慈红 lKF71598
  生成日期   : 2012年8月29日
  最近修改   :
  功能描述   : hifi_loader.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年8月29日
    作    者   : 刘慈红 lKF71598
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __HIFI_LOADER_INTERFACE_H__
#define __HIFI_LOADER_INTERFACE_H__

#include <MemoryLayout.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define MEM_BEGIN_CHECK32_DATA      (0x55AA55AA)
#define MEM_END_CHECK32_DATA        (0xAA55AA55)

#define HIFI_SEC_MAX_NUM            (32)
#define HIFI_DYNAMIC_SEC_MAX_NUM    (HIFI_SEC_MAX_NUM)

#define HIFI_SHARE_ADDR_BASE        (MEMORY_RAM_CORESHARE_LOAD_HIFI_ADDR)
#define HIFI_SHARE_ADDR_LENGTH      (MEMORY_RAM_CORESHARE_LOAD_HIFI_SIZE)

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

/*****************************************************************************
 实 体 名  : drv_hifi_sec_addr_info
 功能描述  : Hifi动态加载段地址结构
*****************************************************************************/
struct drv_hifi_sec_addr
{
    unsigned int  sec_source_addr;	/*段的源地址*/
    unsigned int  sec_length;	    /*段的长度*/
    unsigned int  sec_dest_addr;    /*段的目的地址*/
};

/*****************************************************************************
 实 体 名  : drv_hifi_sec_load_info
 功能描述  : Hifi动态加载段结构
*****************************************************************************/
#define HIFI_SEC_DATA_LENGTH (HIFI_SHARE_ADDR_LENGTH \
                               - HIFI_DYNAMIC_SEC_MAX_NUM*sizeof(struct drv_hifi_sec_addr) \
                               - 3*sizeof(unsigned int))

struct drv_hifi_sec_load_info
{
    unsigned int             sec_magic;         /*段信息开始的保护字*/
    unsigned int             sec_num;           /*段的个数*/
    struct drv_hifi_sec_addr sec_addr_info[HIFI_DYNAMIC_SEC_MAX_NUM]; /*段的地址信息*/
    char                     sec_data[HIFI_SEC_DATA_LENGTH];          /*段信息*/
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

#endif /* end of hifi_loader_interface.h */
