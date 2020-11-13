

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

#ifndef __CQI_OM_DEF_H__
#define __CQI_OM_DEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/************************************************************
                           1. REQ����Ͷ�Ӧ�����ݽṹ����
************************************************************/
typedef enum __LPHY_REQ_MSGID_CQI__
{
    LPHY_REQ_CQI_RESERVED            = OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_REQ, 0x0),
    LPHY_REQ_CQI_TRMS_FILTER,
    LPHY_REQ_CQI_FLAGS,
    LPHY_REQ_CQI_WHITE_THRESHOLD, 
    LPHY_REQ_CQI_NOISE_FACTOR_ETA1, 
    LPHY_REQ_CQI_NOISE_FACTOR_ETA2,
    LPHY_REQ_CQI_NORM,
    LPHY_REQ_CQI_NPA,
    LPHY_REQ_CQI_ESNR_COMPENSATE,
    LPHY_REQ_CQI_RBIR_ALPHA,
    LPHY_REQ_CQI_RI_EFF_ALPHA,
    LPHY_REQ_CQI_RI_THRESH_BETA1,
    LPHY_REQ_CQI_RI_THRESH_BETA2,
    LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_ALPHA,
    LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_THRESH,
    LPHY_REQ_CQI_RI_EFF_SCALE,    
    LPHY_REQ_CQI_RI_ESNRA_ESNRB,  
    LPHY_REQ_CQI_RI_STUB,    
    LPHY_REQ_CQI_R2_WB_CQI_STUB,
    LPHY_REQ_CQI_R1_WB_CQI_STUB,
    LPHY_REQ_CQI_R2_SB_CQI_STUB,
    LPHY_REQ_CQI_R1_SB_CQI_STUB,
    LPHY_REQ_CQI_ESNR_SCOPE,
    LPHY_REQ_CQI_RPT_SG,
    LPHY_REQ_CQI_MCS_CONFIG,
    LPHY_REQ_CQI_SHORTS_SUBFRM_CONFIG,
    LPHY_REQ_CQI_DRX_PARA
}LPHY_REQ_MSGID_CQI_ENUM;

typedef struct __LPHY_REQ_CQI_TRMS_FILTER_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucAlpha;
    UINT8                       ucBelta[6]; 
    UINT8                       ucRsv[3];               /*4�ֽڶ���*/
}LPHY_REQ_CQI_TRMS_FILTER_STRU;

typedef struct __LPHY_REQ_CQI_FLAGS_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16       enEn;
    LTE_CHAN_SPACE_CORR_ENUM_UINT8  enCorrFlag;
    UINT8                           ucTrmsFlag;
    UINT8                           ucSNRFlag;
    UINT8                           ucVelocityFlag;
    UINT8                           ucRsv[2];           /*4�ֽڶ���*/    
}LPHY_REQ_CQI_FLAGS_STRU;

typedef struct __LPHY_REQ_CQI_WHITE_THRESHOLD_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucWhiteThresholdA;      /*����A*/
    UINT8                       ucWhiteThresholdB;      /*����B*/
    UINT8                       ucWhiteThresholdC;      /*����C*/
    UINT8                       ucRsv[3];               /*4�ֽڶ���*/

}LPHY_REQ_CQI_WHITE_THRESHOLD_STRU;

typedef struct __LPHY_REQ_CQI_NOISE_FACTOR_ETA1_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      us12TxNormalCp1;        /*1Tx,2Tx, normal cp*/
    UINT16                      us12TxNormalCp2;        /*1Tx,2Tx, normal cp*/
    UINT16                      us12TxNormalCp3;        /*1Tx,2Tx, normal cp*/
    UINT16                      us12TxExtendCp1;        /*1Tx,2Tx, extend cp*/
    UINT16                      us12TxExtendCp2;        /*1Tx,2Tx, extend cp*/
    UINT16                      us12TxExtendCp3;        /*1Tx,2Tx, extend cp*/
    UINT16                      us4TxNormalCp1;         /*4Tx normal cp*/
    UINT16                      us4TxNormalCp2;         /*4Tx normal cp*/
    UINT16                      us4TxNormalCp3;         /*4Tx normal cp*/
    UINT16                      us4TxExtendCp1;         /*4Tx extend cp*/
    UINT16                      us4TxExtendCp2;         /*4Tx extend cp*/
    UINT16                      us4TxExtendCp3;         /*4Tx extend cp*/
    UINT8                       ucRsv[2];               /*4�ֽڶ���*/
}LPHY_REQ_CQI_NOISE_FACTOR_ETA1_STRU;

