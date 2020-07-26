

#ifndef __DRV_CHARGE_H__
#define __DRV_CHARGE_H__
#include "drv_global.h"

typedef enum CHARGING_STATE_ENUM_tag
{
        CHARGING_INIT = -1,
        NO_CHARGING_UP = 0,  /*����δ���*/
        CHARGING_UP ,              /*���������*/
        NO_CHARGING_DOWN ,  /*�ػ�δ���*/
        CHARGING_DOWN         /*�ػ�δ���*/
}CHARGING_STATE_ENUM;

typedef enum BATT_LEVEL_ENUM_tag
{
        BATT_INIT = -2,
        BATT_LOW_POWER =-1,    /*��ص͵�*/
        BATT_LEVEL_0,                 /*0���ص���*/
        BATT_LEVEL_1,                  /*1���ص���*/
        BATT_LEVEL_2,                   /*2���ص���*/
        BATT_LEVEL_3,                    /*3���ص���*/
        BATT_LEVEL_4,                    /*4���ص���*/
        BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;

typedef struct BATT_STATE_tag
{
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM           battery_level;
}BATT_STATE_T;

#define CHARGER_ONLINE                  (1) /*�������λ*/
#define CHARGER_OFFLINE                 (0)/*���������λ*/

typedef enum   
{
BAT_CHARGING_ON_START = 0,/*�ڿ���ģʽ����ʼ���**/
BAT_CHARGING_OFF_START,	/*�ڹػ�ģʽ����ʼ���*/
BAT_LOW_POWER, 	/*��ص�����ʱ���ϱ����¼�*/
BAT_CHARGING_UP_STOP, 	/*ֹͣ���ʱ���ϱ����¼�*/
BAT_CHARGING_DOWN_STOP,
BAT_EVENT_MAX,  /*����¼�ֵ��С�ڴ�ֵ����Ϊ�Ƿ��¼�*/
}BATTERY_EVENT;

typedef enum
{
    TEMP_BATT_LOW, /*��ص����¼�*/
    TEMP_BATT_HIGH,  /*��ظ����¼�*/
}TEMPERATURE_EVENT;
typedef enum
{
    CHARGER_ATTACH,
    CHARGER_REMOVE,
}CHARGER_EVENT;

typedef enum USB_CHARGE_TYPE_ENUM_tag
{
    USB_CHARGE_TYPE_NON_CHECK = -1,          /* ��δ�жϳ������������� */
    USB_CHARGE_TYPE_HW = 0,                      /* ��Ϊ�����             */
    USB_CHARGE_TYPE_NON_HW =1,                  /* �ǻ�Ϊ�����           */
    
}USB_CHARGE_TYPE_ENUM;

/************************************************************************
 * FUNCTION
 *       app_get_battery_state
 * DESCRIPTION
 *       ��ȡ�ײ���״̬��Ϣ       
 * INPUTS
 *       NONE
 * OUTPUTS: battery_state:�Ƿ���/��ص���
 * return:    0 Success, -1 failed     
 *************************************************************************/
int app_get_battery_state(BATT_STATE_T *battery_state);
/************************************************************************
 * FUNCTION
 *       int getCbcState(unsigned char *pucBcs ,unsigned char  *pucBcl)
 * DESCRIPTION
 *       pucBcs
            0: ����ڹ���
            1: �������ӵ��δ����
            2: û����������
         pucBcl
            0:���������û����������
             100: ʣ������ٷֱ�
 * INPUTS
 *          
 * OUTPUTS: 
           pucBcs
           pucBcl
 * return:    0 Success, -1 failed     
 *******************************************************************/
int getCbcState(unsigned char  *pucBcs ,unsigned char   *pucBcl);

/************************************************************************************
 *Function Name :  usb_charge_type   
 *Description   :  ��ȡ����USB�ߵ�����,�ж��ǻ�Ϊ�����,
                    ���Ƿǻ�Ϊ�����(�����ǻ�Ϊ���������ͨPC��)
 *Others        :  No
************************************************************************************/
USB_CHARGE_TYPE_ENUM usb_charge_type( void );

/************************************************************************************
 *Function Name :  usb_enum_completed   
 *Description   :  �ж�USB��ö���Ƿ����,
                        ��ɷ���(1),δ��ɷ���(0)
 *Others        :  No
************************************************************************************/
unsigned int usb_enum_completed( void );

/************************************************************************
 * FUNCTION
 *       usb_get_charger_status
 * DESCRIPTION
 *       ��ȡ���������λ״̬
 * INPUTS
 *       NONE
 * OUTPUTS
 *       ��λ�򷵻�1�����򷵻�0��
 *************************************************************************/
unsigned int usb_get_charger_status(void);


/*****************************************************************************
 �� �� ��  : BSP_CHG_GetCbcState
 ��������  : ���ص��״̬�͵���
 �������  :pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
                          pucBcl  0:���������û����������
 �������  : pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
                          pucBcl  0:���������û����������
 ����ֵ��   0 �����ɹ�
                         -1����ʧ��

*****************************************************************************/
extern int BSP_CHG_GetCbcState(unsigned char *pusBcs,unsigned char *pucBcl);
extern int DRV_CHG_GET_CBC_STATE(unsigned char *pusBcs,unsigned char *pucBcl);

/*AT ����ö������*/
typedef enum
{
    CHG_AT_BATTERY_CHECK,        /* BatteryУ׼*/
    CHG_AT_BATTERY_LEVEL,        /* Battery���� */
    CHG_AT_BATTERY_ADC,          /* Battery Temperature����*/
    CHG_AT_BATTERY_INVALID
} ENUM_CHG_ATCM_TYPE;

typedef enum ENUM_POWER_ON_MODE_tag
{
    POWER_ON_INVALID = 0,          	/* ��Ч����ģʽ*/
    POWER_ON_NORMAL,       	 	/* ��������ģʽ*/
    POWER_ON_CHARGING,       	/* �ػ����ģʽ*/
    POWER_ON_MAX
} ENUM_POWER_ON_MODE;
/*****************************************************************************
 �� �� ��  : BSP_CHG_GetBatteryState
 ��������  :��ȡ�ײ���״̬��Ϣ
 �������  :battery_state ������Ϣ
 �������  :battery_state ������Ϣ
 ����ֵ��   0 �����ɹ�
                         -1����ʧ��

*****************************************************************************/
extern int BSP_CHG_GetBatteryState(BATT_STATE_T *battery_state);
extern int DRV_CHG_GET_BATTERY_STATE(BATT_STATE_T *battery_state);

/*****************************************************************************
 �� �� ��  : BSP_CHG_ChargingStatus
 ��������  :��ѯĿǰ�Ƿ����ڳ����
 �������  :��
 �������  :��
 ����ֵ��   0 δ���
                          1 �����
*****************************************************************************/
extern int BSP_CHG_ChargingStatus(void);
extern int DRV_CHG_GET_CHARGING_STATUS(void);

/*****************************************************************************
 �� �� ��  : BSP_CHG_StateSet
 ��������  :ʹ�ܻ��߽�ֹ���
 �������  :ulState      0:��ֹ���
 						1:ʹ�ܳ��
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern void DRV_CHG_STATE_SET(unsigned long ulState);

/*****************************************************************************
 �� �� ��  : BSP_CHG_StateGet
 ��������  :��ѯ
 �������  :
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern BSP_S32 BSP_CHG_StateGet(void);
#define DRV_CHG_STATE_GET()    BSP_CHG_StateGet()

/*****************************************************************************
 �� �� ��  : BSP_CHG_Sply
 ��������  :��ѯ
 �������  :
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern BSP_S32 BSP_CHG_Sply(void);
#define DRV_CHG_BATT_SPLY()    BSP_CHG_Sply()


/*****************************************************************************
* �� �� ��  : BSP_TBAT_Read
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/*****************************************************************************
* �� �� ��  : BSP_TBAT_Write
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/**********************************************************************
�� �� ��      : BSP_TBAT_CHRStGet
��������  :  TBAT AT^TCHRENABLE?�Ƿ���Ҫ����

�������  : ��
�������  : ��
�� �� ֵ      : 1:��Ҫ����
			      0:����Ҫ����
ע������  : ��
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRStGet(void);

/**********************************************************************
�� �� ��      : BSP_TBAT_CHRSuply
��������  :  TBAT AT^TCHRENABLE=4����ʵ��
            			��Ҫ�ṩ����ɹ�LCD��ʾͼ��
�������  : ��
�������  : ��
�� �� ֵ      :
ע������  : ��
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRSply(void);

/**********************************************************************
�� �� ��      : BSP_ONOFF_StartupModeGet
��������  :  A���жϿ��ػ�ģʽ

�������  : ��
�������  : ��
�� �� ֵ      :
ע������  : ��
***********************************************************************/
ENUM_POWER_ON_MODE BSP_ONOFF_StartupModeGet( void );
/*****************************************************************************
* �� �� ��  : BSP_PMU_BattCali
*
* ��������  : ���У׼ǰ��С��������ӿڣ���AT����
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
extern void BSP_PMU_BattCali(void);


#endif

