

/*****************************************************************************
 ����˵����
    1�����ļ���MSP����Э��DSP��ɣ��������ƶ�Ӧ�����������ṩ��ԭ��
    2����������FOLLOW DSP�ĵ���������
    3������ά����DSPά��
    4���Ƿ���뷵��ERR_CODE���ĵ�������ϣ��ң�Ŀǰ����ԭ�ĵ�
*****************************************************************************/

#ifndef __DSP_CT_H__
#define __DSP_CT_H__

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

/* DSP CT ���Χ[0x00010000,0x00010F00) */
/* CT��������Ϊ��(0x00010000=<DSP CMDID<0x00010F00) */

/*^FCALIIP2S IDs*/
#define OM_PHY_CT_F_CALIIP2_SET_REQ (0x10220)
#define PHY_OM_CT_F_CALIIP2_SET_CNF (0x10220)
#define PHY_OM_CT_F_CALIIP2_IND (0x10221)

/*^FCALIDCOCS IDs*/
#define OM_PHY_CT_F_CALIDCOCS_SET_REQ (0x10230)
#define PHY_OM_CT_F_CALIDCOCS_SET_CNF (0x10230)
#define PHY_OM_CT_F_CALIDCOCS_IND (0x10231)

/*^FCALITXIQ IDs*/
#define OM_PHY_CT_F_CALITXIQ_SET_REQ (0x10240)
#define PHY_OM_CT_F_CALITXIQ_SET_CNF (0x10240)
#define PHY_OM_CT_F_CALITXIQ_IND (0x10241)

/*^FGAINSTATES IDs*/
#define OM_PHY_CT_F_GAINSTATE_SET_REQ (0x10250)
#define PHY_OM_CT_F_GAINSTATE_SET_CNF (0x10250)

/*^FDBBATTS IDs*/
#define OM_PHY_CT_F_DBBATT_SET_REQ (0x10260)
#define PHY_OM_CT_F_DBBATT_SET_CNF (0x10260)

/*^FBBATTS IDs*/
#define OM_PHY_CT_F_BBATT_SET_REQ (0x10270)
#define PHY_OM_CT_F_BBATT_SET_CNF (0x10270)

#define OM_PHY_CT_TEST_MODE_REQ                      0X10000
#define PHY_OM_CT_TEST_MODE_CNF                      0X10000
#define OM_PHY_CT_TXON_REQ                           0X10020
#define PHY_OM_CT_TXON_CNF                           0X10020
#define OM_PHY_CT_FREQ_SET_REQ                       0X10040
#define PHY_OM_CT_FREQ_SET_CNF                       0X10040
#define OM_PHY_CT_TXPOW_SET_REQ                      0X10060
#define PHY_OM_CT_TXPOW_SET_CNF                      0X10060
#define OM_PHY_CT_VCTCXO_SET_REQ                     0X10080
#define PHY_OM_CT_VCTCXO_SET_CNF                     0X10080
#define OM_PHY_CT_RXON_REQ                           0X100A0
#define PHY_OM_CT_RXON_CNF                           0X100A0
#define OM_PHY_CT_RXANT_SET_REQ                      0X100C0
#define PHY_OM_CT_RXANT_SET_CNF                      0X100C0
#define OM_PHY_CT_AAGC_SET_REQ                       0X100E0
#define PHY_OM_CT_AAGC_SET_CNF                       0X100E0
#define OM_PHY_CT_MEAS_RSSI_REQ                      0X10100
#define PHY_OM_CT_MEAS_RSSI_CNF                      0X10100
#define PHY_OM_CT_RSSI_IND                           0X10110
#define OM_PHY_CT_VOLTAGE_REQ                        0X10111
#define PHY_OM_CT_VOLTAGE_CNF                        0X10111

#define PHY_OM_FAGC_CNF                              0X10112
#define OM_PHY_FAGC_REQ                              0X10112

#define OM_PHY_CT_TSELRF_SET_REQ                     0x10115
#define PHY_OM_CT_TSELRF_SET_CNF                     0x10115

#define OM_PHY_CT_FWAVE_SET_REQ                     0x10116
#define PHY_OM_CT_FWAVE_SET_CNF                     0x10116

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

#define OM_PHY_CT_F_BLK_REQ               (0x101c0)
#define PHY_OM_CT_F_BLK_CNF               (0x101c0)

#define OM_PHY_CT_F_IP2START_REQ          (0x101d0)
#define PHY_OM_CT_F_IP2START_CNF          (0x101d0)

#define PHY_OM_CT_F_IP2_IND               (0x101e0)

#define OM_PHY_CT_F_MAX_POWER_SET_REQ           (0X101f0)
#define PHY_OM_CT_F_MAX_POWER_SET_CNF           (0X101f0)

#define OM_PHY_CT_F_PA_POWER_SET_REQ           (0X101f4)
#define PHY_OM_CT_F_PA_POWER_SET_CNF           (0X101f4)

