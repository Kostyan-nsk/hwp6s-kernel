/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_oled.h
  �� �� ��   : ����
  ��    ��   : �˳� d00221159
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_oled.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    ��    ��   : �˳� d00221159
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_OLED_H__
#define __DRV_OLED_H__
#include "drv_global.h"
#if (defined(FEATURE_E5) &&  defined(FEATURE_HILINK) && ((FEATURE_E5 == FEATURE_ON) || (FEATURE_HILINK == FEATURE_ON)))
/*OLED����ö���¼�*/
typedef enum 
{
    OLED_LIGHT = 0,
    OLED_SLEEP,
    OLED_DIM,
    OLED_RESET,
    OLED_UPDATE,
    OLED_CONTRAST,
    OLED_BRIGHTNESS,
    OLED_POWER_ON,
    OLED_POWER_OFF,
    OLED_DEL_TIMER,/*delete the power on animation timer*/
    OLED_SYS_POWER_DOWN,
    OLED_INIT_IMAGE,
    /* BB5D02326,z67631, begin 2009/8/31 */
    OLED_POWER_OFF_CHARGING_START,/*restart power off charging animation*/
    OLED_POWER_OFF_CHARGING_STOP,/*stop power off charging animation*/
    /* BB5D02326,z67631, end 2009/8/31 */
    OLED_CMD_MAX
}OLED_IOCTL_CMD;


/*************************************************
*  Function:  oledRefresh
*  Description: oled refresh *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          unsigned int ulXStart, unsigned int ulYStart������ʼ����
*          unsigned int ulXOffset, unsigned int ulYOffset������������ƫ��
*          unsigned char *pucBuffer������ʾ����

*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
int oledRefresh(unsigned int ulXStart, unsigned int ulYStart,unsigned int ulXOffset, unsigned int ulYOffset, unsigned char *pucBuffer);

/*************************************************
*  Function:  oledClear
*  Description: oled clear *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          unsigned int ulXStart, unsigned int ulYStart������ʼ����
*           unsigned int ulXOffset, unsigned int ulYOffset������������ƫ��
*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
int oledClear(unsigned int ulXStart, unsigned int ulYStart,unsigned int ulXOffset, unsigned int ulYOffset);


/*************************************************
*  Function:  oledIoctrl
*  Description: oled ioctrl *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*          int cmd --command ID
*          arg--para

*  Output:
*         N/A
*  Return:
*         N/A
*************************************************/
int oledIoctrl(unsigned int cmd,  unsigned long arg);


#else

/*****************************************************************************
*  Function:  DRV_OLED_CLEAR_WHOLE_SCREEN
*  Description: oled clear *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*         N/A
*  Output:
*         N/A
*  Return:
*         N/A
*****************************************************************************/
extern BSP_VOID DRV_OLED_CLEAR_WHOLE_SCREEN(BSP_VOID);

/*****************************************************************************
*  Function:  DRV_OLED_UPDATE_STATE_DISPLAY
*  Description: oled display right or not right  *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*         N/A
*  Output:
*         N/A
*  Return:
*         N/A
*****************************************************************************/
extern  void DRV_OLED_UPDATE_STATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 �� �� ��  : DRV_OLED_UPDATE_DISPLAY
 ��������  : SD����Ƭ�汾������OLED��ʾ
 �������  : BOOL UpdateStatus
 �������  : �ޡ�
 ����ֵ��   ��
 ����:  �����ɹ���NV������ɺ���øú�����
        ����ʧ�ܺ���øú������
*****************************************************************************/
extern void DRV_OLED_UPDATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 �� �� ��  : DRV_OLED_STRING_DISPLAY
 ��������  : oled�ַ�����ʾ����
 �������  :
 *           Ҫ��ʾ�ַ�������ʼ����
 *           Ҫ��ʾ���ַ���
 �������  : none
 �� �� ֵ  : void
*****************************************************************************/
extern BSP_VOID DRV_OLED_STRING_DISPLAY(UINT8 ucX, UINT8 ucY, UINT8 *pucStr);

/*****************************************************************************
 �� �� ��  : DRV_OLED_POWER_OFF
 ��������  : oled�µ纯��
 �������  :
 �������  : None
 �� �� ֵ  : void
*****************************************************************************/
extern BSP_VOID DRV_OLED_POWER_OFF(BSP_VOID);

#endif
#endif
