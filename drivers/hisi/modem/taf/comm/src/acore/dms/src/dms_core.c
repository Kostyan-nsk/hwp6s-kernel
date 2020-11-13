/*******************************************************************************

                  ��Ȩ���� (C), 1998-2010, ��Ϊ�������޹�˾

********************************************************************************
  �� �� ��   : dms_core.c
  �� �� ��   : V1.0
  ��    ��   : ���ķ�/00149739
  ��������   : 2010-06-01
  ��������   : TODO: ...

  �����б�   : TODO: ...
  �޸���ʷ   :
  1.��    �� : 2010-06-01
    ��    �� : ���ķ�/00149739
    �޸����� : �����ļ�

*******************************************************************************/

/*******************************************************************************
 PROJECT   :
 SUBSYSTEM :
 MODULE    :
 OWNER     :
*******************************************************************************/

#include "vos.h"
#include "msp_errno.h"
#include <dms.h>
#include "dms_core.h"
#include "SOCPInterface.h"
#include "PsLib.h"

#define THIS_FILE_ID PS_FILE_ID_DMS_CORE_C

DMS_MAIN_INFO                   g_stDmsMainInfo = {0};
extern VOS_UINT32               g_ulNdisCfgFlag ;
VOS_UINT32                      g_usbPlugFlag = 0;

VOS_UINT32                              g_ulPcuiRxSem;
VOS_UINT32                              g_ulCtrlRxSem;

/* Added by z60575 for logtool, 2013-10-25, begin */
#if (VOS_OS_VER == VOS_WIN32)
static const struct file_operations g_stPortCfgOps;
#else
static const struct file_operations g_stPortCfgOps        =
{
    .owner      = THIS_MODULE,
    .write      = DMS_WritePortCfgFile,
    .read       = DMS_ReadPortCfgFile,
};
#endif
/* Added by z60575 for logtool, 2013-10-25, end */

extern DMS_DEBUG_AT_SERV_NV_STRU g_dms_debug_atserv_nv_info;

extern DMS_DEBUG_INFO_TBL_STRU g_ast_dsm_debug_info_table[EN_SDM_DMS_DEBUG_INFO_MAX];

/*****************************************************************************
 �� �� ��  : initDmsHdlcInit
 ��������  : ��ʼ������ͨ����SOCP�Ķ�Ӧ��ϵ
 �������  :

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��72��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_VOID initDmsHdlcInit(VOS_VOID)
{
#if (RAT_MODE != RAT_GU)
    /*DIAGͨ�� */
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].ucChanStat =0 ;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].ucHdlcFlag = 1;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].ulCodeDesChanId = SOCP_CODER_DST_LOM_IND;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].ulDecodeSrcChanId =SOCP_DECODER_SRC_LOM ;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].ulRecivBufLen = 0;

    /*DIAG��AT�촫��ͨ�� */
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].ucChanStat = 0;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].ucHdlcFlag = 1;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].ulCodeDesChanId = SOCP_CODER_DST_LOM_CNF;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].ulDecodeSrcChanId = SOCP_DECODER_SRC_LOM;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].ulRecivBufLen = 0;

    /*����ϱ�ר�� */
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].ucChanStat = 0;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].ucHdlcFlag = 1;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].ulCodeDesChanId = SOCP_CODER_DST_LOM_IND;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].ulDecodeSrcChanId = 0;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].ulRecivBufLen = 0;
#endif
}

