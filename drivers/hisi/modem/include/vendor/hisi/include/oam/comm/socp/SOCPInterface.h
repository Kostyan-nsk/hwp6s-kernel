/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��      : SOCPInterface.h
  �� �� ��      : ����
  ��    ��      : zhuli
  ��������      : 2012��8��3��
  ����޸�      :
  ��������      : ��ͷ�ļ�ΪSOCP API��ͷ�ļ�����Ҫ�ṩ���ⲿ����ʹ��
  �����б�      :
  �޸���ʷ      :
  1.��    ��    : 2012��8��3��
    ��    ��    : zhuli
    �޸�����    : �����ļ�

******************************************************************************/
#ifndef _SOCP_INTERFACE_H_
#define _SOCP_INTERFACE_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "DrvInterface.h"
#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


/**************************************************************************
  2 �궨��
**************************************************************************/
#define SOCP_SDLOG_TIME_ID                  (1)


#define SOCP_CODER_SRC_CHAN                 0x00
#define SOCP_CODER_DEST_CHAN                0x01
#define SOCP_DECODER_SRC_CHAN               0x02
#define SOCP_DECODER_DEST_CHAN              0x03

#define SOCP_CHAN_DEF(chan_type, chan_id)   ((chan_type<<16)|chan_id)
#define SOCP_REAL_CHAN_ID(unique_chan_id)   (unique_chan_id & 0xFFFF)
#define SOCP_REAL_CHAN_TYPE(unique_chan_id) (unique_chan_id>>16)

/**************************************************************************
  3 ö�ٶ���
**************************************************************************/
#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
/*����Դͨ��IDö�ٶ���*/
enum SOCP_CODER_SRC_ENUM
{
    SOCP_CODER_SRC_GU_OM1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,2),  /* GU OM�����Ϣ */
    SOCP_CODER_SRC_GU_OM2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,3),  /* GU OM�����Ϣ */
    SOCP_CODER_SRC_GUBBP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,0), /* GUBBP�����Ϣ */
    SOCP_CODER_SRC_GUBBP2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,1), /* GUBBP�����Ϣ */
    SOCP_CODER_SRC_GUDSP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,4), /* GUDSP�����Ϣ */
    SOCP_CODER_SRC_GUDSP2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,5), /* GUDSP�����Ϣ */
    SOCP_CODER_SRC_BUTT
};
#else
/*����Դͨ��IDö�ٶ���*/
enum SOCP_CODER_SRC_ENUM
{
    SOCP_CODER_SRC_LOM_CNF1     = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,0),  /* LTE OM������Ӧ */
    SOCP_CODER_SRC_HDLCAT_CNF   = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,1),  /* HDLC��ʽAT������Ӧ */
    SOCP_CODER_SRC_LOM_IND1     = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,2),  /* LTE OM�����Ϣ */
    SOCP_CODER_SRC_GU_OM1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,3),  /* GU OM�����Ϣ */
    SOCP_CODER_SRC_LOM_IND2     = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,4),  /* LTE OM�����Ϣ���� */
    SOCP_CODER_SRC_LOM_IND3     = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,5),  /* LTE OM�����Ϣ���� */
    SOCP_CODER_SRC_LOM_CNF2     = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,6),  /* LTE OM������Ӧ */
    SOCP_CODER_SRC_GU_OM2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,7),  /* GU OM�����Ϣ */
    SOCP_CODER_SRC_RFU          = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,8),  /* ���� */
    SOCP_CODER_SRC_HIFI         = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,9),  /* GU HIFI�����Ϣ */
    SOCP_CODER_SRC_MUTIL_MEDIA1 = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,10), /* ý�������Ϣ */
    SOCP_CODER_SRC_MUTIL_MEDIA2 = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,11), /* ý�������Ϣ */
    SOCP_CODER_SRC_MCU1         = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,12), /* MCU�����Ϣ */
    SOCP_CODER_SRC_MCU2         = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,13), /* MCU�����Ϣ */
    SOCP_CODER_SRC_LDSP1        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,14), /* LDSP�����Ϣ */
    SOCP_CODER_SRC_LDSP2        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,15), /* LDSP�����Ϣ */
    SOCP_CODER_SRC_LBBP1        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,16), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP2        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,17), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP3        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,18), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP4        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,19), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP5        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,20), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP6        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,21), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP7        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,22), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP8        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,23), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_LBBP9        = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,24), /* LBBP�����Ϣ */
    SOCP_CODER_SRC_GUBBP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,25), /* GUBBP�����Ϣ */
    SOCP_CODER_SRC_GUBBP2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,26), /* GUBBP�����Ϣ */
    SOCP_CODER_SRC_GUDSP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,27), /* GUDSP�����Ϣ */
    SOCP_CODER_SRC_GUDSP2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,28), /* GUDSP�����Ϣ */
    SOCP_CODER_SRC_TDDSP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,29), /* TDDSP�����Ϣ */
    SOCP_CODER_SRC_TDBBP1       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,30), /* TDBBP�����Ϣ */
    SOCP_CODER_SRC_TDBBP2       = SOCP_CHAN_DEF(SOCP_CODER_SRC_CHAN,31), /* TDBBP�����Ϣ */
    SOCP_CODER_SRC_BUTT
};
#endif
typedef VOS_UINT32 SOCP_CODER_SRC_ENUM_U32;

