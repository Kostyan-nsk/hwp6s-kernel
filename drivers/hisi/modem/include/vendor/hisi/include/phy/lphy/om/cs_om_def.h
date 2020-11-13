

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

#ifndef __CS_OM_DEF_H__
#define __CS_OM_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*#include "../interface/cs_interface.h"*/
#include "csu_om_def.h"



#define CS_INVALID_UINT16  (UINT16)(65535)

#define LRRC_LPHY_MAX_CANDIDATE_CELL_NUM 32

#define CS_CL_CELL_BUFFER_SIZE     200/*С�����ݻ���Ŀռ��С*/
#define CS_CL_NUM                  6  /*�б�����*/
#define CS_CL_CELL_BUFFER_SPECIAL  (CS_CL_NUM + 1)  /*ÿ���б��Ӧ�Ļ�׼С����һ������С��*/
#define CS_CL_CELL_LIST_SIZE       32 /*С���б�����С����*/
#define CS_CL_ICELL_LIST_SIZE      4 /*����С���б�����С����*/
#define CS_CL_CELL_LIFETIME_MAX    18 /*С���б���С���������*/
#define CS_CL_ICELL_LIFETIME_MAX   9  /*����С���������*/
#define CS_CL_STEP2RATIO_IC_H      32
#define CS_CL_STEP2RATIO_IC        24

#define CS_BS_BAND0_NUM_MAX   LRRC_LPHY_MAX_SCAN_FREQ_NUM/*Ƶ��Ԥ����֧�ֵ����Ƶ����Ŀ*/
#define CS_BS_BAND1_NUM_MAX   LRRC_LPHY_MAX_SCAN_FREQ_NUM/*Ƶ������֧�ֵ����Ƶ����Ŀ*/
#define CS_BS_BAND_NUM_MAX  2*LRRC_LPHY_MAX_SCAN_FREQ_NUM/*�������������ֵ*/

#define CS_OFFSET_BETWEEN_CFG_AND_UPDT 3/*��λΪ��֡*/
/*�ṹ�嶨��*/
/*//////////////////////////////////////////////////////////////////////*/
typedef struct 
{
	UINT16 usPrePtr;/*ǰ��ָ��*/
	UINT16 usNxtPtr;/*����ָ��*/
} CS_QLINK_STRU;

typedef struct  _CS_QUEUE_STRU
{
	CS_QLINK_STRU  *pstQlink;
	UINT16 usHptr;/*ͷָ������*/
	UINT16 usTptr;/*βָ������*/
	UINT16 usLen;/*���г���*/
	UINT16 usReserved;
} CS_QUEUE_STRU;

typedef struct
{
	UINT16 usAge;            /*��С��������*/
	UINT16 usUpdateAge;      /*С����������*/
	UINT16 usBaseCellAge;    /*��С����Ϊ��׼С��������*/
	UINT16 usOverBaseFlag;   /*������׼С����ǣ�1--������0--������*/
	UINT16 usOverBaseAcc;    /*��С������������׼С������*/
	
	UINT16 usCellID;         /*9u*/
	UINT16 usPos;            /*14u*/
	UINT16 usCP;             /*1u*/
	UINT16 usSFType;         /*1u*/
	UINT16 usStep2Ratio;     /*16u*/
	UINT16 usStep1Ratio;     /*10u*/
	INT16  sFFO;             /*16s*/
	INT32  lStep2Ang;	     /*17s*/
	UINT16 usStep2SyncPower; /*�߼����13u,��ҪDSPת��Ϊ15s*/
	UINT16 usSegOddEven;     /*��⵽��С����ʱ���õ��ĵ�һ���������ݶε���ż����*/
	UINT32 ulIto;            /*32u,֡ͬ��λ��,����ʱ��*/
	UINT16 usStep2AccNum;
	INT16  sRsrp;
	INT16  sRsrq;
	UINT16 usReserved;
}CS_CL_CELL_DATA_STRU;

