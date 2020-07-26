/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_sd.h
  �� �� ��   : ����
  ��    ��   : �˳� d00221159
  ��������   : 2013��2��2��
  ����޸�   :
  ��������   : drv_sd.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��2��2��
    ��    ��   : �˳� d00221159
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_SD_H__
#define __DRV_SD_H__
#include "drv_global.h"
typedef enum _SD_EVENT
{
	SD_ATTATCH = 0,
	SD_REMOVE,
	SD_DETACH,      /*l00205892*/
	SD_EVENT_MAX
} SD_EVENT;


/************************************************************************
 * FUNCTION
 *       sdmmcUsbGetStatus
 * DESCRIPTION
 *       Get sd card in or out status
 * INPUTS
 *       NONE
 * OUTPUTS
 *       0: CARD IN 
          -1: CARD out
 *************************************************************************/

unsigned int  sdmmcUsbGetStatus(void);

#define WRFlAG     1
#define RDFlAG     0

typedef enum
{
    TFUP_FAIL = 0,  /*���*/
    TFUP_OK         /*��*/
}TFUP_DISPLAY_TYPE;

enum SD_MMC_OPRT_ENUM
{
    SD_MMC_OPRT_FORMAT = 0,
    SD_MMC_OPRT_ERASE_ALL,
    SD_MMC_OPRT_ERASE_SPEC_ADDR,
    SD_MMC_OPRT_WRITE,
    SD_MMC_OPRT_READ,
    SD_MMC_OPRT_BUTT
};

typedef unsigned int SD_MMC_OPRT_ENUM_UINT32;


/*SD���豸����*/
typedef struct _sd_dev_type_str
{
    unsigned long   devNameLen;
    char            devName[16];
}SD_DEV_TYPE_STR;

/*****************************************************************************
 �� �� ��  : BSP_SDMMC_ATProcess
 ��������  : at^sd,SD��������д����������ʽ������
 �������  : �������� ulOp:
            0  ��ʽ��SD��
            1  ��������SD�����ݣ�
            2  ����ָ����ַ���ݵĲ���������ָ�����������ݳ���Ϊ512�ֽڡ�������ĵ�ַ��дȫ1
            3  д���ݵ�SD����ָ����ַ�У���Ҫ���ڶ����͵���������
            4  ��ȡulAddrָ���ĵ�ַ(ulAddr*512)��512���ֽڵ����ݵ�pucBuffer��

            ulAddr < address >  ��ַ����512BYTEΪһ����λ��������n��ʾ

            ulData
             < data >            �������ݣ���ʾ512BYTE�����ݣ�ÿ���ֽڵ����ݾ���ͬ��
             0       �ֽ�����Ϊ0x00
             1       �ֽ�����Ϊ0x55
             2       �ֽ�����Ϊ0xAA
             3       �ֽ�����Ϊ0xFF

 �������  : pulErr
 �� �� ֵ  : 0 ��OK  �� 0 ��Error

            ����Ĵ���ֵ�����*pulErr��
            0 ��ʾSD������λ
            1 ��ʾSD����ʼ��ʧ��
            2 ��ʾ<opr>�����Ƿ�����Ӧ������֧��(�ô�����ATʹ��,����Ҫ����ʹ��)
            3 ��ʾ<address>��ַ�Ƿ�������SD����������
            4 ����δ֪����
*****************************************************************************/
extern  unsigned long  BSP_SDMMC_ATProcess(SD_MMC_OPRT_ENUM_UINT32 ulOp,
                unsigned long ulAddr,  unsigned long ulData,unsigned char *pucBuffer,unsigned long *pulErr);
