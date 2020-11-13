

#ifndef __TDS_OM_def_H__
#define __TDS_OM_def_H__
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/

/************************************************************
                               �궨��
************************************************************/
/*#define   TDS_OAM_USE_SOCP*/
#define   TDS_OAM_USE_UART  	0
#define   TDS_OAM_USE_SOCP  	1

#define   TDS_OAM_MSG_BASE              (0x70000000UL)

#define   TDS_OAM_ID_MSG01				(TDS_OAM_MSG_BASE 	+ 	0x1)			/* ��HSOԼ��MSG ID ��0x70000001 ��ʼ ����ID �����ۼ�*/

#if( 1 == ASIC_VERSION )
#define   TDPHY_32K_TIMER_BASE_ADDR        		 (U32)(0x9000060CUL)
#else
#define TDPHY_32K_TIMER_BASE_ADDR        		 (U32)(0x5016d000UL)
#endif

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 �ṹ����   : TDS_OAM_MSG_ID
 Э����   :
 ASN.1 ���� :
 �ṹ˵��   : ���岻ͬģ���ϱ���Ϣ�Ļ�ַ
*****************************************************************************/
enum TDS_OAM_MSG_ID
{
    TDS_OAM_ID_CTU_BASE                     =   ((TDS_OAM_MSG_BASE)),
    TDS_OAM_ID_ICS_BASE                     =   ((TDS_OAM_MSG_BASE) + (0x10000)),
    TDS_OAM_ID_IDL_BASE                     =   ((TDS_OAM_MSG_BASE) + (0x20000)),
    TDS_OAM_ID_ACC_BASE                     =   ((TDS_OAM_MSG_BASE) + (0x30000)),
    TDS_OAM_ID_DCH_BASE                     =   ((TDS_OAM_MSG_BASE) + (0x50000)),
    TDS_OAM_ID_IRAT_BASE                    =   ((TDS_OAM_MSG_BASE) + (0x60000)),
    TDS_OAM_ID_BUTT
};

/*****************************************************************************
 �ṹ����   : TDS_OAM_DCH_MSG_ID
 Э����   :
 ASN.1 ���� :
 �ṹ˵��   : DCHģ���������ϱ�����ϢID
*****************************************************************************/
enum TDS_OAM_DCH_MSG_ID
{
    TDS_OAM_ID_DCH_DPCH_BLER_IND            = ((TDS_OAM_ID_DCH_BASE) + 0x8001),

    TDS_OAM_ID_DCH_BUTT
};

/*OM REQö�ٶ���*/
typedef enum TPHY_DT_RPT_REQ
{
    TPHY_DT_UE_PCCPCH_MEAS_REQ = 0x70010001,
    TPHY_DT_UE_TS_INFO_REQ,
    TPHY_DT_UE_DPCH_INFO_REQ,
    TPHY_DT_UE_HS_SICH_INFO_REQ,
    TPHY_DT_UE_HSDPA_INFO_REQ  = 0x70010005,
    TPHY_DT_UE_TDS_OM_RPT_REQ  = 0x70900099
}TPHY_DT_RPT_REQ_ENUM;


/*OM �ϱ�ö�ٶ���*/
typedef enum TPHY_DT_RPT_IND
{
    TPHY_DT_UE_PCCPCH_MEAS_IND = 0x70011001,
    TPHY_DT_UE_TS_INFO_IND,
    TPHY_DT_UE_DPCH_INFO_IND,
    TPHY_DT_UE_HS_SICH_INFO_IND,
    TPHY_DT_UE_HSDPA_INFO_IND = 0x70010005,
    TPHY_DT_UE_HSDPA_HARQ_IND = 0x70100001,

}TPHY_DT_RPT_IND_ENUM;

/*OM���幫��ʹ�ܿ���*/
#if 0
typedef enum __OM_REQ_ENABLE__
{
    OM_REQ_DISABLE = 0,
    OM_REQ_ENABLE = 1,
}OM_REQ_ENABLE_ENUM;

typedef U16 OM_REQ_ENABLE_ENUM_UINT16;
#endif
typedef struct _OM_SOCP_TRACE_
{
    U16 		 UartBuff_SubFN;
    U16 		 UartBuff_STATE;
    U16 		 UartBuff_504;
    U16 		 UartBuff_MsgID;
    U16 		 UartBuffData04;
    U16 		 UartBuffData05;
    U16 		 UartBuffData06;
    U16 		 UartBuffData07;
    U16 		 UartBuffData08;
    U16 		 UartBuffData09;
    U16 		 UartBuffData10;
    U16 		 UartBuffData11;
    U16 		 UartBuffData12;
    U16 		 UartBuffData13;
    U16 		 UartBuffData14;
    U16 		 UartBuffData15;
    U16 		 UartBuffData16;
    U16 		 UartBuffData17;
    U16 		 UartBuffData18;
    U16 		 UartBuffData19;
    U16 		 UartBuffData20;
    U16 		 UartBuffData21;
    U16 		 UartBuffData22;
    U16 		 UartBuffData23;
    U16 		 UartBuffData24;
    U16 		 UartBuffData25;
    U16 		 UartBuffData26;
    U16 		 UartBuffData27;
    U16 		 UartBuffData28;
    U16 		 UartBuffData29;
    U16 		 UartBuffData30;
    U16 		 UartBuffData31;
    U16 		 UartBuffData32;
    U16 		 UartBuffData33;
    U16 		 UartBuffData34;
    U16 		 UartBuffData35;
}OM_SOCP_TRACE_STRU;

