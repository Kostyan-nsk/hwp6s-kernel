/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : BSP_TIMER_PHY.c
  �� �� ��   : ����
  ��    ��   : ��ϲ 220237
  ��������   : 2013��3��30��
  ����޸�   :
  ��������   : �ṩTimer�Ĵ������ƹ���
  �����б�   :
              timer_32bit_count_set
              timer_input_clk_get
              timer_int_clear
              timer_int_mask
              timer_int_status_get
              timer_int_unmask
              timer_load_get
              timer_load_set
              timer_raw_int_status_get
              timer_start
              timer_stop
              timer_sys_ctrl_set
              timer_value_get
              timer_is_running
  �޸���ʷ   :
  1.��    ��   : 2013��3��30��
    ��    ��   : ��ϲ 220237
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "soc_baseaddr_interface.h"
#include "soc_timer_interface.h"
#include "DrvInterface.h"
#include "bsp_timer_phy.h"
#include <linux/spinlock.h>
#include "bsp_ao_sctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 ȫ�ֱ������� 
*****************************************************************************/
 /*��timer���ƼĴ�����ʹ��λ��д���Ᵽ��*/
extern spinlock_t timer_en_lock;

/* ���ֵ�A�˵�����Timer�Ļ���ַ */
BSP_U32 pulTimerBase[TIMER_TOTAL_NUM + CCORE_TIMER_EXP] = {0};

#ifdef _DRV_LLT_
SOC_TIMER_TIMERN_CONTROL_UNION   g_SOC_TIMER_TIMERN_CONTROL_UNION;
SOC_TIMER_TIMERN_INTCLR_UNION   g_SOC_TIMER_TIMERN_INTCLR_UNION;
SOC_TIMER_TIMERN_RIS_UNION   g_SOC_TIMER_TIMERN_RIS_UNION;
SOC_TIMER_TIMERN_MIS_UNION   g_SOC_TIMER_TIMERN_MIS_UNION;
BSP_U32 g_SCtrl_TIMER_EN1_REG;
BSP_U32 g_SCtrl_TIMER_EN0_REG;
#define SOC_SCTRL_SC_TIMER_EN1_ADDR(base)  &g_SCtrl_TIMER_EN1_REG
#define SOC_SCTRL_SC_TIMER_EN0_ADDR(base)  &g_SCtrl_TIMER_EN0_REG
#define TIMER_MIS_REG(base)    (&g_SOC_TIMER_TIMERN_MIS_UNION)
#define TIMER_RIS_REG(base)    (&g_SOC_TIMER_TIMERN_RIS_UNION)
#define TIMER_INTC_REG(base)    (&g_SOC_TIMER_TIMERN_INTCLR_UNION)
#endif



/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : timer_find_phy_id
 ��������  : ���ݴ�����߼�timer ID, ת��Ϊ����timer ID, ������IDΪ�ú��ܷ��ʵ���Чtimer ID
 �������  : ucTimerId        Timer ID
 �������  : ��
 �� �� ֵ  :   �ɹ�:������Ӧ������timerID
                          ʧ��:����-1
 ���ú���  :
 ��������  :

*****************************************************************************/
BSP_S32 timer_find_phy_id (BSP_U8 timerID)
{
    BSP_S32 ret = 0;
    switch (timerID)
    {
        case TIMER_ACPU_SYSTEM_ID:
            ret = TIMER_ACPU_SYSTEM_ID_PHY;
            break;
        case TIMER_ACPU_FREE_RUN:
            ret = TIMER_ACPU_FREE_RUN_PHY;
            break;
        case TIMER_ACPU_OSA_ID:
            ret = TIMER_ACPU_OSA_ID_PHY;
            break;
        case TIMER_ACPU_SOFTTIMER_ID:
            ret = TIMER_ACPU_SOFTTIMER_ID_PHY;
            break;

        case TIMER_ACPU_IDLE_ID:
            ret = TIMER_ACPU_IDLE_ID_PHY;
            break;
        case TIMER_ACPU_OM_TCXO_ID:
            ret = TIMER_ACPU_OM_TCXO_ID_PHY;
            break;
        /* ĿǰֻΪTIMER_CCPU_CPUVIEW_ID��Ȩ�ޣ�ʹ����Զ�ȡc��timer17��valueֵ */
        case TIMER_CCPU_CPUVIEW_ID:
            ret = TIMER_TOTAL_NUM + 0;      
            break;
            
        default:
            ret = -1;    
            break;
    }
    return ret;
}
BSP_VOID timer_load_set(BSP_U8 ucTimerId, BSP_U32 ulValue)
{
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
        (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[ucTimerId]);

    /* ��cntl�Ĵ���Ϊ32λ����ģʽ��Ĭ��Ϊ16λ���� */
    pTIMER_CNTL_REG->reg.timersize = SET_BIT_WIHT_TRUE;

    /* ��ulValueд��load�Ĵ��� */
    TIMER_REG_WRITE(TIMER_LOAD_REG(pulTimerBase[ucTimerId]), ulValue);
}