#define OM_PHY_CT_F_BANDSW_SET_REQ           (0X101f5)
#define PHY_OM_CT_F_BANDSW_SET_CNF           (0X101f5)
#define OM_PHY_CT_F_FDMS_REQ                 (0X101f6)
#define PHY_OM_CT_F_FDMS_CNF                 (0X101f6)
#define CT_F_PA_SET_MAX_NUM              (30)
#define CT_F_POWER_MAX_NUM               (30)
#define CT_F_AAGC_SET_MAX_NUM            (30)
#define CT_F_FREQ_MAX_NUM                (16)
#define CT_F_FREQ_UL_LIST_MAX_NUM        (32)
#define CT_F_FREQ_DL_LIST_MAX_NUM        (32)

#define CT_F_MAX_PA_LEVEL                4

#define CT_F_NOT_START                   0
#define CT_F_READY                       1
#define CT_F_RUNNING                     2
#define CT_F_COMPLETE                    3
#define CT_F_CW_WAVE                     0
#define CT_F_LTE_WAVE                    1
#define CT_F_FDD                         6
#define CT_F_TDD                         7
#define CT_F_MODE_OFFSET                 6
#define CT_F_ONLY_TX                     0
#define CT_F_ONLY_RX                     1
#define CT_F_BOTH_TXRX                   2
#define CT_F_PA_MAX_PWR                3

#define CT_F_HIGH_BAND                    2
#define CT_F_MID_BAND                     1
#define CT_F_LOW_BAND                     0

#define CT_MAX_PWR_CALIBRATE_RUNNING            1
#define CT_MAX_PWR_CALIBRATE_COMPLETE           0

#define CT_MAX_FREQ_NUM                                 16

#define CT_F_PA_OFF                          0
#define CT_F_PA_ON                           1

#define CT_TXEN_OFF                          0
#define CT_TXEN_ON                           1

#define CT_FORCE_TX_OFF                          0
#define CT_FORCE_TX_ON                           1


#define OM_CT_PRIM_NUM                       (23 + 6)

#define BAND_3     3
#define BAND_7     7
#define BAND_20     20
#define BAND_38     38
#define BAND_40     40
#define BAND_41     41

#define     CT_NULL                        (LPHY_NULL)
#define     CT_TRUE                        (LPHY_TRUE)
#define     CT_FALSE                       (LPHY_FALSE)
#define     CT_ERROR                       1

#define   CT_TX_OFF                       0
#define   CT_TX_ON                        1
#define   CT_TX_AFC                       2

#define   CT_F_RX_SYNC_TIME              5

#define   CT_MAX_ANT_NUM                    2
#define   CT_ANT0                           0
#define   CT_ANT1                           1

#define   CT_BIT0                           0x1
#define   CT_BIT1                           0x2

//#define CT_DSP_UP_MAILBOX_MSG_LEN ((LPHY_MAILBOX_CT_UP_SIZE - sizeof(MAIL_BOX_HEAD_STRU))/4)
#define CT_TX_SIN_WAVE_DATA_LEN   32


/*IP2У׼֧�����Ƶ����*/
#define CT_F_CALIIP2_MAX_CHAN_NUM 4

/*DCOCУ׼��AGC��λ����*/
#define CT_F_CALIIP2_CALIDCOCS_AGC_NUM 16

/*GainState����������*/
#define CT_F_GAINSTATE_MAX_NUM  60 

/*DBB˥������������*/
#define CT_F_DBBATT_MAX_NUM CT_F_GAINSTATE_MAX_NUM  

/*BB˥������������*/
#define CT_F_BBATT_MAX_NUM_CT CT_F_GAINSTATE_MAX_NUM 