/*y00151394  Add For HSO Display Begin*/

/*64λ��32K������*/
typedef struct tagUNSIGNED64
{
    U32 uiLow32;
    U32 uiHigh32;
}UNSIGNED64;

typedef struct
{
    U32       ulOmMsgId;
    U32       ulOmMsgLen;
    /*OMԭ����Ϣ*/
    U16       ausData[8];
}TDS_OM_REQ_MSG_STRU;

/*****************************************************************************
 �ṹ����: TDS_OM_DPCH_BLER_INFO_IND_STRU
 Э����:
 ASN.1 ����:
 �ṹ˵��: DPCH �������ϱ�
*****************************************************************************/
typedef struct
{
    U16    ausDataTypeNackNum[6];  /* �ֱ����ͬҵ�����͵���������0: Signling; 2: PS Data; 2: CsA; 3: CsB; 4: CsC; 5: Cs Media*/
    U16    ausDataTypeTotalNum[6]; /* ��ͬҵ������TB���ܸ���*/
    U32    ulDpchTotalTbNack;      /* ����6���ŵ�NACK�ܺ�*/
    U32    ulDpchTotalTbNum;
}TDS_OM_DPCH_BLER_INFO_IND_STRU;

typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptPccpchMeasEna;
    U16 usRptPccpchMeasPeriod;   /*P-CCPCH  ����ֵ�ϱ����ڣ�����Ϊ5ms��������*/
} TPHY_DT_UE_PCCPCH_MEAS_REQ_STRU;

/*��������*/
typedef struct
{
    U16  usTdsSfn;       /*TDS �ϱ���֡�� 0-8191*/
    S16  sPccpchRscp;     /*dBm*/
    S16  sPccpchSir;      /*dB*/
    U16  usRsvd;
}TPHY_DT_UE_PCCPCH_MEAS_IND_STRU;

/*·��ԭ������*/
typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptTdsTsInfoEna;
    U16 usRptTsInfoPeriod;    /*��Ϣ�ϱ����ڣ�����Ϊ5ms��������*/
}TPHY_DT_UE_TS_INFO_REQ_STRU;

/*��������*/
typedef struct
{
    U16 usTdsSfn;             /*TDS �ϱ���֡�� 0-8191*/
    U16 ausTsIscp[5];         /*ÿ��ʱ϶�ϵĸ����ź��빦�ʣ�ʱ϶2-6����ͬ*/
    U16 usDpchTsBitMap;       /*DPCHʱ϶ռ��ָʾ��ռ��Ϊ1������Ϊ0*/
    U16 ausTsDpchRscp[5];     /*ÿ��ʱ϶�ϵ�DPCH�ŵ���RSCP , ʱ϶2-6����ͬ*/
    U16 ausTsDpchSir[5];      /*ÿ��ʱ϶�ϵ�DPCH�ŵ���SIR*/
    U16 usPdschTsBitMap;      /*DDSCHʱ϶ռ��ָʾ��ռ��Ϊ1������Ϊ0*/
    /*UINT16 asTsPdschRscp[5];     */ /*ÿ��ʱ϶�ϵ�PDSCH�ŵ���RSCP*/
    /*UINT16 asTsPdschSir[5];      */ /*ÿ��ʱ϶�ϵ�PDSCH�ŵ���SIR*/
    U16 asTsTxPower[6];       /*ÿʱ϶���͹��� */
    /*UINT16 usRsvd;*/
}TPHY_DT_UE_TS_INFO_IND_STRU;

/*·��ԭ������*/
typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptDpchInfoEna;
    U16 usRptDpchInfoPeriod;    /*��Ϣ�ϱ����ڣ�����Ϊ5ms��������*/
}TPHY_DT_UE_DPCH_INFO_REQ_STRU;