typedef struct  
{
	CS_QUEUE_STRU         *pstQueue;/**/
	CS_QUEUE_STRU         *pstIcQueue;/**/
	CS_CL_CELL_DATA_STRU  *pstCellData;/*ָ��С����Ϣ�洢�ռ�Ŀ�ʼλ��*/
	UINT16                usFreq;   /*��С�����϶�Ӧ��Ƶ�㣬freelist������Ч*/
	UINT16                usAge;    /*��С�����ϵ����������ÿˢ��һ�Σ��������㣬freelist������Ч*/
	UINT16                usUnderBaseCellAcc;/*��б�����ǿ������δ����׼С�������ۼƼ���*/
	UINT16                usBaseCellVld;/*��Ƶ���Ӧ�Ļ�׼С����Ч���*/
	UINT16                usBaseCellIdx;/*��Ƶ���Ӧ�Ļ�׼С������*/
	UINT16                usBaseCellInvalidCnt;/*��Ƶ���޻�׼С���������ѵ���׼С������0��δ�ѵ�����*/
	CS_CL_CELL_DATA_STRU  *pstBaseCell;
} CS_CL_CELL_LIST_STRU;

#define CS_OMEGA_SIZE_MAX  32
typedef struct 
{
	CS_CL_CELL_LIST_STRU *pstClist;       /*С���б�ָ��*/
	UINT16 usCellNum;                     /*������С����Ŀ�����ֵCS_OMEGA_SIZE_MAX*/
	UINT16 usCellIdx[CS_OMEGA_SIZE_MAX];  /*С��������*/
} CS_OMEGA_STRU;

typedef struct 
{
	UINT16 usFreq;/*Ƶ��*/
	UINT16 usNum;/*С������*/
	UINT16 usCellID[8];/*С��ID*/
	UINT32 ulIto[8];/*С��λ�ã�1/4TS����*/
	UINT16 usCp[8];/*С��CP����	*/
	UINT16 usIndex[8];/*С�������������������С�����ݴ洢λ��*/

	INT16  sRsrp[8];/*RSRP*/
	INT16  sRsrq[8];/*RSRQ*/
	UINT16 usAck;/*RSRP����ֵ��Ч���	*/
	UINT16 usReserved;

	UINT16 usCsType;/*CS_TYPE_INITIAL_NBNC,CS_TYPE_INITIAL_BAR,CS_TYPE_INITIAL_CAND*/
	UINT16 usStep2Th;
	CS_OMEGA_STRU stCS_OmegaCurve;
	CS_OMEGA_STRU stCS_OmegaCurveN;
	CS_OMEGA_STRU stCS_OmegaN;
	CS_OMEGA_STRU stCS_OmegaBarN;
	CS_OMEGA_STRU stCS_OmegaCurveRsrp;

} CS_RSRP_REQ_ACK_STRU;
/*Ƶ������ڵ����ݽṹ*/
typedef struct
{
	INT32   lRssi;
	UINT16  usLowBand;
	UINT16  usHighBand;
} CS_BAND_SCAN_NOD_STRU;
/*Ƶ������ṹ*/
typedef struct
{
	CS_QUEUE_STRU             *pstQueue;
	CS_BAND_SCAN_NOD_STRU     *pstBandScanNod;
} CS_BAND_SCAN_LIST_STRU;
/*������ͨ���Ľӿڣ���ģ��ͨ������ͨ��������ÿ��Ƶ���RSSI*/
/*typedef struct*/
/*{*/
/*	INT32                    lRssiValid;//�����źţ�RSSI���ɺ���Ч��CS��ȡ������*/
/*	INT32                    lRssi[5];*/
/*	INT32                    lAagcGain[5];*/ /*AAGC����*/
/*}CS_BAND_SCAN_RSSI_RESULT_STRU;*/

typedef struct
{
	INT32                    lRssiValid;/*�����źţ�RSSI���ɺ���Ч��CS��ȡ������*/
	INT32                    lRssi;
}CS_BAND_SCAN_RSSI_RESULT_STRU;


typedef struct
{
	UINT16                    usRssiEn;/*�����źţ�RSSI���ɺ���Ч��CS��ȡ������*/
	UINT16                    usFreq;
	UINT16                    usBandWidth;/*AAGC����*/
	UINT16                    usReserved;
}CS_BAND_SCAN_RSSI_CMD_STRU;

