

/*****************************************************************************
 ����˵����
    1�����ļ���MSP����Э��DSP��ɣ��������ƶ�Ӧ���ĵ����ṩ��Դ��
    2����������FOLLOW�ĵ���������
    3������ά����DSPά��

*****************************************************************************
*****************************************************************************

                  ��Ȩ���� (C), 2001-2012, ��Ϊ�������޹�˾

*****************************************************************************
  �� �� ��   : dsp_ct_ex.h
  �� �� ��   : V1.0
  ��    ��   : DSP
  ��������   : 2010��12��13��
  ��������   : ���ļ�������V1R1 C00��CT��MSP��DSP������ԭ�Ｐ�������������
  �޸���ʷ   :
  1.��    �� : 2010��12��13��
    ��    �� : MSP
    �޸����� : �����ļ�

*****************************************************************************/
#ifndef __DSP_CT_EX_H__
#define __DSP_CT_EX_H__

#undef UINT32
#define  UINT32 unsigned long
#undef INT32
#define  INT32 long
#undef INT16
#define   INT16 short
#undef UINT16
#define UINT16 unsigned short
#undef UINT8
#define UINT8 unsigned char
#undef INT8
#define INT8 unsigned char

/* DSP CT ���Χ[0x00010000,0x00010F00)*/
/* CT��������Ϊ��(0x00010000=<DSP CMDID<0x00010F00)*/
#define OM_PHY_CT_F_FREQ_SET_REQ         (0x10120)
#define PHY_OM_CT_F_FREQ_SET_CNF         (0x10120)

#define OM_PHY_CT_F_TXPOW_SET_REQ        (0X10130)
#define PHY_OM_CT_F_TXPOW_SET_CNF        (0X10130)

#define OM_PHY_CT_F_AAGC_SET_REQ         (0X10140)
#define PHY_OM_CT_F_AAGC_SET_CNF         (0X10140)

#define PHY_OM_CT_F_MEAS_RSSI_IND        (0X10150)

#define PHY_OM_CT_F_MEAS_CMTMS_IND       (0X10160)

#define OM_PHY_CT_F_PA_SET_REQ           (0X10170)
#define PHY_OM_CT_F_PA_SET_CNF           (0X10170)

#define OM_PHY_CT_F_SEGMENT_SET_REQ      (0X10180)
#define PHY_OM_CT_F_SEGMENT_SET_CNF      (0X10180)

#define OM_PHY_CT_F_TXWAVE_SET_REQ       (0X10190)
#define PHY_OM_CT_F_TXWAVE_SET_CNF       (0X10190)

#define OM_PHY_CT_F_TRIGGER_SET_REQ      (0X101A0)
#define PHY_OM_CT_F_TRIGGER_SET_CNF      (0X101A0)

#define OM_PHY_CT_F_TRIGGER_RD_REQ       (0X101B0)
#define PHY_OM_CT_F_TRIGGER_RD_CNF       (0X101B0)


#define OM_PHY_CT_F_FBLK_SET_REQ         (0X101C0)
#define PHY_OM_CT_F_FBLK_SET_CNF         (0X101C0)

#define OM_PHY_CT_F_FIPSTART_SET_REQ     (0X101D0)
#define PHY_OM_CT_F_FIPSTART_SET_CNF     (0X101D0)
#define PHY_OM_CT_F_MEAS_IP2_IND          (0X101E0)

#define OM_PHY_CT_F_MAX_POWER_SET_REQ           (0X101f0)
#define PHY_OM_CT_F_MAX_POWER_SET_CNF           (0X101f0)

#define OM_PHY_CT_F_PA_POWER_SET_REQ           (0X101f4)
#define PHY_OM_CT_F_PA_POWER_SET_CNF           (0X101f4)

#define OM_PHY_LTESCINFO_RD_REQ          (0X101C2)
#define PHY_OM_LTESCINFO_RD_CNF          (0X101C2)

#define OM_PHY_CT_F_BANDSW_SET_REQ             (0X101F5)
#define PHY_OM_CT_F_BANDSW_SET_CNF             (0X101F5)


#define OM_PHY_BT_TEMP_DISPATCH_REQ        (0x10340)
#define PHY_OM_BT_TEMP_DISPATCH_CNF        (0x10340)

#define OM_PHY_CT_F_FPDMS_REQ              (0X101F6)
#define PHY_OM_CT_F_FPDMS_CNF              (0X101F6)

