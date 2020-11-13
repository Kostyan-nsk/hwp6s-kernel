

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __PMIC_H__
#define __PMIC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/platform_device.h>
#include <hisi/regulator_info.h>

#ifdef    _DRV_LLT_
#include "drv_global.h"
#else
typedef    unsigned int    BSP_U32;
typedef    unsigned short  BSP_U16;
typedef    unsigned char   BSP_U8;
typedef    signed int      BSP_S32;
typedef    void            BSP_VOID;
#endif
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define     PMU_LOCAL   static
#define     BSP_OK     (0)
#define     BSP_ERROR  (-1)

#define PMU_REGISTER_DATA_BIT (8)
#define PMU_IRQ_VALUE_CLEAR   (0xFF)

/* ��regulator_pmic_info.h����һ�� */
#define PMU_CHANNEL_ID_BUTT (LVS_ID_END+1)

#define PMU_POWER_NAME_SIZE             (16)    /*��Դ������*/

/*regulator ops ģ�鷵��ֵ����*/
#define PMIC_MODULE_ERROR -1       /*���󷵻�ֵ*/
#define PMIC_MODULE_OK 0           /*�ɹ�����ֵ*/
#define PMIC_MODULE_STATUS_ON 1    /*mtcmos ״̬Ϊ��*/
#define PMIC_MODULE_STATUS_OFF 2   /*mtcmos ״̬Ϊ�ر�*/

#define PMU_SWITCH_OFF          (0)     /*PMU��Դ��*/
#define PMU_SWITCH_ON           (1)     /*PMU��Դ��*/

#define PMU_LVS_VOL_SET_INVAILD  0xFFFF

#define PMU_STATUS_REG_TYPE_ID_IS_INVALID(x)       (x > PMIC_IRQMASK_REG_TYPE_ID) /*check id�Ϸ���*/

#define PMUSSI_TRACE(fmt, ...)    printk(fmt, ##__VA_ARGS__)

/*����һ��PMU register��������BIT����*/
#define PMU_REG_WIDTH  (8)

/*����PMU RECORD��Դ*/
#define PMU_SCP_RECORD1(_nr)    (_nr)
#define PMU_OCP_RECORD1(_nr)    (PMU_SCP_RECORD1(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD2(_nr)    (PMU_OCP_RECORD1(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD3(_nr)    (PMU_OCP_RECORD2(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD4(_nr)    (PMU_OCP_RECORD3(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD5(_nr)    (PMU_OCP_RECORD4(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD6(_nr)    (PMU_OCP_RECORD5(PMU_REG_WIDTH - 1) + (_nr) + 1)
#define PMU_OCP_RECORD7(_nr)    (PMU_OCP_RECORD6(PMU_REG_WIDTH - 1) + (_nr) + 1)

#define IRQ_OCP_LDO9    PMU_OCP_RECORD3(0)
#define IRQ_OCP_LDO11   PMU_OCP_RECORD3(2)
#define IRQ_OCP_LVS5    PMU_OCP_RECORD5(3)
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum EM_PMIC_IRQ_REG_ID_E
{
    PMIC_REG_IRQ1 = 0,
    PMIC_REG_IRQ2,
    PMIC_REG_IRQ3,
    PMIC_REG_IRQ4,
    PMIC_REG_COUL,
    PMIC_IRQ_REG_BUTT
}EM_PMIC_IRQ_REG_ID;

typedef enum EM_PMIC_IRQMASK_REG_ID_E
{
    PMIC_REG_IRQ1_MASK = 0,
    PMIC_REG_IRQ2_MASK,
    PMIC_REG_IRQ3_MASK,
    PMIC_REG_IRQ4_MASK,
    PMIC_REG_COUL_MASK,
    PMIC_IRQ_REG_MASK_BUTT
}EM_PMIC_IRQMASK_REG_ID;

typedef enum EM_PMIC_STATUS_REG_ID_E
{
    PMIC_STATUS1_REG,
    PMIC_STATUS2_REG,
    PMIC_STATUS_REG_BUTT
}EM_PMIC_STATUS_REG_ID;

