


#ifndef __LTE_NV_PARA_H__
#define __LTE_NV_PARA_H__
#include "product_config.h"
/*#include "rfe_interface.h"*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/

/************************************************************
                               �궨��
************************************************************/
#define    LPHY_TX_DUAL_ANTENNA    2
#define    INTERP_COEF_ROW              9
#define    INTERP_COEF_COLUMN           3
#define    PB_TIME_DOMAIN_INTP_NUM     10
#define    PDU_TIME_DOMAIN_INTP_NUM    14
#define    IIR_ALPHA_NUMBER             6
#define    CHE_CPE_ROW                  8
#define    CHE_CPE_COLOUM               8
#define    LPHY_MAX_BANDWIDTH_NUM       6

#define    RF_9361_HW_TYPE              10
#define    RF_6360_HW_TYPE              11
/* BEGIN: Added by luliwu,  2012/1/13  PN: DTS_V210_12345678 */
#define    UL_APT_VBIAS_NUM             16
/*#ifdef CHIP_RFIC_6360V200*/
#if (defined(CHIP_RFIC_6360V210)||(defined(CHIP_RFIC_6361V100)))
#define    AGC_MAX_GAIN_LEVEL    (16)
/*#define    AGC_MAX_GAIN_LEVEL    (8)*/
/* END:   Added by luliwu,  2012/1/13  PN: DTS_V210_12345678 */


#define  RF_TX_FREQ_NUM (32)
#define RF_RX_FREQ_NUM (32)


#elif defined(CHIP_RFIC_9361)
#define    AGC_MAX_GAIN_LEVEL    (128)

#endif
#define  RF_TX_FREQ_NUM (32)
#define RF_RX_FREQ_NUM (32)
#define    AGC_MAX_GAIN_LEVEL    (16)

#define    AGC_MAX_INTERF_TYPE   2
#define    AGC_MAX_GAIN_TYPE     3

/* BEGIN: Added by w00133787,  2011/5/26  PN: 2011052804846 */
#define NV_RFIC_BALONGV7_MULTIBAN_BAND7
/*#define NV_RFIC_BALONGV7_MULTIBAN_BAND38*/
/* END:   Added by w00133787,  2011/5/26   */
/* BEGIN: Added by shijiahong, 2011/12/23   PN:DTS201112302952*/
#define MAX_HI6360_INITIAL_REG_NUM       40
#define MAX_DSP_CFG_REG_RESV_NUM         4
#define MAX_RF_BAND_NUM                  12
/* END:   Added by shijiahong, 2011/12/23   PN:DTS201112302952*/

/* BEGIN: Added by w00180842, 2013/8/29   PN:tuner*/
#define LTE_TUNER_SUPPORT_SENSOR_NUM  	  	 8
#define LTE_TUNER_DAC_MAX                    6
#define LTE_TUNER_CONTROL_MAX                4
#define LTE_TUNER_CMD_MAX      (LTE_TUNER_DAC_MAX + LTE_TUNER_CONTROL_MAX)
/* END:   Added by w00180842, 2013/8/29 */
/*PBCH  �����׻�*/
/************************************************************
                             ���ݽṹ����
************************************************************/

typedef struct
{
    UINT8 WhitenRuuSize1;                                 /*�����׻�������1*/
    UINT8 WhitenRuuSize2;                                 /*�����׻�������2*/
    UINT8 WhitenThreMode;                                 /*0: ʹ��DSP �����������ֵ  1�� ʹ��NV�����õ�����ֵ*/
    UINT8 Threshold_A;                                    /*�����׻�����Ӧ��������Threshold_A*/
    UINT8 Threshold_B;                                    /*�����׻�����Ӧ��������Threshold_B*/
    UINT8 WhitenSwitchPBCH;                               /*PBCH�����׻�Ӳ����*/
    UINT8 Rsv0[2];
}NV_PB_IRC_STRU;

/*PDC  IRC*/

typedef struct
{
    UINT8 WhitenRuuSize1;                                 /*�����׻�������1*/
    UINT8 WhitenRuuSize2;                                 /*�����׻�������2*/
    UINT8 PdcSInd;                                        /*��ʹ��PDS sym4: 0 ʹ��PDS sym4: 1 */
    UINT8 WhitenThreMode;                                 /*0: ʹ��DSP �����������ֵ  1�� ʹ��NV�����õ�����ֵ*/
    UINT8 Threshold_A;                                    /*�����׻�����Ӧ��������Threshold_A*/
    UINT8 Threshold_B;                                    /*�����׻�����Ӧ��������Threshold_B*/
    UINT8 Threshold_C;                                    /*�����׻�����Ӧ��������Threshold_C*/
    UINT8 WhitenSwitchPDCCH;                              /*PDCCH�����׻�Ӳ����*/
    UINT8 WhitenSwitchPCFICH;                             /*PCFICH�����׻�Ӳ����*/
    UINT8 WhitenSwitchPHICH;                              /*PHICH�����׻�Ӳ����*/
    UINT8 Rsv0[2];
    UINT16 PHICH_TH_NonWhiten;                            /*PHICH�����׻�Ӳ���عر�ʱ������*/
    UINT16 PHICH_TH_Whiten;                               /*PHICH�����׻�Ӳ���ؿ���ʱ������*/
} NV_PDC_IRC_STRU;

/*PDS  IRC*/
typedef struct
{
     UINT8 WhitenRuuSize1;                                /*�����׻�������1*/
     UINT8 WhitenRuuSize2;                                /*�����׻�������2*/
     UINT8 PdsSInd;                                       /*��һ��slot �������� ʹ��������0��1*/
     UINT8 WhitenThreMode;                                /*0: ʹ��DSP �����������ֵ  1�� ʹ��NV�����õ�����ֵ*/
     UINT8 Threshold_A;                                   /*�����׻�����Ӧ��������Threshold_A */
     UINT8 Threshold_B;                                   /*�����׻�����Ӧ��������Threshold_B */
     UINT8 Threshold_C;                                   /*�����׻�����Ӧ��������Threshold_C */
     UINT8 WhitenSwitchPDSCH;                             /*PDSCH�����׻�Ӳ����*/
     UINT8 BfWhitenRuuSize1;                              /*�����׻�������1*/
     UINT8 BfWhitenRuuSize2;                              /*�����׻�������2*/
     UINT8 BfPdsSInd;                                     /*��һ��slot �������� ʹ��������0��1*/
     UINT8 BfWhitenThreMode;                              /*0: ʹ��DSP �����������ֵ  1�� ʹ��NV�����õ�����ֵ*/
     UINT8 BfThreshold_A;                                 /*�����׻�����Ӧ��������Threshold_A */
     UINT8 BfThreshold_B;                                 /*�����׻�����Ӧ��������Threshold_B */
     UINT8 BfThreshold_C;                                 /*�����׻�����Ӧ��������Threshold_C */
     UINT8 BfWhitenSwitchPDSCH;                           /*PDSCH�����׻�Ӳ����*/
} NV_PDS_IRC_STRU;
/* END:   Added by l00174474, 2011/6/8 */

typedef struct
{
    NV_PB_IRC_STRU     stPbIrc;
    NV_PDC_IRC_STRU    stPdcIrc;
    NV_PDS_IRC_STRU    stPdsIrc;

} NV_IRC_PUB_STRU;



typedef struct
{
   UINT8   RsIntpCoef[INTERP_COEF_ROW][INTERP_COEF_COLUMN];         /*����ϵ������*/
   UINT8   Rsv0;
   UINT16  PbchTimeIntCoef[PB_TIME_DOMAIN_INTP_NUM];                /*PBʱ���ֵϵ������*/
   UINT16  PduTimeIntCoef[PDU_TIME_DOMAIN_INTP_NUM];                /*PDUʱ���ֵϵ������*/
   UINT8   AlphaCoef[IIR_ALPHA_NUMBER];                             /*Alpha�˲�ϵ������*/
   UINT16  Rsv1;
   UINT32  ChanPhaseErrRx0[CHE_CPE_ROW][CHE_CPE_COLOUM];
   UINT32  ChanPhaseErrRx1[CHE_CPE_ROW][CHE_CPE_COLOUM];
} NV_CHE_INTP_STRU;

