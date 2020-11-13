


#ifndef __AGC_OM_DEF_H__
#define __AGC_OM_DEF_H__


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

/************************************************************
                             ���ݽṹ����
************************************************************/

/************************************************************
                             �ӿں�������
 ************************************************************/

/************************************************************
                           1. REQ����Ͷ�Ӧ�����ݽṹ����
************************************************************/
typedef enum
{
    LPHY_REQ_AGC_REPORT_ENA         = OM_CMD_ID(LPHY_AGC_MID, OM_TYPE_REQ, 0x1),
    LPHY_REQ_RSSI_DS_FRAME_CONFIG_ENA,
    LPHY_REQ_RSSI_GAP_MODE_CONFIG_ENA,
    LPHY_REQ_RSSI_POINT_CONFIG_ENA,
    LPHY_REQ_RSSI_COMPEMSATE_CONFIG_ENA,

    LPHY_REQ_AAGC_FIXED_RF_GAIN_CONFIG_ENA,
    LPHY_REQ_AAGC_FIXED_PRE_GAIN_CONFIG_ENA,
    LPHY_REQ_AAGC_FIXED_POST_GAIN_CONFIG_ENA,
    LPHY_REQ_AAGC_ANTENNA_ADJUST_CONFIG_ENA,
    LPHY_REQ_AAGC_INTERFERENCE_CONFIG_ENA,
    LPHY_REQ_AAGC_DEM_PERIOD_CONFIG_ENA,
    LPHY_REQ_AAGC_POWER_CALC_CONST_CONFIG_ENA,
    LPHY_REQ_AAGC_PRE_TARGET_CONFIG_ENA,
    LPHY_REQ_AAGC_POST_TARGET_CONFIG_ENA,
    LPHY_REQ_AAGC_PRE_DAGC_ENA,
    LPHY_REQ_AAGC_POST_DAGC_ENA,
    LPHY_REQ_AAGC_NOISE_LOWER_BOUNDARY_CONFIG_ENA,
    LPHY_REQ_AAGC_RS_DIFF_THRE_CONFIG_ENA,
    LPHY_REQ_AAGC_TDD_BLOCK_POWER_THRE_CONFIG_ENA,
    LPHY_REQ_AAGC_TDD_BLOCK_GAIN_STEPSIZE_CONFIG_ENA,

    LPHY_REQ_DAGCF_FIXED_GAIN_CONFIG_ENA,
    LPHY_REQ_DAGCF_ADJ_BIT_CONFIG_ENA,
    LPHY_REQ_DAGCF_EXTRA_PA_CONFIG_ENA,
    LPHY_REQ_DAGCF_SHIFT_BIT_CONFIG_ENA,
    LPHY_REQ_DAGCF_FIXED_NOISE_ENA,

    LPHY_REQ_AGC_SPECIAL_SENARIO_CONFIG_ENA,
    LPHY_REQ_TRIGGER_RFIN_SAMPLING_ENA,
}LPHY_REQ_AGC_ENUM;

typedef enum
{
    AGC_OM_SWITCH_OFF = 0,
    AGC_OM_SWITCH_ON,
}AGC_OM_SWITCH_ENUM;

typedef enum
{
    DEBUG_RFIN_NO_TRIGGER = 0,
    DEBUG_RFIN_SAMPLING_ENABLE,
    DEBUG_RFIN_PBCH_FAILURE,
    DEBUG_RFIN_INTER_FREQ_START,
    DEBUG_RFIN_INTER_FREQ_FINISH,
    DEBUG_RFIN_PRE_DEFINED_SUBFRAME,
    DEBUG_RFIN_DLSCH_CRC,
    DEBUG_RFIN_BCCH_CRC,
    DEBUG_RFIN_PCH_CRC,
    DEBUG_RFIN_TIMING_ERROR,
}LPHY_DEBUG_RFIN_SAMPLING_TYPE_ENUM;
typedef UINT16 LPHY_DEBUG_RFIN_SAMPLING_TYPE_ENUM_UINT16;

typedef struct
{
    LPHY_DEBUG_RFIN_SAMPLING_TYPE_ENUM  enRfinSamplingTrigger;
    UINT32                              ulCounter;
    UINT32                              ulPara;
}LPHY_DEBUG_RFIN_SAMPLING_CTRL_STRU;
typedef struct
{
    AGC_OM_SWITCH_ENUM    enReportEn;    /*1 ʹ��AFC״̬�ϱ�*/
}LPHY_REQ_AGC_REPORT_ENA_STRU;

