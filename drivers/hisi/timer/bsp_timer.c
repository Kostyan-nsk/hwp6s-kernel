/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : BSP_TIMER.c
  �� �� ��   : ����
  ��    ��   : ��ϲ 220237
  ��������   : 2013��3��30��
  ����޸�   :
  ��������   : Timer�߼���
  �����б�   :
              BSP_TIMER_Init
              BSP_TIMER_IntHandle
              BSP_TIMER_UsrClkConnect
              BSP_TIMER_UsrClkDisable
              BSP_TIMER_UsrClkEnableMs
              BSP_TIMER_UsrClkEnableUs
              BSP_TIMER_Show
  �޸���ʷ   :
  1.��    ��   : 2012��3��30��
    ��    ��   : ��ϲ 220237
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
  *****************************************************************************/
#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/io.h>

#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

#include "soc_timer_interface.h"
#include "soc_sctrl_interface.h"
#include "DrvInterface.h"
#include "bsp_timer_phy.h"
#include "bsp_timer.h"
#include "bsp_ao_sctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


extern BSP_U32 pulTimerBase[TIMER_TOTAL_NUM + CCORE_TIMER_EXP];
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*�������timer����Ϣ*/
TIMER_PARA g_timer_para[TIMER_TOTAL_NUM + CCORE_TIMER_EXP] = {{0}};

 /*��timer���ƼĴ�����ʹ��λ��д���Ᵽ��*/
spinlock_t timer_en_lock;
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
    

BSP_S32 BSP_TIMER_InitTimerBase(void)
{        
    void __iomem *timer_base = NULL;

    /* ӳ��timer2,3 */
    timer_base = ioremap(SOC_Timer1_BASE_ADDR, TIMER_REG_SIZE);
    if (!timer_base)
    {
        return BSP_ERROR;
    }
    pulTimerBase[2] = (BSP_U32)timer_base;
    pulTimerBase[2+1] = (BSP_U32)timer_base + 0x20;

    /* ӳ��timer10,11 */
    timer_base = ioremap(SOC_Timer5_BASE_ADDR, TIMER_REG_SIZE);
    if (!timer_base)
    {
        return BSP_ERROR;
    }
    pulTimerBase[10] = (BSP_U32)timer_base;
    pulTimerBase[10+1] = (BSP_U32)timer_base + 0x20;

    /* ӳ��timer12,13 */
    timer_base = ioremap(SOC_Timer6_BASE_ADDR, TIMER_REG_SIZE);
    if (!timer_base)
    {
        return BSP_ERROR;
    }
    pulTimerBase[12] = (BSP_U32)timer_base;
    pulTimerBase[12+1] = (BSP_U32)timer_base + 0x20;

    /* ӳ��C��timer8_1�����±�TIMER_TOTAL_NUM��λ�� */
    timer_base = ioremap(SOC_MTimer8_BASE_ADDR, TIMER_REG_SIZE);
    if (!timer_base)
    {
        return BSP_ERROR;
    }
    pulTimerBase[TIMER_TOTAL_NUM] = (BSP_U32)timer_base + 0x20;
    return BSP_OK;
}
    

BSP_VOID BSP_TIMER_SetCcoreTimerInfo(void)
{
    g_timer_para[TIMER_TOTAL_NUM].ticksPerSecond = TIMER_DEFAULT_TICKS;
    g_timer_para[TIMER_TOTAL_NUM].inputClock = TCXO_CLK_FREQ;
}


BSP_U32 TIMER_BASE(BSP_U32 clk_logic_id)
{   
    BSP_S32 clk_phy_id;
    clk_phy_id = timer_find_phy_id((BSP_U8)clk_logic_id);
    if ((clk_phy_id < 0) ||(clk_phy_id >= TIMER_ID_MAX_PHY) )
    {
        printk(TIMER_PRINT_ERR "TIMER_BASE: clk_phy_id(%d) is invalid.\n", clk_phy_id);
        return TIMER_INVALID_ADDR;
    } 
    return pulTimerBase[clk_phy_id];
}