/****************************************************************************************************
����:   IP2У׼������^FCALIIP2S
ID:     OM_PHY_CT_F_CALIIP2_SET_REQ,      PHY_OM_CT_F_CALIIP2_SET_CNF,      PHY_OM_CT_F_CALIIP2_IND
�ṹ:   OM_PHY_CT_F_CALIIP2_SET_REQ_STRU, PHY_OM_CT_F_CALIIP2_SET_CNF_STRU, PHY_OM_CT_F_CALIIP2_IND_STU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usChanNum;
	UINT16 usRsv;
	UINT16 usChan[CT_F_CALIIP2_MAX_CHAN_NUM];
}OM_PHY_CT_F_CALIIP2_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_CALIIP2_SET_CNF_STRU;

typedef struct 
{ 
	UINT32 ulMsgId;
	UINT16 usChanNum;
	UINT16 usRsv;
	UINT16 usMain_I_DivOff[CT_F_CALIIP2_MAX_CHAN_NUM];
	UINT16 usMain_Q_DivOff[CT_F_CALIIP2_MAX_CHAN_NUM];
	UINT16 usMain_I_DivOn[CT_F_CALIIP2_MAX_CHAN_NUM];
	UINT16 usMain_Q_DivOn[CT_F_CALIIP2_MAX_CHAN_NUM];
	UINT16 usDiv_I[CT_F_CALIIP2_MAX_CHAN_NUM];
	UINT16 usDiv_Q[CT_F_CALIIP2_MAX_CHAN_NUM];
}PHY_OM_CT_F_CALIIP2_IND_STRU;

/****************************************************************************************************
����:   DCOCУ׼������^FCALIDCOCS
ID:     OM_PHY_CT_F_CALIDCOCS_SET_REQ,      PHY_OM_CT_F_CALIDCOCS_SET_CNF,      PHY_OM_CT_F_CALIDCOCS_IND
�ṹ:   OM_PHY_CT_F_CALIDCOCS_SET_REQ_STRU, PHY_OM_CT_F_CALIDCOCS_SET_CNF_STRU, PHY_OM_CT_F_CALIDCOCS_IND_STRU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usChannel;
	UINT16 usRsv;
}OM_PHY_CT_F_CALIDCOCS_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_CALIDCOCS_SET_CNF_STRU;

typedef struct 
{ 
    UINT32 ulMsgId;
    UINT16 usNOBLK_ANT0_I[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usNOBLK_ANT0_Q[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT0_I[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT0_Q[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usNOBLK_ANT1_I[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usNOBLK_ANT1_Q[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT1_I[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT1_Q[CT_F_CALIIP2_CALIDCOCS_AGC_NUM];
}PHY_OM_CT_F_CALIDCOCS_IND_STRU;

/****************************************************************************************************
����:   TXIQУ׼������^FTXIQ
ID:     OM_PHY_CT_F_CALITXIQ_SET_REQ,       PHY_OM_CT_F_CALITXIQ_SET_CNF,       PHY_OM_CT_F_CALITXIQ_IND
�ṹ:   OM_PHY_CT_F_CALIDCOCS_SET_REQ_STRU, PHY_OM_CT_F_CALIDCOCS_SET_CNF_STRU, PHY_OM_CT_F_CALIDCOCS_IND_STRU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usChannel;
	UINT16 usRsv;
}OM_PHY_CT_F_CALITXIQ_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_CALITXIQ_SET_CNF_STRU;

typedef struct 
{ 
	UINT32 ulMsgId;
	UINT16 usAmplitude;
	UINT16 usPhase;
	UINT16 usDCI;
	UINT16 usDCQ;
}PHY_OM_CT_F_CALITXIQ_IND_STRU;

/****************************************************************************************************
����:   ����GainState����������^FGAINSTATES
ID:     OM_PHY_CT_F_GAINSTATE_SET_REQ,       PHY_OM_CT_F_GAINSTATE_SET_CNF   
�ṹ:   OM_PHY_CT_F_CALIDCOCS_SET_REQ_STRU, PHY_OM_CT_F_CALIDCOCS_SET_CNF_STRU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usGainStateNum;
	UINT16 usRsv;
	UINT16 usGainState[CT_F_GAINSTATE_MAX_NUM];
}OM_PHY_CT_F_GAINSTATE_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_GAINSTATE_SET_CNF_STRU;

/****************************************************************************************************
����:   ����DBB˥������������^FDBBATTS
ID:     OM_PHY_CT_F_DBBATT_SET_REQ,         PHY_OM_CT_F_DBBATT_SET_CNF   
�ṹ:   OM_PHY_CT_F_CALIDCOCS_SET_REQ_STRU, PHY_OM_CT_F_CALIDCOCS_SET_CNF_STRU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usDbbAttNum;
	UINT16 usRsv;
	UINT16 usDbbAtt[CT_F_DBBATT_MAX_NUM];
}OM_PHY_CT_F_DBBATT_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_DBBATT_SET_CNF_STRU;

/****************************************************************************************************
����:   ����BB˥������������^FBBATTS
ID:     OM_PHY_CT_F_BBATT_SET_REQ,      PHY_OM_CT_F_BBATT_SET_CNF   
�ṹ:   OM_PHY_CT_F_BBATT_SET_REQ_STRU, PHY_OM_CT_F_BBATT_SET_CNF_STRU
******************************************************************************************************/
typedef struct  
{
	UINT32 ulMsgId;
	UINT16 usBbAttNum;
	UINT16 usRsv;
	UINT16 usBbAtt[CT_F_BBATT_MAX_NUM_CT];
}OM_PHY_CT_F_BBATT_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_BBATT_SET_CNF_STRU;



/*****************************************************************************
 ���� : MSP�յ�AT^TMODE������ж�Ϊ������ģʽʱ�·���ԭ���֪RTT��
        AT����������ģʽ���·���ԭ��
 ID   : OM_PHY_CT_TEST_MODE_REQ,PHY_OM_CT_TEST_MODE_CNF
 REQ  : OM_PHY_CT_TEST_MODE_REQ_STRU
 CNF  : PHY_OM_CT_TEST_MODE_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    /*  1:��������ͨУ׼  */
    /*  6:���������У׼  */
    UINT16 usTMode;     /* ģʽ����  */
    UINT16 usRsv;
} OM_PHY_CT_TEST_MODE_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_TEST_MODE_CNF_STRU;