/*AT^FPDMS*/
typedef struct
{
   UINT32 ulMsgId;
   UINT32 ulIsEnable;
}OM_PHY_CT_F_FPDMS_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
}PHY_OM_CT_F_FPDMS_SET_CNF_STRU;

/*TEMP*/
typedef struct
{
   UINT32 ulMsgId;
   UINT16 usTempVal;
   UINT16 usRsv;
}OM_PHY_BT_TEMP_DISPATCH_REQ_STRU;
/*****************************************************************************
AT^BANDSW
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
	UINT16 usMode;
	UINT16 usBand;
}OM_PHY_CT_F_BANDSW_SET_REQ_STRU;
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_BANDSW_SET_CNF_STRU;



/*****************************************************************************
 ���� : ��ǰ����ͨ���򿪡�
        ����Ϊ������Ƶ�㡣MSP�뽫AT^FCHANS�����е��ŵ��Ÿ���36.101Э��涨���תΪʵ��Ƶ��ֵ��
        ת�����̲ο���¼4��
 ID   : OM_PHY_CT_F_FREQ_SET_REQ,PHY_OM_CT_F_FREQ_SET_CNF
 REQ  : OM_PHY_CT_F_FREQ_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_FREQ_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
#define CT_F_FREQ_UL_LIST_MAX_NUM 32
#define CT_F_FREQ_DL_LIST_MAX_NUM 32
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ*/
    UINT32 ulMode;                       /* MODE TDD��7��FDD��6*/
    UINT16 usUlListNum; /* �������õĸ�������ӦausUlFreqInfo�����16��*/
    UINT16 usDlListNum; /* �������õĸ�������ӦausDlFreqInfo�����16��*/

    /* Ƶ����Ϣ��*/
    /* Ƶդ���100Khz��������ֵ26200��ӦƵ��2.62GHz*/
    UINT16 ausUlFreqInfo[CT_F_FREQ_UL_LIST_MAX_NUM];       /* ����Ƶ����Ϣ*/
    UINT16 ausDlFreqInfo[CT_F_FREQ_DL_LIST_MAX_NUM];       /* ����Ƶ����Ϣ*/
} OM_PHY_CT_F_FREQ_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_FREQ_SET_CNF_STRU;


/*****************************************************************************
 ���� : �������з��书�ʡ�RTT�յ���ԭ���Ӧ�����ı����з��书��
 ID   : OM_PHY_CT_F_TXPOW_SET_REQ,PHY_OM_CT_F_TXPOW_SET_CNF
 REQ  : OM_PHY_CT_F_TXPOW_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_TXPOW_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
#define CT_POWER_SET_MAX_NUM             (30)
typedef struct
{
    UINT32 ulMsgId;                          /* ��Ϣ��ʶ */

    /* ��λ��dBm*/
    UINT16 usTxPowerNum;                     /* ָ����asTxPower�ĸ���*/
    UINT16 usRsv;
    /* ���书���б��Կո�ָÿ��ֵ�ĳ���Ϊһ���� ����λΪdBm*/
    INT16  ausTxPower[CT_POWER_SET_MAX_NUM];  /*���з��书��*/
} OM_PHY_CT_F_TXPOW_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_TXPOW_SET_CNF_STRU;


/*****************************************************************************
 ���� : ����RFIC��AAGC�����棬��������AGC���У׼����λΪdB��
      RTT�յ���ԭ���RFIC��������������Ϊԭ��ָʾ������ֵ��
 ID   : OM_PHY_CT_F_AAGC_SET_REQ,PHY_OM_F_CT_AAGC_SET_CNF
 REQ  : OM_PHY_CT_F_AAGC_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_AAGC_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/

#define CT_F_AAGC_SET_MAX_NUM    (30)
typedef struct
{
    UINT32 ulMsgId;                               /* ��Ϣ��ʶ*/
    UINT16 usAagcNum;
    UINT16 usRsv;

    /*ȡֵ��Χ0��255������Hi6910�ķ�ΧΪ151-234��*/
    UINT8 aucAAGCValue[CT_F_AAGC_SET_MAX_NUM];    /* AAGC����ֵ*/
} OM_PHY_CT_F_AAGC_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_AAGC_SET_CNF_STRU;

/*****************************************************************************
 ���� :
 ID   : PHY_OM_CT_F_MEAS_RSSI_IND
 REQ  : NA
 CNF  : NA
 IND  : PHY_OM_CT_F_MEAS_RSSI_IND_STRU
 ˵�� :
*****************************************************************************/
#define POW_MAX_NUM             (30)
#define FREQ_MAX_NUM            (16)

