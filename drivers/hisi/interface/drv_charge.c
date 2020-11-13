/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_charge.c
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
#include "sys_config.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_CHG_GET_BATTERY_STATE
 ��������  : ��ȡ�ײ���״̬��Ϣ
 �������  : battery_state ������Ϣ
 �������  : battery_state ������Ϣ
 ����ֵ��    0 �����ɹ�
             -1����ʧ��
*****************************************************************************/
int DRV_CHG_GET_BATTERY_STATE(BATT_STATE_T *bttery_state)
{
    return BSP_ERROR;
#if 0    
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
#if( FEATURE_CHARGE == FEATURE_ON )
    BSP_CHG_GetBatteryState(bttery_state);
#else
    DRV_PRINTF("Function DRV_CHG_GET_BATTERY_STATE unsupport!\n");
    return -1;
#endif
#endif
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_CHG_GetCbcState
 ��������  : ���ص��״̬�͵���
 �������  : pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
             pucBcl  0:���������û����������
 �������  : pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
             pucBcl  0:���������û����������
 ����ֵ��    0 �����ɹ�
             -1����ʧ��
*****************************************************************************/
int DRV_CHG_GET_CBC_STATE(unsigned char *pusBcs,unsigned char *pucBcl)
{
    return BSP_ERROR;
#if 0    
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
#if( FEATURE_CHARGE == FEATURE_ON )
    BSP_CHG_GetCbcState(pusBcs, pucBcl);
#else
    DRV_PRINTF("Function DRV_CHG_GET_CBC_STATE unsupport!\n");
    return -1;
#endif
#endif
#endif
}
/*****************************CHARGE END************************************/

/*****************************************************************************
 �� �� ��  : BSP_CHG_ChargingStatus
 ��������  :��ѯĿǰ�Ƿ����ڳ����
 �������  :��
 �������  :��
 ����ֵ��   0 δ���
            1 �����
*****************************************************************************/
int DRV_CHG_GET_CHARGING_STATUS(void)
{
    return 0;
#if 0    
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
#if( FEATURE_CHARGE == FEATURE_ON )
    return BSP_CHG_ChargingStatus();
#else
    DRV_PRINTF("Function DRV_CHG_GET_CHARGING_STATUS unsupport!\n");
    return 0;
#endif
#endif
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_CHG_STATE_SET
 ��������  : ʹ�ܻ��߽�ֹ���
 �������  : ulState    0:��ֹ���
 						1:ʹ�ܳ��
 �������  : ��
 ����ֵ��    ��
*****************************************************************************/
BSP_VOID  DRV_CHG_STATE_SET(unsigned long ulState)
{
    return;
#if 0    
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
#if( FEATURE_CHARGE == FEATURE_ON )
    BSP_CHG_StateSet(ulState);
#else
    DRV_PRINTF("Function DRV_CHG_STATE_SET unsupport!\n");
#endif
#endif
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_CHG_Sply
 ��������  : chg_state_set(UINT32 ulstate)��A�˵�ӳ�亯��,��ѯ�Ƿ���Ҫ����
 �������  :
 �������  :
 ����ֵ��
*****************************************************************************/
BSP_S32 BSP_CHG_Sply( void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    return BSP_ERROR;
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_CHG_StateGet
 ��������  : chg_state_set(UINT32 ulstate)��A�˵�ӳ�亯��,��ѯ�Ƿ���Ҫ����
 �������  :
 �������  :
 ����ֵ��
*****************************************************************************/
BSP_S32 BSP_CHG_StateGet(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    return BSP_ERROR;
#endif
}

/**********************************************************************
�� �� ��  : BSP_ONOFF_StartupModeGet
��������  :  A���жϿ��ػ�ģʽ
�������  : ��
�������  : ��
�� �� ֵ      :
ע������  : ��
***********************************************************************/
ENUM_POWER_ON_MODE BSP_ONOFF_StartupModeGet( void )
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (ENUM_POWER_ON_MODE)0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    return POWER_ON_MAX;
#endif
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

