/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_update.h
  �� �� ��   : ����
  ��    ��   : �˳� d00221159
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_update.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    ��    ��   : �˳� d00221159
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_UPDATE_H__
#define __DRV_UPDATE_H__
#include "drv_global.h"
/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
/* ������ */
#define DLOAD_OK                    BSP_OK
#define DLOAD_ERROR                 BSP_ERROR
#define DLOAD_ERR_NOT_INITED        BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_MODULE_NOT_INITED)
#define DLOAD_ERR_NULL_PTR          BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_NULL_PTR)
#define DLOAD_ERR_INVALID_PARA      BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_INVALID_PARA)
#define DLOAD_ERR_ALLOC_FAILED      BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_BUF_ALLOC_FAILED)
#define DLOAD_ERR_FREE_FAILED       BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_BUF_FREE_FAILED)
#define DLOAD_ERR_RETRY_TIMEOUT     BSP_DEF_ERR(BSP_MODU_DLOAD,BSP_ERR_RETRY_TIMEOUT)

#define DLOAD_ERR_NO_BACKUP         BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 1)  /* �ޱ��ݰ汾 */
#define DLOAD_ERR_OPEN_FAILED       BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 2)  /* ���⴮�ڴ�ʧ�� */
#define DLOAD_ERR_SEM_CREAT         BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 3)  /* �ź�������ʧ�� */
#define DLOAD_ERR_ABORT             BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 4)  /* ������ֹ */
#define DLOAD_ERR_MODE_MISMATCH     BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 5)  /* ����ģʽ��ƥ�� */
#define DLOAD_ERR_INVALID_ATSTRING  BSP_DEF_ERR(BSP_MODU_DLOAD, BSP_ERR_SPECIAL + 6)  /* AT�ַ����Ƿ� */


#define MODE_GSM      0x01
#define MODE_CDMA     0x02
#define MODE_WCDMA    0x04


/*--------------------------------------------------------------*
 * ö�ٶ���                                                     *
 *--------------------------------------------------------------*/
enum UPDATE_STATUS_I
{
	DL_UPDATE = 0,
	DL_NOT_UPDATE = 1
};

typedef enum
{
	NORMAL_DLOAD = 0,	/*��������ģʽ*/
	FORCE_DLOAD = 1    /*ǿ������ģʽ*/
}DLOAD_TYPE;

/* ����ģʽö�� */
typedef enum tagDLOAD_MODE_E
{
    DLOAD_MODE_DOWNLOAD = 0,
    DLOAD_MODE_NORMAL,
    DLOAD_MODE_DATA,
    DLOAD_MODE_MAX
}DLOAD_MODE_E;

/*--------------------------------------------------------------*
 * ����ָ�����Ͷ���                                             *
 *--------------------------------------------------------------*/