VOS_VOID initDmsMainInfo(VOS_VOID)
{
    VOS_UINT32 ulport;

    VOS_MemSet(g_ast_dsm_debug_info_table, 0, sizeof(g_ast_dsm_debug_info_table));

    /*VOS_MemSet(&g_stDmsMainInfo, 0, sizeof(DMS_MAIN_INFO)); */

    g_stDmsMainInfo.ucDmsVcom1SleepFlag = TRUE;
    g_stDmsMainInfo.ucDmsVcomATSleepFlag = TRUE;
    g_stDmsMainInfo.ucDmsVcomUartSleepFlag = TRUE;
    g_stDmsMainInfo.pfnRdDtaCallback   = NULL;
    g_stDmsMainInfo.pfnConnectCallBack = NULL;

    /*��ʼ�����ж˿ڵ�handle*/
    for(ulport = 0; ulport < EN_DMS_BEARER_LAST; ulport++)
    {
        g_stDmsMainInfo.stPhyProperty[ulport].slPortHandle = UDI_INVALID_HANDLE;
    }


    /*�����߼�ͨ��ʹ�ܵ�ͨ������*/
#if (VOS_WIN32 == VOS_OS_VER)
    g_stDmsMainInfo.stLogicPhy[EN_DMS_CHANNEL_DIAG].aenPhyChan = EN_DMS_BEARER_TCP_20248;
#else
    g_stDmsMainInfo.stLogicPhy[EN_DMS_CHANNEL_DIAG].aenPhyChan =EN_DMS_BEARER_USB_COM1_DIAG_CTRL;
#endif

    /*����ͨ��Ĭ�����ԣ����ڶ������ͨ����������Ӧһ���߼�ͨ��*/
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_TCP_20248].aenLogicChan = EN_DMS_CHANNEL_DIAG;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM1_DIAG_CTRL].aenLogicChan = EN_DMS_CHANNEL_DIAG;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM2_DIAG_APP].aenLogicChan  = EN_DMS_CHANNEL_DIAG;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM4_AT].aenLogicChan  = EN_DMS_CHANNEL_AT;
    g_stDmsMainInfo.stPhyProperty[EN_DMS_BEARER_USB_COM_CTRL].aenLogicChan  = EN_DMS_CHANNEL_AT;


    /*��ʼ������ͨ����SOCPͨ���Ķ�Ӧ��ϵ*/
    initDmsHdlcInit();

    /*��ʼ��ATͨ��ʹ�õľ�̬�ڴ�*/
    Dms_StaticBufInit();

}


/*****************************************************************************
 �� �� ��  : dmsGetConnStaFun
 ��������  : ��ȡͨ�����Ӵ�����
 �������  :

 �������  :
 �� �� ֵ  : VOS_NULL/�ص�����
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

DMS_CONNECT_STA_PFN dmsGetConnStaFun(VOS_VOID)
{
    DMS_MAIN_INFO * pstMainInfo = dmsGetMainInfo();

    if (pstMainInfo == NULL)
    {
        return NULL;
    }
    else
    {
        return pstMainInfo->pfnConnectCallBack;
    }
}

/*****************************************************************************
 �� �� ��  : dms_SetConnectStaCallback
 ��������  : ͨ�������¼�����ע�ắ��
 �������  : pfnReg: �ص�����ָ��

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_VOID dms_SetConnectStaCallback(DMS_CONNECT_STA_PFN pfnReg)
{
    DMS_MAIN_INFO * pstMainInfo = dmsGetMainInfo();

    if (NULL == pfnReg)
    {
        return ;
    }

    pstMainInfo->pfnConnectCallBack = pfnReg;

    return ;
}

/*****************************************************************************
 �� �� ��  : dmsGetReadFun
 ��������  : ��ȡAT���ص�����
 �������  :
 �������  :
 �� �� ֵ  : �ص�����ָ��
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

DMS_READ_DATA_PFN dmsGetReadFun(VOS_VOID)
{
    return g_stDmsMainInfo.pfnRdDtaCallback;
}

/*****************************************************************************
 �� �� ��  : dmsGetMainInfo
 ��������  : ��ȡDMSȫ�ֱ���ָ��
 �������  :
 �������  :
 �� �� ֵ  : ȫ�ֱ���ָ��
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

DMS_MAIN_INFO * dmsGetMainInfo(VOS_VOID)
{
    return &g_stDmsMainInfo;
}

/*****************************************************************************
�� �� ��  : dmsgetPhyBearProperty
��������  : ��ȡ����ͨ�����ݽṹָ��
�������  :
�������  :
�� �� ֵ  : �ṹ��ָ��
���ú���  :
��������  :
�޸���ʷ  :
    1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function

*****************************************************************************/

 DMS_PHY_BEAR_PROPERTY_STRU* dmsgetPhyBearProperty(VOS_VOID)
{
    DMS_MAIN_INFO * pstMainInfo = NULL;

    pstMainInfo = dmsGetMainInfo();

    return pstMainInfo->stPhyProperty;
}


