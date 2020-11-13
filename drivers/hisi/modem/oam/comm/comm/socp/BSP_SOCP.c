/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  BSP_SOCP.c
*
*   ��    �� :  yangzhi
*
*   ��    �� :  ���ļ���Ҫ���socpͨ������������ã��Լ���д���ݴ���
*
*   �޸ļ�¼ :  2011��2��19��  v1.00  yangzhi  ����
*************************************************************************/
#include "BSP_SOCP_DRV.h"
#include "DrvInterface.h"
#include "OamSpecTaskDef.h"
#include "FileSysInterface.h"
#include "omnvinterface.h"
#include "NVIM_Interface.h"
#include "msp_cfg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -e767  �޸���: m00128685�������ˣ�l46160 ԭ�����:�����־�ļ���ID����*/
#define    THIS_FILE_ID        PS_FILE_ID_SOCP_C
/*lint +e767  �޸���: m00128685�������ˣ�l46160 */

SOCP_GBL_STATE                  g_strSocpStat;    /* ��¼��ǰȫ��״̬��ͨ��������Ϣ */

SOCP_DEBUG_INFO_S               g_stSocpDebugInfo;      /* ��¼��SOCP����״̬�еĹ�����Ϣ */

SOCP_LOG_INFO_STRU              g_stSocpLog;            /* ��¼��SOCP����״̬�е�log��Ϣ */

VOS_SPINLOCK                    g_stSocpSpinLock;       /* ��������������SOCP�ٽ���Դ���� */

#ifdef SOCP_ACORE
SOCP_ENC_DST_BUF_LOG_CFG_STRU   g_stEncDstBufLogConfig; /* LOG2.0 SOCPˮ�ߡ���ʱ���� */

VOS_SPINLOCK                    g_stSocpFlushSpinLock;  /* ��������������SOCP Flush EncDst �ٽ���Դ���� */

HTIMER                          g_SocpSDLogTimerId;
#endif

/*lint -e662 -e661*/
/*****************************************************************************
* �� �� ��  : OM_Inner_Log
*
* ��������  : ��¼OM���ڲ�log����
*
* �������  :  pucStr       OM������ַ�����Ϣ
               ulP1         �������1
               ulP2         �������2
               pstLogData   �������ݵ�Log������
               ulFileID     �������ݲ������ļ�
               lLineNo      �������ݲ������к�
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
VOS_VOID OM_Inner_Log(VOS_CHAR *pucStr, VOS_UINT32 ulP1, VOS_UINT32 ulP2,
                          INNER_LOG_DATA_STRU *pstLogData ,VOS_UINT32 ulFileID, VOS_INT32 lLineNo)
{
    VOS_UINT32  ulCounter;

#ifdef OM_DEBUG_PRINT
    vos_printf("\r\n T:0x%x ", OM_GetSlice());
    vos_printf("%s ",pucStr);
    vos_printf("F:%d,L:%d,P1:0x%x,P2:0x%x",ulFileID,lLineNo,ulP1,ulP2);
#endif  /* end of OM_DEBUG_PRINT */

    ulCounter = pstLogData->ulCnt;

    pstLogData->astLogData[ulCounter].ulFileNO  = ulFileID;
    pstLogData->astLogData[ulCounter].lLineNO   = lLineNo;
    pstLogData->astLogData[ulCounter].ulP1      = ulP1;
    pstLogData->astLogData[ulCounter].ulP2      = ulP2;
    pstLogData->astLogData[ulCounter].ulSlice   = OM_GetSlice();

    pstLogData->ulCnt                           = (ulCounter+1)%INNER_LOG_DATA_MAX;

    return;
}

/*****************************************************************************
* �� �� ��  : SOCP_REG_SAVE
*
* ��������  : ��������쳣�����浱ǰ��SOCP�ļĴ�����Ϣ
*
* �������  :  ulErrorNO       �쳣�����
               ulPara          �������
               pucData         ��������
               ulDataLen       �������ݳ���
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
VOS_VOID SOCP_Log_Save(cb_buf_t *pstData)
{
    VOS_UINT8       *pucData;
    VOS_UINT32      ulDataLen;

    if(VOS_TRUE != g_strSocpStat.bInitFlag) /*��ʼ��δ��ɲ�����*/
    {
        pstData->ulDataLen = 0;
        VOS_MemSet(pstData->aucName, 0, EXCH_CB_NAME_SIZE);

        return;
    }

    /* ��д������Ϣ */
    VOS_StrNCpy(pstData->aucName, "SOCP LOG Info", EXCH_CB_NAME_SIZE);

    /* ������Ҫ�����logȫ�ֱ����Ĵ�С,�������ڴ�Ϊ�ָ����ֶ� */
    ulDataLen = SOCP_REGISTER_SIZE + sizeof(g_strSocpStat)+sizeof(g_stSocpLog)+(3*sizeof(VOS_UINT32));

    /* �����ڴ� */
#ifdef OAM_UT_DEBUG     /* only for UT */
    pucData = (VOS_UINT8*)VOS_MemAlloc(0,0,ulDataLen);
#else
#if (VOS_OS_VER == VOS_LINUX)
    pucData = (VOS_UINT8*)kmalloc(ulDataLen, GFP_KERNEL);
#elif (VOS_OS_VER == VOS_RTOSCK)
    pucData = (VOS_UINT8*)SRE_MemCacheAlloc(ulDataLen, MEM_ADDR_ALIGN_004);
#else
    pucData = (VOS_UINT8*)malloc(ulDataLen);
#endif  /*(VOS_OS_VER == VOS_LINUX)*/
#endif  /*OAM_UT_DEBUG*/

    /*�ڴ�����ʧ�ܱ��沿����Ϣ*/
    if(VOS_NULL_PTR == pucData)
    {
        pstData->pucData    = (VOS_UINT8*)&g_stSocpLog;
        pstData->ulDataLen  = sizeof(g_stSocpLog);
    }
    else    /*������������*/
    {
        /*lint -e810*/
        pstData->pucData    = pucData;

        pstData->ulDataLen  = ulDataLen;

        /*���浱ǰIP REG��Ϣ*/
        VOS_MemSet(pucData, SOCP_ERROR_INFO_TAG, sizeof(VOS_UINT32));

        pucData += sizeof(VOS_UINT32);

        VOS_MemCpy(pucData, (unsigned char *)g_strSocpStat.u32BaseAddr, SOCP_REGISTER_SIZE);

        pucData += SOCP_REGISTER_SIZE;

        /*���浱ǰͨ��������Ϣ*/
        VOS_MemSet(pucData, SOCP_ERROR_INFO_TAG, sizeof(VOS_UINT32));

        pucData += sizeof(VOS_UINT32);

        VOS_MemCpy(pucData, &g_strSocpStat, sizeof(g_strSocpStat));

        pucData += sizeof(g_strSocpStat);

        /*���浱ǰͨ��log��Ϣ*/
        VOS_MemSet(pucData, SOCP_ERROR_INFO_TAG, sizeof(VOS_UINT32));

        pucData += sizeof(VOS_UINT32);

        VOS_MemCpy(pucData, &g_stSocpLog, sizeof(g_stSocpLog));
        /*lint +e810*/
    }

    return; /*���أ�������������Ҫ�ͷ��ڴ�*/
}

/*****************************************************************************
* �� �� ��  : SocpGetIdleBuffer
*
* ��������  : ��ѯ���л�����
*
* �������  :  pRingBuffer       ����ѯ�Ļ���buffer
               pRWBuffer         ����Ļ���buffer
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
BSP_VOID SocpGetIdleBuffer(SOCP_RING_BUF_S *pRingBuffer, SOCP_BUFFER_RW_S *pRWBuffer)
{
    if(pRingBuffer->u32Write < pRingBuffer->u32Read)
    {
        /* ��ָ�����дָ�룬ֱ�Ӽ��� */
        pRWBuffer->pBuffer = (BSP_CHAR *)(pRingBuffer->u32Write);
        pRWBuffer->u32Size = (BSP_U32)(pRingBuffer->u32Read - pRingBuffer->u32Write - 1);
        pRWBuffer->pRbBuffer = (BSP_CHAR *)BSP_NULL;
        pRWBuffer->u32RbSize = 0;
    }
    else
    {
        /* дָ����ڶ�ָ�룬��Ҫ���ǻؾ� */
        if(pRingBuffer->u32Read != pRingBuffer->u32Start)
        {
            pRWBuffer->pBuffer = (BSP_CHAR *)(pRingBuffer->u32Write);
            pRWBuffer->u32Size = (BSP_U32)(pRingBuffer->u32End - pRingBuffer->u32Write + 1);
            pRWBuffer->pRbBuffer = (BSP_CHAR *)(pRingBuffer->u32Start);
            pRWBuffer->u32RbSize = (BSP_U32)(pRingBuffer->u32Read - pRingBuffer->u32Start - 1);
        }
        else
        {
            pRWBuffer->pBuffer = (BSP_CHAR *)(pRingBuffer->u32Write);
            pRWBuffer->u32Size = (BSP_U32)(pRingBuffer->u32End - pRingBuffer->u32Write);
            pRWBuffer->pRbBuffer = (BSP_CHAR *)BSP_NULL;
            pRWBuffer->u32RbSize = 0;
        }
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpGetDataBuffer
*
* ��������  : ��ȡ���л�����������
*
* �������  :  pRingBuffer       ����ѯ�Ļ���buffer
               pRWBuffer         ����Ļ���buffer
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
BSP_VOID SocpGetDataBuffer(SOCP_RING_BUF_S *pRingBuffer, SOCP_BUFFER_RW_S *pRWBuffer)
{
    if (pRingBuffer->u32Read <= pRingBuffer->u32Write)
    {
        /* дָ����ڶ�ָ�룬ֱ�Ӽ��� */
        pRWBuffer->pBuffer   = (BSP_CHAR *)(pRingBuffer->u32Read);
        pRWBuffer->u32Size   = (BSP_U32)(pRingBuffer->u32Write - pRingBuffer->u32Read);
        pRWBuffer->pRbBuffer = (BSP_CHAR *)BSP_NULL;
        pRWBuffer->u32RbSize = 0;
    }
    else
    {
        /* ��ָ�����дָ�룬��Ҫ���ǻؾ� */
        pRWBuffer->pBuffer   = (BSP_CHAR *)(pRingBuffer->u32Read);
        pRWBuffer->u32Size   = (BSP_U32)(pRingBuffer->u32End - pRingBuffer->u32Read + 1);
        pRWBuffer->pRbBuffer = (BSP_CHAR *)(pRingBuffer->u32Start);
        pRWBuffer->u32RbSize = (BSP_U32)(pRingBuffer->u32Write - pRingBuffer->u32Start);
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpWriteDone
*
* ��������  : ���»�������дָ��
*
* �������  :  pRingBuffer       �����µĻ���buffer
               u32Size           ���µ����ݳ���
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
BSP_VOID SocpWriteDone(SOCP_RING_BUF_S *pRingBuffer, BSP_U32 u32Size)
{
    BSP_U32                             tmp_size;
    BSP_U32                             rb_size;

    tmp_size = pRingBuffer->u32End - pRingBuffer->u32Write + 1;

    if (tmp_size > u32Size)
    {
        (BSP_VOID)SOCP_FLUSH_CACHE(pRingBuffer->u32Write, u32Size);
        pRingBuffer->u32Write += u32Size;
    }
    else
    {
        rb_size = u32Size - tmp_size;

        (BSP_VOID)SOCP_FLUSH_CACHE(pRingBuffer->u32Write, tmp_size);
        (BSP_VOID)SOCP_FLUSH_CACHE(pRingBuffer->u32Start, rb_size);
        pRingBuffer->u32Write = pRingBuffer->u32Start + rb_size;
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpReadDone
*
* ��������  : ���»������Ķ�ָ��
*
* �������  :  pRingBuffer       �����µĻ���buffer
               u32Size           ���µ����ݳ���
*
* �������  : ��
*
* �� �� ֵ  :  ��
*****************************************************************************/
BSP_VOID SocpReadDone(SOCP_RING_BUF_S *pRingBuffer, BSP_U32 u32Size)
{
    pRingBuffer->u32Read += u32Size;

    if (pRingBuffer->u32Read > pRingBuffer->u32End)
    {
        pRingBuffer->u32Read -= pRingBuffer->u32Length;
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SOCP_CleanEncSrcChan
*
* ��������  : ��ձ���Դͨ��
*
* �������  : enSrcChanID       ����ͨ����
*
* �������  : VOS_OK/BSP_ERR_SOCP_INVALID_CHAN
*
* �� �� ֵ  : �ͷųɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 SOCP_CleanEncSrcChan(SOCP_CODER_SRC_ENUM_U32 enSrcChanID)
{
    VOS_UINT32                          ulResetFlag;
    VOS_UINT32                          i;
    VOS_UINT32                          ulChanID;
    VOS_UINT32                          ulChanType;

    ulChanID        = SOCP_REAL_CHAN_ID(enSrcChanID);
    ulChanType      = SOCP_REAL_CHAN_TYPE(enSrcChanID);

    SOCP_CHECK_CHAN_TYPE(ulChanType, SOCP_CODER_SRC_CHAN);

    /* ��λͨ�� */
    SOCP_REG_SETBITS(SOCP_REG_ENCRST, ulChanID, 1, 1);

    /* �ȴ�ͨ������ */
    for(i=0; i< SOCP_RESET_TIME; i++)
    {
        ulResetFlag = SOCP_REG_GETBITS(SOCP_REG_ENCRST, ulChanID, 1);

#ifndef OAM_UT_DEBUG     /* only for UT */
        if(0 == ulResetFlag)
        {
            break;
        }
#endif
    }

    if(SOCP_RESET_TIME == i)
    {
        SOCP_LOG_WARNING("SocpCleanEncChan failed!\n", ulChanID, 0);
    }

    return VOS_OK;
}


#ifdef SOCP_ACORE
/*****************************************************************************
* �� �� ��  : SocpGetDecRdSize
*
* ��������  :  ��ȡ����Դͨ��RDbuffer
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : �ͷųɹ����ı�ʶ��
*****************************************************************************/
BSP_U32 SocpGetDecRdSize(BSP_U32 u32ChanID)
{
    SOCP_BUFFER_RW_S                    Buff;
    BSP_U32                             PAddr;

    SOCP_REG_READ(SOCP_REG_DECSRC_RDQRPTR(u32ChanID), PAddr);

    g_strSocpStat.sDecSrcChan[u32ChanID].sDecRdBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(PAddr);

    SOCP_REG_READ(SOCP_REG_DECSRC_RDQWPTR(u32ChanID), PAddr);

    g_strSocpStat.sDecSrcChan[u32ChanID].sDecRdBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(PAddr);

    SocpGetDataBuffer(&g_strSocpStat.sDecSrcChan[u32ChanID].sDecRdBuf, &Buff);
    return (Buff.u32Size + Buff.u32RbSize);
}

/*****************************************************************************
* �� �� ��  : SocpGetEncRdSize
*
* ��������  :  ��ȡ����Դͨ��RDbuffer
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : �ͷųɹ����ı�ʶ��
*****************************************************************************/
BSP_U32 SocpGetEncRdSize(BSP_U32 u32ChanID)
{
    SOCP_BUFFER_RW_S Buff;
    BSP_U32          PAddr;

    SOCP_REG_READ(SOCP_REG_ENCSRC_RDQRPTR(u32ChanID), PAddr);

    g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(PAddr);

    SOCP_REG_READ(SOCP_REG_ENCSRC_RDQWPTR(u32ChanID), PAddr);

    g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(PAddr);

    SocpGetDataBuffer(&g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf, &Buff);

    return (Buff.u32Size + Buff.u32RbSize);
}

/*****************************************************************************
* �� �� ��   : SocpEncSrcRdHandler
*
* ��������  :  ����Դͨ��RDbuffer�жϴ�����
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpEncSrcRdHandler(BSP_U32 RdSize, BSP_U32 i)
{
    BSP_U32 u32ChanId;

    if (RdSize == g_strSocpStat.sEncSrcChan[i].u32LastRdSize)
    {
        if (g_strSocpStat.sEncSrcChan[i].rd_cb)
        {
            u32ChanId = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN, i);
            (BSP_VOID)g_strSocpStat.sEncSrcChan[i].rd_cb(u32ChanId);

            g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpEncSrcTskRdCbCnt[i]++;
        }

        g_strSocpStat.sEncSrcChan[i].u32LastRdSize = 0;
    }
    else
    {
        g_strSocpStat.sEncSrcChan[i].u32LastRdSize = RdSize;
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpEncSrcTask
*
* ��������  : ģ��������:����Դ�жϣ�˫��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpEncSrcTask(BSP_VOID)
{
    BSP_U32                             i;
    BSP_U32                             IntHeadState = 0;
    BSP_U32                             u32ChanId;
    VOS_UINT32                          ulLockLevel = 0;

    for (;;)
    {
        if (VOS_OK != VOS_SmP(g_strSocpStat.u32EncSrcSemID, 0))
        {
#ifdef OAM_UT_DEBUG
            return;
#endif
            continue;
        }

        /*s32LockKey                          = VOS_SplIMP();*/     /* ���ж� */
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        IntHeadState                        = g_strSocpStat.u32IntEncSrcHeader;
        g_strSocpStat.u32IntEncSrcHeader    = 0;
        g_strSocpStat.u32IntEncSrcRD        = 0;

        /*VOS_Splx(s32LockKey);*/                                   /* �����ж� */
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        /* ��������ͷ'HISI'������� */
        if (IntHeadState)
        {
            for (i=0; i<SOCP_MAX_ENCSRC_CHN; i++)
            {
                /* ���ͨ���Ƿ����� */
                if (SOCP_CHN_SET != g_strSocpStat.sEncSrcChan[i].u32SetStat)
                {
                    continue;
                }

                if (IntHeadState & ((BSP_U32)1 << i))
                {
                    g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpEncSrcTskHeadCbOriCnt[i]++;

                    u32ChanId = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN, i);

                    SOCP_LOG_ERR("SocpEncSrcTask Channel Data Head Error", u32ChanId, 0);

                    if (g_strSocpStat.sEncSrcChan[i].event_cb)
                    {
                        (BSP_VOID)g_strSocpStat.sEncSrcChan[i].event_cb(u32ChanId, SOCP_EVENT_PKT_HEADER_ERROR, 0);
                    }

                    g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpEncSrcTskHeadCbCnt[i]++;
                }
            }
        }

#ifdef OAM_UT_DEBUG
        return;
#endif
    }
}

/*****************************************************************************
* �� �� ��  : SocpEncDstTransHandle
*
* ��������  : ģ��������:����Ŀ�ģ�App��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
VOS_VOID SocpEncDstTransHandle(VOS_UINT32 ulIntTfrState)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulCBTime = 0;
    VOS_UINT32                          ulChanId;

    for (i=0; i<SOCP_MAX_ENCDST_CHN; i++)
    {
        /* ���ͨ���Ƿ�����*/
        if (SOCP_CHN_SET != g_strSocpStat.sEncDstChan[i].u32SetStat)
        {
            continue;
        }

        if (0 != (ulIntTfrState & (VOS_UINT32)(1 << i)))
        {
            g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstTskTrfCbOriCnt[i]++;

            if (VOS_NULL_PTR != g_strSocpStat.sEncDstChan[i].read_cb)
            {
                ulChanId    = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, i);

                ulCBTime    = OM_GetSlice();

                (BSP_VOID)g_strSocpStat.sEncDstChan[i].read_cb(ulChanId);

                ulCBTime    = OM_GetSlice()-ulCBTime;     /* ��ȡ�ص�������ִ��ʱ�� */

                SOCP_LOG_INFO("SocpEncDstTask Dst Channel CallBack",ulChanId, ulCBTime);
            }

            g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstTskTrfCbCnt[i]++;
        }
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpEncDstThOvfHandle
*
* ��������  : ģ��������:����Ŀ�ģ�App��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
VOS_VOID SocpEncDstThOvfHandle(VOS_UINT32 ulIntThresholdOvfState)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulCBTime = 0;
    VOS_UINT32                          ulChanId;

    for (i=0; i<SOCP_MAX_ENCDST_CHN; i++)
    {
        /* ���ͨ���Ƿ�����*/
        if (SOCP_CHN_SET != g_strSocpStat.sEncDstChan[i].u32SetStat)
        {
            continue;
        }

        if (0 != (ulIntThresholdOvfState & (VOS_UINT32)(1 << (i+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN))))
        {
            g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstTskThresholdOvfCbOriCnt[i]++;

            ulChanId = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, i);

            ulCBTime = OM_GetSlice();   /* ��ȡͨ�����ʱ�� */

            SOCP_LOG_WARNING("SocpEncDstTask Dst Channel Threshold Overflow", ulChanId, ulCBTime);

            if (VOS_NULL_PTR != g_strSocpStat.sEncDstChan[i].read_cb)
            {
                (BSP_VOID)g_strSocpStat.sEncDstChan[i].read_cb(ulChanId);

                ulCBTime    = OM_GetSlice()-ulCBTime;     /* ��ȡ�ص�������ִ��ʱ�� */

                SOCP_LOG_WARNING("SocpEncDstTask Dst Channel Overflow CallBack",ulChanId, ulCBTime);
            }

            g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstTskThresholdOvfCbCnt[i]++;
        }
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpGetNoEmptyEncDstChan
*
* ��������  : SOCP����Ŀ��ͨ���Ƿ�������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP_U8 0:������ ��0:��Ӧͨ����λ
*****************************************************************************/
VOS_UINT8 SocpGetNoEmptyEncDstChan(VOS_VOID)
{
    VOS_UINT8               ucChanSet = 0;
    SOCP_BUFFER_RW_S        pBuff;
    VOS_UINT32              i;

    for(i=0; i<SOCP_MAX_ENCDST_CHN; i++)
    {
        VOS_MemSet(&pBuff, 0x0, sizeof(SOCP_BUFFER_RW_S));

        if(BSP_OK != BSP_SOCP_GetReadBuff(SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, i), &pBuff))
        {
            continue;
        }

        if(pBuff.u32Size + pBuff.u32RbSize > 0)
        {
            ucChanSet = (VOS_UINT8)(ucChanSet | (1 << i));
        }
    }

    return ucChanSet;
}

/*****************************************************************************
* �� �� ��  : SocpEncDstFlushHandle
*
* ��������  : ģ��������:����Ŀ�ģ�App��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
VOS_VOID SocpEncDstFlushHandle(SOCP_ENC_DST_BUF_LOG_CFG_STRU       *pstcfg)
{
    VOS_UINT32                          i;
    VOS_UINT8                           chanSet = 0;
    VOS_UINT32                          ulMask;
    VOS_UINT32                          ulChanId;
    VOS_UINT32                          ulLockLevel;

    chanSet = SocpGetNoEmptyEncDstChan();

    if(VOS_NULL != chanSet)
    {
        g_stSocpDebugInfo.stSocpSDLogInfo.ulEncDstRunNum++;

        /* ���δ����ж� */
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        SOCP_REG_READ(SOCP_REG_ENC_MASK0, ulMask);

        SOCP_REG_WRITE(SOCP_REG_ENC_MASK0, (chanSet | ulMask));

        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        for (i = 0; i < SOCP_MAX_ENCDST_CHN; i++)
        {
            /* ���ͨ���Ƿ�����*/
            if (SOCP_CHN_SET == g_strSocpStat.sEncDstChan[i].u32SetStat)
            {
                if(chanSet & ((BSP_U8)1 << i))
                {
                    if (g_strSocpStat.sEncDstChan[i].read_cb)
                    {
                        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstSoftOverTimeOriCnt[i]++;

                        ulChanId = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, i);

                        (BSP_VOID)g_strSocpStat.sEncDstChan[i].read_cb(ulChanId);

                        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstSoftOverTimeCnt[i]++;
                    }
                }
            }
        }
    }

    if(VOS_TRUE == pstcfg->flushFlag)
    {
        VOS_SpinLockIntLock(&g_stSocpFlushSpinLock, ulLockLevel);

        pstcfg->flushFlag = VOS_FALSE;

        VOS_SpinUnlockIntUnlock(&g_stSocpFlushSpinLock, ulLockLevel);

        VOS_SmV(g_strSocpStat.u32EncDstFlushSemID);
    }

    return;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_RefreshSDLogCfg
