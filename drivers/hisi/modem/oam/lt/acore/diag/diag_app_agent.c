/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : diag_app_agent.c
  Description     :
  History         :
     1.y00228784       2012-11-22   Draft Enact

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
/*lint -save -e322 -e7 -e537*/
#include  "vos.h"
#include <linux/slab.h>
#include  "diag_app_agent.h"
#include  "diag_common.h"
#include  "diag_debug.h"
#include  "diag_fw.h"
#include "msp_errno.h"
#include "msp_drx.h"
#include "diag_cfg.h"
#include "diag_port.h"
#include "diag_fs_proc.h"
#include "drx_msp_api.h"
#include "DrvInterface.h"
/*lint -restore*/
/*lint -save -e767 ԭ��:Log��ӡ*/
#define    THIS_FILE_ID          MSP_FILE_ID_DIAG_APP_AGENT_C
/*lint -restore +e767*/
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
#define DIAG_CMD_TOTAL_FILE                    (0xFFFFFFFF)

DIAG_NV_BIN_LOAD_INFOR g_DiagNvLoadInfo ={0};

/*****************************************************************************
  3 Function
*****************************************************************************/

/*****************************************************************************
 Function Name   : diag_AppAgentTimeOutProcEntry
 Description     : APP AGENT����ʱ��Ϣ
 Input           :VOS_VOID* pstPara
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-20  Draft Enact

*****************************************************************************/
VOS_VOID diag_AppAgentTimeOutProcEntry(VOS_VOID* pstPara)
{
    /*lint --e{718, 746} */
    /*��ʱ��Ϣ���ͷ�NV LOAD������ڴ�*/
    if( DIAG_TMR_NV_LOAD_CMD_PARAM == (VOS_UINT32)pstPara)
    {
        DIAG_FREE(g_DiagNvLoadInfo.pucBinData);
        g_DiagNvLoadInfo.pucBinData = NULL;
    }
	else if(DIAG_TMR_FILE_OP_TIMEOUT_PARAM == (VOS_UINT32)pstPara)
	{
		diag_FsClose();
		diag_printf("DIAG_TMR_FILE_OP_TIMEOUT!\n");
	}
    return ;
}
VOS_UINT32 diag_PsProcEntry (VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
	DIAG_CMD_REPLAY_SET_REQ_STRU *pstReplay=NULL;
	DIAG_CMD_REPLAY_SET_CNF_STRU stCnf={0};

    pstReplay = (DIAG_CMD_REPLAY_SET_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

    switch(MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))
    {
        case DIAG_CMD_PS_REPLAY_SET:
			ret = diag_SendMsg(diag_GetAgentPid(),PS_PID_MM,ID_MSG_DIAG_CMD_REPLAY_TO_PS,(VOS_UINT8*)pstReplay,\
							sizeof(DIAG_CMD_REPLAY_SET_REQ_STRU));
            break;
		default:
            break;
    }

	stCnf.ulRet = ret;

	/*�����FW�ظ�*/
	ret = diag_AgentCnfFun((VOS_UINT8*)&stCnf,ulCmdId,sizeof(DIAG_CMD_REPLAY_SET_CNF_STRU));
    return ret;
}

/*****************************************************************************
 Function Name   : diag_NvProcEntry
 Description     : �ú���Ϊ����DIAG FW��������NV�����������
 Input           : pstReq ����������
                   ulCmdId ����ID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvProcEntry (VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;

    switch(MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))
    {
        case DIAG_CMD_NV_RD:
            ret = diag_NvRdProc(pstReq,ulCmdId);
            break;
        case DIAG_CMD_NV_WR:
            ret = diag_NvWrProc(pstReq,ulCmdId);
            break;
#if 0
        case DIAG_CMD_NV_IMPORT:
            ret = diag_NvImportProc(pstReq,ulCmdId);
            break;
        case DIAG_CMD_NV_EXPORT:
            ret = diag_NvExportProc(pstReq,ulCmdId);
            break;
#endif
        default:
            break;

    }
    return ret;
}

/*****************************************************************************
 Function Name   : NvRdProc
 Description     : �ú������ڴ����NvProcEntry�������Ķ�NV����
 Input           : pstReq ����������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvRdProc(VOS_UINT8* pstReq,VOS_UINT32 ulCmdId)
{
    DIAG_CMD_NV_QRY_REQ_STRU* pstNVQryReq = NULL;
    DIAG_CMD_NV_QRY_CNF_STRU* pstNVQryCnf = NULL;
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT32 ulSetRet = 0;
    VOS_UINT32 ulNVLen = 0;

    pstNVQryReq = (DIAG_CMD_NV_QRY_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

     /*��������ID��ȡNV���*/
     ret = NVM_GetItemLength((BSP_U16)(pstNVQryReq->ulNVId), (BSP_U32*)&ulNVLen);
     if(ERR_MSP_SUCCESS != ret)
     {
     	 diag_printf("[%s]:ulNVId=0x%x,ulNVLen=%d!\n",__FUNCTION__,pstNVQryReq->ulNVId,ulNVLen);
         return ERR_MSP_FAILURE;
     }

     pstNVQryCnf = VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,sizeof(DIAG_CMD_NV_QRY_CNF_STRU)+ulNVLen);

     if(NULL  == pstNVQryCnf)
     {
     	 diag_printf("[%s]:malloc error!\n",__FUNCTION__);
         return ERR_MSP_MALLOC_FAILUE;
     }

     ulSetRet = NVM_Read(pstNVQryReq->ulNVId, pstNVQryCnf->aucData, ulNVLen);
     if(ulSetRet != ERR_MSP_SUCCESS)
     {
        diag_printf("[%s]:NV READ ERR 0x%x,ulNVId=0x%x\n",__FUNCTION__,ulSetRet,pstNVQryReq->ulNVId);
     }
     DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_NV_RD,pstNVQryReq->ulNVId,ulSetRet,0);

     pstNVQryCnf->ulNVId = pstNVQryReq->ulNVId;
     pstNVQryCnf->ulRc   = ulSetRet;
     pstNVQryCnf->ulDataSize = ulNVLen;

    /*����ظ���FW*/
    ret = diag_AgentCnfFun((VOS_UINT8*)pstNVQryCnf,ulCmdId,sizeof(DIAG_CMD_NV_QRY_CNF_STRU) + ulNVLen);

    VOS_MemFree(MSP_PID_DIAG_APP_AGENT, pstNVQryCnf);

    return ret;
}

