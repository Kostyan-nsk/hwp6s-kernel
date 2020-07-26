/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ImsCodecInterface.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2013��7��15��
  ����޸�   :
  ��������   : IMSA��CODECģ���Ľӿ��ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��7��15��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __IMSACODECINTERFACE_H__
#define __IMSACODECINTERFACE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
/* _H2ASN_Include v_typedef.h */
/* _H2ASN_Include ImsaCodecInterface.h */
#include "vos.h"
#include "CodecInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define IMSA_CODEC_MAX_DATA_LEN                     (32)           /* ��λ��16 bit����ǰ��֧��G711������֧�֣�Ҫ�ĳ�160 */

/*
IMSA_TX_DATA_STRU�ṹ��������
usFrameType     --���б�֡��FrameType
ausData         --IF1Э���е�A��B��C������ABC�������У�Byte����
*/
#define IMSA_TX_DATA_STRU                                                       \
    unsigned short                      usMsgId;                                \
    unsigned short                      usReserve;                              \
    CODEC_AMR_TYPE_TX_ENUM_UINT16       usStatus;                               \
    unsigned short                      usFrameType;                            \
    unsigned short                      ausData[IMSA_CODEC_MAX_DATA_LEN];

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum IMSA_VOICE_MSG_ID_ENUM
{
    /*====== VOICE->IMSA ======*/
    ID_VOICE_IMSA_TX_DATA_IND                       = 0x49A8,                   /* IMS�������������� */           /* _H2ASN_MsgChoice VOICE_IMSA_TX_DATA_IND_STRU */
    ID_VOICE_IMSA_CFG_CNF                           = 0x49A9,                   /* ����HIFI���еĽ������ڽ�� */    /* _H2ASN_MsgChoice VOICE_IMSA_CFG_CNF_STRU */

    /*====== IMSA->VOICE ======*/
    ID_IMSA_VOICE_RX_DATA_IND                       = 0x94A8,                   /* IMS������������ */               /* _H2ASN_MsgChoice IMSA_VOICE_RX_DATA_IND_STRU */
    ID_IMSA_VOICE_CFG_REQ                           = 0x94A9,                   /* ����HIFI���еĽ������� */        /* _H2ASN_MsgChoice IMSA_VOICE_CFG_REQ_STRU */

    IMSA_VOICE_MSG_ID_BUTT
};
typedef unsigned short IMSA_VOICE_MSG_ID_ENUM_UINT16;

enum IMSA_VOICE_ERRORFLAG_ENUM
{
    IMSA_VOICE_NO_ERROR = 0,                                                    /* Jitter Buffer������ */
    IMSA_VOICE_ERROR,                                                           /* Jitter Bufferû������ */
    IMSA_VOICE_ERRORFLAG_BUTT
};
typedef unsigned short IMSA_VOICE_ERRORFLAG_ENUM_UINT16;

enum IMSA_VOICE_CFG_ENUM
{
    IMSA_VOICE_CFG_SUCC         = 0,                /* ���óɹ� */
    IMSA_VOICE_CFG_FAIL,                            /* ����ʧ�� */
    IMSA_VOICE_CFG_BUTT
};
typedef unsigned long IMSA_VOICE_CFG_ENUM_UINT32;



/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
 ʵ������  : IMSA_VOICE_RX_DATA_IND_STRU
 ��������  : IMS��ȡIP�������RTP�����ͨ��IMSA���͸���Ϣ��VOICE�����ݷ��͹�ȥ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    unsigned short                      usMsgId;                                /* 0x94A8*/ /*_H2ASN_Skip*/
    unsigned short                      usSN;                                   /* �����к� */
    unsigned long                       ulTS;                                   /* ��ʱ��� */
    CODEC_ENUM_UINT16                   usCodecType;                            /* ���������� */
    CODEC_DTX_ENUM_UINT16               usDtxEnable;                            /* ���б����DTX */
    unsigned short                      usRateMode;                             /* ���б����RateMode */
    IMSA_VOICE_ERRORFLAG_ENUM_UINT16    usErrorFlag;                            /* Bufferû����ʱ��ErrorFlag */
    unsigned short                      usFrameType;                            /* ���б�֡��FrameType */
    unsigned short                      usQualityIdx;                           /* Quality indification */
    unsigned short                      ausData[IMSA_CODEC_MAX_DATA_LEN];       /* IF1Э���е�A��B��C������ABC�������У�Byte���� */
    unsigned long                       ulSSRC;                                 /* SSRC��Ϣ */
    unsigned long                       ulReserve;                              /* ������   */
}IMSA_VOICE_RX_DATA_IND_STRU;

/*****************************************************************************
 ʵ������  : VOICE_IMSA_TX_DATA_IND_STRU
 ��������  : VOICE���������ݴ���ͱ���OK��ͨ������Ϣ�����ݷ��͸�IMSA
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    IMSA_TX_DATA_STRU
}VOICE_IMSA_TX_DATA_IND_STRU;

/*****************************************************************************
 ʵ������  : IMSA_VOICE_CFG_REQ_STRU
 ��������  : ����HIFI���еĽ�������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    unsigned short                          usMsgId;                            /* 0x94a9 */ /*_H2ASN_Skip*/
    unsigned short                          usReserve;
    unsigned long                           ulTransTime;                        /* ���к�IMSA�Ľ������� */
}IMSA_VOICE_CFG_REQ_STRU;

/*****************************************************************************
 ʵ������  : IMSA_VOICE_CFG_CNF_STRU
 ��������  : IMSA_VOICE_CFG_REQ�Ļظ�
*****************************************************************************/

typedef struct
{
    VOS_MSG_HEADER
    unsigned short                          usMsgId;                            /* 0x49a9 */ /*_H2ASN_Skip*/
    unsigned short                          usReserve;
    IMSA_VOICE_CFG_ENUM_UINT32              ulResult;                           /* ִ�н�� */
}VOICE_IMSA_CFG_CNF_STRU;



/*****************************************************************************
  6 STRUCT����
*****************************************************************************/


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of imsacodecinterface.h */

