
#ifndef __DRV_WIFI_H__
#define __DRV_WIFI_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))
#ifdef BSP_CORE_MODEM
#include "../../app/CommAppInterface.h"
#else
#include "CommAppInterface.h"
#endif
#define WPS_PIN_LEN (8)                   /* WIFI WPS PIN CODE LEN */          


typedef enum 
{
    GPIO_WLAN_WAKE, /*WIFIоƬ�����ж��ϱ�*/
}GPIO_EVENT;

typedef enum
{
    MMI_PASS, 
}SCREEN_EVENT;

typedef enum
{
    WLAN_WPS_SUC,  /* wps����ɹ� */
    WLAN_WPS_FAIL,  /* wps����ʧ�� */
    WLAN_NO_STA,/* ��station�û����� */
    WLAN_TIME_OUT,/* wlan��ʱ����ʱ */
}WLAN_EVENT;

typedef struct
{
	int ulAID;              /* UID */
	unsigned char ulYiAddr[6];       /* MAC ADDR */
	int ulConnTime;          /* Total time of connected */
} stWLANSTAT;

/*****************************************************************************
 Function   : drivers_event_dispatch
 Description: 
 Calls      :
 Called By  : 
 Input      : None
 Return     : VOS_OK or VOS_ERR
 Other      : just dispatch, no process, no global variable return quikly
              Not support NOW, dont USE it! remove it later.
 *****************************************************************************/
int drivers_event_dispatch(device_event_t * device_event);

typedef int (*DRIVERS_EVENT_DISPATCH)(device_event_t * device_event);

void drv_dispatch_register( DRIVERS_EVENT_DISPATCH pCallback );

void drv_dispatch_unregister( void );



/*
   ע�� MMI ���ԵĻص����������û�ע���Ĭ�Ͻ��� MMI ����ģʽ��
   δע�����ȡ����Ĭ���˳� MMI ����ģʽ
*/
void drv_kpd_mmi_test_register( DRIVERS_EVENT_DISPATCH pCallback );
void drv_kpd_mmi_test_unregister( void );

/* Interface for app */
/************************************************************************
 * FUNCTION
 *       wifi_athr_startup
 * DESCRIPTION
 *       initialize wifi driver include enable wifi power, load firmware, 
 *       init wifi driver and hostapd. after the operate, the wifi modules 
 *       will be normal working status. The function should load hostapd 
 *       config file and wmi config file.
 * INPUTS
 *       type: 
 *           WIFI_NORMAL     0
 *           WIFI_TEST       1
 *           WIFI_ART        2
 *
 *       cal_mode:
 *          WIFI_CAL_HIGH  0
 *          WIFI_CAL_LOW   1
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_startup(int type, int cal_mode);


/************************************************************************
 * FUNCTION
 *       wifi_athr_shutdown
 * DESCRIPTION
 *       deinitialize the wifi driver, cut down the power of wifi.
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_shutdown(void);


/************************************************************************
 * FUNCTION
 *       wifi_athr_reset
 * DESCRIPTION
 *      reset the wifi driver, except cut down the power and load firmware, 
 *      the operate as same to wifi driver startup. all config of the wifi 
 *      will be reset from hostapd config file. and the module enter normal 
 *      working status. 
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_reset(void);


/************************************************************************
 * FUNCTION
 *       wifi_athr_powersave_enter
 * DESCRIPTION
 *       wifi chipset enter power save mode.
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_powersave_enter(void);



/************************************************************************
 * FUNCTION
 *       wifi_athr_powersave_exit
 * DESCRIPTION
 *       wifi_athr_powersave_exit
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_powersave_exit(void);



/************************************************************************
 * FUNCTION
 *       wifi_ath_get_sta
 * DESCRIPTION
 *       get connected station, it include all of ssid.
 *       Attention: the stStaList's memory should be malloc and free by caller(app).
 * INPUTS
 *       stWLANSTAT: user list
 * OUTPUTS
 *       -1: failed other: cnt of connected station
 *************************************************************************/
int wifi_ath_get_sta(stWLANSTAT *stStaList);