/*ԭ�ﴦ��ģ��ӿ�*/
typedef struct
{
	/*UINT16                               usEn;*/
	UINT32                               ulMsgId;
	UINT16                               usOpId;
	UINT16                               enBandInd;
	LRRC_LPHY_BAND_SCAN_TYPE_ENUM_UINT16 usBandScanType;
	UINT16                               usLowBand;
	UINT16                               usHighBand;
	UINT16                               usFddTdd;
}CS_BAND_SCAN_REQ_STRU;

typedef struct
{
	UINT32                      ulMsgId;
	UINT16                      usOpId;
	UINT16                      enBandInd;
	UINT16                      usState;
	UINT16                      usCnt;
	UINT16                      usBsRptNum;
	UINT16                      usBandNum; /*Ƶ�θ���*/
	UINT16                      usCenterFreq[CS_BS_BAND_NUM_MAX];/*Ƶ�ε�����Ƶ��*/
	UINT16                      usLowFreq[CS_BS_BAND_NUM_MAX];/*Ƶ�ε���Ƶ��*/
	UINT16                      usHighFreq[CS_BS_BAND_NUM_MAX];/*Ƶ�ε���Ƶ��*/
	UINT16                      usFddTdd;
}CS_BAND_SCAN_CTRL_STRU;
/*//////////////////////////////////////////////////////////////////////*/

typedef struct  {
	/*RRC IDLE̬����С���б���²���*/
	UINT16                usDelta1Home       ;/*= 32*/
	UINT16                usDelta1Intra      ;/*= 32*/
	UINT16                usDelta1Inter      ;/*= 32*/

	UINT16                usDelta2Home       ;/*= 5*/
	UINT16                usDelta2Intra      ;/*= 0*/
	UINT16                usDelta2Inter      ;/*= 0*/

	INT16                 sEpciHomeRrcIdle   ;/*= -5*/
	INT16                 sEpciHomeRrcConn   ;/*= -5*/
	INT16                 sEpciIntraRrcIdle  ;/*= 0*/
	INT16                 sEpciIntraRrcConn  ;/*= 0*/
	INT16                 sEpciInterRrcIdle  ;/*= 0*/
	INT16                 sEpciInterRrcConn  ;/*= 0*/

	UINT16				  usNHomeRrcIdle     ;/*= 18*/
	UINT16				  usNHomeRrcConn     ;/*= 18*/
	UINT16				  usNIntraRrcIdle    ;/*= 18*/
	UINT16				  usNIntraRrcConn    ;/*= 18*/
	UINT16				  usNInterRrcIdle    ;/*= 18*/
	UINT16				  usNInterRrcConn    ;/*= 18*/

	UINT16				  usTIntra			 ;/*= 1*/
	UINT16				  usTInter			 ;/*= 1	*/
} CS_CLIST_UPDATE_PARA;


typedef struct
{
	UINT16       usNbase1;
	UINT16       usNbase2;
	UINT16       usNbase3;
	UINT16       Step2Ratio_basecell;
}CS_CL_BASECELL_PARA_STRU;

#if 0  //V9R1_CSU to be delete
typedef struct
{
	UINT16       usBaseCellVld;
	UINT16       usBaseCellInvalidCnt;	
	UINT16       usBaseCellId;
	UINT16       usBaseCellCp;
	UINT32       ulBaseCellIto;
}CS_BASE_CELL_INFO_STRU;
#endif

typedef struct
{
	UINT16 usStart;		 /*ʹ���ź�*/
	UINT16 usFreq;		 /*Ƶ��*/
	UINT16 usIntraInter; /*0--ͬƵ��1--��Ƶ*/
	UINT16 usFddTdd;	 /*0--FDD��1--TDD*/
	UINT16 usCsDuration; /*����ʱ����ms*/
	UINT16 usEmuTa; 	 /*������ǰ����ms*/
	UINT16 usMasterMode; /*��ģģʽ��2--2G;3--3G*/
	UINT16 usMasterState;/*��ģ״̬��0--IDLE;1--CONN*/
	UINT16 usNewSearch;  /*��ģʽΪ����̬ʱ��Ч��0--ĳƵ��ĺ���������*/
						 /*1--�·����С������*/
}CS_IRAT_SLAVE_START_PARA_STRU;


