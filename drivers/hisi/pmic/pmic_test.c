/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ssi_test.c
  �� �� ��   : ����
  ��    ��   :
  ��������   :
  ����޸�   :
  ��������   :
  �����б�   :

  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   :

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <mach/irqs.h>

#include "soc_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include "pmic_common.h"
#include "pmic_driver.h"
#include "pmic_ssi.h"
#include "pmicirq_driver.h"

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
#define DEBUG

#ifdef DEBUG
#define MSG(message, args...) printk("irq:" message, ##args)
#else
#define MSG(message, args...)
#endif

int irq = IRQ_VBUS_COMP_VBAT_RISING;
char *interface = "pmutestirq";
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
int ssi_test(int rw, int addr,unsigned int data )
{
    if (0 == rw)/*read*/
    {
        BSP_U32 get_data = 0;
        BSP_PMU_RegGet(addr,&get_data);
        PMUSSI_TRACE("GET DATA  0x%0x\n",get_data);
    }
    else if (1 == rw)/*write*/
    {
        PMUSSI_TRACE("WRITE DATA  0x%0x\n",data);
        BSP_PMU_RegSet(addr,data);
        PMUSSI_TRACE("WRITE FIN\n");
    }

    return 0;
}

void d_set_v(int id ,int min_uV, int max_uV)
{
    int selector = 0;
    pmic_set_voltage(id,min_uV,max_uV,&selector);
    PMUSSI_TRACE("voltage selector num  %d\n",selector);
}


// �жϲ����ʱ��Ĵ�����
int irq_handle_function(int irq, void *device_id)
{
    static int count = 1;
    MSG("[%d]receive the irq %d at %ld...\n",count,irq,jiffies);
    count++;
    return IRQ_HANDLED;
}
int irqtest_init(void)
{

    if(request_irq(irq, (irq_handler_t)irq_handle_function, IRQF_SHARED, interface, (void *)&irq))
    {
        MSG("regist irq failure...\n");
        return -EIO;
    }

    MSG("interface=%s and irq=%d...\n", interface, irq);
    MSG("regist irq success...\n");
    return 0;
}

void irqtest_exit(void)
{
    free_irq(irq, &irq);
    MSG("unregist irq.../n");
}

#if 0
module_init(irqtest_init);
module_exit(irqtest_exit);
#endif 

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