*
* ��������  : ˢ��LOG2.0 SOCP��ʱ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : SOCP_ENC_DST_BUF_LOG_CFG_STRUָ��
*****************************************************************************/
VOS_VOID BSP_SOCP_RefreshSDLogCfg(VOS_UINT32 ulTimerLen)
{
    NV_SOCP_SDLOG_CFG_STRU                stSocpCfg ;

    VOS_MemSet(&stSocpCfg, 0, sizeof(stSocpCfg));

    if(NV_OK != NV_ReadEx(MODEM_ID_0, en_NV_ID_SOCP_SDLOG_CFG, &stSocpCfg, sizeof(NV_SOCP_SDLOG_CFG_STRU)))
    {
        return ;
    }

    if(VOS_TRUE != stSocpCfg.ulSocpDelayWriteFlg)
    {
        return ;
    }

    g_stEncDstBufLogConfig.overTime = ulTimerLen;

    VOS_StopRelTimer(&g_SocpSDLogTimerId);

    VOS_StartCallBackRelTimer(&g_SocpSDLogTimerId, ACPU_PID_OMAGENT, ulTimerLen,
                                SOCP_SDLOG_TIME_ID, 0, VOS_RELTIMER_NOLOOP,
                                BSP_SOCP_EncDstTimerHandler,VOS_TIMER_PRECISION_5);

    return ;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_GetSDLogCfg
*
* ��������  : ��ѯˢ�º�LOG2.0 SOCP��ʱ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : SOCP_ENC_DST_BUF_LOG_CFG_STRUָ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_GetSDLogCfg(VOS_UINT32 *pulTimerLen)
{
    if(VOS_NULL_PTR == pulTimerLen)
    {
        return VOS_ERR;
    }

    *pulTimerLen = g_stEncDstBufLogConfig.overTime;

    return VOS_OK;
}

/*****************************************************************************
* �� �� ��  : SocpInitLogCfg
*
* ��������  : ��ȡLOG2.0 SOCPˮ�ߡ���ʱ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : SOCP_ENC_DST_BUF_LOG_CFG_STRUָ��
*****************************************************************************/
VOS_VOID SocpInitSDLogCfg(VOS_VOID)
{
    NV_SOCP_SDLOG_CFG_STRU                stSocpCfg ;

    VOS_MemSet(&g_stEncDstBufLogConfig, 0, sizeof(g_stEncDstBufLogConfig));

    VOS_MemSet(&stSocpCfg, 0, sizeof(stSocpCfg));

    g_SocpSDLogTimerId = VOS_NULL;

    if(NV_OK != NV_ReadEx(MODEM_ID_0, en_NV_ID_SOCP_SDLOG_CFG, &stSocpCfg, sizeof(NV_SOCP_SDLOG_CFG_STRU)))
    {
        vos_printf("\r\n SocpInitSDLogCfg: Read NV Fail");

        return;
    }

    if(VOS_TRUE != stSocpCfg.ulSocpDelayWriteFlg)
    {
        return;
    }

    g_stEncDstBufLogConfig.flushFlag    = VOS_FALSE;
    g_stEncDstBufLogConfig.logOnFlag    = stSocpCfg.ulSocpDelayWriteFlg;
    g_stEncDstBufLogConfig.guWaterMark  = stSocpCfg.ulGuSocpLevel;
    g_stEncDstBufLogConfig.lWaterMark   = stSocpCfg.ulLSocpLevel;
    g_stEncDstBufLogConfig.overTime     = stSocpCfg.ulTimeOutValue;

    return;
}

/*****************************************************************************
* �� �� ��  : SocpGetLogCfg
*
* ��������  : ��ȡLOG2.0 SOCPˮ�ߡ���ʱ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : SOCP_ENC_DST_BUF_LOG_CFG_STRUָ��
*****************************************************************************/
SOCP_ENC_DST_BUF_LOG_CFG_STRU * SocpGetSDLogCfg(BSP_VOID)
{
    return &g_stEncDstBufLogConfig;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_EncDstBufFlush
*
* ��������  : SOCP����Ŀ��buffer���������������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP_S32 BSP_OK:�ɹ� BSP_ERROR:ʧ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_EncDstBufFlush(VOS_VOID)
{
    SOCP_ENC_DST_BUF_LOG_CFG_STRU   *stcfg;
    VOS_UINT32                      ulLockLevel;

    /* ��ȡ���� */
    stcfg = SocpGetSDLogCfg();

    /* ��һ��FLUSH������δ��ɻ���LOG2.0δ���� */
    if ((VOS_TRUE == stcfg->flushFlag) || (VOS_TRUE != stcfg->logOnFlag))
    {
        return VOS_ERR;
    }

    g_stSocpDebugInfo.stSocpSDLogInfo.ulFlushBufferNum++;

    /*����ȫ�ֱ��*/
    VOS_SpinLockIntLock(&g_stSocpFlushSpinLock, ulLockLevel);

    stcfg->flushFlag = VOS_TRUE;

    VOS_SpinUnlockIntUnlock(&g_stSocpFlushSpinLock, ulLockLevel);

    g_stSocpDebugInfo.stSocpEncDstSemInfo.ulFlushNum++;
    g_stSocpDebugInfo.stSocpEncDstSemInfo.ulFlushSlice = OM_GetSlice();

    /* ��encdst������SOCP���� */
    VOS_SmV(g_strSocpStat.u32EncDstSemID);

    if(VOS_OK != VOS_SmP(g_strSocpStat.u32EncDstFlushSemID, SOCP_LOG_FLUSH_MAX_OVER_TIME))
    {
        SOCP_LOG_WARNING("BSP_SOCP_EncDstBufFlush: flush time out", VOS_NULL, VOS_NULL);

        g_stSocpDebugInfo.stSocpSDLogInfo.ulFlushTimeOutNum++;

        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
* �� �� ��  : SocpEncDstTimerHandler
*
* ��������  : ����Ŀ��ͨ����ʱ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : BSP_VOID
*****************************************************************************/
VOS_VOID BSP_SOCP_EncDstTimerHandler(VOS_UINT32 ulParam, VOS_UINT32 ulTimerName)
{
    g_stSocpDebugInfo.stSocpSDLogInfo.ulTimeOutNum++;

    g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTimerNum++;
    g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTimerSlice = OM_GetSlice();

    VOS_SmV(g_strSocpStat.u32EncDstSemID);

    return;
}


/*****************************************************************************
* �� �� ��  : SocpEncDstTask
*
* ��������  : ģ��������:����Ŀ�ģ�App��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpEncDstTask(BSP_VOID)
{
    VOS_UINT32                          ulIntTfrState = 0;
    VOS_UINT32                          ulIntThresholdOvfState = 0;
    VOS_UINT32                          ulLockLevel;
    SOCP_ENC_DST_BUF_LOG_CFG_STRU       *pstcfg;
    VOS_UINT32                          ulSDLogFlag;

    pstcfg = SocpGetSDLogCfg();

    ulSDLogFlag = pstcfg->logOnFlag;

    for(;;)
    {
        if(VOS_TRUE == ulSDLogFlag)
        {
            VOS_StartCallBackRelTimer(&g_SocpSDLogTimerId, ACPU_PID_OMAGENT, pstcfg->overTime,
                                SOCP_SDLOG_TIME_ID, 0, VOS_RELTIMER_NOLOOP,
                                BSP_SOCP_EncDstTimerHandler,VOS_TIMER_PRECISION_5);
        }

        if(VOS_OK != VOS_SmP(g_strSocpStat.u32EncDstSemID, 0))
        {
#if defined(OAM_UT_DEBUG) || defined(DMT)   /*only for UT and ST*/
            return;
#endif
            g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskErrNum++;
            g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskErrSlice = OM_GetSlice();
        }

        g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskNum++;
        g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskSlice = OM_GetSlice();


        /*lLockKey                        = VOS_SplIMP();*/
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        ulIntTfrState                   = g_strSocpStat.u32IntEncDstTfr;
        g_strSocpStat.u32IntEncDstTfr   = 0;

        ulIntThresholdOvfState          = g_strSocpStat.u32IntEncDstThresholdOvf;
        ulIntThresholdOvfState = ulIntThresholdOvfState&((~ulIntTfrState)<<SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN);
        g_strSocpStat.u32IntEncDstThresholdOvf  = 0;

        /*VOS_Splx(lLockKey);*/
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        /* ������봫������ж�*/
        if (0 != ulIntTfrState)
        {
            SocpEncDstTransHandle(ulIntTfrState);
        }

        /* �������Ŀ�� buffer ��ֵ����ж�*/
        if (0 != ulIntThresholdOvfState)
        {
            SocpEncDstThOvfHandle(ulIntThresholdOvfState);
        }
        /* ��ʱ�������FLUSH���� */
        if ((VOS_TRUE == pstcfg->logOnFlag)
            && ((!ulIntTfrState  && !ulIntThresholdOvfState) /*���Ǵ����жϣ���������ж�*/
                || (VOS_TRUE == pstcfg->flushFlag)))
        {
            SocpEncDstFlushHandle(pstcfg);
        }

#if defined(OAM_UT_DEBUG) || defined(DMT)   /*only for UT and ST*/
        return;
#endif
    }
}

/*****************************************************************************
* �� �� ��  : SocpDecSrcEventHandler
*
* ��������  : ����Դͨ���¼�������
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID  SocpDecSrcEventHandler(BSP_U32 id, BSP_U32 secIntState)
{
    BSP_U32 u32ChanId = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, id);

    SOCP_LOG_WARNING("SocpDecSrcEventHandler Get Channel Err", u32ChanId, secIntState);

    if (g_strSocpStat.sDecSrcChan[id].event_cb)
    {
        if (secIntState & 0x10)
        {
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbOriCnt[id]++;

            (BSP_VOID)g_strSocpStat.sDecSrcChan[id].event_cb(u32ChanId, SOCP_EVENT_DECODER_UNDERFLOW, 0);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbCnt[id]++;
        }

        if (secIntState & 0x100)
        {
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbOriCnt[id]++;

            (BSP_VOID)g_strSocpStat.sDecSrcChan[id].event_cb(u32ChanId, SOCP_EVENT_HDLC_HEADER_ERROR, 0);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbCnt[id]++;
        }

        if (secIntState & 0x1000)
        {
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbOriCnt[id]++;

            (BSP_VOID)g_strSocpStat.sDecSrcChan[id].event_cb(u32ChanId, SOCP_EVENT_DATA_TYPE_ERROR, 0);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbCnt[id]++;
        }

        if (secIntState & 0x10000)
        {
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbOriCnt[id]++;

            (BSP_VOID)g_strSocpStat.sDecSrcChan[id].event_cb(u32ChanId, SOCP_EVENT_CRC_ERROR, 0);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbCnt[id]++;
        }

        if (secIntState & 0x100000)
        {
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbOriCnt[id]++;

            (BSP_VOID)g_strSocpStat.sDecSrcChan[id].event_cb(u32ChanId, SOCP_EVENT_PKT_LENGTH_ERROR, 0);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcTskErrCbCnt[id]++;
        }
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpDecSrcTask
*
* ��������  : ģ��������:����Դ��A��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpDecSrcTask(BSP_VOID)
{
    VOS_UINT32                          ulLockLevel;
    VOS_UINT32                          ulIntState;
    VOS_UINT32                          ulsecIntState;
    VOS_UINT32                          i;

    for(;;)
    {
        if(VOS_OK != VOS_SmP(g_strSocpStat.u32DecSrcSemID, 0))
        {
#if defined(OAM_UT_DEBUG) || defined(DMT)   /*only for UT and ST*/
            return;
#endif
            continue;
        }

        /*s32LockKey = VOS_SplIMP();*/
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        ulIntState = g_strSocpStat.u32IntDecSrcErr;

        g_strSocpStat.u32IntDecSrcErr = 0;

        /*VOS_Splx(s32LockKey);*/
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        /* �������Դ�ж�*/
        if (0 != ulIntState)
        {
            for (i=0; i<SOCP_MAX_DECSRC_CHN; i++)
            {
                /* ���ͨ���Ƿ�����*/
                if(SOCP_CHN_SET == g_strSocpStat.sDecSrcChan[i].u32SetStat)
                {
                    ulsecIntState = ulIntState>>i;

                    SocpDecSrcEventHandler(i, ulsecIntState);
                }
            }
        }

#ifdef OAM_UT_DEBUG
        return;
#endif
    }
}

/*****************************************************************************
* �� �� ��  : SocpDecDstTask
*
* ��������  : ģ��������:����Ŀ�ģ�˫��
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpDecDstTask(BSP_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIntTfrState = 0;
    VOS_UINT32                          ulIntOvfState = 0;
    VOS_UINT32                          ulChanId;
    VOS_UINT32                          ulLockLevel;
    VOS_UINT32                          ulCBTime = 0;

    for(;;)
    {
        if(VOS_OK != VOS_SmP(g_strSocpStat.u32DecDstSemID, 0))
        {
#if defined(OAM_UT_DEBUG) || defined(DMT)   /*only for UT and ST*/
            return;
#endif
            continue;
        }

        /*lLockKey                        = VOS_SplIMP();*/
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        ulIntTfrState                   = g_strSocpStat.u32IntDecDstTfr;
        g_strSocpStat.u32IntDecDstTfr   = 0;

        ulIntOvfState                   = g_strSocpStat.u32IntDecDstOvf;
        g_strSocpStat.u32IntDecDstOvf   = 0;

        /*VOS_Splx(lLockKey);*/
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        /* ������봫������ж�*/
        if (0 != ulIntTfrState)
        {
            for (i=0; i<SOCP_MAX_DECDST_CHN; i++)
            {
                /* ���ͨ���Ƿ�����*/
                if (SOCP_CHN_SET != g_strSocpStat.sDecDstChan[i].u32SetStat)
                {
                    continue;
                }

                if (0 != (ulIntTfrState & ((BSP_U32)1 << i)))
                {
                    g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstTskTrfCbOriCnt[i]++;

                    if (VOS_NULL_PTR != g_strSocpStat.sDecDstChan[i].read_cb)
                    {
                        ulChanId = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN, i);

                        ulCBTime = OM_GetSlice();

                        (BSP_VOID)g_strSocpStat.sDecDstChan[i].read_cb(ulChanId);

                        ulCBTime = OM_GetSlice()-ulCBTime;  /* ��ȡ�ص�������ִ��ʱ�� */

                        SOCP_LOG_INFO("SocpDecDstTask: Dst Trans CB", ulChanId, ulCBTime);
                    }

                    g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstTskTrfCbCnt[i]++;
                }
            }
        }
        /* �������Ŀ�� buffer ����ж�*/
        else if (0 != ulIntOvfState)
        {
            for (i=0; i<SOCP_MAX_DECDST_CHN; i++)
            {
                /* ���ͨ���Ƿ�����*/
                if (SOCP_CHN_SET != g_strSocpStat.sDecDstChan[i].u32SetStat)
                {
                    continue;
                }

                if (0 != (ulIntOvfState & ((BSP_U32)1 << i)))
                {
                    g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstTskOvfCbOriCnt[i]++;

                    ulChanId = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN, i);

                    ulCBTime = OM_GetSlice();   /* ��ȡͨ�����ʱ�� */

                    SOCP_LOG_WARNING("SocpDecDstTask: Dst Overflow CB", ulChanId, ulCBTime);

                    if (VOS_NULL_PTR != g_strSocpStat.sDecDstChan[i].read_cb)
                    {
                        (BSP_VOID)g_strSocpStat.sDecDstChan[i].read_cb(ulChanId);

                        ulCBTime = OM_GetSlice()-ulCBTime;  /* ��ȡ�ص�������ִ��ʱ�� */

                        SOCP_LOG_INFO("SocpDecDstTask: Dst Overflow CB Out", ulChanId, ulCBTime);
                    }

                    g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstTskOvfCbCnt[i]++;
                }
            }
        }
        else
        {

        }

#if defined(OAM_UT_DEBUG) || defined(DMT)   /*only for UT and ST*/
        return;