typedef struct
{
    /* ATָ�-װ������:*/
    /*  ��ֵ������ֵ��ʾ����ȷ��0.125dBm����ֵ��Ϣ��ȡֵ��ΧΪ160~880��*/
    /*  �����ǰ��RSSIֵΪ-85.125dBm����ΧֵΪ-85.125dbm*(-1)/0.125dbm = 681*/

    INT16  sRxANT1RSSI;
    INT16  sRxANT2RSSI;
} CT_F_ANT_RSSI_STRU;

typedef struct
{
    UINT32 ulMsgId;
    UINT16 usFreqNum;
    UINT16 usPowNum;

    CT_F_ANT_RSSI_STRU  astRxANTRSSI[FREQ_MAX_NUM][POW_MAX_NUM];
} PHY_OM_CT_F_MEAS_RSSI_IND_STRU;

/*****************************************************************************
 ���� :
 ID   : PHY_OM_CT_F_MEAS_CMTMS_IND
 REQ  : NA
 CNF  : NA
 IND  : PHY_OM_CT_F_MEAS_CMTMS_IND_STRU
 ˵�� :
*****************************************************************************/

typedef struct
{
    UINT32  ulMsgId;
    UINT16  usCmtmsNum; 					     /* �¶�ֵ������ȡֵ��ΧΪ0��16*/
    UINT16  usRsv;

    UINT16  ausCmtms[FREQ_MAX_NUM]; /* �¶�ֵ�б�ȡֵ��ΧΪ0��256�����¶�ֵ����Ϊ0��û���¶�ֵ�б�*/

} PHY_OM_CT_F_MEAS_CMTMS_IND_STRU;


/*****************************************************************************
 ���� : ָʾRTT����PA
 ID   : OM_PHY_CT_F_PA_SET_REQ,PHY_OM_CT_F_PA_SET_CNF
 REQ  : OM_PHY_CT_F_PA_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_PA_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
#define CT_F_PA_SET_MAX_NUM   (30)
typedef struct
{
    UINT32 ulMsgId;
    UINT16 usLevelNum;
    UINT16 usRsv;
    UINT8 aucLvlList[CT_F_PA_SET_MAX_NUM];
} OM_PHY_CT_F_PA_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_PA_SET_CNF_STRU;


/*****************************************************************************
 ���� : ָʾRTT����SEGMENT
 ID   : OM_PHY_CT_F_SEGMENT_SET_REQ,PHY_OM_CT_F_SEGMENT_SET_CNF
 REQ  : OM_PHY_CT_F_SEGMENT_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_SEGMENT_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT16 usSegment;                    /* 10,20���� */
    UINT16 measOffset;                   /* ��ʾ������ʼλ��ƫ�ƣ���λms��ȡֵ��Χ0~20����Ĭ��ֵ*/
	UINT16 measLen;                      /* ��ʾ�������ȣ���λms��ȡֵ��Χ0~20����Ĭ��ֵ */
    UINT16 usRsv;
} OM_PHY_CT_F_SEGMENT_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_SEGMENT_SET_CNF_STRU;

/*****************************************************************************
 ���� : ָʾRTT���÷��䲨��
 ID   : OM_PHY_CT_F_TXWAVE_SET_REQ,PHY_OM_CT_F_TXWAVE_SET_CNF
 REQ  : OM_PHY_CT_F_TXWAVE_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_TXWAVE_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* 0    CW����*/
    /* 1    LTE����*/
    UINT16 usWaveType;

    UINT16 usRsv;
} OM_PHY_CT_F_TXWAVE_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_TXWAVE_SET_CNF_STRU;

