/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : cmu_om_def.h
  �� �� ��   : ����
  ��    ��   : w54192
  ��������   : 2011��03��29��
  ����޸�   :
  ��������   : ���幫������ģ��OM��Ҫ�ϱ��Ľṹ��,���ļ����ջύ����HiStudioʹ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��03��29��
    ��    ��   : wukeshou(54192)
    �޸�����   : �����ļ�

******************************************************************************/

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
#include "om_base_def.h"

/************************************************************
                               �궨��
************************************************************/
#define CMU_SB_MAX                 25

/************************************************************
                             ���ݽṹ����
************************************************************/

/************************************************************
                             �ӿں�������
 ************************************************************/

#ifndef __CMU_OM_DEF_H__
#define __CMU_OM_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/************************************************************
                           1. REQ����Ͷ�Ӧ�����ݽṹ����
************************************************************/
typedef enum __LPHY_REQ_MSGID_CMU__
{
    LPHY_REQ_CMU_RESERVED            = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_REQ, 0x0),
    LPHY_REQ_CMU_ENA_ALG,              /*ʹ�ܹ�����������ϱ�*/
    LPHY_REQ_CMU_ENA_CFG,              /*���ù����������㷨����*/
}LPHY_REQ_MSGID_CMU_ENUM;
typedef UINT16 LPHY_REQ_MSGID_CMU_ENUM_UINT16;

typedef struct _CMU_ALG_CTRL
{
    UINT16 ebTimingLimitFlag;                    /* ʱƫ����ģ�����޷�ģ���Ƿ��               ��TRUE-�򿪣�FALSE-�ر�*/
    UINT16 ebDrxAlphaFilterFlag;                 /* DRX̬���ŵ��ܹ���/����Alpha�˲����㿪��      ��TRUE-�򿪣�FALSE-�ر�*/
    UINT16 ebDrxTimingFilterFlag;                /* DRX̬��ʱƫ�˲����㿪��                      ��TRUE-�򿪣�FALSE-�ر�*/
    UINT16 ebConnectAlphaFilterFlag;             /* CONNECT̬���ŵ��ܹ���/����Alpha�˲����㿪��  ��TRUE-�򿪣�FALSE-�ر�*/
}CMU_ALG_CTRL_STRU;

/* �㷨��־�ṹ*/
typedef struct _CMU_ALG_CTRL_REQ
{
    LPHY_REQ_MSGID_CMU_ENUM_UINT16   usReqType;  /* ��������*/
    UINT16                           ulReaserve;
    CMU_ALG_CTRL_STRU                stAlg_Ctrl;
}CMU_ALG_CTRL_REQ_STRU;

/* �㷨���ݽṹ*/
typedef struct _CMU_ALG_RESULT_REQ
{
    LPHY_REQ_MSGID_CMU_ENUM_UINT16   usReqType;     /* ��������*/
    UINT16                           ebReportFlag;  /* 0 - ���ϱ���1-�ϱ�*/
}CMU_ALG_RESULT_REQ_STRU;

#if 0
/************************************************************
                           2. CNF���ݽṹ����
************************************************************/
typedef enum __LPHY_CNF_MSGID_CMU__
{
    LPHY_CNF_CMU_RESERVED            = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_CNF, 0x0),
    LPHY_CNF_CMU_ENA_ALG,              /*ʹ�ܹ�����������ϱ�*/
    LPHY_CNF_CMU_ENA_CFG,              /*���ù����������㷨����*/
}LPHY_CNF_MSGID_CMU_ENUM;
typedef UINT16 LPHY_CNF_MSGID_CMU_ENUM_UINT16;

/* REQʱ���ص�CNF�ṹ*/
typedef struct _CMU_CNF_REQ
{
    LPHY_CNF_MSGID_CMU_ENUM_UINT16   usCnfType;  /* ��������*/
    UINT16                           ulReaserve;
}CMU_CNF_STRU;

#endif

/************************************************************
                           3. Ind���ݽṹ����
************************************************************/
typedef enum __LPHY_SG_MSGID_CMU__
{
    LPHY_IND_CMU_RESERVED            = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_SG, 0x0),
    LPHY_SG_CMU_ENA_ALG,              /*ʹ�ܹ�����������ϱ�*/
}LPHY_SG_MSGID_CMU_ENUM;
typedef UINT16 LPHY_SG_MSGID_CMU_ENUM_UINT16;

/* �����㷨���*/
typedef struct
{
    UINT32 ulBwNoise_Rs_Raw_R0;                  /* ��������0��CRS����λ�������*/
    UINT32 ulBwNoise_Rs_Raw_R1;                  /* ��������1��CRS����λ�������*/
    INT16  sSinr_Raw_R0;                         /* ��������0�Ĳ���λSINR*/
    INT16  sSinr_Raw_R1;                         /* ��������1�Ĳ���λSINR*/
    UINT32 ulRsrp_Raw_R0;                        /* ��������0�Ĳ���λRSRP*/
    UINT32 ulRsrp_Raw_R1;                        /* ��������1�Ĳ���λRSRP*/
    UINT32 ulBwAvgRsrp_Shift;                    /* ������ƽ�������λRSRP*/
    UINT32 ulShiftedBitsNum;                     /* ���Ʊ�����*/
    UINT32 ulBwAvgNoise_Rs_Shift;                /* ������ƽ�������λ�����Ƶ����*/
    UINT16 usSBwAvgNoise_Rs_Shift[ CMU_SB_MAX ]; /* ������ƽ�������λ�Ӵ���Ƶ����,�� 0 ��ʾ��Чֵ*/
}CMU_RESULT_SG_STRU;

