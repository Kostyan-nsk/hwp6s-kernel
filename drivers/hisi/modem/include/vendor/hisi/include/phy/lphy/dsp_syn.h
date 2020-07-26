

/*****************************************************************************

                  ��Ȩ���� (C), 2001-2012, ��Ϊ�������޹�˾

*****************************************************************************
  �� �� ��   : dsp_syn.h
  �� �� ��   : V1.0
  ��    ��   : DSP MSP
  ��������   : 2011��8��13��
  ��������   : ���ļ�������V7R1�ۺϲ��Ժ�DSP��ͷ�ļ���
  �޸���ʷ   :
  1.��    �� : 2011��8��13��
    ��    �� : MSP
    �޸����� : �����ļ���

*****************************************************************************/
#ifndef __DSP_SYN_H__
#define __DSP_SYN_H__

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

/* ^SSYNC �������۲����õ���ͬ��״̬*/
#define OM_PHY_SYN_SSYNC_SET_REQ             (0x10200)
#define PHY_OM_SYN_SSYNC_SET_CNF             (0x10200)

/* ^SSYNC �������۲��ѯ����ͬ��״̬*/
#define OM_PHY_SYN_SSYNC_RD_REQ              (0x10201)
#define PHY_OM_SYN_SSYNC_IND              (0x10201)

/* ^STXBW �������۲����÷������*/
#define OM_PHY_SYN_STXBW_SET_REQ             (0x10202)
#define PHY_OM_SYN_STXBW_SET_CNF             (0x10202)

/* ^SSUBFRAME �������۲�������֡���*/
#define OM_PHY_SYN_SSUBFRAME_SET_REQ           (0x10204)
#define PHY_OM_SYN_SSUBFRAME_SET_CNF           (0x10204)

/* ^STXCHAN �������۲��������з����ŵ�*/
#define OM_PHY_SYN_STXCHAN_SET_REQ           (0x10205)
#define PHY_OM_SYN_STXCHAN_SET_CNF           (0x10205)

/*^SPARA*/
/*�������۲������ŵ�����*/
#define OM_PHY_SYN_SPARA_SET_REQ           (0x1020F)
#define PHY_OM_SYN_SPARA_SET_CNF           (0x1020F)

#define OM_PHY_SYN_SPARA_RD_REQ             (0x10210)
#define PHY_OM_SYN_SPARA_RD_CNF             (0x10210)

/*^SSEGNUM�������۲�����segment����*/
#define OM_PHY_SYN_SSEGNUM_SET_REQ           (0x10211)
#define PHY_OM_SYN_SSEGNUM_SET_CNF           (0x10211)

#define OM_PHY_SYN_SSEGNUM_RD_REQ            (0x10212)
#define PHY_OM_SYN_SSEGNUM_RD_CNF            (0x10212)

/* ^STXMODUS �������۲����õ��Ʒ�ʽ*/
#define OM_PHY_SYN_STXMODUS_SET_REQ          (0x10206)
#define PHY_OM_SYN_STXMODUS_SET_CNF          (0x10206)

/* ^STXRBNUMS �������۲�����RB����*/
#define OM_PHY_SYN_STXRBNUMS_SET_REQ         (0x10207)
#define PHY_OM_SYN_STXRBNUMS_SET_CNF         (0x10207)

/* ^STXRBPOSS �������۲�����RBλ��*/
#define OM_PHY_SYN_STXRBPOSS_SET_REQ         (0x10208)
#define PHY_OM_SYN_STXRBPOSS_SET_CNF         (0x10208)

/* ^STXPOWS �������۲����÷��书��*/
#define OM_PHY_SYN_STXPOWS_SET_REQ           (0x10209)
#define PHY_OM_SYN_STXPOWS_SET_CNF           (0x10209)

/* ^STXCHANTYPES �������۲������ŵ�����*/
#define OM_PHY_SYN_STXCHANTYPES_SET_REQ      (0x1020A)
#define PHY_OM_SYN_STXCHANTYPES_SET_CNF      (0x1020A)