/************************************************************************
 * FUNCTION
 *       wifi_reg_wps_hook
 * DESCRIPTION
 *       register wps hook
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
unsigned long wifi_reg_wps_hook(DRIVERS_EVENT_DISPATCH pCallback);


/************************************************************************
 * FUNCTION
 *       wifi_athr_wps_key_startup
 * DESCRIPTION
 *       startup wps authentication with key
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void wifi_athr_wps_key_startup(void);


/************************************************************************
 * FUNCTION
 *       wifi_athr_wps_pin_startup
 * DESCRIPTION
 *       startup wps authentication with pin
 * INPUTS
 *       scWpsPin: PIN CODE
 * OUTPUTS
 *       NONE
 *************************************************************************/
void wifi_athr_wps_pin_startup(char scWpsPin[WPS_PIN_LEN]);

/************************************************************************
 * FUNCTION
 *       wifi_athr_get_sta_cnt
 * DESCRIPTION
 *       startup wps authentication with pin
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0 no station connected, other: the number of connected.
 *************************************************************************/
int wifi_athr_get_sta_cnt(void);


/************************************************************************
 * FUNCTION
 *       wifi_athr_balong_sleep
 * DESCRIPTION
 *       balong enter sleep mode, inform wifi chipset
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_balong_sleep(void);


/************************************************************************
 * FUNCTION
 *       wifi_athr_balong_wakeup
 * DESCRIPTION
 *       balong leave form sleep mode, inform wifi chipset
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: succuess 1: failed
 *************************************************************************/
int wifi_athr_balong_wakeup(void);

#endif


extern unsigned short BSP_WifiCalDataLen(unsigned short usLen);
#define DRV_WifiCalDataLen(usLen) BSP_WifiCalDataLen(usLen)

/*****************************************************************************
 �� �� ��  : WIFI_TEST_CMD
 ��������  : ��������
 �������  : cmdStr�������ַ���
 �������  : ��
 ����ֵ    ����
*****************************************************************************/
extern void WIFI_TEST_CMD(char * cmdStr);

/*****************************************************************************
 �� �� ��  : WIFI_GET_TCMD_MODE
 ��������  : ��ȡ���������ģʽ
 �������  : ��
 �������  : 16��У׼����ģʽ
             17����ʾ���ڷ���ģʽ
             18����ʾ����ģʽ
 ����ֵ    ������ִ�е�״̬���ֵ
*****************************************************************************/
extern int WIFI_GET_TCMD_MODE(void);

/*****************************************************************************
 �� �� ��  : WIFI_POWER_START
 ��������  : WIFI�ϵ�
 �������  : ��
 �������  : ��
 ����ֵ    �� 0: execute ok
              1: execute failed
*****************************************************************************/
extern int WIFI_POWER_START(void);

/*****************************************************************************
 �� �� ��  : WIFI_POWER_SHUTDOWN
 ��������  : WIFI�µ�
 �������  : ��
 �������  : ��
 ����ֵ    �� 0: execute ok
              1: execute failed
*****************************************************************************/
extern int WIFI_POWER_SHUTDOWN(void);

/*****************************************************************************
 �� �� ��  : WIFI_GET_STATUS
 ��������  : WIFI״̬��ȡ
 �������  : ��
 �������  : ��
 ����ֵ    �� 0: wifi is off
              1: wifi is in normal mode
              2: wifi is in tcmd mode
*****************************************************************************/
extern int WIFI_GET_STATUS(void);

/*****************************************************************************
 �� �� ��  : WIFI_GET_RX_DETAIL_REPORT
 ��������  : get result of rx report: totalPkt, GoodPkt, ErrorPkt
 �������  : ��
 �������  : totalPkt��goodPkt��badPkt
 ����ֵ    ����
*****************************************************************************/
extern void WIFI_GET_RX_DETAIL_REPORT(int* totalPkt,int* goodPkt,int* badPkt);

/*****************************************************************************
 �� �� ��  : WIFI_GET_RX_PACKET_REPORT
 ��������  : get result of rx ucast&mcast packets
 �������  : ��
 �������  : ucastPkts��mcastPkts
 ����ֵ    ����
*****************************************************************************/
extern void  WIFI_GET_RX_PACKET_REPORT(unsigned int *ucastPkts, unsigned int *mcastPkts);

