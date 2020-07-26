/*****************************************************************************

                  ��Ȩ���� (C), 1998-2010, ��Ϊ�������޹�˾

*****************************************************************************
  �� �� ��   : nvim.h
  �� �� ��   : V1.0
  ��    ��   : ���ķ�/00149739
  ��������   : 2010-02-27
  ��������   : ����ģ���MSP�ڲ�����ģ���ṩ����������ⲿ����ṩ��
               �ӿڵȣ���������ⶼ�ṩ�������ⲿ����ӿ�msp_xxx.h

  �����б�   : TODO: ...
  �޸���ʷ   :
  1.��    �� : 2010-02-27
    ��    �� : ���ķ�/00149739
    �޸����� : �����ļ�

*****************************************************************************/

/*****************************************************************************
 PROJECT   :
 SUBSYSTEM :
 MODULE    :
 OWNER     :
*****************************************************************************/
#ifndef __NVIM_H__
#define __NVIM_H__

#include "gen_msg.h"
#include <SysNvId.h>

#define NVM_FLASH_BLOCK_SIZE      (64*2048)
#define NVM_BACKUP_VERSION_FILE_BLOCK_NUM  4
#define NVM_IMEI_LENTH    15

typedef enum
{
    EN_NVM_HSO_HNV_FILE = 0,      /*HSO�����ɵ��ļ�*/
    EN_NVM_VERSION_FILE = 1,      /*��ǰ�汾ʹ�õ�NV�ļ�*/
    EN_NVM_BACKUP_FILE =2,           /*��������ʱ�����ݵ��ļ�*/
    EN_NVM_VERSION_INIT_FILE=3,     /* ���ݵ�ǰ�汾�ĳ�ʼֵ*/
    EN_NVM_FILE_LAST
} NVM_FILE_TEPY_ENUM;

typedef struct
{
    /*VOS_UINT32 ulType;        0: ��ʼ�ļ�, 1: ��ǰ�ļ�*/
    VOS_UINT32 ulFileLen;
    VOS_UINT8 *pData;
}NVIM_EXPORT_FILE_INFO_STRU;

typedef  enum
{
    NVIM_REVERT_SUSS,/*�ָ��ɹ�*/
    NVIM_REVERT_FAIL,/*�ָ�ʧ��*/
    NVIM_REVERT_BLOCK,/*�Զ��ָ�����*/
    NVIM_REVERT_BLOCK_FOR_NO_BACKUP,/*�Զ��ָ��ޱ��ݶ�����*/
    NVIM_REVERT_FAIL_FOR_NO_BACKUP/*ǿ�ƻָ��ޱ��ݶ�ʧ��*/
}NVIM_REVERT_STATE_EN;


extern VOS_UINT32 nvim_GetImei(VOS_CHAR szImei[16]);

#endif