/*����Դͨ��IDö�ٶ���*/
enum SOCP_DECODER_SRC_ENUM
{
    SOCP_DECODER_SRC_LOM        = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN,0),   /* LTE OM���� */
    SOCP_DECODER_SRC_HDLC_AT    = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN,1),   /* HDLC�������ʽAT���� */
    SOCP_DECODER_SRC_GUOM       = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN,2),   /* GU OM���� */
    SOCP_DECODER_SRC_RFU        = SOCP_CHAN_DEF(SOCP_DECODER_SRC_CHAN,3),   /* ���� */
    SOCP_DECODER_SRC_BUTT
};
typedef VOS_UINT32 SOCP_DECODER_SRC_ENUM_U32;

#if ((FEATURE_ON == FEATURE_SOCP_CHANNEL_REDUCE)&&(VOS_OS_VER != VOS_WIN32))
/*����Ŀ��ͨ��IDö�ٶ���*/
enum SOCP_CODER_DST_ENUM
{
    SOCP_CODER_DST_GU_OM         = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,0), /* GU OMĿ��ͨ�� */
    SOCP_CODER_DST_BUTT
};
#else
/*����Ŀ��ͨ��IDö�ٶ���*/
enum SOCP_CODER_DST_ENUM
{
    SOCP_CODER_DST_LOM_CNF       = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,0), /* LTE OM������Ӧ��� */
    SOCP_CODER_DST_LOM_IND       = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,1), /* LTE OM�����ϱ���� */
    SOCP_CODER_DST_HDLC_AT       = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,2), /* ��ʽAT������Ӧ��� */
    SOCP_CODER_DST_GU_OM         = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,3), /* GU OMĿ��ͨ�� */
    SOCP_CODER_DST_RFU1          = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,4), /* ���� */
    SOCP_CODER_DST_RFU2          = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,5), /* ���� */
    SOCP_CODER_DST_RFU3          = SOCP_CHAN_DEF(SOCP_CODER_DEST_CHAN,6), /* ���� */
    SOCP_CODER_DST_BUTT
};
#endif
typedef VOS_UINT32 SOCP_CODER_DST_ENUM_U32;

/*����Ŀ��ͨ��IDö�ٶ���*/
enum SOCP_DECODER_DST_ENUM
{
    SOCP_DECODER_DST_LOM        = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN,0),  /* LTE OM���� */
    SOCP_DECODER_DST_HDLC_AT    = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN,1),  /* HDLC�������ʽAT���� */
    SOCP_DECODER_DST_GUOM       = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN,2),  /* GU OM���� */
    SOCP_DECODER_DST_RFU        = SOCP_CHAN_DEF(SOCP_DECODER_DEST_CHAN,3),  /* ���� */
    SOCP_DECODER_DST_BUTT
};
typedef VOS_UINT32 SOCP_DECODER_DST_ENUM_U32;