/*****************************************************************************
 Function Name   : diag_NvWrProc
 Description     : �ú������ڴ����NvProcEntry��������дNV����
 Input           : pstReq ����������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvWrProc(VOS_UINT8* pstReq,VOS_UINT32 ulCmdId)
{
    DIAG_CMD_NV_WR_REQ_STRU* pstNVWRReq = NULL;
    DIAG_CMD_NV_WR_CNF_STRU stNVWRCnf = {0};
    VOS_UINT32 ret;
    VOS_UINT32 ulSetRet;

    pstNVWRReq = (DIAG_CMD_NV_WR_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

    /*����ж�*/
    if((NULL == pstNVWRReq) ||(pstNVWRReq->ulDataSize > NV_ITEM_MAXSIZE)||
      (sizeof(DIAG_CMD_NV_WR_REQ_STRU) > ((MSP_DIAG_HEAD_STRU *)((VOS_UINT8*)pstReq+ \
       sizeof(MSP_SOCP_HEAD_STRU)))->ulDataSize-sizeof(MSP_DIAG_DATA_REQ_STRU)))
    {
    	diag_printf("[%s]:ulNVId=0x%x,ulNVLen=%d!\n",__FUNCTION__,pstNVWRReq->ulNVId,pstNVWRReq->ulDataSize);
        return ERR_MSP_INVALID_PARAMETER;
    }

    /*д��NV��*/
    ulSetRet = NVM_Write(pstNVWRReq->ulNVId, pstNVWRReq->aucData, pstNVWRReq->ulDataSize);
    if(ulSetRet != ERR_MSP_SUCCESS)
    {
       diag_printf("[%s]:NV Write ERR 0x%x,ulNVId=0x%x\n",__FUNCTION__,ulSetRet,pstNVWRReq->ulNVId);
    }
	DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_NV_WR,pstNVWRReq->ulNVId,ulSetRet,0);

    stNVWRCnf.ulNVId = pstNVWRReq->ulNVId;
    stNVWRCnf.ulRc   = ulSetRet;

    /*����ظ���FW*/
    ret = diag_AgentCnfFun((VOS_UINT8*)&stNVWRCnf,ulCmdId,sizeof(DIAG_CMD_NV_WR_CNF_STRU));


    return ret;

}

