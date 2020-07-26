

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef __PMIC_MNTN_DEV_INNER_H__
#define __PMIC_MNTN_DEV_INNER_H__

#include "soc_smart_interface.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/delay.h>

#include <asm/current.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/thread_info.h>
#include <linux/syslog.h>
#include <linux/rtc.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/notifier.h>
#include <linux/device.h>
#include <linux/hardirq.h>
#include <linux/reboot.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/sys.h>
#include <linux/reboot.h>
#include <linux/workqueue.h>

#include  <asm/io.h>

#include <mach/pmussi_drv.h>
#include <mach/hisi_cortex-a9.h>
#include <mach/irqs.h>

#include "DrvNvInterface.h"
#include "../../mntn/excDrv.h"
#include "bsp_mailbox.h"
#include "NvIdList.h"
#include "balong_nvim.h"

#include "pmic_common.h"
#include "pmicirq_driver.h"
#include "pmic_reg.h"
#include "pmic_mntn.h"
#include "pmic_mntn_dev.h"
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define PMU_SHELL_SCRIPT        "/system/etc/log/pmu_reboot.sh"
#define PRINT_MAX_LEN 1024
#define IS_CCPU_EVENT  1
#define NOT_CCPU_EVENT 0

#define TIMEOUT 1
#define NO_TIMEOUT 0
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum PMIC_MNTN_E
{
    PMU_SMART_DEV = 0,
    PMU_MNTN_DEV_BUTT
}PMIC_MNTN_DEV_ID;
/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

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
extern PMUIRQ_DATA  pmuirq_desc;
extern long systemErrorCcore(int modId, int arg1, int arg2, char *arg3, int arg3Length, unsigned long timeout);
extern int mach_call_usermodeshell(char *script, unsigned int params, ...);

void smart_ocp_scp_wq_hander(struct work_struct *work);
static int smart_power_off(struct notifier_block *self, unsigned long event, void *data);
void smart_mntn_init(struct work_struct *work);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

