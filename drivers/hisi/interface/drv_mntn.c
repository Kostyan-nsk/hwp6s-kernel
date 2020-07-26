/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mntn.c
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

/*��ģ������־�Ĵ���*/
#define BBP_CTU_MEAS_FLAG_L_RD          (0x0640)
#define BBP_CTU_MEAS_FLAG_W_RD          (0x0644)
#define BBP_CTU_MEAS_FLAG_T_RD          (0x0648)
#define BBP_CTU_MEAS_FLAG_GM_RD         (0x064C)

extern int printf(const char *format, ...);

/*****************************************************************************
  2 ��������
*****************************************************************************/

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern int exch_notifier_chain_register(exchCBReg cb);
extern int BSP_MEM_READ (unsigned int ulAddress, unsigned int* pulData);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*******************************************************************************
 �� �� ��: DRV_EXCH_CUST_FUNC_REG
 ��������: ��������ģ���ṩ����ӿڣ����ϲ�ע��ص������ڸ�λʱ�����ϲ�ָ����Ϣ
 �������: cb �ϲ�ָ���ص��ӿ�
 �������: ��
 �� �� ֵ: 0-��ӳɹ�������-���ʧ��
 ˵��    :  pucData :ָ����¼��Ϣ��ַ����ע�ⲻҪָ��ջ����ʱ���������ѿռ����ݣ�
                     ʹ�����Լ�ά���������ռ�ʹ���꣬�Լ��ͷš�����ȫ�ֱ�����
            aucName :����
            ulDataLen: ���ݳ���

���Լ��:
            ��������С:64K������8��ͨ����ÿ��ͨ����ֵ��󲻵ó���8k.
            ע�ắ������ֵΪ:
                ����:    ulDataLen data���ݳ��ȣ�
                �쳣:    0

����ֵ�б�:
    CUST_REG_OK = 0,                        ��ӳɹ�
    CUST_REG_PARM_NULL,                     ���Ϊ��
    CUST_REG_EXCH_NOT_READY,                EXCHģ����δ��ʼ��
    CUST_REG_EXCH_REBOOT_CONTEXT,           �Ѿ����븴λ����
    CUST_REG_EXCH_QUEUE_NOT_INIT,           ע�������δ��ʼ��
    CUST_REG_EXCH_QUEUE_FULL,               ��������
    CUST_REG_EXCH_ALREADY_IN_QUEUE,         �ظ�ע��
    CUST_REG_EXCH_MUTEX_REJECT,             MUTEX REJECT
    CUST_REG_EXCH_QUEUE_IN_ERR              �����ʧ��

��ͨ������ֵ��λ�������

*******************************************************************************/
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
int DRV_EXCH_CUST_FUNC_REG(exchCBReg cb)
{
    return exch_notifier_chain_register(cb);
}
#endif

/*****************************************************************************
 �� �� ��  : DRV_MEM_READ
 ��������  : ��32λ��Ȳ�ѯ�ڴ棬���4�ֽ����ݡ���ά�ɲ⣬V7�ȴ�׮
 �������  : ulAddress����ѯ��ַ�ռ��ַ����ַ�����ڴ淶Χ�ڣ����򷵻�-1��
 �������  : pulData��ָ���ѯ���ݵ�ָ�롣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
int DRV_MEM_READ (unsigned int ulAddress, unsigned int* pulData)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)    
    return BSP_MEM_READ(ulAddress, pulData);
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

void drvShutdown(DRV_SHUTDOWN_REASON_ENUM eReason)
{
    return;   /* ��׮ */
}

/*****************************************************************************
 �� �� ��  : DRV_SHUT_DOWN
 ��������  : ����ػ�
 �������  : ��
 �������  : eReason��        �ػ�ԭ��
 �� �� ֵ  : ��
*****************************************************************************/
void DRV_SHUT_DOWN( DRV_SHUTDOWN_REASON_ENUM eReason )
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#if( FEATURE_POWER_ON_OFF == FEATURE_ON )
    drvShutdown(eReason);
#else
    DRV_PRINTF("Function DRV_SHUT_DOWN unsupport!\n");
#endif
#endif

