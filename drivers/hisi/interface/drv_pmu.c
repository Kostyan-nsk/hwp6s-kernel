/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_pmu.c
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

#ifndef _DRV_LLT_
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#include "BSP_PMU_INTERFACE.h"
#endif
#endif
/*****************************************************************************
  2 ��������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : BSP_PMU_GetPMUState
 ��������  : ��ȡPMUģ�鿪����ʽ�����״̬����ص����������λ״̬��
 �������  : �ޡ�
 �������  : Pmu_State :������ʽ�����״̬����ص����������λ״̬��
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ע��������ýӿڽ���PS��PC������ʹ�ã�Ŀǰû��Ӧ�ã��ݱ�����
*****************************************************************************/
unsigned int DRV_GET_PMU_STATE(void*  Pmu_State)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (unsigned int)(-1);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

#define printk(a)


BSP_S32 BSP_PMU_AptDisable(BSP_VOID)
{
    printk("stub: BSP_PMU_AptEnable\n");
	return 0;
}


BSP_S32 BSP_PMU_AptEnable(BSP_VOID)
{
    printk("stub  monan :BSP_PMU_AptEnable. \n");
    return 0;
}


BSP_BOOL BSP_PMU_AptIsEnable(BSP_VOID)
{
    printk("stub: BSP_PMU_AptIsEnable\n");
    return 0;
}

int HI6451_BoardInit(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    printk("stub  monan :HI6451_BoardInit. \n");
    return 0;
#endif
}

#ifndef _DRV_LLT_
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
 /*****************************************************************************
 �� �� ��  : DRV_PASTAR_EXC_CHECK
 ��������  : ͨ��ģ����PASTAR�Ƿ����쳣�ӿ�
 �������  : modem_id       ����
 �������  : ��
 �� �� ֵ  : BSP_OK          û���쳣
             BSP_ERROR       �����쳣
*****************************************************************************/
BSP_S32 DRV_PASTAR_EXC_CHECK(PWC_COMM_MODEM_E modem_id)
{
    return bsp_pastar_exc_check(modem_id);
}

/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_SET
 ��������  : ͨ��ģ�����õ�ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ���õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : BSP_OK          ���óɹ�
             BSP_ERROR       ����ʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_VOLTAGE_SET( EM_MODEM_CONSUMER_ID consumer_id, BSP_U32 voltage_mv )
{
    return bsp_modem_voltage_set(consumer_id, voltage_mv);
}
/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_GET
 ��������  : ͨ��ģ���ȡ��ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ��õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : BSP_OK          ��ȡ�ɹ�
             BSP_ERROR       ��ȡʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_VOLTAGE_GET( EM_MODEM_CONSUMER_ID consumer_id, BSP_U32 *voltage_mv )
{
    return bsp_modem_voltage_get(consumer_id, voltage_mv);
}
/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_LIST
 ��������  : ͨ��ģ���ȡ��ѹ���÷�Χ�ӿ�
 �������  : consumer_id     �û�id
             list            ��ѹ��Χ����
             size            �����С
 �������  : ��
 �� �� ֵ  : BSP_OK          ��ȡ�ɹ�
             BSP_ERROR       ��ȡʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_VOLTAGE_LIST(EM_MODEM_CONSUMER_ID consumer_id,BSP_U16 **list, BSP_U32 *size)
{
    return bsp_modem_voltage_list(consumer_id, list, size);
}
/*****************************************************************************
 �� �� ��  : DRV_MODEM_APT_ENABLE
 ��������  : ͨ��ģ��ʹ��APT�ӿ�
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : BSP_OK         ���óɹ�
             BSP_ERROR      ����ʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_APT_ENABLE(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return bsp_modem_apt_enable(modem_id, mode_id);
}
/*****************************************************************************
 �� �� ��  : DRV_MODEM_APT_DISABLE
 ��������  : ͨ��ģ��ȥʹ��APT�ӿ�
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : BSP_OK         ���óɹ�
             BSP_ERROR      ����ʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_APT_DISABLE(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return bsp_modem_apt_disable(modem_id, mode_id);
}
/*****************************************************************************
 �� �� ��  : DRV_MODEM_APT_STATUS_GET
 ��������  : ͨ��ģ���ȡ��ǰAPT״̬�ӿ�
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : PWRCTRL_COMM_ON     APTʹ��
             PWRCTRL_COMM_OFF    APTδʹ��
             BSP_ERROR           ��ȡʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_APT_STATUS_GET(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return bsp_modem_apt_status_get(modem_id, mode_id);
}

/*****************************************************************************
 �� �� ��  : DRV_MODEM_MODE_CONFIG
 ��������  : ͨ��ģ������Gģ��Wģ�ӿ�
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : BSP_OK         ���óɹ�
             BSP_ERROR      ����ʧ��
*****************************************************************************/
BSP_S32 DRV_MODEM_MODE_CONFIG(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return bsp_modem_mode_config(modem_id, mode_id);
}
/**********************************NEW PMU INTERFACE END*******************************************/
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