typedef enum
{
    AGC_USE_ALL_DS_FRAME = 0,
    AGC_USE_LAST_DS_FRAME,
    AGC_USE_LAST_0_5_DS_FRAME,
}AGC_DEM_SUBFRAME_SEL_ENUM;
typedef UINT16  AGC_DEM_SUBFRAME_SEL_ENUM_UINT16;

typedef enum
{
    AGC_GAP_FAST = 0,
    AGC_GAP_FAST_SLOW =1,
    AGC_GAP_SLOW = 2,
}AGC_GAP_MODE_SEL_ENUM;
typedef UINT16  AGC_GAP_MODE_SEL_ENUM_UINT16;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lFixedRssi;

}LPHY_REQ_RSSI_FIXED_VALUE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_DEM_SUBFRAME_SEL_ENUM     enDemSubFrameSel;
}LPHY_REQ_RSSI_DS_FRAME_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_GAP_MODE_SEL_ENUM     enGAPModeSel;
}LPHY_REQ_RSSI_GAP_MODE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulRssiPoint;
}LPHY_REQ_RSSI_POINT_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulRssiCompensate;
}LPHY_REQ_RSSI_COMPEMSATE_CONFIG_ENA_STRU;

typedef enum
{
    AGC_INDEPENDENT_ANTENNA,
    AGC_COMBINED_ANTENNA,
    AGC_NOISE_DECIDED,
    AGC_ON_AIR,
}AGC_ANT_CONFIG_ENUM;
typedef UINT16  AGC_ANT_CONFIG_ENUM_UINT16;

typedef enum
{
    AGC_NO_INTERFERENCE =0,
    AGC_ACS_INTERF_VAR,
    AGC_ACS_INTERF_CONST,
    AGC_NB_INTERF_VAR,
    AGC_NB_INTERF_CONST,
    AGC_INTERF_BUTT,
}AGC_INTERFERENCE_TYPE_ENUM;
typedef UINT16  AGC_INTERFERENCE_TYPE_ENUM_UINT16;

typedef enum
{
    AGC_DEM_PERIOD_1MS = 0,
    AGC_DEM_PERIOD_5MS,
    AGC_DEM_PERIOD_10MS,
    AGC_DEM_PERIOD_20MS,
    AGC_DEM_PERIOD_BUTT,
}AGC_DEM_MODE_SEL_ENUM;
typedef UINT16  AGC_DEM_MODE_SEL_ENUM_UINT16;
typedef enum
{
    AGC_RF_IDX0 = 0,
    AGC_RF_IDX1,
    AGC_RF_IDX2,
    AGC_RF_IDX3,
    AGC_RF_IDX4,
    AGC_RF_IDX5,
    AGC_RF_IDX6,
    AGC_RF_IDX7,
    AGC_RF_IDX8,
    AGC_RF_IDX9,
    AGC_RF_IDX10,
    AGC_RF_IDX11,
    AGC_RF_IDX12,
    AGC_RF_IDX13,
    AGC_RF_IDX14,
    AGC_RF_IDX15,
}AGC_RF_IDX_ENUM;
typedef UINT16  AGC_RF_IDX_ENUM_UINT16;
typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_RF_IDX_ENUM_UINT16       aenFixedRfIdx[LPHY_MAX_RX_ANTENNA_NUM];
}LPHY_REQ_AAGC_FIXED_RF_GAIN_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT16     asFixedPreDagcGain[LPHY_MAX_RX_ANTENNA_NUM];
}LPHY_REQ_AAGC_FIXED_PRE_GAIN_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT16     asFixedPostDagcGain[LPHY_MAX_RX_ANTENNA_NUM];
}LPHY_REQ_AAGC_FIXED_POST_GAIN_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_ANT_CONFIG_ENUM     enAntennaConfig; /* default AGC_ON_AIR*/
}LPHY_REQ_AAGC_ANTENNA_ADJUST_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_INTERFERENCE_TYPE_ENUM      enInterfConfig;
}LPHY_REQ_AAGC_INTERFERENCE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
}LPHY_REQ_AAGC_GAIN_SWITCH_DELAY_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
}LPHY_REQ_AAGC_IDX_SWITCH_DELAY_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    AGC_DEM_MODE_SEL_ENUM      enDemPeriodSel;
}LPHY_REQ_AAGC_DEM_PERIOD_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lPowerCalcConst;
}LPHY_REQ_AAGC_POWER_CALC_CONST_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lPreTarPower;
}LPHY_REQ_AAGC_PRE_TARGET_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lPostTarPower;
}LPHY_REQ_AAGC_POST_TARGET_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
}LPHY_REQ_AAGC_PRE_DAGC_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
}LPHY_REQ_AAGC_POST_DAGC_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulNoiseLimit;
}LPHY_REQ_AAGC_NOISE_LOWER_BOUNDARY_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulRsDifThre;
}LPHY_REQ_AAGC_RS_DIFF_THRE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lTddBlockPowerThresh;
}LPHY_REQ_AAGC_TDD_BLOCK_POWER_THRE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    INT32     lTddBlockGainStep;
}LPHY_REQ_AAGC_TDD_BLOCK_GAIN_STEPSIZE_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulFixedDagcFGain;
}LPHY_REQ_DAGCF_FIXED_GAIN_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulAdjBit;
}LPHY_REQ_DAGCF_ADJ_BIT_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulExtraPA;
}LPHY_REQ_DAGCF_EXTRA_PA_CONFIG_ENA_STRU;

typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulBitShift;
}LPHY_REQ_DAGCF_SHIFT_BIT_CONFIG_ENA_STRU;
typedef struct
{
    AGC_OM_SWITCH_ENUM    enEnable;
    UINT32    ulNoiseFactor;
}LPHY_REQ_DAGCF_FIXED_NOISE_ENA_STRU;
typedef struct
{
    AGC_OM_SWITCH_ENUM    enCase1Enable;
    AGC_OM_SWITCH_ENUM    enCase3Enable;
    AGC_OM_SWITCH_ENUM    enCase4Enable;
    AGC_OM_SWITCH_ENUM    enCase6Enable;
}LPHY_REQ_AGC_SPECIAL_SENARIO_CONFIG_ENA_STRU;

#if 0
/************************************************************
                           2. CNF���ݽṹ����
************************************************************/
typedef enum
{
    LPHY_CNF_AFC_FIX_PDM_ENA         = OM_CMD_ID(LPHY_AFC_MID, OM_TYPE_CNF, 0x1),

}LPHY_CNF_AFC_ENUM;

typedef struct
{
    UINT32                  ulMsgId;    /*1 CNF��ϢID*/
}LPHY_CNF_AFC_FIX_PDM_ENA_STRU;

#endif
/************************************************************
                           3. Sg���ݽṹ����
************************************************************/

typedef enum
{
    LPHY_SG_AGC_REPORT_ENA         = OM_CMD_ID(LPHY_AGC_MID, OM_TYPE_SG, 0x1),
}LPHY_SG_AGC_ENUM;