/*��ʱNV����*/
typedef struct{
    VOS_UINT16  T_ParTD[6];         /*default: [48 64 64  96 96 96]ʱ�����ʱ�ŵ�PDP����ȱ������ޣ���Ӧ1.4M��20M����                       */
    VOS_UINT16  T_ParFD[6];         /*default: [64 64 64  96 96 96]Ƶ�����ʱ�ŵ�PDP����ȱ������ޣ���Ӧ1.4M��20M����                       */
    VOS_UINT16  TrefShort[6];       /*default: [32 32 11 11 8 8]   DRXԤͬ�� or IDLE��Ѱ�������Ķ�ʱ��ǰ������0.5TsΪ��λ,��Ӧ1.4M��20M���� */
    VOS_UINT16  TrefCommon[6];      /*default: [16 16 11 11 8 8]   ���������Ķ�ʱ��ǰ������0.5TsΪ��λ,��Ӧ1.4M��20M����                    */
    VOS_UINT16  FPWLenShort[6];     /*default: [3 6 12 24 48]      DRXԤͬ�� or IDLE��Ѱ���������׾����������ȣ���Ӧ1.4M��20M����           */
    VOS_UINT16  FPWLenCommon[6];    /*default: [10 20 40 80 160]   �����������׾����������ȣ���Ӧ1.4M��20M����                              */
    VOS_UINT16  Tb;                 /*default: 384                 ��W1b��W2b����������                                                     */
    VOS_UINT16  EndValue;           /*default: 8                   �˲�ϵ����������ֵ                                                      */
    VOS_UINT16  W;                  /*default: 128                 �ྶ��������������                                                       */
    VOS_UINT16  FPWindow;           /*default: 20                  �ྶ���������ڷ�ֵ��ǰ�����׾��ķ�Χ                                     */
    VOS_UINT16  HoldThrsh;          /*default: 32                  �ྶ������������ȱ�������                                               */
    VOS_UINT16  Ncs;                /*default: 5                   ����̬DRX�����жϽ����ʱ��Ч�Ե�С����������                            */
    VOS_UINT16  ResetThrs;          /*default: 288                 �ྶ�������жϽ����ʱ��Ч�Ե�����(0.5Ts)                                */
    VOS_UINT16  Reserved;           /*default: 0                                                                                            */
}NV_TIMING_PARA_STRU;

typedef struct
{
    VOS_UINT8   N_6RB[4];           /*default: {[40,40,20,20]}*/
    VOS_UINT8   N_15RB[4];          /*default: {[40,40,20,20]}*/
    VOS_UINT8   N_Other;            /*default: {10}*/
    VOS_UINT8   Reserverd[3];       /*default: {0}*/
}NV_EMU_FAKECELL_PARA_STRU;

typedef struct{
    VOS_UINT8   RuuAlpha;                       /*default:{{0},             Ruu���˲�����                                                  */
    VOS_UINT8   WhitenRBStep;                   /*default:{0},              �׻����ص�Ƶ���Ӵ���������                                      */
    VOS_UINT8   CSIWhitenFlag;                  /*default:{0},              CQI�����Ƿ�׻���Ӳ����                                          */
    VOS_UINT8   CSIWhitenSwitchMode;            /*default:{0},              CQI�����Ƿ���ȫ����ĸ��ž��������׻�����                        */
    VOS_UINT8   CSIWhitenKg[6];                 /*default:{1,1,1,1,1,1},    �׻����ص�Ƶ��RB��������                                         */
    VOS_INT8    ESINRRepairFactor1[24];         /*default:{1,1,1,0,0,1,-1,-1,-1,-1,-1,-1,1,1,1,0,0,1,-1,-1,-1,-1,-1,-1},  ��Ч����Ȳ���ֵ1            */
    VOS_INT8    ESINRRepairFactor2[24];         /*default:{-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0},  ��Ч����Ȳ���ֵ2            */
    VOS_INT8    ESINRRepairFactor3[24];         /*default:{0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1},  ��Ч����Ȳ���ֵ3            */
    VOS_INT8    ESINRRepairFactor4[24];         /*default:{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},  ��Ч����Ȳ���ֵ4            */
    VOS_INT8    RuuZeroFlag;                    /*default: 1*/
    VOS_INT8    Reserved;                       /*default: 0*/
}NV_CQI_PARA_STRU;

typedef struct{
    VOS_UINT8   RuuAlpha;                        /*default:{{0},      Ruu���˲�����                                                  */
    VOS_INT8    RLMWhitenSwitch;                 /*default:{1},       RLM�����Ƿ�׻���Ӳ����                                         */
    VOS_INT8    RLMSwitchMode;                   /*default:{0},       �׻����ص������Ƿ���ȫ����                                      */
    VOS_INT8    RLMWhitenN[2];                   /*default:{10,1},    �׻����ص�����,[0]��Ӧ����1��[1]��Ӧ����2                       */
    VOS_INT8    RLM_WhitenThd_FDD[24];           /*default:{0,...,0}, FDD��RLM Whiten�о���������ֵ                                  */
    VOS_INT8    RLM_WhitenThd_TDD[24];           /*default:{0,...,0}, TDD��RLM Whiten�о���������ֵ   "                              */
    VOS_INT8    RuuZeroFlag;                     /*default: 1*/
    VOS_INT8    Reserved[2];                     /*Reseved: TBD*/
}NV_RLM_PARA_STRU;

typedef struct{
    VOS_INT8    Corr_Tlayer1IRC;                 /*default: 19,       1�������߻�1��������ʱ���ж������׻��������Ե�����ֵ         */
    VOS_INT8    Corr_Tlayer2IRC;                 /*default: 4},       2��2�ջ�4��2��ʱ���ж������׻��������Ե�����ֵ              */
    VOS_INT8    Reserved[2];                     /*Reseved: TBD*/
 }NV_ANTCORR_PARA_STRU;

/*VITERBI*/
typedef struct
{
    VOS_UINT16 DciTotalPwrThresh[2][4][4];      /*ȫ��CCE�������ֵ�о�����, [�����׻�����][RSSP����][DAGC����������]*/
    VOS_UINT16 DciSinglePwrThresh[2][4][4];     /*��CCE�������ֵ�о�����, [�����׻�����][RSSP����][DAGC����������]*/
    VOS_UINT16 DciBerThresh[2];                 /*BER�о�����, [�����׻�����]*/
    VOS_UINT32 CcDecTraceSelOrder;              /*�ӱ�ѡ���Ҵ�·��ѡ��ͻ���ʱ��ʼ״̬ѡ������ȼ�˳�򿪹�*/
}NV_VITERBI_PARA_STRU;

/*TURBO*/
typedef struct
{
    VOS_UINT32 TurboDecIterFactor;              /*���ε����ɿ�����, ȡֵ��Χ: 35-48, Ĭ��Ϊ45*/
}NV_TURBO_PARA_STRU;