/* ^SSEGLEN �������۲�����SEGMENT����*/
#define OM_PHY_SYN_SSEGLEN_SET_REQ          (0x1020B)
#define PHY_OM_SYN_SSEGLEN_SET_CNF          (0x1020B)

/*^SRXPOW�������۲���ղ��ԵĹ�������*/
/*#define OM_PHY_SYN_SRXPOW_SET_REQ          (0x10213)*/
/*#define PHY_OM_SYN_SRXPOW_SET_CNF          (0x10213)*/
/*#define OM_PHY_SYN_SRXPOW_RD_REQ           (0x10214)*/
/*#define PHY_OM_SYN_SRXPOW_RD_CNF           (0x10214)*/

/*^SRXSUBFRA�������۲���ղ��Ե���֡����*/
#define OM_PHY_SYN_SRXSUBFRA_SET_REQ          (0x10215)
#define PHY_OM_SYN_SRXSUBFRA_SET_CNF          (0x10215)
#define OM_PHY_SYN_SRXSUBFRA_RD_REQ           (0x10216)
#define PHY_OM_SYN_SRXSUBFRA_RD_CNF           (0x10216)

/*^SRXBLER�������۲���ղ��Բ�ѯ������*/
#define OM_PHY_SYN_SRXBLER_RD_REQ           (0x10217)
#define PHY_OM_SYN_SRXBLER_RD_CNF           (0x10217)

/*��ѯ TDS ���� BER ���*/
#define OM_PHY_SYN_SRXBER_RD_REQ           (0x10218)
#define PHY_OM_SYN_SRXBER_RD_CNF           (0x10218)
/* ^SSTART �������۲��������䲢״̬��ѯ*/
#define OM_PHY_SYN_SSTART_SET_REQ            (0x1020C)
#define PHY_OM_SYN_SSTART_SET_CNF            (0x1020C)

/* ^SSTART �������۲�״̬��ѯ*/
#define OM_PHY_SYN_SSTART_RD_REQ             (0x1020D)
#define PHY_OM_SYN_SSTART_RD_CNF             (0x1020D)

/* ^SSTOP �������۲�ֹͣ���з���*/
#define OM_PHY_SYN_SSTOP_SET_REQ             (0x1020E)
#define PHY_OM_SYN_SSTOP_SET_CNF             (0x1020E)

#define OM_PHY_SYN_SRXSET_SET_REQ            (0x10213)
#define PHY_OM_SYN_SRXSET_SET_CNF            (0x10213)
#define OM_PHY_BT_FPOW_SET_REQ                (0x10214)
#define PHY_OM_BT_FPOW_SET_CNF                (0x10214)
/*****************************************************************************
 ���� : �������۲����õ���ͬ��״̬
 �ڵ���״̬�´���UE���۲��ǣ�MT8820C/CMW500����ͬ�����̣���ͬ���ɹ�ֱ�ӷ���OK�����򷵻�ERROR��
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ID   : OM_PHY_SYN_SSYNC_SET_REQ,PHY_OM_SYN_SSYNC_SET_CNF
 REQ  : OM_PHY_SYN_SSYNC_SET_REQ_STRU
 CNF  : PHY_OM_SYN_SSYNC_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;     /* ��Ϣ��ʶ */
    UINT16 usMode;      /* MODE TDD��1��FDD��0*/
    UINT16 usBand;      /* LTEƵ����Ϣ��ȡֵ��Χ1��50����7����Band7*/
    UINT16 usUlchannel; /* ����ͬ���ŵ���ȡֵ��Χ0��65535*/
    UINT16 usDlchannel; /* ����ͬ���ŵ���ȡֵ��Χ0��65535*/
} OM_PHY_SYN_SSYNC_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SSYNC_SET_CNF_STRU;

