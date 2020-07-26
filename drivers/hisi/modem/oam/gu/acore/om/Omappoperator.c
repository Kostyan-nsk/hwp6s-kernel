/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��      : OmappOperator.c
  �� �� ��      : ����
  ��    ��      : jinni168360
  ��������      : 2012��08��08��
  ����޸�      :
  ��������      : ��C�ļ�������OmappOperatorģ���ʵ��
  �����б�      :
  �޸���ʷ      :
  1.��    ��    : 2012��08��08��
    ��    ��    : jinni168360
    �޸�����    : �����ļ�

******************************************************************************/
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "omappoperator.h"
#include "OmApp.h"
#include "OmAppRl.h"
#include "omprintf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/*lint -e767 �޸���:���� 47350;ԭ��:Log��ӡ */
#define    THIS_FILE_ID        PS_FILE_ID_ACPU_OMOPERATOR_C
/*lint +e767 �޸���:���� 47350;*/


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
#if (FEATURE_ON == FEATURE_CBT_LOG)
extern VOS_UINT32                     g_ulCBTLogEnable;
#endif

/*****************************************************************************
  3 ��������
*****************************************************************************/

VOS_UINT32 OM_AcpuEstablishReq(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32                          ulRet;
#if (FEATURE_ON == FEATURE_CBT_LOG)    
    VOS_UINT32                          ulState;

    ulState = g_ulAcpuOMSwitchOnOff;
#endif

    VOS_MemSet(&g_stAcpuTraceEventConfig, 0, sizeof(OM_TRACE_EVENT_CONFIG_PS_STRU));
    VOS_MemSet(g_astOmPrintfOutPutLev, 0, OM_PRINTF_MODULE_MAX_NUM*sizeof(LOG_LEVEL_EN));
    VOS_MemSet(g_aulLogPrintLevPsTable, 0, LOG_PS_MODULE_MAX_NUM*sizeof(LOG_LEVEL_EN));
    VOS_MemSet(g_aulLogPrintLevDrvTable, 0, LOG_DRV_MODULE_MAX_NUM*sizeof(LOG_LEVEL_EN));

    /* �ȶϿ���· */
    g_ulAcpuOMSwitchOnOff = OM_STATE_IDLE;

    /* �Լ���У׼���ߣ������ɹ��ظ�״̬��0x02 */
    ulRet = 0x02;

    /* �����߻ظ������ɹ�״̬ */
    OM_AcpuSendResult(OM_QUERY_FUNC, ulRet, usReturnPrimId);

    /* ������· */
    g_ulAcpuOMSwitchOnOff = OM_STATE_ACTIVE;

    /* ���ڶ�λLOGTOOLץ����LOGû��C��LOG�����⣬����V9ʹ�ã�V8ɾ�� */
    if (VOS_OK != OM_AcpuConnectInfoEventTimerStart())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_AcpuInit: OM_AcpuStartConnectEventTimerStart failed.\n");
    }

#if (FEATURE_ON == FEATURE_CBT_LOG)
    if ((OM_STATE_ACTIVE == ulState) && (OM_CBT_LOG_ENABLE_FLAG == g_ulCBTLogEnable))
    {
        return VOS_OK;
    }
