

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef __PMIC_MNTN_DEV_H__
#define __PMIC_MNTN_DEV_H__



/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define PMU_VERSION_ADDR  SOC_SMART_VERSION_ADDR(0)
#define PMU_VER_START 0x22
#define PMU_VER_END   0x25

#define PMU_32K_STAT_ADDR  SOC_SMART_STATUS1_ADDR(0)
#define PMU_32K_FLAG_OFFSET  (0x01<<2)
#define PMU_32K_OK_FLAG    0

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
extern void  pmic_mntn_dev_register(void);
extern PMUIRQ_STAT* pmic_get_pmustat(void);
extern int pmu_version_check(void);
extern int pmu_32kclk_check(void);

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