typedef struct
{
    UINT16    ausRsThrd1Fdd[LPHY_TX_DUAL_ANTENNA][LPHY_MAX_BANDWIDTH_NUM];
    UINT16    ausRsThrd1TddCfg0[LPHY_TX_DUAL_ANTENNA][LPHY_MAX_BANDWIDTH_NUM];
    UINT16    ausRsThrd1TddNCfg0[LPHY_TX_DUAL_ANTENNA][LPHY_MAX_BANDWIDTH_NUM];
#if 0
    UINT16    ausOLHighThrd[2][LPHY_MAX_BANDWIDTH_NUM];
    UINT16    ausOLLowThrd[2][LPHY_MAX_BANDWIDTH_NUM];
#endif
    UINT16    ausILHighThrd[2][LPHY_MAX_BANDWIDTH_NUM];
    UINT16    ausILLowThrd[2][LPHY_MAX_BANDWIDTH_NUM];
}AFC_NV_RS_CONFIG_STRU;
typedef struct
{
    UINT16 usVctcxoThdHigh;             /* ��̬TCXO������ */
    UINT16 usVctcxoDynamic;             /* ��̬TCXOֵ, UE�ϵ�ʱ��HPA��ȡNV������DSP,
                                               UE�ϵ翪����������̬���DSP��ȡ��Чֵд��NV */
    UINT16 usVctcxoFlag;                /* ��̬TCXOȡֵ�Ƿ���Ч��ʶ, DSP�ɲ���,
                                               0: ��Ч��������Ч����ЧʱusDynamicTcxoֵ������ */
    UINT16 usVctcxoFuncSwitch;          /* ��̬TCXO�����Ƿ�ʹ�ÿ���, DSP�ɲ���,
                                               �޸ĺ�, ��Ҫ�µ����ϵ�Ż���Ч
                                               0: �ر�, �ر�ʱusVctcxoDynamicȡֵusVctcxoInit
                                               1: �� */
}LTE_TCXO_DYNAMIC_CONFIG_STRU;

typedef struct
{
    UINT16    usEnableFlag;
    UINT16    usHSTMode;
    AFC_NV_RS_CONFIG_STRU    stAfcNvCRsConfig;
/*#ifdef CHIP_BB_6920CS*/
    AFC_NV_RS_CONFIG_STRU    stAfcNvMRsConfig;
/*#endif*/
    UINT16    usFreqAdjustLimit;/*usLoopAdjustLimit;*/
    UINT16    usFOHSTCompensation;
    /*UINT16    usInitPDM;*/
    /*UINT16    usDynamicPDM;*/
    UINT16    usVoltageFactor;
    UINT16    usMFreqAdjustLimit;

    UINT16    usPdmHighThreshold;
    UINT16    usFreqOffsetThreLow;
    UINT16    usFreqOffsetThreHigh;
    UINT16    usPdmCcThreshold;
    UINT16    ausPdmParThreshold[LPHY_MAX_BANDWIDTH_NUM];
}AFC_NV_PARA_STRU;

typedef struct {
    UINT8       ucVersion[16];                  /*�����汾������*/
    UINT32      ulVersionId;                    /*�����汾��ʶ*/
}NV_VERSION_STRU;

typedef struct
{
    INT16     sPreTargetPowerFdd;
    INT16     sPreTargetPowerTdd;
    INT16     sPostTargetPower;
    INT16     sLeastSNR;

    UINT8     ucDAGCAdjBitForTM1To6;
    UINT8     ucDAGCAdjBitForTM7To8;
    INT8      cMinAntPwr;
    INT8      cMaxAntPwr;

    UINT8     ucACSFilterThreshold[LPHY_MAX_BANDWIDTH_NUM];
    INT8      cACSPowerThreshold0[LPHY_MAX_BANDWIDTH_NUM];
    INT8      cACSPowerThreshold1[LPHY_MAX_BANDWIDTH_NUM];
    UINT8     ucNBFilterThreshold[LPHY_MAX_BANDWIDTH_NUM];
    INT8      cNBPowerThreshold0[LPHY_MAX_BANDWIDTH_NUM];
    INT8      cNBPowerThreshold1[LPHY_MAX_BANDWIDTH_NUM];
}NV_AGC_PARA_STRU;
/*UL NV */
typedef enum
{
    APC_GAIN_LEVEL_0 = 0,
    APC_GAIN_LEVEL_1 ,
    APC_GAIN_LEVEL_2 ,
    APC_GAIN_LEVEL_3,
    APC_GAIN_LEVEL_INVALID
}UL_APC_GAIN_LEVEL_ENUM;


typedef enum
{
    APC_GAIN_NONCROSS = 0,
    APC_GAIN_CROSS
}UL_APC_GAIN_CROSS_FLAG_ENUM;

typedef UINT16 UL_APC_GAIN_CROSS_FLAG_ENUM_UINT16;
typedef UINT16 UL_APC_GAIN_LEVEL_ENUM_UINT16;

typedef struct
{
    INT16 sPwrLow;
    INT16 sPwrHigh;
    UL_APC_GAIN_LEVEL_ENUM_UINT16 enAPcGainLevel;
    UL_APC_GAIN_CROSS_FLAG_ENUM_UINT16 enAgcGainCrossFlag;
}UL_APC_GAIN_CTRL_STRU;

typedef struct
{
    INT16  sPwr;
    UINT16 usVga;
}UL_APC_TABLE_ITEM_STRU;


#define APC_GAIN_AREA_NUM 3
#define APC_TABLE_NUM 2
#define APC_TABLE_LEN 64
/*#define APC_GAIN_LEVEL_NUM 4*/

typedef struct
{
    UINT32 ulUlCfrSwitch;
    INT16  sSrsCfrThred;
    INT16  sPuschCfrThred;
}UL_CFR_CTRl_STRU;

typedef struct
{
    UL_CFR_CTRl_STRU stCfrCtrl;
    UL_APC_GAIN_CTRL_STRU astApcCtrl[APC_GAIN_AREA_NUM];
    UL_APC_TABLE_ITEM_STRU astApcTable[APC_TABLE_NUM][APC_TABLE_LEN];
}NV_UL_PARA_STRU;

/* BEGIN: Added by shijiahong, 2011/12/23   PN:DTS201112302952*/

typedef struct
{
    UINT32 ulRegAddr;
    UINT32 ulValue;
}NV_DSP_RF2BBP_CFG_STRU;

typedef struct
{
    UINT16 usRficRegAddr;
    UINT16 usRficRegValue;
}NV_LPHY_RFIC_CFG_STRU;

/*****************************************************************************
 �ṹ����  : NV_LPHY_RF_LINECTRL_COMMON_CONFIG
 Э����  :
 ASN.1���� :
 ö��˵��  : LTE RF�߿أ�ȡ����Ӳ����̬
*****************************************************************************/
typedef struct
{
    NV_DSP_RF2BBP_CFG_STRU stRficTcvrOn;                               /*6360RF����*/
    NV_DSP_RF2BBP_CFG_STRU stRficPaPowCtrl;                            /*6360RFPA�����ѹ����*/
    NV_DSP_RF2BBP_CFG_STRU stTddTrxSwitchCtrl;                         /*TDD TRX SWITCH �ɹ̶�����Ϊ0x24000000*/
    NV_DSP_RF2BBP_CFG_STRU stRficTxIQFmt;                              /*ABB TXͨ��IQ��ʽ*/
    NV_DSP_RF2BBP_CFG_STRU stRficResetA;                               /*6360RF��λ�ź�A*/
    NV_DSP_RF2BBP_CFG_STRU stRficResetB;                               /*6360RF��λ�ź�B*/
}NV_LPHY_RF_LINECTRL_COMMON_CONFIG;