/*****************************************************************************
 �� �� ��  : dms_Init
 ��������  : DMSģ���ʼ������
 �������  :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_VOID dms_Init(VOS_VOID)
{
    initDmsMainInfo();
    if (VOS_OK != VOS_SmCCreate( "PCUIRX", 0, VOS_SEMA4_FIFO, &g_ulPcuiRxSem))
    {
        vos_printf("dms_Init: creat pcui rx sem fail!\n");;
    }

    if (VOS_OK != VOS_SmCCreate( "CTRLRX", 0, VOS_SEMA4_FIFO, &g_ulCtrlRxSem))
    {
        vos_printf("dms_Init: creat ctrl rx sem fail!\n");;
    }
    BSP_USB_RegUdiEnableCB(dms_UsbEnableEvtProc);

    BSP_USB_RegUdiDisableCB(dms_UsbDisableEvtProc);

    return ;
}


VOS_VOID dms_UsbDisableEvtProc(VOS_VOID)
{
    VOS_UINT32                          ulRet;

    g_usbPlugFlag = 0;

    DMS_DEBUG_SDM_FUN(EN_SDM_DMS_DISABLE,0, 0, 0);

    /* nvֵΪ1��ʾ��at sever����1��ʾ��at sever����at sever�ر�pcui��ctrl�� */
    if (TRUE == g_stDmsMainInfo.bPortOpenFlg)
    {
        /*�ر�AT PCUIͨ��*/
        ulRet = dmsVcomAtPcuiClose();
        if(ERR_MSP_SUCCESS!=ulRet)
        {
            DMS_DEBUG_SDM_FUN(EN_SDM_DMS_DISABLE_ERR,0, 0, 3);
        }

        /*�ر�AT CTRLͨ��*/
        ulRet = dmsVcomCtrlClose();
        if(ERR_MSP_SUCCESS!=ulRet)
        {
            DMS_DEBUG_SDM_FUN(EN_SDM_DMS_DISABLE_ERR,0, 0, 5);
        }
    }

    /*�ر�NDIS TRCLͨ��*/
    ulRet =  dms_NcmClose();
    if(ERR_MSP_SUCCESS!=ulRet)
    {
        DMS_DEBUG_SDM_FUN(EN_SDM_DMS_DISABLE_ERR,0, 0, 4);
    }

    return ;
}