/*****************************************************************************
 ���� : �������۲��ѯ����ͬ��״̬
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ID   : OM_PHY_SYN_SSYNC_RD_REQ,PHY_OM_SYN_SSYNC_RD_CNF
 REQ  : OM_PHY_SYN_SSYNC_RD_REQ_STRU
 CNF  : PHY_OM_SYN_SSYNC_RD_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
/*DSP�����ϱ�*/
typedef struct
{
    UINT32 ulMsgId;
    
    /* UE���۲���ͬ��״̬ */
		/* 0	�Ѿ�ͬ���ɹ�*/
		/* 1	δͬ����ͬ��ʧ��*/
    UINT32 ulStatus;
} PHY_OM_SYN_SSYNC_RPT_IND_STRU;


/*****************************************************************************
 ���� : ^STXBW �������۲����÷������
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ��(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXBW_SET_REQ,PHY_OM_SYN_STXBW_SET_CNF
 REQ  : OM_PHY_SYN_STXBW_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXBW_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    /* ���۲�������з��������б�ȡֵ��Χ0~5��Ĭ��ֵΪ0*/
    /* 0	1.4M*/
    /* 1	3M*/
    /* 2	5M*/
    /* 3	10M*/
    /* 4	15M*/
    /* 5	20M*/
    UINT16 usBandwide; 
    UINT16 usRsv;
} OM_PHY_SYN_STXBW_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXBW_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SCONFIG �������۲�������֡���
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ID   : OM_PHY_SYN_SCONFIG_SET_REQ,PHY_OM_SYN_SCONFIG_SET_CNF
 REQ  : OM_PHY_SYN_SCONFIG_SET_REQ_STRU
 CNF  : PHY_OM_SYN_SCONFIG_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    /* RTTͨ��usFrameNumer/usFrameDeno,������֡���usFrameNumer=7��usFrameDeno=6��*/
    /* usFrameNumer/usFrameDeno=7/6*/
    UINT16 usSubFrameAssign; 
    UINT16 usSubFramePattern;
} OM_PHY_SYN_SSUBFRAME_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SSUBFRAME_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^STXCHAN �������۲�
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ��(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXCHAN_SET_REQ,PHY_OM_SYN_STXCHAN_SET_CNF
 REQ  : OM_PHY_SYN_STXCHAN_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXCHAN_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    /* ����ͬ���ŵ���ȡֵ��Χ0��65535*/
    UINT16 usUlchannel ; 
    UINT16 usRsv;
} OM_PHY_SYN_STXCHAN_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXCHAN_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SPARA �������۲�������֡���
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 ID   : 
 REQ  : 
 CNF  : 
 IND  : NA
 ˵�� :
*****************************************************************************/

typedef struct
{
	UINT32 ulMsgId;
	/*���ò������ͣ�ȡֵ��Χ0~32*/
	UINT16 usType;
	/*���ò���ֵ��ȡֵ��Χ0~65535*/
	UINT16 usValue;

} OM_PHY_SYN_SPARA_SET_REQ_STRU;
typedef struct
{
	UINT32 ulMsgId;

} PHY_OM_SYN_SPARA_SET_CNF_STRU;
/*****************************************************************************
 ���� : ^SSEGNUM �������۲�����SEGMENT������
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 ID   : 
 REQ  : 
 CNF  : 
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
	UINT32 ulMsgId;
	UINT16 usSegNum;
	UINT16 usReserved;

} OM_PHY_SYN_SSEGNUM_SET_REQ_STRU;
typedef struct
{
	UINT32 ulMsgId;

} PHY_OM_SYN_SSEGNUM_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^STXMODUS �������۲����õ��Ʒ�ʽ
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ��(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXMODUS_SET_REQ,PHY_OM_SYN_STXMODUS_SET_CNF
 REQ  : OM_PHY_SYN_STXMODUS_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXMODUS_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    /* �����۲���Ե��Ʒ�ʽ��������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
    UINT16 usModulution_num  ; 
	UINT16 usRsv;
    
    /* �����۲���Ե��Է�ʽ�б�ȡֵ��Χ0~2��Ĭ��ֵΪ0*/
    /* ������0����QPSK��1����16QAM��2����64QAM��*/
    UINT16  usModulation_list [256];
    
} OM_PHY_SYN_STXMODUS_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXMODUS_SET_CNF_STRU;
/*****************************************************************************
 ���� : ^STXRBNUMS �������۲�����RB����
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 ID   : 
 REQ  : 
 CNF  : 
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    /* �����۲�������з���RB��������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
    UINT16 usRb_num;
	UINT16 usRsv;
	/* �����۲�������з���RB���б�ȡֵ��Χ1~100��Ĭ��ֵ1*/
    UINT16 usrb_list[256];  
} OM_PHY_SYN_STXRBNUMS_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXRBNUMS_SET_CNF_STRU;