typedef struct __LPHY_REQ_CQI_NOISE_FACTOR_ETA2_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usSFBC;                 /*sfbc eta2*/
    UINT16                      usSM1;                  /*sm1 eta2*/
    UINT16                      usSM2;                  /*sm2 eta2*/
}LPHY_REQ_CQI_NOISE_FACTOR_ETA2_STRU;

typedef struct __LPHY_REQ_CQI_NORM_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucRB[6];                /*6RB->100RB*/
}LPHY_REQ_CQI_NORM_STRU;

typedef struct __LPHY_REQ_CQI_NPA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usNpa[2][8];            /*2 or 4 trans ant num*/
    UINT8                       ucRsv[2];               /*4�ֽڶ���*/
}LPHY_REQ_CQI_NPA_STRU;

typedef struct __LPHY_REQ_CQI_ESNR_COMPENSATE_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usEsnrComp[2][8];       /*2 or 4 trans ant num*/
    UINT8                       ucRsv[2];               /*4�ֽڶ���*/
}LPHY_REQ_CQI_ESNR_COMPENSATE_STRU;

typedef struct __LPHY_REQ_CQI_RBIR_ALPHA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usWbAlpha;
    UINT16                      usSbAlpha;
    UINT8                       ucRsv[2];               /*4�ֽڶ���*/
}LPHY_REQ_CQI_RBIR_ALPHA_STRU;

typedef struct __LPHY_REQ_CQI_RI_EFF_ALPHA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usEffAlpha;             /* (0��1��2��3��4��5��6��7����Ӧ��1��1/2��1/4��1/8��1/16��1/32��1/64��1/128*/
}LPHY_REQ_CQI_RI_EFF_ALPHA_STRU;

typedef struct __LPHY_REQ_CQI_RI_THRESH_BETA1_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usBeta1;                /*7u*/
}LPHY_REQ_CQI_RI_THRESH_BETA1_STRU;

typedef struct __LPHY_REQ_CQI_RI_THRESH_BETA2_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usBeta2;                /*7u*/
}LPHY_REQ_CQI_RI_THRESH_BETA2_STRU;

typedef struct __LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_ALPHA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usSnrAlpha;             /*3u*/
}LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_ALPHA_STRU;

typedef struct __LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_THRESH_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    INT16                       sThresh;                /*10s*/
}LPHY_REQ_CQI_RI1_SEL_THRESH_SNR_THRESH_STRU;

typedef struct __LPHY_REQ_CQI_RI_EFF_SCALE_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usEffScale;             /*7u*/
}LPHY_REQ_CQI_RI_EFF_SCALE_STRU;

typedef struct __LPHY_REQ_CQI_RI_ESNRA_ESNRB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucESNRA;                /*6u*/
    UINT8                       ucESNRB;                /*6u*/
}LPHY_REQ_CQI_RI_ESNRA_ESNRB_STRU;

typedef struct __LPHY_REQ_CQI_RI_STUB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                      usPeriRI;
    UINT16                      usAperRI;
    UINT8                       ucRsv[2];       /*4�ֽڶ���*/    
}LPHY_REQ_CQI_RI_STUB_STRU;

typedef struct __LPHY_REQ_CQI_R2_WB_CQI_STUB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                  usCw0Cqi;
    UINT16                  usCw1Cqi;
    UINT16                     usPmi;
}LPHY_REQ_CQI_R2_WB_CQI_STUB_STRU;