typedef BSP_U32 (*BSP_DLOAD_AtCallBack)( BSP_VOID* pDataIn, BSP_U32 ulLen);
typedef BSP_U32 (*BSP_DLOAD_NVGetInfoCb)( BSP_VOID* pDataOut, BSP_U32 ulLen);
typedef BSP_U32 (*BSP_DLOAD_NVSetInfoCb)( BSP_VOID* pDataIn, BSP_U32 ulLen);

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetProductId
*
* ��������  : ��ȡ��Ʒ�����ַ���
*
* �������  : BSP_S8 *str   :�ַ���������
*             BSP_S32 len   :�ַ�������
* �������  : BSP_S8 *str   :�ַ���������
*
* �� �� ֵ  : DLOAD_ERR_INVALID_PARA    :��������Ƿ�
*             DLOAD_ERROR               :ʧ��
*             ����                      :�����ַ�������
*
* ����˵��  : ATģ�����
*             ����ģʽ֧��
*             ����ģʽ֧��
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetProductId(BSP_CHAR *str, BSP_U32 len);
/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetDloadType
*
* ��������  : ��ȡ��������
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  : 0 :����ģʽ
*             1 :ǿ��ģʽ
*
* ����˵��  : ATģ�����
*             ����ģʽ֧��
*             ����ģʽ֧��
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadType(BSP_VOID);
/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetCurMode
*
* ��������  : ���õ�ǰ����ģʽ
*
* �������  : BSP_VOID
* �������  : DLOAD_MODE_NORMAL     :����ģʽ
*             DLOAD_MODE_DATA       :����ģʽ
*             DLOAD_MODE_DOWNLOAD   :����ģʽ
*
* �� �� ֵ  : ��
*
* ����˵��  : ATģ�����
*             ����ģʽ֧�ֵ���:
*                 BSP_DLOAD_GetCurMode(DLOAD_MODE_DOWNLOAD)
*             ����ģʽ֧�ֵ���:
*                 BSP_DLOAD_GetCurMode(DLOAD_MODE_DATA)
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetCurMode(DLOAD_MODE_E eDloadMode);
/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetCurMode
*
* ��������  : ��ȡ��ǰ����ģʽ
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  : DLOAD_MODE_NORMAL     :����ģʽ
*             DLOAD_MODE_DATA       :����ģʽ
*             DLOAD_MODE_DOWNLOAD   :����ģʽ
*
* ����˵��  : ��
*
*****************************************************************************/
DLOAD_MODE_E BSP_DLOAD_GetCurMode(BSP_VOID);
/*****************************************************************************
* �� �� ��  : BSP_DLOAD_SetSoftLoad
*
* ��������  : ����������־
*
* �������  : BSP_BOOL bSoftLoad  :
*             BSP_FALSE :��vxWorks����
*             BSP_TRUE  :��bootrom����
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetSoftLoad (BSP_BOOL bSoftLoad);


/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetSoftLoad
*
* ��������  : ��ȡ������־
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  : BSP_TRUE  :��bootrom����
*             BSP_FALSE :��vxWorks����
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetSoftLoad (BSP_VOID);

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_SendData
*
* ��������  : ͨ�����⴮����PC�˷�������
*
* �������  : pBuf      :���ݻ�����
*             u32Len    :���ݻ���������
* �������  : pBuf      :���ݻ�����
*
* �� �� ֵ  : ��
*
* ����˵��  : pBuf���뱣֤cache line(32�ֽ�)����
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_SendData(BSP_CHAR *pBuf, BSP_U32 u32Len);
/*****************************************************************************
* �� �� ��  : BSP_DLOAD_AtProcReg
*
* ��������  : ע��AT������
*
* �������  : pFun
* �������  : ��
*
* �� �� ֵ  : DLOAD_OK:�ɹ�
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_AtProcReg (BSP_DLOAD_AtCallBack pFun);

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetSoftwareVer
* ��������  : ��ȡ����汾��
* �������  : BSP_S8 *str   :�ַ���������
*             BSP_S32 len   :�ַ�������
* �������  : BSP_S8 *str   :�ַ���������
* �� �� ֵ  : BSP_ERROR     :��������Ƿ�
*             ����          :�����ַ�������
* ����˵��  : ATģ�����
*             ����ģʽ֧��
*             ����ģʽ֧��
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetSoftwareVer(BSP_CHAR *str, BSP_U32 len);


/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetDloadVer
*
* ��������  : ��ѯ����Э��汾�š�����ϢBSP�̶�дΪ2.0��
*
* �������  : BSP_S8 *str   :�ַ���������
*             BSP_S32 len   :�ַ�������
* �������  : BSP_S8 *str   :�ַ���������
*
* �� �� ֵ  : DLOAD_ERROR   :��������Ƿ�
*             ����          :�����ַ�������
*
* ����˵��  : ATģ�����
*             ����ģʽ֧��
*             ����ģʽ֧��
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetDloadVer(BSP_CHAR *str, BSP_U32 len);

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetISOVer
*
* ��������  : ��ȡ��̨�汾���ַ���
*
* �������  : BSP_S8 *str   :�ַ���������
*             BSP_S32 len   :�ַ�������
* �������  : BSP_S8 *str   :�ַ���������
*
* �� �� ֵ  : DLOAD_ERROR   :��������Ƿ�
*             ����          :�����ַ�������
*
* ����˵��  : ATģ�����
*             ����ģʽ֧��
*             ����ģʽ֧��
*
*****************************************************************************/
BSP_S32 BSP_DLOAD_GetISOVer(BSP_CHAR *str, BSP_U32 len);
#define DRV_GET_CDROM_VERSION(pVersionInfo,ulLength)    BSP_DLOAD_GetISOVer(pVersionInfo, ulLength)