VOS_VOID dms_UsbEnableEvtProc(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    NVE_INFO_S                          stAtServerNv;
    VOS_INT32                           lReadNvRet;

    VOS_MemSet(&stAtServerNv, 0, sizeof(NVE_INFO_S));
    VOS_MemCpy(stAtServerNv.nv_name, "ATSERV", sizeof("ATSERV"));
    stAtServerNv.nv_number      = NVE_AT_SERVER_INDEX;
    stAtServerNv.nv_operation   = NVE_READ_OPERATE;
    stAtServerNv.valid_size     = 1;

    g_usbPlugFlag = 1;

    DMS_DEBUG_SDM_FUN(EN_SDM_DMS_INIT,0, 0, 0);

    /* NVEֻ��ȡһ�Σ���ȡ���ٶ�ȡ */
    if (FALSE == g_stDmsMainInfo.bPortCfgFlg)
    {
        lReadNvRet = DRV_NVE_ACCESS(&stAtServerNv);
        g_dms_debug_atserv_nv_info.lOperatRet       = lReadNvRet;
        g_dms_debug_atserv_nv_info.ulNvValue        = stAtServerNv.nv_data[0];

        g_stDmsMainInfo.bPortCfgFlg                 = TRUE;

        /* nvֵΪ1��ʾ��at sever����1��ʾ��at sever����at sever��pcui��ctrl�� */
        if(1 != stAtServerNv.nv_data[0] || ERR_MSP_SUCCESS != lReadNvRet)
        {
            g_stDmsMainInfo.ulPortCfgValue = DMS_TEST_MODE;
        }
        else
        {
            g_stDmsMainInfo.ulPortCfgValue = DMS_NORMAL_MODE;
        }
    }

    if (DMS_TEST_MODE == g_stDmsMainInfo.ulPortCfgValue)
    {
        g_stDmsMainInfo.bPortOpenFlg = TRUE;

        /*�� AT PCUI ͨ��*/
        ulRet = dmsVcomAtPcuiOpen();
        if(ERR_MSP_SUCCESS!=ulRet)
        {
            DMS_DEBUG_SDM_FUN(EN_SDM_DMS_INIT_ERR,0, 0, 3);
        }

        /*�� AT CTRL ͨ��*/
        ulRet = dmsVcomCtrolOpen();
        if(ERR_MSP_SUCCESS!=ulRet)
        {
            DMS_DEBUG_SDM_FUN(EN_SDM_DMS_INIT_ERR,0, 0, 5);
        }
    }

    /*�� NDIS CTRL ͨ��*/
    ulRet =  dms_NcmCfg();
    if(ERR_MSP_SUCCESS!=ulRet)
    {
        DMS_DEBUG_SDM_FUN(EN_SDM_DMS_INIT_ERR,0, 0, 4);
    }

    g_ulNdisCfgFlag = 1;

    return ;

}

/*****************************************************************************
 �� �� ��  : dms_UdiRead
 ��������  : udi_read��װ
 �������  : enCOM: ͨ��ID
             ucRedBuf: �ڴ�buf
             ulToReadSize:��Ҫ��ȡ����
             pulRealySize:ʵ�ʶ�ȡ����
 �������  :
 �� �� ֵ  : ERR_MSP_FAILURE/ERR_MSP_SUCCESS
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_UINT32 dms_UdiRead(DMS_PHY_BEAR_ENUM enCOM,VOS_UINT8 * ucRedBuf, VOS_UINT32 ulToReadSize,  VOS_UINT32 * pulRealySize)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_INT32  slReadlen = 0;

    if (UDI_INVALID_HANDLE != g_stDmsMainInfo.stPhyProperty[enCOM].slPortHandle)
    {
        DMS_DEBUG_SDM_FUN(EN_SDM_DMS_UDI_READ_START, (VOS_UINT32)slReadlen, (VOS_UINT32)ulToReadSize, (VOS_UINT32)ucRedBuf);

        slReadlen = udi_read(g_stDmsMainInfo.stPhyProperty[enCOM].slPortHandle,ucRedBuf,ulToReadSize);

        DMS_DEBUG_SDM_FUN(EN_SDM_DMS_UDI_READ_END, (VOS_UINT32)slReadlen, (VOS_UINT32)ulToReadSize, (VOS_UINT32)ucRedBuf);

        /*�����豸ʧ�ܵķ���ֵ���Ƕ���-1*/
        if (slReadlen < 0 )
        {
            * pulRealySize = 0;
            ret = ERR_MSP_FAILURE;
        }
        else
        {
            * pulRealySize = (VOS_UINT32)slReadlen;
        }
    }
    else
    {
        ret = ERR_MSP_FAILURE;
    }
    return ret;
}