/*�����붨��*/
enum SOCP_ERR_ENUM
{
    BSP_ERR_SOCP_BASE           = VOS_OK,
    BSP_ERR_SOCP_NULL,
    BSP_ERR_SOCP_NOT_INIT,
    BSP_ERR_SOCP_MEM_ALLOC,
    BSP_ERR_SOCP_SEM_CREATE,
    BSP_ERR_SOCP_TSK_CREATE,
    BSP_ERR_SOCP_INVALID_CHAN,
    BSP_ERR_SOCP_INVALID_PARA,
    BSP_ERR_SOCP_NO_CHAN,
    BSP_ERR_SOCP_SET_FAIL,
    BSP_ERR_SOCP_TIMEOUT,
    BSP_ERR_SOCP_NOT_8BYTESALIGN,
    BSP_ERR_SOCP_CHAN_RUNNING,
    BSP_ERR_SOCP_CHAN_MODE,
    BSP_ERR_SOCP_DEST_CHAN,
    BSP_ERR_SOCP_DECSRC_SET,
    BSP_ERR_SOCP_BUTT
};

#define SOCP_MAX_ENCSRC_CHN         (SOCP_REAL_CHAN_ID(SOCP_CODER_SRC_BUTT))
#define SOCP_MAX_ENCDST_CHN         (SOCP_REAL_CHAN_ID(SOCP_CODER_DST_BUTT))
#define SOCP_MAX_DECSRC_CHN         (SOCP_REAL_CHAN_ID(SOCP_DECODER_SRC_BUTT))
#define SOCP_MAX_DECDST_CHN         (SOCP_REAL_CHAN_ID(SOCP_DECODER_DST_BUTT))


enum SOCP_BD_TYPE_ENUM
{
    SOCP_BD_DATA            = 0,    /* SOCP BD���ݿռ�ʹ��ʵ������ */
    SOCP_BD_LIST            = 1,    /* SOCP BD���ݿռ�ʹ��ʵ�����ݵ�ָ�� */
    SOCP_BD_BUTT
};
typedef VOS_UINT16 SOCP_BD_TYPE_ENUM_UINT16;

enum SOCP_HDLC_FLAG_ENUM
{
    SOCP_HDLC_ENABLE         = 0,    /* ��ǰHDLC���ܴ� */
    SOCP_HDLC_DISABLE        = 1,    /* ��ǰHDLC���ܹر� */
    SOCP_HDLC_FLAG_BUTT
};

enum SOCP_STATE_ENUM
{
    SOCP_IDLE               = 0,    /* SOCP���ڿ���̬ */
    SOCP_BUSY,                      /* SOCP��æ */
    SOCP_UNKNOWN_BUTT,              /*  δ֪״̬ */
};
typedef VOS_UINT32 SOCP_STATE_ENUM_UIN32;


/**************************************************************************
  3 �ṹ����
**************************************************************************/
/*����SOCPͨ����BD����Ƭ�ṹ*/
typedef struct
{
    VOS_UINT8                       *pucData;       /* ���ݵ�ָ�� */
    VOS_UINT16                      usMsgLen;       /* ���ݳ��� */
    SOCP_BD_TYPE_ENUM_UINT16        enDataType;     /* ʵ���������ͣ�����������ʵ���� */
}SOCP_BD_DATA_STRU;

/*����SOCPͨ����RD����Ƭ�ṹ*/
typedef struct
{
    VOS_UINT8                       *pucData;       /*����ָ��*/
    VOS_UINT16                      usMsgLen;       /*���ݳ���*/
    SOCP_BD_TYPE_ENUM_UINT16        enDataType;     /*ʵ���������ͣ�����������ʵ����*/
}SOCP_RD_DATA_STRU;


typedef enum tagSOCP_EVENT_E
{
    SOCP_EVENT_PKT_HEADER_ERROR         = 0x1,    /* ��ͷ������"HISI" */
    SOCP_EVENT_OUTBUFFER_OVERFLOW       = 0x2,    /* Ŀ��buffer���� */
    SOCP_EVENT_RDBUFFER_OVERFLOW        = 0x4,    /* RDbuffer���� */
    SOCP_EVENT_DECODER_UNDERFLOW        = 0x8,    /* ����Դbuffer���� */
    SOCP_EVENT_PKT_LENGTH_ERROR         = 0x10,   /* ��������ȼ����� */
    SOCP_EVENT_CRC_ERROR                = 0x20,   /* ����CRCУ����� */
    SOCP_EVENT_DATA_TYPE_ERROR          = 0x40,   /* �����������ʹ���*/
    SOCP_EVENT_HDLC_HEADER_ERROR        = 0x80,   /* ����HDLC������ */
    SOCP_EVENT_OUTBUFFER_THRESHOLD_OVERFLOW = 0x100,/* Ŀ��buffer��ֵ���� */
    SOCP_EVENT_BUTT
}SOCP_EVENT_E;

