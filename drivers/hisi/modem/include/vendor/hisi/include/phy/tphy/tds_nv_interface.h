/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : tds_nv_interface.h
  �� �� ��   : ����
  ��    ��   : baizhongjin
  ��������   : 2013��3��25��
  ����޸�   :
  ��������   : TDS nv ͷ�ļ�
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2013��3��25��
    ��    ��   :
    �޸�����   : �����ļ�

*****************************************************************************

******************************************************************************/
#ifndef __TDS_NV_INTERFACE_H__
#define __TDS_NV_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif


#ifndef LPS_RTT
#include "vos.h"
#else
#include "type_define.h"
#endif

#include "tds_dsp_equip_interface.h"

#define RFIC_VERSION_V210               (210)

#define TDS_RF_SUPPORT_BAND_NUM  	  		 2
/* BEGIN: Added by w00180842, 2013/8/29   PN:tuner*/
#define TDS_TUNER_SUPPORT_SENSOR_NUM  	  	 8
#define TDS_TUNER_DAC_MAX                    6
#define TDS_TUNER_CONTROL_MAX                4
#define TDS_TUNER_CMD_MAX      (TDS_TUNER_DAC_MAX + TDS_TUNER_CONTROL_MAX)
/* END:   Added by w00180842, 2013/8/29 */
/*ǰ�����NV begin*/
/*���߿���bitmap*/
typedef enum
{
    TDS_ANTSEL_INDEX_RX0    = 0x0000000,
    TDS_ANTSEL_INDEX_RX1    = 0x1000000,
    TDS_ANTSEL_INDEX_TX     = 0x2000000,
    TDS_ANTSEL_INDEX_3      = 0x3000000
}TDS_ANTSEL_INDEX_ENUM;


typedef struct
{
    UINT16 usRxMainLnaSel; //��Ƶ��ͨ��LNAѡ��
    UINT16 usRxDvtyLnaSel;
    UINT16 usTxLnaSel;
    UINT16 usResv;
}TDS_RF_PRIVATE_CHANNEL_CFG_STRU;


typedef struct
{
    UINT32 ulAbb0RstBitMap;
    UINT32 ulAbb0RxAEnBitMap;
    UINT32 ulAbb0RxBEnBitMap;
    UINT32 ulAbb0TxEnBitMap;
    UINT32 ulRxABSwapBitMap;
    UINT32 ulRssiCalBitMap;
    UINT32 ulAdOn1BitMap;

}TDS_ABB_LINECTRL_BITMAP_STRU;


typedef struct
{
    UINT16 usCoreSel;    //����ͨ��Coreѡ��
    UINT16 usDivider;    //����ͨ����Ƶѡ��
    //UINT16 usTxCoreSel;
    //UINT16 usTxDivider;
}TDS_RF_PRIVATE_CORE_DIV_CFG_STRU;
typedef enum
{
    TDS_RF_CH0,
    TDS_RF_CH1,
}TDS_RF_CH_SEL_ENUM;
typedef UINT16 TDS_RF_CH_SEL_ENUM_UINT16;


typedef struct
{
    UINT16 usRfRxOnAdvanceTime;
    UINT16 usRfTxOnAdvanceTime;
    UINT16 usFreqSWAdvanceTime;
    UINT16 usFirRxEnAdvanceTime;
    UINT16 usFirTxEnAdvanceTime;
    UINT16 usMidAdvanceTime;
    UINT16 usABBAdOnAdvanceTime;
    UINT16 usABBDaOnAdvanceTime;
    UINT16 usPaEnAdvanceTime;
    UINT16 usTxEnAdvanceTime;
    UINT16 usAntSelAdvanceTime;
    TDS_RF_CH_SEL_ENUM_UINT16 enChannelSel;
}TDS_NV_RF_ADVANCE_TIME_STU;

typedef struct
{
    UINT32 ulAntSelCfg0;
    UINT32 ulAntSelCfg1;
    UINT32 ulAntSelCfg2;
    UINT32 ulAntSelCfg3;
}TDS_ANTSEL_ALLOT_STRU;

typedef struct
{

    UINT8  aucPaVal[4];/*֧�ֵ�pa��λ������ֵ Ĭ������0 1 2������е�*/
}TDS_PA_LEVEL_STRU;


