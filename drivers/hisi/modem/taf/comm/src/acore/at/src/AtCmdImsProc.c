

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "AtCmdImsProc.h"
#include "ATCmdProc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                 PS_FILE_ID_AT_CMD_IMS_PROC_C
/*lint +e767*/

#if (FEATURE_ON == FEATURE_IMS)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*AT��IMSAģ�����Ϣ������ָ��*/
const AT_IMSA_MSG_PRO_FUNC_STRU g_astAtImsaMsgTab[]=
{
    /* ��ϢID */                            /* ��Ϣ������ */
    {ID_IMSA_AT_CIREG_SET_CNF,              AT_RcvImsaCiregSetCnf},
    {ID_IMSA_AT_CIREG_QRY_CNF,              AT_RcvImsaCiregQryCnf},
    {ID_IMSA_AT_CIREP_SET_CNF,              AT_RcvImsaCirepSetCnf},
    {ID_IMSA_AT_CIREP_QRY_CNF,              AT_RcvImsaCirepQryCnf},
    {ID_IMSA_AT_VOLTEIMPU_QRY_CNF,          AT_RcvImsaImpuSetCnf},
    {ID_IMSA_AT_CIREGU_IND,                 AT_RcvImsaCireguInd},
    {ID_IMSA_AT_CIREPH_IND,                 AT_RcvImsaCirephInd},
    {ID_IMSA_AT_CIREPI_IND,                 AT_RcvImsaCirepiInd},
    {ID_IMSA_AT_CCWAI_SET_CNF,              AT_RcvImsaCcwaiSetCnf},
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID AT_ProcImsaMsg(AT_IMSA_MSG_STRU *pstMsg)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulMsgId;
    VOS_UINT32                          ulRst;

    /*��g_astAtProcMsgFromImsaTab�л�ȡ��Ϣ����*/
    ulMsgCnt = sizeof(g_astAtImsaMsgTab)/sizeof(AT_IMSA_MSG_PRO_FUNC_STRU);

    /*����Ϣ���л�ȡMSG ID*/
    ulMsgId  = pstMsg->ulMsgId;

    /*g_astAtProcMsgFromImsaTab���������Ϣ�ַ�*/
    for (i = 0; i < ulMsgCnt; i++)
    {
        if (g_astAtImsaMsgTab[i].ulMsgId == ulMsgId)
        {
            ulRst = g_astAtImsaMsgTab[i].pProcMsgFunc(pstMsg);

            if (VOS_ERR == ulRst)
            {
                AT_ERR_LOG("AT_ProcImsaMsg: Msg Proc Err!");
            }

            return;
        }
    }

    /*û���ҵ�ƥ�����Ϣ*/
    if (ulMsgCnt == i)
    {
        AT_ERR_LOG("AT_ProcImsaMsg: Msg Id is invalid!");
    }

    return;
}