BSP_U32 timer_load_get(BSP_U8 timerId)
{
    BSP_U32 result;
    TIMER_REG_READ(TIMER_LOAD_REG(pulTimerBase[timerId]), result);

    return result;
}


BSP_U32 timer_value_get(BSP_U8 timerId)
{
    BSP_U32 result;
    TIMER_REG_READ(TIMER_VALUE_REG(pulTimerBase[timerId]), result);

    return result;
}


BSP_S32 timer_is_running(BSP_U8 timerId)
{
    /* ���ݶ�ʱ��timerId����ȡ�Ĵ���TIMERN_CONTROL[7] */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG = (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);
    if (SET_BIT_WIHT_TRUE == pTIMER_CNTL_REG->reg.timeren)
    {
        return BSP_OK;
    }
     
    return BSP_ERROR;
}
BSP_VOID timer_start(BSP_U8 timerId, BSP_U32 mode)
{
    unsigned long flags;
    unsigned long base = IO_ADDRESS(REG_BASE_SC_ON);
    BSP_U8 dualTimerId = (BSP_U8)timerId/2;

    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[7]�����ö�ʱ��ֹͣ */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
        (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);

    /*��timer���ƼĴ�����ʹ��λ��д���Ᵽ��*/
    spin_lock_irqsave(&timer_en_lock, flags);
    
    /*ʹ��dualt-timer�����ߺ͹���ʱ��*/
    /*SC_PERIPH_CLKEN4 д1��Ч��д0��Ч*/
    TIMER_REG_WRITE(SOC_AO_SCTRL_SC_PERIPH_CLKEN4_ADDR(base), SET_BITS_WIHT_TRUETRUE<<(2*dualTimerId+1));

    pTIMER_CNTL_REG->reg.timeren = SET_BIT_WIHT_FALSE;

    if (mode)
    {
        /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[0~1]��TIMERN_CONTROL[5,6,7]��
           ���ö�ʱ��Ϊ32bit����������ģʽ��ʹ�� */
        pTIMER_CNTL_REG->reg.oneshot = SET_BIT_WIHT_FALSE;
        pTIMER_CNTL_REG->reg.timersize = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.intenable = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.timermode = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.timeren = SET_BIT_WIHT_TRUE;
    }
    else
    {
        /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[0~1]��TIMERN_CONTROL[5,7]��
           ���ö�ʱ��Ϊ32bit����������ģʽ��ʹ�� */
        pTIMER_CNTL_REG->reg.oneshot = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.timersize = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.intenable = SET_BIT_WIHT_TRUE;
        pTIMER_CNTL_REG->reg.timeren = SET_BIT_WIHT_TRUE;
    }

    spin_unlock_irqrestore(&timer_en_lock, flags);
}


BSP_VOID timer_stop(BSP_U8 timerId)
{
    unsigned long flags;

   

    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[7]�����ö�ʱ��ֹͣ */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
          (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);

    /*����ͬdual-timer����һ��timerId*/
    BSP_U8 another_timerId = (timerId % 2) ? (timerId-1):(timerId+1);

    /*��timer���ƼĴ�����ʹ��λ��д���Ᵽ��*/
    spin_lock_irqsave(&timer_en_lock, flags);

    pTIMER_CNTL_REG->reg.timeren = SET_BIT_WIHT_FALSE;

    /*�ж�ͬdualt-timer����һ��timer��ʹ��״̬*/
    if (BSP_OK != timer_is_running(another_timerId))
    {
        /*���δʹ�ܣ��رմ�dual-timer�����ߺ͹���ʱ��*/
        /*TIMER_REG_WRITE(SOC_AO_SCTRL_SC_PERIPH_CLKDIS4_ADDR(base), SET_BITS_WIHT_TRUETRUE<<(2*dualTimerId+1));*/
    }

    spin_unlock_irqrestore(&timer_en_lock, flags);
}


