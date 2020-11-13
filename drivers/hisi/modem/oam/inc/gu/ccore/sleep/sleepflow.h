/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : SleepFlow.h
  �� �� ��   : ����
  ��    ��   : zuowen
  ��������   : 2005��3��7��
  ����޸�   :
  ��������   : SleepFlow.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :

  1.��    ��   : 2005��3��7��
    ��    ��   : zuowen
    �޸�����   : �����ļ�

  2.��    ��   : 2006��4��13��
    ��    ��   : w45442
    �޸�����   : �޸����ⵥA32D03034

******************************************************************************/
#ifndef __SLEEPFLOW_H__
#define __SLEEPFLOW_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "DrvInterface.h"
#include "omrl.h"
#include "sleepsleep.h"
#include "phyoaminterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define BBP_AWAKE_BIT                           0x01

#define WSLEEP_32K_TO_64M_TYPE_BIT              0       /*DRX���Ѻ�ʱ���л���61.44M�ж�*/
#define WSLEEP_64M_TO_32K_TYPE_BIT              1       /*sleep*/

#define BBP_DRX_INT_SLICE_COUNT                 800       /*BBP DRX INT SLICE DELAY COUNT*/

#define SLEEP_SLOT_LEN                          15      /*Wÿ֡����15��SLOT*/
#define SLEEP_SFN_CYCLE_LEN                     (4096*SLEEP_SLOT_LEN) /*SFN��ΧΪ12bit��ÿ���ֻ�4096*/

#define SLEEP_WAKE_DELAY                        2       /*�����ӳٷ�ֹ����ִ�������������Ѿ�����*/
#define SLEEP_WAKE_INTERVAL                     30      /*ǿ�ƻ���ʱ������������ʱ�̼������Сֵ*/
#define SLEEP_WAKE_BASE_FRM_PROTECT_CHIP        (38200) /* �ھ��Լ�ʱ���Ⱥ��ж�ʱ�ܿ�֡ͷǰ��CHIP */
#define SLEEP_WAKE_BASE_FRM_PROTECT_VALUE       (500)
#define SLEEP_WAKE_BBP_POS_PROTECT_SLICE        (330)   /* BBP�������̳�ʱ����,10ms */

/* GU��֧��ģʽ��BUTTֵ */
#define SLEEP_GU_MODE_TYPE_BUTT                 (2)

#define BBP_SLEEP_32K_CLOCK_SWITCH_IND          (4)
#define BBP_SLEEP_WAKE_IND                      (5)
#define BBP_SLEEP_NORMAL_CLOCK_SWITCH_IND       (6)
#define SLEEP_TL_WAKE_GU_IND                    (7)
#define SLEEP_ACTIVATE_ERROR_ID                 (8)

#define BBP_ERROR_VALUE                         (0xDEADBEEF)

#define SLEEP_TRACK_RECORD_NUM                  (100)

#define SLEEP_RECORD_4MS                        (128)
#define SLEEP_RECORD_3MS                        (96)
#define SLEEP_RECORD_2MS                        (64)
#define SLEEP_RECORD_1MS                        (32)
#define SLEEP_RECORD_MAX_EVENT_COUNT            (5)

#define SLEEP_BBP_ERROR_TIMEOUT                 (30000)

#define SLEEP_INFO_MAX_FILE                     (2)
#define SLEEP_INFO_MAX_NAME_LEN                 (40)
#define SLEEP_INFO_MAX_SIZE                     (1024)
#define SLEEP_INFO_MAX_LEN                      (16)
#define SLEEP_INFO_RESET_ENABLE                 (0x5A5A5A5A)

#define SLEEP_DEVICE_POWUP(enModemId, enMode, enCommMode) \
    { \
        DRV_PWRCTRL_PWRUP((PWC_COMM_MODE_E)enMode, enCommMode, (PWC_COMM_MODEM_E)enModemId); \
        if ( VOS_NULL_PTR != g_pstSleepDeviceState ) \
        { \
            g_pstSleepDeviceState->aulDevicePwState[enModemId][enMode][enCommMode]  = PWRCTRL_COMM_ON; \
            g_pstSleepDeviceState->aulDeviceUpSlice[enModemId][enMode][enCommMode]  = OM_GetSlice(); \
        } \
        \
    } \