typedef struct __LPHY_REQ_CQI_R1_WB_CQI_STUB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                  usCw0Cqi;
    UINT16                     usPmi;
    UINT8                   ucRsv[2];           /*4�ֽڶ���*/
}LPHY_REQ_CQI_R1_WB_CQI_STUB_STRU;

typedef struct __LPHY_REQ_CQI_R2_SB_CQI_STUB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16               usSbPos[13];
    UINT16              usCw0Cqi[13];
    UINT16              usCw1Cqi[13];
    UINT16                 usPmi[13];
    UINT8                   ucRsv[2];           /*4�ֽڶ���*/
}LPHY_REQ_CQI_R2_SB_CQI_STUB_STRU;

typedef struct __LPHY_REQ_CQI_R1_SB_CQI_STUB_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16               usSbPos[13];
    UINT16              usCw0Cqi[13];
    UINT16                 usPmi[13];
}LPHY_REQ_CQI_R1_SB_CQI_STUB_STRU;

typedef struct __LPHY_REQ_CQI_ESNR_SCOPE_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16                     usMin;
    UINT16                     usMax;
    UINT8                   ucRsv[2];           /*4�ֽڶ���*/
}LPHY_REQ_CQI_ESNR_SCOPE_STRU;

typedef struct __LPHY_REQ_CQI_RPT_SG_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                   ucRsv[2];       /*4�ֽڶ���*/
}LPHY_REQ_CQI_RPT_SG_STRU;

typedef struct __LPHY_REQ_CQI_MCS_CONFIG_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                   ucMcType;       /*0,���̶���1�̶�*/
    UINT8                 ucMcsIndex;       /*�̶�ֵ*/
}LPHY_REQ_CQI_MCS_CONFIG_STRU;

typedef struct __LPHY_REQ_CQI_SHORTS_SUBFRM_CONFIG_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16        enEn;
    UINT8          ucRecvShortSSubFrmFlag;     /*���ն�S ��֡���*/
    UINT8    ucTakeMbmsAsShortSSubFrmFlag;     /*�Ƿ񽫶ಥ��֡������S ��֡�ı��*/
}LPHY_REQ_CQI_SHORTS_SUBFRM_CONFIG_STRU;

/*���¾�ΪCQI DRX  ��ص����ò���*/
typedef struct __LPHY_REQ_CQI_DRX_PARA_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT8                       ucDrxCycleFilterFlag;
    UINT8                       ucRsv;
    UINT16                      usAlpha;   /*Ĭ��ֵΪ1*/
    UINT16                      usAlpha_Np_CqiPmi;  /*Ĭ��ֵΪ1*/
}LPHY_REQ_CQI_DRX_PARA_STRU;

/************************************************************
                           2. CNF���ݽṹ����
************************************************************/
typedef enum __LPHY_CNF_MSGID_CQI__
{
    LPHY_CNF_CQI_RESERVED            = OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_CNF, 0x0),
    LPHY_CNF_CQI_STUB,
}LPHY_CNF_MSGID_CQI_ENUM;


/************************************************************
                           3. Ind���ݽṹ����
************************************************************/
typedef enum __LPHY_IND_MSGID_CQI__
{
    LPHY_IND_CQI_RESERVED            = OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_IND, 0x0),
}LPHY_IND_MSGID_CQI_ENUM;

/************************************************************
                           4. TRACE���ݽṹ����
************************************************************/
typedef enum __LPHY_TRACE_CQI__
{
    LPHY_TRACE_CQI_RESERVED         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_TRACE, 0x0),
    LPHY_TRACE_CQI_TRACE_BINARY,
    LPHY_TRACE_CQI_CONFIG,
    LPHY_TRACE_CQI_INVALID_PARA
}LPHY_TRACE_CQI_ENUM;

typedef struct __LPHY_TRACE_CQI_INVALID_PARA_STRU__
{
    INT32       lErrNo;             /*��ʾ������Ч��*/
}LPHY_TRACE_CQI_INVALID_PARA_STRU;

