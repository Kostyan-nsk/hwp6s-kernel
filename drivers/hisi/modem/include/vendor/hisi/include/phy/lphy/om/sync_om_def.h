

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/

/************************************************************
                               �궨��
************************************************************/

/************************************************************
                             ���ݽṹ����
************************************************************/

/************************************************************
                             �ӿں�������
 ************************************************************/

#ifndef __SYNC_OM_DEF_H__
#define __SYNC_OM_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/************************************************************
                           1. REQ����Ͷ�Ӧ�����ݽṹ����
************************************************************/
typedef enum __LPHY_REQ_MSGID_SYNC__
{
    LPHY_REQ_SYNC_RESERVED            = OM_CMD_ID(LPHY_SYNC_MID, OM_TYPE_REQ, 0x0),
    LPHY_REQ_SYNC_NORM,
    LPHY_REQ_SYNC_SAMPLE_MODE,
    LPHY_REQ_SYNC_RBIR_ALPHA,
    LPHY_REQ_SYNC_THRESH,
    LPHY_REQ_SYNC_JUDGE2_THRESH,
    LPHY_REQ_SYNC_RPT_SG,
    LPHY_REQ_SYNC_RPT_MODULE,    
    LPHY_REQ_SYNC_DRX_PARA
}LPHY_REQ_MSGID_SYNC_ENUM;

typedef struct __LPHY_REQ_SYNC_NORM_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucRB[6];            /*6RB->100RB*/
}LPHY_REQ_SYNC_NORM_STRU;

typedef struct __LPHY_REQ_SYNC_SAMPLE_MODE_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usSampleMode;            /*0, 1, 2*/
}LPHY_REQ_SYNC_SAMPLE_MODE_STRU;

typedef struct __LPHY_REQ_SYNC_RBIR_ALPHA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usM0FDDOutOfSync;
    UINT16                      usM0FDDInSync;
    UINT16                      usM0TDDOutOfSync[7];
    UINT16                      usM0TDDInSync[7];
    UINT16                      usM1OutofSync;
    UINT16                      usM1InSync;
    UINT16                      usM2FDDOutOfSync;
    UINT16                      usM2FDDInSync;
    UINT16                      usM2TDDOutOfSync[7];
    UINT16                      usM2TDDInSync[7];
    UINT16                      usDrxFDDBTCycLen[2];
    UINT16                      usDrxFDDLTCYCLen;
    UINT16                      usDrxFDDTondur[11];
    UINT16                      usDrxTDDnBTCycLen2[7];
    UINT16                      usDrxTDDnBTCycLen5[7];
    UINT16                      usDrxTDDnBTCycLen8[7];
    UINT16                      usDrxTDDnBTTondur[7];
    UINT16                      usDrxTDDTondur[11];
    UINT16                      usDrxShift[3];
}LPHY_REQ_SYNC_RBIR_ALPHA_STRU;

typedef struct __LPHY_REQ_SYNC_THRESH_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    INT16                       sThresh1OS[6][2][2];        /*BW FDD/TDD TRMS*/
    INT16                       sThresh1IS[6][2][2];
    INT16                       sThresh1OSCdrxShort[6][2][2];
    INT16                       sThresh1ISCdrxShort[6][2][2];
    INT16                       sThresh1OSCdrxLong[6][2][2];
    INT16                       sThresh1ISCdrxLong[6][2][2];
	
}LPHY_REQ_SYNC_THRESH_STRU;

typedef struct __LPHY_REQ_SYNC_JUDGE2_THRESH_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    INT16                       sThreshOS;
    UINT16                      usAlpha;
}LPHY_REQ_SYNC_JUDGE2_THRESH_STRU;

typedef struct __LPHY_REQ_SYNC_RPT_SG_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
}LPHY_REQ_SYNC_RPT_SG_STRU;

typedef struct __LPHY_REQ_SYNC_RPT_MODULE_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usRsv;
}LPHY_REQ_SYNC_RPT_MODULE_STRU;

typedef struct __LPHY_REQ_SYNC_DRX_PARA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usFddGTDrxCycleLength[2];
    UINT16                      usFddGTDrxOnDuration;
    UINT16                      usFddLTDrxOnDuration[11];
    UINT16                      usTddGTDrxCycleLength[3][7];
    UINT16                      usTddGTDrxOnDuration[7];
    UINT16                      usTddLTDrxOnDuration[11];
    UINT16                      usShiftDrxCycle[3];
    UINT16                      usRsv;
}LPHY_REQ_SYNC_DRX_PARA_STRU;