/************************************************************
                           1. REQ����Ͷ�Ӧ�����ݽṹ����
************************************************************/
typedef enum __LPHY_REQ_MSGID_CS__
{
    LPHY_REQ_CS_RESERVED           = OM_CMD_ID(LPHY_CS_MID, OM_TYPE_REQ, 0x0),

	LPHY_REQ_CS_STEP1_FILTER_CFG,       /*Step1�˲�����:Step1�׶��Ƿ��˲����˲���ϵ��0��1��ֵ���˲�ϵ�������������Max_idx*/
    LPHY_REQ_CS_STEP1_PSS_CORR_SWITCH,  /*Step1�׶ο�������:Step1�׶��Ƿ񿪴�PSSCorrSwitch����������*/
    LPHY_REQ_CS_STEP1_PARA_DISP,        /*Step1�׶εĲ�����ʾ:������طֶθ����������Ƿ��˲��������˲�����������������Ƿ񿪴���*/

	LPHY_REQ_CS_STEP2_FFO_CFG,			/*step2�׶�ƵƫУ������:Step2�׶��Ƿ�����FFOУ����Ƶƫֵ��Դ*/
	LPHY_REQ_CS_STEP2_PARA_CFG, 		/*step2�׶β�������:Step2Mode��Step2�����Ƿ��С���ϡ�ͨ��0��1��Step2����ϵ��*/
	LPHY_REQ_CS_STEP2_THRESHOLD_CFG,    /*Step2����ϵ�����޸�*/
	LPHY_REQ_CS_STEP2_IC_CFG, 			/*step2�׶θ���С������:Step2�Ƿ������š���������С��������Step2Ratio_IC��Step2Ratio_IC_h*/
	LPHY_REQ_CS_STEP2_CELL_NUM_PER_SYNC,/*ÿ��Step1ͬ��λ�ü���С������*/
	LPHY_REQ_CS_STEP2_PARA_DISP, 		/*Step2�׶εĲ�����ʾ:Step2�׶������õġ��Ƿ�FFOУ����������������ʱ�Ƿ��ȥ��С���ϡ�����Step2Mode�������Ƿ������š����������ĸ���С����������������С��ID��ͬ��λ�á�Step1Ratio��Step2Ratio��*/

	LPHY_REQ_CS_CL_DELTA_CFG,		/*������delta_home1��delta_home2������delta_intra1��delta_intra2������delta_inter1��delta_inter2�����޸�*/
	LPHY_REQ_CS_CL_EPCI_RRCIDLE_CFG,/*������epsilon_home_RRCIDLE��epsilon_intra_RRCIDLE��epsilon_inter_RRCIDLE�����޸�*/
	LPHY_REQ_CS_CL_N_RRCIDLE_CFG,   /*������N_home_RRCIDLE��N_intra_RRCIDLE��N_inter_RRCIDLE�����޸�*/
	LPHY_REQ_CS_CL_EPCI_RRCCONN_CFG,/*������epsilon_home_RRCCONN��epsilon_intra_RRCCONN��epsilon_inter_RRCCONN�����޸�*/
	LPHY_REQ_CS_CL_N_RRCCONN_CFG,   /*������N_home_RRCCONN��N_intra_RRCCONN��N_inter_RRCCONN�����޸�*/
	LPHY_REQ_CS_CL_T_CFG,           /*������T_intra������T_inter�����޸�*/
	LPHY_REQ_CS_CL_DISP,            /*ͬƵ�б���Ƶ�б�ʵʱ��ʾ*/

	/*LPHY_REQ_BS_BAND_PREPRO_CFG,  */ /*Ƶ��RSSIԤ�����Ƿ�����*/
	LPHY_REQ_BS_ALG,                /*����RSSI����PSS����RSSI+PSS����*/
	LPHY_REQ_BS_RSSI_PARA,          /*ÿ����������Ƶ��RSSIͳ�ƴ����������RelativeThreshole������Ƶ��RSSI����ʱ��*/
	LPHY_REQ_BS_PSS_PARA,			/*PSS������˳��û��PSS���޵�Ƶ���Ƿ����������PSS�������ޡ�ÿ��Ƶ��PSS����ʱ��,*/
	/*LPHY_REQ_BS_PSS_DISP,			*/ /*RSSI�����㷨������������Ƶ����F��Qֵ������������Ƶ�������ǰ32��Ƶ�㼰��Ӧ�Ĺ��ʶ���ֵ*/
	/*LPHY_REQ_BS_BAND_FIRST_DISP,  */ /*BandFirst��ֵ*/


	LPHY_REQ_CS_STATISTIC_DISP, /*��ȷ�����ʡ��龯������ͳ��*/
	LPHY_REQ_CS_STATISTIC_PARA, /*ͳ�Ʋ�������*/

	LPHY_REQ_CS_HST,            /*0--�Ǹ���ģʽ��1--����ģʽ*/
	LPHY_REQ_CS_DSP_PRINTF      /*DSP��ӡ����*/
}LPHY_REQ_MSGID_CS_ENUM;
/**************************************/
typedef enum
{
    FILTER = 0,
    DONT_FILTER
}OM_REQ_CS_STEP1_FILTER_ENUM;
typedef UINT16 OM_REQ_CS_STEP1_FILTER_ENUM_UINT16;