#define SLEEP_DEVICE_POWDOWN(enModemId, enMode, enCommMode) \
    { \
        DRV_PWRCTRL_PWRDOWN((PWC_COMM_MODE_E)enMode, enCommMode, (PWC_COMM_MODEM_E)enModemId); \
        if ( VOS_NULL_PTR != g_pstSleepDeviceState ) \
        { \
            g_pstSleepDeviceState->aulDevicePwState[enModemId][enMode][enCommMode]      = PWRCTRL_COMM_OFF; \
            g_pstSleepDeviceState->aulDeviceDownSlice[enModemId][enMode][enCommMode]    = OM_GetSlice(); \
        } \
        \
    } \

#define SLEEP_DEVICEPLL_POWUP(enModemId, enMode, enCommMode) \
    { \
        DRV_PWRCTRL_PLLENABLE((PWC_COMM_MODE_E)enMode, enCommMode, (PWC_COMM_MODEM_E)enModemId); \
        if ( VOS_NULL_PTR != g_pstSleepDeviceState ) \
        { \
            g_pstSleepDeviceState->aulPllPwState[enModemId][enMode][enCommMode]  = PWRCTRL_COMM_ON; \
            g_pstSleepDeviceState->aulPLLUpSlice[enModemId][enMode][enCommMode]  = OM_GetSlice(); \
        } \
        \
    } \


#define SLEEP_DEVICEPLL_POWDOWN(enModemId, enMode, enCommMode) \
    { \
        DRV_PWRCTRL_PLLDISABLE((PWC_COMM_MODE_E)enMode, enCommMode, (PWC_COMM_MODEM_E)enModemId); \
        if ( VOS_NULL_PTR != g_pstSleepDeviceState ) \
        { \
            g_pstSleepDeviceState->aulPllPwState[enModemId][enMode][enCommMode]     = PWRCTRL_COMM_OFF; \
            g_pstSleepDeviceState->aulPLLDownSlice[enModemId][enMode][enCommMode]   = OM_GetSlice(); \
        } \
        \
    } \

/* ͨ������ */
#define SLEEP_ABB_CHAN(enMode)          (g_stSleepChanPara.astModeBasicParam[enMode].ucABBSwitch)
#define SLEEP_RF_CHAN(enMode)           (g_stSleepChanPara.astModeBasicParam[enMode].ucRFSwitch)
#define SLEEP_TCXO_CHAN(enMode)         (g_stSleepChanPara.astModeBasicParam[enMode].ucTCXOSwitch)

#define SLEEP_DEBUG

#ifdef SLEEP_DEBUG

#define SLEEP_ISR_RECORD(ulMsgId, enMode) \
    if (VOS_NULL_PTR != g_pstSleepTrackRecord) \
    { \
        g_pstSleepTrackRecord->astIsrRecord[g_pstSleepTrackRecord->ulIsrIndex].usMsgId    = (VOS_UINT16)ulMsgId; \
        g_pstSleepTrackRecord->astIsrRecord[g_pstSleepTrackRecord->ulIsrIndex].usModeType = (VOS_UINT16)enMode; \
        g_pstSleepTrackRecord->astIsrRecord[g_pstSleepTrackRecord->ulIsrIndex].ulSlice    = OM_GetSlice(); \
        \
        g_pstSleepTrackRecord->ulIsrIndex++; \
        /* �ﵽĩβ��Ҫ��ת*/ \
        if (SLEEP_TRACK_RECORD_NUM == g_pstSleepTrackRecord->ulIsrIndex) \
        { \
            g_pstSleepTrackRecord->ulIsrIndex = 0; \
        } \
    } \