/* ����Դ�����ݽṹģʽ */
typedef enum tagSOCP_ENCSRC_CHNMODE_E
{
    SOCP_ENCSRC_CHNMODE_CTSPACKET       = 0,    /* �������ݰ����λ����� */
    SOCP_ENCSRC_CHNMODE_FIXPACKET,              /* �̶��������ݰ����λ����� */
    SOCP_ENCSRC_CHNMODE_LIST,                   /* ��ʽ���λ����� */
    SOCP_ENCSRC_CHNMODE_BUTT
}SOCP_ENCSRC_CHNMODE_E;

/* ����Դ�����ݽṹģʽ */
typedef enum tagSOCP_DECSRC_CHNMODE_E
{
    SOCP_DECSRC_CHNMODE_BYTES        = 0,       /* �����ֽڻ��λ����� */
    SOCP_DECSRC_CHNMODE_LIST,                   /* ��ʽ���λ����� */
    SOCP_DECSRC_CHNMODE_BUTT
}SOCP_DECSRC_CHNMODE_E;

/* ��ʱѡ��ʹ�����ݽṹ�� */
typedef enum tagSOCP_TIMEOUT_EN_E
{
    SOCP_TIMEOUT_BUFOVF_DISABLE        = 0,       /* buffer��������ϱ��ж� */
    SOCP_TIMEOUT_BUFOVF_ENABLE,                   /* buffer�������ʱ�����ϱ��ж� */
    SOCP_TIMEOUT_TRF,                             /* �����жϳ�ʱ���� */
    SOCP_TIMEOUT_BUTT
}SOCP_TIMEOUT_EN_E;

/* ͬһ���͵�ͨ����ͬ���ȼ�ö��ֵ*/
typedef enum tagSOCP_CHAN_PRIORITY_E
{
    SOCP_CHAN_PRIORITY_0     = 0,               /* ������ȼ�*/
    SOCP_CHAN_PRIORITY_1,                       /* �ε����ȼ�*/
    SOCP_CHAN_PRIORITY_2,                       /* �θ����ȼ�*/
    SOCP_CHAN_PRIORITY_3,                       /* ������ȼ�*/
    SOCP_CHAN_PRIORITY_BUTT
}SOCP_CHAN_PRIORITY_E;

/* ��������ö��ֵ*/
typedef enum tagSOCP_DATA_TYPE_E
{
    SOCP_DATA_TYPE_0            = 0,            /* LTE OAM���� */
    SOCP_DATA_TYPE_1,                           /* GU OAM���� */
    SOCP_DATA_TYPE_2,                           /* ���� */
    SOCP_DATA_TYPE_3,                           /* ���� */
    SOCP_DATA_TYPE_BUTT
}SOCP_DATA_TYPE_E;


/* BBP ����ģʽ�����ݶ����򸲸� */
typedef enum tagSOCP_BBP_DS_MODE_E
{
    SOCP_BBP_DS_MODE_DROP           = 0,        /* ���ݶ��� */
    SOCP_BBP_DS_MODE_OVERRIDE,                  /* ���ݸ��� */
    SOCP_BBP_DS_MODE_BUTT
}SOCP_BBP_DS_MODE_E;

/* ����Դͨ��data type ʹ��λ */
typedef enum tagSOCP_DATA_TYPE_EN_E
{
    SOCP_DATA_TYPE_EN           = 0,        /* data type ʹ�ܣ�Ĭ��ֵ */
    SOCP_DATA_TYPE_DIS,                     /* data type ��ʹ�� */
    SOCP_DATA_TYPE_EN_BUTT
}SOCP_DATA_TYPE_EN_E;

/* ����Դͨ��debug ʹ��λ */
typedef enum tagSOCP_ENC_DEBUG_EN_E
{
    SOCP_ENC_DEBUG_DIS          = 0,       /* debug ��ʹ�ܣ�Ĭ��ֵ */
    SOCP_ENC_DEBUG_EN,                     /* debug ʹ�� */
    SOCP_ENC_DEBUG_EN_BUTT
}SOCP_ENC_DEBUG_EN_E;