BSP_S32 BSP_TIMER_Init(BSP_VOID)
{
    BSP_U8 i;
    BSP_U32 base = IO_ADDRESS(REG_BASE_SC_ON);
    int ret = 0;
    if (BSP_ERROR == BSP_TIMER_InitTimerBase())
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_Init: timer reg ioremap error.\n");
        return BSP_ERROR;
    }

    spin_lock_init(&timer_en_lock);

    /* ��ʱ���ſ�DTimer1,DTimer6��д0��Ӱ�� */
    TIMER_REG_WRITE(SOC_AO_SCTRL_SC_PERIPH_CLKEN4_ADDR(base),
        (SET_BITS_WIHT_TRUETRUE<<SOC_AO_SCTRL_SC_PERIPH_CLKEN4_periph_clken4_timer1_pclk_START) |
        (SET_BITS_WIHT_TRUETRUE<<SOC_AO_SCTRL_SC_PERIPH_CLKEN4_periph_clken4_timer6_pclk_START));
    
    /* ��ʱ������д��ϵͳ������ */
    (BSP_VOID)timer_sys_ctrl_set();
    
    /* ���ø���Timerʱ��Դ */
    for (i = 0; i < TIMER_ID_MAX_PHY; i++)
    {
        g_timer_para[i].ticksPerSecond = TIMER_DEFAULT_TICKS;

        /* �õ�ÿ��Timer������ʱ������ */
        g_timer_para[i].inputClock = timer_input_clk_get(i);        
    }    

    /* ����A�˿��Է��ʵ�C��Timer��Ϣ�����ڻ���ʣ��ʱ�� */
    BSP_TIMER_SetCcoreTimerInfo();

    ret = request_irq((unsigned int)INUM_TO_IVEC(INT_IRQ_TIMER2), BSP_TIMER_IntHandle, 
        IRQF_DISABLED | IRQF_TIMER, "TIMER_ACPU_OSA_ID_PHY", (void*)TIMER_ACPU_OSA_ID_PHY); 
    if(ret)
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_Init: %d irq error.\n", TIMER_ACPU_OSA_ID_PHY);
    }
   
    ret = request_irq((unsigned int)INUM_TO_IVEC(INT_IRQ_TIMER3), BSP_TIMER_IntHandle, 
        IRQF_DISABLED | IRQF_TIMER, "TIMER_ACPU_SOFTTIMER_ID_PHY", (void*)TIMER_ACPU_SOFTTIMER_ID_PHY);
    if(ret)
    {
         printk(TIMER_PRINT_ERR "BSP_TIMER_Init: %d irq error.\n", TIMER_ACPU_SOFTTIMER_ID_PHY);
    }
   
    ret = request_irq((unsigned int)INUM_TO_IVEC(INT_IRQ_TIMER12), BSP_TIMER_IntHandle,
        IRQF_DISABLED | IRQF_TIMER, "TIMER_ACPU_IDLE_ID_PHY", (void*)TIMER_ACPU_IDLE_ID_PHY);
    if(ret)
    {
         printk(TIMER_PRINT_ERR "BSP_TIMER_Init: %d irq error.\n", TIMER_ACPU_IDLE_ID_PHY);
    }
   	
    ret = request_irq((unsigned int)INUM_TO_IVEC(INT_IRQ_TIMER13), BSP_TIMER_IntHandle,
        IRQF_DISABLED | IRQF_TIMER, "TIMER_ACPU_OM_TCXO_ID_PHY", (void*)TIMER_ACPU_OM_TCXO_ID_PHY);
    if(ret)
    {
         printk(TIMER_PRINT_ERR "BSP_TIMER_Init: %d irq error.\n", TIMER_ACPU_OM_TCXO_ID_PHY);
    }

    return BSP_OK;
}
irqreturn_t BSP_TIMER_IntHandle(int irq, void* para)
{
    BSP_U32 readValue;
    BSP_U8 timerID;

    timerID = (BSP_U8)(BSP_U32)(para);
    if (timerID >= TIMER_ID_MAX_PHY)
    {
        return IRQ_NONE;
    }

    readValue = timer_int_status_get(timerID);

    /* �ж���Ч */
    if (0x0 != readValue)
    {
        timer_int_clear(timerID);
        if (BSP_NULL != g_timer_para[timerID].routine)
        {
            /* �����жϴ������Ͳ��� */
            (BSP_VOID)((*(g_timer_para[timerID].routine))(g_timer_para[timerID].arg));
        }
    }
    return IRQ_HANDLED;
}


