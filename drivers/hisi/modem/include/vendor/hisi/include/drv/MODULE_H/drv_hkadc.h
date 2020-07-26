

#ifndef __DRV_HKADC_H__
#define __DRV_HKADC_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))

#define HKADC_OK  (0)
#define HKADC_ERROR (-1)
#define HKADC_BBP_CLK_CLOSE (1)

#define SYS_TEM_HKADC_TABLE_LEN  (32)
#define SYS_TEM_LOW              (-20)
#define SYS_TEM_HIGH             (85)
#define TEM_INVALID_VAL         (-100)

#define SIM_TEM_HKADC_TABLE_LEN  (19)
#define SIM_TEM_LOW              (10)
#define SIM_TEM_HIGH             (100)
#define SIM_TEM_OFFSET           (5)
#define ID_NV_Item_AT_SHELL_OPEN_FLAG (33)        /* ���浱ǰSHELL�ڵĿ���״̬ */


#define TEMPERATURE_HKADC_CHANNEL      (3)



/*************************************************
*  Function:  hkadcGsmTempRead
*  Description: Read gsm temperature *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         pslData:��Ӧhkadcͨ�����¶�ֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcGsmPATempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcWcdmaTempRead
*  Description: Read wcdma temperature *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         pslData:��Ӧhkadcͨ�����¶�ֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcWcdmaPATempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcSIMTempRead
*  Description: Read SIM temperature *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         pslData:��Ӧhkadcͨ�����¶�ֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcSIMTempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcOLEDTempRead
*  Description: Read OLED temperature *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         pslData:��Ӧhkadcͨ�����¶�ֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcOLEDTempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcBatTempRead
*  Description: Read battery temperature *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         pslData:��Ӧhkadcͨ�����¶�ֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcBatTempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcBatRefTempRead
*  Description: ͨ���������NTC���������¶ȣ������жϵ���Ƿ���λ*
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:*          
           hkAdcTable:  ��ѹ�¶Ȼ����
*  Output:
*         pslData:     �洢�ĵ�ѹת��Ϊ���¶�ֵ��
*  Return:
*         ����ִ�е�״̬���ֵ��
*************************************************/
int hkadcBatRefTempRead(int *pslData, unsigned short *hkAdcTable);

/*************************************************
*  Function:  hkadcPaPowerRead
*  Description: Read gsm PA power *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
int hkadcPAPowerRead(int *pslData);


/*************************************************
*  Function:  hkadcTemVoltTrans
*  Description:  *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*          
*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
signed short hkadcTemVoltTrans(unsigned char ucChan, unsigned short usVoltage, unsigned short *hkAdcTable);


/*************************************************
*  Function:  hkadcBatVoltRefSet
*  Description:  * ����Battery��ͺ���ߵ�ѹ��Ӧ��adcֵ
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*        minVol:��͵�ѹ��minAdc����͵�ѹ��Ӧ��adcֵ��
*	 maxVol:��ߵ�ѹ��maxAdc����ߵ�ѹ��Ӧ��adcֵ
*  Output:
*         N/A
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcBatVoltRefSet(int minVol, int minAdc, int maxVol, int maxAdc);

/*************************************************
*  Function:  hkadcBatVoltGet
*  Description:  *
*  Called By:sysctrl
*  Table Accessed:
*  Table Updated:
*  Input:
*        N/A  
*  Output:
*         pslData : ��ص�ѹֵ
*  Return:
*         HKADC_OK/HKADC_ERROR
*************************************************/
int hkadcBatVoltGet(int *pslData );



/*************************************************
*  Function:  hkadcBatADCRead
*  Description: Read the battery adc value 
*  Table Accessed:
*  Table Updated:
*  Input:
*         N/A
*  Output:
*         pslData:��Ӧhkadcͨ��adcֵ
*  Return:
*         HKADC_OK/HKADC_ERROR��
*************************************************/
int hkadcBatADCRead(int* pslData);

#endif

#ifndef _DRV_LLT_
typedef enum _KEY_EVENT
{
	KEY_WAKEUP = 0,
	KEY_WLAN,
	KEY_WPS,
	KEY_FACTORY_RESTORE,
	KEY_POWER_OFF,
	KEY_UPDATE,
	KEY_SSID,
    KEY_WPS_PROMPT = 7,
	KEY_WIFI_OFFLOAD = 15,
	KEY_EVENT_MAX
} KEY_EVENT;
#else

