/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : regulator_pmic.h
  版 本 号   : 初稿
  作    者   : 王振伟 176398
  生成日期   : 2012年12月19日
  最近修改   :
  功能描述   : regulator_pmic.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月19日
    作    者   : 王振伟 176398
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/


#ifndef __REGULATOR_PMIC_H__
#define __REGULATOR_PMIC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define DEBUG_OPEN  1
#define DEBUG_OFF    0

#define PMIC_TRACE(exp,message, args...)         if(exp)printk(KERN_DEBUG ":" message, ##args)

/*****************************************************************************
  3 枚举定义
*****************************************************************************/
#define PMIC_INVAILD        (0x0FFFFFFFF)

/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/


/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/
typedef struct _PMIC_COMSUMER_INFO_
{
    int  comsumer_channel_status; 
    int  comsumer_voltage_mini;
    int  comsumer_voltage_max;
    struct regulator_dev *dev;
}PMIC_COMSUMER_INFO;

/*****************************************************************************
  7 UNION定义
*****************************************************************************/


/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  9 全局变量声明
*****************************************************************************/

extern struct regulator_ops regulator_pmic_ops;
/*****************************************************************************
  10 函数声明
*****************************************************************************/

extern int pmic_power_status(int channel_id);
extern int pmic_power_switch( int id,int onoff);
extern int pmic_list_vol_tab( int id ,unsigned int selector );
extern int pmic_get_voltage(int id);
extern int pmic_set_voltage(int id ,int min_uV, int max_uV, unsigned *selector);
extern char* regulator_pmic_getname_by_id(int id);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of regulator_pmic.h */