/*****************************************************************************
 ���� : ��������У׼TRIGGER������У׼����
 ID   : OM_PHY_CT_F_TRIGGER_SET_REQ,PHY_OM_CT_F_TRIGGER_SET_CNF ����ID
        OM_PHY_CT_F_TRIGGER_RD_REQ,PHY_OM_CT_F_TRIGGER_RD_CNF   ��ѯID
 REQ  : OM_PHY_CT_F_TRIGGER_SET_REQ_STRU OM_PHY_CT_F_TRIGGER_RD_REQ_STRU
 CNF  : PHY_OM_CT_F_TRIGGER_SET_CNF_STRU PHY_OM_CT_F_TRIGGER_RD_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* 0    ����ͨ��У׼*/
    /* 1    ����ͨ��У׼������Ĭ��Ϊ˫ͨ����*/
    /* 2    ����/����ͨ��ͬʱУ׼������Ĭ��Ϊ˫ͨ����*/
    UINT8 ucType;

    UINT8 padding[3];
} OM_PHY_CT_F_TRIGGER_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_TRIGGER_SET_CNF_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_CT_F_TRIGGER_RD_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;

    /* 0    ����ͨ��У׼*/
    /* 1    ����ͨ��У׼������Ĭ��Ϊ˫ͨ����*/
    /* 2    ����/����ͨ��ͬʱУ׼������Ĭ��Ϊ˫ͨ����*/
    UINT8 ucType;

    /* ����У׼״̬*/
    /* 0    δ������ִ�����*/
    /* 1    ִ����*/
    UINT8 ucStatus;

    UINT8 ucRsv[2];
} PHY_OM_CT_F_TRIGGER_RD_CNF_STRU;

/*BLOCKING*/
typedef struct
{
    UINT32 ulMsgId;
	UINT16 usIsBLKMode;
	UINT16 usRsv;
}OM_PHY_CT_F_BLK_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
}PHY_OM_CT_F_BLK_SET_CNF_STRU;

/*IP2*/
typedef struct
{
    UINT32 ulMsgId;       /*��Ϣ��ʶ*/
    UINT16 usBand;        /*IP2У׼bandֵ*/
    UINT16 usRsv;
} OM_PHY_CT_F_IP2START_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;       /*��Ϣ��ʶ*/
} PHY_OM_CT_F_IP2START_CNF_STRU;


typedef struct
{
    UINT32 ulMsgId;       /*��Ϣ��ʶ*/
    INT16 ulIP2Value1;     /*IP2У׼���1*/
    INT16 ulIP2Value2;     /*IP2У׼���2*/
} PHY_OM_CT_F_MEAS_IP2_IND_STRU;
/*PHY_OM_CT_F_MEAS_IP2_IND_STRU*/

/*****************************************************************************
 ���� : MAXPOWERУ׼����
 ID   : OM_PHY_CT_F_MAX_POWER_REQ,PHY_OM_CT_F_MAX_POWER_CNF
 REQ  : OM_PHY_CT_F_MAX_POWER_REQ_STRU
 CNF  : PHY_OM_CT_F_MAX_POWER_CNF_STRU
 IND  : NAN
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ*/
    UINT16 usPaLevelNum;                 /*��ҪУ׼��PA ��λ��Ŀ0-4*/
    UINT16 usPaReduceGain;               /*����20111029��0-20*/
} OM_PHY_CT_F_MAX_POWER_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ*/
} PHY_OM_CT_F_MAX_POWER_SET_CNF_STRU;

/*****************************************************************************
 ���� : PAPOWER����
 ID   : OM_PHY_CT_F_PA_POWER_SET_REQ,PHY_OM_CT_F_PA_POWER_SET_CNF
 REQ  : OM_PHY_CT_F_PA_POWER_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_PA_POWER_SET_CNF_STRU
 IND  : NAN
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ*/
    UINT16 usPaLevelNum;
    UINT16 usRsv;
    INT16 asMaxPwrList[4];              /*У׼���MAXPOWERֵ*/
} OM_PHY_CT_F_PA_POWER_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ*/
} PHY_OM_CT_F_PA_POWER_SET_CNF_STRU;

/*****************************************************************************
 ���� : 8.6	����С����Ϣ��ѯ����^LTESCINFO
 ID   : OM_PHY_RD_LTESCINFO_REQ_STRU,OM_PHY_RD_LTESCINFO_CNF_STRU   ��ѯID
 REQ  : OM_PHY_RD_LTESCINFO_REQ_STRU
 CNF  : OM_PHY_RD_LTESCINFO_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_RD_LTESCINFO_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;

    UINT16 usCid;     /* ����С��ID,ȡֵ��Χ��0 ~ 503 ��65535��ʾû�л�ȡ����ǰ����С��ID*/

    INT16  usDlSinr ; /* ������·�����,ȡֵ��Χ��-20 ~ 30*/
    UINT16 usDlBw ;   /* LTE downlink bandwidth*/

    UINT8  ucNumAnt;  /* ��ǰ���߸���,0~4*/
    UINT8  ucRsv;
} PHY_OM_RD_LTESCINFO_CNF_STRU;

#endif