#ifndef KEY_WAKEUP
#define KEY_WAKEUP 0
#endif

#ifndef KEY_WLAN
#define KEY_WLAN 1
#endif

#ifndef KEY_FACTORY_RESTORE
#define KEY_FACTORY_RESTORE 3
#endif

#ifndef KEY_POWER_OFF
#define KEY_POWER_OFF 4
#endif

#ifndef KEY_UPDATE
#define KEY_UPDATE       5
#endif

#ifndef KEY_SSID
#define KEY_SSID         6
#endif

#ifndef KEY_WPS_PROMPT
#define KEY_WPS_PROMPT   7
#endif

#ifndef KEY_WIFI_OFFLOAD
#define KEY_WIFI_OFFLOAD 15
#endif

#ifndef KEY_EVENT_MAX
#define KEY_EVENT_MAX    16
#endif

#endif

typedef enum tagHKADC_TEMP_PROTECT_E
{
    HKADC_TEMP_TOP = 0,
    HKADC_TEMP_BATTERY = HKADC_TEMP_TOP,     /*0*/
    HKADC_TEMP_PA0,                          /*1*/
    HKADC_TEMP_PA1,                          /*2*/
    HKADC_TEMP_DCXO0,                        /*3*/
    HKADC_TEMP_DCXO1,                        /*4*/
    HKADC_TEMP_LCD,                          /*5*/
    HKADC_TEMP_SIM_CARD,                     /*6*/
    HKADC_TEMP_SYSTEMH,                      /*7*/
    HKADC_TEMP_SYSTEML,                      /*8*/
    HKADC_TEMP_BATT_ID,                      /*9*/
    HKADC_TEMP_CAMERA0_ID,                   /*10*/
    HKADC_TEMP_CAMERA1_ID,                   /*11*/
    HKADC_TEMP_DCXO0_LOW,                    /*12*/
    HKADC_TEMP_DCXO1_LOW,                    /*13*/
    HKADC_TEMP_SCAMERA,                      /*14*/
    HKADC_TEMP_FLASH,                        /*15*/


	HKADC_TEMP_BUTT
}HKADC_TEMP_PROTECT_E;


typedef enum tagHKADC_CONV_MODE_E
{
    HKADC_CONV_DELAY = 0,
    HKADC_CONV_NO_DELAY,
    HKADC_CONV_BUTT
}HKADC_CONV_MODE_E;

/*************************************************
 �� �� ��   : DRV_HKADC_BAT_VOLT_GET
 ��������   : ���ص�ǰ��ص�ѹֵ
 �������   : pslData : ��ص�ѹֵ
 �������   : pslData : ��ص�ѹֵ
 �� �� ֵ   :0:��ȡ�ɹ�
            -1:��ȡʧ��
*************************************************/
extern BSP_S32 DRV_HKADC_BAT_VOLT_GET(BSP_S32 *ps32Data);


/*****************************************************************************
* �� �� ��  : BSP_HKADC_PaValueGet
*
* ��������  :
*
* �������  :
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011-3-29 wuzechun creat
*
*****************************************************************************/
BSP_S32 BSP_HKADC_PaValueGet( BSP_U16 *pusValue );

/*****************************************************************************
 �� �� ��  : hkadcBatADCRead
 ��������  : ��ȡ��ص�ѹ����ֵ
 �������  : ��
 �������  : pTemp��        ָ�����¶ȵ�ָ�롣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern BSP_S32 DRV_GET_BATTERY_ADC(BSP_S32 * pslData);

/*****************************************************************************
 �� �� ��  : DRV_HKADC_GET_TEMP
 ��������  : ��ȡ�豸���¶�ֵ�͵�ѹֵ
 �������  : enMode, *pusVolt, *psTemp, ulMode
 �������  : *pusVolt, *psTemp
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int DRV_HKADC_GET_TEMP(HKADC_TEMP_PROTECT_E enMode, unsigned short* pusVolt, short* psTemp, HKADC_CONV_MODE_E ulMode);

#endif