/*****************************************************************************
 ���� : ָʾ����������ͨ����RTT�յ���ԭ������ͨ������һֱ�򿪣�ֱ��MSPͨ����ԭ��ر����С�
 ID   : OM_PHY_CT_TXON_REQ,PHY_OM_CT_TXON_CNF
 REQ  : OM_PHY_CT_TXON_REQ_STRU
 CNF  : PHY_OM_CT_TXON_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* 0:��ʾ�رշ����  */
	/* 1:��ʾ�򿪷�����������������ƺ�RF����ͨ��  */
    /* 2:��ʾֻ��RFIC Transmitter��PA ��BB����ͨ�����򿪣�����VCTCXOУ׼  */
    UINT16 usTxType;                     /*Tx������*/
    UINT16 usRsv;
} OM_PHY_CT_TXON_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_TXON_CNF_STRU;

/*****************************************************************************
 ���� : ����ǰ����ͨ���򿪣�Tx����Ϊ1��2������RTT��Ϊ��ԭ����������Ƶ�㣬������ͨ���رգ�
        ����Ϊ������Ƶ�㡣MSP�뽫AT^FCHAN�����е��ŵ��Ÿ���36.101Э��涨���תΪʵ��Ƶ��ֵ��
        ת�����̲ο���¼4��
 ID   : OM_PHY_CT_FREQ_SET_REQ,PHY_OM_CT_FREQ_SET_CNF
 REQ  : OM_PHY_CT_FREQ_SET_REQ_STRU
 CNF  : PHY_OM_CT_FREQ_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* Ƶ����Ϣ  */
    /* Ƶդ���100Khz��������ֵ26200��ӦƵ��2.62GHz  */
    UINT16 usFreqInfo;                   /*Ƶ����Ϣ*/
    UINT16 usULMode;                     /*FDD:6     TDD:7*/
} OM_PHY_CT_FREQ_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_FREQ_SET_CNF_STRU;


/*****************************************************************************
 ���� : �������з��书�ʡ�RTT�յ���ԭ���Ӧ�����ı����з��书��
 ID   : OM_PHY_CT_TXPOW_SET_REQ,PHY_OM_CT_TXPOW_SET_CNF
 REQ  : OM_PHY_CT_FREQ_SET_REQ_STRU
 CNF  : PHY_OM_CT_FREQ_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* ��λ��dBm  */
    INT16  sTxPower;                     /*���з��书��*/
    UINT16 usRsv;
} OM_PHY_CT_TXPOW_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_TXPOW_SET_CNF_STRU;

/*****************************************************************************
 ���� : �������з��书�ʡ�RTT�յ���ԭ���Ӧ�����ı����з��书��
 ID   : OM_PHY_CT_VCTCXO_SET_REQ,PHY_OM_CT_VCTCXO_SET_CNF
 REQ  : OM_PHY_CT_FREQ_SET_REQ_STRU
 CNF  : PHY_OM_CT_FREQ_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/

typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* VCTCXO���Ƶ�ѹ		[0,65535]  */
    UINT16 usVctcxoCtrl;                 /*Vctcxo������*/
    UINT16 usRsv;
} OM_PHY_CT_VCTCXO_SET_REQ_STRU;


typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_VCTCXO_SET_CNF_STRU;

/*****************************************************************************
 ���� : ָʾ���������ջ�������������ڽ��������ģʽ�Ѵ򿪽���˫ͨ����
        �˴��յ�ԭ��������մ���ֱ�ӷ���CNFԭ��
 ID   : OM_PHY_CT_RXON_REQ,PHY_OM_CT_RXON_CNF
 REQ  : OM_PHY_CT_RXON_REQ_STRU
 CNF  : PHY_OM_CT_RXON_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/

typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_CT_RXON_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_RXON_CNF_STRU;

/*****************************************************************************
 ���� : ����RXͨ����RTT�յ���ԭ���ѡ����Ҫ����RSSI������ϱ��Ľ���ͨ����
      ע��RTT�ڷ�����ģʽ��Rx˫ͨ����һֱ�򿪡�����ԭ��ָʾ�ر�˫ͨ����
      RTT�����κδ���
 ID   : OM_PHY_CT_RXANT_SET_REQ,PHY_OM_CT_RXANT_SET_CNF
 REQ  : OM_PHY_CT_RXANT_SET_REQ_STRU
 CNF  : PHY_OM_CT_RXANT_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/

typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* 0:�ر�˫ͨ��  */
    /* 1:ֻ��ͨ��1 */
    /* 2:ֻ��ͨ��2 */
    /* 3:��˫ͨ��  */
    UINT16 usRxAntType;                  /* RXͨ��ѡ�� */
    UINT16 usRsv;
} OM_PHY_CT_RXANT_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_RXANT_SET_CNF_STRU;