#endif
    }
}

/*****************************************************************************
* �� �� ��   : SocpHandlerEncSrc
*
* ��������  : ����Դͨ��������
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/

/*lint -e774*/
BSP_VOID SocpHandlerEncSrc(BSP_VOID)
{
    BSP_U32 IntFlag     = 0;
    BSP_U32 IntState    = 0;
    BSP_U32 FlagMask    = 0;
    BSP_U32 FlagState   = 0;
    BSP_U32 IntMask     = 0;
    BSP_BOOL bHandle    = BSP_FALSE;
    BSP_U32 i           = 0;

    FlagMask    = SOCP_APP_ENC_FLAGINT_MASK;
    FlagState   = SOCP_REG_APP_INTSTAT1;
    IntMask     = SOCP_REG_APP_MASK1;

    /*read and clear the interrupt flags*/
    SOCP_REG_READ(SOCP_REG_GBL_INTSTAT, IntFlag);

    if (IntFlag & FlagMask)
    {
        SOCP_REG_READ(FlagState, IntState);
        SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT1, IntState);

        g_strSocpStat.u32IntEncSrcHeader |= IntState;
        bHandle = BSP_TRUE;

        for (i=0; i<SOCP_MAX_ENCSRC_CHN; i++)
        {
            if (IntState & ((BSP_U32)1 << i))
            {
                if(SOCP_REG_GETBITS(SOCP_REG_ENCSRC_BUFCFG1(i), 31, 1))
                {
                    SOCP_REG_SETBITS(IntMask, i, 1, 1);
                }

                g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpEncSrcIsrHeadIntCnt[i]++;
            }
        }
    }

    if(bHandle)
    {
        VOS_SmV(g_strSocpStat.u32EncSrcSemID);
    }

    return;
}

/*lint +e774*/

/*****************************************************************************
* �� �� ��   : SocpHandlerEncDst
*
* ��������  : ����Ŀ���жϴ�����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpHandlerEncDst(BSP_VOID)
{
    BSP_U32  IntFlag = 0;
    BSP_U32  IntState = 0;
    BSP_BOOL bHandle = BSP_FALSE;
    BSP_U32  i;
    BSP_U32  mask;

    /*��ȡȫ���ж�״̬*/
    SOCP_REG_READ(SOCP_REG_GBL_INTSTAT, IntFlag);

    /*����Ŀ�Ĵ����ж�*/
    if (IntFlag & SOCP_APP_ENC_TFRINT_MASK)
    {
        SOCP_REG_READ(SOCP_REG_ENC_INTSTAT0, IntState);
        SOCP_REG_READ(SOCP_REG_ENC_MASK0, mask);
        SOCP_REG_WRITE(SOCP_REG_ENC_MASK0, (IntState | mask));   /* mask int 2011.7.27 by yangzhi */
        SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT0, IntState);

        /*������ֵ����жϣ����⴫��������ϱ����������֮����ٴ�*/
        SOCP_REG_READ(SOCP_REG_ENC_MASK2, mask);
        SOCP_REG_WRITE(SOCP_REG_ENC_MASK2, (mask|((BSP_U32)(IntState<<SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN))));

        g_strSocpStat.u32IntEncDstTfr |= IntState;

        bHandle = BSP_TRUE;

        for (i = 0; i < SOCP_MAX_ENCDST_CHN; i++)
        {
            if (IntState & ((BSP_U32)1 << i))
            {
                g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstIsrTrfIntCnt[i]++;
            }
        }
    }
    /* �����ж���DMA����жϹ���һ���Ĵ�����DMA����жϲ�������ͨ�����ñ�֤�����ϱ���
       ͨ������жϺʹ����ж�ͬʱ�ʹ���һ�����ȴ������жϣ���������жϵ�ʱ�򲻻��д����ж��ϱ� */
    else if (IntFlag & SOCP_APP_ENC_OUTOVFINT_MASK)
    {
        SOCP_REG_READ(SOCP_REG_ENC_INTSTAT2, IntState);

        /* ����Ŀ��buffer�����ж� */
        if(0 != (IntState & SOCP_ENC_DST_BUFF_THRESHOLD_OVF_MASK))
        {
            SOCP_REG_READ(SOCP_REG_ENC_MASK2, mask);
            SOCP_REG_WRITE(SOCP_REG_ENC_MASK2, (IntState|mask));    /*�����ж�Mask*/
            SOCP_REG_WRITE(SOCP_REG_ENC_RAWINT2, IntState);         /*����ж�״̬*/

            g_strSocpStat.u32IntEncDstThresholdOvf |= IntState;

            bHandle = BSP_TRUE;

            for (i = 0; i < SOCP_MAX_ENCDST_CHN; i++)
            {
                if (IntState & ((BSP_U32)1 << (i + SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN)))
                {
                    g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpEncDstIsrThresholdOvfIntCnt[i]++;
                }
            }
        }
    }
    else
    {
        bHandle = BSP_FALSE;
    }

    if(bHandle)
    {
        g_stSocpDebugInfo.stSocpEncDstSemInfo.ulIntNum++;
        g_stSocpDebugInfo.stSocpEncDstSemInfo.ulIntSlice = OM_GetSlice();

        VOS_SmV(g_strSocpStat.u32EncDstSemID);
    }

    return ;
}

/*****************************************************************************
* �� �� ��   : SocpHandlerDecSrc
*
* ��������  : ����Դͨ���жϴ�����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SocpHandlerDecSrc(BSP_VOID)
{
    BSP_U32 IntFlag  = 0;
    BSP_U32 IntState = 0;
    BSP_BOOL bHandle = BSP_FALSE;
    BSP_U32 i = 0;

    /*�����������*/
    SOCP_REG_READ(SOCP_REG_GBL_INTSTAT, IntFlag);
    if (IntFlag & SOCP_DEC_INERRINT_MASK)
    {
        SOCP_REG_READ(SOCP_REG_DEC_INTSTAT1, IntState);
        SOCP_REG_WRITE(SOCP_REG_DEC_RAWINT1, IntState);

        g_strSocpStat.u32IntDecSrcErr |= IntState;
        bHandle = BSP_TRUE;

        for (i = 0; i < SOCP_MAX_DECSRC_CHN; i++)
        {
            if (IntState & 0x1)
            {
                g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpDecSrcIsrErrIntCnt[i]++;
            }
        }
    }

    if(bHandle)
    {
        VOS_SmV(g_strSocpStat.u32DecSrcSemID);
    }

    return;
}

/*****************************************************************************
* �� �� ��   : SocpHandlerDecDst
*
* ��������  : ����Ŀ��ͨ���жϴ�����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/

/*lint -e774*/
BSP_VOID SocpHandlerDecDst(BSP_VOID)
{
    BSP_U32 IntFlag  = 0;
    BSP_U32 IntState = 0;
    BSP_BOOL bHandle = BSP_FALSE;
    BSP_U32 TfMask  = 0;
    BSP_U32 OvMask    = 0;
    BSP_U32 i = 0;

    TfMask    = SOCP_CORE0_DEC_TFRINT_MASK;
    OvMask  = SOCP_CORE0_DEC_OUTOVFINT_MASK;

    /*���봫���ж�*/
    SOCP_REG_READ(SOCP_REG_GBL_INTSTAT, IntFlag);
    if (IntFlag & TfMask)
    {
        BSP_U32 mask;

        SOCP_REG_READ(SOCP_REG_DEC_CORE0ISTAT0, IntState);
        IntState = IntState & 0xffff;
        SOCP_REG_READ(SOCP_REG_DEC_CORE0MASK0, mask);
        SOCP_REG_WRITE(SOCP_REG_DEC_CORE0MASK0, (IntState | mask));  /*added by yangzhi 2011.7.27 */
        SOCP_REG_WRITE(SOCP_REG_DEC_RAWINT0, IntState);

        SOCP_REG_READ(SOCP_REG_DEC_CORE0MASK2, mask);
        SOCP_REG_WRITE(SOCP_REG_DEC_CORE0MASK2, (IntState | mask)); /*���������ж�*/

        g_strSocpStat.u32IntDecDstTfr |= IntState;
        bHandle = BSP_TRUE;

        for (i = 0; i < SOCP_MAX_DECDST_CHN; i++)
        {
            if (IntState & ((BSP_U32)1 << i))
            {
                g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstIsrTrfIntCnt[i]++;
            }
        }
    }
    /*����Ŀ��buffer ����*/
    else if (IntFlag & OvMask)
    {
        SOCP_REG_READ(SOCP_REG_DEC_CORE0ISTAT2, IntState);
        IntState = IntState & 0xffff;
        SOCP_REG_WRITE(SOCP_REG_DEC_RAWINT2, IntState);

        SOCP_REG_WRITE(SOCP_REG_DEC_CORE0MASK2, IntState); /*���������ж�*/

        g_strSocpStat.u32IntDecDstOvf |= IntState;
        bHandle = BSP_TRUE;

        for (i = 0; i < SOCP_MAX_DECDST_CHN; i++)
        {
            if (IntState & ((BSP_U32)1 << i))
            {
                g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpDecDstIsrOvfIntCnt[i]++;
            }
        }
    }
    else
    {
        bHandle = BSP_FALSE;
    }

    if(bHandle)
    {
        VOS_SmV(g_strSocpStat.u32DecDstSemID);
    }

    return;
}

/*lint +e774*/

/*****************************************************************************
* �� �� ��   : SocpAppInthandler
*
* ��������  : APP ���жϴ�����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SocpAppInthandler(BSP_VOID)
{
    VOS_UINT32          ulLockLevel;

    VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

    g_stSocpDebugInfo.sSocpDebugGBl.u32SocpAppEtrIntCnt++;

    SocpHandlerEncSrc();

    SocpHandlerEncDst();

    SocpHandlerDecSrc();

    SocpHandlerDecDst();

    g_stSocpDebugInfo.sSocpDebugGBl.u32SocpAppSucIntCnt++;

    VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

    return 1;
}
/*****************************************************************************
* �� �� ��   : BSP_SOCP_GetSrcChannelReg
*
* ��������  : ��ȡָ������Դͨ���ļĴ���
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
VOS_VOID BSP_SOCP_GetEncSrcReg(VOS_UINT32 *pulData, VOS_UINT32 ulDataMax)
{
    VOS_UINT32      i;

    /* ��ȡ 0 �� 7�ܹ�8��ͨ�������üĴ�����ÿ��ͨ����9��32bit �Ĵ������ܹ� 288 ��byte */
    if ( ulDataMax < 72 )/* ���ȵĵ�λ��4byte���������� */
    {
        return;
    }

    for(i=0; i<8; i++)
    {
        VOS_MemCpy((pulData+(i*9)), (VOS_VOID*)(g_strSocpStat.u32BaseAddr+SOCP_REG_ENCSRC_BUFWPTR(i)), 9*sizeof(VOS_UINT32));
    }

    return;
}

#endif

/*****************************************************************************
* �� �� ��  : SocpInitGobal
*
* ��������  : ģ���ʼ������,��ʼ��ȫ�ֱ���
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
BSP_VOID SocpInitGobal(BSP_VOID)
{
#ifdef SOCP_ACORE
    BSP_U32 i;
#endif

    VOS_MemSet(&g_stSocpDebugInfo, 0x0 ,sizeof(SOCP_DEBUG_INFO_S));

    VOS_MemSet(&g_strSocpStat, 0x0 ,sizeof(SOCP_GBL_STATE));

    VOS_MemSet(&g_stSocpLog, 0, sizeof(SOCP_LOG_INFO_STRU));

#ifdef SOCP_ACORE
    for(i=0; i<SOCP_MAX_ENCDST_CHN; i++)
    {
        g_strSocpStat.sEncDstChan[i].u32Thrh        = 0;
        g_strSocpStat.sEncDstChan[i].u32SetStat     = SOCP_CHN_UNSET;
    }

    for(i=0; i<SOCP_MAX_DECSRC_CHN; i++)
    {
        g_strSocpStat.sDecSrcChan[i].u32ChanID      = i;
        g_strSocpStat.sDecSrcChan[i].u32ChanEn      = SOCP_CHN_DISABLE;
        g_strSocpStat.sDecSrcChan[i].eDataTypeEn    = SOCP_DATA_TYPE_EN_BUTT;
        g_strSocpStat.sDecSrcChan[i].u32SetStat     = SOCP_CHN_UNSET;
    }

    for(i=0; i<SOCP_MAX_DECDST_CHN; i++)
    {
        g_strSocpStat.sDecDstChan[i].u32ChanID      = i;
        g_strSocpStat.sDecDstChan[i].u32SetStat     = SOCP_CHN_UNSET;
        g_strSocpStat.sDecDstChan[i].eDataType      = SOCP_DATA_TYPE_BUTT;
    }
#endif

    return;
}

#ifdef SOCP_ACORE
/*****************************************************************************
* �� �� ��  : SocpAcpuCreateTask
*
* ��������  : ���񴴽�����
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : �����ɹ����ı�ʶ��
*****************************************************************************/

BSP_S32 SocpAcpuCreateTask(BSP_VOID)
{
    VOS_UINT32      aulArguments[4] = {0,0,0,0};
    VOS_UINT32      ulTaskID;

    /* ����Դͨ��������ź���*/
    if(VOS_OK != VOS_SmBCreate("SCP1", 0, VOS_SEMA4_FIFO, (VOS_UINT32*)&g_strSocpStat.u32EncSrcSemID))
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32EncSrcSemID failed", 0, 0);

        return BSP_ERR_SOCP_SEM_CREATE;
    }

    /* ����Դͨ������Ĵ��� "EncSrc"*/
    if( VOS_OK != VOS_CreateTask( "EncSrc",
                                    &ulTaskID,
                                    (VOS_TASK_ENTRY_TYPE)SocpEncSrcTask,
                                    SOCP_ENCSRC_SELFTASK_PRIO,
                                    SOCP_ENCSRC_TASK_STACK_SIZE,
                                    aulArguments) )
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create SocpEncSrcTask failed", 0, 0);

        return BSP_ERR_SOCP_TSK_CREATE;
    }

    /* �������ͨ��������ź���*/
    if(VOS_OK != VOS_SmCCreate("SCP2", 0, VOS_SEMA4_FIFO, (VOS_UINT32*)&g_strSocpStat.u32EncDstSemID))
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32EncDstSemID failed", 0, 0);

        return BSP_ERR_SOCP_SEM_CREATE;
    }

    /* �������ͨ�����񴴽� "EncDst"*/
    if( VOS_OK != VOS_CreateTask(  "EncDst",
                                    &ulTaskID,
                                    (VOS_TASK_ENTRY_TYPE)SocpEncDstTask,
                                    SOCP_ENCDST_SELFTASK_PRIO,
                                    SOCP_ENCDST_TASK_STACK_SIZE,
                                    aulArguments) )
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create SocpEncDstTask failed",0,0);

        return BSP_ERR_SOCP_TSK_CREATE;
    }

#if 0   /* �ȴ������ṩ�ӿ� */
    /* ע��������쳣��λ��ʱ�򱣴�EncDstջ��Ϣ */
    DRV_EXCH_TASK_INFO_ADD((int)ulTaskID);
#endif

    /* ����Դͨ�������ź���*/
    if(VOS_OK != VOS_SmBCreate("SCP3", 0, VOS_SEMA4_FIFO, (VOS_UINT32*)&g_strSocpStat.u32DecSrcSemID))
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32DecSrcSemID failed", 0, 0);

        return BSP_ERR_SOCP_SEM_CREATE;
    }

    /* ����Դͨ�����񴴽� "DecSrc"*/
    if ( VOS_OK != VOS_CreateTask( "DecSrc",
                                    &ulTaskID,
                                    (VOS_TASK_ENTRY_TYPE)SocpDecSrcTask,
                                    SOCP_DECSRC_SELFTASK_PRIO,
                                    SOCP_DECSRC_TASK_STACK_SIZE,
                                    aulArguments) )
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32DecSrcTskID failed", 0, 0);

        return BSP_ERR_SOCP_TSK_CREATE;
    }

    /* ����Ŀ��ͨ�������ź���*/
    if(VOS_OK != VOS_SmBCreate("SCP4", 0, VOS_SEMA4_FIFO, (VOS_UINT32*)&g_strSocpStat.u32DecDstSemID))
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32DecDstSemID failed", 0, 0);

        return BSP_ERR_SOCP_SEM_CREATE;
    }

    /* ����Ŀ��ͨ������Ĵ��� "DecDst"*/
    if ( VOS_OK != VOS_CreateTask( "DecDst",
                                    &ulTaskID,
                                    (VOS_TASK_ENTRY_TYPE)SocpDecDstTask,
                                    SOCP_DECDST_SELFTASK_PRIO,
                                    SOCP_DECDST_TASK_STACK_SIZE,
                                    aulArguments) )
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32DecDstTskID failed", 0, 0);

        return BSP_ERR_SOCP_TSK_CREATE;
    }

#if 0   /* �ȴ������ṩ�ӿ� */
    /* ע��������쳣��λ��ʱ�򱣴�DecDstջ��Ϣ */
    DRV_EXCH_TASK_INFO_ADD((int)ulTaskID);
#endif

    /* ����Ŀ��ͨ��Flush�ź���*/
    if(VOS_OK != VOS_SmBCreate("SCP5", 0, VOS_SEMA4_FIFO, (VOS_UINT32*)&g_strSocpStat.u32EncDstFlushSemID))
    {
        SOCP_LOG_ERR("SocpAcpuCreateTask: create u32EncDstFlushSemID failed", 0, 0);

        return BSP_ERR_SOCP_SEM_CREATE;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : SocpCoreInit
*
* ��������  : ģ���ʼ������, �����ʼ��оƬ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/

BSP_VOID SocpCoreInit(BSP_VOID)
{
    BSP_U32 i;
    BSP_U32 u32ResetValue = 0;

    /* ��ȫ�ּĴ������и�λ */
    SOCP_REG_WRITE(SOCP_REG_GBLRST, 1);

    /* �ȴ�ͨ����λ״̬���� */
    for (i = 0; i < SOCP_RESET_TIME; i++)
    {
        SOCP_REG_READ(SOCP_REG_GBLRST, u32ResetValue);
        if (0 == u32ResetValue)
        {
            break;
        }
    }

    if (SOCP_RESET_TIME == i)
    {
        SOCP_LOG_WARNING("SocpCoreInit: reset global regs failed!", 0, 0);
    }

    /* ����ram�мĴ����ĸ�λ����*/
    for (i = 0; i < SOCP_REAL_CHAN_ID(SOCP_CODER_SRC_BUTT); i++)
    {
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR(i), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFCFG0(i), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQWPTR(i), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQRPTR(i), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR(i), 0);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQCFG(i),  0);
    }

    for (i = 0; i < SOCP_REAL_CHAN_ID(SOCP_CODER_DST_BUTT); i++)
    {
        SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFADDR(i), 0);
    }

    return;
}