#define SLEEP_MSG_RECORD(ulMsgId, enMode, StartSlice) \
    if (VOS_NULL_PTR != g_pstSleepTrackRecord) \
    { \
        g_pstSleepTrackRecord->astMsgRecord[g_pstSleepTrackRecord->ulMsgIndex].usMsgId      = (VOS_UINT16)ulMsgId; \
        g_pstSleepTrackRecord->astMsgRecord[g_pstSleepTrackRecord->ulMsgIndex].usModeType   = (VOS_UINT16)enMode; \
        g_pstSleepTrackRecord->astMsgRecord[g_pstSleepTrackRecord->ulMsgIndex].ulStartSlice = StartSlice; \
        g_pstSleepTrackRecord->astMsgRecord[g_pstSleepTrackRecord->ulMsgIndex].ulEndSlice   = OM_GetSlice(); \
        \
        g_pstSleepTrackRecord->ulMsgIndex++; \
        /* �ﵽĩβ��Ҫ��ת*/ \
        if (SLEEP_TRACK_RECORD_NUM == g_pstSleepTrackRecord->ulMsgIndex) \
        { \
            g_pstSleepTrackRecord->ulMsgIndex = 0; \
        } \
    } \

#define SLEEP_NOR_STATE_TRACK(enMode, enType) \
    if (VOS_NULL_PTR != g_pstSleepNormalState) \
    { \
        g_pstSleepNormalState->astRecordItem[enMode][enType].ulCount++; \
        g_pstSleepNormalState->astRecordItem[enMode][enType].ulSlice = OM_GetSlice(); \
    } \

#else

#define SLEEP_ISR_RECORD(ulMsgId, enMode)
#define SLEEP_MSG_RECORD(ulMsgId, enMode, ulStartSlice) (ulStartSlice = ulStartSlice) /* for pclint
 */
#define SLEEP_NOR_STATE_TRACK(enMode, enType)

#endif

#define HPA_UART_PRINT(data)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ö����    : SLEEP_SYSTEM_STATE_ENUM
 ö��˵��  : ϵͳ״̬

  1.��    ��   : 2012��07��31��
    ��    ��   : g47350
    �޸�����   : V900R001 MSP��ֺͿ�ά�ɲ���Ŀ����
*****************************************************************************/
enum SLEEP_SYSTEM_STATE_ENUM
{
    SLEEP_NORMAL_STATE = 0,                             /* ����״̬ */
    SLEEP_MASTER_TEMP_LP_STATE,                         /* ��ģ�͹���״̬(��̬) */
    SLEEP_MASTER_LP_STATE,                              /* ��ģ�͹���״̬ */
    SLEEP_TEMP_NORMAL_STATE,                            /* ����״̬(��̬) */
    SLEEP_SLAVE_LP_STATE,                               /* ��ģ�͹���״̬ */
    SLEEP_STATE_BUTT
};
typedef VOS_UINT32 SLEEP_SYSTEM_STATE_ENUM_UINT32;

/*****************************************************************************
 ö����    : SLEEP_NORMAL_STATE_TRACK_ENUM
 ö��˵��  : ����״̬�¹켣����

  1.��    ��   : 2012��07��31��
    ��    ��   : g47350
    �޸�����   : V900R001 MSP��ֺͿ�ά�ɲ���Ŀ����
*****************************************************************************/
enum SLEEP_NORMAL_STATE_TRACK_ENUM
{
    SLEEP_MASTER_BBP_32K = 0,
    SLEEP_MASTER_PHY_HALT,
    SLEEP_MASTER_LP,
    SLEEP_MASTER_BBP_WAKE,
    SLEEP_MASTER_ACTIVATE_START,
    SLEEP_MASTER_ACTIVATE_END,
    SLEEP_MASTER_BBP_RESUME_START,
    SLEEP_MASTER_BBP_RESUME_END,
    SLEEP_MASTER_FORCE_WAKE,
    SLEEP_MASTER_BBP_NORMAL,
    SLEEP_MASTER_WAKE_PHY_PRE,
    SLEEP_MASTER_WAKE_PHY,
    SLEEP_SLAVE_PHY_HALT,
    SLEEP_SLAVE_LP,
    SLEEP_SLAVE_FORCE_WAKE,
    SLEEP_NORMAL_BUTT
};

/*****************************************************************************
 ö����    : SLEEP_CHANNEL_ENUM
 ö��˵��  : ͨ��ö��ֵ

  1.��    ��   : 2013��09��27��
    ��    ��   : g47350
    �޸�����   :
*****************************************************************************/
enum SLEEP_CHANNEL_ENUM
{
    SLEEP_CHANNEL_ZERO = 0,
    SLEEP_CHANNEL_ONE,
    SLEEP_CHANNEL_BOTH,
};

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : SLEEP_MODE_INFO_STRU
 �ṹ˵��  : GUģ��ǰϵͳ״̬