/************************************************************
                           2. CNF���ݽṹ����
************************************************************/
typedef enum __LPHY_CNF_MSGID_SYNC__
{
    LPHY_CNF_SYNC_RESERVED            = OM_CMD_ID(LPHY_SYNC_MID, OM_TYPE_CNF, 0x0),
}LPHY_CNF_MSGID_SYNC_ENUM;

/************************************************************
                           3. Ind���ݽṹ����
************************************************************/
typedef enum __LPHY_IND_MSGID_SYNC__
{
    LPHY_IND_SYNC_RESERVED            = OM_CMD_ID(LPHY_SYNC_MID, OM_TYPE_IND, 0x0),
}LPHY_IND_MSGID_SYNC_ENUM;


/************************************************************
                           4. TRACE���ݽṹ����
************************************************************/
typedef enum __LPHY_TRACE_SYNC_
{
    LPHY_TRACE_SYNC_RESERVED         = OM_CMD_ID(LPHY_SYNC_MID, OM_TYPE_TRACE, 0x0),
    LPHY_TRACE_SYNC_TRACE_BINARY,  
    LPHY_TRACE_SYNC_TRACE,  
}LPHY_TRACE_SYNC_ENUM;

typedef enum __LPHY_TRACE_SYNC_STAT__
{
    LPHY_TRACE_SYNC_STAT_NEW_CFG_PARA = 1,   /*�յ�CONFIG��Ϣ*/
    LPHY_TRACE_SYNC_STAT_INVALID_CFG_PARA,   /*�յ�CONFIG��Ϣ���Ϸ�*/
    LPHY_TRACE_SYNC_STAT_GET_CNF_BUF_FAIL,       /*�յ�CONFIG��Ϣ�ظ�ȷ����Ϣ�޻�����*/
}LPHY_TRACE_SYNC_STAT_ENUM;
typedef UINT16 LPHY_TRACE_SYNC_STAT_ENUM_UINT16;

typedef struct __LPHY_TRACE_SYNC_STRU__
{
    LPHY_TRACE_SYNC_STAT_ENUM_UINT16  enTraceStat;    
}LPHY_TRACE_SYNC_STRU;


/************************************************************
                           5. SG���ݽṹ����
************************************************************/
typedef enum __LPHY_SG_SYNC_
{
    LPHY_SG_SYNC_RESERVED         = OM_CMD_ID(LPHY_SYNC_MID, OM_TYPE_SG, 0x0),
    LPHY_SG_SYNC_SNR,
    LPHY_SG_SYNC_RESULT,
    LPHY_SG_SYNC_STAT,
    LPHY_SG_SYNC_MODULE
}LPHY_SG_SYNC_ENUM;

typedef struct __LPHY_SG_SYNC_SNR_STRU__
{
    INT16       sOSSINR;
    INT16       sISSINR;
}LPHY_SG_SYNC_SNR_STRU;

typedef struct __LPHY_SG_SYNC_RESULT_STRU__
{
    UINT16      usOSFlag;   
    UINT16      usISFlag;   
}LPHY_SG_SYNC_RESULT_STRU;

typedef struct __LPHY_SG_SYNC_STAT_STRU__
{
    INT16       sOSAvgSINR;             /*ʧ��ƽ��SNRֵ*/
    INT16       sOSMaxSINR;             /*ʧ�����SNRֵ*/
    INT16       sOSMinSINR;             /*ʧ����СSNRֵ*/
    INT16       sISAvgSINR;             /*ͬ��ƽ��SNRֵ*/
    INT16       sISMaxSINR;             /*ͬ�����SNRֵ*/
    INT16       sISMinSINR;             /*ͬ����СSNRֵ*/
}LPHY_SG_SYNC_STAT_STRU;

/*
    ͬʧ���ṹ�嶨�壬������Ը��ݿ���ȫ���ϱ�
*/
typedef enum __SYNC_TYPE_E__
{
    SYNC_TYPE_NONE,
    SYNC_TYPE_FILTER,
    SYNC_TYPE_REPORT    
}SYNC_TYPE_E;
typedef UINT16 SYNC_TYPE_ENUM_UINT16;

