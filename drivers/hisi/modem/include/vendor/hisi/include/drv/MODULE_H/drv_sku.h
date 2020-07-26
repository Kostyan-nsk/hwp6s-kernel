
#ifndef __DRV_SKU_H__
#define __DRV_SKU_H__
#include "drv_global.h"


#define NV_MAX_CARRIER_PATH_LEN         (100)                    /* ��Ӫ��·����󳤶� ������\0*/
#define NV_MAX_IMAGE_PATH_LEN           (100)                     /* IMAGE ·����󳤶� ������\0*/
/* ��Ӫ�̺���Ӫ���л���� */
typedef struct
{
    char     acCurrentCarrierPath[NV_MAX_CARRIER_PATH_LEN + 1]; /* ��ǰ��Ӫ��·��*/
    char     acSwitchCarrierPath[NV_MAX_CARRIER_PATH_LEN + 1];  /* �л���Ӫ��·��*/
    char     acCurrentImagePath[NV_MAX_IMAGE_PATH_LEN + 1];     /* ��ǰImage·�� */
    char     acSwitchImagePath[NV_MAX_IMAGE_PATH_LEN + 1];      /* ���л�Image·�� */
    unsigned long  ulCarrierSwitchFlag;      /* 0:�����л� 1����Ҫ�л� */
    unsigned long  ulImageSwitchFlag;        /* 0:�����л� 1����Ҫ�л� */
}SKU_SWITCH_FLAG_STRU;

/*****************************************************************************
�� �� ��  : signed long sku_drv_get_carrier_path
��������  : ��OM�ṩ�ӿڣ���ѯָ����Ӫ��·��
�������  :
char   *pcCarrierName         ��Ӫ������
unsigned long  ulCarrierNameLen     ��Ӫ�����Ƴ���
char   *pcCurrentCarrierPath  ��Ӧ��Ӫ��·��
unsigned long *pulCurrentCarrierPathLen ��Ӧ��Ӫ��·���ĳ���
����ֵ    : OSAL_OK     �ɹ�
            OSAL_ERROR  ʧ��

***************************************************************************/
signed long sku_drv_get_carrier_path(
                                    char    *pcCarrierName,
                                    unsigned long   ulCarrierNameLen,
                                    char    *pcCurrentCarrierPath,
                                    unsigned long  *pulCurrentCarrierPathLen);

#define DRV_GET_CARRIER_PATH(pcCarrierName,ulCarrierNameLen,pcCurrentCarrierPath,pulCurrentCarrierPathLen)  \
        sku_drv_get_carrier_path(pcCarrierName,ulCarrierNameLen,pcCurrentCarrierPath,pulCurrentCarrierPathLen)


/*****************************************************************************
�� �� ��  : sku_switch_flag_file_read
��������  : �л���ʶ������
�������  :
            SKU_SWITCH_FLAG_STRU * pstSkuUpdateSwitchFlag �л������·��
�������  : ��
�� �� ֵ  : SKU_OK �ɹ�
            SKU_ERROR ʧ��
***************************************************************************/
int sku_switch_flag_file_read(SKU_SWITCH_FLAG_STRU * pstSkuUpdateSwitchFlag);
#define DRV_READ_SWITCH_FLAG(pstSkuUpdateSwitchFlag) \
        sku_switch_flag_file_read(pstSkuUpdateSwitchFlag)


/*****************************************************************************
�� �� ��  : sku_switch_flag_file_write
��������  : �л���ʶд����
�������  :
            SKU_SWITCH_FLAG_STRU * pstSkuUpdateSwitchFlag �л������·��
�������  : ��
�� �� ֵ  : SKU_OK �ɹ�
            SKU_ERROR ʧ��
***************************************************************************/

int sku_drv_write_switch_flag(SKU_SWITCH_FLAG_STRU *pstSkuUpdateSwitchFlag);
#define DRV_WRITE_SWITCH_FLAG(pstSkuUpdateSwitchFlag) \
        sku_drv_write_switch_flag(pstSkuUpdateSwitchFlag)


/*****************************************************************************
�� �� ��  : sku_get_current_image_path
��������  : ��õ�ǰʹ�õ�image·��
�������  : ��

�������  : ��
�� �� ֵ  : img_ptr
***************************************************************************/
int sku_get_current_image_path(char * pcImagePath);
#define DRV_GET_CURRENT_IMAGE_PATH(pcImagePath) \
        sku_get_current_image_path(pcImagePath)

/*****************************************************************************
�� �� ��  : sku_get_available_carrier_path
��������  : �ڵ�ǰImage�£���ȡһ�����õ���Ӫ��·��
�������  :  char     *pcCarrierPath     ��Ӫ�̴�ŵ�ַ
              VOS_UNIT *pulCarrierPathLen  ��󳤶�

�������  : char     *pcCarrierPath     ��Ӫ�̴�ŵ�ַ
             VOS_UNIT *pulCarrierPathLen ��Ӫ�̵�ַʵ�ʳ���
�� �� ֵ  : img_ptr
***************************************************************************/
signed long sku_get_available_carrier_path(char * pcCarrierPath, unsigned long *pulCarrierPathLen);
#define DRV_GET_AVAILABLE_CARRIER_PATH(pcImagePath, pulCarrierPathLen)\
        sku_get_available_carrier_path(pcImagePath, pulCarrierPathLen)

/*****************************************************************************
�� �� ��  : sku_drv_return_Image_path
��������  : ͨ��Image���ƻ�ȡImage·��
�������  : ��

�������  : ��
�� �� ֵ  : int 0 �����ɹ� ��������ʧ��
***************************************************************************/
int sku_drv_return_image_path(char *pcImageName, char *pcImagePath);
#define DRV_GET_IMAGE_PATH(pcFileName, pcImagePath) \
        sku_drv_return_image_path(pcFileName, pcImagePath)

#endif

