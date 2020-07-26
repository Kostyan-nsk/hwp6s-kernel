/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : diag_Common.c
  Description     :
  History         :
     1.w00182550       2012-11-20   Draft Enact

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "diag_common.h"
#include  "msp_errno.h"
#include  "diag_debug.h"
#include  "DrvInterface.h"
#include  "soc_baseaddr_interface.h"
#define    THIS_FILE_ID        MSP_FILE_ID_DIAG_COMMON_C


/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
DIAG_LOG_PKT_NUM_ACC_STRU g_DiagLogPktNum ={0};


#define DIAG_REG_ADDR_MASK          0xff000000

/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_UINT32 diag_GetAgentPid(VOS_VOID)
{
#if(VOS_OS_VER == VOS_LINUX)
    return MSP_PID_DIAG_APP_AGENT;
#else
    return MSP_PID_DIAG_AGENT;
#endif
}

/*yuyangyang NV ��ȡimei�Žӿ� begin*/
/*****************************************************************************
 Function Name   : diag_GetImei
 Description     : ��ȡimei��
 Input           :VOS_VOID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784      2012-12-19  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_GetImei(VOS_CHAR szimei [ 16 ])
{
    VOS_UINT32  ret;
    VOS_UINT32  uslen = 0;
    VOS_UINT32  subscript = 0;
    VOS_CHAR   checkdata = 0;
    VOS_CHAR   auctemp[DIAG_NV_IMEI_LEN+1] = {0};

    uslen = DIAG_NV_IMEI_LEN+1;

    ret = NVM_Read(0, auctemp, uslen);

    if(ret != 0)
    {
        return ret;
    }
    else
    {
        for (subscript = 0; subscript < (DIAG_NV_IMEI_LEN - 1); subscript += 2)
        {
            checkdata += (VOS_CHAR)(((auctemp[subscript])
                           +((auctemp[subscript + 1] * 2) / 10))
                           +((auctemp[subscript + 1] * 2) % 10));
        }
        checkdata = (10 - (checkdata%10)) % 10;

        for (subscript = 0; subscript < uslen; subscript++)
        {
            *(szimei + subscript) = *(auctemp + subscript) + 0x30; /*�ַ�ת��*/
        }

        szimei[DIAG_NV_IMEI_LEN - 1] = checkdata + 0x30;
        szimei[DIAG_NV_IMEI_LEN] = 0;
    }

    return 0;
}
/*yuyangyang NV ��ȡimei�Žӿ� end*/

/*****************************************************************************
 Function Name   : diag_GetFrameTime
 Description     : ��ȡʱ���
 Input           :VOS_VOID
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-19  Draft Enact

*****************************************************************************/
VOS_UINT64 diag_GetFrameTime(VOS_VOID)
{
    VOS_UINT64 ulFrameCount = 0;
    VOS_UINT32 ret;

    ret = BSP_BBPGetCurTime(&ulFrameCount);
    if(ERR_MSP_SUCCESS != ret)
    {
       ulFrameCount = 0;
    }
    return ulFrameCount;
}

/*****************************************************************************
 Function Name   : diag_RegRead
 Description     : �Ĵ������ӿ�
 Input           :VOS_UINT32 u32RegAddr
                ENADDRTYPE enAddrType
                VOS_UINT32 *pu32Value
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2013-3-25  Draft Enact

*****************************************************************************/
VOS_INT32 diag_RegRead(VOS_UINT32 u32RegAddr,ENADDRTYPE enAddrType, VOS_UINT32 *pu32Value)
{
    if(   ((DIAG_REG_ADDR_MASK & u32RegAddr) != (DIAG_REG_ADDR_MASK & DRV_GET_IP_BASE_ADDR(BSP_IP_TYPE_SOCP)))
        &&((DIAG_REG_ADDR_MASK & u32RegAddr) != (DIAG_REG_ADDR_MASK & SOC_BBP_LTE0_BASE_ADDR))
        )
    {
        diag_printf("diag_RegRead invalid parameter :0x%x\n",u32RegAddr);
        return ERROR;
    }
    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *pu32Value = *((volatile VOS_UINT8 *)u32RegAddr);
            return OK;

        case ADDRTYPE16BIT:
            *pu32Value = *((volatile VOS_UINT16 *)u32RegAddr);
            return OK;

        case ADDRTYPE32BIT:
            *pu32Value = *((volatile VOS_UINT32 *)u32RegAddr);
            return OK;

        default:
            return ERROR;
    }
}

/*****************************************************************************
 Function Name   : diag_RegWrite
 Description     : �Ĵ���д�ӿ�
 Input           :VOS_UINT32 u32RegAddr
                ENADDRTYPE enAddrType
                VOS_UINT32 u32Value
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2013-3-25  Draft Enact

*****************************************************************************/
VOS_INT32 diag_RegWrite(VOS_UINT32 u32RegAddr,ENADDRTYPE enAddrType, VOS_UINT32 u32Value)
{
    if(   ((DIAG_REG_ADDR_MASK & u32RegAddr) != (DIAG_REG_ADDR_MASK & DRV_GET_IP_BASE_ADDR(BSP_IP_TYPE_SOCP)))
        &&((DIAG_REG_ADDR_MASK & u32RegAddr) != (DIAG_REG_ADDR_MASK & SOC_BBP_LTE0_BASE_ADDR))
        )
    {
        diag_printf("diag_RegRead invalid parameter :0x%x\n",u32RegAddr);
        return ERROR;
    }
    switch(enAddrType)
    {
        case ADDRTYPE8BIT:
            *((volatile VOS_UINT8 *)u32RegAddr) = (VOS_UINT8)u32Value;
            return OK;

        case ADDRTYPE16BIT:
            *((volatile VOS_UINT16 *)u32RegAddr) = (VOS_UINT16)u32Value;
            return OK;

        case ADDRTYPE32BIT:
            *((volatile VOS_UINT32 *)u32RegAddr) = u32Value;
            return OK;

        default:
            return ERROR;
    }
}