/************************************************************
                           5. SG ���ݽṹ����, �㷨��Ҫ��ͼ��
************************************************************/
typedef enum __LPHY_SG_MSGID_CQI__
{
    LPHY_SG_CQI_RESERVED            = OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_SG, 0x0),
    LPHY_SG_CQI_RPT_SNR,            /*�ϱ�CQI��Ӧ��SNR*/
    LPHY_SG_CQI_RPT_APER_RESULT,    /*�ϱ�ÿ�������ڵĽ��*/
    LPHY_SG_CQI_RPT_PERI_RESULT,    /*�ϱ�ÿ�����ڵĽ��*/
    LPHY_SG_CQI_RPT_RI_CNT,         /*�ϱ�RI�ֱ�Ϊ0��1��2�ļ���*/
    LPHY_SG_CQI_RPT_WCQI_CNT,       /*�ϱ����CQI����CQIȡֵ(0~15)�ļ���*/
    LPHY_SG_CQI_RPT_SCQI_CNT,       /*�ϱ��Ӵ�CQI�����Ӵ�����CQIȡֵ(0~15)�ļ���*/
}LPHY_SG_MSGID_CQI_ENUM;

typedef struct __LPHY_SG_CQI_RPT_SNR_STRU__
{
    INT16       sR1SbCqi[25];       /*��Ϊ1���Ӵ�SNR*/
    INT16       sR1WbCqi;           /*��Ϊ1�Ŀ��SNR*/
    INT16       sR2SbCqi[2][25];    /*��Ϊ2���Ӵ�SNR*/
    INT16       sR2WbCqi[2];        /*��Ϊ2�Ŀ��SNR*/
}LPHY_SG_CQI_RPT_SNR_STRU;

/*�߲����õķ�����ģʽ*/
typedef enum CQI_APERIODIC_MODE
{                              
    CQI_APERI_MODE_12 = 0,        
    CQI_APERI_MODE_20,            
    CQI_APERI_MODE_22,            
    CQI_APERI_MODE_30,            
    CQI_APERI_MODE_31,            
    CQI_APERI_MODE_INVALID = 0xffff
}CQI_APERIODIC_MODE_ENUM;      
typedef UINT16 CQI_APERIODIC_MODE_ENUM_UINT16;

/*�߲����õ�����ģʽ*/
typedef enum CQI_PERIODIC_MODE
{
    CQI_PERIOD_MODE_10 = 0,
    CQI_PERIOD_MODE_11,
    CQI_PERIOD_MODE_20,
    CQI_PERIOD_MODE_21,
    CQI_PERIOD_MODE_INVALID = 0xFFFF
}CQI_PERIODIC_MODE_ENUM;
typedef UINT16 CQI_PERIODIC_MODE_ENUM_UINT16;

typedef struct __LPHY_SG_CQI_RPT_APER_RESULT_STRU__
{
    CQI_APERIODIC_MODE_ENUM_UINT16  enAperMode; /*������ģʽ*/
    UINT16      usRI;                   /*���μ����ȡ��RI���*/
    UINT8       ucR2WbCqi[2];           /*����0������1�Ŀ��CQI*/
    UINT8       ucR2SbCqi[2][25];       /*����0������1���Ӵ�CQI, bestM�ڶ�Ӧλ��*/
    UINT8       ucR2WbPmi;              /*����0������1�Ŀ��PMI*/
    UINT8       ucR2SbPmi[25];          /*����0������1���Ӵ�PMI*/
    UINT8       ucR1WbCqi;              /*����0������1�Ŀ��CQI*/
    UINT8       ucR1SbCqi[25];          /*����0������1���Ӵ�CQI, bestM�ڶ�Ӧλ��*/
    UINT8       ucR1WbPmi;              /*����0������1�Ŀ��PMI*/
    UINT8       ucR1SbPmi[25];          /*����0������1���Ӵ�PMI*/
}LPHY_SG_CQI_RPT_APER_RESULT_STRU;