/*****************************************************************************
AT^TSELRF
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT8  ucPath;
    UINT8  ucGroup;
    UINT16 usRsv;
} OM_PHY_CT_TSELRF_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_TSELRF_SET_CNF_STRU;

/*****************************************************************************
AT^FWAVE
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT16 usType;                       /*0:��ʾcontinues wave�ź�*/
    INT16 usPower;                       /*2300:23db*/
} OM_PHY_CT_FWAVE_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_FWAVE_SET_CNF_STRU;
/*****************************************************************************
 ���� : ����RFIC��AAGC�����棬��������AGC���У׼����λΪdB��
      RTT�յ���ԭ���RFIC��������������Ϊԭ��ָʾ������ֵ��
 ID   : OM_PHY_CT_AAGC_SET_REQ,PHY_OM_CT_AAGC_SET_CNF
 REQ  : OM_PHY_CT_AAGC_SET_REQ_STRU
 CNF  : PHY_OM_CT_AAGC_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */

    /* [0,127]	��λ:dB��  */
    UINT16 usAAGCValue;                  /* AAGC����ֵ */
    UINT16 usRsv;
} OM_PHY_CT_AAGC_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_AAGC_SET_CNF_STRU;

/*****************************************************************************
 ���� :ָʾRTT���������RSSI��������������AGC���У׼��RTT�յ���ԭ�������RSSI
       ��������ɺ�ͨ��PHY_OM_CT_RSSI_INDԭ���ϱ���ӦRXͨ����RSSI�������
 ID   : OM_PHY_CT_MEAS_RSSI_REQ,PHY_OM_CT_MEAS_RSSI_CNF
 REQ  : OM_PHY_CT_MEAS_RSSI_REQ_STRU
 CNF  : PHY_OM_CT_MEAS_RSSI_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_CT_MEAS_RSSI_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_MEAS_RSSI_CNF_STRU;
/*****************************************************************************
 ���� :ָʾ������������������ѹ������
 ID   :
 REQ  : OM_PHY_CT_VOLTAGE_REQ_STRU
 CNF  : PHY_OM_CT_VOLTAGE_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_CT_VOLTAGE_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
    UINT32 ulVoltage;
} PHY_OM_CT_VOLTAGE_CNF_STRU;

/*****************************************************************************
 ���� :ָʾRTT���������RSSI��������������AGC���У׼��RTT�յ���ԭ�������RSSI
       ��������ɺ�ͨ��PHY_OM_CT_RSSI_INDԭ���ϱ���ӦRXͨ����RSSI�������
 ID   : PHY_OM_CT_RSSI_IND
 REQ  : NA
 CNF  : NA
 IND  : PHY_OM_CT_RSSI_IND_STRU
 ˵�� : 
*****************************************************************************/ 

/* usRxANT1ValidFlag	Rxͨ��1��Ч��־		[0,1]	0:Rxͨ��1��Ч��sRxANT1RSSIֵ��Ч*/
/* 1:Rxͨ��1��Ч��sRxANT1RSSIֵ��Ч*/
/* sRxANT1RSSI	Rxͨ��1��RSSI		[-110*8,-20*8]	��λ:1/8dBm��ֻ��usRxANT1ValidFlag=1��Ч*/
/* usRxANT2ValidFlag	Rxͨ��2��Ч��־		[0,1]	0:Rxͨ��2��Ч��sRxANT2RSSIֵ��Ч*/
/* 1:Rxͨ��2��Ч��sRxANT2RSSIֵ��Ч*/
/* sRxANT2RSSI	Rxͨ��2��RSSI		[-110*8,-20*8]	��λ:1/8dBm��ֻ��usRxANT2ValidFlag=1��Ч*/
typedef struct
{
    UINT32  ulMsgId;
    UINT16  usRxANT1ValidFlag;
    UINT16  usRxANT1RSSI;
    UINT16  usRxANT2ValidFlag;
    UINT16  usRxANT2RSSI;
} PHY_OM_CT_RSSI_IND_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_FAGC_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
    INT16 sRsrp;
    INT16 sRsv;
} PHY_OM_FAGC_CNF_STRU;


/*DSP��MSPԼ����ԭ��ṹ*/
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
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT32 ulMode;                       /* MODE TDD��7��FDD��6  */
    UINT16 usUlFreqNum; /* �������õĸ�������ӦausUlFreqInfo�����16��  */
    UINT16 usDlFreqNum; /* �������õĸ�������ӦausDlFreqInfo�����16��  */

    /* Ƶ����Ϣ��  */
    /* Ƶդ���100Khz��������ֵ26200��ӦƵ��2.62GHz  */
    UINT16 ausUlFreqInfo[CT_F_FREQ_UL_LIST_MAX_NUM];       /* ����Ƶ����Ϣ  */
    UINT16 ausDlFreqInfo[CT_F_FREQ_DL_LIST_MAX_NUM];       /* ����Ƶ����Ϣ  */
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
typedef struct
{
    UINT32 ulMsgId;                          /* ��Ϣ��ʶ */

    /* ��λ��dBm  */
    UINT16 usTxPowerNum;                     /* ָ����asTxPower�ĸ���  */
    UINT16 usRsv;
    /* ���书���б��Կո�ָÿ��ֵ�ĳ���Ϊһ���� ����λΪdBm
    INT16  asTxPower[CT_F_POWER_MAX_NUM];  /*���з��书��*/
} OM_PHY_CT_F_TXPOW_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_TXPOW_SET_CNF_STRU;