/*****************************************************************************
 �� �� ��  : WIFI_GET_PA_CUR_MODE
 ��������  : get the currrent PA mode of the wifi chip
 �������  : ��
 �������  : ��
 ����ֵ    ��0:  (WIFI_ONLY_PA_MODE) WIFI chip is in PA mode
             1:  (WIFI_ONLY_NOPA_MODE) WIFI chip is in no PA mode
             -1: wifi chip is in abnormal mode
*****************************************************************************/
extern int WIFI_GET_PA_CUR_MODE(void);

/*****************************************************************************
 �� �� ��  : WIFI_GET_PA_MODE
 ��������  : get the support PA mode of wifi chip
 �������  : ��
 �������  : ��
 ����ֵ    ��0:  (WIFI_ONLY_PA_MODE) WIFI suppport only PA mode
             1:  (WIFI_ONLY_NOPA_MODE) WIFI suppport only no PA mode
             2:  (WIFI_PA_NOPA_MODE) WIFI suppport both PA &  no PA mode
             -1: failed
*****************************************************************************/
extern int WIFI_GET_PA_MODE(void);

/*****************************************************************************
 �� �� ��  : WIFI_SET_PA_MODE
 ��������  : set the PA mode of wifi chip
 �������  : 0:  (WIFI_ONLY_PA_MODE) WIFI suppport only PA mode
             1:  (WIFI_ONLY_NOPA_MODE) WIFI suppport only no PA mode
             2:  (WIFI_PA_NOPA_MODE) WIFI suppport both PA &  no PA mode
 �������  : ��
 ����ֵ    ��0: success
             -1: failed
*****************************************************************************/
extern int WIFI_SET_PA_MODE(int wifiPaMode);

/*****************************************************************************
 �� �� ��  : DRV_WIFI_DATA_RESERVED_TAIL
 ��������  : WIFI������Ҫ���ݿ��ڴ�
 �������  : usLen - �û��������ݳ���Len
 �������  : ��
 �� �� ֵ  : ��������β��Ԥ���ĳ���
*****************************************************************************/
extern unsigned int DRV_WIFI_DATA_RESERVED_TAIL(unsigned int len);

/*h00106354 20120201 �������ؽӿ� add start */
/*****************************************************************************
 �� �� ��  : DRV_WIFI_SET_RX_FCTL
 ��������  : ����WIFI�������ر�ʶ
 �������  : para1��para2
 �������  : ��
 �� �� ֵ  : BSP_OK/BSP_ERROR
*****************************************************************************/
extern unsigned long DRV_WIFI_SET_RX_FCTL(unsigned long para1, unsigned long para2);

/*****************************************************************************
 �� �� ��  : DRV_WIFI_CLR_RX_FCTL
 ��������  : ���WIFI�������ر�ʶ
 �������  : para1��para2
 �������  : ��
 �� �� ֵ  : 1 : ��
             0 : ��
*****************************************************************************/
extern unsigned long DRV_WIFI_CLR_RX_FCTL(unsigned long para1, unsigned long para2);

/*****************************************************************************
 �� �� ��  : DRV_AT_GET_USER_EXIST_FLAG
 ��������  : ���ص�ǰ�Ƿ���USB���ӻ���WIFI�û�����(C�˵���)
 �������  : ��
 �������  : ��
 �� �� ֵ  : 1 : ��
             0 : ��
*****************************************************************************/
extern unsigned long  DRV_AT_GET_USER_EXIST_FLAG(void);


unsigned long USB_ETH_DrvSetHostAssembleParam(unsigned long ulHostOUtTimeout);
unsigned long USB_ETH_DrvSetDeviceAssembleParam(unsigned long ulEthTxMinNum,
											    unsigned long ulEthTxTimeout,
											    unsigned long ulEthRxMinNum,
											    unsigned long ulEthRxTimeout);
/*h00106354 20120201 �������ؽӿ� add end */

#endif