BSP_VOID timer_int_mask(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[5]������Ϊ�ж�����״̬ */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
        (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);
    pTIMER_CNTL_REG->reg.intenable = SET_BIT_WIHT_FALSE;
}
BSP_VOID timer_int_unmask(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[5]������Ϊ�жϽ�����״̬ */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
        (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);
    pTIMER_CNTL_REG->reg.intenable = SET_BIT_WIHT_TRUE;
}
BSP_VOID timer_int_clear(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_INTCLR[0]����ж� */
    SOC_TIMER_TIMERN_INTCLR_UNION *pTIMER_INTC_REG =
        (SOC_TIMER_TIMERN_INTCLR_UNION *)TIMER_INTC_REG(pulTimerBase[timerId]);
    pTIMER_INTC_REG->reg.timern_intclr = SET_BIT_WIHT_TRUE;
}
BSP_U32 timer_int_status_get(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId�����Ĵ���TIMERN_MIS[0]�������ظ�ֵ */
    SOC_TIMER_TIMERN_MIS_UNION *pTIMER_MIS_REG =
        (SOC_TIMER_TIMERN_MIS_UNION *)TIMER_MIS_REG(pulTimerBase[timerId]);

    return(pTIMER_MIS_REG->reg.timernmis);
}
BSP_U32 timer_raw_int_status_get(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId�����Ĵ���TIMERN_RIS[0]�������ظ�ֵ */
    SOC_TIMER_TIMERN_RIS_UNION *pTIMER_RIS_REG =
        (SOC_TIMER_TIMERN_RIS_UNION *)TIMER_RIS_REG(pulTimerBase[timerId]);

    return(pTIMER_RIS_REG->reg.timernris);
}
BSP_VOID  timer_32bit_count_set(BSP_U8 timerId)
{
    /* �Զ�ʱ��timerId��д�Ĵ���TIMERN_CONTROL[1]������Ϊ32-bitģʽ */
    SOC_TIMER_TIMERN_CONTROL_UNION *pTIMER_CNTL_REG =
        (SOC_TIMER_TIMERN_CONTROL_UNION *)TIMER_CNTL_REG(pulTimerBase[timerId]);
    pTIMER_CNTL_REG->reg.timersize = SET_BIT_WIHT_TRUE;
}
BSP_U32 timer_input_clk_get(BSP_U8 timerId)
{
    switch (timerId)
    {
        /* Timerʱ��Ƶ��Ϊ32KHz */
        case TIMER_ACPU_OSA_ID_PHY:
        case TIMER_ACPU_SOFTTIMER_ID_PHY:
        case TIMER_ACPU_IDLE_ID_PHY:
        case TIMER_ACPU_OM_TCXO_ID_PHY:
            return SLEEP_CLK_FREQ;

        /* Timerʱ��Ƶ��Ϊ19.2MHz */
        case TIMER_ACPU_SYSTEM_ID_PHY:
        case TIMER_ACPU_FREE_RUN_PHY:
            return TCXO_CLK_FREQ;

        default:
            return BSP_FALSE;
    }
}



BSP_S32 timer_sys_ctrl_set(void)
{
    BSP_U32 *pSCtrl_TIMER_EN0_REG = (BSP_U32 *)(IO_ADDRESS(AOSCTRL_SC_TIMER_EN0));
    BSP_U32 *pSCtrl_TIMER_EN1_REG = (BSP_U32 *)(IO_ADDRESS(AOSCTRL_SC_TIMER_EN1));
    SOC_AO_SCTRL_SC_TIMER_EN0_UNION SCtrl_TIMER_EN0_REG;
    SOC_AO_SCTRL_SC_TIMER_EN1_UNION SCtrl_TIMER_EN1_REG;

    /* ����timer2,3 */
    SCtrl_TIMER_EN0_REG.value = 0;
    SCtrl_TIMER_EN0_REG.reg.timer_en0_5acpu_dualtimer1_en0 = TIMER_32K;
    SCtrl_TIMER_EN0_REG.reg.timer_en0_msk_5acpu_dualtimer1_en0 = SET_BITS_WIHT_TRUETRUE;
    
    SCtrl_TIMER_EN0_REG.reg.timer_en0_7acpu_dualtimer1_en1 = TIMER_32K;
    SCtrl_TIMER_EN0_REG.reg.timer_en0_msk_7acpu_dualtimer1_en1 = SET_BITS_WIHT_TRUETRUE;

    *pSCtrl_TIMER_EN0_REG = SCtrl_TIMER_EN0_REG.value;
   

    SCtrl_TIMER_EN1_REG.value = 0;
    SCtrl_TIMER_EN1_REG.reg.timer_en1_9acpu_dualtimer6_en0 = TIMER_32K;
    SCtrl_TIMER_EN1_REG.reg.timer_en1_msk_9acpu_dualtimer6_en0 = SET_BITS_WIHT_TRUETRUE;

    SCtrl_TIMER_EN1_REG.reg.timer_en1_11acpu_dualtimer6_en1 = TIMER_32K;
    SCtrl_TIMER_EN1_REG.reg.timer_en1_msk_11acpu_dualtimer6_en1 = SET_BITS_WIHT_TRUETRUE;

    *pSCtrl_TIMER_EN1_REG = SCtrl_TIMER_EN1_REG.value;

    return BSP_OK;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

