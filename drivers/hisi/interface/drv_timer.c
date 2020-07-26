/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_timer.c
  �� �� ��   : ����
  ��    ��   : c61362
  ��������   : 2012��3��2��
  ����޸�   :
  ��������   : ������ϲ������װ�Ľӿڲ�
  �޸���ʷ   :
  1.��    ��   : 2012��3��2��
    ��    ��   : c61362
    �޸�����   : �½�Drvinterface.c
    
  2.��    ��   : 2013��2��19��
    ��    ��   : ��ϲ 220237
    �޸�����   : ��Drvinterface.c�������

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "BSP.h"
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#include <vxWorks.h>
#include <private/vmLibP.h>
#include <cacheLib.h>
#include "arm_pbxa9.h"
#include <cacheLib.h>
#include "BSP_TIMER.h"
#include "sys_config.h"
#include "soc_baseaddr_interface.h"
#include "string.h"
#include "stdio.h"
#endif

#if (defined BSP_CORE_APP)
#include "bsp_timer_if.h"
#endif

#include "soc_baseaddr_interface.h"
#include "soc_timer_interface.h"
#include "soc_sctrl_interface.h"
#include <soc_ao_sctrl_interface.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern BSP_U32 BSP_BBP_GetCurTime(BSP_U64 *pCurTime);

/*****************************************************************************
  2 ��������
*****************************************************************************/
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern BSP_S32 BSP_TIMER_GetRestTime(BSP_U32 usrClkId, BSP_U32 unitType, BSP_U32 * pRestTime);
extern BSP_S32 BSP_TIMER_UsrClkStart
(
    BSP_U32     usrClkId,
    FUNCPTR_1   routine,
    BSP_S32     arg,
    BSP_U32     timerValue,
    BSP_U32     mode,
    BSP_U32     unitType
);
extern BSP_S32 BSP_TIMER_UsrClkDisable(BSP_U32 usrClkId);
#endif

#if (defined BSP_CORE_APP)
extern int start_om_hard_timer(unsigned int value);
extern int stop_om_hard_timer(void);
extern int clear_om_hard_timer_int(void);
extern unsigned int GetHardTimerCurTime(void);
extern BSP_U32 BBPGetCurTime(BSP_U64 *pCurTime);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/************************************************************************
 * FUNCTION
 *       BSP_StartHardTimer
 * DESCRIPTION
 *       start hard timer
 * INPUTS
 *       value -- timer's value.uint is 32K cycle
 * OUTPUTS
 *       NONE
 *************************************************************************/