typedef struct 
{
    OM_REQ_CS_STEP1_FILTER_ENUM_UINT16 enStep1FilterEn        ;/*Step1�˲�ʹ���ź�*/
    UINT16                             usMaxIdx               ;/*�˲�ϵ�������������    */
	UINT16                             aCS_Step1FilterCoef0[8];
	UINT16                             aCS_Step1FilterCoef1[8];
}LPHY_REQ_CS_STEP1_FILTER_CFG_STRU;
/**************************************/
typedef enum
{
    WIN_VALID = 0,
    WIN_INVALID
}OM_REQ_CS_STEP1_WIN_ENUM;
typedef UINT16 OM_REQ_CS_STEP1_WIN_ENUM_UINT16;

typedef struct 
{
    OM_REQ_CS_STEP1_WIN_ENUM_UINT16 enWinEn;
    UINT16                          usWinOffset;
	UINT16                          usWinLen;
	UINT16                          usReserved;
}LPHY_REQ_CS_STEP1_PSS_CORR_SWITCH_STRU;
/**************************************/
typedef struct 
{
    OM_REQ_ENABLE_ENUM_UINT16 enEna;
	UINT16                    usReserved;
}LPHY_REQ_CS_STEP1_PARA_DISP_EN_STRU;

/**************************************/
typedef enum
{
    FFO_INVALID = 0,
	FFO_VALID
}OM_REQ_CS_STEP1_FFO_EN_ENUM;
typedef UINT16 OM_REQ_CS_STEP1_FFO_EN_ENUM_UINT16;

typedef enum
{
    STEP1_RESULT = 0,
    DSP_CFG
}OM_REQ_CS_STEP1_FFO_SOURCE_ENUM;
typedef UINT16 OM_REQ_CS_STEP1_FFO_SOURCE_ENUM_UINT16;


typedef struct 
{
    OM_REQ_CS_STEP1_FFO_EN_ENUM_UINT16     enFfoEn;      /*ƵƫУ��ʹ��*/
    OM_REQ_CS_STEP1_FFO_SOURCE_ENUM_UINT16 usFfoAdjMod;/*Ƶƫֵ��Դ:0--Ƶƫֵʹ��step1�Ľ��;1--Ƶƫֵʹ��DSP���ý��*/
}LPHY_REQ_CS_STEP2_FFO_CFG_STRU;
/**************************************/
typedef struct 
{
    UINT16   usStep2Mode;/*1--��ʾǿ��Ϊ1; 0--��ʾ��ǿ��Ϊ1*/
    UINT16   usStep2NoiseType;/*��ʼ�Ǳ�����������1��������0--Ŀǰ������?*/
}LPHY_REQ_CS_STEP2_PARA_CFG_STRU;
/**************************************/
typedef enum
{
    INITIAL = 0,
    PERI_NGAG,
    PERI_GAG
}OM_REQ_CS_TYPE_ENUM;
typedef UINT16 OM_REQ_CS_TYPE_ENUM_UINT16;