/*�߿���Ч����bitmap����map����Ӳ�����Ӷ�ȷ��*/
typedef struct
{
    UINT32 aulCtuPaMaxConfig[8];
    UINT32 ulBandNumSupt;  //֧��band��Ŀ
    UINT32 aulBandList[TDS_RF_SUPPORT_BAND_NUM];     //֧��band�б�
    UINT32 aulPaEnAllot[TDS_RF_SUPPORT_BAND_NUM]; /*pa en ӳ���bitλ��*/
    UINT32 aulPaAtt0Allot[TDS_RF_SUPPORT_BAND_NUM];/*pa att0 ӳ���bitλ��*/
    UINT32 aulPaAtt1Allot[TDS_RF_SUPPORT_BAND_NUM];/*pa att1 ӳ���bitλ��*/
    TDS_PA_LEVEL_STRU astPaVal[TDS_RF_SUPPORT_BAND_NUM];/*��ͬ����䲻ͬ��λ����ֵ*/
    TDS_ANTSEL_ALLOT_STRU astAntSelAllot[TDS_RF_SUPPORT_BAND_NUM];
}TDS_NV_LINECTRL_ALLOT_BY_HARDWARE_STRU;

typedef struct
{
    UINT16      ulRficVersion;    //������̬ ����
    UINT16      ulBandNumSupt;  //֧��band��Ŀ
    UINT16      aulBandList[TDS_RF_SUPPORT_BAND_NUM];     //֧��band�б�
    TDS_RF_PRIVATE_CHANNEL_CFG_STRU  astRfChanCfg[TDS_RF_SUPPORT_BAND_NUM];//��Ƶͨ������
    TDS_RF_PRIVATE_CORE_DIV_CFG_STRU astRfCoreAndDivSel[TDS_RF_SUPPORT_BAND_NUM];
}TDS_NV_RFIC_CFG_STRU;
/*ǰ�����NV end*/


    /* rx tx  nv  start   */
typedef struct
{
    TDS_BAND_COMMON_PARA_STRU astCommon[TDS_RF_SUPPORT_BAND_NUM];
    TDS_BAND_RXPATH_PARA_STRU astRxPara[TDS_RF_SUPPORT_BAND_NUM];
    TDS_BAND_TXPATH_PARA_STRU astTxPara[TDS_RF_SUPPORT_BAND_NUM];
}TDS_NV_RXTX_PARA_STRU;

    /* rx tx nv end   */
     
/* BEGIN: Added by w00180842, 2013/8/16   PN:tuner*/
    
 /*MIPIд�����ʽ����
31--29  001  ---->ָ������
28--25  0111 ---->SLAVE ADDRESS
24--22  010  ---->cmd
21--17         ---->addr ����0x1c,0x00��
16                ---->17--24��У��λ
15--8           ---->���õ�����
7                  ---->15---8��У��λ
6--0             ---->����λ
 aulControlCfgPara[i] �谴������BITλȫ��д��
 usDacCmd    15--0���ζ�Ӧ���� 31--16BITλд��
 ausDacData[j][k]15---0���ζ�Ӧ����15--0BITλд��
 */
typedef struct 
{
    UINT16 usDacCmd;
    UINT16 usRev;
    UINT16 ausDacData[TDS_RF_SUPPORT_BAND_NUM][TDS_TUNER_SUPPORT_SENSOR_NUM];//BAND˳����BAND�б���ͬ
}TDS_TUNER_DAC_CFG_PARA_STRU;
typedef struct 
{
    UINT16 usDacCfgNum;//������ox00�Ĵ�����DACʹ�ܵĸ���һ��
    UINT16 usRev;
    TDS_TUNER_DAC_CFG_PARA_STRU stDacCfgPara[TDS_TUNER_DAC_MAX];
}TDS_TUNER_DAC_CFG_STRU;
 typedef struct 
 {
     UINT16 usControlCfgNum;//���ֵΪ4��SLEEP MODE ĿǰĬ������һ��(0x1c)��activeMODE Ĭ������3��
     UINT16 usRev;
     UINT32 aulControlCfgPara[TDS_TUNER_CONTROL_MAX];//����ʱ��0--5�������ã�0-->0x1c, 1-->0x10, 2-->0x0���������η���
 }TDS_TUNER_CONTROL_CFG_STRU;