/*****************************************************************************
 �� �� ��  : DRV_SET_UPDATA_FLAG
 ��������  : �����������ر�־��
 �������  : flag���������ر�־��
                   0�����������bootrom�������������ء�
                   1�������󲻽���bootrom������������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern int DRV_SET_UPDATA_FLAG(int flag);

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_SetCdromMarker
*
* ��������  : �����豸��̬��־
*
* �������  : BSP_BOOL bCdromMarker  :
*             BSP_TRUE  :���������豸��̬
*             BSP_FALSE :����bootrom�豸��̬
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��������̨�ļ�ʱ������Ϊ�ϱ������豸��̬�����ָ�NV��
*             ������ǰ̨�ļ�ʱ������Ϊ�ϱ�bootrom�豸��̬���ָ�NV��
*
*****************************************************************************/
BSP_VOID BSP_DLOAD_SetCdromMarker(BSP_BOOL bCdromMarker);
#define DRV_SET_CDROM_FLAG(flag)    BSP_DLOAD_SetCdromMarker(flag)

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetCdromMarker
*
* ��������  : ��ȡ�豸��̬��־
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  : BSP_TRUE  :�ϱ������豸��̬
*             BSP_FALSE :�ϱ�bootrom�豸��̬
*
* ����˵��  : ��������̨�ļ�ʱ������Ϊ�ϱ������豸��̬�����ָ�NV��
*             ������ǰ̨�ļ�ʱ������Ϊ�ϱ�bootrom�豸��̬���ָ�NV��
*
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetCdromMarker (BSP_VOID);
#define DRV_GET_CDROM_FLAG()    BSP_DLOAD_GetCdromMarker()

/* Begin: DRVΪOAMʵ��NV���ݻָ����̶��ṩ�Ľӿ� */

/********************************************************************************************************
 �� �� ��  : DRV_UPGRADE_NV_SET_RESULT
 ��������  : OAMͨ���ýӿڸ��ߵ���NV�����ɹ��˻���ʧ����
 �������  : status��0      ��ʾNV�����ɹ�
                     ����ֵ ��ʾNV����ʧ��
 �������  : 
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT �� V9 BBIT ��׮
             V9 SFT  ʵ��
                ���NV�����ɹ���������fastboot�������óɹ���ʶ
                ���NV����ʧ�ܣ�������fastboot��������ʧ�ܱ�ʶ
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �����ɺ���               
********************************************************************************************************/
extern int DRV_UPGRADE_NV_SET_RESULT(int status);

/********************************************************************************************************
 �� �� ��  : DRV_NV_FILE_SEC_CHECK
 ��������  : ���ļ�����У��             
 �������  : pnvFilePath����У���ļ���·��
 �������  : 
 �� �� ֵ  : 0 :    �ļ�У����ȷ
             -1��   �ļ�У��ʧ��
 ����ע��  : Ŀǰ�������߱���ֱ�Ӵ�׮���Ժ�ʵ��			 
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �����ɺ���               
********************************************************************************************************/
extern int DRV_NV_FILE_SEC_CHECK(char * pnvFilePath);

/********************************************************************************************************
 �� �� ��  : BSP_DLOAD_NVBackupRead
 ��������  : ��Flash�е�NV�������ȡ���ݣ�ʵ��NV��Ļָ����ܡ�
 �������  : len����NV�������ʼ����ʼ����Ҫ��ȡ��NV��ȣ��ֽ�������
 �������  : pRamAddr��Ŀ��RAM��ַ�����ڴ�Ŷ�����NV�����ݡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT �� V9 BBIT ����ԭ��
             V9 SFT  ������nand�ӿ��޸�Ϊ����emmc�ӿڶ�дNV������
			 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �޸ĺ���  
********************************************************************************************************/
extern int BSP_DLOAD_NVBackupRead(unsigned char *pRamAddr, unsigned int len);
#define DRV_NVBACKUP_READ(pRamAddr,len)    BSP_DLOAD_NVBackupRead(pRamAddr, len)

/********************************************************************************************************
 �� �� ��  : BSP_DLOAD_NVBackupWrite
 ��������  : ���ض�����д��Flash�е�NV�������ʵ��NV��ı��ݹ��ܡ�
 �������  : pRamAddr��ԴRAM��ַ�����ڴ����Ҫд���NV�����ݡ�
             len����NV�������ʼ����ʼ����Ҫд���NV��ȣ��ֽ�������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT �� V9 BBIT ����ԭ��
             V9 SFT  ������nand�ӿ��޸�Ϊ����emmc�ӿڶ�дNV������
			  
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �޸ĺ���  
********************************************************************************************************/
extern int BSP_DLOAD_NVBackupWrite(unsigned char *pRamAddr, unsigned int len);
#define DRV_NVBACKUP_WRITE(pRamAddr, len)   BSP_DLOAD_NVBackupWrite(pRamAddr, len)