typedef struct
{
    NV_DSP_RF2BBP_CFG_STRU stTxAntSel;                 /* B38/B40��������A,Bͨ��ѡ��,ǿ�����߻����� 0:��ͨ����0x3ffffff :��ͨ��,0x60000000�߼��Կ� */
    NV_DSP_RF2BBP_CFG_STRU stAntennaSwitchIntraRx;    /* �����Ƿ���Ҫˢ��?  INTTRA */
	NV_DSP_RF2BBP_CFG_STRU stAntennaSwitchIntraTx;    /* �����Ƿ���Ҫˢ��?  INTTRA */
    NV_DSP_RF2BBP_CFG_STRU stAntennaSwitchInter;      /* �����Ƿ���Ҫˢ��? INTTER */
    NV_DSP_RF2BBP_CFG_STRU stPaEnBitMap;               /*PA BITMAP���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stPaMode0BitMap;            /*PA MODE0 BITMAP���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stPaMode1BitMap;            /*PA MODE1 BITMAP���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stTRxSwitchBitMap;          /*TDD�շ������ź�map���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stTxSwitch0BitMap;          /*TM8/TM9��������ѡ���ź�map���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stTxSwitch1BitMap;          /*TM8/TM9��������ѡ���ź�map���ã���������GPIOѡ����Ӧbit*/
    NV_DSP_RF2BBP_CFG_STRU stPaEnCfg;                   /*PA�����ź�����*/
    NV_DSP_RF2BBP_CFG_STRU stRficPaAtt1;                /*PA MODE0�����ź�����*/
    NV_DSP_RF2BBP_CFG_STRU stRficPaAtt2;                /*PA MODE1�����ź�����*/
}NV_LPHY_RF_LINECTRL_PRIVAT_CONFIG;

typedef struct
{
    UINT32 ulReserveCfgNum;
    NV_DSP_RF2BBP_CFG_STRU stReserveCfgInfo[MAX_DSP_CFG_REG_RESV_NUM];
}NV_LPHY_RF_LINECTRL_RESERVED_CONFIG;

typedef struct
{
    UINT8 ucRfMainRxLnaSel;             /*RF RX ����ʹ��LNA*/
    UINT8 ucRfDvtyRxLnaSel;             /*RF RX ����ʹ��LNA*/
    UINT8 ucRfTxLnaSel;                 /*RF RX ����ʹ��LNA*/
    UINT8 ucRfRxVcoCoreSel;             /*RF RX VcoCoreѡ��*/
    UINT8 ucRfRxDividerSel;             /*RF RX Dividerѡ��*/
    UINT8 ucResv;
    UINT8 ucRfTxVcoCoreSel;             /*RF TX VcoCoreѡ��*/
    UINT8 ucRfTxDividerSel;             /*RF TX Dividerѡ��*/
}NV_LPHY_RFIC_REG_PRIVAT_PARA;

typedef struct
{
    UINT32 ulRficInitalRegNum;
    NV_LPHY_RFIC_CFG_STRU stRficCfgVar[MAX_HI6360_INITIAL_REG_NUM];
}NV_LPHY_RFIC_INITIAL_CFG_STRU;

typedef struct
{
    NV_LPHY_RF_LINECTRL_COMMON_CONFIG   stRFLineCtrlComnVar;                        /*�����Ĵ���ֵ��Value */
    NV_LPHY_RF_LINECTRL_RESERVED_CONFIG stRFLineCtrlComnRsv;                   /*��Band�޹ص�������չʹ�� */
}NV_LPHY_LINECTRL_COMN_INFO_STRU;

typedef struct
{
    UINT32 ulHWBoardType;                                                          /*��ǰ�ĵ������� */
    UINT32 ulBandNum;                                                              /*��ǰ��Band��Ŀ; */
    UINT32 ulInitialBandIndex;                                                     /*��ǰĬ�ϵ�Band */
    UINT8  ucBandInd[MAX_RF_BAND_NUM];                                             /*֧�ֵ�Band�б� */
    NV_LPHY_RF_LINECTRL_PRIVAT_CONFIG   stRFLineCtrlPrivatVar[MAX_RF_BAND_NUM];     /*�����Ĵ���ֵ��Value */
    NV_LPHY_RF_LINECTRL_RESERVED_CONFIG stRFLineCtrlPrivatRsv[MAX_RF_BAND_NUM];/*��Band��ص�������չʹ�� */
    NV_LPHY_RFIC_REG_PRIVAT_PARA        stRficRegPrivatPara[MAX_RF_BAND_NUM];       /*���漰�����õ�ֵ */
}NV_LPHY_LINECTRL_PRIV_INFO_STRU;

typedef struct
{
    UINT8 enSingleReceiverChnSel;
    INT8  cRsv[7];
}NV_PHY_FUNC_VERIFY_STUB_STRU;



/* BEGIN: Added by w00180842, 2013/8/29   PN:tuner*/

/*MIPIд�����ʽ����
31--29  001  ---->ָ������
28--25  0111 ---->SLAVE ADDRESS
24--22  010  ---->cmd
21--17         ---->addr ����0x1c,0x00��
16                ---->17--24��У��λ
15--8           ---->���õ�����
7                  ---->15---8��У��λ
6--0             ---->����λ
 aulControlCfgPara[i] �谴������BITλȫ��д��
 usDacCmd    15--0���ζ�Ӧ���� 31--16BITλд��
 ausDacData[j][k]15---0���ζ�Ӧ����15--0BITλд��
 */
typedef struct 
{
    UINT16 usDacCmd;
    UINT16 usRev;
    UINT16 ausDacData[MAX_RF_BAND_NUM][LTE_TUNER_SUPPORT_SENSOR_NUM];//BAND˳����BAND�б���ͬ
}LTE_TUNER_DAC_CFG_PARA_STRU;
typedef struct 
{
    UINT16 usDacCfgNum;//������ox00�Ĵ�����DACʹ�ܵĸ���һ��
    UINT16 usRev;
    LTE_TUNER_DAC_CFG_PARA_STRU stDacCfgPara[LTE_TUNER_DAC_MAX];
}LTE_TUNER_DAC_CFG_STRU;
 typedef struct 
 {
     UINT16 usControlCfgNum;//���ֵΪ4��SLEEP MODE ĿǰĬ������һ��(0x1c)��activeMODE Ĭ������3��
     UINT16 usRev;
     UINT32 aulControlCfgPara[LTE_TUNER_CONTROL_MAX];//����ʱ��0--5�������ã�0-->0x1c, 1-->0x10, 2-->0x0���������η���
 }LTE_TUNER_CONTROL_CFG_STRU;
typedef struct 
{
    LTE_TUNER_CONTROL_CFG_STRU stControlCfg;
    LTE_TUNER_DAC_CFG_STRU stDacCfg;
}LTE_TUNER_MIPI_ACTIVE_MODE_CMD_STRU;
typedef struct 
{
    LTE_TUNER_CONTROL_CFG_STRU stControlCfg;
}LTE_TUNER_MIPI_SLEEP_MODE_CMD_STRU;
typedef struct
{
    UINT16 usTunerValidFlag;//1--֧��   0--��֧��
    UINT16 usCfgTimeAdv;//ʱ����ǰ��
    UINT16 usRev;
    UINT16 usBandNum;//֧�ֵ�BAND��
    UINT16 ausBandList[MAX_RF_BAND_NUM];//BAND�б�
    LTE_TUNER_MIPI_SLEEP_MODE_CMD_STRU stSleepModeCfg;
    LTE_TUNER_MIPI_ACTIVE_MODE_CMD_STRU stActiveModeCfg;
     
}NV_LPHY_TUNER_NV_PARA_STRU ;
/* END:   Added by w00180842, 2013/8/29 */