/*****************************************************************************
 Function Name   : diag_SendMsg
 Description     : DIAG��ģ�鷢��Ϣ��װ�ӿ�
 Input           :VOS_UINT32 ulSenderId
                VOS_UINT32 ulRecverId
                VOS_UINT32 ulMsgId
                VOS_VOID* pDta
                VOS_UINT32 dtaSize
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-11-20  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_SendMsg(VOS_UINT32 ulSenderId, VOS_UINT32 ulRecverId, VOS_UINT32 ulMsgId, VOS_UINT8* pDta, VOS_UINT32 dtaSize)
{
    VOS_UINT32 ret = ERR_MSP_UNKNOWN;

    DIAG_DATA_MSG_STRU* pDataMsg = NULL;
    /*lint -save -e740*/
    pDataMsg = (DIAG_DATA_MSG_STRU*)VOS_AllocMsg(ulSenderId,(sizeof(DIAG_DATA_MSG_STRU)+ dtaSize - 20));
    /*lint -restore*/

    if (pDataMsg != NULL)
    {
        pDataMsg->ulReceiverPid = ulRecverId;
        pDataMsg->ulSenderPid   = ulSenderId;
        pDataMsg->ulLength      = sizeof(DIAG_DATA_MSG_STRU)+ dtaSize - 20;

        pDataMsg->ulMsgId    = ulMsgId;
        pDataMsg->ulLen      = dtaSize;
        VOS_MemCpy(pDataMsg->pContext,pDta,dtaSize);

        ret = VOS_SendMsg(ulSenderId, pDataMsg);
        if (ret != VOS_OK)
        {
            /*lint -e717*/
			diag_printf("diag_SendMsg ulSenderId=%d,ulRecverId=%d,ulMsgId=0x%x,pDta=0x%x,dtaSize=%d!\n",ulSenderId,ulRecverId,ulMsgId,pDta,dtaSize);
            /*lint +e717*/

        }
        else
        {
            ret = ERR_MSP_SUCCESS;
        }
    }
    return ret;
}

VOS_UINT32 diag_SendPsTransMsg(VOS_UINT32 ulRecverId, VOS_UINT8* pDta, VOS_UINT32 dtaSize)
{
    VOS_UINT32 ret = ERR_MSP_UNKNOWN;

    DIAG_PS_MSG_STRU* pDataMsg = NULL;

	/*��������dtaSize�Ѿ�������VOS HEADER���������ʱ��Ҫ-20*/
    /*lint -save -e740*/
    pDataMsg = (DIAG_PS_MSG_STRU*)VOS_AllocMsg(diag_GetAgentPid(),dtaSize -VOS_MSG_HEAD_LENGTH);
    /*lint -restore*/
    if (pDataMsg)
    {

        VOS_MemCpy(pDataMsg,pDta,dtaSize);
        pDataMsg->ulReceiverPid = ulRecverId;
        pDataMsg->ulSenderPid   = diag_GetAgentPid();
        pDataMsg->ulLength      = dtaSize - VOS_MSG_HEAD_LENGTH;
        /*lint -e717*/
        ret = VOS_SendMsg(diag_GetAgentPid(), pDataMsg);
        if (ret != VOS_OK)
        {
			diag_printf(" ulRecverId=%d,pDta=0x%x,dtaSize=%d!\n",ulRecverId,pDta,dtaSize);
        }
        /*lint +e717*/
        else
        {
            ret = ERR_MSP_SUCCESS;
        }
    }
    return ret;
}

/*****************************************************************************
 Function Name   : diag_AgentCnfFun
 Description     : �ú�������AGENT��DIAG FW�ظ�
 Input           :VOS_UINT8* pstCmdCnf
                VOS_UINT32 ulCmdId
                VOS_UINT8 ulDataSize
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-10  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_AgentCnfFun(VOS_UINT8* pstCmdCnf,VOS_UINT32 ulCmdId,VOS_UINT32 ulDataSize )
{
    DIAG_CMD_AGENT_TO_FW_CNF_STRU* pstCnf = NULL;
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    VOS_UINT32 ulSendSize =0;

    ulSendSize = sizeof(DIAG_CMD_AGENT_TO_FW_CNF_STRU)+ ulDataSize;

    pstCnf = VOS_MemAlloc(diag_GetAgentPid(),DYNAMIC_MEM_PT,ulSendSize);

    if(NULL != pstCnf)
    {
        pstCnf->ulCmdId = ulCmdId;
        VOS_MemCpy(pstCnf->aucData, pstCmdCnf, ulDataSize);
        /*lint -save -e40*/
#if(VOS_OS_VER == VOS_LINUX)
        ret = diag_SendMsg(diag_GetAgentPid(),MSP_PID_DIAG_FW,ID_MSG_DIAG_CMD_CNF_APP_AGENT_TO_FW,\
                  (VOS_UINT8*)pstCnf,ulSendSize);
#else
        ret = diag_SendMsg(diag_GetAgentPid(),MSP_PID_DIAG_FW,ID_MSG_DIAG_CMD_CNF_AGENT_TO_FW,\
                  (VOS_UINT8*)pstCnf,ulSendSize);
#endif
        /*lint -restore  +e40*/
        VOS_MemFree(diag_GetAgentPid(), pstCnf);
        return ret;
	}
    /*lint -e717*/
    else
    {
    	 diag_printf("[%s]: malloc fail!\n",__FUNCTION__);
         return ERR_MSP_MALLOC_FAILUE;
    }
    /*lint +e717*/

    return ret;/*lint !e527 */
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