#if 0
/*****************************************************************************
 Function Name   : diag_NvCheckImportPara
 Description     : �ú������NV�����������
 Input           : pstLoadReq ����������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvCheckImportPara(DIAG_CMD_NV_FILE_LOAD_REQ_STRU* pstLoadReq)
{
    if((NULL == pstLoadReq)
        ||(MAX_HNV_LENGTH <= pstLoadReq->ulTotalSize)
        ||(pstLoadReq->ulBlockSize > pstLoadReq->ulTotalSize)
        ||(0 == pstLoadReq->ulBlockSize)
        ||(0 == pstLoadReq->ulTotalSize)
        ||(pstLoadReq->ulBlockIndex >= pstLoadReq->ulTotalBlockCnt))
    {
    	diag_printf("[%s]:ulTotalSize=%d,ulBlockIndex=%d,ulTotalBlockCnt=%d!\n",__FUNCTION__,pstLoadReq->ulTotalSize,pstLoadReq->ulBlockIndex,pstLoadReq->ulTotalBlockCnt);
        return ERR_MSP_INVALID_PARAMETER;
    }
	return ERR_MSP_SUCCESS;
}

/*****************************************************************************
 Function Name   : diag_NvImportProc
 Description     : �ú������ڴ����NvProcEntry�������ĵ���NV����
 Input           : pstReq ����������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvImportProc(VOS_UINT8* pstReq,VOS_UINT32 ulCmdId)
{
    DIAG_CMD_NV_FILE_LOAD_REQ_STRU* pstLoadReq = NULL;
    DIAG_CMD_NV_FILE_LOAD_CNF_STRU stLoadCnf = {0};
    VOS_UINT32 ulSetRet = ERR_MSP_SUCCESS;
    VOS_UINT32 ret = ERR_MSP_SUCCESS;

    pstLoadReq = (DIAG_CMD_NV_FILE_LOAD_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));
    DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_NV_IMPORT,pstLoadReq->ulBlockIndex,pstLoadReq->ulTotalSize,0);

    /* ������ֵ�Ƿ�Ϸ�*/
    if(ERR_MSP_SUCCESS != diag_NvCheckImportPara(pstLoadReq))
    {
        return ERR_MSP_INVALID_PARAMETER;
    }

    /*���������ڴ棬�Ͳ��������ڴ棬��������*/
    if(NULL == g_DiagNvLoadInfo.pucBinData)
    {
        g_DiagNvLoadInfo.pucBinData = (VOS_UINT8 *)DIAG_MALLOC(pstLoadReq->ulTotalSize);

        if(NULL == g_DiagNvLoadInfo.pucBinData)
        {
			diag_printf("[%s]:diag malloc fail!\n",__FUNCTION__);
            return ERR_MSP_MALLOC_FAILUE;
        }

        /*����V7R1��������ص�һ��ͻȻ��ֹ���������ڴ�ò����ͷ�*/
        VOS_StartRelTimer(&g_DiagNvLoadInfo.NvLoadTimer,MSP_PID_DIAG_APP_AGENT,DIAG_CMD_NV_LOAD_TIMEOUT_TIME_LEN, 0,DIAG_TMR_NV_LOAD_CMD_PARAM,VOS_RELTIMER_NOLOOP,VOS_TIMER_NO_PRECISION);
    }

    /*����·������������������,�����һ��֮ǰ�����Ƶ�pBINData*/
    if((pstLoadReq->ulBlockIndex == g_DiagNvLoadInfo.ulExpectBlockIndex)&&
        ((pstLoadReq->ulBlockIndex + 1) != pstLoadReq->ulTotalBlockCnt))
    {
        memcpy(g_DiagNvLoadInfo.pucBinData + g_DiagNvLoadInfo.ulNvLen,pstLoadReq->aucData,pstLoadReq->ulBlockSize);

        g_DiagNvLoadInfo.ulNvLen += pstLoadReq->ulBlockSize;
        g_DiagNvLoadInfo.ulExpectBlockIndex = pstLoadReq->ulBlockIndex + 1;
        g_DiagNvLoadInfo.ulCnfBlockIndex = pstLoadReq->ulBlockIndex + 1;
    }
    /*�ۼƵ����һ�飬һ����NV��д�룬����д�뵽һ���쳣������NV�ļ��ƻ�����������*/
    else if((pstLoadReq->ulBlockIndex == g_DiagNvLoadInfo.ulExpectBlockIndex)&&
        ((pstLoadReq->ulBlockIndex + 1) == pstLoadReq->ulTotalBlockCnt))
    {
        /*�����һ�����ݸ��Ƶ�pBINData*/
        memcpy(g_DiagNvLoadInfo.pucBinData + g_DiagNvLoadInfo.ulNvLen,pstLoadReq->aucData,pstLoadReq->ulBlockSize);

        /*д�뵥��*/
        ulSetRet= nvim_DiagDownLoad(g_DiagNvLoadInfo.pucBinData,pstLoadReq->ulTotalSize);
        DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_NV_IMPORT,&g_DiagNvLoadInfo,ulSetRet,0);

        g_DiagNvLoadInfo.ulNvLen =0;
        g_DiagNvLoadInfo.ulExpectBlockIndex = 0;
        g_DiagNvLoadInfo.ulCnfBlockIndex = DIAG_CMD_TOTAL_FILE;
        /*ֹͣ��ʱ��*/
        VOS_StopRelTimer(&g_DiagNvLoadInfo.NvLoadTimer);

        /*д����ɣ��ͷ��ڴ�*/
        DIAG_FREE(g_DiagNvLoadInfo.pucBinData);
        g_DiagNvLoadInfo.pucBinData = NULL;
    }
    else
    {
        //���������·��鲻һ��ʱ����HSO�ظ���������Ų���
    }

    /*�ظ�NV���ؽ��*/
    stLoadCnf.ulIndex = g_DiagNvLoadInfo.ulCnfBlockIndex;
    stLoadCnf.ulRc = ulSetRet;
    ret = diag_AgentCnfFun((VOS_UINT8*)&stLoadCnf,ulCmdId,sizeof(DIAG_CMD_NV_FILE_LOAD_CNF_STRU));

    return ret;

}