/* BEGIN:   Added by x00228734, 2013/9/30 */
typedef struct
{
    UINT16 usLteMasterIdrxEnable;
    UINT16 usLteMasterCdrxEnable;
    UINT16 usLteSlaveSleepEnable;
    UINT16 usLteDrxSwitch32kTime;
    UINT16 usRsvd[4];
}LTE_DRX_NV_PARA_STRU;
/* END:   Added by x00228734, 2013/9/30 */
/*****************************************************************************
 �ṹ����  : NV_LPHY_DSP_CONFIG
 Э����  :
 ASN.1���� :
 ö��˵��  : LPHY DSP������ʼ��������������NV�ṹ�嶨��
*****************************************************************************/
typedef struct{
    LRRC_LPHY_ANTANNA_PORT_ENUM_UINT16  enAntennaPortsCount;                /*LPHY UE���������������������źŵ�������Ŀ*/
    LRRC_LPHY_LTE_MODE_TYPE_ENUM_UINT16 enLteMode;                          /*LPHY ��ʼ��ʱĬ�ϵĸ���ģʽ*/
    UINT32                              aulNvLteAntennaSwitchBand[MAX_RF_BAND_NUM];       /*LTE��Band��Ӧ�����߿���ֵ, ����0:9��FDD���߿��ػ�TDD Rx���߿���ֵ;����16:25��TDD Tx���߿���ֵ*/
    UINT16                              usNvLteOccaPreTime;                 /*lte������ϵͳoccasionʱԤ�����ж���ǰ������λus*/
    UINT8                               ucPowerOnLoadFlag;                  /*LPHY DSP�ϵ��һ�μ��ر�ʶ: "1"�ϵ��һ�μ���;"0"���ϵ��һ�μ���*/
    UINT8                               ucNvLteInitialCfgRfabb;             /*LPHY ��ʼ��ʱ�Ƿ�����RF��ABB��ʶ��"1"����;"0"������*/
    UINT8                               ucHwVersion;                        /*BalongV7R1Ӳ���汾��*/
    UINT8                               ucNvLteBandNumber;                  /*LTE Band����,<8*/
    UINT8                               aucNvLteBandInd[MAX_RF_BAND_NUM];                  /*LTE Band�����飬ÿ��ֵ��ӦLTE Band��*/
    UINT8                               aucNvLtePaEnInd[MAX_RF_BAND_NUM];                  /*LTE��Band��Ӧ��PA En����λ,Bandָʾ��aucNvLteBandIndһ��*/
    UINT8                               ucNvLteDefaultBandIdx;              /*��ʼ��ʱȱʡBAND���*/
    UINT8                               ucUeCap;                            /*1��ʾue����1��2��ʶue����2��5��ʾue����5*/
    UINT8                               ucAntSelect;                    /*0:false,1:true */
    UINT8                               ucSupport64Qam;                    /*0:64qam��֧�֣�1:64qcam֧��*/
    /* END:   Added by l00131321, 2011/10/15 */
    #if 0
    /*UINT8                               aucDspCfgRsv[2];   */                 /*LPHY ��ʼ������Ԥ������*/
    UINT8                               ucIdleDrxControlFlag;/*IDLE DRX���Ʊ�־ */
    UINT8                               aucDspCfgRsv;                    /*LPHY ��ʼ������Ԥ������*/

    #endif
    UINT16                              usLteTxTrxSwitchCfg;                /*LTE��Tx_switch��TRx_switch��չ��־����ʾTx_switch�Ƿ�Ҫ��չΪ2��*/
    UINT16                              usLteAbbIfFormat;                   /*LTE ABB�ӿ�����*/
    UINT8                               ucLteC0C1Sel;                       /*˫��ʱlte����ͨ��c0����c1��*/
    UINT8                               ucLteSsiReadChSel;                  /*lte SSI�ض���ͨ��ѡ��*/
    UINT8                               ucLteRficAd9361Sel;                 /*lte �Ƿ�ѡ��AD9361*/
    UINT8                               ucLteAd9361TcvrOnFlag;              /*LTEʹ��AD9361֧�ֶ�ģ����ʱLTE��TCVR_ON�ܽ���ͨ��0�����״̬*/
    UINT8                               ucTxiqFeedbackPath;                 /*Hi6361s ͨ·A/Bѡ��*/
    /*UINT8                               aucDspCfgRsv;    */                /*LPHY ��ʼ������Ԥ������*/
    UINT8                               ucIdleDrxControlFlag;/*IDLE DRX���Ʊ�־ */
}NV_LPHY_DSP_CONFIG;



/*****************************************************************************
 �ṹ����  : NV_GULPHY_COMMON_PARA_CONFIG
 Э����  :
 ASN.1���� :
 ö��˵��  : GUL��ģ������������NV�ṹ�嶨��
*****************************************************************************/
typedef struct{
    UINT8                               ucNvSingleDualModeInd;              /*��˫��ָʾ��0��������1��˫��*/
    UINT8                               ucNvSingleDualRficInd;              /*��˫RFIC��0��˫RFIC��1����RFIC*/
    UINT8                               ucNvCtuTimingDelayCnt;              /*CTU��⵽timing_get_ind��̶��ӳ�nv_ctu_timing_delay_cnt��CTUʱ�����ں�����timing_valid�źš�*/
    UINT8                               ucNvPaMuxCtrl;                      /*GUL PA���ÿ��ƣ�3bit*/
}NV_GULPHY_COMMON_PARA_CONFIG;

typedef struct
{
              UINT16 usT1;                  /*T1Ĭ��ֵ����(������Ƶ���üĴ�����Ŀ����)*/
              UINT16 usT2;                  /*T2Ĭ��ֵ����(RFIC��Ƶʱ��)*/
              UINT16 usT3FDD;               /*T3_FDDĬ��ֵ����(��Ƶ����Ƶ֮��ı���ʱ��)*/
              UINT16 usT3TDD;               /*T3_TDDĬ��ֵ����(��Ƶ����Ƶ֮��ı���ʱ��)*/
              UINT16 usT4;                  /*T4Ĭ��ֵ����(FIR�˲�����ʱ)*/
              UINT16 usAGCCfgTime;          /*AGC������ǰ��(�߼�������Ƶ�������ǰ��)*/
              UINT16 usFDDADCOpenTime;      /*FDD ABB ADC����ǰ��*/
              UINT16 usTDDADCOpenTime;      /*TDD ABB ADC����ǰ��*/
              UINT16 usDACOpenTime;         /*ABB DAC����ǰ��*/
              UINT16 usT7UL;                /*UL_RFIC_T7(TDD����ģʽ�л�������ǰ��)*/
              UINT16 usT7DL;                /*RFIC_T7(TDD����ģʽ�л�������ǰ��)*/
              UINT16 usT8ULFDD[6];          /*UL_FDD_T8_COMPENSATE*/
              UINT16 usT8ULTDD[6];          /*UL_TDD_T8_COMPENSATE*/
              UINT16 usT9;                  /*T9(TDD����ģʽ�л�������ǰ��)*/
              UINT16 usT10VGASPICfgTime;    /*T10����VGA��SPI����ʱ��*/
              UINT16 usT15;                 /*T15*/
              UINT16 usTul[6];              /*����ͨ����ʱ*/
              UINT16 usPaOpenTime;          /*PA��ʱ�䣬����PA����*/
              UINT16 usPaAttTime;           /*PA ATT��ʱ�䣬����PA����*/
              UINT16 usAntSelTime;          /*ANT��ʱ�䣬����ANT����*/
              UINT16 usReserve;             /*��������*/
}NV_DSP_CONFIG_TIME_STRU;
/*****************************************************************************
 �ṹ����  : NV_LPHY_AGC_BASE_TABLE_STRU
 Э����  :
 ASN.1���� :
 ö��˵��  : LPHY DSP AGC��׼��NV�ṹ�嶨��
*****************************************************************************/
typedef struct{
    UINT32 aulRx1AgcBaseTable[128];
    UINT32 aulRx2AgcBaseTable[128];
}NV_LPHY_AGC_BASE_TABLE_STRU;

typedef struct
{

     INT16 usFreqCmpInd;
     INT16 usTempCmpInd;
}NV_BT_CMP_SWITCH_STRU;