typedef struct
{
    INT16     sPreTargetPower;
    INT16     sPostTargetPower;
}AGC_TARGET_POWER_STRU;
typedef struct
{
    INT16     asPreRssidB[LPHY_MAX_RX_ANTENNA_NUM];                /*��������0/1���˲�ǰʱ��DAGC��RSSI*/
    INT16     asPreDAGCTgain[LPHY_MAX_RX_ANTENNA_NUM];         /*��������0/1���˲�ǰʱ��DAGC����*/
    INT16     asPreAntPwrdBm[LPHY_MAX_RX_ANTENNA_NUM];           /*�˲�ǰ��������1���߿ڹ���,dBm*/
    INT16     asPostRssidB[LPHY_MAX_RX_ANTENNA_NUM];              /*�˲����������0/1��ʱ��DAGC��RSSI*/
    INT16     asPostDAGCTgain[LPHY_MAX_RX_ANTENNA_NUM];       /*�˲����������0/1��ʱ��DAGC����*/
    INT16     asPostAntPwrdBm[LPHY_MAX_RX_ANTENNA_NUM];          /*�˲����������1���߿ڹ���,dBm*/
    INT16     asAAGCgain[LPHY_MAX_RX_ANTENNA_NUM];             /*��������0/1��AAGC����*/
    UINT16    ausLNAGain[LPHY_MAX_RX_ANTENNA_NUM];             /*��ǰ��������0/1��LNA��λ*/
    UINT16    ausRFGain[LPHY_MAX_RX_ANTENNA_NUM];
    UINT16    ausRFIdex[LPHY_MAX_RX_ANTENNA_NUM];
    AGC_RF_IDX_ENUM_UINT16    aenFinalIdx[LPHY_MAX_RX_ANTENNA_NUM];
    AGC_INTERFERENCE_TYPE_ENUM_UINT16    aenAAGCIdxSel[LPHY_MAX_RX_ANTENNA_NUM];          /*��������0/1����ѡ���ź�*/
    /*last Value*/
    INT16     asLastRfGain[LPHY_MAX_RX_ANTENNA_NUM];      /*��������0/1��AAGC����*/
    INT16     asLastPreAntPwrdBm[LPHY_MAX_RX_ANTENNA_NUM];      /*��������0/1��AAGC����*/

    INT16     asLastAAGCgain[LPHY_MAX_RX_ANTENNA_NUM];      /*��������0/1��AAGC����*/
    INT16     asLastPreDAGCTgain[LPHY_MAX_RX_ANTENNA_NUM];  /*�˲����������0/1��ʱ��DAGC����*/
    INT16     asLastPostDAGCTgain[LPHY_MAX_RX_ANTENNA_NUM]; /*��������0/1��AAGC����*/
    /*��ģ��������*/
     UINT8    ucFixAgcFlag;                                /*��Ƶ��̶�AGC��ʶ*/
     UINT8    ucReserved;                                     /*����ʱ��*/
    UINT16   usReserved;
    /*END*/
}AAGC_PARA_STRU;
typedef struct
{
    UINT8     ucCounter1;
    UINT8     ucCounter3;
    INT16     sDAGCFGain;              /*��ǰ��֡DAGCF������ֵ*/
    /*UINT16    usDAGCFSftBit;            //���ݷ������߸�����ϵͳ����,�����������л���Ҫ�任*/
    /*UINT16    usDAGCFPAFac;             //���ݴ���ģʽ��PA�������õ�,�ڷ���2�ж��и���*/
    INT16     sAGCNoiseFactor;         /*��������*/
    UINT16    usDAGCFTar;               /*����ADJBit�õ�,����DAGCFĿ�깦��,�ڷ���2�ж��и���*/
    UINT16    usRSREAvgdb[LPHY_MAX_RX_ANTENNA_NUM];
    UINT32    ulRSREAccPower[LPHY_MAX_RX_ANTENNA_NUM];
}DAGCF_PARA_STRU;
typedef struct
{
    UINT8     ucAgcPaValue;                /*PA ����*/
    UINT8     ucExtraPa;                   /*Extra PA*/
    UINT8     ucIndepAGCFlag;
    UINT8     ucFreqIndex;
	
    UINT8     ucMflag;
    UINT8     ucReserved;
    UINT16    usSubFrameNum;/* = SCHED_GetSubFrmNum();*/

    UINT16    usFreqInfo;
    UINT16    usMs1DagcfGain;
	
    INT16     sOutDiffPowdBm;/*PMCH��֡��֮ǰ���ڵĵ�����֡�˲������߿ڹ���֮��*/
    UINT16    usMDagcfGain;
    UINT16    usDagcfGainDiff;
    UINT16    usDagcfGainCoef;

    AAGC_PARA_STRU    stAAGCPara;
    DAGCF_PARA_STRU   stDAGCFPara;             /*��ǰ��֡DAGCF�Ĳ���*/
} LPHY_SG_AGC_REPORT_ENA_STRU;