typedef enum EM_PMIC_RECORD_REG_ID_E
{
    PMIC_SCP_RECORD1_REG,
    PMIC_OCP_RECORD1_REG,
    PMIC_OCP_RECORD2_REG,
    PMIC_OCP_RECORD3_REG,
    PMIC_OCP_RECORD4_REG,
    PMIC_OCP_RECORD5_REG,
    PMIC_OCP_RECORD6_REG,
    PMIC_OCP_RECORD7_REG,
    PMIC_RECORD_REG_BUTT
}EM_PMIC_RECORD_REG_ID;

typedef enum EM_PMIC_NP_IRQ_REG_ID_E
{
    PMIC_IRQ1_NO_PWR_REG,
    PMIC_IRQ2_NO_PWR_REG,
    PMIC_IRQ3_NO_PWR_REG,
    PMIC_SCP_RECORD_NO_PWR_REG,
    PMIC_OCP_RECORD1_NO_PWR_REG,
    PMIC_OCP_RECORD2_NO_PWR_REG,
    PMIC_OCP_RECORD3_NO_PWR_REG,
    PMIC_OCP_RECORD4_NO_PWR_REG,
    PMIC_OCP_RECORD5_NO_PWR_REG,
    PMIC_OCP_RECORD6_NO_PWR_REG,
    PMIC_OCP_RECORD7_NO_PWR_REG,
    PMIC_NO_PWR_REG_BUTT
}EM_PMIC_NP_IRQ_REG_ID;

#define PMIC_BUF_MAX_SIZE   PMIC_NO_PWR_REG_BUTT

typedef enum _CHANNEL_SWI_TYPE_
{
  CHANNEL_SWI_ENABLE,
  CHANNEL_SWI_DISABLE,
  CHANNEL_SWI_ONOFF_STATUS,
  CHANNEL_SWI_BUTTOM
}CHANNEL_SWI_TYPE;

typedef enum EM_PMICSTATUS_REG_TYPE_ID_E
{
    PMIC_IRQ_REG_TYPE_ID,/*�жϼĴ�����id*/
    PMIC_STAT_REG_TYPE_ID,/*״̬�Ĵ�����id*/
    PMIC_RECORD_REG_TYPE_ID,
    PMIC_NOPWR_REG_TYPE_ID, /*���µ�Ĵ�����id*/
    PMIC_IRQMASK_REG_TYPE_ID, /*�ж����μĴ�����id*/
    PMIC_REG_TYPE_ID_BUTT
}EM_PMICSTATUS_REG_TYPE_ID;
/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/* ��¼��ѹԴ�����Դ���õļĴ�����ַ�������õ�bitλ��bitλ�ڼĴ�����ַ�е�ƫ��������bit�ƣ�*/
/*  �����������������ש����������ש�������������
    ���Ĵ�������ַ  ��bitƫ���� ������bitλ�� ��
    �����������������ߩ����������ߩ�������������    */
typedef struct pmu_common_reg_config
{
    BSP_U32 baseaddr;    /*�Ĵ�������ַ*/
    BSP_U32 bits_offset; /*������bit��ƫ����*/
    BSP_U32 bits_num;    /*�����ļ���bit*/
}COMMON_REG_CFG;

/*  �������������ש���������������������
    ���Ĵ�����ַ����ѹ����bitλ��     ��
    �������������ߩ���������������������    */
typedef struct pmu_common_switch
{
    COMMON_REG_CFG reg_cfg[CHANNEL_SWI_BUTTOM];
    BSP_U32 def_sw; /*��ʼ����״̬*/
}COMMON_SWITCH;

/* ��¼��ѹԴ�����Դ���õļĴ�����ַ�������õ�bitλ��bitλ�ڼĴ�����ַ�е�ƫ��������bit�ƣ�*/
/*  �����������������ש��������ש������������ש����������ש�������������
    ����Чֵ��Χ���驧�����С���Ĵ�������ַ��bitƫ���� ������bitλ�� ��
    �����������������ߩ��������ߩ������������ߩ����������ߩ�������������    */