/*****************************************************************************
 Function Name   : diag_NvExportFileInfo
 Description     : ��ȡNV�����ļ���Ϣ
 Input           : ulType   �����ļ�����, 0: ��ʼ�ļ�, 1: ��ǰ�ļ�
                   pstExportFileInfo  �����ļ���Ϣ
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvExportFileInfo(VOS_UINT32 ulType,NVIM_EXPORT_FILE_INFO_STRU *pstExportFileInfo)
{
    VOS_UINT32 ulRst;

    switch (ulType)
    {
    case DIAG_CMD_EXPORT_FILE_TYPE_INITIAL:
        ulRst = nvim_GetInitExportFileInfo(pstExportFileInfo);
        break;
    case DIAG_CMD_EXPORT_FILE_TYPE_CURRENT:
        ulRst = nvim_GetWorkExportFileInfo(pstExportFileInfo);
        break;
    default:
        ulRst = ERR_MSP_INVALID_PARAMETER;
        break;
    }

    if(ERR_MSP_SUCCESS != ulRst)
    {
        pstExportFileInfo->ulfilelen = 0;
        pstExportFileInfo->pdata     = NULL;
    }

    return ulRst;
}


/*****************************************************************************
 Function Name   : diag_NvExportProc
 Description     : �ú������ڴ����NvProcEntry�������ĵ���NV����
 Input           : pstReq ����������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-11-22  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_NvExportProc(VOS_UINT8* pstReq,VOS_UINT32 ulCmdId)
{
    DIAG_CMD_NV_EXPORT_FILE_REQ_STRU* pstExpFileReq = NULL;
    DIAG_CMD_NV_EXPORT_FILE_CNF_STRU* pstExpFileCnf = NULL;
    NVIM_EXPORT_FILE_INFO_STRU stExportFileInfo = {0};
    VOS_UINT32 ulDataSize =0;
    VOS_UINT32 ulSetRet =0,ret = 0;

    pstExpFileReq = (DIAG_CMD_NV_EXPORT_FILE_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

    /*�����·�����������ļ�ϵͳ��FLASH��NV*/
    stExportFileInfo.pdata = DIAG_MALLOC(NVM_FLASH_BLOCK_SIZE * NVM_BACKUP_VERSION_FILE_BLOCK_NUM);

    if(stExportFileInfo.pdata == NULL)
    {
    	diag_printf("[%s]:diag malloc fail!\n",__FUNCTION__);
        return ERR_MSP_MALLOC_FAILUE;
    }

    ulSetRet = diag_NvExportFileInfo(pstExpFileReq->ulType,&stExportFileInfo);
	DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_NV_EXPORT,pstExpFileReq->ulType,ulSetRet,0);

    if(pstExpFileReq->ulOffset < stExportFileInfo.ulfilelen)
    {
         ulDataSize = stExportFileInfo.ulfilelen - pstExpFileReq->ulOffset;
         ulDataSize = ulDataSize > DIAG_TRANSFER_BLOCK_MAX_SIZE ? DIAG_TRANSFER_BLOCK_MAX_SIZE :ulDataSize;
    }
    else
    {
    	diag_printf("[%s]:diag_NvExportFileInfo fail!\n",__FUNCTION__);
        DIAG_FREE(stExportFileInfo.pdata);
        return ERR_MSP_INVALID_PARAMETER;
    }
    pstExpFileCnf = DIAG_MALLOC(sizeof(DIAG_CMD_NV_EXPORT_FILE_CNF_STRU) + ulDataSize);

    if(NULL == pstExpFileCnf)
    {
       diag_printf("[%s]:diag malloc2 fail!\n",__FUNCTION__);
       DIAG_FREE(stExportFileInfo.pdata);
       return ERR_MSP_MALLOC_FAILUE;
    }

    pstExpFileCnf->ulRst = ulSetRet;
    pstExpFileCnf->ulOffset = pstExpFileReq->ulOffset;
    pstExpFileCnf->ulType = pstExpFileReq->ulType;
    if(ERR_MSP_SUCCESS == ulSetRet)
    {
        pstExpFileCnf->ulSize= ulDataSize;
        pstExpFileCnf->ulRemainSize = stExportFileInfo.ulfilelen - pstExpFileReq->ulOffset - ulDataSize;
        memcpy(pstExpFileCnf->aucData, stExportFileInfo.pdata + pstExpFileReq->ulOffset, ulDataSize);
    }
    else
    {
        pstExpFileCnf->ulSize = 0;
        pstExpFileCnf->ulRemainSize = stExportFileInfo.ulfilelen - pstExpFileReq->ulOffset;
    }

    ret = diag_AgentCnfFun((VOS_UINT8*)pstExpFileCnf,ulCmdId,sizeof(DIAG_CMD_NV_EXPORT_FILE_CNF_STRU)+ ulDataSize);

    /*�ظ�����ͷ�����ռ�*/
    DIAG_FREE(stExportFileInfo.pdata);
    DIAG_FREE(pstExpFileCnf);
    return ret;
}
#endif