*****************************************************************************/
typedef struct
{
    SLEEP_SYSTEM_STATE_ENUM_UINT32      enSystemState;
    UPHY_OAM_WAKE_TYPE_ENUM_UINT32      enWakeType;
}SLEEP_MODE_INFO_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_MSG_FUN_STRU
 �ṹ˵��  : ָʾ����Ϣ�Ĵ�����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulMsgId;                /* ��ϢID */
    Msg_Fun_Type                        pMsgFun;                /* ��Ϣ������ */
}SLEEP_MSG_FUN_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_INFO_STATE_STRU
 �ṹ˵��  : GUģ��λ��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                      ulAwakeIsrSlice;
    VOS_UINT32                      ulAwakeStartSlice;
    VOS_UINT32                      ulAwakeEndSlice;
    VOS_UINT32                      ulSwitchIsrSlice;
    VOS_UINT32                      ulSwitchTaskSlice;
    VOS_UINT32                      ulWarnEventFlag;
    VOS_UINT32                      ulWarnEventCount;
    VOS_UINT32                      ulSwitchDelayType1Count;
    VOS_UINT32                      ulSwitchDelayType2Count;
    VOS_UINT32                      ulSwitchDelayType3Count;
    VOS_UINT32                      ulSwitchDelayType4Count;
    VOS_UINT32                      ulSwitchDelayType5Count;
    VOS_UINT32                      ulEndFlag;
}SLEEP_INFO_STATE_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_ISR_MSG_STRU
 �ṹ˵��  : �ж���Ϣ��
*****************************************************************************/
typedef struct
{
   VOS_MSG_HEADER
   VOS_UINT16                           usMsgId;                /* ��ϢID */
   VOS_UINT16                           usReserved;             /* ���� */
   SLEEP_MODE_ENUM_UINT32               enRatMode;              /* �ϱ�ģʽ */
   VOS_UINT32                           ulSlice;                /* �жϽ���ʱ�� */
}SLEEP_ISR_MSG_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_RECORD_ITEM_STRU
 �ṹ˵��  : ���̼�¼��Ԫ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usMsgId;                /* ��ϢID */
    VOS_UINT16                          usModeType;             /* ģʽ���� */
    VOS_UINT32                          ulSlice;                /* Sliceֵ */
}SLEEP_RECORD_ITEM_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_RECORD_ITEM_EX_STRU
 �ṹ˵��  : ���̼�¼��չ��Ԫ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usMsgId;                /* ��ϢID */
    VOS_UINT16                          usModeType;             /* ģʽ���� */
    VOS_UINT32                          ulStartSlice;           /* ��ʼSliceֵ */
    VOS_UINT32                          ulEndSlice;             /* ����Sliceֵ */
}SLEEP_RECORD_ITEM_EX_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_TRACK_RECORD_STRU
 �ṹ˵��  : ����SLEEP�жϺ���Ϣ���̼�¼
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulIsrIndex;             /* �жϼ�¼���� */
    VOS_UINT32                          ulMsgIndex;             /* ��Ϣ��¼���� */
    SLEEP_RECORD_ITEM_STRU              astIsrRecord[SLEEP_TRACK_RECORD_NUM]; /* �ж����̼�¼ */
    SLEEP_RECORD_ITEM_EX_STRU           astMsgRecord[SLEEP_TRACK_RECORD_NUM]; /* ��Ϣ���̼�¼ */
}SLEEP_TRACK_RECORD_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_NORMAL_STATE_RECORD_STRU
 �ṹ˵��  : ����SLEEP����״̬�л�����
*****************************************************************************/
typedef struct
{
    SLEEP_RECORD_STRU   astRecordItem[SLEEP_GU_MODE_TYPE_BUTT][SLEEP_NORMAL_BUTT];
}SLEEP_NORMAL_STATE_RECORD_STRU;