typedef struct
{
    UINT16 usDbbAtt;
	UINT16 usAbbAtt;
}NV_APC_ATT_TABLE_STRU;

typedef struct
{
    INT16 sHigh2MidThre;
	INT16 sMid2HighThre;
	INT16 sMid2LowThre;
	INT16 sLow2MidThre;
}NV_PA_LEVEL_THRE_STRU;
typedef struct
{
    UINT32 ulDbbAttRegVal;
    UINT32 ulAbbAttRegVal;
    UINT32 ulAptPdmRegVal;
}UL_PWR_REG_STRU;
typedef struct
{
    UINT16 usEnableFlg;
    UINT16 usTimingAhead;
}UL_APT_PARA_STRU;
typedef struct
{
    INT16  sTxpower;
    UINT16 usPdmVal;
}UL_APT_TX_POWER_PDMVAL_STRU;
typedef struct
{
    UINT32 ulNum;
    UL_APT_TX_POWER_PDMVAL_STRU stPower2PdmVal[UL_APT_VBIAS_NUM];
}UL_APT_GAIN_VS_VBIAS_STRU;

#define PA_LEVEL_NUM  3
#define LNA_LEVEL_NUM 4
#define APC_ATT_MAX_NUM 88

typedef struct __NV_DEM_LIST_STRU__
{
    UINT8    ucInAdjBit;
    UINT8    ucInAdjBitBF;
    UINT8    ucPdschSfbcDemAlg;
    UINT8    ucPdschSdmaDemAlg;
    UINT16   usPHICHDemTHIrcOFF;
    UINT16   usPHICHDemTHIrcON;
}NV_DEM_LIST_STRU;
typedef enum
{
    DCOC_SAMPLE = 0,
    DCOC_SYMBLE,
    DCOC_SUBFRAME,
}DCOC_UPDATECONFIG_ENUM;
typedef UINT16 DCOC_UPDATECONFIG_ENUM_UINT16;
typedef struct
{
    INT16 usPaDiff[4];
}NV_PA_POWER_DIFFERENCE;

typedef struct
{
    UINT16 usCompDCI[32];
    UINT16 usCompDCQ[32];
}RF_DCOC_CAL_STRU;

typedef struct
{
    RF_DCOC_CAL_STRU stDcocComp[2];
}NV_DCOC_CAL_TABLE;


typedef struct
{
    UINT16 usIIP2Comp[24];
}NV_IIP2_CAL_TABLE;

typedef struct
{
    INT16  sAmplitude;
    INT16  sPhase;
    INT16  sDCI;
    INT16  sDCQ;
}RF_TXIQ_CAL_STRU;

typedef struct
{
    INT16 sGainStatus[16];
}NV_TX_APC_GAIN_STRU;

typedef struct
{
    UINT16 usBias[16];
}NV_TX_RF_BIAS_STRU;

typedef struct
{
    UINT16 usBB0DBAllowedFlg;
    INT16 sMaxBBAttVal;
    INT16 sMaxPower;
    INT16 rsd;
}NV_TX_RF_BB_ATT_STRU;

typedef struct
{
    INT16 sFreqComp[RF_TX_FREQ_NUM];
}NV_TX_RF_FREQ_COMP_STRU;
typedef struct
{
    INT16 sPaTempComp[3][32];
}NV_TX_PA_TEMP_COMP;
typedef struct
{
    UINT16 usAttenTable[60];
}NV_TX_ATTEN_TABLE;
typedef struct
{
    UINT16 usPowerVoltage[3];
}NV_TX_POWERDET_VOLTAGE;


typedef struct
{
    /*band related*/

    UINT8     ucAGCAcsMinGainZ;
    UINT8     ucAGCNbMinGainY;
    UINT8     ucReserved;
    UINT8     ucDefaultGain0;

    UINT8     ucDefaultGain1;
    UINT8     ucDefaultGain2;
    DCOC_UPDATECONFIG_ENUM_UINT16    enDcocUpdateConfig;

    /*RF/AGC�����*/
    UINT32     ulAGCGainTable[AGC_MAX_GAIN_LEVEL];
    UINT32     ulAGCAcsGainTable[AGC_MAX_GAIN_LEVEL];
    UINT32     ulAGCNbGainTable[AGC_MAX_GAIN_LEVEL];
    UINT32     ulRFIdxTable[2][AGC_MAX_GAIN_LEVEL];
    INT16      sRFErrorTable[2][AGC_MAX_GAIN_LEVEL];
}NV_AGC_BAND_GAIN_TABLE_STRU;

typedef struct
{
    UINT16      usRxCalibrFreqList[RF_RX_FREQ_NUM];            /*RxƵ��У׼��*/

    INT16   	asAgcNoBlkAnt1FreqComp[8][RF_RX_FREQ_NUM];
    INT16     	asAgcNoBlkAnt2FreqComp[8][RF_RX_FREQ_NUM];
    INT16     	asAgcBlkAnt1FreqComp[8][RF_RX_FREQ_NUM];       /*����table*/
    INT16     	asAgcBlkAnt2FreqComp[8][RF_RX_FREQ_NUM];
    INT16     	asRxTempCmpVal[32];
    INT16       asIP2CalChanList[8];
    INT16       asIP2CalResult[8][2];
    NV_IIP2_CAL_TABLE stIP2CalResult;
    UINT16      usIP2CalThreshold;
    UINT16      usRsv2;
    NV_AGC_BAND_GAIN_TABLE_STRU           stAGCTable;        /*agc NV�� */
    RF_DCOC_CAL_STRU stAnt0Dcoc;
    RF_DCOC_CAL_STRU stAnt1Dcoc;
}NV_BAND_RXPATH_PARA_STRU;

typedef struct{
    UINT8 ucRxBtLeverMapTable[16];
}NV_RX_BT_LEVEL_MAP_TABLE_STRU;

typedef struct
{
    UINT16                      usRegionAVal;
    UINT16                      usRegionBVal;
    UINT16                      usRegionCVal;
    UINT16                      usReserved;
}UL_AMPR_STRU;

typedef struct
{
	INT16  sWireLessMaxPwr;                   /*����״̬�����*/
	INT16  sWiredMaxPwr;                      /*����״̬�����*/
	UINT16 usTestModeInd;                     /*��SAR����ģʽָʾ*/
	INT16  sReducedPwr[9];                    /*��SAR���˹���*/
}NV_ANT_SAR_STRU;
typedef struct
{
    INT16  sDivLossComp;
    UINT16 usRes;
}LTE_TX_DIVERSITY_LOSS_COMP;
#if 0
typedef struct
{
	INT16 sTx1RbAdjVal;                      /*1RB����ʱ����ֵ 1/8dbΪ��λ*/
	INT16 sRsv[15];
}NV_TX_CMP_RSV_STRU;
#endif
/* BEGIN: Added by xueqiuyan, 2013/1/4   PN:ONE_RB*/
typedef struct
{
    UINT16 usWirelessOneRbMprVal;
    UINT16 usWiredOneRbMprVal;
    UINT16 usRsv[4];
}UL_ONE_RB_MPR_STRU;
/* END:   Added by xueqiuyan, 2013/1/4 */