/*****************************************************************************
 Function Name   : diag_LogSaveProc
 Description     : LOG SAVE ����
 Input           :VOS_UINT8* pstReq
                VOS_UINT32 ulCmdId
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-26  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_LogSaveProc (VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    DIAG_CMD_SET_LPD_MODE_REQ_STRU* pstLdpReq;
    DIAG_CMD_SET_LPD_MODE_CNF_STRU stLdpCnf = {0};
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT32 ulSetRet;

    pstLdpReq = (DIAG_CMD_SET_LPD_MODE_REQ_STRU*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

    stLdpCnf.ulSDFileSaveTime = pstLdpReq->ulSDFileSaveTime;
    stLdpCnf.ulLpdMode = pstLdpReq->ulLpdMode;
    stLdpCnf.ulSDFileSize = pstLdpReq->ulSDFileSize;
    VOS_MemCpy(stLdpCnf.ucSDFileName,pstLdpReq->ucSDFileName,sizeof(pstLdpReq->ucSDFileName));

    ulSetRet = diag_SetLogCtrlInfo(pstLdpReq->ulLpdMode,pstLdpReq->ulSDFileSize,\
        pstLdpReq->ulSDFileSaveTime);
	DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_LOG_SAVE,pstLdpReq->ulLpdMode,ulSetRet,0);

    stLdpCnf.ulRc = ulSetRet;

    /*�����FW�ظ�*/
    ret = diag_AgentCnfFun((VOS_UINT8*)&stLdpCnf,ulCmdId,sizeof(DIAG_CMD_SET_LPD_MODE_CNF_STRU));

    return ret;

}


VOS_UINT32 g_ulCmdId;

/*****************************************************************************
 Function Name   : diag_PsTransCnfProc
 Description     : DSP??
 Input           :VOS_UINT8* pstReq
                VOS_UINT32 ulLen
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-8  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_PsTransCnfProc(VOS_UINT8* pstCnf ,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulCmdId = 0;
    VOS_UINT32 ret =0;
    /*VOS_UINT32 ulOffsetLen =0;*/
    VOS_UINT32 ulMsgId =0;
#if 0
    ulMsgId =  *((VOS_UINT32*)(pstCnf + DIAG_PS_TRANS_HEAD_LEN));
    ulOffsetLen = DIAG_IS_GUPS_MSG_ID(ulMsgId)? DIAG_GUPS_TRANS_CMD_HEAD_LEN : DIAG_LPS_TRANS_CMD_HEAD_LEN;
    ulCmdId = *((VOS_UINT32*)(pstCnf+ulOffsetLen));

	if(!DIAG_IS_PS_CMD(MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId)))
	{
		return ERR_MSP_INVALID_OP;
	}
#endif

    ulCmdId = g_ulCmdId;

	DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_PS_TRANS_CNF, ulMsgId, ulCmdId,0);

    ret = diag_AgentCnfFun((VOS_UINT8*)pstCnf, ulCmdId, ulLen);

    return ret;
}


/*****************************************************************************
 Function Name   : diag_PsTransProcEntry
 Description     : �ú������ڴ����PS͸������
 Input           :VOS_UINT8* pstReq
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-7  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_PsTransProcEntry(VOS_UINT8* pstReq)
{
    VOS_UINT32 ulPsCmdParasize = 0;
    DIAG_PS_MSG_STRU* pstSendReq = NULL;
    VOS_UINT32 ulCmdId = 0;

    ulPsCmdParasize = ((MSP_DIAG_HEAD_STRU*)DIAG_OFFSET_SOCP_GET_DIAG_HEAD(pstReq))->ulDataSize - sizeof(MSP_DIAG_DATA_REQ_STRU);

 	ulCmdId = ((MSP_DIAG_HEAD_STRU*)DIAG_OFFSET_SOCP_GET_DIAG_HEAD(pstReq))->ulID;

    g_ulCmdId = ulCmdId;

     /* ���͸������*/
    pstSendReq = (DIAG_PS_MSG_STRU*)((MSP_DIAG_DATA_REQ_STRU*)DIAG_OFFSET_SOCP_DIAG_GET_DIAG_DATA(pstReq))->ucData;

	DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_PS_TRANS_REQ,0,0,0);

    pstSendReq->ulSenderPid = MSP_PID_DIAG_APP_AGENT;

	return diag_SendPsTransMsg(pstSendReq->ulReceiverPid, pstSendReq, ulPsCmdParasize);
}

