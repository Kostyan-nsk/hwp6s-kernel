#ifndef _DSP_CT_EX2_H
#define _DSP_CT_EX2_H

#undef UINT32
#define UINT32 unsigned long

#undef INT32
#define INT32 long

#undef  UINT16
#define UINT16 unsigned short

#undef INT16
#define INT16 short

#undef UINT8
#define UINT8 unsigned char

#undef INT8
#define INT8 char

//^FCALIIP2S IDs
#define OM_PHY_CT_F_CALIIP2_SET_REQ (0x10220)
#define PHY_OM_CT_F_CALIIP2_SET_CNF (0x10220)
#define PHY_OM_CT_F_CALIIP2_IND (0x10221)

//^FCALIDCOCS IDs
#define OM_PHY_CT_F_CALIDCOCS_SET_REQ (0x10230)
#define PHY_OM_CT_F_CALIDCOCS_SET_CNF (0x10230)
#define PHY_OM_CT_F_CALIDCOCS_IND (0x10231)

//^FCALITXIQ IDs
#define OM_PHY_CT_F_CALITXIQ_SET_REQ (0x10240)
#define PHY_OM_CT_F_CALITXIQ_SET_CNF (0x10240)
#define PHY_OM_CT_F_CALITXIQ_IND (0x10241)

//^FGAINSTATES IDs
#define OM_PHY_CT_F_GAINSTATE_SET_REQ (0x10250)
#define PHY_OM_CT_F_GAINSTATE_SET_CNF (0x10250)

//^FDBBATTS IDs
#define OM_PHY_CT_F_DBBATT_SET_REQ (0x10260)
#define PHY_OM_CT_F_DBBATT_SET_CNF (0x10260)

//^FBBATTS IDs
#define OM_PHY_CT_F_BBATT_SET_REQ (0x10270)
#define PHY_OM_CT_F_BBATT_SET_CNF (0x10270)

//IP2У׼֧�����Ƶ����
#define CT_F_CALIIP2_MAX_CHAN_NUM 4

//DCOCУ׼��AGC��λ����
#define CT_F_CALIDCOCS_AGC_NUM 16

//GainState����������
#define CT_F_GAINSTATE_MAX_NUM  60 

//DBB˥������������
#define CT_F_DBBATT_MAX_NUM CT_F_GAINSTATE_MAX_NUM  

//BB˥������������
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
	UINT16 usNOBLK_ANT1_I[CT_F_CALIDCOCS_AGC_NUM];
    UINT16 usNOBLK_ANT1_Q[CT_F_CALIDCOCS_AGC_NUM];
	UINT16 usBLK_ANT1_I[CT_F_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT1_Q[CT_F_CALIDCOCS_AGC_NUM];
	UINT16 usNOBLK_ANT2_I[CT_F_CALIDCOCS_AGC_NUM];
    UINT16 usNOBLK_ANT2_Q[CT_F_CALIDCOCS_AGC_NUM];
	UINT16 usBLK_ANT2_I[CT_F_CALIDCOCS_AGC_NUM];
    UINT16 usBLK_ANT2_Q[CT_F_CALIDCOCS_AGC_NUM];
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
}OM_PHY_CT_F_TXIQ_SET_REQ_STRU;

typedef struct  
{
	UINT32 ulMsgId;
}PHY_OM_CT_F_TXIQ_SET_CNF_STRU;

typedef struct 
{ 
	UINT32 ulMsgId;
	UINT16 usAmplitude;
	UINT16 usPhase;
	UINT16 usDCI;
	UINT16 usDCQ;
}PHY_OM_CT_F_TXIQ_IND_STRU;

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

#endif //end of _DSP_CT_EX2_H