/*****************************************************************************
* �� �� ��  : SocpAcpuInit
*
* ��������  : ģ���ʼ������,��ʼ��Acpu�������Դ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
BSP_S32 SocpAcpuInit(BSP_VOID)
{
    BSP_U32                             u32BaseAddr;
    BSP_S32                             s32IntLevel;
    BSP_S32                             s32Result;
    SOCP_ENC_DST_BUF_LOG_CFG_STRU       *pstcfg;

    vos_printf("\r\n ---------------ACPU zzz SocpInit--------------------\r\n");

    VOS_SpinLockInit(&g_stSocpFlushSpinLock);

    SocpInitSDLogCfg();

    pstcfg = SocpGetSDLogCfg();

    u32BaseAddr = DRV_GET_IP_BASE_ADDR(BSP_IP_TYPE_SOCP);

#if(VOS_OS_VER == VOS_LINUX)
    g_strSocpStat.u32BaseAddr = (BSP_U32)ioremap(u32BaseAddr, SOCP_REGISTER_SIZE);

    if(0 == g_strSocpStat.u32BaseAddr)
    {
        SOCP_LOG_ERR("SocpAcpuInit: Socp Reg Base Addr ioremap Error", 0, 0);
        return BSP_ERROR;
    }
#else
    g_strSocpStat.u32BaseAddr = u32BaseAddr;
#endif

    SocpCoreInit(); /*A�˳�ʼ��оƬ*/

    /* APP CPUʱ����Ҫ���ε��ж� */
    SOCP_REG_SETBITS(SOCP_REG_ENC_MASK0, 0, 7, 0x7f);           /* ����Bit0~6 */
    SOCP_REG_WRITE(SOCP_REG_APP_MASK1, VOS_NULL_DWORD);
    SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, 0, 7, 0x7f);           /* ����Bit0~6 */
    SOCP_REG_WRITE(SOCP_REG_APP_MASK3, VOS_NULL_DWORD);
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0, 0, 16, VOS_NULL_WORD);    /* ����Bit0~15 */
    SOCP_REG_SETBITS(SOCP_REG_DEC_MASK1, 0, 24, 0xffffff);              /* ����Bit0~23 */
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK2, 0, 16, VOS_NULL_WORD);    /* ����Bit0~15 */

    /* ���������ж� */
    SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, 16, 7, 0x7f);      /* ����Bit16~22 */

    if(VOS_TRUE == pstcfg->logOnFlag)
    {
        SOCP_REG_WRITE(SOCP_REG_INTTIMEOUT, VOS_NULL);                      /* �رճ�ʱ�����жϳ�ʱ���� */
    }
    else
    {
        SOCP_REG_WRITE(SOCP_REG_INTTIMEOUT, SOCP_TRANS_TIMEOUT_DEFAULT);    /* ��ʱ�����жϳ�ʱ���� */
    }

    SOCP_REG_SETBITS(SOCP_REG_BUFTIMEOUT, 31, 1, 0);                    /* ԴĿ��buffer�����ʱ�ж϶���ʹ�� */
    SOCP_REG_SETBITS(SOCP_REG_BUFTIMEOUT, 0, 16, SOCP_OVERFLOW_TIMEOUT_DEFAULT); /* ԴĿ��buffer�����ʱ���� */

    /* ���ý���ͨ·���������üĴ���*/
    SOCP_REG_SETBITS(SOCP_REG_DEC_PKTLEN, 0, 12, SOCP_DEC_PKTLGTH_MAX); /* �����������ݰ���С����KBΪ��λ��Ĭ��Ϊ2KB */
    SOCP_REG_SETBITS(SOCP_REG_DEC_PKTLEN, 12, 5, SOCP_DEC_PKTLGTH_MIN); /* �������С���ݰ���С���ֽ�Ϊ��λ */

     /* �����Զ�ʱ���ſ� */
    SOCP_REG_WRITE(SOCP_REG_CLKCTRL, VOS_NULL_DWORD); /* RAM�Զ��ſؿ��� */

    s32IntLevel = DRV_GET_INT_NO(BSP_INT_TYPE_SOCP_A);

    if (BSP_ERROR == s32IntLevel)
    {
        SOCP_LOG_ERR("SocpAcpuInit: Query app core int failed", 0, 0);
        return BSP_ERROR;
    }

    /* ���ж� */
    if (BSP_OK != DRV_VICINT_CONNECT((VOIDFUNCPTR *)s32IntLevel, (VOIDFUNCPTR)SocpAppInthandler, 0))
    {
        SOCP_LOG_ERR("SocpAcpuInit: connect app core int failed", 0, 0);
        return BSP_ERROR;
    }

    if (BSP_OK != DRV_VICINT_ENABLE(s32IntLevel))
    {
        SOCP_LOG_ERR("SocpAcpuInit: enable int failed", 0, 0);
        return BSP_ERROR;
    }

    s32Result = SocpAcpuCreateTask();

    if (BSP_OK != s32Result)
    {
        SOCP_LOG_ERR("SocpAcpuInit: create task failed", (VOS_UINT32)s32Result, 0);

        return BSP_ERROR;
    }

    vos_printf("\r\n ---------------ACPU SocpInit end--------------------\r\n");

    return BSP_OK;
}
#endif

#ifdef SOCP_CCORE
/*****************************************************************************
* �� �� ��  : SocpCcpuInit
*
* ��������  : ģ���ʼ������,��ʼ��Ccpu�������Դ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
BSP_S32 SocpCcpuInit(BSP_VOID)
{
    BSP_S32     s32IntLevel;

#if 0 /* log2.0������ע�� */
    NV_SOCP_LOG_CFG_STRU nvCfg = {0};
    SOCP_ENC_DST_BUF_LOG_CFG_STRU * logCfg;

    logCfg = BSP_SOCP_GetLogCfg();
#endif

    vos_printf("\r\n ---------------CCPU zzz SocpInit--------------------\r\n");

    g_strSocpStat.u32BaseAddr = DRV_GET_IP_BASE_ADDR(BSP_IP_TYPE_SOCP);

    if(0 == g_strSocpStat.u32BaseAddr)
    {
        SOCP_LOG_ERR("SocpCcpuInit: Query SOCP base addr failed",0,0);
        return BSP_ERROR;
    }

#if 0 /* log2.0������ע�� */
#if(FEATURE_LTE == FEATURE_ON)
    ret = NVM_Read(EN_NV_ID_SOCP_LOG_CFG, (VOID *)&nvCfg, sizeof(NV_SOCP_LOG_CFG_STRU));
    if(ret != BSP_OK)
    {
        printf("SocpInit READ NV FAIL\n\n\n\n");
        nvCfg.ulSocpDelayWriteFlg = FALSE;
        logCfg->logOnFlag = FALSE;
    }
#endif
#endif

    /* ���ô����жϳ�ʱʱ���buffer�����ʱʱ�� */
#if 0
    #if(FEATURE_LTE == FEATURE_ON)
    if( TRUE == nvCfg.ulSocpDelayWriteFlg )
    {
        SOCP_REG_WRITE(SOCP_REG_INTTIMEOUT, 0);
    }
    #endif
#endif

    /* Modem Cpuʱ����Ҫ�����ж� */
    SOCP_REG_WRITE(SOCP_REG_MODEM_MASK1, VOS_NULL_DWORD); /* ����ͨ·Դ���ݰ�ͷ�����ж����ο���λ MODEM-cpu���� */
    SOCP_REG_WRITE(SOCP_REG_MODEM_MASK3, VOS_NULL_DWORD); /* CORE1����ͨ·RD��������ж����ο���λ */
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE1MASK0, 0, 16, VOS_NULL_WORD);   /* core1 DATA-TYPE ����*/
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE1MASK2, 0, 16, VOS_NULL_WORD);   /* core1����DATA-TYPE */

    /* ���α��봫���ж� */
    SOCP_REG_WRITE(SOCP_REG_MODEM_ENC_MASK0, 0x7f);         /* core1����ͨ·�����ж����ο���λ */

    /* ���α���Ŀ��buffer�����ж� */
    SOCP_REG_SETBITS(SOCP_REG_MODEM_ENC_MASK2, 0, 7, 0x7f); /* core1����Ŀ��bufferʣ��ռ䲻��һ��dma����ʱ�����ж����ο��� */
    SOCP_REG_SETBITS(SOCP_REG_MODEM_ENC_MASK2, 16, 7,0x7f); /* core1����Ŀ��bufferʣ��ռ�С�����õ���ֵʱ�����ж����ο��� */

    /* ���ν���ͨ���쳣�ж� */
    SOCP_REG_WRITE(SOCP_REG_APP_DEC_MASK1, VOS_NULL_DWORD); /* core1����ͨ· */

    s32IntLevel = DRV_GET_INT_NO(BSP_INT_TYPE_SOCP_C);

    if(BSP_ERROR == s32IntLevel)
    {
        SOCP_LOG_ERR("SocpCcpuInit: Query modem core int failed",0,0);
        return BSP_ERROR;
    }

#if 0   /* �����˴��������ں�����չ */
    /* ��Ccpu Socp�ж� ����Ҫ���Բſ���ʹ��*/
    if(BSP_OK != DRV_VICINT_CONNECT((VOIDFUNCPTR *)s32IntLevel, (VOIDFUNCPTR)SocpModemInthandler, 0))
    {
        BSP_TRACE(BSP_LOG_LEVEL_ERROR, BSP_MODU_SOCP, "SocpCcpuInit: connect modem core int failed.\n",0,0,0,0,0,0);
        return BSP_ERROR;
    }

    if (BSP_OK != BSP_INT_Enable(s32IntLevel))
    {
        BSP_TRACE(BSP_LOG_LEVEL_ERROR, BSP_MODU_SOCP, "SocpCcpuInit: enable int failed.\n", 0, 0, 0, 0, 0, 0);
        return BSP_ERROR;
    }

    if (BSP_OK != SocpCreateTask())
    {
        BSP_TRACE(BSP_LOG_LEVEL_ERROR, BSP_MODU_SOCP, "SocpAcpuCreateTask: create task failed(0x%x).\n", s32Result);

        return BSP_ERROR;
    }
#endif

    vos_printf("\r\n ---------------CCPU SocpInit end--------------------\r\n");

    return BSP_OK;
}
#endif

/*****************************************************************************
* �� �� ��  : socpInit
*
* ��������  : ģ���ʼ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/

/*lint -e529*/
BSP_S32 SocpInit(BSP_VOID)
{
    BSP_S32 s32Result;

    if(BSP_TRUE == g_strSocpStat.bInitFlag)
    {
        return BSP_OK;
    }

    DRV_EXCH_CUST_FUNC_REG((exchCBReg)SOCP_Log_Save);

    VOS_SpinLockInit(&g_stSocpSpinLock);

    SocpInitGobal();    /*��ʼ�� A��C ��ȫ�ֱ���*/

#ifdef SOCP_ACORE
    s32Result = SocpAcpuInit();
#endif

#ifdef SOCP_CCORE
    s32Result = SocpCcpuInit();
#endif

    if(BSP_OK != s32Result)
    {
        SOCP_LOG_ERR("SocpInit: Init Func failed", (VOS_UINT32)s32Result, 0);

        return s32Result;
    }

    /* ���ó�ʼ��״̬ */
    g_strSocpStat.bInitFlag = BSP_TRUE;

    return BSP_OK;
}
/*****************************************************************************
* �� �� ��  : BSP_SOCP_CoderSetSrcChan
*
* ��������  : ����Դͨ�����ú���
*
* �������  : pSrcAttr     ����Դͨ�����ò���
*             ulSrcChanID  ����Դͨ��ID
*
* �������  : ��
*
* �� �� ֵ  : ���뼰���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_CoderSetSrcChan(SOCP_CODER_SRC_ENUM_U32 enSrcChanID,
                                            SOCP_CODER_SRC_CHAN_S *pSrcAttr)
{
    BSP_U32             start;
    BSP_U32             end;
    BSP_U32             rdstart         = 0;
    BSP_U32             rdend           = 0;
    BSP_U32             buflength       = 0;
    BSP_U32             Rdbuflength     = 0;
    BSP_U32             srcChanId;
    BSP_U32             u32SrcChanType;
    BSP_U32             u32DstChanID;
    BSP_U32             u32DstChanType;
    BSP_U32             ResetFlag;
    BSP_U32             i;
    SOCP_ENCSRC_CHAN_S *pChan;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pSrcAttr);
    SOCP_CHECK_CHAN_PRIORITY(pSrcAttr->ePriority);
    SOCP_CHECK_DATA_TYPE(pSrcAttr->eDataType);
    SOCP_CHECK_DATA_TYPE_EN(pSrcAttr->eDataTypeEn);
    SOCP_CHECK_ENC_DEBUG_EN(pSrcAttr->eDebugEn);

    srcChanId       = SOCP_REAL_CHAN_ID(enSrcChanID);
    u32SrcChanType  = SOCP_REAL_CHAN_TYPE(enSrcChanID);

    SOCP_CHECK_CHAN_TYPE(u32SrcChanType, SOCP_CODER_SRC_CHAN);
    SOCP_CHECK_CHAN_ID(srcChanId, SOCP_MAX_ENCSRC_CHN);

    u32DstChanID   = SOCP_REAL_CHAN_ID(pSrcAttr->u32DestChanID);
    u32DstChanType = SOCP_REAL_CHAN_TYPE(pSrcAttr->u32DestChanID);

    SOCP_CHECK_CHAN_TYPE(u32DstChanType, SOCP_CODER_DEST_CHAN);
    SOCP_CHECK_CHAN_ID(u32DstChanID, SOCP_MAX_ENCDST_CHN);

    if ((SOCP_ENCSRC_CHNMODE_CTSPACKET != pSrcAttr->eMode)
        && (SOCP_ENCSRC_CHNMODE_LIST != pSrcAttr->eMode))
    {
        SOCP_LOG_ERR("CoderAllocSrcChan: chnnel mode is invalid ", pSrcAttr->eMode, 0);

        return BSP_ERR_SOCP_INVALID_PARA;
    }

    /* ʹ�����ò����������� */
    /* �ж���ʼ��ַ�Ƿ�8�ֽڶ��� */
    start   = pSrcAttr->sCoderSetSrcBuf.u32InputStart;
    end     = pSrcAttr->sCoderSetSrcBuf.u32InputEnd;

    SOCP_CHECK_PARA(start);
    SOCP_CHECK_8BYTESALIGN(start);
    SOCP_CHECK_PARA(end);
    SOCP_CHECK_BUF_ADDR(start, end);

    buflength = end - start + 1;

    SOCP_CHECK_8BYTESALIGN(buflength);

    if(buflength > SOCP_ENC_SRC_BUFLGTH_MAX)
    {
        SOCP_LOG_ERR("CoderAllocSrcChan: buffer length is too large", buflength, 0);

        return BSP_ERR_SOCP_INVALID_PARA;
    }

    /* �����������������������RDbuffer����ʼ��ַ�ͳ��� */
    if(SOCP_ENCSRC_CHNMODE_LIST == pSrcAttr->eMode)
    {
        /* �ж�RDBuffer����ʼ��ַ�Ƿ�8�ֽڶ��� */
        rdstart = pSrcAttr->sCoderSetSrcBuf.u32RDStart;
        rdend   = pSrcAttr->sCoderSetSrcBuf.u32RDEnd;

        SOCP_CHECK_PARA(rdstart);
        SOCP_CHECK_8BYTESALIGN(rdstart);
        SOCP_CHECK_PARA(rdend);
        SOCP_CHECK_BUF_ADDR(rdstart, rdend);
        /* SOCP_CHECK_PARA(pSrcAttr->sCoderSetSrcBuf.u32RDThreshold); ��ʱ����Ҫ���ò���*/

        Rdbuflength = rdend - rdstart + 1;

        SOCP_CHECK_8BYTESALIGN(Rdbuflength);

        if(Rdbuflength > SOCP_ENC_SRC_RDLGTH_MAX)
        {
            SOCP_LOG_ERR("CoderAllocSrcChan: RD buffer length is too large", Rdbuflength, 0);

            return BSP_ERR_SOCP_INVALID_PARA;
        }
    }

    /* ��λͨ�� */
    SOCP_REG_SETBITS(SOCP_REG_ENCRST, srcChanId, 1, 1);

    /* �ȴ�ͨ������ */
    for (i = 0; i < SOCP_RESET_TIME; i++)
    {
        ResetFlag = SOCP_REG_GETBITS(SOCP_REG_ENCRST, srcChanId, 1);

        if(0 == ResetFlag)
        {
            break;
        }
    }

    if (SOCP_RESET_TIME == i)
    {
        SOCP_LOG_ERR("CoderAllocSrcChan: reset channel failed!", srcChanId, 0);
    }

    /* д����ʼ��ַ��Դbuffer��ʼ��ַ�Ĵ���*/
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFADDR(srcChanId),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFWPTR(srcChanId),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFRPTR(srcChanId),SOCP_VIRT_PHY(start));
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG0(srcChanId), 0, 27, buflength);

    /* �����������������������RDbuffer����ʼ��ַ�ͳ��� */
    if(SOCP_ENCSRC_CHNMODE_LIST == pSrcAttr->eMode)
    {
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQADDR(srcChanId),SOCP_VIRT_PHY(rdstart));
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQRPTR(srcChanId),SOCP_VIRT_PHY(rdstart));
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQWPTR(srcChanId),SOCP_VIRT_PHY(rdstart));
        SOCP_REG_SETBITS(SOCP_REG_ENCSRC_RDQCFG(srcChanId), 0, 16, Rdbuflength);
    }

    /*������������*/
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 1, 2, pSrcAttr->eMode);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 4, 4, pSrcAttr->u32DestChanID);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 8, 2, pSrcAttr->ePriority);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 10, 1, pSrcAttr->u32BypassEn);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 11, 1, pSrcAttr->eDataTypeEn);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 31, 1, pSrcAttr->eDebugEn);
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(srcChanId), 16, 8, pSrcAttr->eDataType);

    /* ��g_strSocpStat�б������*/
    pChan = &g_strSocpStat.sEncSrcChan[srcChanId];

    pChan->u32SetStat             = SOCP_CHN_SET;
    pChan->eChnMode               = pSrcAttr->eMode;
    pChan->sEncSrcBuf.u32Start    = start;
    pChan->sEncSrcBuf.u32End      = end;
    pChan->sEncSrcBuf.u32Write    = start;
    pChan->sEncSrcBuf.u32Read     = start;
    pChan->sEncSrcBuf.u32Length   = buflength;
    pChan->sEncSrcBuf.u32IdleSize = 0;

    if(SOCP_ENCSRC_CHNMODE_LIST == pSrcAttr->eMode)
    {
        pChan->sRdBuf.u32Start    = rdstart;
        pChan->sRdBuf.u32End      = rdend;
        pChan->sRdBuf.u32Write    = rdstart;
        pChan->sRdBuf.u32Read     = rdstart;
        pChan->sRdBuf.u32Length   = Rdbuflength;
    }

    return BSP_OK;
}

#ifdef SOCP_ACORE
/*****************************************************************************
* �� �� ��  : BSP_SOCP_DecoderSetDestChan
*
* ��������  : ����Ŀ��ͨ�����뼰���ú���
*
* �������  : pAttr           ����Ŀ��ͨ�����ò���
*             pDestChanID     ��ʼ������Ŀ��ͨ��ID
*
* �������  :
*
* �� �� ֵ  : ���뼰���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_DecoderSetDestChan(SOCP_DECODER_DST_ENUM_U32 enDestChanID,
                                                SOCP_DECODER_DEST_CHAN_S *pAttr)
{
    BSP_U32             start;
    BSP_U32             end;
    BSP_U32             bufThreshold;
    BSP_U32             buflength;
    BSP_U32             u32ChanID;
    BSP_U32             u32SrcChanID;
    BSP_U32             u32ChanType;
    SOCP_DECDST_CHAN_S *pChan;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pAttr);
    SOCP_CHECK_DATA_TYPE(pAttr->eDataType);

    u32ChanID    = SOCP_REAL_CHAN_ID(enDestChanID);
    u32ChanType  = SOCP_REAL_CHAN_TYPE(enDestChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_DEST_CHAN);
    SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECDST_CHN);

    u32SrcChanID = SOCP_REAL_CHAN_ID(pAttr->u32SrcChanID);
    u32ChanType  = SOCP_REAL_CHAN_TYPE(pAttr->u32SrcChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);
    SOCP_CHECK_CHAN_ID(u32SrcChanID, SOCP_MAX_DECSRC_CHN);

    /* �жϸ����ĵ�ַ�ͳ����Ƿ�Ϊ���ֽڱ���*/
    start           = pAttr->sDecoderDstSetBuf.u32OutputStart;
    end             = pAttr->sDecoderDstSetBuf.u32OutputEnd;
    bufThreshold    = pAttr->sDecoderDstSetBuf.u32Threshold;

    SOCP_CHECK_PARA(start);
    SOCP_CHECK_8BYTESALIGN(start);
    SOCP_CHECK_PARA(end);
    SOCP_CHECK_BUF_ADDR(start, end);
    SOCP_CHECK_PARA(bufThreshold);

    if (bufThreshold > SOCP_DEC_DST_TH_MAX)
    {
        SOCP_LOG_ERR("DecoderAllocDestChan: buffer threshold is too large", bufThreshold, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    buflength = end - start + 1;

    SOCP_CHECK_8BYTESALIGN(buflength);

    if (buflength > SOCP_DEC_DST_BUFLGTH_MAX)
    {
        SOCP_LOG_ERR("DecoderAllocDestChan: buffer length is too large", buflength, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    /* д����ʼ��ַ��Դbuffer��ʼ��ַ�Ĵ���*/
    SOCP_REG_SETBITS(SOCP_REG_DECDEST_BUFCFG(u32ChanID), 24, 8, pAttr->eDataType);
    SOCP_REG_WRITE(SOCP_REG_DECDEST_BUFRPTR(u32ChanID), SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_DECDEST_BUFWPTR(u32ChanID), SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_DECDEST_BUFADDR(u32ChanID), SOCP_VIRT_PHY(start));
    SOCP_REG_SETBITS(SOCP_REG_DECDEST_BUFCFG(u32ChanID), 0, 16, buflength);
    SOCP_REG_SETBITS(SOCP_REG_DECDEST_BUFCFG(u32ChanID), 16, 8, bufThreshold);

    /* ������� */
    pChan = &g_strSocpStat.sDecDstChan[u32ChanID];

    pChan->eDataType              = pAttr->eDataType;
    pChan->sDecDstBuf.u32Start    = start;
    pChan->sDecDstBuf.u32End      = end;
    pChan->sDecDstBuf.u32Length   = buflength;
    pChan->sDecDstBuf.u32Read     = start;
    pChan->sDecDstBuf.u32Write    = start;

    /* ͨ���Ѿ����� */
    pChan->u32SetStat             = SOCP_CHN_SET;

    /* �����жϣ��ٴ��ж�*/
    SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT0,      u32ChanID, 1, 1);
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0,   u32ChanID, 1, 0);

    SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT2,      u32ChanID, 1, 1);
    SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK2,   u32ChanID, 1, 0);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_CoderSetDestChanAttr