BSP_S32 BSP_TIMER_UsrClkConnect(FUNCPTR_1 routine, BSP_S32 arg, BSP_U32 clk_phy_id)
{
    if ((clk_phy_id >= TIMER_ID_MAX_PHY) || (TIMER_ACPU_SYSTEM_ID_PHY == clk_phy_id))
    {
        return BSP_ERROR;
    }
    
    if (BSP_OK == timer_is_running(clk_phy_id))
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkConnect: g_timer_para[%d].running!\n",clk_phy_id);
        return BSP_ERROR;
    }
    
    g_timer_para[clk_phy_id].routine = BSP_NULL;
    g_timer_para[clk_phy_id].arg = arg;
    g_timer_para[clk_phy_id].routine = routine;

    return BSP_OK;
}


BSP_S32 BSP_TIMER_UsrClkDisable(BSP_U32 clk_logic_id)
{
    BSP_S32 clk_phy_id;
    
    clk_phy_id = timer_find_phy_id((BSP_U8)clk_logic_id);    
    if ((clk_phy_id < 0) ||(clk_phy_id >= TIMER_ID_MAX_PHY) || (TIMER_ACPU_SYSTEM_ID_PHY == clk_phy_id))
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkDisable: clk_phy_id(%d) is invalid.\n", clk_phy_id);
        return BSP_ERROR;
    }

    if (BSP_OK == timer_is_running(clk_phy_id))
    {
        timer_stop((BSP_U8)clk_phy_id);
        g_timer_para[clk_phy_id].running = BSP_FALSE;
    }
    
    return BSP_OK;
}


BSP_S32 BSP_TIMER_UsrClkEnableMs(BSP_U32 clk_phy_id, BSP_U32 value, BSP_U32 mode)
{
    BSP_U32 count = 0;

    if (g_timer_para[clk_phy_id].running)
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkEnableMs: g_timer_para[%d].running!\n", clk_phy_id);
        return BSP_ERROR;
    }
     
    /* ȷ��������ֵ��   ����19.2MHz�ȳ�1000�������value��ȡֵ��Χ[1,223696]
                        ����32KHz�ȳ�value���Ա�֤�Ĵ����ľ��ȣ���ȡֵ��Χ[1,131071]*/
    if (TCXO_CLK_FREQ == g_timer_para[clk_phy_id].inputClock)
    {
        count = g_timer_para[clk_phy_id].inputClock / Second_To_Millisecond * value;
    }
    else if (SLEEP_CLK_FREQ == g_timer_para[clk_phy_id].inputClock)
    {           
        count = g_timer_para[clk_phy_id].inputClock * value / Second_To_Millisecond;
        
    }
    else
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkEnableMs: ERROR FREQ!\n");
        return BSP_ERROR;
    }

    timer_stop((BSP_U8)clk_phy_id);

    /* ���üĴ���timer_load��ֵ,��������ʱ�� */
    timer_load_set((BSP_U8)clk_phy_id, count);
    timer_start((BSP_U8)clk_phy_id, mode);

    /* ����timer����״̬��־ */
    g_timer_para[clk_phy_id].running = BSP_TRUE;

    return BSP_OK;
}


