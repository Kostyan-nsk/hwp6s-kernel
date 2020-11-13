

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __PMIC_IRQ_DRIVER_H__
#define __PMIC_IRQ_DRIVER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/bitops.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/irqnr.h>
#include <linux/err.h>

#include <mach/early-debug.h>
#include <mach/gpio.h>
#include <mach/irqs.h>

#include "soc_smart_interface.h"
#include "soc_ao_sctrl_interface.h"

#include "pmic_driver.h"
#include "pmic_common.h"
#include "regulator_pmic_info.h"
#include "pmic_ssi.h"
#include "pmic_reg.h"

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define PMIC_NOMAL_PD 0
#define PMIC_EXC_PD   1
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct __PMUIRQ_DATA__
{
     struct platform_device *pdev;
     BSP_U32                 version ;
     unsigned  int           irq;
     spinlock_t              pmic_spinlock ;
     PMIC_MNTN_DESC*         mntn_desc;
}PMUIRQ_DATA;

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
extern void pmu_irq_unmask(unsigned int irq_num);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of __PMIC_IRQ_DRIVER_H__ */