typedef enum
{
    STEP2_MODE0 = 0,
    STEP2_MODE1
}OM_REQ_CS_STEP2MODE_ENUM;
typedef UINT16 OM_REQ_CS_STEP2MODE_ENUM_UINT16;

typedef enum
{
    NHST = 0,
    HST
}OM_REQ_CS_HST_ENUM;
typedef UINT16 OM_REQ_CS_HST_ENUM_UINT16;

typedef enum
{
    ONE_ANT = 1,
    TWO_ANTS
}OM_REQ_CS_ANT_NUM_ENUM;
typedef UINT16 OM_REQ_CS_ANT_NUM_ENUM_UINT16;

typedef enum
{
    S5MS  = 0,
	S10MS,
	S20MS,
	S30MS,
	S40MS
}OM_REQ_CS_ACC_NUM_ENUM;
typedef UINT16 OM_REQ_CS_ACC_NUM_ENUM_UINT16;

typedef struct 
{
    OM_REQ_CS_TYPE_ENUM_UINT16        enCsType;
	OM_REQ_CS_STEP2MODE_ENUM_UINT16   enCsStep2Mode;
	OM_REQ_CS_HST_ENUM_UINT16         enCsHst;
	OM_REQ_CS_ANT_NUM_ENUM_UINT16     enCsAntNum;
	OM_REQ_CS_ACC_NUM_ENUM_UINT16     enCsAccNUM;
	UINT16                            usStep2Th;		
}LPHY_REQ_CS_STEP2_THRESHOLD_CFG_STRU;
/**************************************/
typedef enum
{
    REMOVE = 0,
    DONT_REMOVE
}OM_REQ_CS_IC_REMOVE_ENUM;
typedef UINT16 OM_REQ_CS_IC_REMOVE_ENUM_UINT16;

typedef struct 
{
    OM_REQ_CS_IC_REMOVE_ENUM_UINT16 enIcRemove;
    UINT16                          usIcNum;
    UINT16                          usStep2Ratio_IC;
    UINT16                          usStep2Ratio_IC_h;	
}LPHY_REQ_CS_STEP2_IC_CFG_STRU;
/**************************************/
typedef struct 
{
    UINT16 usCellNumPerSync;/*ÿ��step1ͬ��λ��������С������*/
	UINT16 usReserved;
}LPHY_REQ_CS_STEP2_CELL_NUM_PER_SYNC_STRU;
/**************************************/
typedef struct 
{
    OM_REQ_ENABLE_ENUM_UINT16 enEna;
	UINT16                    usReserved;
}LPHY_REQ_CS_STEP2_PARA_DISP_EN_STRU;

/**************************************/
typedef struct _LPHY_REQ_CS_CL_DELTA_CFG_STRU_
{
	UINT16                usDelta1Home       ;/*= 32*/
	UINT16                usDelta1Intra      ;/*= 32*/
	UINT16                usDelta1Inter      ;/*= 32*/

	UINT16                usDelta2Home       ;/*= 5*/
	UINT16                usDelta2Intra      ;/*= 0*/
	UINT16                usDelta2Inter      ;/*= 0*/
} LPHY_REQ_CS_CL_DELTA_CFG_STRU; 

typedef struct
{
	INT16                 sEpciHomeRrcIdle   ;/*= -5*/
	INT16                 sEpciIntraRrcIdle  ;/*= 0*/
	INT16                 sEpciInterRrcIdle  ;/*= 0*/
	UINT16                usReserved;
} LPHY_REQ_CS_CL_EPCI_RRCIDLE_CFG_STRU;

typedef struct
{
	UINT16				  usNHomeRrcIdle     ;/*= 18*/
	UINT16				  usNIntraRrcIdle    ;/*= 18*/
	UINT16				  usNInterRrcIdle    ;/*= 18*/
	UINT16                usReserved;
} LPHY_REQ_CS_CL_N_RRCIDLE_CFG_STRU;

typedef struct
{
	INT16                 sEpciHomeRrcConn   ;/*= -5*/
	INT16                 sEpciIntraRrcConn  ;/*= 0*/
	INT16                 sEpciInterRrcConn  ;/*= 0*/
	UINT16                usReserved;
} LPHY_REQ_CS_CL_EPCI_RRCCONN_CFG_STRU;