/*****************************************************************************
 ���� : ����ʹ��APT��־
 ID   : OM_PHY_CT_F_TXPOW_SET_REQ,PHY_OM_CT_F_TXPOW_SET_CNF
 REQ  : OM_PHY_CT_F_TXPOW_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_TXPOW_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                          /* ��Ϣ��ʶ */
    UINT32 ulAptEnbFlg;                      /*ָʾAPT�Ƿ�ʹ��*/
} OM_PHY_CT_F_FDMS_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_CT_F_FDMS_CNF_STRU;

/*****************************************************************************
 ���� : ����RFIC��AAGC�����棬��������AGC���У׼����λΪdB��
      RTT�յ���ԭ���RFIC��������������Ϊԭ��ָʾ������ֵ��
 ID   : OM_PHY_CT_F_AAGC_SET_REQ,PHY_OM_F_CT_AAGC_SET_CNF
 REQ  : OM_PHY_CT_F_AAGC_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_AAGC_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT16 usAagcNum;
    UINT16 usRsv;

    /*ȡֵ��Χ0��255������Hi6910�ķ�ΧΪ151-234��  */
    UINT8 aucAAGCValue[CT_F_AAGC_SET_MAX_NUM];                  /* AAGC����ֵ */
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
typedef struct
{
    /*
    ATָ�-װ������:
    ÿ��ֵ������ֵ��ʾ����ȷ��0.125dBm����ֵ��Ϣ��ȡֵ��ΧΪ160~880��
    �����ǰ��RSSIֵΪ-85.125dBm����ΧֵΪ-85.125dbm*(-1)/0.125dbm = 681
    */
    INT16  sRxAnt1Rssi;
    INT16  sRxAnt2Rssi;
} CT_F_RSSI_STRU;

typedef struct
{
    UINT32 ulMsgId;
    UINT16 usFreqNum;
    UINT16 usPowNum;

    CT_F_RSSI_STRU  astRxRssi[CT_F_FREQ_MAX_NUM][CT_F_POWER_MAX_NUM];
} PHY_OM_CT_F_MEAS_RSSI_IND_STRU;

/*****************************************************************************
 ���� : ָʾRTT����PA
 ID   : OM_PHY_CT_F_PA_SET_REQ,PHY_OM_CT_F_PA_SET_CNF
 REQ  : OM_PHY_CT_F_PA_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_PA_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT16 usLevelNum;
    UINT16 usRsv;
    UINT8  aucLvlList[CT_F_PA_SET_MAX_NUM];
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
    UINT16 usMeasOffset;                   /* ��ʾ������ʼλ��ƫ�ƣ���λms��ȡֵ��Χ0~20����Ĭ��ֵ*/
    UINT16 usMeasLen;                      /* ��ʾ�������ȣ���λms��ȡֵ��Χ0~20����Ĭ��ֵ */
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

    /* 0    CW����  */
    /* 1    LTE����  */
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

    /* 0    ����ͨ��У׼  */
    /* 1    ����ͨ��У׼������Ĭ��Ϊ˫ͨ����  */
    /* 2    ����/����ͨ��ͬʱУ׼������Ĭ��Ϊ˫ͨ����  */
    UINT8 ucType;

    UINT8 ucRsv[3];
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

    /* 0    ����ͨ��У׼  */
    /* 1    ����ͨ��У׼������Ĭ��Ϊ˫ͨ����  */
    /* 2    ����/����ͨ��ͬʱУ׼������Ĭ��Ϊ˫ͨ����  */
    UINT8 ucType;

    /* ����У׼״̬  */
    /* 0    δ������ִ�����  */
    /* 1    ִ����  */
    UINT8 ucStatus;

    UINT8 ucRsv[2];
} PHY_OM_CT_F_TRIGGER_RD_CNF_STRU;

/*****************************************************************************
 ���� : ��ѯ����AGC��BLK����״̬
 ID   : OM_PHY_CT_F_BLK_REQ,PHY_OM_CT_F_BLK_CNF       
 REQ  : OM_PHY_CT_F_BLK_REQ_STRU
 CNF  : PHY_OM_CT_F_BLK_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ   */

    /* 0    BLOCKING ����  */
    /* 1    NOBLOCKING ����  */
    UINT16 usISBLKMode;                 /*��������   */    
    UINT16 usRsv;
} OM_PHY_CT_F_BLK_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ  */
} PHY_OM_CT_F_BLK_CNF_STRU;