*
* ��������  : ����Ŀ��ͨ�����ú���
*
* �������  : u32DestChanID      ����Ŀ��ͨ��ID
              pDestAttr          ����Ŀ��ͨ�����ò���
*
* �������  : ��
* �� �� ֵ  : ���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_CoderSetDestChanAttr(VOS_UINT32 u32DestChanID, SOCP_CODER_DEST_CHAN_S *pDestAttr)
{
    BSP_U32             start;
    BSP_U32             end;
    BSP_U32             bufThreshold;
    BSP_U32             buflength;
    BSP_U32             u32ChanID;
    BSP_U32             u32ChanType;
    BSP_U32             u32Thrh;
    SOCP_ENCDST_CHAN_S  *pChan;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pDestAttr);

    u32ChanID   = SOCP_REAL_CHAN_ID(u32DestChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32DestChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_CODER_DEST_CHAN);
    SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCDST_CHN);

    start       = pDestAttr->sCoderSetDstBuf.u32OutputStart;
    end         = pDestAttr->sCoderSetDstBuf.u32OutputEnd;
    bufThreshold= pDestAttr->sCoderSetDstBuf.u32Threshold;
    u32Thrh     = pDestAttr->u32EncDstThrh;

    SOCP_CHECK_PARA(start);
    SOCP_CHECK_8BYTESALIGN(start);
    SOCP_CHECK_PARA(end);
    SOCP_CHECK_BUF_ADDR(start, end);
    SOCP_CHECK_PARA(bufThreshold);

    buflength = end - start + 1;
    SOCP_CHECK_8BYTESALIGN(buflength);

    if (buflength > SOCP_ENC_DST_BUFLGTH_MAX)
    {
        SOCP_LOG_ERR("CoderSetDestChanAttr: buffer length is too large", buflength, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    if (bufThreshold > SOCP_ENC_DST_TH_MAX)
    {
        SOCP_LOG_ERR("CoderSetDestChanAttr: buffer threshold is too large", bufThreshold, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    if(u32Thrh < SOCP_ENC_DST_THRH_MIN)
    {
        SOCP_LOG_ERR("CoderSetDestChanAttr: buffer thrh is too large", u32Thrh, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    pChan = &g_strSocpStat.sEncDstChan[u32ChanID];

    /* ����������������ٴ�����,ͨ����λ֮��ֻ����һ�� */
    if (SOCP_CHN_SET == pChan->u32SetStat)
    {
        SOCP_LOG_ERR("CoderSetDestChanAttr: channel can't be set twice!", u32ChanID, 0);
        return BSP_ERR_SOCP_SET_FAIL;
    }

    /* ʹ�����ò����������� */
    /* д����ʼ��ַ��Ŀ��buffer��ʼ��ַ�Ĵ���*/
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFADDR(u32ChanID),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFRPTR(u32ChanID),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFWPTR(u32ChanID),SOCP_VIRT_PHY(start));
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFCFG(u32ChanID), 0, 21, buflength);
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFCFG(u32ChanID), 21, 11, bufThreshold);
    SOCP_REG_SETBITS(SOCP_REG_ENCDEST_BUFTHRH(u32ChanID), 0, 31, u32Thrh);

    /* ��g_strSocpStat�б������*/
    pChan->u32ChanID              = u32ChanID;
    pChan->u32Thrh                = u32Thrh;
    pChan->sEncDstBuf.u32Start    = start;
    pChan->sEncDstBuf.u32End      = end;
    pChan->sEncDstBuf.u32Write    = start;
    pChan->sEncDstBuf.u32Read     = start;
    pChan->sEncDstBuf.u32Length   = buflength;

    /* ������ͨ���Ѿ����� */
    pChan->u32SetStat             = SOCP_CHN_SET;

    /* �����жϣ��ٴ��ж� */
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT0, u32ChanID, 1, 1);
    SOCP_REG_SETBITS(SOCP_REG_ENC_MASK0, u32ChanID, 1, 0);

    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, u32ChanID, 1, 1);
    /*SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, u32ChanID, 1, 0);*//*�����ж��������жϣ�ͨ�����ñ�֤*/

    /* ����Ŀ��buffer��ֵ����ж� */
    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 1);
    SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 0);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_DecoderSetSrcChanAttr
*
* ��������  : ����Դͨ�����ú���
*
* �������  : u32SrcChanID    ����Դͨ��ID
*             pInputAttr      ����Դͨ�����ò���
*
* �������  :
*
* �� �� ֵ  : ���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_DecoderSetSrcChanAttr(VOS_UINT32 u32SrcChanID, SOCP_DECODER_SRC_CHAN_S *pInputAttr)
{
    BSP_U32             start;
    BSP_U32             end;
    BSP_U32             buflength = 0;
    BSP_U32             u32ChanID;
    BSP_U32             u32ChanType;
    BSP_U32             i;
    BSP_U32             u32ResetFlag;
    SOCP_DECSRC_CHAN_S *pDecSrcChan;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pInputAttr);

    u32ChanID   = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);
    SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECSRC_CHN);
    SOCP_CHECK_DATA_TYPE_EN(pInputAttr->eDataTypeEn);

    start     = pInputAttr->sDecoderSetSrcBuf.u32InputStart;
    end       = pInputAttr->sDecoderSetSrcBuf.u32InputEnd;

    SOCP_CHECK_PARA(start);
    SOCP_CHECK_8BYTESALIGN(start);
    SOCP_CHECK_PARA(end);
    SOCP_CHECK_BUF_ADDR(start, end);

    buflength = end - start + 1;

    SOCP_CHECK_8BYTESALIGN(buflength);

    if (buflength > SOCP_DEC_SRC_BUFLGTH_MAX)
    {
        SOCP_LOG_ERR("DecoderSetSrcChanAttr: buffer length is too large", buflength, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    pDecSrcChan = &g_strSocpStat.sDecSrcChan[u32ChanID];

    if (SOCP_CHN_SET == pDecSrcChan->u32SetStat)
    {
        SOCP_LOG_ERR("DecoderSetSrcChanAttr: channel has been configed", u32ChanID, 0);
        return BSP_ERR_SOCP_DECSRC_SET;
    }

    /* ���ȸ�λͨ�� */
    SOCP_REG_SETBITS(SOCP_REG_DECRST, u32ChanID, 1, 1);

    /* �ȴ�ͨ����λ״̬���� */
    for (i = 0; i < SOCP_RESET_TIME; i++)
    {
        u32ResetFlag = SOCP_REG_GETBITS(SOCP_REG_DECRST, u32ChanID, 1);
        if (0 == u32ResetFlag)
        {
            break;
        }
    }

    if (SOCP_RESET_TIME == i)
    {
        SOCP_LOG_WARNING("DecoderSetSrcChanAttr: reset channel failed!\n", u32ChanID, 0);
    }

    /* ʹ�����ò����������� */
    SOCP_REG_WRITE(SOCP_REG_DECSRC_BUFWPTR(u32ChanID),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_DECSRC_BUFRPTR(u32ChanID),SOCP_VIRT_PHY(start));
    SOCP_REG_WRITE(SOCP_REG_DECSRC_BUFADDR(u32ChanID),SOCP_VIRT_PHY(start));

    SOCP_REG_SETBITS(SOCP_REG_DECSRC_BUFCFG0(u32ChanID), 0, 16, buflength);
    SOCP_REG_SETBITS(SOCP_REG_DECSRC_BUFCFG0(u32ChanID), 31, 1, pInputAttr->eDataTypeEn);
    SOCP_REG_SETBITS(SOCP_REG_DECSRC_RDQCFG(u32ChanID), 29, 1, 0);

    /* ��ȫ�ֱ����б��氲װ�Ĳ��� */
    pDecSrcChan->u32ChanID              = u32ChanID;
    pDecSrcChan->eDataTypeEn            = pInputAttr->eDataTypeEn;
    pDecSrcChan->sDecSrcBuf.u32Start    = start;
    pDecSrcChan->sDecSrcBuf.u32End      = end;
    pDecSrcChan->sDecSrcBuf.u32Length   = buflength;
    pDecSrcChan->sDecSrcBuf.u32Read     = start;
    pDecSrcChan->sDecSrcBuf.u32Write    = start;

    pDecSrcChan->u32SetStat = SOCP_CHN_SET;

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_DecoderGetErrCnt
*
* ��������  : ����ͨ���л�ȡ�����������
*
* �������  : u32ChanID       ����ͨ��ID

* �������  : pErrCnt         ����ͨ����������ṹ��ָ��
*
* �� �� ֵ  : ��ȡ�ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_DecoderGetErrCnt (VOS_UINT32 u32DstChanID, SOCP_DECODER_ERROR_CNT_S *pErrCnt)
{
    BSP_U32 u32ChanID;
    BSP_U32 u32ChanType;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �ж�ͨ��ID�Ƿ���Ч */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32DstChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32DstChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);
    SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECSRC_CHN);
    SOCP_CHECK_DECSRC_SET(u32ChanID);

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pErrCnt);

    /* �ж�ͨ���Ƿ�򿪣�������Ϊdebugģʽ*/
    if (g_strSocpStat.sDecSrcChan[u32ChanID].u32ChanEn)
    {
        pErrCnt->u32PktlengthCnt = SOCP_REG_GETBITS(SOCP_REG_DEC_BUFSTAT0(u32ChanID), 16, 16);
        pErrCnt->u32CrcCnt       = SOCP_REG_GETBITS(SOCP_REG_DEC_BUFSTAT0(u32ChanID), 0, 16);
        pErrCnt->u32DataTypeCnt  = SOCP_REG_GETBITS(SOCP_REG_DEC_BUFSTAT1(u32ChanID), 16, 16);
        pErrCnt->u32HdlcHeaderCnt= SOCP_REG_GETBITS(SOCP_REG_DEC_BUFSTAT1(u32ChanID), 0, 16);
    }
    else
    {
        return BSP_ERR_SOCP_SET_FAIL;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_SetTimeout
*
* ��������  : ��ʱ��ֵ���ú���
*
* �������  :   eTmOutEn          ���ö���ѡ��ʹ��
                u32Timeout        ��ʱ��ֵ
*
* �������  :
*
* �� �� ֵ  : ���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_SetTimeout (SOCP_TIMEOUT_EN_E eTmOutEn, VOS_UINT32 u32Timeout)
{
    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ��� */
    if (u32Timeout > SOCP_TIMEOUT_MAX)
    {
        SOCP_LOG_ERR("SetTimeout: the value is too large", u32Timeout, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    switch(eTmOutEn)
    {
        case SOCP_TIMEOUT_BUFOVF_DISABLE:
            SOCP_REG_SETBITS(SOCP_REG_BUFTIMEOUT, 31, 1, 0);
            break;

        case SOCP_TIMEOUT_BUFOVF_ENABLE:
            SOCP_REG_SETBITS(SOCP_REG_BUFTIMEOUT, 31, 1, 1);
            /* ���ӻ���ķ��� */
            SOCP_REG_SETBITS(SOCP_REG_BUFTIMEOUT, 0, 16, u32Timeout);
            break;

        case SOCP_TIMEOUT_TRF:
            /* ���䳬ʱʱ�����ò���Ҫ�漰ͨ��ID*/
            SOCP_REG_WRITE(SOCP_REG_INTTIMEOUT, u32Timeout);
            break;

        default:
            SOCP_LOG_ERR("SetTimeout: invalid timeout choice type", eTmOutEn, 0);
            return BSP_ERR_SOCP_SET_FAIL;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_SetDecPktLgth
*
* ��������  : ��ʱ��ֵ���ú���
*
* �������  :   pPktlgth          ������������ò����ṹ��
*
* �������  :
*
* �� �� ֵ  : ���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_SetDecPktLgth(SOCP_DEC_PKTLGTH_S *pPktlgth)
{
    BSP_U32 u32PktMaxLgth;
    BSP_U32 u32PktMinLgth;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pPktlgth);
    u32PktMaxLgth = pPktlgth->u32PktMax;
    u32PktMinLgth = pPktlgth->u32PktMin;

    SOCP_CHECK_PARA(u32PktMaxLgth);

    if (u32PktMaxLgth < u32PktMinLgth)
    {
        SOCP_LOG_ERR("SetDecPktLgth: max length is smaller than min length",u32PktMaxLgth, u32PktMinLgth);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    if (u32PktMaxLgth > SOCP_DEC_MAXPKT_MAX)
    {
        SOCP_LOG_ERR("SetDecPktLgth: u32PktMaxLgth is too large!\n", u32PktMaxLgth, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    if (u32PktMinLgth > SOCP_DEC_MINPKT_MAX)
    {
        SOCP_LOG_ERR("SetDecPktLgth: u32PktMinLgth is too large", u32PktMinLgth, 0);
        return BSP_ERR_SOCP_INVALID_PARA;
    }

    /* ���ý���ͨ·���������üĴ���*/
    SOCP_REG_SETBITS(SOCP_REG_DEC_PKTLEN, 0, 12, u32PktMaxLgth);
    SOCP_REG_SETBITS(SOCP_REG_DEC_PKTLEN, 12, 5, u32PktMinLgth);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_SetDebug
*
* ��������  : ���ý���Դͨ��debugģʽ����
*
* �������  : u32DecChanID  ����Դͨ��ID
              u32DebugEn    debugģʽʹ�ܱ�ʶ
*
* �������  :
*
* �� �� ֵ  : ���óɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_SetDebug(VOS_UINT32 u32DecChanID, VOS_UINT32 u32DebugEn)
{
    BSP_U32 u32ChanID;
    BSP_U32 u32ChanType;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �ж�ͨ��ID�Ƿ���Ч */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32DecChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32DecChanID);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);
    SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECSRC_CHN);
    SOCP_CHECK_DECSRC_SET(u32ChanID);

    /* �жϸ�ͨ����ģʽ��û�д򿪵Ļ����������� */
    if(g_strSocpStat.sDecSrcChan[u32ChanID].u32ChanEn)
    {
        SOCP_LOG_ERR("SetDebug: decoder channel is open, can't set debug bit", u32DecChanID, u32DebugEn);
        return BSP_ERR_SOCP_SET_FAIL;
    }
    else
    {
        SOCP_REG_SETBITS(SOCP_REG_DECSRC_BUFCFG0(u32ChanID), 29, 1, u32DebugEn);
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : SocpDecSrcStart
*
* ��������  : ������������
*
* �������  : ulRealChanID      ͨ��ID
* �������  :
*
* �� �� ֵ  : �����ɹ����ı�ʶ��
*****************************************************************************/
/*lint -e529*/
VOS_UINT32 SocpDecSrcStart(VOS_UINT32 ulRealChanID)
{
    VOS_UINT32 i;
    VOS_UINT32 ulIntIDMask  = 0;
    VOS_UINT32 ulLogic      = 1;

    SOCP_CHECK_CHAN_ID(ulRealChanID, SOCP_MAX_DECSRC_CHN);
    SOCP_CHECK_DECSRC_SET(ulRealChanID);

    /* ��rd����ж�*/
    if (SOCP_DECSRC_CHNMODE_LIST == g_strSocpStat.sDecSrcChan[ulRealChanID].eChnMode)
    {
        SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT1, ulRealChanID, 1, 1);
        SOCP_REG_SETBITS(SOCP_REG_DEC_MASK1, ulRealChanID, 1, 0);
    }

    for (i = 1; i < SOCP_DEC_SRCINT_NUM; i++)
    {
        ulIntIDMask  = SOCP_REG_GETBITS(SOCP_REG_DEC_RAWINT1, i * 4, 4);
        ulIntIDMask |= 1 << ulRealChanID;
        SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT1, (i*4), 4, ulIntIDMask);

        ulIntIDMask  = SOCP_REG_GETBITS(SOCP_REG_DEC_MASK1, i * 4, 4);
        ulIntIDMask &= ~(ulLogic << ulRealChanID);
        SOCP_REG_SETBITS(SOCP_REG_DEC_MASK1, (i*4), 4, ulIntIDMask);
    }

    /* ���ô�״̬*/
    SOCP_REG_SETBITS(SOCP_REG_DECSRC_BUFCFG0(ulRealChanID), 30, 1, 1);

    g_strSocpStat.sDecSrcChan[ulRealChanID].u32ChanEn = SOCP_CHN_ENABLE;

    g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpStartDecSrcCnt[ulRealChanID]++;

    return VOS_OK;
}
/*lint +e529*/

#endif

/*****************************************************************************
* �� �� ��  : SocpEncSrcStart
*
* ��������  : ������߽�����������
*
* �������  : ulRealChanID      ͨ��ID
* �������  :
*
* �� �� ֵ  : �����ɹ����ı�ʶ��
*****************************************************************************/
/*lint -e529*/
VOS_UINT32 SocpEncSrcStart(VOS_UINT32 ulRealChanID)
{
    VOS_UINT32      ulDstId;
#ifdef SOCP_CCORE
    VOS_UINT32      ulStart;
#endif

    SOCP_CHECK_CHAN_ID(ulRealChanID, SOCP_MAX_ENCSRC_CHN);
    SOCP_CHECK_ENCSRC_SET(ulRealChanID);

    /* ����ͨ�� */
    ulDstId = SOCP_REG_GETBITS(SOCP_REG_ENCSRC_BUFCFG1(ulRealChanID), 4, 4);

#ifdef SOCP_ACORE
    if (SOCP_CHN_SET != g_strSocpStat.sEncDstChan[ulDstId].u32SetStat)
    {
        SOCP_LOG_ERR("BSP_SOCP_Start: Acpu enc src's dst Channel is valid", ulRealChanID, 0);
        return BSP_ERR_SOCP_DEST_CHAN;
    }

    SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT1, ulRealChanID, 1, 1);
    SOCP_REG_SETBITS(SOCP_REG_APP_MASK1, ulRealChanID, 1, 0);
#endif

#ifdef SOCP_CCORE
    /* ͨ���Ĵ����ж� */
    SOCP_REG_READ(SOCP_REG_ENCDEST_BUFADDR(ulDstId), ulStart);

    if (0 == ulStart)
    {
        SOCP_LOG_ERR("BSP_SOCP_Start: Ccpu enc dst Channel is valid", ulRealChanID, 0);
        return BSP_ERR_SOCP_DEST_CHAN;
    }
#endif

    /* ���ô�״̬*/
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(ulRealChanID), 0, 1, 1);

    g_strSocpStat.sEncSrcChan[ulRealChanID].u32ChanEn = SOCP_CHN_ENABLE;
    g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpStartEncSrcCnt[ulRealChanID]++;

    return VOS_OK;
}

/*lint +e529*/

/*****************************************************************************
* �� �� ��  : BSP_SOCP_Start
*
* ��������  : ������߽�����������
*
* �������  : u32SrcChanID      ͨ��ID
* �������  :
*
* �� �� ֵ  : �����ɹ����ı�ʶ��
*****************************************************************************/

/*lint -e529*/
VOS_UINT32 BSP_SOCP_Start(VOS_UINT32 u32SrcChanID)
{
    BSP_U32 u32RealChanID;
    BSP_U32 u32ChanType;


    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �ж�ͨ��ID�Ƿ���Ч */
    u32RealChanID = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        return SocpEncSrcStart(u32RealChanID);
    }

#ifdef SOCP_ACORE
    else if (SOCP_DECODER_SRC_CHAN == u32ChanType)
    {
        return SocpDecSrcStart(u32RealChanID);
    }
#endif
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_Start: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }
}

/*lint +e529*/

/*****************************************************************************
* �� �� ��  : BSP_SOCP_Stop
*
* ��������  : ������߽���ֹͣ����
*
* �������  : u32SrcChanID      ͨ��ID
*
* �������  :
*
* �� �� ֵ  : ֹͣ�ɹ����ı�ʶ��
*****************************************************************************/

/*lint -e529*/
VOS_UINT32 BSP_SOCP_Stop(VOS_UINT32 u32SrcChanID)
{
    BSP_U32 u32RealChanID;
    BSP_U32 u32ChanType;

#ifdef SOCP_ACORE
    BSP_U32 IntIDMask = 0;
    BSP_U32 i;
#endif

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    SOCP_LOG_ERR("BSP_SOCP_Stop Channel ID", u32SrcChanID, 0);

    /* �ж�ͨ��ID�Ƿ���Ч */
    u32RealChanID = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_ENCSRC_CHN);
        SOCP_CHECK_ENCSRC_SET(u32RealChanID);

        /* �ر��ж�*/
#ifdef SOCP_ACORE
        SOCP_REG_SETBITS(SOCP_REG_APP_MASK1, u32RealChanID, 1, 1);
#endif

        /* ����ͨ���ر�״̬*/
        SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(u32RealChanID), 0, 1, 0);

        g_strSocpStat.sEncSrcChan[u32RealChanID].u32ChanEn = SOCP_CHN_DISABLE;

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpStopEncSrcCnt[u32RealChanID]++;
    }