/*****************************************************************************
 ���� : ^STXRBPOSS �������۲�����RBλ��
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����RB������^SRBNUMS����ִ�С�(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXRBPOSS_SET_REQ,PHY_OM_SYN_STXRBPOSS_SET_CNF
 REQ  : OM_PHY_SYN_STXRBPOSS_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXRBPOSS_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    UINT16 usRbpos_num; /* �����۲�������з���RBλ�õ�������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
	UINT16 usRsv;

	UINT16 usrbpos_list[256];  /* �����۲�������з���RBλ�õ��б�ȡֵ��Χ0~99��Ĭ��ֵΪ0*/
} OM_PHY_SYN_STXRBPOSS_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXRBPOSS_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^STXPOWS �������۲����÷��书��
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ�С�(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXPOWS_SET_REQ,PHY_OM_SYN_STXPOWS_SET_CNF
 REQ  : OM_PHY_SYN_STXPOWS_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXPOWS_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    UINT16 ulPow_num;   /* �����۲�������з���RBλ�õ�������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
	UINT16 usRsv;

	INT16 slPow_list[256];  /* �����۲�������з���RBλ�õ��б�ȡֵ��Χ0~99��Ĭ��ֵΪ0*/
} OM_PHY_SYN_STXPOWS_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXPOWS_SET_CNF_STRU;