typedef struct __LPHY_SG_CQI_RPT_PERI_RESULT_STRU__
{
    CQI_PERIODIC_MODE_ENUM_UINT16   enPeriMode; /*����ģʽ*/
    UINT16      usRI;                   /*���μ����ȡ��RI���*/
    UINT8       ucR2WbCqi[2];           /*����0������1�Ŀ��CQI*/
    UINT8       ucR2SbCqi[2][25];       /*����0������1���Ӵ�CQI, bestM�ڶ�Ӧλ��*/
    UINT8       ucR2WbPmi;              /*����0������1�Ŀ��PMI*/
    UINT8       ucR2SbPmi[25];          /*����0������1���Ӵ�PMI*/
    UINT8       ucR1WbCqi;              /*����0������1�Ŀ��CQI*/
    UINT8       ucR1SbCqi[25];          /*����0������1���Ӵ�CQI, bestM�ڶ�Ӧλ��*/
    UINT8       ucR1WbPmi;              /*����0������1�Ŀ��PMI*/
    UINT8       ucR1SbPmi[25];          /*����0������1���Ӵ�PMI*/
}LPHY_SG_CQI_RPT_PERI_RESULT_STRU;

typedef struct __LPHY_SG_CQI_RPT_RI_CNT_STRU__
{
    UINT16      usRI[3];            /*RI�ֱ�λ0��1��2�ļ���*/
}LPHY_SG_CQI_RPT_RI_CNT_STRU;

typedef struct __LPHY_SG_CQI_RPT_WCQI_CNT_STRU__
{
    UINT16      usR2WbCqi[2][16];       /*����0��16�������ͳ��������*/
    UINT16      usR2WbPmi[16];          /*����0��16�������ͳ��������*/
    UINT16      usR2WbCqiMid[2];        /*����0�Ŀ��CQI��λ��*/
    UINT16      usR1WbCqi[16];          /*����0��16�������ͳ��������*/
    UINT16      usR1WbPmi[16];          /*����0��16�������ͳ��������*/
    UINT16      usR1WbCqiMid;           /*����1�Ŀ��CQI��λ��*/
}LPHY_SG_CQI_RPT_WCQI_CNT_STRU;

typedef struct __LPHY_SG_CQI_RPT_SCQI_CNT_STRU__
{
    UINT16      usR2Cw0SbCqi[25][16];   /*����0��13���Ӵ�ÿ��CQI��ͳ��������*/
    UINT16      usR2Cw1SbCqi[25][16];   /*����1��13���Ӵ�ÿ��CQI��ͳ��������*/
    UINT16      usR2SbPmi[25][16];      /*����0��13���Ӵ�ÿ��CQI��ͳ��������*/
    UINT16      usR1SbCqi[25][16];      /*����0��13���Ӵ�ÿ��CQI��ͳ��������*/
    UINT16      usR1SbPmi[25][16];      /*����0��13���Ӵ�ÿ��PMI��ͳ��������*/
}LPHY_SG_CQI_RPT_SCQI_CNT_STRU;


/************************************************************
                           6. Error���ݽṹ����
************************************************************/
typedef enum __LPHY_ERROR_CQI__
{
    LPHY_ERROR_CQI_RESERVED         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_ERROR, 0x0),
    LPHY_ERROR_CQI_STAT    
}LPHY_ERROR_CQI_ENUM;

typedef enum __LPHY_ERROR_CQI_STAT_ENUM__
{
    LPHY_ERROR_CQI_STAT_NOT_FINISHED = 0x1,
    LPHY_ERROR_CQI_RESET_HWFSM = 0x2,
}LPHY_ERROR_CQI_STAT_ENUM_UINT16;

typedef struct __LPHY_ERROR_CQI_STAT_STRU__
{
    LPHY_ERROR_CQI_STAT_ENUM_UINT16   enErrNo;
}LPHY_ERROR_CQI_STAT_STRU;