#endif

    /* ֪ͨCCPU��·״̬ */
    if(VOS_OK != GU_OamSndPcMsgToCcpu((VOS_UINT8*)pstAppToOmMsg, sizeof(APP_OM_MSG_EX_STRU)))
    {
        /* ��ӡ���� */
    }

    return VOS_OK;
}
VOS_VOID OM_NotifyOtherCpuReleaseLink(VOS_VOID)
{
    VOS_UINT32                          ulMsgId;

    ulMsgId                     = APP_OM_RELEASE_REQ;

    /* Ŀǰ���������ͨ�Ż������⣬��˳��˸�CCPU�������˲����Ͷ�����Ϣ������A�˻Ḵλ begin */
    /* ��HIFI֪ͨ��·�Ͽ���Ϣ */
    OMRL_AcpuSendMsg((VOS_UINT8*)&ulMsgId, sizeof(VOS_UINT32), ACPU_PID_OM, DSP_PID_HIFI_OM);

    /* ��MCU֪ͨ��·�Ͽ���Ϣ */
    /* OMRL_AcpuSendMsg((VOS_UINT8*)&ulMsgId, sizeof(VOS_UINT32), ACPU_PID_OM, ACPU_PID_MCU_OM); */
    /* Ŀǰ���������ͨ�Ż������⣬��˳��˸�CCPU�������˲����Ͷ�����Ϣ������A�˻Ḵλ end */
    return;
}
VOS_UINT32 OM_AcpuReleaseReq(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{

#if (FEATURE_ON == FEATURE_CBT_LOG)
    if (OM_CBT_LOG_ENABLE_FLAG == g_ulCBTLogEnable)
    {
        return VOS_OK;
    }
#endif

    g_ulAcpuOMSwitchOnOff       = OM_STATE_IDLE;

    /* ͨ��ICCͨ��֪ͨCCPU��·�Ͽ� */
    if(VOS_OK != GU_OamSndPcMsgToCcpu((VOS_UINT8*)pstAppToOmMsg, sizeof(APP_OM_MSG_EX_STRU)))
    {
        /* ��ӡ���� */
        LogPrint("OM_AcpuReleaseReq: The ICC UDI Write is Error.\n");
    }

    OM_NotifyOtherCpuReleaseLink();

    g_stAcpuPcToUeSucRecord.stRlsData.ulDataLen = OM_GetSlice();
    g_stAcpuPcToUeSucRecord.stRlsData.ulNum++;

    return VOS_OK;
}

/*****************************************************************************
  4 ��Ϣ������ӳ���
*****************************************************************************/
/*Global map table used to find the function according the PrimId.*/
OM_MSG_FUN_STRU g_astAcpuOmMsgFunTbl[] =
{
    {APP_OM_ESTABLISH_REQ,              OM_AcpuEstablishReq,              OM_APP_ESTABLISH_CNF},
    {APP_OM_RELEASE_REQ,                OM_AcpuReleaseReq,                OM_APP_RELEASE_CNF}
};
VOS_VOID OM_AcpuQueryMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    APP_OM_MSG_EX_STRU                 *pstAppToOmMsg;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulTotalNum;
    VOS_UINT16                          usPrimId;
    VOS_UINT16                          usReturnPrimId;
    VOS_UINT32                          ulResult = VOS_ERR;

    pstAppToOmMsg = (APP_OM_MSG_EX_STRU*)pRspPacket;
    ulTotalNum = sizeof(g_astAcpuOmMsgFunTbl)/sizeof(OM_MSG_FUN_STRU);
    usPrimId = pstAppToOmMsg->usPrimId;
    /*Search the corresponding function and return PrimId*/
    for (ulIndex = 0; ulIndex < ulTotalNum; ulIndex++)
    {
        if (usPrimId == (VOS_UINT16)(g_astAcpuOmMsgFunTbl[ulIndex].ulPrimId))
        {
            usReturnPrimId = (VOS_UINT16)(g_astAcpuOmMsgFunTbl[ulIndex].ulReturnPrimId);
            ulResult = g_astAcpuOmMsgFunTbl[ulIndex].pfFun(pstAppToOmMsg, usReturnPrimId);
            break;
        }
    }

    /*Can't find the function handles the usPrimId.*/
    if (ulIndex == ulTotalNum)
    {
        OM_AcpuSendResult(pstAppToOmMsg->ucFuncType, ulResult, usPrimId);
    }
    return;
}


/*****************************************************************************
 �� �� ��  : OmAcpuShowOnOff
 ��������  : �鿴��·����״̬
 �������  :
 �������  :
 �� �� ֵ  :
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2009��5��18��
     ��    ��  : g47350
     �޸�����  : Creat Function
*****************************************************************************/
VOS_VOID OmAcpuShowOnOff(VOS_VOID)
{
    vos_printf("g_ulAcpuOMSwitchOnOff = %d\r\n",g_ulAcpuOMSwitchOnOff);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