BSP_S32 BSP_TIMER_UsrClkEnableUs(BSP_U32 clk_phy_id, BSP_U32 value, BSP_U32 mode)
{
     BSP_U32 count = 0;

    if (g_timer_para[clk_phy_id].running)
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkEnableUs: g_timer_para[%d].running!\n", clk_phy_id);
        return BSP_ERROR;
    }

    /* ��19.2M��ʱ�ӣ��������� */
    if (TCXO_CLK_FREQ == g_timer_para[clk_phy_id].inputClock)
    {
        count = value * (g_timer_para[clk_phy_id].inputClock / Second_To_Millisecond) / Second_To_Millisecond;
    }
    /* ��32K��ʱ�ӣ����⴦����С�̶�Լ31us */
    else if (SLEEP_CLK_FREQ == g_timer_para[clk_phy_id].inputClock)
    {
        if (value > TIMER_32K_US_BOUNDARY)
        {
            count = (value * g_timer_para[clk_phy_id].inputClock) / Second_To_Millisecond / Second_To_Millisecond;
        }
        else
        {
            /* ���ڲ�����31us�Ķ�ʱ��ֱ����load�Ĵ���д1 */
            count = 1;
        }
    }
    /* ����Ƶ�ʵ�ʱ�ӱ��� */
    else
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkEnableUs: error! CLK_FREQ is error!\n");
        return BSP_ERROR;
    }

    timer_stop((BSP_U8)clk_phy_id);

    /* ���üĴ���timer_load��ֵ,��������ʱ�� */
    timer_load_set((BSP_U8)clk_phy_id, count);
    timer_start((BSP_U8)clk_phy_id, mode);

    /* ����timer����״̬��־ */
    g_timer_para[clk_phy_id].running = BSP_TRUE;

    return BSP_OK;
}


BSP_S32 BSP_TIMER_UsrClkEnableNone(BSP_U32 clk_phy_id, BSP_U32 value, BSP_U32 mode)
{
    if (g_timer_para[clk_phy_id].running)
    {
        printk(TIMER_PRINT_ERR "g_timer_para[%d].running!\n", clk_phy_id);
        return BSP_ERROR;
    }

    timer_stop((BSP_U8)clk_phy_id);

    /* ���üĴ���timer_load��ֵ,��������ʱ�� */
    timer_load_set((BSP_U8)clk_phy_id, value);
    timer_start((BSP_U8)clk_phy_id, mode);

    /* ����timer����״̬��־ */
    g_timer_para[clk_phy_id].running = BSP_TRUE;

    return BSP_OK;
}


BSP_S32 BSP_TIMER_GetRestTime(BSP_U32 clk_logic_id, BSP_U32 unitType, BSP_U32 * pRestTime)
{       
    BSP_U32 RestCount = 0;
    BSP_S32 clk_phy_id = 0;
    
    clk_phy_id = timer_find_phy_id((BSP_U8)clk_logic_id);
    
    /* ���ѯC��Timer��valueֵ���ſ��������*/
    if ((clk_phy_id < 0) || (clk_phy_id >= (TIMER_TOTAL_NUM + CCORE_TIMER_EXP)) || (TIMER_ACPU_SYSTEM_ID_PHY == clk_phy_id))
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_GetRestTime: clk_phy_id(%d) is invalid.\n", clk_phy_id);
        return BSP_ERROR;
    }
        
    RestCount = timer_value_get((BSP_U8)clk_phy_id);
    
    if ((TIMER_UNIT_US == unitType) && (0 != g_timer_para[clk_phy_id].inputClock))
    {
        RestCount = (RestCount * Second_To_Millisecond) / (g_timer_para[clk_phy_id].inputClock / Second_To_Millisecond);
    }
    else if ((TIMER_UNIT_MS == unitType) && (0 != g_timer_para[clk_phy_id].inputClock))
    {
        RestCount = RestCount / (g_timer_para[clk_phy_id].inputClock / Second_To_Millisecond);
    }
    else if (TIMER_UNIT_NONE == unitType)
    {
        /* ����Ҫ��RestCount���д��� */
    }
    else
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_GetRestTime: Cannot get RestTime!");
        return BSP_ERROR;
    } 
    
    *pRestTime = RestCount;
    
    return BSP_OK;
}