/* BEGIN: Added by xueqiuyan, 2012/9/17   PN:CMCC_DT*/
/************************************************************
                           7. DT���ݽṹ����
************************************************************/
typedef enum __LPHY_DT_REQ_CQI__
{
    LPHY_DT_APER_CQI_INFO_REQ         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_DT, 0x1),
    LPHY_DT_PERI_CQI_INFO_REQ         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_DT, 0x2)
}LPHY_DT_REQ_CQI_ENUM;

typedef struct __LPHY_DT_APER_CQI_INFO_REQ_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16  usRsv;
} LPHY_DT_APER_CQI_INFO_REQ_STRU;

typedef struct __LPHY_DT_PERI_CQI_INFO_REQ_STRU__
{
    OM_REQ_ENABLE_ENUM_UINT16   enEn;
    UINT16  usRsv;
} LPHY_DT_PERI_CQI_INFO_REQ_STRU;


typedef enum __LPHY_DT_IND_CQI__
{
    LPHY_DT_APER_CQI_INFO_IND         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_DT, 0x1001),
    LPHY_DT_PERI_CQI_INFO_IND         =  OM_CMD_ID(LPHY_CQI_MID, OM_TYPE_DT, 0x1002)
}LPHY_DT_IND_CQI_ENUM;



typedef struct __LPHY_DT_APER_CQI_INFO_IND_STRU__
{
    UINT16     usSFN;
    UINT16     usSubFn;
    CQI_APERIODIC_MODE_ENUM_UINT16  enAperMode;
    UINT16      usRI;
    UINT8       ucR2WbCqi[2];
    UINT8       ucR2SbCqi[2][25];
    UINT8       ucR2WbPmi;
    UINT8       ucR2SbPmi[25];
    INT16       sR2WbCqiSnr[2];
    INT16       sR2SbCqiSnr[2][25];
	 /* END:   Added by xueqiuyan, 2012/10/30 */
    UINT8       ucR1WbCqi;
    UINT8       ucR1SbCqi[25];
    UINT8       ucR1WbPmi;
    UINT8       ucR1SbPmi[25];
    INT16       sR1WbCqiSnr;
    INT16       sR1SbCqiSnr[25];
	 /* END:   Added by xueqiuyan, 2012/10/30 */
    UINT16      usBestMNum;
    /*UINT16      usRsv;*/
     /* END:   Added by wujing, 2013/01/07 */
    UINT32      ulCombinationrialIndex;
}LPHY_DT_APER_CQI_INFO_IND_STRU;

typedef enum __CURRENT_REPORT_TYPE__
{
    RI_MODE = 0,
    /*CQI_PMI_MODE*/
    WCQI_MODE,
    SCQI_MODE,
    CQI_NONE_MODE
    /* END:   Added by wujing, 2012/12/4 */
}CURRENT_REPORT_TYPE_ENUM;

typedef UINT16 CURRENT_REPORT_TYPE_ENUM_UINT16;


typedef struct __LPHY_DT_PERI_CQI_INFO_IND_STRU__
{
    UINT16     usSFN;
    UINT16     usSubFn;
    CURRENT_REPORT_TYPE_ENUM_UINT16  enCurRptType;
    CQI_PERIODIC_MODE_ENUM_UINT16  enPeriMode;
    UINT16      usCQIRptPeriod;
    UINT16      usLabelLen;
    UINT16      usBpNum;
    UINT16      usK;
    UINT16      usMRI;
    UINT16      usRI;
    UINT8       ucR2WbCqi[2];
    UINT8       ucR2SbCqi[2][25];
    UINT8       ucR2WbPmi;
    UINT8       ucR2SbPmi[25];
    INT16       sR2WbCqiSnr[2];
    INT16       sR2SbCqiSnr[2][25];
    UINT8       ucR1WbCqi;
    UINT8       ucR1SbCqi[25];
    UINT8       ucR1WbPmi;
    UINT8       ucR1SbPmi[25];
    INT16       sR1WbCqiSnr;
    INT16       sR1SbCqiSnr[25];
    UINT16      usRsv;
}LPHY_DT_PERI_CQI_INFO_IND_STRU;

/* END:   Added by xueqiuyan, 2012/9/17 */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __CQI_OM_DEF_H__ */