/* ����ͨ·�������ýṹ�� */
typedef struct tagSOCP_DEC_PKTLGTH_S
{
    BSP_U32                 u32PktMax;         /*���������ֵ*/
    BSP_U32                 u32PktMin;         /*��������Сֵ*/
}SOCP_DEC_PKTLGTH_S;


/* ͨ��Դͨ��buffer�ṹ�嶨��*/
typedef struct tagSOCP_SRC_SETBUF_S
{
    BSP_U32                 u32InputStart;      /* ����ͨ����ʼ��ַ*/
    BSP_U32                 u32InputEnd;        /* ����ͨ��������ַ*/
    BSP_U32                 u32RDStart;         /* RD buffer��ʼ��ַ*/
    BSP_U32                 u32RDEnd;           /* RD buffer������ַ*/
    BSP_U32                 u32RDThreshold;     /* RD buffer�����ϱ���ֵ*/
}SOCP_SRC_SETBUF_S;

/* ͨ��Ŀ��ͨ��buffer�ṹ�嶨��*/
typedef struct tagSOCP_DST_SETBUF_S
{
    BSP_U32                 u32OutputStart;     /* ���ͨ����ʼ��ַ*/
    BSP_U32                 u32OutputEnd;       /* ���ͨ��������ַ*/
    BSP_U32                 u32Threshold;       /* ���ͨ������ֵ */
}SOCP_DST_SETBUF_S;

/* ����Դͨ���ṹ�嶨��*/
typedef struct tagSOCP_CODER_SRC_CHAN_S
{
    BSP_U32                 u32DestChanID;      /* Ŀ��ͨ��ID*/
    BSP_U32                 u32BypassEn;        /* ͨ��bypassʹ��*/
    SOCP_DATA_TYPE_E        eDataType;          /* �������ͣ�ָ�����ݷ�װЭ�飬���ڸ��ö�ƽ̨*/
    SOCP_DATA_TYPE_EN_E     eDataTypeEn;        /* ��������ʹ��λ*/
    SOCP_ENC_DEBUG_EN_E     eDebugEn;           /* ����λʹ��*/
    SOCP_ENCSRC_CHNMODE_E   eMode;              /* ͨ������ģʽ*/
    SOCP_CHAN_PRIORITY_E    ePriority;          /* ͨ�����ȼ�*/
    SOCP_SRC_SETBUF_S       sCoderSetSrcBuf;
}SOCP_CODER_SRC_CHAN_S;

/* ����Ŀ�����ýṹ�嶨��*/
typedef struct tagSOCP_CODER_DEST_CHAN_S
{
    BSP_U32                 u32EncDstThrh;     /* ����Ŀ��ͨ����ֵ���ޣ��ٲ�ͨ��ʱʹ��*/
    SOCP_DST_SETBUF_S       sCoderSetDstBuf;
}SOCP_CODER_DEST_CHAN_S;

/* ����Դͨ���ṹ�嶨��*/
typedef struct tagSOCP_DECODER_SRC_CHAN_S
{
    SOCP_DATA_TYPE_EN_E     eDataTypeEn;        /* ��������ʹ��λ*/
    SOCP_DECSRC_CHNMODE_E   eMode;              /* ͨ��ģʽ*/
    SOCP_SRC_SETBUF_S       sDecoderSetSrcBuf;
}SOCP_DECODER_SRC_CHAN_S;

/* ����Ŀ��ͨ���ṹ�嶨��*/
typedef struct tagSOCP_DECODER_DEST_CHAN_S
{
    BSP_U32                 u32SrcChanID;       /* ������Դͨ��ID*/
    SOCP_DATA_TYPE_E        eDataType;          /* �������ͣ�ָ�����ݷ�װЭ�飬���ڸ��ö�ƽ̨*/
    SOCP_DST_SETBUF_S       sDecoderDstSetBuf;
}SOCP_DECODER_DEST_CHAN_S;