typedef struct __SYNC_MODULE_STRU__
{
    /*ԭ���·�ͬʧ��ʹ��*/
    UINT8           ucInSyncReportEn;       /*ͬ���ϱ�ʹ��*/
    UINT8           ucOutofSyncReportEn;    /*ʧ���ϱ�ʹ��*/

    /*ͬʧ������*/
    UINT8           ucInitFliterAlhpaFlag;  /*��ʼ���˲���alphaϵ�����*/
    UINT8           ucJudge2InitAlphaFlag;  /*�о���ʽalpha��ʼ�����*/
    UINT16          usCounter;              /*������ʱ���������λ����*/
    SYNC_TYPE_ENUM_UINT16     enType;       /*����ͬʧ����������øñ�ǣ�ͬʧ���о�������ñ��*/
    UINT16          usInSyncAlpha;
    UINT16          usOutSyncAlpha;
    
    /*ͬʧ���о�*/
    UINT16          usOSRptFlag;            /*ʧ������ϱ���� 1����ʾʧ��; 0, ��ʾδʧ��*/
    UINT16          usISRptFlag;            /*ͬ������ϱ���� 1����ʾͬ��; 0, ��ʾδͬ��*/
    INT32           lSINR;                  /*�о���ʽ2�˲�ֵ*/
    UINT16          usJudge2OSFlag;         /*�о���ʽ2ʧ�����*/

    /*��¼ͬʧ�����쳣��֧*/
    UINT8           ucStartDgbBranch1;      /*��¼��ǰ��֡���쳣��֧���������*/
    UINT8           ucCompleteDbgBranch2;   /*��¼��ǰ��֡�Ľ�����֧, �������*/  

    /*��¼OM�ϱ�ÿ����֡�������*/
    LPHY_SG_SYNC_SNR_STRU   stSNR;          /*����ȣ�ÿ��������һ�ζ�һ��*/
    
    /*OM�ϱ�����*/
    UINT32          ulAccTimeCounter;       /*�ۼӾ�����ʱ���������λ����*/
    INT32           lAccNum;                /*�ۼӴ���*/
    INT32           lOSSNRSum;              /*ʧ��������ۼ�ֵ*/
    INT16           sMaxOSSINR;             /*���ʧ��ֵ*/
    INT16           sMinOSSINR;             /*��Сʧ��ֵ*/
    INT32           lISSNRSum;              /*ͬ��������ۼ�ֵ*/
    INT16           sMaxISSINR;             /*���ͬ��ֵ*/
    INT16           sMinISSINR;             /*��Сͬ��ֵ*/

    /*����OM*/
    INT16           sOrgCmuSINR;            /*��CMU��ȡ��SINR*/
    INT16           sOrgCqiSINR;            /*CMU��CQI��CQI�ٸ�RLM��SINR, RLM�ø�ֵ���˲�*/
    INT16           usValidSINR;            /*���Ƿ���Ч�ı�ʾSINR*/
    INT16           usRsv;
	
    /*��ǰ��Drx ״̬*/
    UINT16          usCurDrxFlag;           /*��ǰ�Ƿ����㷨�涨��DRX ״̬*/
    UINT16          usReportPeriod;         /*ͬʧ���ϱ�������ڣ�NON-DRXΪ10���룬��������DRX_CYCLE_LENGTH*/

    /*DRX״̬�µ��µ�ͬʧ���仯���*/
    UINT16          usDrxChgCounter;        /*״̬�仯������*/
    UINT16          usDrxCurEvaPeriod;      /*��ǰDRX ����������*/
    UINT16          usDrxNewEvaPeriod;      /*�µ�DRX ��������*/
    UINT16          usDrxCycleLengthNotified;/*DRX���ڳ���*/    
    UINT8           ucDrxStatusNotified;    /*DRX���DRX ���*/
    UINT8           ucDrxStatusChgFlag;     /*DRX ״̬֪ͨ���±��*/
    UINT8           ucT310TimerStartFlag;   /*LRRC_LPHY_T310_STATUS_IND T310��ʱ���������*/
    UINT8           ucRsv;
}SYNC_MODULE_STRU;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SYNC_OM_DEF_H__ */