#ifdef SOCP_ACORE
    else if (SOCP_DECODER_SRC_CHAN == u32ChanType)
    {
        SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_DECSRC_CHN);
        SOCP_CHECK_DECSRC_SET(u32RealChanID);

        /* �ر��ж�*/
        if (SOCP_DECSRC_CHNMODE_LIST == g_strSocpStat.sDecSrcChan[u32RealChanID].eChnMode)
        {
            SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0, u32RealChanID, 1, 1);
        }

        for (i = 1; i < SOCP_DEC_SRCINT_NUM; i++)
        {
            IntIDMask  = SOCP_REG_GETBITS(SOCP_REG_DEC_MASK1, i * 4, 4);
            IntIDMask |= 1 << u32RealChanID;
            SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0, i * 4, 4, IntIDMask);
        }

        /* ����ͨ���ر�״̬*/
        SOCP_REG_SETBITS(SOCP_REG_DECSRC_BUFCFG0(u32RealChanID), 30, 1, 0);

        g_strSocpStat.sDecSrcChan[u32RealChanID].u32ChanEn = SOCP_CHN_DISABLE;

        g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpStopDecSrcCnt[u32RealChanID]++;
    }
#endif
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_Stop: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}

/*lint +e529*/
/*****************************************************************************
* �� �� ��  : BSP_SOCP_RegisterEventCB
*
* ��������  : �쳣�¼��ص�����ע�ắ��
*
* �������  : u32ChanID      ͨ��ID
*             EventCB        �쳣�¼��Ļص�����
* �������  :
*
* �� �� ֵ  : ע��ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_RegisterEventCB(VOS_UINT32 u32ChanID, socp_event_cb EventCB)
{
    BSP_U32  u32RealChanID;
    BSP_U32  u32ChanType;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* ��ȡͨ�����ͺ�ʵ�ʵ�ͨ��ID */
    u32RealChanID = SOCP_REAL_CHAN_ID(u32ChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32ChanID);

    switch (u32ChanType)
    {
        case SOCP_CODER_SRC_CHAN:      /* ����Դͨ�� */
            SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_ENCSRC_CHN);
            SOCP_CHECK_ENCSRC_SET(u32RealChanID);
            g_strSocpStat.sEncSrcChan[u32RealChanID].event_cb = EventCB;
            g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpRegEventEncSrcCnt[u32RealChanID]++;
            break;

#ifdef SOCP_ACORE
        case SOCP_DECODER_SRC_CHAN:       /* ����Դͨ��*/
            SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_DECSRC_CHN);
            SOCP_CHECK_DECSRC_SET(u32RealChanID);
            g_strSocpStat.sDecSrcChan[u32RealChanID].event_cb = EventCB;
            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpRegEventDecSrcCnt[u32RealChanID]++;
            break;
#endif

        default:
            SOCP_LOG_ERR("BSP_SOCP_RegisterEventCB: invalid Channel type", u32ChanID, u32ChanType);
            return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_GetWriteBuff
*
* ��������  : �ϲ��ȡд����buffer����
*
* �������  : u32SrcChanID    Դͨ��ID
* �������  : pBuff           ��ȡ��buffer
*
* �� �� ֵ  : ��ȡ�ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_GetWriteBuff(VOS_UINT32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff)
{
    BSP_U32 u32ChanID;
    BSP_U32 u32ChanType;
    BSP_U32 uPAddr;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pBuff);

    SOCP_LOG_INFO("BSP_SOCP_GetWriteBuff: Channel id ", u32SrcChanID, 0);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpGetWBufEncSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCSRC_CHN);
        SOCP_CHECK_ENCSRC_SET(u32ChanID);

        /* ���ݶ�дָ���ȡbuffer */
        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFRPTR(u32ChanID), uPAddr);

        g_strSocpStat.sEncSrcChan[u32ChanID].sEncSrcBuf.u32Read =(BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFWPTR(u32ChanID), uPAddr);

        g_strSocpStat.sEncSrcChan[u32ChanID].sEncSrcBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetIdleBuffer(&g_strSocpStat.sEncSrcChan[u32ChanID].sEncSrcBuf, pBuff);

        SOCP_LOG_INFO("BSP_SOCP_GetWriteBuff: Enc Src Buffer Info", (VOS_UINT32)pBuff->pBuffer, pBuff->u32Size);

        SOCP_LOG_INFO("BSP_SOCP_GetWriteBuff: Enc Src Buffer Rb Info", (VOS_UINT32)pBuff->pRbBuffer, pBuff->u32RbSize);

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpGetWBufEncSrcSucCnt[u32ChanID]++;
    }

#ifdef SOCP_ACORE
    else if (SOCP_DECODER_SRC_CHAN == u32ChanType) /* ����ͨ�� */
    {
        g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpGetWBufDecSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECSRC_CHN);
        SOCP_CHECK_DECSRC_SET(u32ChanID);

        SOCP_REG_READ(SOCP_REG_DECSRC_BUFRPTR(u32ChanID), uPAddr);

        g_strSocpStat.sDecSrcChan[u32ChanID].sDecSrcBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_DECSRC_BUFWPTR(u32ChanID), uPAddr);

        g_strSocpStat.sDecSrcChan[u32ChanID].sDecSrcBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetIdleBuffer(&g_strSocpStat.sDecSrcChan[u32ChanID].sDecSrcBuf, pBuff);

        SOCP_LOG_INFO("BSP_SOCP_GetWriteBuff: Dec Src Buffer Info", (VOS_UINT32)pBuff->pBuffer, pBuff->u32Size);

        SOCP_LOG_INFO("BSP_SOCP_GetWriteBuff: Dec Src Buffer Rb Info", (VOS_UINT32)pBuff->pRbBuffer, pBuff->u32RbSize);

        g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpGetWBufDecSrcSucCnt[u32ChanID]++;
    }
#endif  /*SOCP_ACORE*/
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_GetWriteBuff: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_WriteDone
*
* ��������  : д������ɺ���
*
* �������  : u32SrcChanID    Դͨ��ID
              u32WrtSize      д�����ݵĳ���
*
* �������  :
*
* �� �� ֵ  : ����������ı�ʶ��
*****************************************************************************/
#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
VOS_UINT32  g_aulSOCPWriteSlice[SOCP_MAX_ENCSRC_CHN] = {0};
#endif

VOS_UINT32 BSP_SOCP_WriteDone(VOS_UINT32 u32SrcChanID, VOS_UINT32 u32WrtSize)
{
    BSP_U32             u32ChanID;
    BSP_U32             u32ChanType;
    BSP_U32             u32WrtPad;
#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
    BSP_U32             ulBaseAddReg;
    BSP_U32             ulLenReg;
#endif
    SOCP_BUFFER_RW_S    RwBuff;
    BSP_U32             uPAddr;
#if(VOS_OS_VER == VOS_WIN32)    /* for PC Stub */
    VOS_UINT32          ulWritePtr;
#endif
    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(u32WrtSize);

    SOCP_LOG_INFO("BSP_SOCP_WriteDone: Channel id ", u32SrcChanID, u32WrtSize);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        SOCP_ENCSRC_CHAN_S *pChan;

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpWriteDoneEncSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCSRC_CHN);
        SOCP_CHECK_ENCSRC_SET(u32ChanID);

        pChan = &g_strSocpStat.sEncSrcChan[u32ChanID];

        u32WrtPad = u32WrtSize % 8;

        if (0 != u32WrtPad)
        {
            u32WrtSize += (8 - u32WrtPad);
        }

        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFWPTR(u32ChanID), uPAddr);

#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFADDR(u32ChanID), ulBaseAddReg);
        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFCFG0(u32ChanID), ulLenReg);

        if ((uPAddr < ulBaseAddReg)||(uPAddr > (ulBaseAddReg+ulLenReg)))
        {
            SOCP_LOG_ERR("BSP_SOCP_WriteDone: Channel write reg error", u32SrcChanID, u32WrtSize);

            return BSP_ERR_SOCP_INVALID_PARA;
        }

        g_aulSOCPWriteSlice[u32ChanID] = OM_GetSlice();
#endif

        pChan->sEncSrcBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCSRC_BUFRPTR(u32ChanID), uPAddr);

        pChan->sEncSrcBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetIdleBuffer(&pChan->sEncSrcBuf, &RwBuff);

        if (RwBuff.u32Size + RwBuff.u32RbSize < u32WrtSize)
        {
            SOCP_LOG_ERR("BSP_SOCP_WriteDone: enc src too large write size. write ptr is, read ptr is",
                      pChan->sEncSrcBuf.u32Write, pChan->sEncSrcBuf.u32Read);

            SOCP_LOG_ERR("BSP_SOCP_WriteDone: enc src too large write size. u32Size is, u32RbSize is",
                      RwBuff.u32Size, RwBuff.u32RbSize);

            g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpWriteDoneEncSrcFailCnt[u32ChanID]++;

            return BSP_ERR_SOCP_INVALID_PARA;
        }

#if(VOS_OS_VER == VOS_WIN32)    /* for PC Stub */
        ulWritePtr = pChan->sEncSrcBuf.u32Write;
#endif

        /* ���ö�дָ�� */
        SocpWriteDone(&pChan->sEncSrcBuf, u32WrtSize);

        /* д��дָ�뵽дָ��Ĵ���*/
        uPAddr = SOCP_VIRT_PHY(pChan->sEncSrcBuf.u32Write);

        SOCP_REG_WRITE(SOCP_REG_ENCSRC_BUFWPTR(u32ChanID), uPAddr);

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpWriteDoneEncSrcSucCnt[u32ChanID]++;
    }

#ifdef SOCP_ACORE
    else  if(SOCP_DECODER_SRC_CHAN == u32ChanType) /* ����ͨ�� */
    {
        SOCP_DECSRC_CHAN_S  *pChan;

        g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpWriteDoneDecSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECSRC_CHN);
        SOCP_CHECK_DECSRC_SET(u32ChanID);

        pChan = &g_strSocpStat.sDecSrcChan[u32ChanID];

        SOCP_REG_READ(SOCP_REG_DECSRC_BUFWPTR(u32ChanID), uPAddr);

        pChan->sDecSrcBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_DECSRC_BUFRPTR(u32ChanID), uPAddr);

        pChan->sDecSrcBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetIdleBuffer(&pChan->sDecSrcBuf, &RwBuff);

        if (RwBuff.u32Size + RwBuff.u32RbSize < u32WrtSize)
        {
            SOCP_LOG_ERR("BSP_SOCP_WriteDone: dec src too large write size. write ptr is, read ptr is",
                      pChan->sDecSrcBuf.u32Write, pChan->sDecSrcBuf.u32Read);

            SOCP_LOG_ERR("BSP_SOCP_WriteDone: dec src too large write size. u32Size is, u32RbSize is",
                      RwBuff.u32Size, RwBuff.u32RbSize);

            g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpWriteDoneDecSrcFailCnt[u32ChanID]++;

            return BSP_ERR_SOCP_INVALID_PARA;
        }

#if(VOS_OS_VER == VOS_WIN32)    /* for PC Stub */
        ulWritePtr = pChan->sDecSrcBuf.u32Write;
#endif

        /* ���ö�дָ�� */
        SocpWriteDone(&pChan->sDecSrcBuf, u32WrtSize);

        /* д��дָ�뵽дָ��Ĵ���*/
        uPAddr = SOCP_VIRT_PHY(pChan->sDecSrcBuf.u32Write);

        SOCP_REG_WRITE(SOCP_REG_DECSRC_BUFWPTR(u32ChanID), uPAddr);

        g_stSocpDebugInfo.sSocpDebugDecSrc.u32SocpWriteDoneDecSrcSucCnt[u32ChanID]++;
    }
#endif
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_WriteDone: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

#if(VOS_OS_VER == VOS_WIN32)    /* for PC Stub */
    Stub_SendSocpData(u32SrcChanID, (VOS_UINT8*)ulWritePtr, u32WrtSize);
#endif

#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
    VOS_FlushCpuWriteBuf();
#endif    

    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : BSP_SOCP_GetRDBuffer
*
* ��������  : ��ȡRDbuffer����
*
* �������  : u32SrcChanID    Դͨ��ID
*
* �������  : pBuff           ��ȡ��RDbuffer
*
* �� �� ֵ  : ��ȡ�ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_GetRDBuffer(VOS_UINT32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff)
{
    BSP_U32     u32ChanID;
    BSP_U32     u32ChanType;
    BSP_U32     uPAddr;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pBuff);

    SOCP_LOG_INFO("BSP_SOCP_GetRDBuffer: Channel is", u32SrcChanID, 0);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpGetRdBufEncSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCSRC_CHN);
        SOCP_CHECK_ENCSRC_SET(u32ChanID);

        /* ���ݶ�дָ���ȡbuffer */
        SOCP_REG_READ(SOCP_REG_ENCSRC_RDQRPTR(u32ChanID), uPAddr);
        g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCSRC_RDQWPTR(u32ChanID), uPAddr);
        g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&g_strSocpStat.sEncSrcChan[u32ChanID].sRdBuf, pBuff);

        SOCP_LOG_INFO("BSP_SOCP_GetRDBuffer: pBuff Info is", (VOS_UINT32)pBuff->pBuffer, pBuff->u32Size);

        SOCP_LOG_INFO("BSP_SOCP_GetRDBuffer: pBuff Rb Info is", (VOS_UINT32)pBuff->pRbBuffer, pBuff->u32RbSize);

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpGetRdBufEncSrcSucCnt[u32ChanID]++;
    }
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_GetRDBuffer: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : BSP_SOCP_ReadRDDone
*
* ��������  : ��ȡRDbuffer������ɺ���
*
* �������  : u32SrcChanID   Դͨ��ID
              u32RDSize      ��ȡ��RDbuffer���ݳ���
*
* �������  :
*
* �� �� ֵ  : ��ȡ�ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_ReadRDDone(VOS_UINT32 u32SrcChanID, VOS_UINT32 u32RDSize)
{
    BSP_U32             u32ChanID;
    BSP_U32             u32ChanType;
    SOCP_BUFFER_RW_S    RwBuff;
    BSP_U32             uPAddr;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(u32RDSize);

    SOCP_LOG_INFO("BSP_SOCP_ReadRDDone: Read RD Done", u32SrcChanID, u32RDSize);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32SrcChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32SrcChanID);

    /* ����ͨ�� */
    if (SOCP_CODER_SRC_CHAN == u32ChanType)
    {
        SOCP_ENCSRC_CHAN_S *pChan;

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpReadRdDoneEncSrcEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCSRC_CHN);
        SOCP_CHECK_ENCSRC_SET(u32ChanID);

        pChan = &g_strSocpStat.sEncSrcChan[u32ChanID];
        g_strSocpStat.sEncSrcChan[u32ChanID].u32LastRdSize = 0;

        /* ���ö�дָ�� */
        SOCP_REG_READ(SOCP_REG_ENCSRC_RDQWPTR(u32ChanID), uPAddr);
        pChan->sRdBuf.u32Write = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCSRC_RDQRPTR(u32ChanID), uPAddr);
        pChan->sRdBuf.u32Read  = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&pChan->sRdBuf, &RwBuff);

        if ((RwBuff.u32Size+RwBuff.u32RbSize) < u32RDSize)
        {
            SOCP_LOG_ERR("BSP_SOCP_ReadRDDone: enc src too large rd size, write ptr is, read ptr is",
                      pChan->sRdBuf.u32Write, pChan->sRdBuf.u32Read);

            SOCP_LOG_ERR("BSP_SOCP_ReadRDDone: enc src too large rd size u32Size is, u32RbSize is",
                      RwBuff.u32Size, RwBuff.u32RbSize);

            g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpReadRdDoneEncSrcFailCnt[u32ChanID]++;

            return BSP_ERR_SOCP_INVALID_PARA;
        }

        SocpReadDone(&pChan->sRdBuf, u32RDSize);

        /* д��дָ�뵽дָ��Ĵ���*/
        uPAddr= SOCP_VIRT_PHY(pChan->sRdBuf.u32Read);
        SOCP_REG_WRITE(SOCP_REG_ENCSRC_RDQRPTR(u32ChanID), uPAddr);

        g_stSocpDebugInfo.sSocpDebugEncSrc.u32SocpReadRdDoneEncSrcSucCnt[u32ChanID]++;
    }
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_ReadRDDone: invalid Channel type", u32SrcChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
    VOS_FlushCpuWriteBuf();
#endif    

    return BSP_OK;
}

/*����Ŀ��ͨ��ר��
 */
#ifdef SOCP_ACORE
/*****************************************************************************
* �� �� ��  : BSP_SOCP_RegisterReadCB
*
* ��������  : �����ݻص�����ע�ắ��
*
* �������  : u32DestChanID  Ŀ��ͨ��ID
              ReadCB         �����ݻص�����
*
* �������  :
*
* �� �� ֵ  : ע��ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_RegisterReadCB(VOS_UINT32 u32DestChanID, socp_read_cb ReadCB)
{
    BSP_U32 u32RealChanID;
    BSP_U32 u32ChanType;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* ��ȡͨ�����ͺ�ʵ�ʵ�ͨ��ID */
    u32RealChanID = SOCP_REAL_CHAN_ID(u32DestChanID);
    u32ChanType   = SOCP_REAL_CHAN_TYPE(u32DestChanID);

    if (SOCP_DECODER_DEST_CHAN == u32ChanType) /* ����ͨ�� */
    {
        SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_DECDST_CHN);
        SOCP_CHECK_DECDST_SET(u32RealChanID);

        g_strSocpStat.sDecDstChan[u32RealChanID].read_cb = ReadCB;

        g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpRegReadCBDecDstCnt[u32RealChanID]++;
    }

    else if (SOCP_CODER_DEST_CHAN == u32ChanType)/* ����ͨ�� */
    {
        SOCP_CHECK_CHAN_ID(u32RealChanID, SOCP_MAX_ENCDST_CHN);
        SOCP_CHECK_ENCDST_SET(u32RealChanID);

        /* ���ö�Ӧͨ���Ļص�����*/
        g_strSocpStat.sEncDstChan[u32RealChanID].read_cb = ReadCB;

        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpRegReadCBEncDstCnt[u32RealChanID]++;
    }
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_RegisterReadCB: invalid Channel type", u32DestChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_GetReadBuff
*
* ��������  : ��ȡ������buffer����
*
* �������  : u32DestChanID  Ŀ��ͨ��buffer