/********************************************************************************************************
 �� �� ��  : DRV_GET_FS_ROOT_DIRECTORY
 ��������  : ��ѯ��Ŀ¼�ķ������
 �������  : data�����ڴ�Ÿ�Ŀ¼�ַ����Ļ��������׵�ַ
             len:������data�ĳ���
 �������  : 
 �� �� ֵ  : 0 :    �����ɹ�
             -1��   ����ʧ��
 
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �����ɺ���               
********************************************************************************************************/
extern int DRV_GET_FS_ROOT_DIRECTORY(char * data, unsigned int len);

/********************************************************************************************************
 �� �� ��  : DRV_NVBACKUP_EXT_READ
 ��������  : ��Flash�е�LastBackup����ȡ���ݣ�ΪNV�������ݶ�ʧ���ݴ�ָ������ṩ֧��
 �������  : len����LastBackup��ʼ����ʼ����Ҫ��ȡ��NV��ȣ��ֽ�������
 �������  : pRamAddr��Ŀ��RAM��ַ�����ڴ�Ŷ�����NV�����ݡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT  ��׮
             V9R1 SFT  ʵ��

 �޸���ʷ  :
 1.��    ��   : 2013��10��22��
   ��    ��   : ��ϲ 00220237
   �޸�����   : ��������
********************************************************************************************************/
extern int DRV_NVBACKUP_EXT_READ(unsigned char *pRamAddr, unsigned int len);

/********************************************************************************************************
 �� �� ��  : DRV_NVBACKUP_EXT_WRITE
 ��������  : ���ض�����д��Flash�е�LastBackup����ΪNV�������ݶ�ʧ���ݴ�ָ������ṩ֧��
 �������  : pRamAddr��ԴRAM��ַ�����ڴ����Ҫд���NV�����ݡ�
             len����LastBackup����ʼ����ʼ����Ҫд���NV��ȣ��ֽ�������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT  ��׮
             V9R1 SFT  ʵ��

 �޸���ʷ  :
 1.��    ��   : 2013��10��22��
   ��    ��   : ��ϲ 00220237
   �޸�����   : ��������
********************************************************************************************************/
extern int DRV_NVBACKUP_EXT_WRITE(unsigned char *pRamAddr, unsigned int len);
/* End: DRVΪOAMʵ��NV���ݻָ����̶��ṩ�Ľӿ� */

/********************************************************************************************************
 �� �� ��  : NVBackupFlashDataWrite
 ��������  : ʵ��FLASH ��NV���ݻָ�����ָ��λ��д���ܡ�
 �������  : pRamAddr��ԴRAM��ַ�����ڴ����Ҫд���NV�����ݡ�
             offset :��NV��������ַ��ʼ��ƫ��
             len����ƫ�ƴ���ʼ����Ҫд��NV���ݵĳ��ȣ��ֽ�������
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ����ע��  : V3R3 SFT �� V9 BBIT ����ԭ��
             V9 SFT  ������nand�ӿ��޸�Ϊ����emmc�ӿڶ�дNV�����������ʱ�����ݷ������ֶ���Ϊ"/mnvbp"       
             
 �޸���ʷ  :
 1.��    ��   : 2012��12��6��
   ��    ��   : ��ϲ 00220237
   �޸�����   : �޸ĺ���  
********************************************************************************************************/
extern int NVBackupFlashDataWrite(unsigned char* pRamAddr, unsigned int offset,unsigned int len);
#define DRV_NV_FLASH_WRITE(pRamAddr, offset,len) NVBackupFlashDataWrite(pRamAddr, offset,len)


/*****************************************************************************
 �� �� ��  : BSP_DLOAD_GetTFUpdateFlag
 ��������  : �ж��Ƿ���TF������
 �������  : None
 �������  : None
 �� �� ֵ  : ����1��TF������NV�ָ��ɹ�����Ҫ������
             ����0����TF������NV�ָ��ɹ�������Ҫ������

*****************************************************************************/
extern int BSP_DLOAD_GetTFUpdateFlag(void);
#define DRV_GET_TFUPDATE_FLAG()    BSP_DLOAD_GetTFUpdateFlag()