BSP_S32 BSP_TIMER_UsrClkStart
(
    BSP_U32     clk_logic_id,
    FUNCPTR_1   routine,
    BSP_S32     arg,
    BSP_U32     timerValue,
    BSP_U32     mode,
    BSP_U32     unitType
)
{    
    BSP_S32 ConnectResult = BSP_ERROR;
    BSP_S32 EnableResult = BSP_ERROR;
    BSP_S32 clk_phy_id;

    clk_phy_id = timer_find_phy_id((BSP_U8)clk_logic_id);
    if ((clk_phy_id < 0) ||(clk_phy_id >= TIMER_ID_MAX_PHY) || (TIMER_ACPU_SYSTEM_ID_PHY == clk_phy_id))
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkStart: error! clk_logic_id = %d, clk_phy_id = %d.\n", clk_logic_id, clk_phy_id);
        return BSP_ERROR;
    }

    if ((mode >= TIMER_COUNT_BUTT) || (unitType >= TIMER_UNIT_BUTT))
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkStart: error! mode = %d, unitType = %d.\n", mode, unitType);
        return BSP_ERROR;
    }

    /* �ҽ��û�ʱ���жϴ����� */
    ConnectResult = BSP_TIMER_UsrClkConnect(routine, arg, clk_phy_id);
    if (BSP_OK != ConnectResult)
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkStart: Connect fail! clk_phy_id=%d\n", clk_phy_id);
        return BSP_ERROR;
    }

    /* �������ֵ��ʹ�ܶ�ʱ�� */    
    if (TIMER_UNIT_MS == unitType)
    {
        EnableResult = BSP_TIMER_UsrClkEnableMs(clk_phy_id, timerValue, mode);
    }
    else if (TIMER_UNIT_US == unitType)
    {
        EnableResult = BSP_TIMER_UsrClkEnableUs(clk_phy_id, timerValue, mode);
    }
    else if (TIMER_UNIT_NONE == unitType)
    {
        EnableResult = BSP_TIMER_UsrClkEnableNone(clk_phy_id, timerValue, mode);
    }
    else
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkStart: unitType is error!");
        return BSP_ERROR;
    }    
    
    if (BSP_OK != EnableResult)
    {
        printk(TIMER_PRINT_ERR "BSP_TIMER_UsrClkStart: Enable fail!clk_phy_id=%d\n",clk_phy_id);
        return BSP_ERROR;
    }

    return BSP_OK;
}


BSP_S32 BSP_TIMER_Show(BSP_VOID)
{
#if 0    
    BSP_S32 i = 0;
    BSP_S32 LoadValue = 0;
    BSP_S32 mode = 0;
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG = NULL;

    BSP_CHAR sTimerID[TIMER_ACPU_ID_MAX][24] = {
        "TIMER_OM_TCXO_TIMER",
        "TIMER_ACPU_SYSTEM_ID",
        "TIMER_ACPU_SOFTTIMER_ID",
        "TIMER_ACPU_OSA_ID",
        "TIMER_ACPU_CODEC_ID",
        "TIMER_ACPU_APP_ID",
    };

    printk("===============================================================================\n");
    printk("TimerID IDName                  InputClock IsRunning LoadRegValue IsPeriodicMode\n");
    for(i = TIMER_OM_TCXO_TIMER_PHY; i < TIMER_ID_MAX; i++)
    {
        LoadValue = (BSP_S32)timer_load_get((BSP_U8)i);
        pTIMER_CNTL_REG = (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[i]);

        /* oneshot: 0��ʾ����ģʽ 1��ʾ����ģʽ */
        mode = (BSP_S32)(!(pTIMER_CNTL_REG->reg.oneshot)); 
        printk("%-7d %-23s %-10d %-9d 0x%-10x %-14d\n", i, sTimerID[i - TIMER_ACPU_ID_BEGIN] , g_timer_para[i].inputClock, g_timer_para[i].running, LoadValue, mode);
    }
    printk("===============================================================================\n");
#endif    
    return BSP_OK;
}
BSP_U32 BSP_TIMER_Query(BSP_U8 clk_phy_id)
{
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].load = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]);
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].value = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]+0x4);
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].ctrl = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]+0x8);
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].INTCLR = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]+0xc);
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].RIS = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]+0x10);
    printk(TIMER_PRINT_ERR "pulTimerBase[%d].MIS = %d \n", clk_phy_id, pulTimerBase[clk_phy_id]+0x14);
    return 0;
}