* �������  : pBuffer        ��ȡ�Ķ�����buffer
*
* �� �� ֵ  : ��ȡ�ɹ����ı�ʶ��
*****************************************************************************/
VOS_UINT32 BSP_SOCP_GetReadBuff(VOS_UINT32 u32DestChanID, SOCP_BUFFER_RW_S *pBuffer)
{
    BSP_U32  u32ChanID;
    BSP_U32  u32ChanType;
    BSP_U32  uPAddr;

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /* �жϲ�����Ч�� */
    SOCP_CHECK_PARA(pBuffer);

    SOCP_LOG_INFO("BSP_SOCP_GetReadBuff: Channel is", u32DestChanID, 0);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32DestChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32DestChanID);
    pBuffer->u32Size   = 0;
    pBuffer->u32RbSize = 0;

    if (SOCP_DECODER_DEST_CHAN == u32ChanType) /* ����ͨ�� */
    {
        g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpGetReadBufDecDstEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECDST_CHN);
        SOCP_CHECK_DECDST_SET(u32ChanID);

        /* ���ݶ�дָ���ȡbuffer */
        SOCP_REG_READ(SOCP_REG_DECDEST_BUFRPTR(u32ChanID), uPAddr);
        g_strSocpStat.sDecDstChan[u32ChanID].sDecDstBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_DECDEST_BUFWPTR(u32ChanID), uPAddr);
        g_strSocpStat.sDecDstChan[u32ChanID].sDecDstBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&g_strSocpStat.sDecDstChan[u32ChanID].sDecDstBuf, pBuffer);

        SOCP_LOG_INFO("BSP_SOCP_GetReadBuff: Channel is", u32DestChanID, 0);

        g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpGetReadBufDecDstSucCnt[u32ChanID]++;
    }
    else if (SOCP_CODER_DEST_CHAN == u32ChanType)
    {
        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpGetReadBufEncDstEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCDST_CHN);
        SOCP_CHECK_ENCDST_SET(u32ChanID);

        /* ���ݶ�дָ���ȡbuffer */
        SOCP_REG_READ(SOCP_REG_ENCDEST_BUFRPTR(u32ChanID), uPAddr);
        g_strSocpStat.sEncDstChan[u32ChanID].sEncDstBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCDEST_BUFWPTR(u32ChanID), uPAddr);
        g_strSocpStat.sEncDstChan[u32ChanID].sEncDstBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&g_strSocpStat.sEncDstChan[u32ChanID].sEncDstBuf, pBuffer);

        SOCP_LOG_INFO("BSP_SOCP_GetReadBuff: Channel is", u32DestChanID, (pBuffer->u32RbSize+pBuffer->u32Size));

        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpGetReadBufEncDstSucCnt[u32ChanID]++;
    }
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_GetReadBuff: invalid Channel type", u32DestChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : BSP_SOCP_ReadDataDone
*
* ��������  : ��������ɺ���
*
* �������  : u32DestChanID    Ŀ��ͨ��ID
*             u32ReadSize      ��ȡ���ݴ�С
* �������  : ��
*
* �� �� ֵ  : �����ݳɹ����ı�ʶ��
*****************************************************************************/

VOS_UINT32 BSP_SOCP_ReadDataDone(VOS_UINT32 u32DestChanID, VOS_UINT32 u32ReadSize)
{
    BSP_U32             u32ChanID;
    BSP_U32             u32ChanType;
    VOS_UINT32          ulLockLevel;
    SOCP_BUFFER_RW_S    RwBuff;
    BSP_U32             uPAddr;
    SOCP_ENC_DST_BUF_LOG_CFG_STRU       *pstcfg;

    pstcfg = SocpGetSDLogCfg();

    /* �ж��Ƿ��Ѿ���ʼ�� */
    SOCP_CHECK_INIT();

    /*SOCP_CHECK_PARA(u32ReadSize);*/   /* Ϊ�˷�ֹIP�ϱ������ݵ��ж� */

    SOCP_LOG_INFO("BSP_SOCP_ReadDataDone: Channel ID and len is", u32DestChanID, u32ReadSize);

    /* ���ʵ��ͨ��id */
    u32ChanID   = SOCP_REAL_CHAN_ID(u32DestChanID);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32DestChanID);

    if (SOCP_DECODER_DEST_CHAN == u32ChanType) /* ����ͨ�� */
    {
        SOCP_DECDST_CHAN_S *pChan;

        g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpReadDoneDecDstEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_DECDST_CHN);
        SOCP_CHECK_DECDST_SET(u32ChanID);

        pChan = &g_strSocpStat.sDecDstChan[u32ChanID];

        SOCP_REG_READ(SOCP_REG_DECDEST_BUFWPTR(u32ChanID), uPAddr);
        pChan->sDecDstBuf.u32Write= (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_DECDEST_BUFRPTR(u32ChanID), uPAddr);
        pChan->sDecDstBuf.u32Read = (BSP_U32)SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&pChan->sDecDstBuf, &RwBuff);

        if(RwBuff.u32Size + RwBuff.u32RbSize < u32ReadSize)
        {
            SOCP_LOG_ERR("BSP_SOCP_ReadDataDone: dec dst too large read size. write ptr is, read ptr is", pChan->sDecDstBuf.u32Write,
                      pChan->sDecDstBuf.u32Read);

            SOCP_LOG_ERR("BSP_SOCP_ReadDataDone: dec dst too large read size. u32Size is 0x%x, u32RbSize is 0x%x\n",
                      RwBuff.u32Size, RwBuff.u32RbSize);

            /*s32LockKey = (BSP_S32)VOS_SplIMP();*/
            VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

            /* added by yangzhi 2011.7.25 */
            SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0, u32ChanID, 1, 0);

            /*�������ж�״̬��������ж�����*/
            SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT2,      u32ChanID, 1, 1);
            SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK2,   u32ChanID, 1, 0);

            /*VOS_Splx((VOS_INT32)s32LockKey);*/
            VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

            g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpReadDoneDecDstFailCnt[u32ChanID]++;

            return BSP_ERR_SOCP_INVALID_PARA;
        }

        /* ���ö�дָ�� */
        SocpReadDone(&pChan->sDecDstBuf, u32ReadSize);

        /* д��дָ�뵽дָ��Ĵ���*/
        uPAddr = SOCP_VIRT_PHY(pChan->sDecDstBuf.u32Read);
        SOCP_REG_WRITE(SOCP_REG_DECDEST_BUFRPTR(u32ChanID), uPAddr);

        /*s32LockKey = (BSP_S32)VOS_SplIMP(); */        /*���ж�*/
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        /* added by yangzhi 2011.7.25 */
        SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK0, u32ChanID, 1, 0);

        /*�������ж�״̬��������ж�����*/
        SOCP_REG_SETBITS(SOCP_REG_DEC_RAWINT2,      u32ChanID, 1, 1);
        SOCP_REG_SETBITS(SOCP_REG_DEC_CORE0MASK2,   u32ChanID, 1, 0);

        /*VOS_Splx((VOS_INT32)s32LockKey);*/
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        g_stSocpDebugInfo.sSocpDebugDecDst.u32SocpReadDoneDecDstSucCnt[u32ChanID]++;
    }
    else if (SOCP_CODER_DEST_CHAN == u32ChanType)/* ����ͨ�� */
    {
        SOCP_ENCDST_CHAN_S *pChan;

        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpReadDoneEncDstEtrCnt[u32ChanID]++;

        /* ����ͨ��id */
        SOCP_CHECK_CHAN_ID(u32ChanID, SOCP_MAX_ENCDST_CHN);
        SOCP_CHECK_ENCDST_SET(u32ChanID);

        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpReadDoneValidEncDstCnt[u32ChanID]++;

        pChan = &g_strSocpStat.sEncDstChan[u32ChanID];

        SOCP_REG_READ(SOCP_REG_ENCDEST_BUFWPTR(u32ChanID), uPAddr);
        pChan->sEncDstBuf.u32Write= SOCP_PHY_VIRT(uPAddr);

        SOCP_REG_READ(SOCP_REG_ENCDEST_BUFRPTR(u32ChanID), uPAddr);
        pChan->sEncDstBuf.u32Read = SOCP_PHY_VIRT(uPAddr);

        SocpGetDataBuffer(&pChan->sEncDstBuf, &RwBuff);

        if (RwBuff.u32Size + RwBuff.u32RbSize < u32ReadSize)
        {
            SOCP_LOG_ERR("BSP_SOCP_ReadDataDone: enc dst too large read size. write ptr is, read ptr is",
                            pChan->sEncDstBuf.u32Write, pChan->sEncDstBuf.u32Read);

            SOCP_LOG_ERR("BSP_SOCP_ReadDataDone: enc dst too large read size. u32Size is, u32RbSize is",
                            RwBuff.u32Size, RwBuff.u32RbSize);

            /*s32LockKey = (BSP_S32)VOS_SplIMP();*/
            VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

            SOCP_REG_SETBITS(SOCP_REG_ENC_MASK0, u32ChanID, 1, 0);

            /*�����ֵ�����жϣ�����ֵ�����жϣ�����жϿ����ٴ��ϱ�*/
            SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 1);
            SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 0);

            g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpReadDoneEncDstFailCnt[u32ChanID]++;

            /*VOS_Splx((VOS_INT32)s32LockKey);*/
            VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

            if(VOS_TRUE != pstcfg->logOnFlag)
            {
                DRV_SYSTEM_ERROR(SOCP_ERRORNO_ENCDST_READ_DONE_REGERR, (int)u32ReadSize, 0, (char*)&RwBuff, sizeof(RwBuff));
            }

            return BSP_ERR_SOCP_INVALID_PARA;
        }

        /* ���ö�дָ�� */
        SocpReadDone(&pChan->sEncDstBuf, u32ReadSize);

        /* д��дָ�뵽дָ��Ĵ���*/
        uPAddr = SOCP_VIRT_PHY(pChan->sEncDstBuf.u32Read);
        SOCP_REG_WRITE(SOCP_REG_ENCDEST_BUFRPTR(u32ChanID), uPAddr);

        /*s32LockKey = (BSP_S32)VOS_SplIMP();*/
        VOS_SpinLockIntLock(&g_stSocpSpinLock, ulLockLevel);

        SOCP_REG_SETBITS(SOCP_REG_ENC_MASK0, u32ChanID, 1, 0);

        /*�����ֵ�����жϣ�����ֵ�����жϣ�����жϿ����ٴ��ϱ�*/
        SOCP_REG_SETBITS(SOCP_REG_ENC_RAWINT2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 1);
        SOCP_REG_SETBITS(SOCP_REG_ENC_MASK2, u32ChanID+SOCP_ENC_DST_BUFF_THRESHOLD_OVF_BEGIN, 1, 0);

        /*VOS_Splx((VOS_INT32)s32LockKey);*/
        VOS_SpinUnlockIntUnlock(&g_stSocpSpinLock, ulLockLevel);

        g_stSocpDebugInfo.sSocpDebugEncDst.u32SocpReadDoneEncDstSucCnt[u32ChanID]++;
    }
    else
    {
        SOCP_LOG_ERR("BSP_SOCP_ReadDataDone: invalid Channel type", u32DestChanID, u32ChanType);
        return BSP_ERR_SOCP_INVALID_CHAN;
    }

#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
    VOS_FlushCpuWriteBuf();
#endif    

    return BSP_OK;
}
#endif  /*SOCP_ACORE*/