/* ͨ��buffer�����ṹ�嶨��*/
typedef struct tagSOCP_BUFFER_RW_S
{
    BSP_CHAR   *pBuffer;                        /* bufferָ��*/
    BSP_U32     u32Size;                        /* ����buffer��С*/
    BSP_CHAR   *pRbBuffer;                      /* �ؾ�bufferָ��*/
    BSP_U32     u32RbSize;                      /* �ؾ�buffer��С*/
}SOCP_BUFFER_RW_S;

/* ����Դͨ����������ṹ�嶨��*/
typedef struct tagSOCP_DECODER_ERROR_CNT_S
{
    BSP_U32     u32PktlengthCnt;                /* �������������*/
    BSP_U32     u32CrcCnt;                      /* CRCУ��������*/
    BSP_U32     u32DataTypeCnt;                 /* �����������ͼ���������*/
    BSP_U32     u32HdlcHeaderCnt;               /* 0x7EУ�����*/
}SOCP_DECODER_ERROR_CNT_S;

/* ����ͨ·�������ýṹ�� */
typedef struct tagSOCP_ENCSRC_RSVCHN_SCOPE_S
{
    BSP_U32                 u32RsvIDMin;        /*����Դ����ͨ��ID��Сֵ*/
    BSP_U32                 u32RsvIDMax;        /*����Դ����ͨ��ID���ֵ*/
}SOCP_ENCSRC_RSVCHN_SCOPE_S;

typedef BSP_S32 (*socp_event_cb)(BSP_U32 u32ChanID, SOCP_EVENT_E u32Event, BSP_U32 u32Param);
typedef BSP_S32 (*socp_read_cb)(BSP_U32 u32ChanID);
typedef BSP_S32 (*socp_rd_cb)(BSP_U32 u32ChanID);

#define INNER_LOG_DATA_MAX                   0x40

typedef struct
{
    VOS_UINT32                  ulSlice;
    VOS_UINT32                  ulFileNO;
    VOS_INT32                   lLineNO;
    VOS_UINT32                  ulP1;
    VOS_UINT32                  ulP2;
}INNER_LOG_RECORD_STRU;

typedef struct
{
    VOS_UINT32                  ulCnt;
    INNER_LOG_RECORD_STRU       astLogData[INNER_LOG_DATA_MAX];
}INNER_LOG_DATA_STRU;

/**************************************************************************
  ��������
**************************************************************************/
extern VOS_VOID OM_Inner_Log(VOS_CHAR *pucStr, VOS_UINT32 ulP1, VOS_UINT32 ulP2,
                          INNER_LOG_DATA_STRU *pstLogData ,VOS_UINT32 ulFileID, VOS_INT32 lLineNo);

#define OM_INNER_LOG( pucStr, ulP1, ulP2, pstLogData )\
    OM_Inner_Log( pucStr, ulP1, ulP2, pstLogData, VOS_FILE_ID, __LINE__ )