/*****************************************************************************
 �� �� ��  : dms_GetPortHandle
 ��������  : �ж�ͨ��handle�Ƿ���Ч
 �������  : enChan: ͨ��ID

 �������  :
 �� �� ֵ  : FALSE/TRUE
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL dms_GetPortHandle(DMS_PHY_BEAR_ENUM enChan)
{
    if (UDI_INVALID_HANDLE == g_stDmsMainInfo.stPhyProperty[enChan].slPortHandle)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/*****************************************************************************
 �� �� ��  : dmsAtPcuiTaskSetSleepFlag
 ��������  :
 �������  :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_VOID dmsAtPcuiTaskSetSleepFlag(VOS_BOOL ucSleepFlag)
{
    DMS_MAIN_INFO* pstMainInfo = dmsGetMainInfo();

    pstMainInfo->ucDmsVcomATSleepFlag = ucSleepFlag;
    return ;
}

/*****************************************************************************
 �� �� ��  : dmsAtPcuiTaskGetSleepFlag
 ��������  :
 �������  :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL dmsAtPcuiTaskGetSleepFlag(VOS_VOID)
{
    DMS_MAIN_INFO* pstMainInfo = dmsGetMainInfo();

    return  pstMainInfo->ucDmsVcomATSleepFlag ;

}

/*****************************************************************************
 �� �� ��  : dmsAtCtrlTaskSetSleepFlag
 ��������  :
 �������  :

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_VOID dmsAtCtrlTaskSetSleepFlag(VOS_BOOL ucSleepFlag)
{
    DMS_MAIN_INFO* pstMainInfo = dmsGetMainInfo();

    pstMainInfo->ucDmsVcomUartSleepFlag = ucSleepFlag;

    return ;
}

/*****************************************************************************
 �� �� ��  : dmsAtCtrlTaskGetSleepFlag
 ��������  :
 �������  :

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/

VOS_BOOL dmsAtCtrlTaskGetSleepFlag(VOS_VOID)
{
    DMS_MAIN_INFO* pstMainInfo = dmsGetMainInfo();

    return  pstMainInfo->ucDmsVcomUartSleepFlag ;

}


extern VOS_VOID   At_MsgProc(MsgBlock* pMsg);
extern VOS_UINT32 At_PidInit(enum VOS_INIT_PHASE_DEFINE enPhase);

/*****************************************************************************
 �� �� ��  : DMS_DsFidInit
 ��������  : dms FID ��ʼ������
 �������  :

 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2012��8��27��
     ��    ��  : heliping
     �޸�����  : Creat Function
*****************************************************************************/
VOS_UINT32 DMS_DsFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ulRelVal = 0;

    switch (ip)
    {
    case VOS_IP_LOAD_CONFIG:

        dms_Init();

        ulRelVal = VOS_RegisterPIDInfo(WUEPS_PID_AT, (Init_Fun_Type) At_PidInit, (Msg_Fun_Type) At_MsgProc);

        if (ulRelVal != VOS_OK)
        {
            return VOS_ERR;
        }

        ulRelVal = VOS_RegisterMsgTaskPrio(MSP_APP_DS_FID, VOS_PRIORITY_M2);
        if (ulRelVal != VOS_OK)
        {
            return VOS_ERR;
        }


        ulRelVal = VOS_RegisterSelfTask(MSP_APP_DS_FID, (VOS_TASK_ENTRY_TYPE)dms_AtPcuiTask, VOS_PRIORITY_M2, 8196);    /*lint !e64 */
        if (VOS_NULL_BYTE  == ulRelVal)
        {
            return VOS_ERR;
        }

        /*CTRL�Դ�������ע�� */
        ulRelVal = VOS_RegisterSelfTask(MSP_APP_DS_FID, (VOS_TASK_ENTRY_TYPE)dms_VcomCtrlAtTask, VOS_PRIORITY_M2,8196);    /*lint !e64 */
        if (VOS_NULL_BYTE  == ulRelVal)
        {
            return VOS_ERR;
        }

        break;

    default:
        break;
    }

    if(ulRelVal == 0)
    {

    }
    return VOS_OK;
}

/* Added by z60575 for logtool, 2013-10-25, begin */

