/******************************************************************************

    Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : LNvCommon.h
  Description     : LNvCommon.h header file
  History           :
     1.Name+ID      yyyy-mm-dd  Draft Enact

******************************************************************************/

#ifndef __LNVCOMMON_H__
#define __LNVCOMMON_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
#include  "LNvId.h"

#pragma pack(4)

/*****************************************************************************
  2 macro
*****************************************************************************/


/* �Ǳ�׼Ƶ�� begin */
#define LTE_COMM_NONSTANDARD_BAND_BEGIN                     101
#define LTE_COMM_NONSTANDARD_BAND_END                       116

#define LTE_COMM_NONSTANDARD_BAND_MAX_COUNT                 16
/* �Ǳ�׼Ƶ�� end */

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/


/*****************************************************************************
  4 Enum
*****************************************************************************/



/*****************************************************************************
 ö����    : LRRC_LTE_MODE_TYPE_ENUM_UINT16
 Э����  :
 ASN.1���� :
 ö��˵��  : LTEģʽ����
*****************************************************************************/
enum LTE_COMM_BAND_MODE_TYPE_ENUM
{
    LTE_COMM_BAND_MODE_TYPE_FDD                = 0,                             /* LTE FDD ��ʽ */
    LTE_COMM_BAND_MODE_TYPE_TDD                = 1,                             /* LTE TDD ��ʽ */
    LTE_COMM_BAND_MODE_TYPE_BUTT
};
typedef VOS_UINT8 LTE_COMM_BAND_MODE_TYPE_ENUM_UINT8;

/*****************************************************************************
   5 STRUCT
*****************************************************************************/
/* add by lishangfeng for NorStandard begin */
/*****************************************************************************
 �ṹ��    : LTE_NON_STANDARD_BAND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �Ǳ�Ƶ�ε�Ƶ����Ϣ��ص����ݽṹ,��36101 talbe 5.7.3-1��Ӧ

            RealFreq = usFLow + (RealArfcn - usRangOfNLow),
            ���� RealFreq ��ʵ��Ƶ����Ϣ��usRangOfNLow��usFLow��NV���л�ȡ�����庬��
            �����ݽṹ�е�˵��; RealArfcnʵ�ʵ�arfcn��
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                usFLow;                           /* ��ʼƵ�㣬��λ��100khz */
    VOS_UINT16                                usFHigh;                          /* ����Ƶ�㣬��λ��100khz */
    VOS_UINT16                                usRangOfNLow;                     /* Nlow,��Noffsetһ�� */
    VOS_UINT16                                usRangOfNHigh;                    /* Nhigh*/
}LTE_COMM_NON_STANDARD_FREQ_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LTE_NON_STANDARD_POWER_CLASS_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �Ǳ�Ƶ��power class��ص����ݽṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                 usUEPowerClass;
    VOS_INT16                                  sPowerToleranceHigh;    /* ��λ 0.1dBm */
    VOS_INT16                                  sPowerToleranceLow;     /* ��λ 0.1dBm */
    VOS_UINT8                                  aucReservered[2];
}LTE_COMM_NON_STANDARD_POWER_CLASS_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LTE_NON_STANDARD_BAND_TEST_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �Ǳ�Ƶ�εĲ�����Ϣ��ص����ݽṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT8                                   ucBandWidth;                       /* ������λ0.1MHz����ѡֵ14;30;50;150;200 */
    VOS_UINT8                                   aucReservered[3];
    VOS_UINT16                                  usTestFreqs[3];                    /* ���Ե͡��С���Ƶ�㣬��λ0.1MHz */
    VOS_INT16					                sRxRefSensPower;                   /* ���в��������ȹ��ʣ���λ0.1dBm */
    VOS_UINT32                                  aulReservered[16];
}LTE_NON_STANDARD_BAND_TEST_INFO_STRU;

/*****************************************************************************
 �ṹ��    : LTE_NON_STANDARD_BAND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �Ǳ�Ƶ�ε�Ƶ����Ϣ��ص����ݽṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT8                                   ucBandID;                       /* band iD */
    LTE_COMM_BAND_MODE_TYPE_ENUM_UINT8          enBandMode;
    VOS_INT16                                   sTxRxFreqSepara;
    LTE_COMM_NON_STANDARD_FREQ_INFO_STRU        stUlFreqInfo;
    LTE_COMM_NON_STANDARD_FREQ_INFO_STRU        stDlFreqInfo;
    LTE_COMM_NON_STANDARD_POWER_CLASS_INFO_STRU stUePowerClassInfo;
    LTE_NON_STANDARD_BAND_TEST_INFO_STRU		stTestInfo[6];                 /*1.4MHz��3MHz��5MHz,10MHz,15MHz��20MHz����Ĳ��Բ��� */
    VOS_UINT32                                  aulReservered[16];
}LTE_COMM_NON_STANDARD_BAND_COMM_STRU;

/* add by lishangfeng for NorStandard end */



/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/


/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/










#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of PsNvInterface.h */