/*****************************************************************************
 ���� : IP2У׼
 ID   : OM_PHY_CT_F_IP2START_REQ,PHY_OM_CT_F_IP2START_CNF, PHY_OM_CT_F_IP2START_IND      
 REQ  : OM_PHY_CT_F_IP2START_REQ_STRU
 CNF  : PHY_OM_CT_F_IP2START_CNF_STRU
 IND  : PHY_OM_CT_F_IP2_IND_STRU
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ   */
    UINT16 usBand;                 /* IP2У׼bandֵ  */
    UINT16 usRsv;
} OM_PHY_CT_F_IP2START_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ  */
} PHY_OM_CT_F_IP2START_CNF_STRU;


typedef struct
{
    UINT32 ulMsgId;       /*��Ϣ��ʶ  */
    INT16 ulIP2Value1;     /*IP2У׼���1  */
    INT16 ulIP2Value2;     /*IP2У׼���2  */
} PHY_OM_CT_F_IP2_IND_STRU;

/*****************************************************************************
 ���� : MAXPOWERУ׼����
 ID   : OM_PHY_CT_F_MAX_POWER_SET_REQ,PHY_OM_CT_F_MAX_POWER_SET_CNF     
 REQ  : OM_PHY_CT_F_MAX_POWER_SET_REQ_STRU
 CNF  : PHY_OM_CT_F_MAX_POWER_SET_CNF_STRU
 IND  : NAN
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ   */
    UINT16 usPaLevelNum;                 /*��ҪУ׼��PA ��λ��Ŀ  */
    UINT16 usPaReduceGain;               /*PA���˵ĵ�λ  */
} OM_PHY_CT_F_MAX_POWER_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ  */
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
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ   */
    UINT16 usMode;                       /*TDD:7 FDD:6  */
    UINT16 usBand;                       /*bandֵ3 7 20 38 40 41  */
} OM_PHY_CT_F_BANDSW_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ  */
} PHY_OM_CT_F_BANDSW_SET_CNF_STRU;
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
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ   */
    UINT16 usPaLevelNum;                /*PA��λ��Ŀ  */
    UINT16 usRsv;
    INT16 asMaxPwrList[4];              /*У׼���MAXPOWERֵ  */
} OM_PHY_CT_F_PA_POWER_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;                      /*��Ϣ��ʶ  */
} PHY_OM_CT_F_PA_POWER_SET_CNF_STRU;

typedef struct
{
    UINT32 ulMsgId;
}PHY_MSP_CT_SINGLE_CNF_STRU;
typedef struct
{
    UINT16 usCtTestModeFlg;/*����ģʽ��� 0: ����ģʽ��ʹ�� 1: ����ģʽʹ��*/
    UINT16 usTxTestMode1Flg;/*���Ͳ���ģʽ1ʹ�ܱ��*/
    UINT16 usTXModeFlg;     /*TX���ͱ�� 0:TXδ�� 1:TX��,Ϊ�ı�Ƶ��ʹ��*/
    UINT16 usFreq;
    UINT16 usRxBW;
    UINT16 usAGC;
    UINT16 usTxFreq;
    UINT16 usTxBW;
    UINT16 usTxPow;
    UINT16 usRxAntType;/*1:ͨ��1 ��2:ͨ��2 ;3:ͨ��1,2  */
    INT16  sMeasSbNum;
    UINT16 usMeasTask;
    UINT16 usTxPowChangeFlg;
    INT16  sTxPowSbNum;
    UINT16 usTxType;
    UINT16 usVoltageGetFlg;
} DSP_CT_CTRL_INFO_STRU;

/*DSP�Զ������ݽṹ*/
typedef struct
{
    UINT16 usWave;                                 /* CW����0�� LTE����1  */
    UINT16 usSegment;                              /* ���͵�һ����ʱ������λms  */

    UINT16 usFreqNum;
    UINT16 ausFreqList[CT_F_FREQ_MAX_NUM];

    UINT16 usPowerNum;
    INT16  asPowerValue[CT_F_POWER_MAX_NUM];

    UINT16 usGainNum;
    UINT16 usGainState[CT_F_GAINSTATE_MAX_NUM];

    INT16  usBbAtt;
    UINT16 usDbbAtt;
    
    UINT16 usPaNum;
    UINT8  aucPaLevel[CT_F_POWER_MAX_NUM];
    UINT32 ulAptEnbFlg;
}CT_F_TX_PARA_STRU;

typedef struct
{
    UINT16 usSegment;                              /*���յ�һ����ʱ������λms  */
    UINT16 usMeasOffset;                           /*RSSI����ʱ��ƫ��  */
    UINT16 usMeasTime;                             /*����ʱ��  */

    UINT16 usFreqNum;
    UINT16 ausFreqList[CT_F_FREQ_MAX_NUM];

    UINT16  usGainNum;
	UINT16  usRsv;
    UINT8  aucRfGain[CT_F_POWER_MAX_NUM];        /*RF����  */
    UINT16  aucRfControlWord[CT_F_POWER_MAX_NUM];        /*RF ������  */
    INT16   aucAagcGain[CT_MAX_ANT_NUM][CT_F_POWER_MAX_NUM];       
}CT_F_RX_PARA_STRU;