typedef struct 
{
    TDS_TUNER_CONTROL_CFG_STRU stControlCfg;
    TDS_TUNER_DAC_CFG_STRU stDacCfg;
}TDS_TUNER_MIPI_ACTIVE_MODE_CMD_STRU;
typedef struct 
{
    TDS_TUNER_CONTROL_CFG_STRU stControlCfg;
}TDS_TUNER_MIPI_SLEEP_MODE_CMD_STRU;
typedef struct 
{
    UINT16 usTunerValidFlag;//1--֧��   0--��֧��
    UINT16 usCfgTimeAdv;//ʱ����ǰ��
    UINT16 usRev;
    UINT16 usBandNum;//֧�ֵ�BAND��
    UINT16 ausBandList[TDS_RF_SUPPORT_BAND_NUM];//BAND�б�
    TDS_TUNER_MIPI_SLEEP_MODE_CMD_STRU stSleepModeCfg;
    TDS_TUNER_MIPI_ACTIVE_MODE_CMD_STRU stActiveModeCfg;
}TDS_NV_TUNER_PARA_STRU;

typedef struct
{
    UINT16 usTdsDrxIdleEnableFlag;
    UINT16 usTdsDrxSwitch32kTime;
    UINT16 usTdsClkMeasParaIndex;
    UINT16 usTdsPresyncStartGate;
    UINT16 usTdsDrxIdleWaitTaskTime;
    UINT16 usTdsDrxIdleClkMeasStubFlag;
    UINT16 usTdsDrxIdleSleepStubFlag;
    UINT16 usTdsDrxSlaveEnableFlag;
    UINT16 usRsvd[4];
}TDS_NV_DRX_PARA_STRU;

typedef struct
{
    TDS_NV_RFIC_CFG_STRU stRficCfg;
    TDS_NV_LINECTRL_ALLOT_BY_HARDWARE_STRU stLineCtrlMapped;
    TDS_NV_RF_ADVANCE_TIME_STU stTdsTimer;
    TDS_NV_RXTX_PARA_STRU stRxTxPara;
    /* BEGIN: Added by w00180842, 2013/8/16   PN:tuner*/
    TDS_NV_TUNER_PARA_STRU stTunerPara;
    /* END:   Added by w00180842, 2013/8/16 */
    /* BEGIN: Added by x00228734, 2013/9/30*/
    TDS_NV_DRX_PARA_STRU stTdsDrxPara;
    /* END: Added by x00228734, 2013/9/30*/
    UINT16 usVctcxoInit;
    UINT16 usResv;
}TDS_NV_STRU;

/*˽��nv*/
typedef struct
{
    UINT32 ulRes;
}TDS_NV_PRIVATE_STRU;

typedef struct
{
    UINT16      usCurrBand;
    UINT16      usLineCfgIndex;    //������̬ ����
    UINT16      usRfCfgIndex;  //֧��band��Ŀ
    UINT16      usResv;
}TDS_NV_INDEX_STRU;


extern TDS_NV_STRU * gpstTdsV9NvPara;
extern TDS_NV_INDEX_STRU            gstNvCfgInd;

TDS_BAND_TXPATH_PARA_STRU * TDS_GetTxNv(void);
TDS_PA_LEVEL_THRE_STRU * TDS_GetPaTreNv(void);
TDS_BAND_RXPATH_PARA_STRU * TDS_GetRxNv(void);
TDS_TEMPERATURE_SENSOR_STRU * TDS_GetSensorNv(void);
TDS_AGC_BAND_PARA_STRU * TDS_GetAgcBandNv(void);
UINT16  TDS_GetRxMainLnaSel(UINT32 ulBandIdx);
UINT16  TDS_GetRxDvtyLnaSel(UINT32 ulBandIdx);
UINT16  TDS_GetTxLnaSel(UINT32 ulBandIdx);
TDS_RF_PRIVATE_CORE_DIV_CFG_STRU*  TDS_GetCoreDivCfg(UINT32 ulBandIdx);
TDS_RF_TXIQ_CAL_STRU* TDS_GetTxiqFromNv(void);
UINT16 TDS_GetDcocComFromNv(UINT16 usGainCode);
TDS_NV_TUNER_PARA_STRU * TDS_GetTunerNv(void);
TDS_NV_DRX_PARA_STRU * TDS_GetTdsDrxNv(void);
static inline TDS_RF_CH_SEL_ENUM_UINT16 TDS_MEAS_GetTdsCtuChNum(void)
{
    return gpstTdsV9NvPara->stTdsTimer.enChannelSel;
}
#ifdef __cplusplus
}/*end of extern C*/
#endif
#endif