/*****************************************************************************
 �ṹ��    : SLEEP_DEVICE_STATE
 �ṹ˵��  : GUģ��ǰ����״̬
*****************************************************************************/
typedef struct
{
    VOS_UINT32                      ulDeviceFlag;
    VOS_UINT32                      aulDevicePwState[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    VOS_UINT32                      aulDeviceUpSlice[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    VOS_UINT32                      aulDeviceDownSlice[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    VOS_UINT32                      aulPllPwState[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    VOS_UINT32                      aulPLLUpSlice[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    VOS_UINT32                      aulPLLDownSlice[SLEEP_CHANNEL_BOTH][SLEEP_GU_MODE_TYPE_BUTT][PWC_COMM_MODULE_BUTT];
    SLEEP_RECORD_STRU               astDevicePowerError[SLEEP_GU_MODE_TYPE_BUTT];
    SLEEP_RECORD_STRU               stDeviceTcxoError;
    VOS_UINT32                      aulABBState[64];
    VOS_UINT32                      ulBBPResumeTimeout;
}SLEEP_DEVICE_STATE_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
extern SLEEP_NORMAL_STATE_RECORD_STRU         *g_pstSleepNormalState;

extern SLEEP_TRACK_RECORD_STRU                *g_pstSleepTrackRecord;

/*****************************************************************************
  10 ��������
*****************************************************************************/

VOS_VOID SLEEP_EnableDspPll(VOS_VOID);
VOS_VOID SLEEP_ExitLowPower(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_BOOL SLEEP_IsSleep(VOS_VOID);
VOS_VOID Sleep_ActiveDspPll(VOS_VOID);
VOS_VOID SLEEP_NotifySndMsg(VOS_VOID);
VOS_VOID GUSLEEP_MasterAwakeSlave(VOS_VOID);
VOS_VOID SLEEP_32KIsr(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_NormalClockIsr(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_AwakeIsr(SLEEP_MODE_ENUM_UINT32 enMode);

VOS_VOID SLEEP_VoteLock(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_VoteUnlock(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_TimeCal(VOS_VOID);
VOS_VOID SLEEP_ResumeReg(VOS_VOID);
VOS_VOID SLEEP_PowerUp(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_DeactivateHw(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_ActivateHw(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID SLEEP_InfoInd( SLEEP_MODE_ENUM_UINT32 enMode );
VOS_UINT32  SLEEP_ForceWakeProtected(VOS_VOID);
VOS_UINT32  SLEEP_WakePhy(VOS_UINT32 ulMsgId, SLEEP_MODE_ENUM_UINT32 enMode, UPHY_OAM_WAKE_TYPE_ENUM_UINT32 enWakeType);
VOS_VOID    SLEEP_IsrProc(VOS_UINT32 ulMsgId, SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID    SLEEP_MasterSleepMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_32KClkMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_WakeMasterMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_NormalClkMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_SlaveSleepMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_TLWakeGUProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_MasterForceWake(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID    SLEEP_SlaveForceWake(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID    SLEEP_HookMsgProc(MsgBlock *pstMsg);
VOS_VOID    SLEEP_MsgProc(MsgBlock* pstMsg);
VOS_VOID    SLEEP_Init(VOS_VOID);
VOS_UINT32  WuepsSleepPidInit(enum VOS_INIT_PHASE_DEFINE ip);
VOS_UINT32  WuepsSleepFidInit(enum VOS_INIT_PHASE_DEFINE ip);
VOS_UINT32  SLEEP_ReadBaseCntChip( VOS_VOID );

VOS_VOID    I1_SLEEP_VoteLock(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_VOID    I1_SLEEP_PowerUp(SLEEP_MODE_ENUM_UINT32 enMode);
VOS_BOOL    SLEEP_IsAwakeProc(SLEEP_MODE_ENUM_UINT32 enMode);

VOS_UINT32  SLEEP_InfoFileName( VOS_CHAR * cFileName );
VOS_UINT32  SLEEP_InfoWriteLog( VOS_UINT32 *pstInfo, VOS_UINT32 uLen );
VOS_VOID    SLEEP_ActivateHWMsgProc(MsgBlock *pstMsg);
VOS_UINT32  SLEEP_GetLogPath(VOS_CHAR *pucBuf, VOS_CHAR *pucOldPath, VOS_CHAR *pucUnitaryPath);

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SLEEPFLOW_H__ */