/*****************************************************************************
 �� �� ��  : BSP_SOCP_CoderSetSrcChan
 ��������  : �˽ӿ����ĳһ����Դͨ�������ã����غ���ִ�еĽ����
 �������  : enSrcChanID:SOCP��������Ŀ��ͨ��ID��
             pDestAttr:SOCP������Ŀ��ͨ�������ṹ��ָ�롣
 �������  : �ޡ�
 �� �� ֵ  : BSP_OK:����Ŀ��ͨ�����óɹ���
             other:����Ŀ��ͨ������ʧ�ܴ����롣
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_CoderSetSrcChan(SOCP_CODER_SRC_ENUM_U32 enSrcChanID,SOCP_CODER_SRC_CHAN_S *pSrcAttr);

/*****************************************************************************
 �� �� ��  : BSP_SOCP_CoderSetDestChanAttr
 ��������  : �˽ӿ����ĳһ����Ŀ��ͨ�������ã����غ���ִ�еĽ����
 �������  : u32DestChanID:SOCP��������Ŀ��ͨ��ID��
             pDestAttr:SOCP������Ŀ��ͨ�������ṹ��ָ�롣
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:����Ŀ��ͨ�����óɹ���
             SOCP_ERROR:����Ŀ��ͨ������ʧ�ܡ�
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_CoderSetDestChanAttr(VOS_UINT32 u32DestChanID, SOCP_CODER_DEST_CHAN_S *pDestAttr);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_DecoderAllocDestChan
 ��������  :�˽ӿ����SOCP������Ŀ��ͨ���ķ��䣬
                ���ݽ���Ŀ��ͨ����������ͨ�����ԣ�
                ������Դͨ�������غ���ִ�н����
 �������  : pAttr:������Ŀ��ͨ�������ṹ��ָ��
                         pDestChanID:���뵽��Ŀ��ͨ��ID
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:����Ŀ��ͨ������ɹ���
                             SOCP_ERROR:����Ŀ��ͨ������ʧ�ܡ�
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_DecoderSetDestChan(SOCP_DECODER_DST_ENUM_U32 enDestChanID,SOCP_DECODER_DEST_CHAN_S *pAttr);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_DecoderSetSrcChanAttr
 ��������  :�˽ӿ����ĳһ����Դͨ�������ã����غ���ִ�еĽ����
 �������  : u32SrcChanID:������Դͨ��ID
                         pInputAttr:������Դͨ�������ṹ��ָ��
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:����Դͨ�����óɹ�
                             SOCP_ERROR:����Դͨ������ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_DecoderSetSrcChanAttr(VOS_UINT32 u32SrcChanID,SOCP_DECODER_SRC_CHAN_S *pInputAttr);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_DecoderGetErrCnt
 ��������  :�˽ӿڸ�������ͨ���������쳣����ļ���ֵ��
 �������  : u32ChanID:������ͨ��ID
                         pErrCnt:�������쳣�����ṹ��ָ��
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:�����쳣�����ɹ�
                             SOCP_ERROR:�����쳣����ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_DecoderGetErrCnt (VOS_UINT32 u32ChanID, SOCP_DECODER_ERROR_CNT_S *pErrCnt);

/*****************************************************************************
 �� �� ��  : BSP_SOCP_RegisterEventCB
 ��������  : �˽ӿ�Ϊ����ͨ��ע���¼��ص�������
 �������  : u32ChanID:ͨ��ID��
             EventCB:�¼��ص��������ο�socp_event_cb��������
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ע���¼��ص������ɹ���
             SOCP_ERROR:ע���¼��ص�����ʧ�ܡ�
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_RegisterEventCB(VOS_UINT32 u32ChanID, socp_event_cb EventCB);

/*****************************************************************************
 �� �� ��  : BSP_SOCP_Start
 ��������  : �˽ӿ�����Դͨ��������������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:�������������ɹ���
             SOCP_ERROR:������������ʧ�ܡ�
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_Start(VOS_UINT32 u32SrcChanID);

/*****************************************************************************
 �� �� ��  : BSP_SOCP_Stop
 ��������  : �˽ӿ�����Դͨ����ֹͣ������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��������ֹͣ�ɹ���
             SOCP_ERROR:��������ֹͣʧ�ܡ�
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_Stop(VOS_UINT32 u32SrcChanID);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_SetTimeout
 ��������  :�˽ӿ����ó�ʱ��ֵ��
 �������  : u32Timeout:��ʱ��ֵ

 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:���ó�ʱʱ����ֵ�ɹ���
                             SOCP_ERROR:���ó�ʱʱ����ֵʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_SetTimeout (SOCP_TIMEOUT_EN_E eTmOutEn, VOS_UINT32 u32Timeout);

/*****************************************************************************
 �� �� ��   : BSP_SOCP_SetDecPktLgth
 ��������  :���ý�������ȼ���ֵ
 �������  : pPktlgth:��������ȼ�ֵ

 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:���óɹ���
                    ����ֵ:����ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_SetDecPktLgth(SOCP_DEC_PKTLGTH_S *pPktlgth);

/*****************************************************************************
 �� �� ��   : BSP_SOCP_SetDebug
 ��������  :���ý���Դͨ����debugģʽ
 �������  : u32ChanID:ͨ��ID
                  u32DebugEn: debug��ʶ
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:���óɹ���
                     ����ֵ:����ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_SetDebug(VOS_UINT32 u32DestChanID, VOS_UINT32 u32DebugEn);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_GetWriteBuff
 ��������  :�˽ӿ����ڻ�ȡд����buffer��
 �������  : u32SrcChanID:Դͨ��ID
                  pBuff:           :д����buffer

 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:��ȡд����buffer�ɹ���
                             SOCP_ERROR:��ȡд����bufferʧ��
*****************************************************************************/
extern  VOS_UINT32 BSP_SOCP_GetWriteBuff( VOS_UINT32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_WriteDone
 ��������  :�ýӿ��������ݵ�д�������ṩд�����ݵĳ��ȡ�
 �������  : u32SrcChanID:Դͨ��ID
                  u32WrtSize:   ��д�����ݵĳ���
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:д�����ݳɹ���
                             SOCP_ERROR:д������ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_WriteDone(VOS_UINT32 u32SrcChanID, VOS_UINT32 u32WrtSize);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_GetRDBuffer
 ��������  :�ô˽ӿ����ڻ�ȡRD buffer������ָ�롣
 �������  : u32SrcChanID:Դͨ��ID
                  pBuff:  RD buffer
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:��ȡRD���λ������ɹ�
                             SOCP_ERROR:��ȡRD���λ�����ʧ��
*****************************************************************************/
extern  VOS_UINT32 BSP_SOCP_GetRDBuffer( VOS_UINT32 u32SrcChanID,SOCP_BUFFER_RW_S *pBuff);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_ReadRDDone
 ��������  :�˽ӿ������ϲ�֪ͨSOCP��������RD buffer��ʵ�ʶ�ȡ�����ݡ�
 �������  : u32SrcChanID:Դͨ��ID
                  u32RDSize:  ��RD buffer��ʵ�ʶ�ȡ�����ݳ���
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:��ȡRDbuffer�е����ݳɹ�
                             SOCP_ERROR:��ȡRDbuffer�е�����ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_ReadRDDone(VOS_UINT32 u32SrcChanID, VOS_UINT32 u32RDSize);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_RegisterReadCB
 ��������  :�ýӿ�����ע������ݵĻص�������
 �������  : u32DestChanID:Ŀ��ͨ��ID
                  ReadCB: �¼��ص�����
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:ע������ݻص������ɹ�
                             SOCP_ERROR:ע������ݻص�����ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_RegisterReadCB( VOS_UINT32 u32DestChanID, socp_read_cb ReadCB);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_RegisterReadCB
 ��������  :�ô˽ӿ����ڻ�ȡ�����ݻ�����ָ�롣
 �������  : u32DestChanID:Ŀ��ͨ��ID
                  ReadCB: ������buffer
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:��ȡ�����ݻ������ɹ���
                             SOCP_ERROR:��ȡ�����ݻ������ɹ���
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_GetReadBuff( VOS_UINT32 u32DestChanID,SOCP_BUFFER_RW_S *pBuffer);

/*****************************************************************************
 �� �� ��      : BSP_SOCP_ReadDataDone
 ��������  :�ýӿ������ϲ����SOCP��������Ŀ��ͨ���ж��ߵ�ʵ�����ݡ�
 �������  : u32DestChanID:Ŀ��ͨ��ID
                  u32ReadSize: �Ѷ������ݵĳ���
 �������  : �ޡ�
 �� �� ֵ      : SOCP_OK:�����ݳɹ���
                             SOCP_ERROR:������ʧ��
*****************************************************************************/
extern VOS_UINT32 BSP_SOCP_ReadDataDone(VOS_UINT32 u32DestChanID,VOS_UINT32 u32ReadSize);

/*****************************************************************************
 �� �� ��  : SOCP_CleanEncSrcChan
 ��������  :�ýӿ������ϲ����SOCP���������SOCP�ı���Դͨ�������üĴ�����
 �������  : u32SrcChanID:ͨ��ID
 �������  : �ޡ�
 �� �� ֵ  : VOS_OK:�ɹ�
             VOS_ERR:ʧ��
*****************************************************************************/
extern VOS_UINT32 SOCP_CleanEncSrcChan(SOCP_CODER_SRC_ENUM_U32 enSrcChanID);

extern VOS_UINT32 BSP_SOCP_EncDstBufFlush(VOS_VOID);

extern VOS_VOID BSP_SOCP_EncDstTimerHandler(VOS_UINT32 ulParam, VOS_UINT32 ulTimerName);

extern VOS_VOID BSP_SOCP_StartSDLogTimerFail(VOS_VOID);

extern VOS_VOID BSP_SOCP_RefreshSDLogCfg(VOS_UINT32 ulTimerLen);

extern VOS_UINT32 BSP_SOCP_GetSDLogCfg(VOS_UINT32 *pulTimerLen);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