typedef struct
{
	UINT16				  usNHomeRrcConn     ;/*= 18*/
	UINT16				  usNIntraRrcConn    ;/*= 18*/
	UINT16				  usNInterRrcConn    ;/*= 18*/
	UINT16                usReserved;
} LPHY_REQ_CS_CL_N_RRCCONN_CFG_STRU;

typedef struct 
{
	UINT16				  usTIntra			 ;/*= 1*/
	UINT16				  usTInter			 ;/*= 1	*/
} LPHY_REQ_CS_CL_T_CFG_STRU;
/*******�б���ʾ*******************************/
typedef struct 
{
    OM_REQ_ENABLE_ENUM_UINT16 enEna;
	UINT16 usReserved;
}LPHY_REQ_CS_CL_DISP_EN_STRU;

/********ͳ��******************************/
typedef struct 
{
    OM_REQ_ENABLE_ENUM_UINT16 enEna;
	UINT16 usReserved;
}LPHY_REQ_CS_STATISTIC_DISP_EN_STRU;

typedef struct 
{
	UINT16  usStatisticsPeri; /*ͳ�����ڣ�Ĭ��Ϊ1000������*/
	UINT16  usFreq;
	UINT16  usLegalCellsNum;
	UINT16  usLegalCellId[CS_CL_CELL_LIST_SIZE];
}LPHY_REQ_CS_STATISTIC_PARA_STRU;
/********ɨƵ******************************/
typedef enum
{
    RSSI_PSS = 0,/*����RSSI+PSS����*/
    RSSI_RSSI =1,   /*����RSSI����*/
	PSS          /*����PSS����*/
}OM_REQ_BS_ALG_ENUM;
typedef UINT16 OM_REQ_BS_ALG_ENUM_UINT16;

typedef struct 
{
	OM_REQ_BS_ALG_ENUM_UINT16 enBsAlg;
	UINT16 usReserved;
}LPHY_REQ_BS_ALG_STRU;
/**/
typedef struct 
{
	UINT16 usRssiBandWidth;    /*ÿ����������Ƶ��RSSIͳ�ƴ���*/
	INT16  usRelativeThreshold;/*�������RelativeThreshole*/
	UINT16 usRssiDuration;     /*����Ƶ��RSSI����ʱ��,ms*/
	UINT16 usReserved;
}LPHY_REQ_BS_RSSI_PARA_STRU;
/**/
typedef struct 
{
	UINT16 usOrderMode_PSS;     /*PSS������˳��0-����1-˳��*/
	UINT16 usSearchSwitch;      /*û��PSS���޵�Ƶ���Ƿ��������,0-��������1-����*/
	UINT16 usStep1Threshold_PSS;/*PSS��������*/
	UINT16 usPssDuration;       /*ÿ��Ƶ��PSS����ʱ��,ms*/
	UINT16 usPssFilter;         /*Step1Ratio�˲�ʹ��*/
	UINT16 usReserved;
}LPHY_REQ_BS_PSS_PARA_STRU;

/************************************************************
                           2. CNF���ݽṹ����
************************************************************/


