

/*****************************************************************************
 ����˵����
    1�����ļ���MSP����Э��DSP��ɣ��������ƶ�Ӧ�����������ṩ��ԭ��
    2����������FOLLOW DSP�ĵ���������
    3������ά����DSPά��
    4���Ƿ���뷵��ERR_CODE���ĵ�������ϣ��ң�Ŀǰ����ԭ�ĵ�
*****************************************************************************
*****************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

*****************************************************************************
  �� �� ��   : dsp_ct.h
  �� �� ��   : V1.0 
  ��    ��   : DSP
  ��������   : 2008��07��07��
  ��������   : ���ļ�������DD800��CT��MSP��DSP������ԭ�Ｐ�������������
  �޸���ʷ   : 
  1.��    �� : 2008��07��07��
    ��    �� : MSP 
    �޸����� : �����ļ� 

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

/* DSP CT ���Χ[0x00010000,0x00010F00)*/
/* CT��������Ϊ��(0x00010000=<DSP CMDID<0x00010F00)*/
#define OM_PHY_CT_TEST_MODE_REQ		                 0X10000
#define PHY_OM_CT_TEST_MODE_CNF		                 0X10000
#define OM_PHY_CT_TXON_REQ	                         0X10020
#define PHY_OM_CT_TXON_CNF		                     0X10020
#define OM_PHY_CT_FREQ_SET_REQ	                     0X10040
#define PHY_OM_CT_FREQ_SET_CNF		                 0X10040
#define OM_PHY_CT_TXPOW_SET_REQ	                     0X10060
#define PHY_OM_CT_TXPOW_SET_CNF		                 0X10060
#define OM_PHY_CT_VCTCXO_SET_REQ	                 0X10080
#define PHY_OM_CT_VCTCXO_SET_CNF		             0X10080
#define OM_PHY_CT_RXON_REQ	                         0X100A0
#define PHY_OM_CT_RXON_CNF		                     0X100A0
#define OM_PHY_CT_RXANT_SET_REQ	                     0X100C0
#define PHY_OM_CT_RXANT_SET_CNF		                 0X100C0
#define OM_PHY_CT_AAGC_SET_REQ	                     0X100E0
#define PHY_OM_CT_AAGC_SET_CNF		                 0X100E0
#define OM_PHY_CT_MEAS_RSSI_REQ	                     0X10100
#define PHY_OM_CT_MEAS_RSSI_CNF		                 0X10100
#define PHY_OM_CT_RSSI_IND		                     0X10110

/* �¶Ȼ�ȡ�ӿ�*/
#define OM_PHY_CT_VOLTAGE_REQ		                 0X10111
#define PHY_OM_CT_VOLTAGE_CNF		                 0X10111

/* AT^FAGC*/
#define OM_PHY_FAGC_REQ		                         0X10112
#define PHY_OM_FAGC_CNF		                         0X10112

#define OM_PHY_CT_FPA_SET_REQ                        0x10113 /*AT^FPA*/
#define PHY_OM_CT_FPA_SET_CNF                        0x10113

#define OM_PHY_CT_FDAC_SET_REQ                       0x10114 /*AT^FDAC*/
#define PHY_OM_CT_FDAC_SET_CNF                       0x10114


#define OM_PHY_CT_TSELRF_SET_REQ                     0x10115 /*AT^TSELRF*/
#define PHY_OM_CT_TSELRF_SET_CNF                     0x10115
 
/* ���ش�ԭ��*/
/* ��Ӧ Polaris V100R001 ȫϵͳ��װ���汾��һ������Ф��*/
/* ������CT����*/
#define OM_PHY_POWER_CONFIG_REQ                      0x0053
#define PHY_OM_POWER_CONFIG_CNF                      0x0053


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
    
    /*1: ��������ͨУ׼ģʽ*/
    /*6�����������У׼ģʽ*/
    UINT16 usTMode;     /* ģʽ����*/
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
    
    /* 0:��ʾ�رշ����*/
	  /* 1:��ʾ�򿪷�����������������ƺ�RF����ͨ��*/
    /* 2:��ʾֻ��RFIC Transmitter��PA ��BB����ͨ�����򿪣�����VCTCXOУ׼*/
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
    
    /* Ƶ����Ϣ��*/
    /* Ƶդ���100Khz��������ֵ26200��ӦƵ��2.62GHz*/
    UINT16 usFreqInfo;                   /*Ƶ����Ϣ*/
    UINT16 usMode;    /*6,FDD;7,TDD */
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

    /* ��λ��dBm*/
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

    /* VCTCXO���Ƶ�ѹ		[0,65535]*/
    UINT16 usVctcxoCtrl;                 /*Vctcxo������*/
    UINT16 usRsv;                        
} OM_PHY_CT_VCTCXO_SET_REQ_STRU;