/*****************************************************************************
 �� �� ��  : DRV_GET_DLOAD_VERSION
 ��������  : Get dload version
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern BSP_S32 DRV_GET_DLOAD_VERSION(BSP_U8 *str, int len);


/*****************************************************************************
 �� �� ��  : DRV_GET_DLOAD_INFO
 ��������  : Get dload infomation
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/

extern BSP_S32 DRV_GET_DLOAD_INFO(unsigned char atCmdBuf[], unsigned int dloadType);

/*****************************************************************************
 �� �� ��  : DRV_GET_AUTHORITY_VERSION
 ��������  : Get Authority version
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern BSP_S32 DRV_GET_AUTHORITY_VERSION(BSP_U8 *str, BSP_S32 len);

/*****************************************************************************
 �� �� ��  : DRV_GET_AUTHORITY_ID
 ��������  : ��ȡ���ؼ�ȨЭ��Id
 �������  : unsigned char *buf
                          int len
 �������  : ��
 �� �� ֵ  : 0:  �����ɹ���
                      -1������ʧ�ܡ�
*****************************************************************************/
extern BSP_S32 DRV_GET_AUTHORITY_ID(unsigned char *buf, BSP_S32 len);

/*****************************************************************************
 �� �� ��  : BSP_DLOAD_GetDloadNetMode
 ��������  : Get net mode
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern BSP_S32  BSP_DLOAD_GetDloadNetMode(BSP_U32 *netMode);
#define DRV_GET_DLOAD_NETMODE(netMode)    BSP_DLOAD_GetDloadNetMode (netMode)

/*****************************************************************************
 �� �� ��  : BSP_DLOAD_GetDloadFlashInfo
 ��������  : Get dload flash infomation
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ޡ�
*****************************************************************************/
extern BSP_S32 BSP_DLOAD_GetDloadFlashInfo(DLOAD_FLASH_STRU* pFlashInfo);
#define DRV_GET_DLOAD_FLASHINFO(pFlashInfo)    BSP_DLOAD_GetDloadFlashInfo(pFlashInfo)


/*****************************************************************************
 �� �� ��  : BSP_DLOAD_GetWebUIVersion
 ��������  : ���WEBUI �汾��Ϣ
 �������  : pVersionInfo: ��ŷ��صİ汾��Ϣ���ڴ��ַ
                           ulLength: ��ŷ��صİ汾��Ϣ���ڴ泤�ȣ�Ŀǰ�̶�
                           128
 �������  : pVersionInfo: ���صİ汾��Ϣ���ڴ��ַ
 �� �� ֵ  :  0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
extern BSP_S32 BSP_DLOAD_GetWebUIVersion(BSP_CHAR *pVersionInfo, BSP_U32 u32Length);
#define DRV_GET_WEBUI_VERSION(pVersionInfo, u32Length) BSP_DLOAD_GetWebUIVersion(pVersionInfo,u32Length)

/*****************************************************************************
* �� �� ��  : BSP_TFUP_CompleteDeal
* ��������  : TF��������ɺ���
* �������  :
* �������  :
* �� �� ֵ  : 
*****************************************************************************/
extern void BSP_TFUP_CompleteDeal(void);
#define DRV_TFUP_COMPLETEDEAL() BSP_TFUP_CompleteDeal()

/*****************************************************************************
* �� �� ��  : BSP_TFUP_CompleteDeal
* ��������  : ����������ɺ���
* �������  : 
* �������  : 
* �� �� ֵ  : 
*****************************************************************************/
void BSP_ONUP_CompleteDeal(void);
#define DRV_ONUP_COMPLETEDEAL() BSP_ONUP_CompleteDeal()

/*****************************************************************************
* �� �� ��  : BSP_DLOAD_GetDashFlag
* ��������  : �����Ƿ�ֻ������̨����
* �������  : BSP_DLOAD_GetDashFlag
* �������  : BSP_TRUE  :ֻ������̨����
*             BSP_FALSE :��ֻ������̨����
* �� �� ֵ  : ��
* ����˵��  : ��
*****************************************************************************/
BSP_BOOL BSP_DLOAD_GetDashFlag(BSP_VOID);
#define DRV_DLOAD_GETDASHFLAG() BSP_DLOAD_GetDashFlag()

#endif