/*****************************************************************************
 Function Name   : diag_AppAgentDispatchCmd
 Description     : APP AGENT����CMDID�ַ������
 Input           :VOS_UINT8* pContext
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-26  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_AppAgentDispatchCmd(VOS_UINT8* pstReq )
{
    VOS_UINT32 ret =ERR_MSP_FAILURE;
    VOS_UINT32 ulCmdId = 0;

    if(NULL == pstReq)
    {
        return ret;
    }

    ulCmdId = (((MSP_DIAG_HEAD_STRU*)DIAG_OFFSET_SOCP_GET_DIAG_HEAD(pstReq))->ulID);
    DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_APP_AGENT_DISPATCH_CMD,ulCmdId,0,0);

    if(MSP_STRU_ID_28_31_GROUP_MSP == MSP_STRU_ID_28_31_CMD_GROUP(ulCmdId))
    {
        switch (MSP_STRU_ID_16_23_CMD_TYPE(ulCmdId))
        {
            /*���ݶ����ֶ������Ǹ�ps��͸�������msp�ڲ��������PMU������*/
            case MSP_STRU_ID_16_23_MSP_CMD:
                switch(MSP_STRU_ID_8_11_CMD_CATEGORY(ulCmdId))
                {
                    /*��������ID��Χ����������msp�ڲ�����*/
					case DIAG_CATEGORY_PS:
						ret = diag_PsProcEntry(pstReq,ulCmdId);
                        break;
                    case DIAG_CATEGORY_CFG:
                        ret = diag_CfgProcEntry(pstReq,ulCmdId);
                        break;
                    case DIAG_CATEGORY_NV:
                        ret = diag_NvProcEntry(pstReq,ulCmdId);
                        break;
                    case DIAG_CATEGORY_FS:
                        ret = diag_FsProcEntry(pstReq,ulCmdId);
                        break;
                    default:
                        break ;
                }
                break;

            case MSP_STRU_ID_16_23_PS_TRANS:
                ret = diag_PsTransProcEntry(pstReq);
                break;

            case MSP_STRU_ID_16_23_SYS_CMD:
                switch(MSP_STRU_ID_8_11_CMD_CATEGORY(ulCmdId))
                {
                    case DIAG_CATEGORY_CONN:
                        ret = diag_ConnProcEntry(pstReq,ulCmdId);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
#if (FEATURE_BSP_LCH_OM == FEATURE_ON)
    else if(MSP_STRU_ID_28_31_GROUP_BSP == MSP_STRU_ID_28_31_CMD_GROUP(ulCmdId))
    {
        switch (MSP_STRU_ID_16_23_CMD_TYPE(ulCmdId))
        {
            case MSP_STRU_ID_16_23_BSP_CMD:
                ret =  diag_BspProcEntry(pstReq,ulCmdId);
                break;

            default:
                break;
        }
    }
#endif
    else
    {

    }
    return ret;
}


#if (FEATURE_BSP_LCH_OM == FEATURE_ON)