typedef struct
{
    NV_PA_LEVEL_THRE_STRU stPaThre;          /*PA��λ����*/
    UINT16 usTxCalibrFreqList[RF_TX_FREQ_NUM];           /*TXУ׼Ƶ�㣬�ɵ͵��߷���*/
    INT16 asTxMaxPwr[4];                     /*У׼max powerֵ*/
    INT16 asPaHighCalPwr[4];                 /*PA�ߵ�У׼����ֵ*/
    INT16 asPaMidCalPwr[4];                  /*PA�е�У׼����ֵ*/
    INT16 asPaLowCalPwr[10];                 /*PA�͵�У׼����ֵ*/
    INT16 asTxFreqCmpHighVal[4][16];         /*PA�ߵ�����ֵ 1/8dbΪ��λ*/
    INT16 asTxFreqCmpMidVal[4][16];          /*PA�е�����ֵ*/
    INT16 asTxFreqCmpLowVal[10][16];         /*PA�͵�����ֵ*/
    INT16 asTxTempCmpVal[16][32];  /*tx�²�*/
    UL_APT_PARA_STRU                   stAptPara;        /*APT����ʹ�ܿ���*/
    UL_APT_GAIN_VS_VBIAS_STRU          stPaHighPdmTbl;   /*APT PDM�ߵ�λ������*/
    UL_APT_GAIN_VS_VBIAS_STRU          stPaMidPdmTbl;    /*APT PDM�п�����*/
    UL_APT_GAIN_VS_VBIAS_STRU          stPaLowPdmTbl;    /*APT PDM�Ϳ�����*/
    /*MPR begin*/
    UINT16 ausMprVal[4];
    /*MPR end*/
    UL_AMPR_STRU astAMprVal[LPHY_MAX_BANDWIDTH_NUM];
    /* END:   Added by xueqiuyan, 2012/5/17 */
    UINT32 ulAmprValNS05;
    UINT16 usAmprValNS09[2];
    /* END:   Added by xueqiuyan, 2012/10/10 */
    /* BEGIN: Added by xueqiuyan, 2013/1/4   PN:ONE_RB*/
    UL_ONE_RB_MPR_STRU stOneRbMprVal;
    /* END:   Added by xueqiuyan, 2013/1/4 */
    INT16  asAptPaHighCmp[UL_APT_VBIAS_NUM];             /*APT PA���油��*/
    INT16  asAptPaMidCmp[UL_APT_VBIAS_NUM];              /*APT PA���油��*/
    INT16  asAptPaLowCmp[UL_APT_VBIAS_NUM];              /*APT PA���油��*/
	/*NV_TX_CMP_RSV_STRU   stTxCmpRsv;*/
	NV_ANT_SAR_STRU   stAntSarPwr;
    LTE_TX_DIVERSITY_LOSS_COMP stDivComp;
    NV_APC_ATT_TABLE_STRU astApcAttTable[APC_ATT_MAX_NUM];     /*˥����*/
    NV_PA_POWER_DIFFERENCE stPaPowerDiff;
    NV_TX_APC_GAIN_STRU stTxGainStatus;
    RF_TXIQ_CAL_STRU stTxiqCal;
    NV_TX_RF_BB_ATT_STRU stBbAttPara;
    NV_TX_RF_BIAS_STRU stBias;
    NV_TX_RF_FREQ_COMP_STRU stFreq;
    NV_TX_PA_TEMP_COMP stPaTempComp;
    NV_TX_ATTEN_TABLE stAttenTable;
    NV_TX_POWERDET_VOLTAGE stPowerVoltage;

}NV_BAND_TXPATH_PARA_STRU;

typedef struct
{
    UINT16                      usModemLoss[2];                        /*Modem���������߼�����*/
    UINT16                      ausTempSensor[32];                      /*PA���������⵽��ƽ̨�¶�ת��ɵ�ѹ*/
}NV_BAND_COMMON_PARA_STRU;



typedef struct
{
	UINT16    usLowRbArea;         /*���������¶˶��ٸ�RB����Ҫ����*/
	UINT16    usHighRbArea;        /*���������϶˶��ٸ�RB����Ҫ����*/
	UINT16    usRbNum;             /*С�ڶ��ٸ�RB��Ҫ����*/
	INT16     sLowPosCmpVal;       /*��RBλ�ò���ֵ*/
	INT16     sHighPosCmpVal;      /*��RBλ�ò���ֵ*/
}NV_TX_RF_ABB_FILTER_CMP_STRU;

typedef struct
{
	NV_TX_RF_ABB_FILTER_CMP_STRU stBandWidthCmp[4]; /*���5 10 15 20M������*/
}NV_TX_FILTER_CMP_STRU;


/* Begin: Added by luzhejun, 2013-09-23 PN:LTE_WIFI*/
typedef struct
{
    UINT16 				  usLWCoexEnbale;
    UINT16                usTimerAheadTx;
    UINT16				  usTimerDelayTx;
    UINT16                usTimerAheadRx;
    UINT16				  usTimerDelayRx;
    UINT16				  Reserved;	
}NV_LPHY_LWCOEX_INIT_PARA_STRU;

/* END: Added by luzhejun, 2013-09-23 PN:LTE_WIFI*/


#if 1
/*added by shijiahong PA_STAR begin*/
typedef enum
{
	MIPI_WRITE = 0x3,
	MIPI_READ
}LPHY_MIPI_CMD_TYPE_ENUM;
typedef UINT32 LPHY_MIPI_CMD_TYPE_ENUM_UINT32;

typedef enum
{
	LTE_TO_COM_RF0_SSI  = 0x2,
	LTE_TO_COM_RF1_SSI
}LPHY_MIPI_SSI_OUT_SEL_ENUM;
typedef UINT32 LPHY_MIPI_SSI_OUT_SEL_ENUM_UINT32;

typedef enum
{
	MIPI_ARB_CFG_DISABLE  = 0x0,
	MIPI_ARB_CFG_ENABLE       /*mipiʹ���ٲ�*/
}LPHY_MIPI_ARB_CFG_ENUM;
typedef UINT32 LPHY_MIPI_ARB_CFG_ENUM_UINT32;

typedef enum
{
	PA_STAR_DISABLE = 0x0,
	PA_STAR_ENABLE
}PA_STAR_SWITCH_ENUM;
typedef UINT32 PA_STAR_SWITCH_ENUM_UINT32;

typedef enum
{
	PA_STAR_POWER_OFF   = 0x0,
	PA_STAR_POWER_ON
}PA_STAR_POWER_PROC_ENUM;
typedef UINT32 PA_STAR_POWER_PROC_ENUM_UINT32;

typedef struct
{
	PA_STAR_SWITCH_ENUM_UINT32 enPaStarEn;
	UINT32 			    ulSlaveAddr;
	UINT32 				ulCmdTypeWR;
	UINT32				ulCmdTypeRD;
	UINT32              ulByteCnt;
	UINT32				ulPaStarAdvanceTime;
}LPHY_PA_STAR_MIPI_INIT_STRU;

typedef struct
{
	UINT8 ucRegAddr;    /*PA_STAR�Ĵ�����ַ*/
	UINT8 ucRegCfg;     /*PA_STAR�Ĵ�����Ӧ����ֵ*/
}LPHY_PA_STAR_CONFIG_STRU;

typedef struct
{
	UINT8 ucRegAddr;    /*PA_STAR�Ĵ�����ַ*/
	UINT8 ucPowerOn;     /*PA_STAR�Ĵ�����Ӧ����ֵ*/
	UINT8 ucPowerOff;
	UINT8 ucResv;
}LPHY_PA_STAR_POWER_PROC_STRU;


typedef struct
{
	UINT32 ulMipiCmd01;
	UINT32 ulMipiCmd02;
}LPHY_PA_STAR_MIPI_CMD_STRU;


typedef struct
{
	LPHY_PA_STAR_CONFIG_STRU stBuck0InitPara;
	LPHY_PA_STAR_CONFIG_STRU stBuck1InitPara;
	LPHY_PA_STAR_CONFIG_STRU stBuck2InitPara;
	LPHY_PA_STAR_CONFIG_STRU stLDO1InitPara;
	LPHY_PA_STAR_CONFIG_STRU stLDO2InitPara;
	LPHY_PA_STAR_CONFIG_STRU stResv;
	UINT32					 ulResvNum;
	LPHY_PA_STAR_CONFIG_STRU stInitParaResv[12];
}LPHY_PA_STAR_INIT_STRU;