typedef struct
{
    UINT32 ulMsgId;  
} PHY_OM_CT_VCTCXO_SET_CNF_STRU;

/*****************************************************************************/

/*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT8  ucLevel;
    UINT8  ucPading[3];
                      
} OM_PHY_CT_FPA_SET_REQ_STRU;


typedef struct
{
    UINT32 ulMsgId;  
} PHY_OM_CT_FPA_SET_CNF_STRU;


/*****************************************************************************

*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
    UINT16  usApc;
    UINT16  usRsv;
                      
} OM_PHY_CT_FDAC_SET_REQ_STRU;


typedef struct
{
    UINT32 ulMsgId;  
} PHY_OM_CT_FDAC_SET_CNF_STRU;


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

    /* 0:�ر�˫ͨ��*/
    /* 1:ֻ��ͨ��1*/
    /* 2:ֻ��ͨ��2*/
    /* 3:��˫ͨ��*/
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

    /* [0,127]	��λ:dB��*/
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
    UINT32 ulMsgId;  
    UINT16 usRxANT1ValidFlag;
    INT16  sRxANT1RSSI;
    UINT16 usRxANT2ValidFlag;
    INT16  sRxANT2RSSI;
} PHY_OM_CT_RSSI_IND_STRU;

/* OM_PHY_POWER_CONFIG_REQ*/
/* PHY_OM_POWER_CONFIG_CNF*/
typedef struct
{
    UINT32 ulPuschPwrCtrlSwitch :2; /*0���رչ��� 1���򿪹���/�رձջ�  3���򿪹���/�򿪱ջ� 2�����ش�׮BTʱ����3*/
    UINT32 ulPucchPwrCtrlSwitch :2; /*0���رչ��� 1���򿪹���/�رձջ�  3���򿪹���/�򿪱ջ� 2�����ش�׮BTʱ����3*/
    UINT32 ulSrsPwrCtrlSwitch   :2; /*0���ص����� 1���򿪹��� 2�����ʴ�׮ ������Ч BTʱ����1*/
    UINT32 ulPrachPwrCtrlSwitch :2; /*0���ص����� 1���򿪹��� 2�����ʴ�׮ ������ЧBTʱ����1*/
    UINT32 ulFreqCmpnstSwitch :1;   /*0���ص�Ƶ�ʲ��� 1:��Ƶ�ʲ���BTʱ����1*/
    UINT32 ulTempCmpnstSwitch :1;   /*0���ص��¶Ȳ��� 1:���¶Ȳ���BTʱ����1*/
    UINT32 ulSpare :22;
}PHY_OM_UPLINK_PWR_CTRL_STRU;

typedef struct
{
    UINT32 ulMsgId;
    PHY_OM_UPLINK_PWR_CTRL_STRU stulPwrCtrlFlag;
    INT16  sPucchTxPower;
    INT16  sPuschTxPower;
    INT16  sSrsTxPower;
    INT16  sPrachTxPower;
}OM_PHY_POWER_CONFIG_REQ_STRU;

/*****************************************************************************
 ���� : AGC��ѯ
 ID   : OM_PHY_FAGC_REQ,PHY_OM_FAGC_CNF
 REQ  : OM_PHY_FAGC_REQ_STRU
 CNF  : PHY_OM_FAGC_CNF_STRU
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;                      /* ��Ϣ��ʶ */
} OM_PHY_FAGC_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
    INT16 sRsrp;
    INT16 sRsv;
} PHY_OM_FAGC_CNF_STRU;


#endif