typedef struct
{
    UINT16 usTddFddSel;                            /*FDD:6, TDD:7  */
    UINT16 usTxRxSel;                              /*ֻУTX:0,ֻУRX: 1,TX RXͬʱУ��2  */
    CT_F_TX_PARA_STRU stTxPara;
    CT_F_RX_PARA_STRU stRxPara;
    UINT16 usBand;
    UINT16 usRsv;
}CT_F_PARA_STRU;

typedef struct
{
    UINT16 usSubFrameCnt;                          /*�Ѳ��Ե���֡����  */
    UINT16 usSegmentCnt;                           /*У׼��ɵĹ��ʼ���  */
    UINT16 usSequenceCnt;                          /*У׼��ɵ�Ƶ�����  */
}CT_F_CNT_STRU;

typedef struct
{
    UINT16        usTempReadFlag;                  /*һ���¶ȶ�ȡ��ɱ�־  */
    UINT16        usTxStatus;                      /*��ɣ�0������У׼��1  */
    UINT16        usTxSwitchedFlag;                /*�Ѿ��л�һ�α�־  */
    CT_F_CNT_STRU stTxCnt;
    UINT16        usTxFreqStatus;                 /*��ǰУ׼Ƶ���Ƿ���Ч  */

    UINT16        usRxStatus;                      /*��ɣ�0������У׼��1
    UINT16        usRxSyncInd;                     /*δ������ͬ���ϣ�0,ͬ���ϣ�1  */
    UINT16        usRxSwitchedFlag;                /*�Ѿ��л�һ�α�־  */
    CT_F_CNT_STRU stRxCnt;
    UINT16        usRxIsBlk;                       /*Rx����У׼ʱ���龳ģʽ  0:noblocking   1:blocking  */
    UINT16        usIP2Band;                       /*IP2 bandֵ  */
    UINT16        usRFICStatus;
    UINT16        usWaitCnt;
    UINT16        usMaxPwrInd;                    /*ָʾ���ڽ���max PowerУ׼ ��У׼�Ḵ��TxУ׼�����ݽṹ  */
    UINT16        usPaReduceGain;                 /*У׼maxpowerʱPA���˵ĵ�λ  */
    UINT16        usIp2CalStatus;
    UINT16        usDcocsCalStatus;
    UINT16        usTxiqCalStatus;
}CT_F_CTRL_STRU;

typedef struct
{
    UINT16 usTxPowerSwitchCnt;
    UINT16 usTxFreqSwitchCnt;
    UINT16 usTxSubFrameCnt;
    UINT16 usRxPowerSwitchCnt;
    UINT16 usRxFreqSwitchCnt;
    UINT16 usRxSubFrameCnt;
}CT_F_DEBUG_STRU;

typedef struct
{
	UINT8 usTmodeError;/*У׼ģʽ����*/
	UINT8 usSetPaLevelError;/*����PAʱ��λ��������*/
	UINT8 usUpMailBoxFullError;/*�ϱ���������*/
	UINT8 usPaSwitchTypeError;/*�л�PA Ŀ��״̬��������*/
	UINT8 usPaSwitchBandError;/*�л�PA band��֧��*/
	UINT8 usTxTypeError;/*������CT_TX_ON CT_TX_OFF CT_TX_AFC*/
	UINT8 usRsv[2];
}CT_F_ERROR_TYPE_STRU;

   
typedef struct
{
    UINT32  ulMsgId;
    UINT16  usTempNum;                  /* �¶�ֵ����  */
    UINT16  usType;                         /* TX: 0  RX: 1 TXRX: 2  */
    UINT16  ausVol4Temp[CT_F_FREQ_MAX_NUM]; /* �Ե�ѹ�������¶�ֵ  */
}PHY_OM_CT_F_MEAS_CMTMS_IND_STRU;


typedef struct
{
    UINT32 enMsgId;
    UINT16 usOpId;
    UINT8  aucRsv[2];
}LMAC_PHY_TEMP_REQ_STRU;

typedef struct
{
    UINT32 enMsgId;
    UINT16 usOpId;
    INT16  sCurrentTemp;
}LMAC_PHY_TEMP_IND_STRU;

typedef struct
{
    UINT16 usOpId;
    UINT16 usReq;
    INT16  sTemp;
    UINT16 usVol;
}UL_MAC_GET_TEMP_STRU;

typedef struct
{
    UINT32 ulMsgID;
    void (*pfFunc)(UINT32*);
    UINT32 ulCount;
}OM_CT_PRIMITIVE_STRU;

/* CT�ϱ�����ṹ�嶨�� */
//typedef struct
//{
//    MAIL_BOX_HEAD_STRU MailBoxHead;         /* ����ͷ*/
//    UINT32             aMessage[CT_DSP_UP_MAILBOX_MSG_LEN]; /* ��Ϣ֡,���ڴ洢ԭ��͸�ԭ�ﳤ��  */
//} CT_DSP_UP_MAIL_BOX_STRU;

#endif