BSP_VOID BSP_StartHardTimer(BSP_U32 value)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return;/*delete by z00212992*/
#if 0 
    BSP_REG_WRITE((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_CTRL, CLK_DEF_TC_DISABLE);
    BSP_REG_WRITE((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_LOAD, value);
    BSP_REG_WRITE((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_CTRL, CLK_DEF_TC_ENABLE);
#endif
#endif

#if (defined BSP_CORE_APP)
    start_om_hard_timer(value);
#endif
}

/************************************************************************
 * FUNCTION
 *       BSP_StopHardTimer
 * DESCRIPTION
 *       Stop hard timer
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
BSP_VOID BSP_StopHardTimer(BSP_VOID)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return;/*delete by z00212992*/
#if 0    
    BSP_REG_WRITE((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_CTRL, CLK_DEF_TC_DISABLE);
#endif
#endif

#if (defined BSP_CORE_APP)
    stop_om_hard_timer();
#endif
}

/************************************************************************
 * FUNCTION
 *       BSP_GetHardTimerCurTime
 * DESCRIPTION
 *       Get hard timer Current Value
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
BSP_U32 BSP_GetHardTimerCurTime(BSP_VOID)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;/*delete by z00212992*/
#if 0    
    BSP_U32 readValue;
    BSP_REG_READ((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_CTRL, readValue);

    if(CLK_DEF_TC_DISABLE == readValue)
    {
        return 0;
    }

    BSP_REG_READ((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_VALUE, readValue);

    return readValue;
#endif    
#endif

#if (defined BSP_CORE_APP)
    return GetHardTimerCurTime();
#endif
}

/************************************************************************
 * FUNCTION
 *       BSP_ClearTimerINT
 * DESCRIPTION
 *       
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
BSP_VOID BSP_ClearTimerINT(BSP_VOID)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return;/*delete by z00212992*/
#if 0    
    BSP_U32 ulValue;    
    BSP_REG_READ((RTC_TIMER_REGBASE_ADDR), CLK_REGOFF_CLEAR, ulValue);
    
    /*for pclint*/
    ulValue = ulValue;

    return ;
#endif    
#endif

#if (defined BSP_CORE_APP)
    clear_om_hard_timer_int();
#endif
}

extern unsigned int omTimerGet(void);
/********************************************************************************************************************
 �� �� ��  : BSP_GetSliceValue
 ��������  : ��ȡʱ���Timer�ĵ�ǰֵ
 �������  :
 �� �� ֵ  :
********************************************************************************************************************/
unsigned int BSP_GetSliceValue(void)
{
    return omTimerGet();
}

extern BSP_S32 BSP_TIMER_GetRestTime(BSP_U32 clk_logic_id, BSP_U32 unitType, BSP_U32 * pRestTime);

int DRV_TIMER_GET_REST_TIME(unsigned int usrClkId, unsigned int unitType, unsigned int * pRestTime)
{    
    return BSP_TIMER_GetRestTime(usrClkId, unitType, pRestTime); 
}

extern BSP_S32 BSP_TIMER_UsrClkStart
(
    BSP_U32     clk_logic_id,
    FUNCPTR_1   routine,
    BSP_S32     arg,
    BSP_U32     timerValue,
    BSP_U32     mode,
    BSP_U32     unitType
);

int DRV_TIMER_START
(
    unsigned int     usrClkId,
    FUNCPTR_1        routine,
    int              arg,
    unsigned int     timerValue,
    unsigned int     mode,
    unsigned int     unitType
)
{ 
    return BSP_TIMER_UsrClkStart(usrClkId, routine, arg, timerValue, mode, unitType);
}

extern BSP_S32 BSP_TIMER_UsrClkDisable(BSP_U32 clk_logic_id);

int DRV_TIMER_STOP(unsigned int usrClkId)
{
    return BSP_TIMER_UsrClkDisable(usrClkId);
}

/*****************************************************************************
* �� �� ��  : BSP_PWC_SetDrxTimerWakeSrc
* ��������  : ����DRX timer ��Ϊ����Դ
* �������  :
* �������  :
* �� �� ֵ   :
* �޸ļ�¼  : ��v7r1��ֲ�������ڲ��̶�ʹ��TIMER_CCPU_DRX_TIMER_ID
*****************************************************************************/
void BSP_PWC_SetDrxTimerWakeSrc(void)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    BSP_TIMER_SetDrxTimerWakeSrc();
#endif

#if (defined BSP_CORE_APP)
    return;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_PWC_DelDrxTimerWakeSrc
* ��������  : ����DRX timer ����Ϊ����Դ
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  : ��v7r1��ֲ�������ڲ��̶�ʹ��TIMER_CCPU_DRX_TIMER_ID
*****************************************************************************/
void BSP_PWC_DelDrxTimerWakeSrc(void)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    BSP_TIMER_DelDrxTimerWakeSrc();
#endif

#if (defined BSP_CORE_APP)
    return;
#endif
}

extern unsigned int BSP_GET_RTC_VALUE(void);
/*****************************************************************************
 �� �� ��  : DRV_GET_RTC_VALUE
 ��������  : ����rtc�Ĵ�������õ�ǰRTC��ֵ
 �������  : ��
 �������  : ��
 ����ֵ    ��RTCֵ����λs
 �޸�˵��  :
             V9sft A��C�˹��ʵ��(A����mach_info.c�ļ��У�C����BSP_TIMER.c�ļ���)
             V3sft A��C�˴�׮
*****************************************************************************/
unsigned int DRV_GET_RTC_VALUE(void)
{
    return BSP_GET_RTC_VALUE();
}

/*****************************************************************************
* �� �� ��  : BSP_USRCLK_Alloc
*
* ��������  : �����û�ʱ��ID
*
* �������  : BSP_S32 * ps32UsrClkId
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
BSP_U32 BSP_USRCLK_Alloc(BSP_S32 * ps32UsrClkId)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : BSP_BBPGetCurTime
*
* ��������  : ��PS���ã�������ȡϵͳ��ȷʱ��
*
* �������  : ��
* �������  : BSP_U32 u32CurTime:��ǰʱ��
*
* �� �� ֵ  : BSP_OK : ��ȡ�ɹ�
*                        BSP_ERR_INVALID_PARA  :��ȡʧ��
*
* �޸ļ�¼  : 2011��3��7��  wangjing  creat
*****************************************************************************/
BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)    
    return BSP_BBP_GetCurTime(pCurTime);
#endif

#if (defined BSP_CORE_APP)
    return BBPGetCurTime(pCurTime);
#endif
}

BSP_S32 BSP_TIMER_UsrClkEnable(BSP_U32 usrClkId, BSP_U32 value, BSP_U32 mode)
{
    return 0;
}

BSP_S32 BSP_USRCLK_Connect(FUNCPTR routine, BSP_S32 arg, BSP_S32 s32UsrClkid)
{
    return 0;
}   

BSP_S32 BSP_USRCLK_Disable (BSP_S32 s32UsrClkid)
{
    return 0;
}
 
BSP_S32 BSP_USRCLK_Enable (BSP_S32 s32UsrClkid)
{
    return 0;
}

BSP_S32 BSP_USRCLK_RateSet(BSP_S32 ticksPerSecond, BSP_S32 s32UsrClkid)
{
    return 0;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