VOS_INT DMS_InitPorCfgFile(VOS_VOID)
{
    g_stDmsMainInfo.bPortCfgFlg     = FALSE;
    g_stDmsMainInfo.bPortOpenFlg    = FALSE;
    g_stDmsMainInfo.ulPortCfgValue  = DMS_TEST_MODE;

/*lint -e960 */
    if(VOS_NULL_PTR == proc_create("portcfg", DMS_VFILE_CRT_LEVEL, VOS_NULL_PTR, &g_stPortCfgOps))
    {
        LogPrint("\r\n DMS_InitPorCfgFile: proc_create Return NULL \r\n");

        return VOS_ERR;
    }
/*lint +e960 */

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : DMS_ReadPortCfgFile
 ��������  : PortCfg�����ļ���ʵ��
 �������  : file --- �ļ����
             buf  --- �û��ռ�
             ppos --- �ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
  1.��    ��   : 2013��10��25��
    ��    ��   : z6057
    �޸�����   : �����ɺ���

*****************************************************************************/
ssize_t DMS_ReadPortCfgFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_CHAR                            acModeTemp[DMS_PORTCFG_FILE_LEN];
    VOS_INT32                           lRlst;
    VOS_UINT32                          ulLength;
    if (*ppos > 0)
    {
        return 0;
    }

    VOS_MemSet(acModeTemp, 0x00, DMS_PORTCFG_FILE_LEN);

    VOS_sprintf((VOS_CHAR *)acModeTemp, "%d", g_stDmsMainInfo.ulPortCfgValue);

    ulLength        = VOS_StrLen(acModeTemp);
    len             = PS_MIN(len, ulLength);

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst           = copy_to_user(buf,(VOS_VOID *)acModeTemp, len);

    if (lRlst < 0)
    {
        return -EPERM;
    }

    *ppos += (loff_t)len;

    return (ssize_t)len;
}

/*****************************************************************************
 �� �� ��  : DMS_WritePortCfgFile
 ��������  : PortCfg�����ļ�дʵ��
 �������  : file ----- �ļ����
             buf  ----- �û��ռ�����
             lLength -- �û����ݳ���
             ppos - ----�ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
  1.��    ��   : 2013��10��25��
    ��    ��   : z6057
    �޸�����   : �����ɺ���

*****************************************************************************/
ssize_t DMS_WritePortCfgFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_CHAR                            acModeTemp[DMS_PORTCFG_FILE_LEN];
    VOS_UINT32                          ulRlst;
    VOS_UINT32                          ulStrLen;
    VOS_UINT32                          i;
    VOS_UINT32                          ulValue;

    ulValue = 0;
    VOS_MemSet(acModeTemp, 0x00, DMS_PORTCFG_FILE_LEN);

    if (len >= DMS_PORTCFG_FILE_LEN)
    {
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    ulRlst = copy_from_user((VOS_VOID *)acModeTemp, (VOS_VOID *)buf, len);
    if (ulRlst > 0)
    {
        return -EFAULT;
    }

    acModeTemp[len] = '\0';

    ulStrLen = VOS_StrLen(acModeTemp);

    for ( i = 0; i < ulStrLen; i++ )
    {
        if ( (acModeTemp[i] >= '0') && (acModeTemp[i] <= '9') )
        {
            ulValue = (ulValue * 10) + (acModeTemp[i] - '0');
        }
    }

    g_stDmsMainInfo.ulPortCfgValue  = ulValue;

    /* ����Ѿ�д������ļ�������д��ֵΪ׼����������Ҫ�ٶ�NVE */
    g_stDmsMainInfo.bPortCfgFlg     = TRUE;

    return (ssize_t)len;
}

/*****************************************************************************
 �� �� ��  : DMS_ShowFlag
 ��������  : show DMS��Ϣ
 �������  : ��

 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
  1.��    ��   : 2013��10��25��
    ��    ��   : z6057
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID  DMS_ShowFlag(VOS_VOID)
{
    vos_printf("ulPortCfgValue: %d\r\n", g_stDmsMainInfo.ulPortCfgValue);
    vos_printf("bNveReadFlg: %d\r\n", g_stDmsMainInfo.bPortCfgFlg);
    vos_printf("bPortOpenFlg: %d\r\n", g_stDmsMainInfo.bPortOpenFlg);

    return;
}

/* Added by z60575 for logtool, 2013-10-25, end */

#if (VOS_LINUX == VOS_OS_VER)
#if (FEATURE_ON == FEATURE_DELAY_MODEM_INIT)
module_init(DMS_InitPorCfgFile);
#endif
#endif