int timer_base_show(void)
{
    int i=0;

     for(i=0;i<(TIMER_TOTAL_NUM + CCORE_TIMER_EXP);i++){
        printk(TIMER_PRINT_ERR "pulTimerBase[%d]=0x%x\n",i,pulTimerBase[i]);
     }
    
return 0;
}


int bsp_time_info_dump(void)
{
    int i=0;
    TIMER_PARA *cur_item=NULL;

     for(i=0;i<TIMER_TOTAL_NUM;i++){
        cur_item=&(g_timer_para[i]);
        printk(TIMER_PRINT_ERR "cur_item[%d].routine=%x\n",i, (unsigned int)cur_item->routine);
        printk(TIMER_PRINT_ERR "cur_item[%d].arg=%d\n",i,cur_item->arg);
        printk(TIMER_PRINT_ERR "cur_item[%d].running=%d\n",i,cur_item->running);
        printk(TIMER_PRINT_ERR "cur_item[%d].tickPerSecond=%d\n",i,cur_item->ticksPerSecond);
        printk(TIMER_PRINT_ERR "cur_item[%d].inputClock=%d\n",i,cur_item->inputClock);
     }
    
return 0;
}
EXPORT_SYMBOL(timer_base_show);
EXPORT_SYMBOL(BSP_TIMER_UsrClkStart);
EXPORT_SYMBOL(BSP_TIMER_UsrClkDisable);
EXPORT_SYMBOL(BSP_TIMER_GetRestTime);
EXPORT_SYMBOL(BSP_TIMER_Show);
EXPORT_SYMBOL(BSP_TIMER_Query);
EXPORT_SYMBOL(bsp_time_info_dump);




/* just for test,begin */
#define TIMER_DEBUG
#ifdef TIMER_DEBUG
BSP_U32 acore_timer_milliSecond = 5000;  /* 5s */
BSP_U32 acore_timer_mode = 0;            /* 0:���Σ�1:���� */
BSP_U32 acore_timer_unitType = 2;        /* unitType:0��ʾ��λms��1��ʾ��λus��2��ʾֱ��дload�Ĵ�������timer */
BSP_S32 IntCount[TIMER_ID_MAX] = {0};   /* Ϊÿһ������timer����ռ� */
BSP_U32 acore_timer_restTime = 0;
void tsTimerSetPara(BSP_U32 milliSecond, BSP_U32 mode, BSP_U32 unitType)
{
    acore_timer_milliSecond = milliSecond;
    acore_timer_mode        = mode;
    acore_timer_unitType    = unitType;
    printk(TIMER_PRINT_ERR "acore_timer_milliSecond = %d.\n", acore_timer_milliSecond);
    printk(TIMER_PRINT_ERR "acore_timer_mode        = %d.\n", acore_timer_mode);
    printk(TIMER_PRINT_ERR "acore_timer_unitType    = %d.\n", acore_timer_unitType);
}
void Init_IntCount(void)
{
    int i = 0;
    for(i = 0; i < TIMER_ID_MAX; i++)
    {
        IntCount[i] = 0;
    }
}

/* �����߱�֤�����±겻Խ�� */
BSP_S32 tsTimerComplete(BSP_S32 arg)
{
    IntCount[arg]++;
    printk(TIMER_PRINT_INFO "tsTimerComplete!usrClkId=%d,IntCount[%d]=%d.\n", (BSP_S32)arg, arg,IntCount[arg]);
    return 0;
}

/* �����߱�֤�ö���timer��a�˿��Է��ʣ� 
   Ŀ��: shell�ڲ��ðѺ���tsTimerComplete���������˺���ֻΪ����ϸ��ʱʹ�ã�ͨ���ò��� */
BSP_S32 tsTimerConnect(BSP_U32 clk_logic_id)
{
    BSP_U32 clk_phy_id;
    clk_phy_id = timer_find_phy_id((BSP_U8)clk_logic_id);
    return BSP_TIMER_UsrClkConnect((FUNCPTR_1)tsTimerComplete, (BSP_S32)clk_phy_id, clk_phy_id);
}

