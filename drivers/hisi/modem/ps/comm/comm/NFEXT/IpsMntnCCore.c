/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : IpsMntnCCore.c
  �� �� ��   : ����
  ��    ��   : caikai
  ��������   : 2013��7��22��
  ����޸�   :
  ��������   : IPS MNTNģ��ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��7��22��
    ��    ��   : caikai
    �޸�����   : �����ļ�

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "IpsMntnCCore.h"
#include "vos.h"

#if( FEATURE_ON == FEATURE_IMS )

#include "ImsNicInterface.h"
#include "om.h"
#include "PsCommonDef.h"
#include "TTFComm.h"
#include "OmToolInterface.h"





/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_IPS_MNTN_CCORE_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU          g_stIpsCcoreTraceMsgCfg;     /*TCP/IPЭ��ջ��ά�ɲ�������Ϣ*/
IPS_MNTN_CCORE_INFO_ENTITY                    g_stIpsCcoreMntnInfoEntity;  /*TCP/IPЭ��ջ��ά�ɲ�ʵ��*/
/* �����������ϱ����ڴ� */
VOS_UINT8                                     g_aucIpsCCoreInfo[IPS_CCORE_INFO_LEN] = {0};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_VOID IPS_MNTN_CCORE_TransMsg
(
    VOS_UINT8                                       *pucTransMsg,
    VOS_UINT32                                       ulTransMsgContentLen,
    IPS_MNTN_CCORE_TRACE_MSG_TYPE_ENUM_UINT16        enMsgName
)
{
    VOS_UINT32                          ulTraceMsgLen;
    OM_APP_TRACE_STRU                  *pstAppTrace = VOS_NULL_PTR;

    if (VOS_NULL_PTR == pucTransMsg)
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_TransMsg, ERROR, pucTransMsg is NULL ");
        return;
    }

    /*������Ϣ*/
    ulTraceMsgLen   = IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN + ulTransMsgContentLen;
    pstAppTrace     = (OM_APP_TRACE_STRU *)pucTransMsg;

    /* ���ͷ����ı�����UT������Ҫ */
    PS_MEM_SET(&pstAppTrace->stAppHeader, 0, sizeof(OM_APP_HEADER_STRU));

    /* �����Ϣ */
    pstAppTrace->stAppHeader.ucFuncType = OM_TRANS_FUNC;
    pstAppTrace->stAppHeader.usLength   = (VOS_UINT16)(ulTraceMsgLen - VOS_OM_HEADER_LEN);

    OM_AddSNTime(&(pstAppTrace->stAppHeader.ulSn),
                    &(pstAppTrace->stAppHeader.ulTimeStamp));
    pstAppTrace->usPrimId   = enMsgName;
    pstAppTrace->usToolId   = 0;

    /* ����͸����Ϣ */
    OM_SendData((OM_RSP_PACKET_STRU*)pucTransMsg, (VOS_UINT16)ulTraceMsgLen);

    return;
}


