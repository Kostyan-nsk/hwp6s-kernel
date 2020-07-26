/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_hkadc.c
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


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern int printf(const char *format, ...);

/*****************************************************************************
  2 ��������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_GET_BATTERY_ADC
 ��������  : ��ȡ��ص�ѹ����ֵ
 �������  : ��
 �������  : pslData�� ָ���ص�ѹ��ָ�롣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_GET_BATTERY_ADC(BSP_S32 * pslData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#if( FEATURE_CHARGE == FEATURE_ON )
    return BSP_HKADC_BatADCRead(pslData);
#else
    DRV_PRINTF("Function DRV_GET_BATTERY_ADC unsupport!\n");
    return BSP_ERROR;
#endif
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*************************************************
 �� �� ��   : DRV_HKADC_BAT_VOLT_GET
 ��������   : ���ص�ǰ��ص�ѹֵ
 �������   : pslData : ��ص�ѹֵ
 �������   : pslData : ��ص�ѹֵ
 �� �� ֵ   :0:��ȡ�ɹ�
            -1:��ȡʧ��
*************************************************/
BSP_S32 DRV_HKADC_BAT_VOLT_GET(BSP_S32 *ps32Data)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#if( FEATURE_CHARGE == FEATURE_ON )
    return BSP_HKADC_BatVoltGet(ps32Data);
#else
    DRV_PRINTF("Function DRV_HKADC_BAT_VOLT_GET unsupport!\n");
    return BSP_ERROR;
#endif
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