#define DRV_SDMMC_AT_PROCESS(ulOp,ulAddr,ulData,pucBuffer,pulErr)   \
                     BSP_SDMMC_ATProcess (ulOp,ulAddr,ulData,pucBuffer,pulErr)

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
/*****************************************************************************
* �� �� ��  : sd_mmc_blk_w
*
* ��������  :д���ӿ�
*
* �������  : u32StartBlk ��ʼ��
				  pu8DataBuf  д���ݻ���
				  u32Len д���ݴ�С������Ϊ512�ֽڵ�������
* �������  : NA
*
* �� �� ֵ  :  �ɹ�����0
*
* ����˵��  : NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_w(BSP_U32 u32StartBlk,BSP_U8 *pu8DataBuf,BSP_U32 u32Len);
/*****************************************************************************
* �� �� ��  : sd_mmc_blk_r
*
* ��������  : �����ӿ�
*
* �������  : u32StartBlk ��ʼ��
				  pu8DataBuf  �����ݻ���
				  u32Len  �����ݴ�С������Ϊ512�ֽڵ�������
* �������  : NA
*
* �� �� ֵ  :  �ɹ�����0
*
* ����˵��  : NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_r(BSP_U32 u32StartBlk,BSP_U8 *pu8DataBuf,BSP_U32 u32Len);
/*****************************************************************************
* �� �� ��  : sd_mmc_blk_erase
*
* ��������  : ��������ӿ�
*
* �������  : u32StartBlk ��ʼ��
				  u32EndBlk  ��ֹ��
* �������  : NA
*
* �� �� ֵ  :  �ɹ�����0
*
* ����˵��  : NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_blk_erase(BSP_U32 u32StartBlk,BSP_U32 u32EndBlk);
/*****************************************************************************
* �� �� ��  : sd_mmc_get_status
*
* ��������  : ����λ��ѯ
*
* �������  : void
* �������  : NA
*
* �� �� ֵ  : 0 : ��λ��-1: ����λ
*
* ����˵��  : NA
*
*****************************************************************************/
extern BSP_S32 sd_mmc_get_status(BSP_VOID);
#endif

/*****************************************************************************
 �� �� ��  : BSP_SDMMC_GetOprtStatus
 ��������  : at^sd,SD����ǰ����״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  :
            0: δ����������Ѿ����;
            1: ������
            2: �ϴβ���ʧ��
*****************************************************************************/
extern unsigned long BSP_SDMMC_GetOprtStatus(void);
#define DRV_SDMMC_GET_OPRT_STATUS()    BSP_SDMMC_GetOprtStatus()

/*****************************************************************************
 �� �� ��  : BSP_SDMMC_AddHook
 ��������  : TFģ�鹳�Ӻ���ע�ᡣ
 �������  : hookType���ص��������ͣ�
                       0����������ļ���Ϣ����õĻص�������
                       1��TF������֪ͨOM�Ļص�������
                       2��TF���γ�֪ͨOM�Ļص�������
                       3�� TF����ʼ����֪ͨU�̵Ļص�������
             p���ص�����ָ�롣
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
 ע������  ������ͬһ���ͻص��������ظ�ע��Ḳ����ǰ���á�
*****************************************************************************/
/*#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
 */
extern int sdmmcAddHook (int hookType , void * p);/*y00186965 for sd_update*/
#define DRV_SDMMC_ADD_HOOK(hookType,p)    sdmmcAddHook(hookType,p)
/*#else
 */
/*extern int BSP_SDMMC_AddHook(int hookType, void * p);
 */
/*#define DRV_SDMMC_ADD_HOOK(hookType,p)    BSP_SDMMC_AddHook(hookType,p)
 */
/*#endif
 */

/*****************************************************************************
 �� �� ��  : BSP_SDMMC_UsbGetStatus
 ��������  : ����SD��״̬
 �������  : �ޡ�
 �������  : �ޡ�
 ����ֵ��   0  Ϊ����
           ��0 ������

*****************************************************************************/
extern unsigned int BSP_SDMMC_UsbGetStatus(void);
#define DRV_SDMMC_USB_STATUS()    BSP_SDMMC_UsbGetStatus();

/*****************************************************************************
* �� �� ��  : sdmmc_ClearWholeScreen
* ��������  : SD������NV�ָ������ʾ������
* �������  : ��
* �������  : ��
* �� �� ֵ  :��
* ����˵��  : ��
*****************************************************************************/
extern void sdmmc_ClearWholeScreen(void);
#define DRV_SDMMC_CLEAR_WHOLE_SCREEN()    sdmmc_ClearWholeScreen()