/*****************************************************************************
  �����Ƕ���ӿڲ���
*****************************************************************************/

int tsTimerGetRestTime(BSP_U32 clk_logic_id)
{
    int result = 0;
    result = BSP_TIMER_GetRestTime(clk_logic_id, acore_timer_unitType, &acore_timer_restTime);
    printk(TIMER_PRINT_ERR "clk_logic_id=%d, RestTime=0x%x.\n", clk_logic_id, acore_timer_restTime);
    return result;
}

/* ����1��timer�� 5s, ����, ��׼ȷ�� ��start�ӿ� */
int tsTimerClkGating(BSP_U32 test_id)
{
    BSP_U32  clk_logic_id_1 = 6;
    BSP_U32  clk_logic_id_2 = 13;
    BSP_U32  l_timerValue=5000;
    BSP_U32  l_mode=1;   /*����*/
    BSP_U32   l_unitType=0;   /*��λms*/

    unsigned long base = IO_ADDRESS(REG_BASE_SC_ON);
    BSP_U32 result=0;

    #if 1
    if (2 == test_id)
    {
        BSP_TIMER_UsrClkStart(clk_logic_id_1, tsTimerComplete, clk_logic_id_1,  l_timerValue, l_mode, l_unitType);
        BSP_TIMER_UsrClkStart(clk_logic_id_2, tsTimerComplete, clk_logic_id_2,  l_timerValue, l_mode, l_unitType);
    }


    if (1 == test_id)
    {
        /*stop clk_logic_id_1*/
        BSP_TIMER_UsrClkDisable(clk_logic_id_1);
        TIMER_REG_READ(SOC_AO_SCTRL_SC_PERIPH_CLKSTAT4_ADDR(base), result);
        printk(TIMER_PRINT_ERR "SC_PERIPH_CLKSTAT4: 0x%x\n",result);
    }

    if (0 == test_id)
    {
        /*stop clk_logic_id_2*/
        BSP_TIMER_UsrClkDisable(clk_logic_id_2);
        TIMER_REG_READ(SOC_AO_SCTRL_SC_PERIPH_CLKSTAT4_ADDR(base), result);
        printk(TIMER_PRINT_ERR "SC_PERIPH_CLKSTAT4: 0x%x\n",result);
    }
    #endif
    return BSP_OK;
}

/* ����1��timer�� 5s, ����, ��׼ȷ�� ��start�ӿ� */
int tsTimerStartOne(BSP_U32 clk_logic_id)
{
    return BSP_TIMER_UsrClkStart(clk_logic_id, tsTimerComplete, clk_logic_id, acore_timer_milliSecond, acore_timer_mode, acore_timer_unitType);
}

/* ����all timer�� 1s, ����, ΢��ģʽ �⹦�� ��֤�жϡ�����ַ */
int tsTimerStartAll(void)
{
/*    BSP_TIMER_UsrClkStart(22, tsTimerComplete, 22, 1000, 0, 0); */
    BSP_TIMER_UsrClkStart(23, tsTimerComplete, 23, 1000, 0, 0);
    BSP_TIMER_UsrClkStart(6,  tsTimerComplete, 6,  1000, 0, 0);
    BSP_TIMER_UsrClkStart(13, tsTimerComplete, 13, 1000, 0, 0);
    return 0;
}

/* �⹦�� */
int tsTimerStopAll(void)
{


/*    BSP_TIMER_UsrClkDisable(22); */
    BSP_TIMER_UsrClkDisable(23);
    BSP_TIMER_UsrClkDisable(6);
    BSP_TIMER_UsrClkDisable(13);
    return 0;
}
EXPORT_SYMBOL(tsTimerSetPara);
EXPORT_SYMBOL(tsTimerStartOne);
EXPORT_SYMBOL(tsTimerStartAll);
EXPORT_SYMBOL(tsTimerStopAll);
#endif
/* just for test,end */

module_init(BSP_TIMER_Init);
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