VOS_VOID IPS_MNTN_CCORE_SndCfgCnf2Om
(
    VOS_UINT16      usPrimId,
    VOS_UINT16      usTransMsgContentLen,
    VOS_VOID       *pTransMsgContent
)
{
    VOS_UINT16                   usIpsMntnToOmMsgLen;
    PS_OM_TRANS_IND_STRU        *pstIpsMntnToOmMsg = VOS_NULL_PTR;

    /*������Ϣ*/
    usIpsMntnToOmMsgLen = IPS_CCORE_PS_OM_TRANS_DATA_OFFSET_LEN + usTransMsgContentLen;
    pstIpsMntnToOmMsg   = (PS_OM_TRANS_IND_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(UEPS_PID_IPS_CCORE, usIpsMntnToOmMsgLen);

    if (VOS_NULL_PTR == pstIpsMntnToOmMsg)
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_SndCfgCnf2Om, ERROR, Call PS_ALLOC_MSG_WITH_HEADER_LEN \
            fail!");
        return;
    }

    /* ��� �� OM��͸����Ϣ */
    IPS_CCORE_FILL_MSG_HEADER(pstIpsMntnToOmMsg, WUEPS_PID_OM);

    pstIpsMntnToOmMsg->usTransPrimId    = OM_TRANS_PRIMID;          /*͸����Ϣ�̶���Ϊ0x5001*/
    pstIpsMntnToOmMsg->ucFuncType       = OM_TRANS_FUNC;            /*͸������*/
    pstIpsMntnToOmMsg->usAppLength      = usIpsMntnToOmMsgLen - IPS_CCORE_PS_OM_TRANS_OM_HEADER_LEN; /*��ȥusAppLength����֮ǰ��ie����*/
    pstIpsMntnToOmMsg->ulSn             = 0;                        /*��ʱ��0*/
    pstIpsMntnToOmMsg->ulTimeStamp      = OM_GetSlice();
    pstIpsMntnToOmMsg->usPrimId         = usPrimId;                 /*����������ϢID��д*/
    pstIpsMntnToOmMsg->usToolsId        = 0;                        /*����ID,Ŀǰ��û��ʹ��*/

    /* ���ȷ����Ϣ */
    if ( (0 != usTransMsgContentLen) && (VOS_NULL_PTR != pTransMsgContent) )
    {
        PS_MEM_CPY(pstIpsMntnToOmMsg->aucData, pTransMsgContent, usTransMsgContentLen);
    }

    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_IPS_CCORE, pstIpsMntnToOmMsg) )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_SndCfgCnf2Om, ERROR, Call PS_SEND_MSG fail!");
    }

    return;
}
VOS_UINT32 IPS_MNTN_CCORE_ChkMntnInfoIsStart(VOS_VOID)
{
    /*�ж϶�ʱ���Ƿ�����*/
    if(IPS_MNTN_CCORE_INFO_REPORT_START == g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag)
    {
        return PS_TRUE;
    }
    else if(IPS_MNTN_CCORE_INFO_REPORT_STOP == g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag)
    {
        return PS_FALSE;
    }
    else
    {
        return PS_FALSE;
    }
}
VOS_UINT32 IPS_MNTN_CCORE_StartRelTimer
(
    HTIMER                         *phTm,
    VOS_PID                         Pid,
    VOS_UINT32                      ulLength,
    VOS_UINT32                      ulName,
    VOS_UINT32                      ulParam,
    VOS_UINT8                       ucMode,
    VOS_TIMER_PRECISION_ENUM_UINT32 enPrecision
)
{
    VOS_UINT32      ulErrCode;

    /*=============*/   /*��ʱ���Ѿ�����������������*/
    if (VOS_NULL_PTR != *phTm)
    {
        ulErrCode = VOS_RestartRelTimer(phTm);

        if ( VOS_OK != ulErrCode )
        {
            PS_LOG1(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_StartRelTimer, ERROR, Call VOS_RestartRelTimer fail!\
                ulErrCode:<1>", (VOS_INT32)ulErrCode);
            return PS_FAIL;
        }
    }
    /*=============*/   /*��ʱ��δ��������������ʱ��*/
    else
    {
        ulErrCode = VOS_StartRelTimer(phTm, Pid, ulLength, ulName, ulParam, ucMode, enPrecision);

        if ( VOS_OK != ulErrCode )
        {
            PS_LOG1(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_StartRelTimer, ERROR, Call VOS_StartRelTimer fail!\
                 ulErrCode:<1>", (VOS_INT32)ulErrCode);
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}


VOS_VOID IPS_MNTN_CCORE_StopMntnInfoTimer(VOS_VOID)
{
    VOS_UINT32  ulErrCode;
    /*�ж϶�ʱ��ָ���Ƿ�Ϊ��*/
    if ( VOS_NULL_PTR != g_stIpsCcoreMntnInfoEntity.hIpsMntnTimer )
    {
        /*�����Ϊ�գ���ֹͣ��ʱ��*/
        ulErrCode = VOS_StopRelTimer(&g_stIpsCcoreMntnInfoEntity.hIpsMntnTimer);

        if ( VOS_OK != ulErrCode )
        {
            g_stIpsCcoreMntnInfoEntity.hIpsMntnTimer = VOS_NULL_PTR;

            PS_LOG1(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_StopMntnInfoTimer, ERROR, Call VOS_StopRelTimer \
                fail! ulErrCode:<1>", (VOS_INT32)ulErrCode);
        }
    }
}


VOS_UINT32 IPS_MNTN_CCORE_GetPktLenByTraceCfg
(
    IPS_MNTN_CCORE_TRACE_CONFIG_STRU     *pstTraceCfg,
    VOS_UINT16                            usPktLen,
    VOS_UINT32                           *pulDataLen
)
{
    *pulDataLen    =   0;

    /*�ж϶�ʱ���Ƿ�����*/
    switch ( pstTraceCfg->ulChoice )
    {
        case IPS_MNTN_CCORE_TRACE_NULL_CHOSEN:
            return PS_FALSE;
        case IPS_MNTN_CCORE_TRACE_MSG_HEADER_CHOSEN:
            break;
        case IPS_MNTN_CCORE_TRACE_CONFIGURABLE_LEN_CHOSEN:
            *pulDataLen = TTF_MIN(pstTraceCfg->ulTraceDataLen, usPktLen);
            break;
        case IPS_MNTN_CCORE_TRACE_WHOLE_DATA_LEN_CHOSEN:
            *pulDataLen = usPktLen;
            break;
        default:
            return PS_FALSE;
    }

    return PS_TRUE;
}
VOS_VOID IPS_MNTN_CCORE_PktInfo
(
    VOS_UINT8                                    *pucNetIfName,
    VOS_UINT8                                    *pucPktData,
    VOS_UINT16                                    usPktLen,
    IPS_MNTN_CCORE_TRACE_MSG_TYPE_ENUM_UINT16     usType
)
{
    VOS_UINT8                                *pucTraceData;
    IPS_MNTN_CCORE_TRACE_CONFIG_STRU         *pstTraceCfg;
    IPS_MNTN_CCORE_PKT_INFO_STRU             *pstTraceMsg;
    VOS_UINT32                                ulTransDataLen;
    VOS_UINT32                                ulDataLen;
    VOS_UINT32                                ulRst;

    ulTransDataLen  =   0;
    ulDataLen       =   0;

    if ( VOS_NULL_PTR == pucPktData )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_PktInfo, ERROR, paucPktData is NULL ");
        return;
    }

    /*������Ϣ����ѡ���Ӧ������Ϣ*/
    if(ID_IPS_CCORE_TRACE_INPUT_DATA_INFO == usType)
    {
        pstTraceCfg = &(g_stIpsCcoreTraceMsgCfg.stInputTraceCfg);
    }
    else if(ID_IPS_CCORE_TRACE_OUTPUT_DATA_INFO == usType)
    {
        pstTraceCfg = &(g_stIpsCcoreTraceMsgCfg.stOutputTraceCfg);
    }
    else
    {
        /*��ӡ������Ϣ,��Ϣ���Ͳ�ƥ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_PktInfo : ERROR : usType is not Expectes!" );
        return;
    }

    /*������Ϣ������Ϣ����ȡ������ĳ���*/
    ulRst = IPS_MNTN_CCORE_GetPktLenByTraceCfg(pstTraceCfg, usPktLen, &ulDataLen);
    if(PS_FALSE == ulRst)
    {
        /*������ñ���*/
        return;
    }

    ulDataLen           = PS_MIN(IPS_MNTN_CCORE_TRACE_MAX_BYTE_LEN, ulDataLen);
    ulTransDataLen      = IPS_CCORE_PKT_INFO_OFFSET_LEN + ulDataLen;

    pucTraceData        = PS_MEM_ALLOC(UEPS_PID_IPS_CCORE, ulTransDataLen + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    if (VOS_NULL_PTR == pucTraceData)
    {
        /*��ӡ������Ϣ,�����ڴ�ʧ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_PktInfo : ERROR : alloc mem failed!" );

        return;
    }

    /*����͸����Ϣ*/
    pstTraceMsg         = (IPS_MNTN_CCORE_PKT_INFO_STRU *)(pucTraceData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    pstTraceMsg->usLen  = usPktLen;
    PS_MEM_CPY(pstTraceMsg->aucNetIfName, pucNetIfName, IPS_CCORE_IFNAMSIZ);
    /*lint -e669 */
    PS_MEM_CPY(pstTraceMsg->aucData, pucPktData, ulDataLen);
    /*lint +e669 */

    IPS_MNTN_CCORE_TransMsg(pucTraceData, ulTransDataLen, usType);

    PS_MEM_FREE(UEPS_PID_IPS_CCORE, pucTraceData);

}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_PktInfoCB
 ��������  : ����TCP/IPЭ��ջ���ձ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_InputPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8               *pucNetIfName;
    VOS_UINT8               *pucPktData;
    VOS_UINT16               usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_InputPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_PktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_INPUT_DATA_INFO);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_OutputPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���ͱ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_OutputPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8               *pucNetIfName;
    VOS_UINT8               *pucPktData;
    VOS_UINT16               usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_OutputPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_PktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_OUTPUT_DATA_INFO);

    return;
}


VOS_VOID IPS_MNTN_CCORE_BridgePktInfo
(
    VOS_UINT8                                   *pucSrcPort,
    VOS_UINT8                                   *pucDestPort,
    VOS_UINT8                                   *pucPktData,
    VOS_UINT16                                   usPktLen,
    IPS_MNTN_CCORE_TRACE_MSG_TYPE_ENUM_UINT16    usType
)
{
    VOS_UINT8                                *pucTraceData;
    IPS_MNTN_CCORE_TRACE_CONFIG_STRU         *pstTraceCfg;
    IPS_MNTN_CCORE_BRIDGE_PKT_INFO_STRU      *pstTraceMsg;
    VOS_UINT32                                ulTransDataLen;
    VOS_UINT32                                ulDataLen;
    VOS_UINT32                                ulRst;

    ulTransDataLen  =   0;
    ulDataLen       =   0;

    if ( VOS_NULL_PTR == pucPktData )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_BridgePktInfo, ERROR, paucPktData is NULL ");
        return;
    }

    /*������Ϣ����ѡ���Ӧ������Ϣ*/
    if(ID_IPS_CCORE_TRACE_BRIDGE_DATA_INFO == usType)
    {
        pstTraceCfg = &(g_stIpsCcoreTraceMsgCfg.stBridgeTraceCfg);
    }
    else
    {
        /*��ӡ������Ϣ,��Ϣ���Ͳ�ƥ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_BridgePktInfo : ERROR : usType is not Expectes!" );
        return;
    }

    /*������Ϣ������Ϣ����ȡ������ĳ���*/
    ulRst = IPS_MNTN_CCORE_GetPktLenByTraceCfg(pstTraceCfg, usPktLen, &ulDataLen);
    if(PS_FALSE == ulRst)
    {
        /*������ñ���*/
        return;
    }

    ulDataLen           = PS_MIN(IPS_MNTN_CCORE_TRACE_MAX_BYTE_LEN, ulDataLen);
    ulTransDataLen      = (IPS_CCORE_BRIDGE_PKT_INFO_OFFSET_LEN + ulDataLen);

    pucTraceData        = PS_MEM_ALLOC(UEPS_PID_IPS_CCORE, ulTransDataLen + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    if (VOS_NULL_PTR == pucTraceData)
    {
        /*��ӡ������Ϣ,�����ڴ�ʧ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_BridgePktInfo : ERROR : alloc mem failed!" );

        return;
    }

    /*����͸����Ϣ*/
    pstTraceMsg = (IPS_MNTN_CCORE_BRIDGE_PKT_INFO_STRU *)(pucTraceData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    pstTraceMsg->usLen  = usPktLen;
    PS_MEM_CPY(pstTraceMsg->aucSrcPort, pucSrcPort, IPS_CCORE_END_NAME);
    PS_MEM_CPY(pstTraceMsg->aucDestPort, pucDestPort, IPS_CCORE_END_NAME);
    /*lint -e669 */
    PS_MEM_CPY(pstTraceMsg->aucData, pucPktData, ulDataLen);
    /*lint +e669 */

    IPS_MNTN_CCORE_TransMsg(pucTraceData, ulTransDataLen, usType);

    PS_MEM_FREE(UEPS_PID_IPS_CCORE, pucTraceData);
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_BridgePktInfoCB
 ��������  : ����������ת���Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_BridgePktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                   *pucSrcPort;
    VOS_UINT8                                   *pucDestPort;
    VOS_UINT8                                   *pucPktData;
    VOS_UINT16                                   usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_BridgePktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucSrcPort      = pstIpsMntnCapturePara->paucSrcPort;
    pucDestPort     = pstIpsMntnCapturePara->paucDestPort;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_BridgePktInfo(pucSrcPort, pucDestPort, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_BRIDGE_DATA_INFO);

    return;
}


VOS_VOID IPS_MNTN_CCORE_CtrlPktInfo
(
    VOS_UINT8                                     *pucNetIfName,
    VOS_UINT8                                     *pucPktData,
    VOS_UINT16                                     usPktLen,
    IPS_MNTN_CCORE_TRACE_MSG_TYPE_ENUM_UINT16      usType
)
{
    VOS_UINT8                                *pucTraceData;
    IPS_MNTN_CCORE_PKT_INFO_STRU             *pstTraceMsg;
    VOS_UINT32                                ulTransDataLen;
    VOS_UINT32                                ulDataLen;

    ulTransDataLen  =   0;
    ulDataLen       =   0;

    if ( VOS_NULL_PTR == pucPktData )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_CtrlPktInfo, ERROR, pucPktData is NULL ");
        return;
    }

    ulDataLen           = PS_MIN(IPS_MNTN_CCORE_TRACE_MAX_BYTE_LEN, usPktLen);
    ulTransDataLen      = (IPS_CCORE_PKT_INFO_OFFSET_LEN + ulDataLen);

    pucTraceData        = PS_MEM_ALLOC(UEPS_PID_IPS_CCORE, ulTransDataLen + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    if (VOS_NULL_PTR == pucTraceData)
    {
        /*��ӡ������Ϣ,�����ڴ�ʧ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_CtrlPktInfo : ERROR : alloc mem failed!" );

        return;
    }

    /*����͸����Ϣ*/
    pstTraceMsg = (IPS_MNTN_CCORE_PKT_INFO_STRU *)(pucTraceData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    pstTraceMsg->usLen  = usPktLen;
    PS_MEM_CPY(pstTraceMsg->aucNetIfName, pucNetIfName, IPS_CCORE_IFNAMSIZ);
    /*lint -e669 */
    PS_MEM_CPY(pstTraceMsg->aucData, pucPktData, ulDataLen);
    /*lint +e669 */

    IPS_MNTN_CCORE_TransMsg(pucTraceData, ulTransDataLen, usType);

    PS_MEM_FREE(UEPS_PID_IPS_CCORE, pucTraceData);
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_ArpRecvPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���յ�ARP���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_ArpRecvPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_ArpRecvPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_RECV_ARP_PKT);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_ArpSendPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���͵�ARP���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_ArpSendPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_ArpSendPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_SEND_ARP_PKT);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_DhcpcRecvPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���յ�DHCP���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_DhcpcRecvPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_DhcpcRecvPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_RECV_DHCPC_PKT);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_DhcpcSendPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���͵�DHCP���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_DhcpcSendPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_DhcpcSendPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_SEND_DHCPC_PKT);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_DhcpsRecvktInfoCB
 ��������  : ����TCP/IPЭ��ջ���յ�DHCPS���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_DhcpsRecvPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_DhcpsRecvktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_RECV_DHCPS_PKT);

    return;
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_DhcpsSendPktInfoCB
 ��������  : ����TCP/IPЭ��ջ���͵�DHCPS���Ʊ��Ļص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_DhcpsSendPktInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                                    *pucNetIfName;
    VOS_UINT8                                    *pucPktData;
    VOS_UINT16                                    usPktLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_DhcpsSendPktInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucNetIfName    = pstIpsMntnCapturePara->paucNetIfName;
    pucPktData      = pstIpsMntnCapturePara->pucData;
    usPktLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_CtrlPktInfo(pucNetIfName, pucPktData, usPktLen, ID_IPS_CCORE_TRACE_SEND_DHCPS_PKT);

    return;
}


VOS_VOID IPS_MNTN_CCORE_AppCmdInfo
(
    VOS_UINT8                                    *pucData,
    VOS_UINT16                                    usCmdLen,
    IPS_MNTN_CCORE_TRACE_MSG_TYPE_ENUM_UINT16     usType
)
{
    VOS_UINT8                                *pucTraceData;
    IPS_MNTN_CCORE_APP_CMD_INFO_STRU         *pstTraceMsg;
    VOS_UINT32                                ulTransDataLen;
    VOS_UINT32                                ulDataLen;

    ulTransDataLen  =   0;
    ulDataLen       =   0;

    if ( VOS_NULL_PTR == pucData )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_AppCmdInfo, ERROR, pucData is NULL ");
        return;
    }

    ulDataLen           = PS_MIN(IPS_CCORE_APP_CMD_INFO_LEN, usCmdLen);
    ulTransDataLen      = (IPS_CCORE_APP_CMD_INFO_OFFSET_LEN + ulDataLen);

    pucTraceData        = PS_MEM_ALLOC(UEPS_PID_IPS_CCORE, ulTransDataLen + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    if (VOS_NULL_PTR == pucTraceData)
    {
        /*��ӡ������Ϣ,�����ڴ�ʧ��*/
        PS_LOG( UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_AppCmdInfo : ERROR : alloc mem failed!" );

        return;
    }

    /*����͸����Ϣ*/
    pstTraceMsg = (IPS_MNTN_CCORE_APP_CMD_INFO_STRU *)(pucTraceData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN);
    pstTraceMsg->usLen  = usCmdLen;
    /*lint -e669 */
    PS_MEM_CPY(pstTraceMsg->aucData, pucData, ulDataLen);
    /*lint +e669 */

    IPS_MNTN_CCORE_TransMsg(pucTraceData, ulTransDataLen, usType);

    PS_MEM_FREE(UEPS_PID_IPS_CCORE, pucTraceData);
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_AppCmdInfoCB
 ��������  : ����APP��������ص�����
 �������  : pstIpsMntnCapturePara  �ص����������ṹ
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��8��1��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_AppCmdInfoCB
(
    IPS_MNTN_CCORE_CAPTURE_PARA_STRU *pstIpsMntnCapturePara
)
{
    VOS_UINT8                              *pucData;
    VOS_UINT16                              usCmdLen;

    if ( VOS_NULL_PTR == pstIpsMntnCapturePara )
    {
        PS_LOG(UEPS_PID_IPS_CCORE, PS_SUBMOD_NULL, PS_PRINT_ERROR,
            "IPS, IPS_MNTN_CCORE_AppCmdInfoCB, ERROR, pstIpsMntnCapturePara is NULL ");
        return;
    }

    pucData         = pstIpsMntnCapturePara->pucData;
    usCmdLen        = pstIpsMntnCapturePara->usLen;

    IPS_MNTN_CCORE_AppCmdInfo(pucData, usCmdLen, ID_IPS_CCORE_TRACE_APP_CMD);

    return;
}


PS_BOOL_ENUM_UINT8  IPS_MNTN_CCORE_BridgeTraceCfgChkParam(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU *pRcvMsg)
{
    if ( (IPS_MNTN_CCORE_TRACE_NULL_CHOSEN !=pRcvMsg->stBridgeTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_MSG_HEADER_CHOSEN !=pRcvMsg->stBridgeTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_CONFIGURABLE_LEN_CHOSEN !=pRcvMsg->stBridgeTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_WHOLE_DATA_LEN_CHOSEN !=pRcvMsg->stBridgeTraceCfg.ulChoice) )
    {
        return PS_FALSE;
    }

    return PS_TRUE;
}


PS_BOOL_ENUM_UINT8  IPS_MNTN_CCORE_TraceCfgChkParam(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU *pRcvMsg)
{
    if ( (IPS_MNTN_CCORE_TRACE_NULL_CHOSEN != pRcvMsg->stInputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_MSG_HEADER_CHOSEN != pRcvMsg->stInputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_CONFIGURABLE_LEN_CHOSEN != pRcvMsg->stInputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_WHOLE_DATA_LEN_CHOSEN != pRcvMsg->stInputTraceCfg.ulChoice) )
    {
        return PS_FALSE;
    }

    if ( (IPS_MNTN_CCORE_TRACE_NULL_CHOSEN != pRcvMsg->stOutputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_MSG_HEADER_CHOSEN != pRcvMsg->stOutputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_CONFIGURABLE_LEN_CHOSEN != pRcvMsg->stOutputTraceCfg.ulChoice)
        && (IPS_MNTN_CCORE_TRACE_WHOLE_DATA_LEN_CHOSEN != pRcvMsg->stOutputTraceCfg.ulChoice) )
    {
        return PS_FALSE;
    }

    return PS_TRUE;
}


PS_BOOL_ENUM_UINT8  IPS_MNTN_CCORE_TraceAdvancedCfgChkParam(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU *pRcvMsg)
{
    /*TCP/IPЭ��ջ���Ĺ������ò������*/
    if(PS_FALSE == IPS_MNTN_CCORE_TraceCfgChkParam(pRcvMsg))
    {
        return PS_FALSE;
    }

    /*������ת���Ĺ������ò������*/
    if(PS_FALSE == IPS_MNTN_CCORE_BridgeTraceCfgChkParam(pRcvMsg))
    {
        return  PS_FALSE;
    }

    return PS_TRUE;

}


PS_BOOL_ENUM_UINT8 IPS_MNTN_CCORE_SetCallbackFunc(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU *pstTraceCfgReq)
{
    IPS_MNTN_CCORE_CAPTURE_CB           pBridgePktInfoCb;
    IPS_MNTN_CCORE_CAPTURE_CB           pInputPktInfoCb;
    IPS_MNTN_CCORE_CAPTURE_CB           pOutputPktInfoCb;

    pBridgePktInfoCb         = VOS_NULL_PTR;

    if ( IPS_MNTN_CCORE_TRACE_NULL_CHOSEN != pstTraceCfgReq->stBridgeTraceCfg.ulChoice )
    {
        pBridgePktInfoCb   =   IPS_MNTN_CCORE_BridgePktInfoCB;
    }

    if ((VOS_ERROR) == IPS_MNTN_CCORE_RegExtFunc(pBridgePktInfoCb, IPS_MNTN_CCORE_BRIDGE_DATA_CB))
    {
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_SetCallbackFunc : ERROR : Reg IPS_MNTN_CCORE_BridgePktInfoCB Failed!" );
        return PS_FALSE;
    }

    pInputPktInfoCb          = VOS_NULL_PTR;

    if ( IPS_MNTN_CCORE_TRACE_NULL_CHOSEN != pstTraceCfgReq->stInputTraceCfg.ulChoice )
    {
        pInputPktInfoCb   =   IPS_MNTN_CCORE_InputPktInfoCB;
    }

    if ((VOS_ERROR) == IPS_MNTN_CCORE_RegExtFunc(pInputPktInfoCb, IPS_MNTN_CCORE_INPUT_DATA_CB))
    {
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_SetCallbackFunc : ERROR : Reg IPS_MNTN_CCORE_InputPktInfoCB Failed!" );
        return PS_FALSE;
    }

    pOutputPktInfoCb         = VOS_NULL_PTR;

    if ( IPS_MNTN_CCORE_TRACE_NULL_CHOSEN != pstTraceCfgReq->stOutputTraceCfg.ulChoice )
    {
        pOutputPktInfoCb  =   IPS_MNTN_CCORE_OutputPktInfoCB;
    }

    if ((VOS_ERROR) == IPS_MNTN_CCORE_RegExtFunc(pOutputPktInfoCb, IPS_MNTN_CCORE_OUTPUT_DATA_CB))
    {
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_SetCallbackFunc : ERROR : Reg IPS_MNTN_CCORE_OutputPktInfoCB Failed!" );
        return PS_FALSE;
    }

    return PS_TRUE;
}


VOS_VOID IPS_MNTN_CCORE_TraceAdvancedCfgReq(VOS_VOID *pMsg)
{
    PS_BOOL_ENUM_UINT8                  enResult;
    IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU     *pRcvMsg;
    IPS_MNTN_CCORE_TRACE_CONFIG_CNF_STRU      stIpsTraceCfgCnf;

    pRcvMsg = (IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU *) (((OM_PS_PARAM_REQ_STRU *)pMsg)->aucData);

    /*������ò����Ƿ�Ϸ�*/
    enResult = IPS_MNTN_CCORE_TraceAdvancedCfgChkParam(pRcvMsg);

    if ( PS_FALSE == enResult )
    {
        /*���������ⲻ�Ϸ�����OM�ظ�����ʧ��*/
        stIpsTraceCfgCnf.enRslt =   PS_FAIL;
        IPS_MNTN_CCORE_SndCfgCnf2Om(ID_IPS_CCORE_OM_ADVANCED_TRACE_CONFIG_CNF,
            sizeof(IPS_MNTN_CCORE_TRACE_CONFIG_CNF_STRU), &stIpsTraceCfgCnf );
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
            "Warning: IPS_MNTN_CCORE_TraceAdvancedCfgReq check fail");

        return;
    }

    enResult = IPS_MNTN_CCORE_SetCallbackFunc(pRcvMsg);
    if( PS_FALSE == enResult )
    {
        /*���������ⲻ�Ϸ�����OM�ظ�����ʧ��*/
        stIpsTraceCfgCnf.enRslt =   PS_FAIL;
        IPS_MNTN_CCORE_SndCfgCnf2Om(ID_IPS_CCORE_OM_ADVANCED_TRACE_CONFIG_CNF,
            sizeof(IPS_MNTN_CCORE_TRACE_CONFIG_CNF_STRU), &stIpsTraceCfgCnf );
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
            "Warning: IPS_MNTN_CCORE_SetCallbackFunc fail");

        return;
    }

    /*�������ò���*/
    PS_MEM_CPY(&g_stIpsCcoreTraceMsgCfg, pRcvMsg, sizeof(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU));

    /*��OM�ظ����óɹ�*/
    stIpsTraceCfgCnf.enRslt =   PS_SUCC;
    IPS_MNTN_CCORE_SndCfgCnf2Om(ID_IPS_CCORE_OM_ADVANCED_TRACE_CONFIG_CNF,
            sizeof(IPS_MNTN_CCORE_TRACE_CONFIG_CNF_STRU), &stIpsTraceCfgCnf );

}
VOS_UINT32 IPS_MNTN_CCORE_RcvIpsMntnInfoCfgReq(VOS_VOID *pMsg)
{
    OM_PS_PARAM_REQ_STRU                     *pstOmMsg;
    IPS_MNTN_CCORE_INFO_CFG_REQ_STRU         *pstIpsMntnCfgReq;
    IPS_MNTN_CCORE_INFO_CFG_CNF_STRU          stIpsMntnCfgCnf;
    IPS_MNTN_CCORE_RESULT_TYPE_ENUM_UINT16    enResult;
    VOS_UINT32                                ulIpsMntnTimerLen;

    pstOmMsg            = ( (OM_PS_PARAM_REQ_STRU *)pMsg );
    pstIpsMntnCfgReq    = ( (IPS_MNTN_CCORE_INFO_CFG_REQ_STRU *)(pstOmMsg->aucData) );

    enResult            = IPS_MNTN_CCORE_RESULT_OK;

    /*��������TCP/IPЭ��ջ������Ϣ*/
    if( IPS_MNTN_CCORE_INFO_REPORT_START == pstIpsMntnCfgReq->enCommand )
    {
        /*��Ⲷ��TCP/IPЭ��ջ������Ϣ�����Ƿ���������*/
        if(PS_FALSE == IPS_MNTN_CCORE_ChkMntnInfoIsStart())
        {
            /*���ö�ʱ��ʱ����Ĭ��Ϊ1000����*/
            ulIpsMntnTimerLen = 1000 * pstIpsMntnCfgReq->usTimeLen;

            if (PS_SUCC != IPS_MNTN_CCORE_StartRelTimer(&(g_stIpsCcoreMntnInfoEntity.hIpsMntnTimer), UEPS_PID_IPS_CCORE,
                    ulIpsMntnTimerLen, IPS_MNTN_CCORE_INFO_TIMER, 0,
                    VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION))
            {
                /*������ʱ��ʧ�ܣ���ֹͣ��ʱ����ע����ģ��*/
                IPS_MNTN_CCORE_StopMntnInfoTimer();
                enResult = IPS_MNTN_CCORE_RESULT_START_TIMER_FAIL;

                g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag = IPS_MNTN_CCORE_INFO_REPORT_STOP;
            }
            else
            {
                g_stIpsCcoreMntnInfoEntity.ulIpsMntnTimerLen = ulIpsMntnTimerLen;
                g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag     = IPS_MNTN_CCORE_INFO_REPORT_START;
            }
        }
    }
    /*ֹͣ����TCP/IPЭ��ջ������Ϣ*/
    else if( IPS_MNTN_CCORE_INFO_REPORT_STOP == pstIpsMntnCfgReq->enCommand )
    {
        /*��Ⲷ��TCP/IPЭ��ջ������Ϣ�����Ƿ���������*/
        if(PS_TRUE == IPS_MNTN_CCORE_ChkMntnInfoIsStart())
        {
            /*�����ģ������������ֹͣ��ʱ����ע����ģ��*/
            IPS_MNTN_CCORE_StopMntnInfoTimer();

            g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag = IPS_MNTN_CCORE_INFO_REPORT_STOP;
        }

    }
    /*================*/    /*OM ����Ĳ�����Ч*/
    else
    {
        enResult = IPS_MNTN_CCORE_RESULT_INPUT_PARAM_ERR;
    }

    /*================================*/
    /*�����ظ���Ϣ*/
    /*================================*/

    /* ���ظ�OM�����ȷ����Ϣ */
    stIpsMntnCfgCnf.enCommand       = pstIpsMntnCfgReq->enCommand;
    stIpsMntnCfgCnf.enRslt          = enResult;

    /* ����OM͸����Ϣ */
    IPS_MNTN_CCORE_SndCfgCnf2Om( ID_IPS_CCORE_OM_MNTN_INFO_CONFIG_CNF,
        sizeof(IPS_MNTN_CCORE_INFO_CFG_CNF_STRU), &stIpsMntnCfgCnf );

    return PS_SUCC;
}
VOS_VOID IPS_MNTN_CCORE_OutputIpsInfo
(
    VOS_UINT8                          *pucData,
    VOS_UINT16                          usType
)
{
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                          *pucTransMsg;

    ulDataLen       =   0;
    pucTransMsg     =   pucData;

    /*��ȡTCP/IPЭ��ջ������Ϣ*/
    IPS_MNTN_CCORE_GetIpsInfo((pucData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN), &ulDataLen, usType);
    if ((0 == ulDataLen) || (ulDataLen >IPS_MNTN_CCORE_INFO_LEN))
    {
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_IpsMntnInfoTimeoutProc : ERROR : ID_IPS_MNTN_IFCONFIG_INFO Failed!" );
        return;
    }

    IPS_MNTN_CCORE_TransMsg(pucTransMsg, ulDataLen, ID_IPS_CCORE_TRACE_MNTN_INFO);
}
VOS_VOID IPS_MNTN_CCORE_OutputIpsNatMappingsInfo
(
    VOS_UINT8                          *pucData
)
{
    VOS_INT                                   lSeqNo;
    VOS_UINT32                                ulTransDataLen;
    VOS_UINT32                                ulDataLen;
    VOS_UINT8                                *pucDataCur;
    VOS_UINT8                                *pucTransMsg;
    IPS_MNTN_CCORE_INFO_STRU                 *pstMntnInfo;

    lSeqNo          =   0;
    /*��¼��ʼ�α�,����֮����дTRANSͷ*/
    pucTransMsg     =   pucData;
    /*ƫ��TRANSͷ�ĳ���*/
    pucData         +=  IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN;
    pstMntnInfo     =   (IPS_MNTN_CCORE_INFO_STRU *)pucData;

    for(;;)
    {
        /*ѭ������NAT MAPPING��Ϣ*/
        ulTransDataLen  =   0;
        ulDataLen       =   0;
        pucDataCur      =   pucData;

        /*�����Ϣ����*/
        pstMntnInfo->usType          =  ID_IPS_MNTN_CCORE_NAT_MAPPINGS_INFO;
        pucDataCur                  +=  IPS_MNTN_CCORE_INFO_TYPE_OFFSET;
        ulTransDataLen              +=  IPS_MNTN_CCORE_INFO_TYPE_OFFSET;

        /*��¼�����Ϣ���ȵ��α��ַ*/
        pucDataCur                  +=  IPS_MNTN_CCORE_INFO_LEN_OFFSET;
        ulTransDataLen              +=  IPS_MNTN_CCORE_INFO_LEN_OFFSET;

        while((ulDataLen + IPS_MNTN_CCORE_INFO_TYPE_OFFSET + IPS_MNTN_CCORE_INFO_LEN_OFFSET + IPS_MNTN_CCORE_NAT_MAPPING_INFO_LEN) < IPS_MNTN_CCORE_INFO_LEN)
        {
            /*��ȡTCP/IPЭ��ջ������Ϣ*/
            if (VOS_OK != IPS_MNTN_CCORE_GetNatMappings(pucDataCur, lSeqNo))
            {
                /*�����Ϣ����*/
                pstMntnInfo->ulLen           =   ulDataLen;
                ulTransDataLen              +=  ulDataLen;
                /*NAT��ӳ����Ϣ��ȡ�꣬�����Ϣ*/
                IPS_MNTN_CCORE_TransMsg(pucTransMsg, ulTransDataLen, ID_IPS_CCORE_TRACE_MNTN_INFO);
                return;
            }

            lSeqNo++;
            ulDataLen       += IPS_MNTN_CCORE_NAT_MAPPING_INFO_LEN;
            pucDataCur      += IPS_MNTN_CCORE_NAT_MAPPING_INFO_LEN;
        }

        /*�����Ϣ����*/
        pstMntnInfo->ulLen           =   ulDataLen;
        ulTransDataLen              +=  ulDataLen;
        /*��Ϣ������,�����Ϣ*/
        IPS_MNTN_CCORE_TransMsg(pucTransMsg, ulTransDataLen, ID_IPS_CCORE_TRACE_MNTN_INFO);
    }
}

/*****************************************************************************
 �� �� ��  : IPS_MNTN_CCORE_OutputImsNicStatsInfo
 ��������  : �������������������ͳ����Ϣ
 �������  : pucData    -- �����ڴ�
             usType     -- ��ά�ɲ���Ϣ����
 �������  : ��
 �� �� ֵ  : ��
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IPS_MNTN_CCORE_OutputImsNicStatsInfo
(
    VOS_UINT8                          *pucData,
    VOS_UINT16                          usType
)
{
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                          *pucTransMsg;
    VOS_UINT32                          ulResult;


    ulDataLen       =   0;
    pucTransMsg     =   pucData;

    /* ��ȡIMS����������ͳ����Ϣ */
    ulResult    = IMS_NIC_GetMntnInfo((pucData + IPS_CCORE_OM_TRANS_DATA_OFFSET_LEN), &ulDataLen, usType);

    if ((IMS_NIC_SUCC != ulResult)|| (0 == ulDataLen) || (ulDataLen > IPS_MNTN_CCORE_INFO_LEN))
    {
        PS_LOG1(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_OutputImsNicStatsInfo : ERROR : IMS_NIC_GetMntnInfo  Failed! ulDataLen =%d\n", (long)ulDataLen);
        return;
    }

    IPS_MNTN_CCORE_TransMsg(pucTransMsg, ulDataLen, usType);

    return;
}
VOS_VOID IPS_MNTN_CCORE_IpsMntnInfoTimeoutProc(VOS_VOID)
{
    VOS_UINT8                          *pucTraceData;

    g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag = IPS_MNTN_CCORE_INFO_REPORT_STOP;

    /*Ϊģ��ʵ��ָ���ڴ�*/
    pucTraceData    = g_aucIpsCCoreInfo;

    /*�������������Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_IFCONFIG_INFO);

    /*���·�ɱ���Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_NETSTAT_ROUTES_INFO);

    /*�������״̬��Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_NETSTAT_ENTRIES_INFO);

    /*���NAT�������Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_NAT_RULES_INFO);

    /*���NAT��״̬��Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_NAT_STATS_INFO);

    /*���NAT��Э����Ϣ*/
    IPS_MNTN_CCORE_OutputIpsInfo(pucTraceData, ID_IPS_MNTN_CCORE_NAT_PROXIES_INFO);

    /*���NATӳ����Ϣ*/
    IPS_MNTN_CCORE_OutputIpsNatMappingsInfo(pucTraceData);

    /* ���IMS��������ͳ����Ϣ */
    IPS_MNTN_CCORE_OutputImsNicStatsInfo(pucTraceData, ID_IPS_MNTN_CCORE_IMS_NIC_INFO);

    /*������ά�ɲⶨʱ��*/
    if (PS_SUCC != IPS_MNTN_CCORE_StartRelTimer(&(g_stIpsCcoreMntnInfoEntity.hIpsMntnTimer), UEPS_PID_IPS_CCORE,
                    g_stIpsCcoreMntnInfoEntity.ulIpsMntnTimerLen, IPS_MNTN_CCORE_INFO_TIMER, 0,
                    VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION))
    {
        /*������ʱ��ʧ�ܣ���ֹͣ��ʱ����ע����ģ��*/
        IPS_MNTN_CCORE_StopMntnInfoTimer();
        g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag = IPS_MNTN_CCORE_INFO_REPORT_STOP;
    }
    else
    {
        g_stIpsCcoreMntnInfoEntity.enIpsMntnFlag = IPS_MNTN_CCORE_INFO_REPORT_START;
    }

    return;
}
VOS_VOID IPS_MNTN_CCORE_RcvOmMsgProc(VOS_VOID *pMsg)
{
    VOS_UINT16                          usMsgType;


    usMsgType = *((VOS_UINT16 *)((VOS_UINT8 *)(pMsg) + VOS_MSG_HEAD_LENGTH));

    switch ( usMsgType )
    {
        case ID_OM_IPS_CCORE_ADVANCED_TRACE_CONFIG_REQ:
            IPS_MNTN_CCORE_TraceAdvancedCfgReq(pMsg);
            break;

        case ID_OM_IPS_MNTN_CCORE_INFO_CONFIG_REQ:
            IPS_MNTN_CCORE_RcvIpsMntnInfoCfgReq(pMsg);
            break;

        default:
            PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_RcvOmMsgProc : ERROR : Receive Unkown Type Message !" );
            break;
    }
    return;
}


VOS_VOID IPS_MNTN_CCORE_RcvTimerExpireMsgProc(REL_TIMER_MSG *pTimerMsg)
{
    switch ( pTimerMsg->ulName)
    {
        case IPS_MNTN_CCORE_INFO_TIMER:
            IPS_MNTN_CCORE_IpsMntnInfoTimeoutProc();
            break;

        default:
            PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_RcvTimerExpireMsgProc : ERROR : Receive Unkown Type Message !" );
            break;
    }

    return;
}


VOS_UINT32 IPS_MNTN_CCORE_Init(enum VOS_INIT_PHASE_DEFINE InitPhase )
{
    VOS_UINT16                  usIndex;
    IPS_MNTN_CCORE_CAPTURE_CB   astCallbackTbl[IPS_MNTN_CCORE_CB_TYPE_BUTT]   ={ VOS_NULL_PTR,
                                                                                 VOS_NULL_PTR,
                                                                                 VOS_NULL_PTR,
                                                                                 IPS_MNTN_CCORE_ArpRecvPktInfoCB,
                                                                                 IPS_MNTN_CCORE_ArpSendPktInfoCB,
                                                                                 IPS_MNTN_CCORE_DhcpcRecvPktInfoCB,
                                                                                 IPS_MNTN_CCORE_DhcpcSendPktInfoCB,
                                                                                 IPS_MNTN_CCORE_DhcpsRecvPktInfoCB,
                                                                                 IPS_MNTN_CCORE_DhcpsSendPktInfoCB,
                                                                                 IPS_MNTN_CCORE_AppCmdInfoCB };
    switch( InitPhase )
    {
        case   VOS_IP_LOAD_CONFIG:
            /* ע��TCP/IPЭ��ջ��ά�ɲ�ص����� */
            if ((VOS_ERROR) == IPS_MNTN_CCORE_InterpeakMntnInit())
            {
                PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                        "IPS, IPS_MNTN_CCORE_Init : ERROR : IPS_MNTN_INTERPEAKMNTNINIT Failed!" );
                return VOS_ERR;
            }

            for ( usIndex = 0; usIndex < IPS_MNTN_CCORE_CB_TYPE_BUTT; usIndex++ )
            {
                if ((VOS_ERROR) == IPS_MNTN_CCORE_RegExtFunc(astCallbackTbl[usIndex], usIndex))
                {
                    PS_LOG1(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                            "IPS, IPS_MNTN_CCORE_Init : ERROR : IPS_MNTN_CCORE_RegExtFunc Failed! usIndex = %d", usIndex);
                    return VOS_ERR;
                }
            }

            /* ��ʼ����ά�ɲ�������Ϣ */
            PS_MEM_SET(&g_stIpsCcoreTraceMsgCfg, 0, sizeof(IPS_MNTN_CCORE_TRACE_CONFIG_REQ_STRU));
            /* ��ʼ��TCP/IPЭ��ջ��ά�ɲ�ʵ�� */
            PS_MEM_SET(&g_stIpsCcoreMntnInfoEntity, 0, sizeof(IPS_MNTN_CCORE_INFO_ENTITY));
            break;

        case   VOS_IP_FARMALLOC:
        case   VOS_IP_INITIAL:
        case   VOS_IP_ENROLLMENT:
        case   VOS_IP_LOAD_DATA:
        case   VOS_IP_FETCH_DATA:
        case   VOS_IP_STARTUP:
        case   VOS_IP_RIVAL:
        case   VOS_IP_KICKOFF:
        case   VOS_IP_STANDBY:
        case   VOS_IP_BROADCAST_STATE:
        case   VOS_IP_RESTART:
            break;

        default:
            break;
    }

    return VOS_OK;
}
VOS_VOID IPS_MNTN_CCORE_MsgProc( struct MsgCB * pMsg )
{
    if(VOS_NULL_PTR == pMsg)
    {
        PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_MsgProc : ERROR : Message NULL !" );
        return;
    }

    switch( pMsg->ulSenderPid )
    {
        case WUEPS_PID_OM: /* ����AT����Ϣ���� */
            IPS_MNTN_CCORE_RcvOmMsgProc( (VOS_VOID *)pMsg );
            break;

        case VOS_PID_TIMER:
            IPS_MNTN_CCORE_RcvTimerExpireMsgProc((REL_TIMER_MSG *)pMsg);
            break;

        default:
            break;
    }

    return;
}
VOS_UINT32 IPS_MNTN_CCORE_FidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    VOS_UINT32  ulReturnCode;


    switch( ip )
    {
    case    VOS_IP_LOAD_CONFIG:

        /* ��ʼ��IMS�������� */
        ulReturnCode        = IMS_NIC_InitAll();
        if ( IMS_NIC_SUCC != ulReturnCode )
        {
            /* ��ӡ��ulResult��ֵ */
            PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                "IPS, IPS_MNTN_CCORE_FidInit IMS_NIC_Init Failed! \n");
            return VOS_ERR;
        }

        /* PIDΪWUEPS_PID_IPS_MNTN_CCORE*/
        ulReturnCode = VOS_RegisterPIDInfo(UEPS_PID_IPS_CCORE,
                                    (Init_Fun_Type)IPS_MNTN_CCORE_Init,
                                    (Msg_Fun_Type)IPS_MNTN_CCORE_MsgProc);
        if( VOS_OK != ulReturnCode )
        {
            PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                    "IPS, IPS_MNTN_CCORE_FidInit VOS_RegisterPIDInfo Failed" );
            return VOS_ERR;
        }

        /* FIDΪWUEPS_PID_IPS_MNTN*/
        ulReturnCode = VOS_RegisterMsgTaskPrio(UEPS_FID_IPS_CCORE, VOS_PRIORITY_M4);
        if( VOS_OK != ulReturnCode )
        {
            PS_LOG(UEPS_PID_IPS_CCORE, 0, PS_PRINT_ERROR,
                    "IPS, IPS_MNTN_CCORE_FidInit VOS_RegisterTaskPrio Failed" );
            return VOS_ERR;
        }
        break;

    case   VOS_IP_FARMALLOC:
    case   VOS_IP_INITIAL:
    case   VOS_IP_ENROLLMENT:
    case   VOS_IP_LOAD_DATA:
    case   VOS_IP_FETCH_DATA:
    case   VOS_IP_STARTUP:
    case   VOS_IP_RIVAL:
    case   VOS_IP_KICKOFF:
    case   VOS_IP_STANDBY:
    case   VOS_IP_BROADCAST_STATE:
    case   VOS_IP_RESTART:
        break;

    default:
        break;
    }

    return VOS_OK;
} /* ISP_MNTN_CCORE_FidInit */

#else /*FEATURE_ON == FEATURE_IMS*/

VOS_UINT32 IPS_MNTN_CCORE_FidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
    case    VOS_IP_LOAD_CONFIG:

        break;

    case   VOS_IP_FARMALLOC:
    case   VOS_IP_INITIAL:
    case   VOS_IP_ENROLLMENT:
    case   VOS_IP_LOAD_DATA:
    case   VOS_IP_FETCH_DATA:
    case   VOS_IP_STARTUP:
    case   VOS_IP_RIVAL:
    case   VOS_IP_KICKOFF:
    case   VOS_IP_STANDBY:
    case   VOS_IP_BROADCAST_STATE:
    case   VOS_IP_RESTART:
        break;

    default:
        break;
    }

    return VOS_OK;
} /* ISP_MNTN_CCORE_FidInit */

#endif  /*FEATURE_ON == FEATURE_IMS*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif




