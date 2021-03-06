

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef __PMIC_MNTN_H__
#define __PMIC_MNTN_H__

#include "pmicirq_driver.h"
/*****************************************************************************
  2 宏定义
*****************************************************************************/

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
extern int  pmic_mntn_init(void);
extern int  pmic_mntn_uninit(void);
extern PMUIRQ_DATA* pmic_get_pmuirq_desc(void);


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