/*****************************************************************************
 Function Name   : diag_BspProcEntry
 Description     : �ú���Ϊ����DIAG FW��������BSP ��������Ĵ������
 Input           : pstReq ����������
                   		ulCmdId ����ID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.h00135900      2013-02-21  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_BspProcEntry(VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32 ret;

    switch (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))
    {
        case DIAG_CMD_BSP_LOG_SET_ACORE:
        case DIAG_CMD_BSP_LOG_SET_CCORE:
            ret = diag_BspLogProcEntry(pstReq,ulCmdId);
            break;

        case DIAG_CMD_BSP_SYSVIEW_SWT_ACORE:
        case DIAG_CMD_BSP_SYSVIEW_SWT_CCORE:
        case DIAG_CMD_BSP_CPU_SWT_ACORE:
        case DIAG_CMD_BSP_CPU_SWT_CCORE:
            ret = diag_BspSysviewProcEntry(pstReq,ulCmdId);
            break;

        case DIAG_CMD_UTRACE_START:
        case DIAG_CMD_UTRACE_TERMINATE:
            ret = diag_BspUtraceProcEntry(pstReq,ulCmdId);
            break;

        default:
            ret = ERR_MSP_FAILURE;
            break ;
    }

    return ret;
}

/*****************************************************************************
 Function Name   : diag_BspUtraceProcEntry
 Description     : USB Trace��������
 Input           :VOS_UINT8* pstReq
                VOS_UINT32 ulCmdId
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2013-3-5  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_BspUtraceProcEntry (VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;

	VOS_UINT8 *pdata =NULL;
	DIAG_CMD_COMMON_CNF stUtraceCnf ={0};


    diag_printf("[%s enter!]\n",__FUNCTION__);



    pdata = (VOS_UINT8*)(DIAG_OFFSET_HEAD_GET_DATA(pstReq));

    switch(MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))
    {
        case DIAG_CMD_UTRACE_START:
            ret = DRV_UTRACE_START(pdata);
            break;
        case DIAG_CMD_UTRACE_TERMINATE:
            ret = DRV_UTRACE_TERMINATE(pdata);
            break;
        default:
            break;

    }

	stUtraceCnf.ulRet = ret;
	ret = diag_AgentCnfFun((VOS_UINT8*)&stUtraceCnf,ulCmdId,sizeof(DIAG_CMD_COMMON_CNF));

    return ret;
}

/*****************************************************************************
 Function Name   : diag_BspLogProcEntry
 Description     : �ú���Ϊ����DIAG FW��������BSP log��������Ĵ������
 Input           : pstReq ����������
                   		ulCmdId ����ID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.h00135900      2013-02-21  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_BspLogProcEntry(VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32  ret = ERR_MSP_SUCCESS;
    VOS_UINT32 data_len = 0;
    VOS_UINT32 cnf_data_len = 0;
    MSP_DIAG_HEAD_STRU *pstDiagHead         = VOS_NULL;
    DIAG_bsp_log_swt_cfg_s *pstLogSet = VOS_NULL;
    DIAG_BSP_PRINT_LOG_SWT_CNF_STRU  stLogSetCnf  = {0};
    VOS_UINT8 *pstCnf = VOS_NULL;

    pstDiagHead = (MSP_DIAG_HEAD_STRU*)(DIAG_OFFSET_SOCP_GET_DIAG_HEAD(pstReq));

    if(DIAG_CMD_BSP_LOG_SET_ACORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId)) )
    {
        pstLogSet = (DIAG_bsp_log_swt_cfg_s *)DIAG_OFFSET_HEAD_GET_DATA(pstReq);
        data_len = pstDiagHead->ulDataSize  - sizeof(MSP_DIAG_DATA_REQ_STRU);
        stLogSetCnf.ulRet  = DRV_LOG_LVL_SET(pstLogSet,data_len);
        vos_printf("diag_BspLogProcEntry  stLogSetCnf.ulRet  = 0x%x\n",stLogSetCnf.ulRet );
        pstCnf = (VOS_UINT8*)&stLogSetCnf ;
        cnf_data_len = sizeof(DIAG_BSP_PRINT_LOG_SWT_CNF_STRU);
    }
    else if(DIAG_CMD_BSP_LOG_SET_CCORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId)))
    {
        ret = ERR_MSP_SUCCESS;
    }
    else
    {
        ret = ERR_MSP_FAILURE;
    }

    if(VOS_NULL!=pstCnf)
    {
        ret = diag_AgentCnfFun(pstCnf,ulCmdId,cnf_data_len);
    }

    return ret;
}


/*****************************************************************************
 Function Name   : diag_BspSysviewProcEntry
 Description     : �ú���Ϊ����DIAG FW��������BSP sysview��������Ĵ������
 Input           : pstReq ����������
                   		ulCmdId ����ID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.h00135900      2013-03-06  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_BspSysviewProcEntry(VOS_UINT8* pstReq , VOS_UINT32 ulCmdId)
{
    VOS_UINT32  ret ;
    VOS_UINT32 cnf_data_len = 0;
    MSP_DIAG_HEAD_STRU *pstDiagHead         = VOS_NULL;
    DIAG_BSP_SYVIEW_SWT_CFG_STRU *pstSysviewSet = VOS_NULL;
    DIAG_BSP_SYVIEW_SWT_CNF_STRU  *pstLogSysviewCnf  = pstLogSysviewCnf;
    VOS_UINT8 *pstCnf = VOS_NULL;

    pstDiagHead = (MSP_DIAG_HEAD_STRU*)(DIAG_OFFSET_SOCP_GET_DIAG_HEAD(pstReq));

    if((DIAG_CMD_BSP_SYSVIEW_SWT_ACORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId)))
        ||(DIAG_CMD_BSP_CPU_SWT_ACORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))))
    {
        pstSysviewSet = (DIAG_BSP_SYVIEW_SWT_CFG_STRU *)DIAG_OFFSET_HEAD_GET_DATA(pstReq);

        ret  = DRV_SYSVIEW_SWT_SET(pstSysviewSet->trace_type,pstSysviewSet->trace_swt,pstSysviewSet->period);

        if(ret  == ERR_MSP_SUCCESS)
        {
            if((pstSysviewSet->trace_type ==SYSVIEW_TASK_INFO )&&(pstSysviewSet->trace_swt ==1 ))
            {
                cnf_data_len = sizeof(DIAG_BSP_SYVIEW_SWT_CNF_STRU)+sizeof(DIAG_BSP_TASK_INFO_STRU)*128;
                pstLogSysviewCnf =(DIAG_BSP_SYVIEW_SWT_CNF_STRU*)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,cnf_data_len);
                pstLogSysviewCnf->ullen = sizeof(DIAG_BSP_TASK_INFO_STRU)*128;
                DRV_GET_ALL_TASK_ID_NAME(&(pstLogSysviewCnf->st_task_info), sizeof(DIAG_BSP_TASK_INFO_STRU)*128);
            }
            else
            {
                cnf_data_len = sizeof(DIAG_BSP_SYVIEW_SWT_CNF_STRU);
                pstLogSysviewCnf =(DIAG_BSP_SYVIEW_SWT_CNF_STRU*)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,cnf_data_len);
                pstLogSysviewCnf->ullen  = 0;

            }

        }
        else
        {
            cnf_data_len = sizeof(DIAG_BSP_SYVIEW_SWT_CNF_STRU);
            pstLogSysviewCnf =(DIAG_BSP_SYVIEW_SWT_CNF_STRU*)VOS_MemAlloc(MSP_PID_DIAG_APP_AGENT, DYNAMIC_MEM_PT,cnf_data_len);
            pstLogSysviewCnf->ullen  = 0;

        }

        pstLogSysviewCnf->ulRet = ret;
        pstLogSysviewCnf->trace_type =pstSysviewSet->trace_type;
        pstCnf = (VOS_UINT8*)pstLogSysviewCnf ;

    }
    else if((DIAG_CMD_BSP_SYSVIEW_SWT_CCORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId)))
                ||(DIAG_CMD_BSP_CPU_SWT_CCORE == (MSP_STRU_ID_0_15_CMD_CATEGORY(ulCmdId))))
    {
        ret = ERR_MSP_SUCCESS;
    }
    else
    {
        ret = ERR_MSP_INVALID_PARAMETER;
    }

    if(VOS_NULL!=pstCnf)
    {
        ret = diag_AgentCnfFun(pstCnf,ulCmdId,cnf_data_len);
         VOS_MemFree(diag_GetAgentPid(), pstCnf);
    }

    return ret;
}
#endif

/***********************************************************************
 Function Name   : diag_AppAgentMsgProcInit
 Description     : DIAG APP AGENT��ʼ��
 Input           :enum VOS_INIT_PHASE_DEFINE ip
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-7  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_AppAgentMsgProcInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ret = ERR_MSP_SUCCESS;

	return ret;
}


/*****************************************************************************
 Function Name   : diag_AppAgentMsgProc
 Description     : DIAG APP AGENT���յ�����Ϣ�������
                  0.	HSO���ӹ����ࣨ0x5000-0x50ff ��
                  1.	�Ĵ��������ࣨ0x5100-0x51ff ��
                  2.	���ɲ�����     ��0x5200-0x52ff ��
                  3.	����������     ��0x5300-0x53ff ��
                  4.	�Զ�����          ��0x5400-0x54ff ��
                  5.	NV������            ��0x5500-0x55ff ��
 Input           :MsgBlock* pMsgBlock
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-11-19  Draft Enact

*****************************************************************************/
VOS_VOID diag_AppAgentMsgProc(MsgBlock* pMsgBlock)
{
    DIAG_DATA_MSG_STRU* pMsgTmp =NULL;
    REL_TIMER_MSG *pTimer =NULL;
    /*lint --e{740, 142} */
    /*����ж�*/
    if (NULL == pMsgBlock)
    {
        return;
    }

    /*����ʼ����������˯��*/
    drx_msp_fid_vote_lock(DRX_DIAG_APP_AGENT_VOTE);
     /*lint -save -e30*/
    /*���ݷ���PID��ִ�в�ͬ����*/
    switch(pMsgBlock->ulSenderPid)
    {
        case DOPRA_PID_TIMER:
            DIAG_DEBUG_SDM_FUN(EN_DIAG_DEBUG_APP_AGENT_TIME_OUT_ENTRY,0,0,0);
            pTimer   = (REL_TIMER_MSG*)pMsgBlock;
            diag_AppAgentTimeOutProcEntry((VOS_VOID*)(pTimer->ulPara));
            break;

            /*����HDLC��������� */
        case MSP_PID_DIAG_FW:
            pMsgTmp = (DIAG_DATA_MSG_STRU*)pMsgBlock;

            /*��������ID�ַ�����*/
            diag_AppAgentDispatchCmd(pMsgTmp->pContext);
            break;

        case PS_PID_OM:
		case PS_PID_ESM:
		case PS_PID_PDCP_UL:
		case PS_PID_MM:
        case PS_PID_ERRC:
        /*lint -restore +e30*/
            diag_PsTransCnfProc((VOS_UINT8*)pMsgBlock, (pMsgBlock->ulLength + VOS_MSG_HEAD_LENGTH));
            break;

        default:
            break;

   }

   /*����ʼ����������˯��*/
   drx_msp_fid_vote_unlock(DRX_DIAG_APP_AGENT_VOTE);

   return ;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