typedef struct
{
	LPHY_PA_STAR_POWER_PROC_STRU stBuck0PowerProc;
	LPHY_PA_STAR_POWER_PROC_STRU stBuck1PowerProc;
	LPHY_PA_STAR_POWER_PROC_STRU stBuck2PowerProc;
	LPHY_PA_STAR_POWER_PROC_STRU stLDO1PowerProc;
	LPHY_PA_STAR_POWER_PROC_STRU stLDO2PowerProc;
	LPHY_PA_STAR_POWER_PROC_STRU stPowerResv[8];
}LPHY_PA_STAR_CTRL_STRU;

typedef struct
{
	LPHY_PA_STAR_MIPI_INIT_STRU stMipiInit;
	LPHY_PA_STAR_INIT_STRU      stPaStarInit;
	LPHY_PA_STAR_CTRL_STRU      stPaStarCtrl;
}NV_LPHY_PASTAR_NV_PARA_STRU;

/*added by shijiahong PA_STAR end*/
#endif

/* add for dcxo by l62099 */
typedef struct
{
    UINT16                          usXoType;
    UINT16                          usDcxoFeatureEnable;
}DCXO_NV_ENABLE_STRU;

typedef struct
{
    INT16                           sPaTempTemp;                           /* �¶�:0.1�� */
    UINT16                          usPaTempVol;                           /* �����������ѹ */
}DCXO_NV_TEMP_ADC_STRU;

typedef struct
{
    DCXO_NV_TEMP_ADC_STRU           astXoTempAdc[32];
}DCXO_NV_TEMP_ADC_TABLE_STRU;

typedef struct
{
    /* A3 A2 A1 A0 ϵ�� */
    UINT32                          aulMantissa[4];
    UINT16                          ausExponent[4];
}DCXO_NV_TEMP_COEF_TABLE_STRU;

typedef struct
{
    UINT16                          usDefaultAfcValue;
    UINT16                          usAfcCfix;
    UINT16                          usAfcCvar;
    UINT16                          usAfcIadj;
}DCXO_NV_TEMP_AFC_PARA_STRU;
/* add for dcxo by l62099 end */

typedef struct
{
    INT16                           asBandWithFixCmp[4];  /*У׼ʹ��1.4M�����������������1.4�Ĳ���ֵ*/
    INT16                           sCmpValRsv0;
    INT16                           sCmpValRsv1;
    INT16                           sCmpValRsv2;
    INT16                           sCmpValRsv3;
}NV_LPHY_TX_CMP_STRU;
/*****************************************band�޹ص�nv��*********************************/
typedef struct
{
    NV_TIMING_PARA_STRU         stTiming;           /*��ʱNV��*/
    NV_EMU_FAKECELL_PARA_STRU   stEmuFakeCell;      /*��������С����ʱʧЧ�о�*/
    NV_CQI_PARA_STRU            stCqi;              /*CQI NV��*/
    NV_ANTCORR_PARA_STRU        stCorr;             /*��� NV��*/
    NV_RLM_PARA_STRU            stRLM;              /*RLM�����*/
    AFC_NV_PARA_STRU            stAFC;              /*AFC NV��*/
    NV_IRC_PUB_STRU             stIrcPub;
    NV_CHE_INTP_STRU            stCheIntp;
    NV_VITERBI_PARA_STRU        stViterbi;
    NV_TURBO_PARA_STRU          stTurbo;
    NV_DEM_LIST_STRU            stDEM;
    NV_LPHY_AGC_BASE_TABLE_STRU stAgcBase;          /*AGC��׼��*/
    NV_UL_PARA_STRU             stUl;
    NV_AGC_PARA_STRU            stAGC;                         /*agc NV�� */
    NV_VERSION_STRU             stVersion;          /*NV �汾��Ϣ*/

    NV_LPHY_DSP_CONFIG          stDspCfg;           /*DSP������ʼ��������������NV*/
    NV_GULPHY_COMMON_PARA_CONFIG stGULCommParaCfg;  /*GUL��ģ������������NV�ṹ��*/

    LTE_TCXO_DYNAMIC_CONFIG_STRU    stVctcxoDynamicConfig;
    UINT16 	usVctcxoInit;	                        /*Vco��ֵ*/
    UINT16      usRsv1;

    NV_BT_CMP_SWITCH_STRU stBtCmpSwitch;

    /*NV_APC_ATT_TABLE_STRU astApcAttTable[APC_ATT_MAX_NUM];*/     /*˥����*/


	NV_DSP_CONFIG_TIME_STRU      stRFECfgTime;      /*DSP�������ʱ������NV����*/
   /* BEGIN: Added by shijiahong, 2011/12/23   PN:DTS201112302952*/
   NV_LPHY_RFIC_INITIAL_CFG_STRU           stNvLteRFInitialCfg;  /*Hi6360 RF��ʼ������*/
   NV_LPHY_LINECTRL_COMN_INFO_STRU         stNvLteLineCtrlComn;  /*���������߿�����*/
   NV_LPHY_LINECTRL_PRIV_INFO_STRU         stNvLteLineCtrlPriv;  /*BAND����߿�����*/

    /*NV_LPHY_RF_LINECTRL_INFO_STRU         stLphyRFLineCtrlCfgPara;*/
   /* END:   Added by shijiahong, 2011/12/23   PN:DTS201112302952*/
    /* BEGIN: Added by luliwu,  2012/2/3  PN: DTS_TISTISTIS */
    NV_PHY_FUNC_VERIFY_STUB_STRU       stPhyFuncDebugPara;
    /* END:   Added by luliwu,  2012/2/3  PN: DTS_TISTISTIS */
    NV_RX_BT_LEVEL_MAP_TABLE_STRU stRxBtMapTable;

    VOS_UINT32                    ulSleepAddr;
	NV_TX_FILTER_CMP_STRU   stTxFilterCmp;

	/*added by shijiahong PA_STAR begin*/
	NV_LPHY_PASTAR_NV_PARA_STRU stPaStarCfg;
	/*added by shijiahong PA_STAR end*/

    /* BEGIN: Added by w00180842, 2013/8/19   PN:tuner*/
	NV_LPHY_TUNER_NV_PARA_STRU stTunerPara;
    /* END:   Added by w00180842, 2013/8/19 */

    LTE_DRX_NV_PARA_STRU stLteDrxPara;
    /* add for dcxo by l62099 */
    DCXO_NV_ENABLE_STRU             stDcxoNvEnable;

    DCXO_NV_TEMP_ADC_TABLE_STRU     stDcxoTempAdcTable;

    DCXO_NV_TEMP_COEF_TABLE_STRU    stDcxoTempCoefTable;

    DCXO_NV_TEMP_AFC_PARA_STRU      stDcxoTempAfcPara;
    /* add for dcxo by l62099 end */
 
    /* Begin: Added by luzhejun, 2013-09-23 PN:LTE_WIFI*/ 
    
    NV_LPHY_LWCOEX_INIT_PARA_STRU  stLWCoexInitPara;
    /* END: Added by luzhejun, 2013-09-23 PN:LTE_WIFI*/

    NV_LPHY_TX_CMP_STRU   stTxCmpPara;
}LTE_NV_PARA_STRU;
/**********************************************************************************************/
/*******************************band��ص�nv��*******************************************/
typedef struct
{
    NV_BAND_COMMON_PARA_STRU         stCommon;
    NV_BAND_RXPATH_PARA_STRU         stRxPath;
    NV_BAND_TXPATH_PARA_STRU         stTxPath;
}LTE_BAND_NV_PARA_STRU;
/**********************************************************************************************/

/*����ȫ�ֱ���,������ģ�����*/
extern LTE_NV_PARA_STRU  *gptrLteNvPara;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LTE_NV_PARA_H__ */