/************************************************************
                           3. Ind���ݽṹ����
************************************************************/
typedef enum 
{
	LPHY_IND_CS_RESERVED         = OM_CMD_ID(LPHY_CS_MID, OM_TYPE_IND, 0x0),
	LPHY_IND_CS_STEP1_PARA_DISP,
	LPHY_IND_CS_STEP2_PARA_DISP,
	LPHY_IND_CS_SVRCELL_DISP,
	LPHY_IND_CS_CLIST0_DISP,
	LPHY_IND_CS_CLIST1_DISP,
	LPHY_IND_CS_CLIST2_DISP,
	LPHY_IND_CS_CLIST3_DISP,	
	LPHY_IND_CS_STATISTIC_DISP /*��ȷ�����ʡ��龯������ͳ��*/
}LPHY_IND_MSGID_CS_ENUM;
/**************************************/
/*LPHY_IND_CS_STATISTIC_DISP*/
typedef struct 
{
	UINT16	usPassCnt;	     /*��ȷ������*/
	UINT16	usFalseCellCnt;  /*�龯������*/
	UINT16	usStatisticsPeri;/*ͳ�����ڣ�ûusStatisticsPeri���ϱ������ͳ�ƽ��*/
	UINT16  usReserved;
}LPHY_IND_CS_STATISTIC_DISP_STRU;
/*LPHY_IND_CS_STEP1_PARA_DISP*/
typedef struct 
{
	UINT16					  usCorrNum;	   /*����طֶθ���*/
	UINT16					  usStep1FilterEna;/*�Ƿ��˲�*/
	UINT16					  usMaxIdx; 	   /*�˲����������*/
	UINT16					  usPssCorrSwitch; /*�Ƿ񿪴�*/
}LPHY_IND_CS_STEP1_PARA_DISP_STRU;
/**************************************/
/*LPHY_IND_CS_STEP2_PARA_DISP*/
typedef struct 
{
	UINT16  usFfoEn;            /*�Ƿ�FFOУ��*/
	UINT16  usStep2NoiseType;   /*��������ʱ�Ƿ��ȥ��С����*/
	UINT16  usStep2Mode;        /*Step2Mode*/
	UINT16  usIcRemove;         /*�Ƿ�������*/
	UINT16  usIcNum;            /*�����ĸ���С������*/
	UINT16  usIcID[4];          /*����С��ID*/
	UINT16  usIcStep1Ratio[4];  /*����С��Step1Ratio*/
	UINT16  usIcStep2Ratio[4];  /*����С��Step2Ratio*/
	UINT32  ulIcPos[4];         /*����С��ͬ��λ��,0.25TS*/
}LPHY_IND_CS_STEP2_PARA_DISP_STRU;
/**************************************/
/*LPHY_IND_CS_CLIST0_DISP*/
/*LPHY_IND_CS_CLIST1_DISP*/
/*LPHY_IND_CS_CLIST2_DISP*/
/*LPHY_IND_CS_CLIST3_DISP*/
typedef struct 
{
	UINT16               usFreq;   /*��С�����϶�Ӧ��Ƶ�㣬freelist������Ч*/
	UINT16               usAge;    /*��С�����ϵ����������ÿˢ��һ�Σ��������㣬freelist������Ч*/
	UINT16               usUnderBaseCellAcc;/*��б�����ǿ������δ����׼С�������ۼƼ���*/
	UINT16               usBaseCellVld;/*��Ƶ���Ӧ�Ļ�׼С����Ч���*/
	UINT16               usBaseCellInvalidCnt;/*��Ƶ���޻�׼С���������ѵ���׼С������0��δ�ѵ�����*/
	UINT16               usCellsNum;/*�����С����������ЧС������Ŀ*/
	CS_CL_CELL_DATA_STRU stBaseCell;/*��Ƶ���Ӧ�Ļ�׼С��*/
	CS_CL_CELL_DATA_STRU stCellData[CS_CL_CELL_LIST_SIZE];/*С����������*/
}LPHY_IND_CS_CL_DISP_STRU;
/**************************************/
/*LPHY_IND_CS_SVRCELL_DISP*/
typedef struct 
{
	UINT16               SvrCellVld;
	UINT16               SvrCellFreq;
	CS_CL_CELL_DATA_STRU SvrCell;
}LPHY_IND_CS_SVRCELL_DISP_STRU;
/**************************************/
/*typedef struct 
{
	LPHY_IND_CS_SVRCELL_DISP_STRU stSvrCell;
	LPHY_IND_CS_CL_DISP_STRU      stClist0;
	LPHY_IND_CS_CL_DISP_STRU      stClist1;
	LPHY_IND_CS_CL_DISP_STRU      stClist2;
	LPHY_IND_CS_CL_DISP_STRU      stClist3;
}LPHY_IND_CS_CELL_LISTS_DISP_STRU;*/

/************************************************************
                           4. SG ���ݽṹ����, �㷨��Ҫ��ͼ��
************************************************************/


/************************************************************
                           5. TRACE���ݽṹ����
************************************************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CS_OM_DEF_H__ */

