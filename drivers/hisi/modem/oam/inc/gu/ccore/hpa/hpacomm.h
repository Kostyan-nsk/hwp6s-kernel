/*****************************************************************************/
/*                                                                           */
/*                Copyright 1999 - 2003, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileName: HPAComm.h                                                     */
/*                                                                           */
/* Author: Xu cheng                                                          */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date: 2008-02                                                             */
/*                                                                           */
/* Description: implement HPA subroutine                                     */
/*                                                                           */
/* Others:                                                                   */
/*                                                                           */
/* History:                                                                  */
/* 1. Date: 200802-02                                                        */
/*    Author: Xu cheng                                                       */
/*    Modification: Create this file                                         */
/*                                                                           */
/*****************************************************************************/
#ifndef _HPA_COMM_H__
#define _HPA_COMM_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#include "vos.h"
#include "pslog.h"
#include "hpamailboxdef.h"
#include "hpaglobalvar.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*****************************************************************************
  2 STRUCT����
*****************************************************************************/
typedef struct
{
    VOS_UINT32          ulAHBBaseAddr;
    VOS_UINT32          ulWBBPBaseAddr;
    VOS_UINT32          ulWBBPDRXBaseAddr;
    VOS_UINT32          ulGBBPBaseAddr;
    VOS_UINT32          ulGBBPDRXBaseAddr;
    VOS_UINT32          ulSYSCTRLBaseAddr;
    VOS_UINT32          ulHIFIAXIBaseAddr;
    VOS_UINT32          ulDSPTempBaseAddr;
    VOS_UINT32          ulGBBP1BaseAddr;
    VOS_UINT32          ulGBBP1DRXBaseAddr;
}PLATFORM_SYSADDR_STRU;

/*****************************************************************************
  3 ȫ�ֱ�������
*****************************************************************************/
extern PLATFORM_SYSADDR_STRU           g_stSysAddr;

extern VOS_UINT32                      g_ulOmNosigEnable;

/*****************************************************************************
  4 �궨��
*****************************************************************************/

#define HPA_MATCH_MSGID_RECEIVER_PID_ERR    0xff

/*#define GHPA_MID_L1                         0
 */

/* AHB �����ַƫ�� */
#define AHB_BASE_ADDR                   (g_stSysAddr.ulAHBBaseAddr)

/* SYSCTRL ��ַ */
#define SOC_SYS_CTRL_BASE_ADDR          (g_stSysAddr.ulSYSCTRLBaseAddr)

/* HIFI��ARM����������ַ */
#define HIFI_AXI_BASE_ADDR              (g_stSysAddr.ulHIFIAXIBaseAddr)

/* д��DSP�¶Ȳ����ĵ�ַ */
#define DSP_TEMP_BASE_ADDR              (g_stSysAddr.ulDSPTempBaseAddr)

/* д�뵱ǰHIFI DSP״̬��ַ�����¶Ȳ��������л��� */
#define HIFI_STATUS_BASE_ADDR           (DSP_TEMP_BASE_ADDR+sizeof(CPHY_TEMP_INFO_STRU))

/*****************************************************************************
  5 ��������
*****************************************************************************/

extern VOS_UINT32 HPA_AhbMailBoxInit(HPA_MB_TYPE_ENUM_UINT32 enMbType);

extern VOS_VOID HPA_HifiTransferEntry(VOS_VOID);

extern VOS_VOID HPA_ApmMsgPIDProc( MsgBlock *pRcvMsg );

extern VOS_VOID HPA_PpMsgPIDProc( MsgBlock *pRcvMsg );

extern VOS_VOID HPA_NppMsgPIDProc( MsgBlock *pRcvMsg );

extern VOS_VOID HPA_R99MsgPIDProc(MsgBlock *pMsgBlock);

extern VOS_VOID HPA_RBufDeliverEntry(HPA_MB_TYPE_ENUM_UINT32 enMbType);

extern VOS_VOID HPA_Conncet2GISR0(VOS_VOID);

extern VOS_VOID HPA_Conncet2GISR1(VOS_VOID);

extern VOS_VOID HPA_ConncetAPMISR(VOS_VOID);

extern VOS_VOID HPA_Conncet3GISR(VOS_VOID);

extern VOS_VOID HPA_ReadCfnSfn(VOS_VOID);

extern VOS_VOID HPA_PutAhbToRBuf(HPA_MB_TYPE_ENUM_UINT32 enMbType);

extern VOS_VOID HPA_PutRBufToAhb(HPA_MB_TYPE_ENUM_UINT32 enMbType,
                                            VOS_UINT32 ulTriggerType);

extern VOS_VOID HPA_3GIsrEnable(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_3GIsrDisable(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_2GIsrEnable(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_2GIsrDisable(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_3G0MsIsr(VOS_VOID);

extern VOS_VOID HPA_3GAwakeIsr(VOS_VOID);

extern VOS_BOOL HPA_DLBufferIsEmpty(HPA_MB_TYPE_ENUM_UINT32 enMbType);

extern void atomic_inc(hpa_atomic_t *v);

extern void atomic_dec( hpa_atomic_t *v );

VOS_UINT32 APM_InitSystemAddr(VOS_VOID);

extern VOS_VOID HPA_2GAwakeIsr(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_2GFrameClockIsr(MODEM_ID_ENUM_UINT16 enModemID);

extern VOS_VOID HPA_2GFrameClockIsr0(VOS_VOID);

extern VOS_VOID HPA_2GFrameClockIsr1(VOS_VOID);

extern VOS_VOID HPA_2GAwakeIsr0(VOS_VOID);

extern VOS_VOID HPA_2GAwakeIsr1(VOS_VOID);

extern VOS_VOID HPA_HifiIpcIsr(VOS_VOID);

extern VOS_VOID HPA_3GSwitchClockIsr(VOS_VOID);

extern VOS_VOID HPA_ShowINTNo(VOS_VOID);

extern VOS_UINT32 HPA_Init(VOS_VOID);

extern VOS_VOID HPA_TransferTaskEntry(VOS_VOID);

extern VOS_VOID HPA_MsgToAhbProc(HPA_MB_TYPE_ENUM_UINT32 enMbType,
                                MsgBlock *pMsgBlock, VOS_UINT32 ulPriority);

extern VOS_UINT32 HPA_SendDspMsg(VOS_PID Pid, VOS_VOID **ppMsg,
            VOS_UINT32 ulFileID, VOS_INT32 lLineNo);

extern VOS_UINT32 HPA_SendDspUrgentMsg(VOS_PID Pid, VOS_VOID **ppMsg,
            VOS_UINT32 ulFileID, VOS_INT32 lLineNo);

extern VOS_VOID APM_InitAPMGlobal(VOS_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