VOS_UINT32 AT_RcvImsaCiregSetCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREG_SET_CNF_STRU         *pstCiregCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    pstCiregCnf = (IMSA_AT_CIREG_SET_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCiregCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_CIREG_SET */
    if ( AT_CMD_CIREG_SET != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaCiregSetCnf: WARNING:Not AT_CMD_CIREG_SET!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    if ( VOS_OK == pstCiregCnf->ulResult )
    {
        ulResult    = AT_OK;
    }
    else
    {
        ulResult    = AT_ERROR;
    }

    gstAtSendData.usBufLen = 0;

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}
VOS_UINT32 AT_RcvImsaCiregQryCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREG_QRY_CNF_STRU         *pstCiregCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usLength;

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    usLength    = 0;
    pstCiregCnf = (IMSA_AT_CIREG_QRY_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCiregCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_CIREG_QRY */
    if ( AT_CMD_CIREG_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaCiregQryCnf: WARNING:Not AT_CMD_CIREG_SET!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    if ( VOS_OK == pstCiregCnf->ulResult )
    {

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           "%s: %d,",
                                           g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                           pstCiregCnf->enCireg);

        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr+usLength,
                                           "%d",
                                           pstCiregCnf->ulRegInfo);

        /* ���IMSδע�ᣬ<ext_info>���������壬�Ҳ���������3GPP 27007 v11 8.71 */
        if ((VOS_FALSE != pstCiregCnf->ulRegInfo) && (VOS_FALSE != pstCiregCnf->bitOpExtInfo))
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr+usLength,
                                               ",%d",
                                               pstCiregCnf->ulExtInfo);
        }

        ulResult                = AT_OK;
    }
    else
    {
        ulResult                = AT_ERROR;
    }

    gstAtSendData.usBufLen  = usLength;

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirepSetCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREP_SET_CNF_STRU         *pstCirepCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    pstCirepCnf = (IMSA_AT_CIREP_SET_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCirepCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_CIREP_SET */
    if ( AT_CMD_CIREP_SET != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaCirepSetCnf: WARNING:Not AT_CMD_CIREP_SET!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    if ( VOS_OK == pstCirepCnf->ulResult )
    {
        ulResult    = AT_OK;
    }
    else
    {
        ulResult    = AT_ERROR;
    }

    gstAtSendData.usBufLen = 0;

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}
VOS_UINT32 AT_RcvImsaCirepQryCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREP_QRY_CNF_STRU         *pstCirepCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    pstCirepCnf = (IMSA_AT_CIREP_QRY_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCirepCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_CIREP_QRY */
    if ( AT_CMD_CIREP_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaCirepQryCnf: WARNING:Not AT_CMD_CIREP_QRY!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    if ( VOS_OK == pstCirepCnf->ulResult )
    {

        gstAtSendData.usBufLen= (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                                       "%s: %d,%d",
                                                       g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                       pstCirepCnf->enReport,
                                                       pstCirepCnf->enImsvops);

        ulResult                = AT_OK;
    }
    else
    {
        gstAtSendData.usBufLen  = 0;
        ulResult                = AT_ERROR;
    }

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : AT_RcvImsaImpuSetCnf
 ��������  : +CIREP��ѯ����ظ�������
 �������  : *pMsg        �ظ���Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��17��
    ��    ��   : c64416
    �޸�����   : ��������

*****************************************************************************/
VOS_UINT32 AT_RcvImsaImpuSetCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_VOLTEIMPU_QRY_CNF_STRU      *pstImpuCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;
    VOS_CHAR                            acString[AT_IMSA_IMPU_MAX_LENGTH+1];

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    pstImpuCnf  = (IMSA_AT_VOLTEIMPU_QRY_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstImpuCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_IMPU_SET */
    if ( AT_CMD_IMPU_SET != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:Not AT_CMD_IMPU_SET!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    VOS_MemSet(acString, 0, sizeof(acString));
    if(pstImpuCnf->ulImpuLen > AT_IMSA_IMPU_MAX_LENGTH)
    {
        VOS_MemCpy(acString, pstImpuCnf->aucImpu, AT_IMSA_IMPU_MAX_LENGTH);
    }
    else
    {
        VOS_MemCpy(acString, pstImpuCnf->aucImpu, pstImpuCnf->ulImpuLen);
    }

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    if ( VOS_OK == pstImpuCnf->ulResult )
    {

        gstAtSendData.usBufLen= (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                                       "%s: %s",
                                                       g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                       acString);

        ulResult                = AT_OK;
    }
    else
    {
        gstAtSendData.usBufLen  = 0;
        ulResult                = AT_ERROR;
    }

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}
VOS_UINT32 AT_RcvImsaCirephInd(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREPH_IND_STRU            *pstCirephInd;
    VOS_UINT8                           ucIndex;

    /* ��ʼ����Ϣ���� */
    ucIndex      = 0;
    pstCirephInd = (IMSA_AT_CIREPH_IND_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCirephInd->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCirephInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s%s: %d%s",
                                                    gaucAtCrLf,
                                                    gastAtStringTab[AT_STRING_CIREPH].pucText,
                                                    pstCirephInd->enHandover,
                                                    gaucAtCrLf);

    At_SendResultData(ucIndex, pgucAtSndCodeAddr, gstAtSendData.usBufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCirepiInd(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREPI_IND_STRU            *pstCirepiInd;
    VOS_UINT8                           ucIndex;

    /* ��ʼ����Ϣ���� */
    ucIndex      = 0;
    pstCirepiInd = (IMSA_AT_CIREPI_IND_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCirepiInd->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCirepiInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s%s: %d%s",
                                                    gaucAtCrLf,
                                                    gastAtStringTab[AT_STRING_CIREPI].pucText,
                                                    pstCirepiInd->enImsvops,
                                                    gaucAtCrLf);

    At_SendResultData(ucIndex, pgucAtSndCodeAddr, gstAtSendData.usBufLen);

    return VOS_OK;
}


VOS_UINT32 AT_RcvImsaCireguInd(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CIREGU_IND_STRU            *pstCireguInd;
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLength;

    /* ��ʼ����Ϣ���� */
    ucIndex      = 0;
    usLength     = 0;
    pstCireguInd = (IMSA_AT_CIREGU_IND_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCireguInd->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCireguInd: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* �жϲ�ѯ�����Ƿ�ɹ� */
    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       "%s%s: %d",
                                       gaucAtCrLf,
                                       gastAtStringTab[AT_STRING_CIREGU].pucText,
                                       pstCireguInd->ulRegInfo);

    /* ���IMSδע�ᣬ<ext_info>���������� */
    if ((VOS_FALSE != pstCireguInd->ulRegInfo) && (VOS_FALSE != pstCireguInd->bitOpExtInfo))
    {
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                           ",%d",
                                           pstCireguInd->ulExtInfo);
    }

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                       "%s",
                                       gaucAtCrLf);

    gstAtSendData.usBufLen = usLength;
    At_SendResultData(ucIndex, pgucAtSndCodeAddr, gstAtSendData.usBufLen);

    return VOS_OK;
}
VOS_UINT32 AT_RcvImsaCcwaiSetCnf(VOS_VOID * pMsg)
{
    /* ����ֲ����� */
    IMSA_AT_CCWAI_SET_CNF_STRU          *pstCcwaiCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;

    /* ��ʼ����Ϣ���� */
    ucIndex     = 0;
    pstCcwaiCnf = (IMSA_AT_CCWAI_SET_CNF_STRU *)pMsg;

    /* ͨ��ClientId��ȡucIndex */
    if ( AT_FAILURE == At_ClientIdToUserId(pstCcwaiCnf->usClientId, &ucIndex) )
    {
        AT_WARN_LOG("AT_RcvImsaCcwaiSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvImsaImpuSetCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* �жϵ�ǰ���������Ƿ�ΪAT_CMD_CCWAI_SET */
    if ( AT_CMD_CCWAI_SET != gastAtClientTab[ucIndex].CmdCurrentOpt )
    {
        AT_WARN_LOG("AT_RcvImsaCcwaiSetCnf: WARNING:Not AT_CMD_CCWAI_SET!");
        return VOS_ERR;
    }

    /* ��λAT״̬ */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* �ж����ò����Ƿ�ɹ� */
    if (VOS_OK == pstCcwaiCnf->ulResult)
    {
        ulResult = AT_OK;
    }
    else
    {
        ulResult = AT_ERROR;
    }

    gstAtSendData.usBufLen = 0;

    /* ����At_FormatResultData���������� */
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