/*��������*/
typedef struct
{
    U16 usTdsSfn;       /*TDS �ϱ���֡�� 0-8191*/
    U16 usMidambleID;   /*ר�������ŵ���Midamble������*/
    S16  sDpchRscp;      /*ר�������ŵ���ƽ�������ź��빦��*/
    S16  sTargetDpchSir; /*ר�������ŵ�Ŀ���Ÿɱ�*/
    U16 sDchSir;        /*ר���ŵ��ŸɱȵĲ���ֵ*/
    U16 usTpcDl;        /*���д��书�ʿ�����*/
    U16 usTpcUl;        /*����ϫ�书�ʿ�����*/
    U16 usTpcStep;      /*���ʿ��Ʋ���*/
    U16 usTimeTsCnt;    /*ר�������ŵ����������ʱ϶��*/
    U16 usTimeTsInfo[5];/*ר�������ŵ�ʱ϶����*/
    U16 usSf[5];        /*ר�������ŵ�ʹ�õ���Ƶ����*/
    U16 usOvsfNum[5];    /*OVSF NUM,ר�������ŵ�����������*/
    U16 usOvsfID[5][16];    /*OVSF ID,ר�������ŵ�����������*/
}TPHY_DT_UE_DPCH_INFO_IND_STRU;

/*·��ԭ������*/
typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptHssichInfoEna;
    U16 usRptHssichInfoPeriod;    /*��Ϣ�ϱ����ڣ�����Ϊ5ms��������*/
}TPHY_DT_UE_HSSICH_INFO_REQ_STRU;

/* CQI�������ṹ��*/
typedef struct
{
    U16 usCqiOutTbi;    /*TBI �ϱ�ֵ���*/
    U16 usCqiOutMod;    /*���Ʒ�ʽ�ϱ�ֵ���*/
}TDS_CQI_OUT_STRU;

/*��������*/
typedef struct
{
    U16 usTdsSfn;      /*TDS �ϱ���֡�� 0-8191*/
    /*UINT16 usModType;     */ /*���Ʒ�ʽ*/
    /*UINT16 usSf;          */ /*��Ƶ���ӣ�SF16*/
    U16 usRsvd;
    /*UINT16 usTsFormat[6];  */ /*ʱ϶��ʽ����TFCI��TPC��SS*/
    TDS_CQI_OUT_STRU stCqiData;/*CQI�������*/
}TPHY_DT_UE_HSSICH_INFO_IND_STRU;

/*·��ԭ������*/
typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptHsdpaInfoEna;
    U16 usRptHsdpaPeriod;    /*�ϱ����ڣ�����Ϊ5ms��������*/
}TPHY_DT_UE_HSDPA_INFO_REQ_STRU;

/*·��ԭ������*/
typedef struct
{
    OM_REQ_ENABLE_ENUM_UINT16 enRptTdsOmflg;
    U16 usFlg;    /*�ϱ�����0:ȱʡȫ���ϱ���1:ֻ�ϱ�������*/
}TPHY_DT_UE_TDS_OM_RPT_REQ_STRU;


/*��������*/
typedef struct
{
    U16 usTdsSfn;         /*TDS �ϱ���֡�� 0-8191*/
    /*UINT16 usQam16ModulRat;  */ /*16QAM���Ʊ��뷽ʽռ�ĵ��Ʊ��뷽ʽ�ı���*/
    /*UINT16 usQpskModulRat;   */ /*1QPSK���Ʊ��뷽ʽռ�ĵ��Ʊ��뷽ʽ�ı���*/
    /*UINT16 usHRnti;          */ /*HSPA�µ�����������ʱ��ʶ*/
    U16 usHsdschAckCnt;  /*ͳ�������ڣ�HS-DSCH�ŵ��ϵ�ACKռ�ı���*/
    U16 usHsdschNackCnt; /*ͳ�������ڣ�HS-DSCH�ŵ��ϵ�NACKռ�ı���*/
    U16 usHsDschTbiTotal;     /*ͳ�������ڣ�HS-DSCH�ŵ��϶�Ӧ�������*/
    /*UINT16 usRsvd;*/
}TPHY_DT_UE_HSDPA_INFO_IND_STRU;
typedef struct
{
    U16                 usQpskCnt;
    U16                 us16Qamcnt;
    U16                 usNewTrans;
    U16                 usFistRetxIndNum;
    U16                 usSecondRetxNum;
    U16                 usThridRetxNum;
    U16                 usForthRetxNum;
    U16                 usMoreRetxNum;
    U16                 usTotalNewInd;
    U16                 usTotalNdi;
    U16                 usTotalDecodeCnt;
    U16                 usTotalCrcRight;
    U16                 usTotalCrcWrong;
    U16                 ausResv[3];
}TDS_OM_DSCH_HARQ_INFO_STRU;
/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/



/*****************************************************************************
  10 ��������
*****************************************************************************/
extern U32 OM_Report(U32 ulMsgId, U16 * pInfo, U32 ulInfoLenOf16bit);
extern void OM_MboxMsgProc();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TDS_OM_H__ */