/*****************************************************************************
* �� �� ��  : sdmmc_UpdateDisplay
* ��������  : SD������NV�ָ������ʾ����ʾ
* �������  : BOOL UpdateStatus
* �������  : ��
* �� �� ֵ  :��
* ����˵��  : �����ɹ���NV������ɺ���øú�����
              ����ʧ�ܺ���øú������
*****************************************************************************/
extern void sdmmc_UpdateDisplay(unsigned int UpdateStatus);
#define DRV_SDMMC_UPDATE_DISPLAY(state)    sdmmc_UpdateDisplay(state)

/*****************************************************************************
* �� �� ��  : DRV_SD_GET_STATUS
*
* ��������  : ����λ��ѯ
*
* �������  : void
* �������  : NA
*
* �� �� ֵ  : 0 : ��λ��-1: ����λ
*
* ����˵��  : NA
*
*****************************************************************************/
extern int DRV_SD_GET_STATUS(void);

/*****************************************************************************
* �� �� ��  : DRV_SD_GET_CAPACITY
*
* ��������  : ��������ѯ
*
* �������  : void
* �������  : NA
*
* �� �� ֵ  : 0 : ʧ�ܣ�>0: ������
*
* ����˵��  : NA
*
*****************************************************************************/
extern int DRV_SD_GET_CAPACITY(void);

/*****************************************************************************
* �� �� ��  : DRV_SD_TRANSFER
*
* ��������  : ���ݴ���
*
* �������  : struct scatterlist *sg    ���������ݽṹ��ָ��
                            unsigned dev_addr   ��д���SD block ��ַ
                            unsigned blocks    ��д���block����
                            unsigned blksz      ÿ��block�Ĵ�С����λ�ֽ�
                            int wrflags    ��д��־λ��д:WRFlAG ; ��:RDFlAG
* �������  : NA
*
* �� �� ֵ  : 0 : �ɹ�������: ʧ��
*
* ����˵��  : NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_TRANSFER(struct scatterlist *sg, unsigned dev_addr,unsigned blocks, unsigned blksz, int wrflags);
#endif
/*****************************************************************************
* �� �� ��  : DRV_SD_SG_INIT_TABLE
*
* ��������  : SD������ݴ���sg list��ʼ��
*
* �������  : const void *buf		��������buffer��ַ
				  unsigned int buflen	��������buffer��С��С��32K, ��СΪ512B��������
				  					����32K, ��СΪ32KB�������������bufferΪ128K
* �������  : NA
*
* �� �� ֵ  :  0 : �ɹ�;  ����:ʧ��
* ����˵��  : NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_SG_INIT_TABLE(const void *buf,unsigned int buflen);
#endif
/*****************************************************************************
* �� �� ��  : DRV_SD_MULTI_TRANSFER
*
* ��������  : SD������ݴ���
*
* �������  : unsigned dev_addr	��д���SD block��ַ
				  unsigned blocks		��д���block ����
				  unsigned blksz		ÿ��block �Ĵ�С����λ�ֽ�
				  int write			��д��־λ��д:1;	��:0
* �������  : NA
*
* �� �� ֵ  :  0 : �ɹ�;  ����:ʧ��
* ����˵��  : NA
*
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#else
extern int DRV_SD_MULTI_TRANSFER(unsigned dev_addr,unsigned blocks,unsigned blksz,int write);
#endif
/*****************************************************************************
 �� �� ��  : BSP_MNTN_GetSDDevName
 ��������  : ��ȡ�豸�б��е�SD�豸��
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : SD_DEV_TYPE_STR�ṹָ�롣
*****************************************************************************/
extern SD_DEV_TYPE_STR * BSP_MNTN_GetSDDevName(void);
#define DRV_GET_SD_PATH()     BSP_MNTN_GetSDDevName()

#endif

