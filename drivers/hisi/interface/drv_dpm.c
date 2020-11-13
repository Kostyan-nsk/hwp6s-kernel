/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_dpm.c
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
#if (defined BSP_CORE_APP)
#include <linux/module.h>
#endif

#include "BSP.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern void BSP_PWRCTRL_GetCcpuLoadCB(PWRCTRLFUNCPTR pFunc );
extern int printf(const char *format, ...);

/*****************************************************************************
  2 ��������
*****************************************************************************/

#define printk(a)

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
Function:   DRV_PWRCTRL_SLEEP_IN_CB
Description:
Input:
Output:     None;
Return:
Others:
*****************************************************************************/
int DRV_PWRCTRL_SLEEP_IN_CB(PWC_DS_SOCP_CB_STRU stFunc)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_POWER_OFF
 ��������  : ����ֱ���µ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void DRV_POWER_OFF(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#if( FEATURE_POWER_ON_OFF == FEATURE_ON )
    BSP_ONOFF_DrvPowerOff();
#else
    DRV_PRINTF("Function DRV_POWER_OFF unsupport!\n");
#endif
#endif

#if (defined BSP_CORE_APP)
    return;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEP_CALLBACK_REG
 ��������  : ע��PWC����������ʹ��
 �������  : ��ע��ĺ���
 �������  : None
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
int DRV_PWRCTRL_SLEEP_CALLBACK_REG(PWC_SLEEP_CALLBACK_STRU pCallback)
{
    return 0;   /* ��׮ */
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC
 ��������  : arm���µ繦�ܣ���Ҫ����ahb���ݣ��ýӿ��������ݱ����ַ�ͳ���
 �������  : length     ��������ռ��С��ָ��
 �������  : ��
 �� �� ֵ  : ����ռ�ĵ�ַ ��
*****************************************************************************/
void * DRV_PWRCTRL_AHB_BAK_ADDR_ALLOC(unsigned int * length)
{
    return (void *)0;
}

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_RLS_PRF_CCPU
Description:Release the Vote Result
Input:      ulClientID: The Vote Module Client
Output:     None;
Return:     The Vote Result
Others:
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_DFS_RLS_PRF_CCPU(PWC_DFS_ID_E ulClientID)
{
    return 0;
}

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_SET_PRF_CCPU
Description:Set the System Min Profile
Input:      ulClientID: The Vote Module Client
            enProfile:  The Min Profile Value
Output:     None
Return:     The Vote Result
Others:
*****************************************************************************/
BSP_U32  DRV_PWRCTRL_DFS_SET_PRF_CCPU(PWC_DFS_ID_E ulClientID, PWC_DFS_PROFILE_E enProfile)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GET_TEMP
 ��������  : ��ȡOLED����ء�GPA��WPA��SIM����DCXO���¶ȡ�
 �������  : enModule:PWC_TEMP_PROTECT_E
             hkAdcTable:��ѹ�¶Ȼ����
 �������  : pslData:�洢�ĵ�ѹת��Ϊ���¶�ֵ
 �� �� ֵ  : HKADC_OK/HKADC_ERROR ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_GET_TEMP(PWC_TEMP_PROTECT_E enModule, BSP_U16 *hkAdcTable,BSP_S32 *pslData)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCcpuLoadCB
 ��������  : arm �ṩ��TTF�Ļص�����
 �������  : pFunc:TTF����ָ��
 �������  : ��
 �� �� ֵ  : ����ռ�ĵ�ַ ��
*****************************************************************************/
void DRV_PWRCTRL_GetCcpuLoadCB(PWRCTRLFUNCPTR pFunc )
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    BSP_PWRCTRL_GetCcpuLoadCB(pFunc);
#endif

#if (defined BSP_CORE_APP)
    ;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCpuLoad
 ��������  : arm cpuռ���ʲ�ѯ����
 �������  : ��
 �������  : ulAcpuLoad:Acpuռ���ʵ�ַ.
             ulCcpuLoad:Ccpuռ���ʵ�ַ.
 �� �� ֵ  : 0/1 ��
*****************************************************************************/
unsigned int DRV_PWRCTRL_GetCpuLoad(unsigned int *ulAcpuLoad,unsigned int *ulCcpuLoad)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (unsigned int)BSP_PWRCTRL_GetCpuLoad(ulAcpuLoad,ulCcpuLoad);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PLL_ENABLE
 ��������  : ͨ��ģ�����ʱ��ʹ�ܽӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PLLENABLE (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PllEnable (enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PLL_DISABLE
 ��������  : ͨ��ģ�����ʱ�ӹرսӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PLLDISABLE (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PllDisable (enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : BSP_PWRCTRL_PllStatusGet
 ��������  : ͨ��ģ�����ʱ��״̬��ѯ�ӿ�
 �������  : enCommMode     ͨ��ģʽ
             enCommModule   ͨ��ģ��
             enCommCard    ʱ ����
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         ʱ�Ӵ�
             PWRCTRL_COMM_OFF        ʱ�ӹر�
             RET_ERR                 ��ѯʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PLLSTATUSGET (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PllStatusGet(enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRUP
 ��������  : ͨ��ģ����Ƶ�Դ�ϵ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PWRUP (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PwrUp (enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRDOWN
 ��������  : ͨ��ģ����Ƶ�Դ�µ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PWRDOWN (PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PwrDown (enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : BSP_PWRCTRL_PwrStatusGet
 ��������  : ͨ��ģ����Ƶ�Դ״̬��ѯ�ӿ�
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON         �ϵ�
             PWRCTRL_COMM_OFF        �µ�
             RET_ERR                 ״̬��ѯʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_PWRCTRL_PwrStatusGet(enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif

}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEPVOTE_LOCK
 ��������  : �����ֹ˯��ͶƱ�ӿڡ�
 �������  : enClientId:PWC_CLIENT_ID_E
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_SLEEPVOTE_LOCK(PWC_CLIENT_ID_E enClientId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (BSP_U32)BSP_PWRCTRL_SleepVoteLock(enClientId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_SLEEPVOTE_UNLOCK
 ��������  : ��������˯��ͶƱ�ӿڡ�
 �������  : enClientId:PWC_CLIENT_ID_E
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_SUCCESS
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWC_CLIENT_ID_E enClientId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (BSP_U32)BSP_PWRCTRL_SleepVoteUnLock(enClientId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKENABLE
 ��������  : ����ͨ��ģ��ʱ��
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return PWRCTRL_BbpClkEnable(enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKDISABLE
 ��������  : �ر�ͨ��ģ��ʱ��
 �������  : enModeId     ͨ��ģʽ
             enModuleId   ͨ��ģ��
             enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
             RET_ERR        ʧ��
*****************************************************************************/
BSP_S32 DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return PWRCTRL_BbpClkDisable(enModeId, enModuleId, enModemId);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}


#define __DRV_STUB_DEF__

unsigned int BSP_PWRCTRL_StandbyStateCcpu(unsigned int ulStandbyTime, unsigned int ulSwitchtime)
{
    return 0; /*nas stub*/
}

void PWRCTRL_AcpuWakeUpInt(unsigned int type)
{
    return ;  /*drv ipc ģ���׮*/
}

int BSP_Modem_OS_Status_Switch(int enable)
{
    /*stub interface*/
    return 0;
}

BSP_VOID BSP_PWC_MspVoteRegister(FUNCPTR routine)
{
    /*stub interface*/
    return;
}

VOID BSP_PWC_SetTimer4WakeSrc(VOID)
{
    /*stub interface*/
    return;
}

VOID BSP_PWC_DelTimer4WakeSrc(VOID)
{
    /*stub interface*/
    return;
}

/*****************************************************************************
 Prototype      : BSP_PWRCTRL_StandbyStateAcpu
 Description    : AT^PSTANDBY
 Input          :
 Output         :
 Return Value   :
 Calls          :
 Called By
*****************************************************************************/
unsigned int BSP_PWRCTRL_StandbyStateAcpu(unsigned int ulStandbyTime, unsigned int ulSwitchtime)
{
    return 0;   /* ��׮ */
}


#if (defined BSP_CORE_APP)
EXPORT_SYMBOL(BSP_Modem_OS_Status_Switch);
EXPORT_SYMBOL(DRV_PWRCTRL_SLEEPVOTE_LOCK);    /* nas acpu    ��Ҫ��׮*/
EXPORT_SYMBOL(DRV_PWRCTRL_SLEEPVOTE_UNLOCK);  /* nas acpu    ��Ҫ��׮*/
#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