#if (defined BSP_CORE_APP)
    return;   /* ��׮ */
#endif
}

/*****************************************************************************
* �� �� ��  : DRV_MSP_PROC_REG
*
* ��������  : DRV�ṩ��OM��ע�ắ��
*
* �������  : MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc
* �������  : NA
*
* �� �� ֵ  : NA
*
* ����˵��  : ��ά�ɲ�ӿں���
*****************************************************************************/
void DRV_MSP_PROC_REG(MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc)
{
    BSP_MspProcReg(eFuncID, pFunc);
}

#define printk(a)

int BSP_MNTN_BootFlagConfig(unsigned int ulBootFlag)
{
    return 0;
}
 
int BSP_MNTN_GetFlashSpec(unsigned char * pFlashInfo, unsigned int ulLength)
{
    return 0;
}

int BSP_MNTN_GetSupportBands(unsigned short *pusWBands, unsigned short *pusGBands)
{
    return 0;
}

int BSP_MNTN_RFGLockStateGet(unsigned int *Status)
{
    return 0;
}

int BSP_MNTN_WriteSysBackupFile(void)
{
    return 0;
}


BSP_BOOL DRV_MNTN_GetExtAntenLockState(unsigned int *Status)
{
    printk("stub: DRV_MNTN_GetExtAntenLockState\n");
    return 0;
}

/*****************************************************************************
 �� �� ��  : BSP_MNTN_GETSDSTATUS
 ��������  : ����SD����λ״̬
 �������  : �ޡ�
 �������  : �ޡ�
 ����ֵ��   1��λ
            0����λ

*****************************************************************************/
int BSP_SDMMC_GetSDStatus(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_OM_GetChipType
*
* ��������  : ��ȡоƬ����,HSO����ʹ��
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSPоƬ����ö��
*
* ����˵��  : ��
*
*****************************************************************************/
BSP_CHIP_TYPE_E BSP_OM_GetChipType(void)
{
    return 8; /* V9ASIC��ʱд8�������˶� */
}

/*****************************************************************************
* �� �� ��  : BSP_HwGetVerMain
* ��������  : ��ȡ��Ʒ�汾��,����HSO����ʱUE�汾�ŵ���ʾ
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
BSP_U16 BSP_HwGetVerMain(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (BSP_U16)(getHwVersionIndex());
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_TFUP_CompleteDeal
* ��������  : װ��ʹ��,TLʹ��
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
BSP_VOID BSP_TFUP_CompleteDeal(void)
{
   return ; /* ��׮ */
}

/*****************************************************************************
* �� �� ��  : BSP_TBAT_Read
* ��������  : װ��ʹ��,TLʹ��
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
BSP_S32 BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    return  0; /* ��׮ */
}