/*****************************************************************************
* �� �� ��   : SOCP_Help
*
* ��������  : ��ȡsocp��ӡ��Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_Help(BSP_VOID)
{
    vos_printf("\r |*************************************|\n");
    vos_printf("\r SOCP_ShowDebugGbl   : �鿴ȫ��ͳ����Ϣ:ͨ�����롢���ú��ж��ܼ������޲���\n");
    vos_printf("\r SOCP_ShowEncSrcChanCur : �鿴����Դͨ�����ԣ�����Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowEncSrcChanAdd : �鿴����Դͨ������ͳ��ֵ������Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowEncSrcChanAll : �鿴���б���Դͨ�����Ժ�ͳ��ֵ���޲���\n");
#ifdef SOCP_ACORE
    vos_printf("\r SOCP_ShowEncDstChanCur : �鿴����Ŀ��ͨ�����ԣ�����Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowEncDstChanAdd : �鿴����Ŀ��ͨ������ͳ��ֵ������Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowEncDstChanAll : �鿴���б���Ŀ��ͨ�����Ժ�ͳ��ֵ���޲���\n");
    vos_printf("\r SOCP_ShowDecSrcChanCur : �鿴����Դͨ�����ԣ�����Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowDecSrcChanAdd : �鿴����Դͨ������ͳ��ֵ������Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowDecSrcChanAll : �鿴���н���Դͨ�����Ժ�ͳ��ֵ���޲���\n");
#endif
    vos_printf("\r SOCP_ShowDecDstChanCur : �鿴����Ŀ��ͨ�����ԣ�����Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowDecDstChanAdd : �鿴����Ŀ��ͨ������ͳ��ֵ������Ϊͨ��ID\n");
    vos_printf("\r SOCP_ShowDecDstChanAll : �鿴���н���Ŀ��ͨ�����Ժ�ͳ��ֵ���޲���\n");

    vos_printf("\r SOCP_debugCntShow : �鿴ȫ��ͳ����Ϣ���޲���\n");
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDebugGbl
*
* ��������  : ��ʾȫ��debug ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowDebugGbl(BSP_VOID)
{
    SOCP_DEBUG_GBL_S *sSocpDebugGblInfo;

    sSocpDebugGblInfo = &g_stSocpDebugInfo.sSocpDebugGBl;

    vos_printf("\r SOCPȫ��״̬ά����Ϣ:\n");
    vos_printf("\r socp����ַ:                                            : 0x%x\n",
           (BSP_S32)g_strSocpStat.u32BaseAddr);

#ifdef SOCP_ACORE
    vos_printf("\r socp����APP�жϵĴ���          : 0x%x\n", (BSP_S32)sSocpDebugGblInfo->u32SocpAppEtrIntCnt);
    vos_printf("\r socp���APP�жϵĴ���          : 0x%x\n", (BSP_S32)sSocpDebugGblInfo->u32SocpAppSucIntCnt);
#endif

#ifdef SOCP_CCORE
    vos_printf("\r socp����Modem�жϵĴ���        : 0x%x\n", (BSP_S32)sSocpDebugGblInfo->u32SocpModemEtrIntCnt);
    vos_printf("\r socp���Modem�жϵĴ���        : 0x%x\n", (BSP_S32)sSocpDebugGblInfo->u32SocpModemSucIntCnt);
#endif

    return;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncSrcChanCur
*
* ��������  : ��ӡ����Դͨ����ǰ����
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/

/*lint -e529*/
BSP_U32 SOCP_ShowEncSrcChanCur(BSP_U32 u32UniqueId)
{
    BSP_U32 u32RealId   = 0;
    BSP_U32 u32ChanType = 0;

    u32RealId   = SOCP_REAL_CHAN_ID(u32UniqueId);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_CODER_SRC_CHAN);

    vos_printf("================== ����ı���Դͨ�� 0x%x  ����:=================\n", u32UniqueId);
    vos_printf("ͨ������״̬:\t\t%d\n", g_strSocpStat.sEncSrcChan[u32RealId].u32SetStat);
    vos_printf("ͨ��ʹ��״̬:\t\t%d\n", g_strSocpStat.sEncSrcChan[u32RealId].u32ChanEn);
    vos_printf("ͨ�����ݸ�ʽ����:\t\t%d\n", g_strSocpStat.sEncSrcChan[u32RealId].eChnMode);
    vos_printf("ͨ��buffer ��ʼ��ַ:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sEncSrcBuf.u32Start);
    vos_printf("ͨ��buffer ������ַ:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sEncSrcBuf.u32End);
    vos_printf("ͨ��buffer ��ָ��:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sEncSrcBuf.u32Read);
    vos_printf("ͨ��buffer дָ��:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sEncSrcBuf.u32Write);
    vos_printf("ͨ��buffer ����:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sEncSrcBuf.u32Length);

    if (SOCP_ENCSRC_CHNMODE_LIST == g_strSocpStat.sEncSrcChan[u32RealId].eChnMode)
    {
        vos_printf("ͨ��RD buffer ��ʼ��ַ:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sRdBuf.u32Start);
        vos_printf("ͨ��RD buffer ������ַ:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sRdBuf.u32End);
        vos_printf("ͨ��RD buffer ��ָ��:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sRdBuf.u32Read);
        vos_printf("ͨ��RD buffer дָ��:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sRdBuf.u32Write);
        vos_printf("ͨ��RD buffer ����:\t\t0x%x\n", g_strSocpStat.sEncSrcChan[u32RealId].sRdBuf.u32Length);
    }

    return BSP_OK;
}

/*lint +e529*/

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncSrcChanAdd
*
* ��������  : ��ӡ����Դͨ���ۼ�ͳ��ֵ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowEncSrcChanAdd(BSP_U32 u32UniqueId)
{
    BSP_U32             u32ChanType;
    BSP_U32             u32RealChanID;
    SOCP_DEBUG_ENCSRC_S *sSocpAddDebugEncSrc;

    u32RealChanID   = SOCP_REAL_CHAN_ID(u32UniqueId);
    u32ChanType     = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_CODER_SRC_CHAN);

    sSocpAddDebugEncSrc = &g_stSocpDebugInfo.sSocpDebugEncSrc;


    vos_printf("================== ����Դͨ�� 0x%x  �ۼ�ͳ��ֵ:=================\n",
            u32UniqueId);
    vos_printf("\r socp��������Դͨ���ɹ��Ĵ���                           : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpStartEncSrcCnt[u32RealChanID]);
    vos_printf("\r socpֹͣ����Դͨ���ɹ��Ĵ���                           : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpStopEncSrcCnt[u32RealChanID]);
    vos_printf("\r socpע�����Դͨ���쳣�������Ĵ���               : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpRegEventEncSrcCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ի��дbuffer�Ĵ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpGetWBufEncSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����дbuffer�ɹ��Ĵ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpGetWBufEncSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ը���дbufferָ��Ĵ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpWriteDoneEncSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������дbufferָ��ɹ��Ĵ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpWriteDoneEncSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������дbufferָ��ʧ�ܵĴ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpWriteDoneEncSrcFailCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ի��RD buffer�Ĵ���                   : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpGetRdBufEncSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����RD buffer�ɹ��Ĵ���                   : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpGetRdBufEncSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ը���RDbufferָ��Ĵ���              : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpReadRdDoneEncSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������RDbufferָ��ɹ��Ĵ���              : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpReadRdDoneEncSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������RDbufferָ��ʧ�ܵĴ���              : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpReadRdDoneEncSrcFailCnt[u32RealChanID]);
    vos_printf("\r socp ISR �н������Դͨ����ͷ�����жϴ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpEncSrcIsrHeadIntCnt[u32RealChanID]);
    vos_printf("\r socp �����лص�����Դͨ����ͷ�����жϴ���������   : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpEncSrcTskHeadCbOriCnt[u32RealChanID]);
    vos_printf("\r socp �ص�����Դͨ����ͷ�����жϴ������ɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpEncSrcTskHeadCbCnt[u32RealChanID]);
    vos_printf("\r socp ISR �н������Դͨ��Rd ����жϴ���                     : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpEncSrcIsrRdIntCnt[u32RealChanID]);
    vos_printf("\r socp �ص�����Դͨ��Rd ����жϴ������ɹ��Ĵ���       : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncSrc->u32SocpEncSrcTskRdCbCnt[u32RealChanID]);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncSrcChanAll
*
* ��������  : ��ӡ���б���Դͨ����Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowEncSrcChanAll(BSP_VOID)
{
    BSP_U32 i;

    for (i=0; i<SOCP_MAX_ENCSRC_CHN; i++)
    {
        (BSP_VOID)SOCP_ShowEncSrcChanCur(i);
        (BSP_VOID)SOCP_ShowEncSrcChanAdd(i);
    }

    return;
}

#ifdef SOCP_ACORE
/*****************************************************************************
* �� �� ��   : SOCP_ShowEncDstSemInfo
*
* ��������  : ��ӡ����Ŀ��ͨ��Sem��Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowEncDstSemInfo(BSP_VOID)
{

    vos_printf("��ʱд�볬ʱ�ͷ�:\t\t%d\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTimerNum);
    vos_printf("��ʱд�볬ʱ�ͷ�ʱ��:\t\t0x%x\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTimerSlice);
    vos_printf("��ʱд��flush�ͷ�:\t\t%d\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulFlushNum);
    vos_printf("��ʱд��flush�ͷ�ʱ��:\t\t0x%x\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulFlushSlice);
    vos_printf("�ж��ͷ�:\t\t%d\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulIntNum);
    vos_printf("�ж��ͷ�ʱ��:\t\t0x%x\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulIntSlice);
    vos_printf("�����ȡʧ��:\t\t%d\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskErrNum);
    vos_printf("�����ȡʧ��ʱ��:\t\t0x%x\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskErrSlice);
    vos_printf("�����ȡ�ɹ�:\t\t%d\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskNum);
    vos_printf("�����ȡ�ɹ�ʱ��:\t\t0x%x\n", g_stSocpDebugInfo.stSocpEncDstSemInfo.ulTaskSlice);

    return;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncDstChanCur
*
* ��������  : ��ӡ����Ŀ��ͨ����Ϣ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowEncDstChanCur(BSP_U32 u32UniqueId)
{
    BSP_U32 u32RealId   = 0;
    BSP_U32 u32ChanType = 0;

    u32RealId   = SOCP_REAL_CHAN_ID(u32UniqueId);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_CODER_DEST_CHAN);

    vos_printf("================== ����Ŀ��ͨ�� 0x%x  ����:=================\n", u32UniqueId);
    vos_printf("ͨ��ID:\t\t%d\n", g_strSocpStat.sEncDstChan[u32RealId].u32ChanID);
    vos_printf("ͨ������״̬:\t\t%d\n", g_strSocpStat.sEncDstChan[u32RealId].u32SetStat);
    vos_printf("ͨ��buffer ��ʼ��ַ:\t\t0x%x\n", g_strSocpStat.sEncDstChan[u32RealId].sEncDstBuf.u32Start);
    vos_printf("ͨ��buffer ������ַ:\t\t0x%x\n", g_strSocpStat.sEncDstChan[u32RealId].sEncDstBuf.u32End);
    vos_printf("ͨ��buffer ��ָ��:\t\t0x%x\n", g_strSocpStat.sEncDstChan[u32RealId].sEncDstBuf.u32Read);
    vos_printf("ͨ��buffer дָ��:\t\t0x%x\n", g_strSocpStat.sEncDstChan[u32RealId].sEncDstBuf.u32Write);
    vos_printf("ͨ��buffer ����:\t\t0x%x\n", g_strSocpStat.sEncDstChan[u32RealId].sEncDstBuf.u32Length);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncDstChanAdd
*
* ��������  : ��ӡ����Ŀ��ͨ���ۼ�ͳ��ֵ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowEncDstChanAdd(BSP_U32 u32UniqueId)
{
    BSP_U32 u32RealChanID;
    BSP_U32 u32ChanType = 0;
    SOCP_DEBUG_ENCDST_S *sSocpAddDebugEncDst;

    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_CODER_DEST_CHAN);

    u32RealChanID = SOCP_REAL_CHAN_ID(u32UniqueId);

    sSocpAddDebugEncDst = &g_stSocpDebugInfo.sSocpDebugEncDst;

    vos_printf("================== ����Ŀ��ͨ�� 0x%x  �ۼ�ͳ��ֵ:=================\n",
            u32UniqueId);
    vos_printf("\r socpע�����Ŀ��ͨ���쳣�������Ĵ���           : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpRegEventEncDstCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ��ע������ݻص������ɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpRegReadCBEncDstCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����Ի�ö�buffer �Ĵ���                : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpGetReadBufEncDstEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ����ö�buffer�ɹ��Ĵ���                : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpGetReadBufEncDstSucCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����Ը��¶�bufferָ��Ĵ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpReadDoneEncDstEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ��ɹ��Ĵ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpReadDoneEncDstSucCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ��ʧ�ܵĴ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpReadDoneEncDstFailCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ���ƶ���0 �ֽڳɹ��Ĵ���    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpReadDoneValidEncDstCnt[u32RealChanID]);
    vos_printf("\r socpISR �н������Ŀ��ͨ����������жϴ���  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstIsrTrfIntCnt[u32RealChanID]);
    vos_printf("\r socp�����лص�����Ŀ��ͨ����������жϴ���������  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskTrfCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Ŀ��ͨ����������жϴ������ɹ��Ĵ���  : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskTrfCbCnt[u32RealChanID]);
    vos_printf("\r socp�����лص�����Ŀ��ͨ��buf ����жϴ���������    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskOvfCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Ŀ��ͨ��buf ����жϴ������ɹ��Ĵ���    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskOvfCbCnt[u32RealChanID]);
    vos_printf("\r socpISR �н������Ŀ��ͨ��buf��ֵ����жϴ���    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstIsrThresholdOvfIntCnt[u32RealChanID]);
    vos_printf("\r socp�����лص�����Ŀ��ͨ��buf��ֵ����жϴ���������    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskThresholdOvfCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Ŀ��ͨ��buf��ֵ����жϴ������ɹ��Ĵ���    : 0x%x\n",
           (BSP_S32)sSocpAddDebugEncDst->u32SocpEncDstTskThresholdOvfCbCnt[u32RealChanID]);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowEncDstChanAll
*
* ��������  : ��ӡ����Ŀ��ͨ����Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowEncDstChanAll(BSP_VOID)
{
    BSP_U32 i;
    BSP_U32 u32UniqueId = 0;

    for (i = 0; i < SOCP_MAX_ENCDST_CHN; i++)
    {
        u32UniqueId = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN, i);
        (BSP_VOID)SOCP_ShowEncDstChanCur(u32UniqueId);
        (BSP_VOID)SOCP_ShowEncDstChanAdd(u32UniqueId);
    }

    return;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecSrcChanCur
*
* ��������  : ��ӡ����Դͨ����Ϣ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowDecSrcChanCur(BSP_U32 u32UniqueId)
{
    BSP_U32 u32RealId   = 0;
    BSP_U32 u32ChanType = 0;

    u32RealId   = SOCP_REAL_CHAN_ID(u32UniqueId);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);

    vos_printf("================== ����Դͨ�� 0x%x  ����:=================\n", u32UniqueId);
    vos_printf("ͨ��ID:\t\t%d\n",               g_strSocpStat.sDecSrcChan[u32RealId].u32ChanID);
    vos_printf("ͨ������״̬:\t\t%d\n",         g_strSocpStat.sDecSrcChan[u32RealId].u32SetStat);
    vos_printf("ͨ��ʹ��״̬:\t\t%d\n",         g_strSocpStat.sDecSrcChan[u32RealId].u32ChanEn);
    vos_printf("ͨ��ģʽ:\t\t%d\n",             g_strSocpStat.sDecSrcChan[u32RealId].eChnMode);
    vos_printf("ͨ��buffer ��ʼ��ַ:\t\t0x%x\n",g_strSocpStat.sDecSrcChan[u32RealId].sDecSrcBuf.u32Start);
    vos_printf("ͨ��buffer ������ַ:\t\t0x%x\n",g_strSocpStat.sDecSrcChan[u32RealId].sDecSrcBuf.u32End);
    vos_printf("ͨ��buffer ��ָ��:\t\t0x%x\n",  g_strSocpStat.sDecSrcChan[u32RealId].sDecSrcBuf.u32Read);
    vos_printf("ͨ��buffer дָ��:\t\t0x%x\n",  g_strSocpStat.sDecSrcChan[u32RealId].sDecSrcBuf.u32Write);
    vos_printf("ͨ��buffer ����:\t\t0x%x\n",    g_strSocpStat.sDecSrcChan[u32RealId].sDecSrcBuf.u32Length);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecSrcChanAdd
*
* ��������  : ��ӡ����Դͨ���ۼ�ͳ��ֵ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowDecSrcChanAdd(BSP_U32 u32UniqueId)
{
    BSP_U32             u32RealChanID;
    SOCP_DEBUG_DECSRC_S *sSocpAddDebugDecSrc;
    BSP_U32             u32ChanType = 0;

    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_SRC_CHAN);

    u32RealChanID = SOCP_REAL_CHAN_ID(u32UniqueId);

    sSocpAddDebugDecSrc = &g_stSocpDebugInfo.sSocpDebugDecSrc;

    vos_printf("================== ����Դͨ�� 0x%x  �ۼ�ͳ��ֵ:=================\n",
            u32UniqueId);
    vos_printf("\r socp��������Դͨ���ɹ��Ĵ���                           : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpStartDecSrcCnt[u32RealChanID]);
    vos_printf("\r socpֹͣ����Դͨ���ɹ��Ĵ���                           : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpStopDecSrcCnt[u32RealChanID]);
    vos_printf("\r socpע�����Դͨ���쳣�������Ĵ���              : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpRegEventDecSrcCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ի��дbuffer�Ĵ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpGetWBufDecSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����дbuffer�ɹ��Ĵ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpGetWBufDecSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ�����Ը���дbufferָ��Ĵ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpWriteDoneDecSrcEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������дbufferָ��ɹ��Ĵ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpWriteDoneDecSrcSucCnt[u32RealChanID]);
    vos_printf("\r socp����Դͨ������дbufferָ��ʧ�ܵĴ���            : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpWriteDoneDecSrcFailCnt[u32RealChanID]);
    vos_printf("\r socpISR �н������Դͨ�������жϴ���                      : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpDecSrcIsrErrIntCnt[u32RealChanID]);
    vos_printf("\r socp�����лص�����Դͨ�������жϴ���������       : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpDecSrcTskErrCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Դͨ�������жϴ������ɹ��Ĵ���       : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecSrc->u32SocpDecSrcTskErrCbCnt[u32RealChanID]);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecSrcChanAll
*
* ��������  : ��ӡ����Դͨ����Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowDecSrcChanAll(BSP_VOID)
{
    BSP_U32 i;
    BSP_U32 u32UniqueId = 0;

    for (i = 0; i < SOCP_MAX_DECSRC_CHN; i++)
    {
        u32UniqueId = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN, i);
        (BSP_VOID)SOCP_ShowDecSrcChanCur(u32UniqueId);
        (BSP_VOID)SOCP_ShowDecSrcChanAdd(u32UniqueId);
    }

    return;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecDstChanCur
*
* ��������  : ��ӡ����Ŀ��ͨ����Ϣ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowDecDstChanCur(BSP_U32 u32UniqueId)
{
    BSP_U32 u32RealId   = 0;
    BSP_U32 u32ChanType = 0;

    u32RealId   = SOCP_REAL_CHAN_ID(u32UniqueId);
    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_DEST_CHAN);

    vos_printf("================== ����Ŀ��ͨ�� 0x%x  ����:=================\n", u32UniqueId);
    vos_printf("ͨ��ID:\t\t%d\n",               g_strSocpStat.sDecDstChan[u32RealId].u32ChanID);
    vos_printf("ͨ��ʹ��ģ����:\t\t%d\n",       g_strSocpStat.sDecDstChan[u32RealId].eDataType);
    vos_printf("ͨ��buffer ��ʼ��ַ:\t\t0x%x\n",g_strSocpStat.sDecDstChan[u32RealId].sDecDstBuf.u32Start);
    vos_printf("ͨ��buffer ������ַ:\t\t0x%x\n",g_strSocpStat.sDecDstChan[u32RealId].sDecDstBuf.u32End);
    vos_printf("ͨ��buffer ��ָ��:\t\t0x%x\n",  g_strSocpStat.sDecDstChan[u32RealId].sDecDstBuf.u32Read);
    vos_printf("ͨ��buffer дָ��:\t\t0x%x\n",  g_strSocpStat.sDecDstChan[u32RealId].sDecDstBuf.u32Write);
    vos_printf("ͨ��buffer ����:\t\t0x%x\n",    g_strSocpStat.sDecDstChan[u32RealId].sDecDstBuf.u32Length);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecDstChanAdd
*
* ��������  : ��ӡ����Ŀ��ͨ���ۼ�ͳ��ֵ
*
* �������  : ͨ��ID
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_U32 SOCP_ShowDecDstChanAdd(BSP_U32 u32UniqueId)
{
    BSP_U32             u32RealChanID;
    SOCP_DEBUG_DECDST_S *sSocpAddDebugDecDst;
    BSP_U32             u32ChanType = 0;

    u32ChanType = SOCP_REAL_CHAN_TYPE(u32UniqueId);

    SOCP_CHECK_CHAN_TYPE(u32ChanType, SOCP_DECODER_DEST_CHAN);

    u32RealChanID = SOCP_REAL_CHAN_ID(u32UniqueId);

    sSocpAddDebugDecDst = &g_stSocpDebugInfo.sSocpDebugDecDst;

    vos_printf("================== ����Ŀ��ͨ�� 0x%x  �ۼ�ͳ��ֵ:=================\n",
            u32UniqueId);
    vos_printf("\r socpע�����Ŀ��ͨ���쳣�������Ĵ���           : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpRegEventDecDstCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ��ע������ݻص������ɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpRegReadCBDecDstCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����Ի�ö�buffer�Ĵ���                : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpGetReadBufDecDstEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ����ö�buffer�ɹ��Ĵ���                : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpGetReadBufDecDstSucCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����Ը��¶�bufferָ��Ĵ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpReadDoneDecDstEtrCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ��ɹ��Ĵ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpReadDoneDecDstSucCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ��ʧ�ܵĴ���          : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpReadDoneDecDstFailCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ���ƶ�0 �ֽڳɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpReadDoneZeroDecDstCnt[u32RealChanID]);
    vos_printf("\r socp����Ŀ��ͨ�����¶�bufferָ���ƶ���0 �ֽڳɹ��Ĵ���: 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpReadDoneValidDecDstCnt[u32RealChanID]);
    vos_printf("\r socpISR �н������Ŀ��ͨ����������жϴ���                : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstIsrTrfIntCnt[u32RealChanID]);
    vos_printf("\r socp������ �ص�����Ŀ��ͨ����������жϴ������Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstTskTrfCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Ŀ��ͨ����������жϴ������ɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstTskTrfCbCnt[u32RealChanID]);
    vos_printf("\r socpISR �н������Ŀ��ͨ��buf ����жϴ���                  : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstIsrOvfIntCnt[u32RealChanID]);
    vos_printf("\r socp������ �ص�����Ŀ��ͨ��buf ����жϴ���������  : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstTskOvfCbOriCnt[u32RealChanID]);
    vos_printf("\r socp�ص�����Ŀ��ͨ��buf ����жϴ������ɹ��Ĵ���   : 0x%x\n",
           (BSP_S32)sSocpAddDebugDecDst->u32SocpDecDstTskOvfCbCnt[u32RealChanID]);

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecDstChanAll
*
* ��������  : ��ӡ����Ŀ��ͨ����Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_ShowDecDstChanAll(BSP_VOID)
{
    BSP_U32 i;
    BSP_U32 u32UniqueId = 0;

    for (i = 0; i < SOCP_MAX_DECDST_CHN; i++)
    {
        u32UniqueId = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN, i);
        (BSP_VOID)SOCP_ShowDecDstChanCur(u32UniqueId);
        (BSP_VOID)SOCP_ShowDecDstChanAdd(u32UniqueId);
    }

    return;
}

/*****************************************************************************
* �� �� ��   : SOCP_ShowDecDstChanAll
*
* ��������  : ��ӡ����Ŀ��ͨ����Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
VOS_VOID SOCP_ShowSDLogInfo(VOS_VOID)
{
    SOCP_ENC_DST_BUF_LOG_CFG_STRU       *pstcfg;

    pstcfg = SocpGetSDLogCfg();

    vos_printf("\r\n The SD Log Flag is         %d",    pstcfg->logOnFlag);
    vos_printf("\r\n The GU Water is            %d KB", pstcfg->guWaterMark);
    vos_printf("\r\n The Lte Water is           %d KB", pstcfg->lWaterMark);
    vos_printf("\r\n The Over Time is           %d ms", pstcfg->overTime);
    vos_printf("\r\n The Flush Flag is          %d",    pstcfg->flushFlag);
    vos_printf("\r\n EncDst Start Time num is   %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulStartTimeNum);
    vos_printf("\r\n EncDst Time Out num is     %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulTimeOutNum);
    vos_printf("\r\n EncDst Time Out Run Num is %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulEncDstRunNum);
    vos_printf("\r\n EncDst Start Fail num is   %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulStartTimeFailNum);
    vos_printf("\r\n EncDst Flush num is        %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulFlushBufferNum);
    vos_printf("\r\n EncDst Flush Time Out is   %d",    g_stSocpDebugInfo.stSocpSDLogInfo.ulFlushTimeOutNum);
    vos_printf("\r\n");

    return;
}

#endif

/*****************************************************************************
* �� �� ��   : SOCP_debugCntShow
*
* ��������  : ��ʾdebug ������Ϣ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ   : ��
*****************************************************************************/
BSP_VOID SOCP_debugCntShow(BSP_VOID)
{
    SOCP_ShowDebugGbl();
    SOCP_ShowEncSrcChanAll();
#ifdef SOCP_ACORE
    SOCP_ShowEncDstChanAll();
    SOCP_ShowDecSrcChanAll();
    SOCP_ShowDecDstChanAll();
#endif

    return;
}

/*****************************************************************************
 �� �� ��  : SOCP_Stop1SrcChan
 ��������  : disable һ��SOCP����Դͨ��
 �������  : ulSrcChanID ͨ��ID
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
 �޸���ʷ  :
   1.��    ��  : 2013��5��2��
     ��    ��  : j0174725s
     �޸�����  : Creat Function
*****************************************************************************/
VOS_VOID SOCP_Stop1SrcChan(VOS_UINT32 ulSrcChanID)
{
    VOS_UINT32                          ulRealChanID;

    /* ��ȡ��ʵͨ��ID */

    ulRealChanID = SOCP_REAL_CHAN_ID(ulSrcChanID);

    /* ����ͨ���ر� */
    SOCP_REG_SETBITS(SOCP_REG_ENCSRC_BUFCFG1(ulRealChanID), 0, 1, 0);

    return ;
}


VOS_VOID SOCP_LogToFile(FILE *fp)
{
    DRV_FILE_WRITE((VOS_VOID*)&g_strSocpStat, sizeof(VOS_CHAR), sizeof(g_strSocpStat), fp);
    DRV_FILE_WRITE((VOS_VOID*)&g_stSocpDebugInfo, sizeof(VOS_CHAR), sizeof(g_stSocpDebugInfo), fp);
}


VOS_UINT8* SOCP_GetDebugLogInfo(VOS_UINT32  ulType)
{
    /* 1��ʾ socp statue */
    if (1 == ulType)
    {
        return (VOS_UINT8*)&g_strSocpStat;
    }
    /* �����ʾDebug Info */
    return (VOS_UINT8*)&g_stSocpDebugInfo;
}


VOS_UINT32 SOCP_GetDebugLogInfoLen(VOS_UINT32  ulType)
{
    /* 1��ʾ socp statue */
    if (1 == ulType)
    {
        return (VOS_UINT32)sizeof(g_strSocpStat);
    }

    /* �����ʾDebug Info */
    return (VOS_UINT32)sizeof(g_stSocpDebugInfo);
}
/*lint +e662 +e661*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