/************************************************************
                           4. TRACE���ݽṹ����
************************************************************/
typedef enum
{
    LPHY_TRACE_AGC_STATE         = OM_CMD_ID(LPHY_AGC_MID, OM_TYPE_TRACE, 0x1),
    LPHY_TRACE_AGC_TDD_BLOCK_EVENT,
    LPHY_TRACE_AGC_SPECIAL_SENARIO_1,
    LPHY_TRACE_AGC_SPECIAL_SENARIO_3,
    LPHY_TRACE_AGC_SPECIAL_SENARIO_4,
    LPHY_TRACE_AGC_SPECIAL_SENARIO_6,
    LPHY_TRACE_AGC_FREQ_INFO,
    LPHY_TRACE_AGC_EMBMS_FRAME,
    LPHY_TRACE_AGC_BAND_INFO,
}LPHY_TRACE_AGC_ENUM;
typedef enum
{
    AGC_BAND_SCAN = 0,                                /*��ʼģʽ����ɨƵʹ�ã�RSSIͳ��ʱ��FDD1ms/TDD5ms��ʹ��ȫ����������*/
    AGC_INIT_CELL_SEARCH,                             /*��ʼС������ģʽ��RSSIͳ��ʱ��5ms��ʹ��ȫ����������*/
    AGC_DEMOD,                                        /*���ģʽ��RSSIͳ��ʹ�õ�Ƶ����*/
    AGC_GAP,                                          /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
    AGC_DRX,                                          /*DRXģʽ,TDD/FDD,FASTSLOW/SLOW��AGCģ���ڲ��ж�*/
    AGC_DIF_SYS,                                      /*��ϵͳ����,�ݲ�ʵ��,Ԥ���ӿ�*/
    AGC_IDLE,
    AGC_FIXED_GAIN0,                                  /*�̶�����*/
    AGC_FIXED_GAIN1,
    /*��ģ��������*/
    /*END*/
} AGC_WORKING_MODE_ENUM;
typedef UINT16 AGC_WORKING_MODE_ENUM_UINT16;
typedef enum
{
    AGC_INIT_MODE = 0,                                /*��ʼģʽ����ɨƵʹ�ã�RSSIͳ��ʱ��FDD1ms/TDD5ms��ʹ��ȫ����������*/

    AGC_DEMOD_1MS = 4,                                        /*���ģʽ��RSSIͳ��ʹ�õ�Ƶ����*/
    AGC_DEMOD_5MS = 5,                                        /*���ģʽ��RSSIͳ��ʹ�õ�Ƶ����*/
    AGC_DEMOD_10MS = 6,                                        /*���ģʽ��RSSIͳ��ʹ�õ�Ƶ����*/
    AGC_DEMOD_20MS = 7,                                        /*���ģʽ��RSSIͳ��ʹ�õ�Ƶ����*/

    AGC_GAP_FDD_FAST = 8,                                 /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
    AGC_GAP_FDD_FAST_SLOW = 9,                            /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
    AGC_GAP_FDD_SLOW = 10,                                 /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/

    AGC_GAP_TDD_FAST = 12,                                 /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
    AGC_GAP_TDD_FAST_SLOW = 13,                            /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
    AGC_GAP_TDD_SLOW = 14,                                 /*GAPģʽ,TDD/FDD,FAST/SLOW��AGCģ���ڲ��ж�*/
} AGC_CONFIG_MODE_ENUM;

typedef UINT16 AGC_CONFIG_MODE_ENUM_UINT16;

typedef struct
{
    LTE_BAND_WIDTH_ENUM_UINT16      enAGCRFBW;     /*RF�˲�������*/
    LTE_BAND_WIDTH_ENUM_UINT16      enAGCBBBW;     /*�����˲�������*/
} AGC_BW_CHANGE_STRU ;

typedef struct
{
    UINT16                             usAGCFreqInfo; /*AGCƵ����Ϣ*/
    UINT16                             usBand;
    LTE_FRAME_TYPE_ENUM_UINT16         enFrametype;
    AGC_WORKING_MODE_ENUM_UINT16       enAAGCMode;    /*��ǰ��֡AAGC����ģʽ*/
    UINT32                             ulBaseCellPos;
    AGC_BW_CHANGE_STRU                 stAGCBWChange; /*AGC����仯*/
} AGC_MODE_CFG_REQ_STRU;

typedef struct
{
    UINT32      ulLineNum;
    UINT16      usReserved;
    AGC_CONFIG_MODE_ENUM_UINT16      enLastConfigState;
    AGC_MODE_CFG_REQ_STRU            stAgcModeCfg;
}LPHY_TRACE_AGC_STATE_STRU;
typedef struct
{
    UINT32      ulSubframe;
}LPHY_TRACE_AGC_EMBMS_FRAME_STRU;

typedef enum
{
    AGC_ENTER_TDD_BLOCK_STATE = 0,
    AGC_TDD_BLOCK_INC_PWR,
    AGC_TDD_BLOCK_REACH_MAX_GAIN,
}AGC_TDD_BLK_EVENT_TRACE_ENUM;