/*****************************************************************************
* �� �� ��  : BSP_TBAT_Write
* ��������  : װ��ʹ��,TLʹ��
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
BSP_S32 BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem)
{
    return 0; /* ��׮ */
}

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
BSP_S32 BSP_DLOAD_GetSoftwareVer(BSP_CHAR *str, BSP_U32 len)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_MNTN_MemVersionCtrl((signed char*)str, (unsigned char)len, (unsigned char)VER_SOFTWARE, (unsigned char)VERIONREADMODE);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_GetProductName
* ��������  : ��ȡ��Ʒ����
* �������  : char* pProductName,�ַ���ָ�룬��֤��С��32�ֽ�
*             BSP_U32 ulLength,����������
* �������  : ��
* �� �� ֵ  : 0����ȷ����0: ʧ��
* �޸ļ�¼  :
*****************************************************************************/
BSP_S32 BSP_GetProductName (char * pProductName, unsigned int ulLength)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_MNTN_MemVersionCtrl((signed char*)pProductName, (unsigned char)ulLength, (unsigned char)VER_PRODUCT_ID, (unsigned char)VERIONREADMODE);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_HwGetHwVersion
* ��������  : ��ȡӲ���汾����
* �������  : BSP_CHAR* pHwVersion,�ַ���ָ�룬��֤��С��32�ֽ�
* �������  : ��
* �� �� ֵ  : ��
* �޸ļ�¼  :
*****************************************************************************/
BSP_S32 BSP_HwGetHwVersion (char* pFullHwVersion, BSP_U32 ulLength)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return BSP_MNTN_MemVersionCtrl((signed char*)pFullHwVersion, (unsigned char)ulLength, (unsigned char)VER_HARDWARE, (unsigned char)VERIONREADMODE);
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_PWC_GetMeansFlag
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
BSP_S32 BSP_PWC_GetMeansFlag(PWC_COMM_MODE_E enCommMode)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    BSP_U32 pwc_meas_offset = 0;
    BSP_U32 pwc_meas_value = 0;

    switch(enCommMode){
            case PWC_COMM_MODE_WCDMA:
                pwc_meas_offset = BBP_CTU_MEAS_FLAG_W_RD;
                break;
            case PWC_COMM_MODE_GSM:
                pwc_meas_offset = BBP_CTU_MEAS_FLAG_GM_RD;
                break;
            case PWC_COMM_MODE_LTE:
                pwc_meas_offset = BBP_CTU_MEAS_FLAG_L_RD;
                break;
            case PWC_COMM_MODE_TDS:
                pwc_meas_offset = BBP_CTU_MEAS_FLAG_T_RD;
                break;
            default:
                return -1;
                break;
        }

    /*lint -e40*/
    BSP_REG_READ(SOC_BBP_COMM_BASE_ADDR, pwc_meas_offset, pwc_meas_value);
    /*lint +e40*/

    return pwc_meas_value;
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}
/*****************************************************************************
* �� �� ��  : BSP_BBPDMA_DrxBakReg
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
BSP_S32 BSP_BBPDMA_DrxBakReg(void)
{
    return  0; /* ��׮ */
}

/*****************************************************************************
* �� �� ��  : BSP_CPU_GetTotalUtilize
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
BSP_U32 BSP_CPU_GetTotalUtilize(void)
{
    return  0; /* ��׮ */
}

/*****************************************************************************
 �� �� ��  : BSP_GUSYS_ModeSwitchs
 ��������  : ���ӿ�ʵ��WCDMA/GSMϵͳģʽ�л���
 �������  : uiSysMode��0���л���WCDMAģʽ��1���л���GSMģʽ��
 �������  : �ޡ�
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
*****************************************************************************/
BSP_S32 BSP_GUSYS_ModeSwitch(BSP_U32 uiSysMode)
{
    return 0; /* ��ʱ��׮��������Ҫ���ʵ�� */
}

/*****************************************************************************
* �� �� ��  : nand_isbad
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
BSP_U32 nand_isbad(BSP_U32 blockID, BSP_U32 *flag)
{
    return 0; /* ��׮,����NAND_OK */
}

/*****************************************************************************
* �� �� ��  : BSP_PM_SetWakeSrc
* ��������  : ��̬���û���Դ
* �������  : eWakeSrc ����Դö��ֵ
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
BSP_VOID BSP_PM_SetWakeSrc(DRV_PM_WAKE_SRC_ENUM wake_src)
{
    /*V9R1��׮����*/
    return;
}

/*****************************************************************************
* �� �� ��  : BSP_PM_UnsetWakeSrc
* ��������  : ��̬���û���Դ
* �������  : eWakeSrc ����Դö��ֵ
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
BSP_VOID BSP_PM_UnsetWakeSrc(DRV_PM_WAKE_SRC_ENUM wake_src)
{
    /*V9R1��׮����*/
    return;
}

/********************************************************************************************************
 �� �� ��  : BSP_FS_GetDskspc
 ��������  : �õ��ļ�ϵͳ�ռ䡣(Modem Core,/yaffs0)
 �������  : path���ļ�ϵͳ·����
 �������  : DskSpc���ܿռ䡣
                           UsdSpc���ù��ռ䡣
                           VldSpc�����пռ䡣
 �� �� ֵ  : 0:  �����ɹ���
             -1������ʧ�ܡ�
********************************************************************************************************/
int BSP_FS_GetDskspc(const char *path,unsigned int *DskSpc,unsigned int *UsdSpc,  unsigned int *VldSpc)
{
    return 0;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