typedef struct pmu_common_value_set
{
    BSP_U32 *value_range;   /*����ֵ��Ч��Χ����  eg ldo1_set*/
    BSP_U32 value_range_num; /*��Ч��Χ�����С eg sizeof(ldo1_set)*/
    COMMON_REG_CFG reg_cfg;
}COMMON_VALUE_SET;

/*bat channel & voltage relation table*/
typedef struct pmu_common_resource
{
    BSP_U32 id;/*��Դ id*/
    signed char  name[PMU_POWER_NAME_SIZE];/*��Դ������*/
    COMMON_SWITCH power_switch;     /*��Դ����*/
    COMMON_VALUE_SET value_set;     /*��Դ����ֵ*/
}COMMON_RES;

typedef struct _PMU_RES_CTRL_
{
    COMMON_RES *res;/*��Դ����*/
    BSP_U32 res_num;/*��Դ����*/
    BSP_U32 data_width;  /*PMU���ݿ�ȣ�ĿǰΪ8bit*/
    COMMON_REG_CFG version;
}PMU_RES_CTRL;

typedef struct _PMUIRQ_RES_CTRL_
{
    BSP_U32 data_width;  /*PMU���ݿ�ȣ�ĿǰΪ8bit*/
    /*�жϼĴ�����״̬�Ĵ����ͷ��µ�Ĵ�������*/
    BSP_U32*stat_reg[PMIC_REG_TYPE_ID_BUTT];
    /*�жϼĴ�����״̬�Ĵ����ͷ��µ�Ĵ��������С*/
    BSP_U32 stat_reg_size[PMIC_REG_TYPE_ID_BUTT];
}PMUIRQ_RES_CTRL;

typedef void (*mntn_reg_ops)(void);

typedef struct _PMIC_MNTN_POWER_ON_OPS_
{
    mntn_reg_ops no_pwr_read;
    mntn_reg_ops no_pwr_clear;
    mntn_reg_ops irq_read;
    mntn_reg_ops irq_clear;
    mntn_reg_ops record_read;
    mntn_reg_ops record_clear;
    mntn_reg_ops mntn_power_on_info;
}PMIC_MNTN_POWER_ON_OPS;

typedef struct _PMU_MNTN_LOG_
{
 COMMON_REG_CFG exc_addr;
 BSP_U32  is_exc_pd;
 char* mntn_log;
}PMU_MNTN_LOG;

typedef struct _PMUIRQ_STAT_
{
    /*�жϼĴ�����״̬�Ĵ����ͷ��µ�Ĵ������飬�Ĵ�������Ϊ(stat_reg_size)*/
    BSP_U32* stat_reg_buf[PMIC_REG_TYPE_ID_BUTT];
    BSP_U32* mask_vote;
    BSP_U32* unmask_vote;
    BSP_U32  is_exc_pd;
}PMUIRQ_STAT;

typedef struct _PMIC_MNTN_DESC_
{
    PMIC_MNTN_POWER_ON_OPS* mntn_ops;
    struct notifier_block* power_off_notifier;
    PMU_MNTN_LOG* power_onoff_event;
    struct workqueue_struct *ocp_scp_wq;
    struct work_struct ocp_scp_wk;
    void (*ocp_scp_wk_ops)(struct work_struct *work);
    struct workqueue_struct *mntn_init_wq;
    struct delayed_work mntn_init_wk;
    void (*mntn_init_wk_ops)(struct work_struct *work);
    struct workqueue_struct *otmp_wq;
    struct work_struct otmp_wk;
    void (*otmp_wk_ops)(struct work_struct *work);
    PMUIRQ_STAT             stat;
}PMIC_MNTN_DESC;

typedef struct _PMU_CPU_HANDLER_
{
    int             reg_id;
    int             bit_id;
}PMU_CPU_HANDLER;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