/************************************************************
                           4. TRACE���ݽṹ����
************************************************************/
typedef enum __LPHY_TRACE_CMU_
{
    LPHY_TRACE_CMU_RESERVED              = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_TRACE, 0x0),
	LPHY_TRACE_CMU_STATE,                        /* ����CMU��״̬*/
	LPHY_TRACE_CMU_CFG,                          /* ����CMU�����ù���*/
	LPHY_TRACE_CMU_RESULT,                       /* ����CMU�ļ������*/
	LPHY_TRACE_CMU_CELL,                         /* ����CMU��С��״��*/
	LPHY_TRACE_CMU_SFNUM,                        /* ����CMU����֡��*/
}LPHY_TRACE_CMU_ENUM;
typedef UINT16 LPHY_TRACE_CMU_ENUM_UINT16;


typedef struct
{
    UINT32      ulLineNum;
    UINT32      ulCmuState;
}LPHY_TRACE_CMU_STATE_STRU;

typedef struct
{
    UINT32      ulLineNum;
    UINT32      ulCmuCell;
}LPHY_TRACE_CMU_CELL_STRU;
typedef enum
{
    CMU_CFG_IDLE = 0,
    CMU_CFG_WAIT,
    CMU_CFG_UPLINK,
    CMU_CFG_MAXTAU,
    CMU_CFG_ALL,
    CMU_CFG_SERVCELL,
    CMU_CFG_ONLYTAU,
}CMU_CONFIG_ENUM;

typedef struct
{
    CMU_CONFIG_ENUM      enCmuCfg;
}LPHY_TRACE_CMU_CFG_STRU;

typedef struct
{
    UINT32      ulLineNum;
    UINT32      ulCmuResult;
}LPHY_TRACE_CMU_RESULT_STRU;

typedef struct
{
    UINT32      ulLineNum;
    UINT32      ulCmuSFNum;
}LPHY_TRACE_CMU_SFNUM_STRU;


/************************************************************
                           5. ERROR���ݽṹ����
************************************************************/
typedef enum __LPHY_ERROR_CMU_ENUM__
{
    /*PBCH, ERROR ID 1 - 15*/
    LPHY_ERROR_CMU_LOG_ERROR              = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_ERROR, 0x1),
    LPHY_ERROR_CMU_EXP_ERROR,
    LPHY_ERROR_CMU_DMA_ERROR
}LPHY_ERROR_CMU_ENUM;
typedef UINT16 LPHY_ERROR_CMU_ENUM_UINT16;

typedef struct
{
    UINT32      ulErrorType;
    INT32       lErrorValue;
}LPHY_TRACE_CMU_ERROR_STRU;



/************************************************************
                           6. DEBUG���ݽṹ����
************************************************************/
typedef enum __LPHY_DEBUG_CMU_ENUM__
{
    LPHY_DEBUG_CMU_RESERVED                         = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_DEBUG, 0x0),
    LPHY_DEBUG_CMU_LOGIC_REG,
    LPHY_DEBUG_CMU_BUFFER_VALUE,
    LPHY_DEBUG_CMU_CURRCAL_VALUE,
}LPHY_DEBUG_CMU_ENUM;
typedef UINT16 LPHY_DEBUG_CMU_ENUM_UINT16;

/* BEGIN: Added by xueqiuyan, 2012/9/28   PN:CMCC_DT*/
/************************************************************
                           7. DT���ݽṹ����
************************************************************/
typedef enum
{
    LPHY_DT_MEAS_INFO_REQ              = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_DT, 0x1)

}LPHY_DT_REQ_CMU_ENUM;

typedef struct __DT_MEAS_INFO_REQ_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   usEna;
    UINT16   usReportPeriod;
} LPHY_DT_MEAS_INFO_REQ_STRU;


typedef enum
{
    LPHY_DT_MEAS_INFO_IND              = OM_CMD_ID(LPHY_CMU_MID, OM_TYPE_DT, 0x1001)

}LPHY_DT_IND_CMU_ENUM;

typedef struct
{
    LTE_CP_TYPE_ENUM_UINT16 enCpType;
    INT16  sCRS_SinrR0;
    INT16  sCRS_SinrR1;
    INT16  sCRS_AvgSinr;
    INT32  lCRS_Rsrq;
    INT32  lCRS_RsrpR0;
    INT32  lCRS_RsrpR1;
    INT32  lDRS_RsrpR0;
    INT32  lDRS_RsrpR1;
    INT32  lCRS_RssiR0;
    INT32  lCRS_RssiR1;
} LPHY_DT_MEAS_INFO_IND_STRU;


/* END:   Added by xueqiuyan, 2012/9/28 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CMU_OM_DEF_H__ */