typedef struct
{
    INT16     sCounter;
    UINT16    usState;
}AGC_SPECIAL_SENARION_TRACE_STRU;
typedef struct
{
    AGC_TDD_BLK_EVENT_TRACE_ENUM    enEvent;
}LPHY_TRACE_TDD_BLK_STRU;
typedef enum
{
    AGC_CONFIG_SCELL_FREQ_INFO = 0,                     /*����AGC��ƵƵ����Ϣ*/
    AGC_CONFIG_NCELL_FREQ_INFO,                         /*����AGC��ƵƵ����Ϣ*/
    AGC_SWITCH_SCELL_FREQ_INFO,                         /*�л�AGC��ƵƵ��*/
} AGC_FREQ_CMD_ENUM;
typedef UINT16 AGC_FREQ_CMD_ENUM_UINT16;
#define AGC_FREQ_NUM_MAX         (4)
typedef struct
{
    AGC_FREQ_CMD_ENUM_UINT16            enAGCFreqCmd;
    UINT16                              usServCellBand;
    UINT16                              usAGCNumOfFreq;
    UINT16                             usAGCFreqInfo[AGC_FREQ_NUM_MAX];
} AGC_FREQ_INFO_STRU;

typedef struct
{
    AGC_FREQ_INFO_STRU      stFreqInfo;
}LPHY_TRACE_FREQ_INFO_STRU;
/*band ���nv buffer*/
typedef enum
{
    AGC_SERV_CELL_NV_INDEX = 0,
    AGC_NEIGHBOR_CELL_NV_INDEX,    
}AGC_CELL_NV_TYPE_ENUM;
typedef UINT16   AGC_CELL_NV_TYPE_ENUM_UINT16;
typedef struct
{
    UINT16    usBandInd;
    AGC_CELL_NV_TYPE_ENUM_UINT16  enCellInd; 
    UINT16    usOldIndex;
    UINT16    usNewIndex;
}LPHY_TRACE_BAND_INFO_STRU;
/************************************************************
                           5. ERROR���ݽṹ����
************************************************************/
typedef enum
{
    LPHY_ERROR_AGC_ARRAY_INDEX         = OM_CMD_ID(LPHY_AGC_MID, OM_TYPE_ERROR, 0x1),
    LPHY_ERROR_AGC_CASE,
    LPHY_ERROR_AGC_FREQ_INFO,
    LPHY_ERROR_AGC_ISR_BLOCK,
    LPHY_ERROR_AGC_ZERO_DIV,
}LPHY_ERROR_AGC_ENUM;

typedef struct
{
    UINT32      ulLineNum;/*__LINE__*/
    UINT32      ulIncorrectValue;
}LPHY_ERROR_AGC_ARRAY_INDEX_STRU;

typedef struct
{
    UINT32      ulLineNum;/*__LINE__*/
    UINT32      ulCurrentCase;
    UINT32      ulLastCase;
}LPHY_ERROR_AGC_CASE_STRU;

typedef struct
{
    UINT32      ulLineNum;/*__LINE__*/
    UINT32      ulFreqInfo;
}LPHY_ERROR_AGC_FREQ_INFO_STRU;

typedef struct
{
    UINT32      ultimeout;
}LPHY_ERROR_AGC_ISR_BLOCK_STRU;
typedef struct
{
    UINT32      ulAgcMode;
}LPHY_ERROR_AGC_ISR_DELAY_STRU;

#if 0
/************************************************************
                           7. Ind���ݽṹ����
************************************************************/
typedef enum
{
    LPHY_IND_AGC_STATUS_ENA         = OM_CMD_ID(LPHY_AGC_MID, OM_TYPE_IND, 0x1),
}LPHY_IND_AGC_ENUM;

typedef struct
{
    AGC_WORKING_MODE_ENUM_UINT16    enAGCWorkingMode;                           /*��ǰAGCģʽ*/
    AGC_CONFIG_MODE_ENUM_UINT16     enAGCConfigMode;
    UINT16                          usFreqIndex;
    AAGC_FREQ_PARA                  stAAGCPara;
    DAGCF_PARA_STRU                 stDAGCFPara;             /*��ǰ��֡DAGCF�Ĳ���*/
    AGC_TDD_BLOCK_PARA              stTDDBlockPara;
}LPHY_IND_AGC_STATUS_ENA_STRU;
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AGC_OM_DEF_H__ */