/*****************************************************************************
 ���� : ^STXCHANTYPES �������۲������ŵ�����
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ�С�(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_STXCHANTYPES_SET_REQ,PHY_OM_SYN_STXCHANTYPES_SET_CNF
 REQ  : OM_PHY_SYN_STXCHANTYPES_SET_REQ_STRU
 CNF  : PHY_OM_SYN_STXCHANTYPES_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    
    UINT16 usType_num; /* �����۲�������з����ŵ���������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
    UINT16 usRsv;   
		/* 0	PUSCH*/
		/* 1	PUCCH*/
		/* 2	PRACH*/
    UINT16 usType_list[256];/*	�����۲�������з����ŵ����б�ȡֵ��Χ0~2*/
} OM_PHY_SYN_STXCHANTYPES_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_STXCHANTYPES_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SSEGLEN �������۲�����SEGMENT����
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ��������Ҫ�����÷������۲�ͬ����^SSYNC����ִ�С�(MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_SSEGMENT_SET_REQ,PHY_OM_SYN_SSEGMENT_SET_CNF
 REQ  : OM_PHY_SYN_SSEGMENT_SET_REQ_STRU
 CNF  : PHY_OM_SYN_SSEGMENT_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    /* 10	10ms*/
    /* 20	20ms*/
	/* 50	50ms*/
	/* 100	100ms*/
    UINT16 usSegment_Len; /* �����۲�������з����ŵ���������ȡֵ��Χ0~50��Ĭ��ֵΪ0*/
    

    UINT16 usReserved;
} OM_PHY_SYN_SSEGLEN_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SSEGLEN_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SRXPOW �������۲���ղ��ԵĹ�������
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 
 
 ID  : 
 REQ : 
 CNF : 
 IND : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
	UINT32 ulMsgId;
	UINT16 usSwitch;
	INT16  sPower;
	UINT16 usMod;
	UINT16 usRBNum;
	UINT16 usRBPos;
	UINT16 usRsv;

}OM_PHY_SYN_SRXSET_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SRXSET_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SRXSUBFRA �������۲���ղ��Ե���֡����
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 
 
 ID  : 
 REQ : 
 CNF : 
 IND : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
	UINT32 ulMsgId;
	UINT16 usSubFrameNum;
	UINT16 usReserved;

}OM_PHY_SYN_SRXSUBFRA_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SRXSUBFRA_SET_CNF_STRU;
/*****************************************************************************
 ���� : ^SRXBLER �������۲���ղ��Բ�ѯ������
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 
 
 
 ID  : 
 REQ : 
 CNF : 
 IND : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
    UINT32 ulBler;
	INT16  sSnr;
	UINT16 usRsv;
} PHY_OM_SYN_SRXMEAS_RPT_IND_STRU;

typedef struct
{
    UINT32 ulMsgId;
    UINT32 ulBersum;
	UINT32 ulBerErr;
	UINT32 usRsv;
} PHY_OM_SYN_SRXMEAS_TDS_RPT_IND_STRU;
/*****************************************************************************
 ���� : ^SSTART �������۲��������䲢״̬��ѯ
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ����״̬������sequence��ʽ�ķ������۲⡣
 ��������Ҫ������AT^SSYNC��AT^SMODUS��AT^STXBWS��AT^SPOWS��AT^SCHANS��AT^SSEGMENT�����ִ�С�����ģʽ�·��ش����롣
 (MSP���ԼӼ��жϣ�����ʵ�ֿ����Ȳ�����)
 ID   : OM_PHY_SYN_SSTART_SET_REQ,PHY_OM_SYN_SSTART_SET_CNF
 REQ  : OM_PHY_SYN_SSTART_SET_REQ_STRU
 CNF  : PHY_OM_SYN_SSTART_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
	UINT16 usType;
	UINT16 usRsv;
} OM_PHY_SYN_SSTART_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SSTART_SET_CNF_STRU;

/*****************************************************************************
 ���� : ^SSTART? �������۲�״̬��ѯ,��״̬��������RTT����
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ID   : OM_PHY_SYN_SSTART_RD_REQ,PHY_OM_SYN_SSTART_RD_CNF
 REQ  : OM_PHY_SYN_SSTART_RD_REQ_STRU
 CNF  : PHY_OM_SYN_SSTART_RD_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
#if 0
typedef struct
{
    UINT32 ulMsgId;
	
	UINT16 usType;
	UINT16 usRsv;
} OM_PHY_SYN_SSTART_RD_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
    
    /* 0	ִ�����*/
    /* 1	����ִ��*/
    /* 2	δִ��*/
    UINT16 usStatus; /* �������۲�״̬*/
    
    UINT16 usRsv;
} PHY_OM_SYN_SSTART_RD_CNF_STRU;
#endif
/*****************************************************************************
 ���� : ^SSTOP �������۲�ֹͣ���з���
 ^TMODE=7��ʹ�ã����ڷ�TMODE=7�������MSPֱ�ӷ��ش����롣
 ID   : OM_PHY_SYN_SSTOP_SET_REQ,PHY_OM_SYN_SSTOP_SET_CNF
 REQ  : OM_PHY_SYN_SSTOP_SET_REQ_STRU
 CNF  : PHY_OM_SYN_SSTOP_SET_CNF_STRU
 IND  : NA
 ˵�� :
*****************************************************************************/
typedef struct
{
    UINT32 ulMsgId;
	
	UINT16 usType;
	UINT16 usRsv;
} OM_PHY_SYN_SSTOP_SET_REQ_STRU;

/* ��������Ϊ�ɹ�*/
typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_SSTOP_SET_CNF_STRU;

typedef struct
{
    UINT32 ulMsgId;	
	INT16  usPower;
	UINT16 usRsv;
} OM_PHY_SYN_FPOW_SET_REQ_STRU;

typedef struct
{
    UINT32 ulMsgId;
} PHY_OM_SYN_FPOW_SET_CNF_STRU;
#endif
