

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "drv_global.h"


#ifndef __DRV_SCI_H__
#define __DRV_SCI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum
{
    SIM_CARD_OUT = 0,
    SIM_CARD_IN  = 1
} SCI_DETECT_STATE;

/*for dsda*/
typedef enum
{
    RF_ID0 = 0,
    RF_ID1 = 1,
    RF_ID_BUTTOM
}RF_ID;

/*for reset mode*/
typedef enum
{
    WARM_RESET = 0, /*�ȸ�λ*/
    COLD_RESET = 1, /*�临λ*/
    RESET_MODE_BUTTOM
}RESET_MODE;

/*for protocol mode*/
typedef enum
{
    T0_PROTOCOL_MODE = 0,          /*T=0*/
    T1_PROTOCOL_MODE = 1,          /*T=1*/
    PROTOCOL_MODE_BUTTOM
}PROTOCOL_MODE;

/*for EDC mode*/
typedef enum
{
    T1_ERR_DETECT_MODE_LRC = 0, /*LRC*/
    T1_ERR_DETECT_MODE_CRC = 1, /*CRC*/
    T1_ERR_DETECT_MODE_BUTTOM
}T1_ERR_DETECT_MODE;


/* �ϲ�ʹ�õ���SCI����ķ���ֵ */
#define SCI_CARD_BUSY       (1)
#define SCI_VLTG_HIGH       (1)

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*for ATR info*/
typedef struct {
    PROTOCOL_MODE      Tbyte;   
    BSP_U32            valIFSC;
    BSP_U32            valIFSD;
    T1_ERR_DETECT_MODE errDetectMode;
}SCI_ATRInfo;

/*for sci record info*/
typedef struct stSciRecordInfo{
    BSP_U16            cardClass;         /*����ѹ����:1800��3000*/
    BSP_U16            ATRlen;            /*ATR����ʵ�ʳ���*/
    BSP_U8             ATRData[40];       /*ATR���ݼ�¼��������40���ֽ�*/
    BSP_U32            SciRegData[36];    /*SCI���мĴ�����¼*/
    BSP_U8             SimRecordData[72]; /*�Ϳ����������8�����ݣ�ÿ��������9���ֽ�*/
    BSP_U32            SciEvent[32];      /*SCI �ײ��¼����8����¼*/
}SCI_RECORD_INFO;


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
/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_GET_ERRNO
*
* ��������  : ���ӿ����ڻ�ȡSCI�����Ĵ����
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : SCI�����Ĵ����
*
* �޸ļ�¼  : 2012��12��28�� c61362 create
*
*****************************************************************************/
BSP_S32 BSP_SCI_GetErrno(BSP_VOID);
BSP_S32 I1_BSP_SCI_GetErrno(BSP_VOID);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_GET_ERRNO()    I1_BSP_SCI_GetErrno()
#else
#define DRV_USIMMSCI_GET_ERRNO()    BSP_SCI_GetErrno()
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_RST
*
* ��������  : ���ӿ����ڸ�λSCI��Smart Card Interface��������USIM��Universal
*           Subscriber Identity Module����
*
* �������  : RESET_MODE enResetMode
              WARM_RESET:�ȸ�λ
              CODE_RESET:�临λ

* �������  : ��
*
* �� �� ֵ  : OK  ��λ�ɹ�
*
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_Reset(RESET_MODE enResetMode);
BSP_U32 I1_BSP_SCI_Reset(RESET_MODE enResetMode);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_RST(enResetMode)    I1_BSP_SCI_Reset(enResetMode)
#else
#define DRV_USIMMSCI_RST(enResetMode)    BSP_SCI_Reset(enResetMode)
#endif

/********************************************************************************************************************
 �� �� ��  : DRV_USIMMSCI_GET_CARD_STAU
 ��������  : ���ӿ����ڻ�ÿ���ǰ��״̬��ĿǰLTE�ṩ�ӿڲ������Ͳ�һ��
 �������  : �ޡ�
 �������  :
 �� �� ֵ  : OK:  ������Ready��
             ERROR����δ����Ready��
             SCI_CARD_BUSY:������busy״̬
 ע������  ����Ч����BSP_SCI_Reset()�������ٵ��ô˺�����
********************************************************************************************************************/
BSP_S32 BSP_GUSCI_GetCardStatus(BSP_VOID);
BSP_S32 I1_BSP_GUSCI_GetCardStatus(BSP_VOID);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_GET_CARD_STAU()    I1_BSP_GUSCI_GetCardStatus()
#else
#define DRV_USIMMSCI_GET_CARD_STAU()    BSP_GUSCI_GetCardStatus()
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_SND_DATA
*
* ��������  : ���ӿ����ڷ���һ�����ݵ�USIM��
*
* �������  : BSP_U32 u32DataLength �������ݵ���Ч���ȡ�ȡֵ��Χ1��256����λΪ�ֽ�
*             BSP_U8 *pu8DataBuffer �������������ڴ���׵�ַ�����Ƕ�̬���䣬���ýӿ�
*                                   ���������ͷţ�������Ӳ����������
* �������  : ��
*
* �� �� ֵ  : OK/ERROR
*
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_SendData(BSP_U32 u32DataLength, BSP_U8 *pu8DataBuffer);
BSP_S32 I1_BSP_SCI_SendData(BSP_U32 u32DataLength, BSP_U8 *pu8DataBuffer);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_SND_DATA(u32DataLength, pu8DataBuffer)    I1_BSP_SCI_SendData(u32DataLength, pu8DataBuffer)
#else
#define DRV_USIMMSCI_SND_DATA(u32DataLength, pu8DataBuffer)    BSP_SCI_SendData(u32DataLength, pu8DataBuffer)
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_RCV
*
* ��������  : ���ӿ�����USIM Manager��ȡ����SCI Driver�Ŀ���������
*             �ýӿ�Ϊ�����ӿڣ�ֻ��SCI���յ��㹻����������Ż᷵�أ�
*             �ýӿڵĳ�ʱ����Ϊ1s
*
* �������  : BSP_U32 u32DataLength USIM Manager����SCI Driver��ȡ�����ݳ��ȡ�
* �������  : BSP_U8 *pu8DataBuffer USIM Managerָ����Buffer��SCI Driver�����ݿ�������Buffer��
* �� �� ֵ  : OK/ERROR
*
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_ReceiveSync(BSP_U32 u32Length, BSP_U8 *pu8Data);
BSP_S32 I1_BSP_SCI_ReceiveSync(BSP_U32 u32Length, BSP_U8 *pu8Data);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_RCV(u32Length, pu8Data)    I1_BSP_SCI_ReceiveSync(u32Length, pu8Data)
#else
#define DRV_USIMMSCI_RCV(u32Length, pu8Data)    BSP_SCI_ReceiveSync(u32Length, pu8Data)
#endif

/*************************************************************************************
* �� �� ��  : DRV_USIMMSCI_GET_ATR
*
* ��������  : ���ӿ����ڽ�Driver�㻺���ATR���ݺ����ݸ������ظ�USIM Manager��
*
* �������  : BSP_VOID
* �������  : unsigned long *u8DataLength  Driver��ȡ��ATR���ݳ��ȣ����ظ�USIM Manager��
*                                   ȡֵ��Χ0��32����λ���ֽ�
*             BSP_U8 *pu8ATR          USIM Managerָ����Buffer��SCI Driver��ATR
*                                   ���ݿ�������Buffer��һ��Ϊ����ϵͳ������̬����
*                                   ���߾�̬����ĵ�ַ
*             SCI_ATRInfo *sciATRInfo ���ڴ洢�����������ATR��Ϣ
*
*
*
* �� �� ֵ  : OK/ERROR
*
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
************************************************************************************/
BSP_S32 BSP_SCI_GetATR(unsigned long *u32DataLength,BSP_U8 *pu8ATR,SCI_ATRInfo *sciATRInfo);
BSP_S32 I1_BSP_SCI_GetATR(unsigned long *u32DataLength,BSP_U8 *pu8ATR,SCI_ATRInfo *sciATRInfo);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_GET_ATR(u32DataLength, pu8ATR,sciATRInfo)    I1_BSP_SCI_GetATR(u32DataLength, pu8ATR,sciATRInfo)
#else
#define DRV_USIMMSCI_GET_ATR(u32DataLength, pu8ATR,sciATRInfo)    BSP_SCI_GetATR(u32DataLength, pu8ATR,sciATRInfo)
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_CLASS_SWITCH
*
* ��������  : ���ӿ�����֧��PS�Կ��ĵ�ѹ���ͽ����л�����1.8V�л���3V
*
* �������  : BSP_VOID
* �������  : ��
*
* �� �� ֵ  :  OK    ��ǰ��ѹ������ߵģ����е�ѹ�л�����
*              ERROR �л�ʧ��
*              SCI_VLTG_HIGH   ��ǰ��ѹ�Ѿ�����ߵ�ѹ��û�н��е�ѹ�л�
*
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_ClassSwitch(BSP_VOID);
BSP_S32 I1_BSP_SCI_ClassSwitch(BSP_VOID);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_CLASS_SWITCH()    I1_BSP_SCI_ClassSwitch()
#else
#define DRV_USIMMSCI_CLASS_SWITCH()    BSP_SCI_ClassSwitch()
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_TM_STOP
*
* ��������  : ���ӿ�����֧��PS�ر�SIM��ʱ��
*
* �������  :
*           SCI_CLK_STOP_TYPE_E enTimeStopCfg ʱ��ֹͣģʽ
*
* �������  : ��
*
 * �� �� ֵ  : ��
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_VOID BSP_SCI_TimeStop(BSP_U32 ulStopType);
BSP_VOID I1_BSP_SCI_TimeStop(BSP_U32 ulStopType);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_TM_STOP(ulStopType)    I1_BSP_SCI_TimeStop(ulStopType)
#else
#define DRV_USIMMSCI_TM_STOP(ulStopType)    BSP_SCI_TimeStop(ulStopType)
#endif

/*****************************************************************************
* �� �� ��  : DRV_USIMMSCI_DEACT
*
* ��������  : ���ӿ����ڶ�SIM����ȥ�������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2009��6��29��   liumengcun  creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_Deactive(BSP_VOID);
BSP_U32 I1_BSP_SCI_Deactive(BSP_VOID);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_DEACT()    I1_BSP_SCI_Deactive()
#else
#define DRV_USIMMSCI_DEACT()    BSP_SCI_Deactive()
#endif

/*****************************************************************************
* �� �� ��  : DRV_PCSC_GET_CLK_FREQ
*
* ��������  : ���ӿ����ڻ�ȡ��ǰSIM��ʱ��Ƶ��
*
* �������  : ��
*
* �������  : unsigned long *pLen   ʱ��Ƶ�����ݵĳ���
*             BSP_U8 *pBuf    ʱ��Ƶ������
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2010��8��24��   zhouluojun  creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetClkFreq(unsigned long *pLen, BSP_U8 *pBuf);
BSP_U32 I1_BSP_SCI_GetClkFreq(unsigned long *pLen, BSP_U8 *pBuf);
#if defined(INSTANCE_1)
#define DRV_PCSC_GET_CLK_FREQ(pLen, pBuf)    I1_BSP_SCI_GetClkFreq(pLen, pBuf)
#else
#define DRV_PCSC_GET_CLK_FREQ(pLen, pBuf)    BSP_SCI_GetClkFreq(pLen, pBuf)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_GetBaudRate
*
* ��������  : ���ӿ����ڻ�ȡ��ǰSIM��ʱ��Ƶ��
*
* �������  : ��
*
* �������  : unsigned long *pLen   ���������ݵĳ���
*             BSP_U8 *pBuf    ����������
*
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2010��8��24��   zhouluojun  creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetBaudRate(unsigned long *pLen, BSP_U8 *pBuf);
BSP_U32 I1_BSP_SCI_GetBaudRate(unsigned long *pLen, BSP_U8 *pBuf);
#if defined(INSTANCE_1)
#define DRV_PCSC_GET_BAUD_RATE(pLen, pBuf)    I1_BSP_SCI_GetBaudRate(pLen, pBuf)
#else
#define DRV_PCSC_GET_BAUD_RATE(pLen, pBuf)    BSP_SCI_GetBaudRate(pLen, pBuf)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_GetPCSCParameter
*
* ��������  : ���ӿ����ڻ�ȡ��ǰSIM����PCSC��ز���
*
* �������  : ��
*
* �������  : BSP_U8 *pBuf    PCSC��ز���
*
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2010��8��24��   zhouluojun  creat
*
*****************************************************************************/
BSP_U32 BSP_SCI_GetPCSCParameter(BSP_U8 *pBuf);
BSP_U32 I1_BSP_SCI_GetPCSCParameter(BSP_U8 *pBuf);
#if defined(INSTANCE_1)
#define DRV_PCSC_GET_SCI_PARA(pBuf)    I1_BSP_SCI_GetPCSCParameter(pBuf)
#else
#define DRV_PCSC_GET_SCI_PARA(pBuf)    BSP_SCI_GetPCSCParameter(pBuf)
#endif

/*****************************************************************************
 �� �� ��  : DRV_USIMMSCI_RECORD_DATA_SAVE
 ��������  : ���ӿ����ڻ�ȡ��ǰSIM���������ݡ������쳣����ģ�飬V7�ȴ�׮
 �������  : �ޡ�
 �������  : ��
 �� �� ֵ  : ��
 ע������  ��
*****************************************************************************/
/*extern int BSP_USIMMSCI_STUB(void);*/
BSP_VOID BSP_SCI_RecordDataSave(BSP_VOID);
BSP_VOID I1_BSP_SCI_RecordDataSave(BSP_VOID);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_RECORD_DATA_SAVE()       I1_BSP_SCI_RecordDataSave()
#else
#define DRV_USIMMSCI_RECORD_DATA_SAVE()       BSP_SCI_RecordDataSave()
#endif

typedef void (*OMSCIFUNCPTR)(unsigned int ulVal);
BSP_VOID BSP_SCIFuncRegister(OMSCIFUNCPTR omSciFuncPtr);
BSP_VOID I1_BSP_SCIFuncRegister(OMSCIFUNCPTR omSciFuncPtr);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_FUNC_REGISTER(omSciFuncPtr)    I1_BSP_SCIFuncRegister(omSciFuncPtr)
#else
#define DRV_USIMMSCI_FUNC_REGISTER(omSciFuncPtr)    BSP_SCIFuncRegister(omSciFuncPtr)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_SetBWT
*
* ��������  : ���ӿ��������ÿ�ȴ�ʱ��
*
* �������  : BSP_U32 nBWT :���ó�ʱʱ��ΪBWT��nBWT��
*
* �������  : ��
*
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2013��10��15��   liuyi  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_SetBWT(BSP_U32 nBWT);
BSP_S32 I1_BSP_SCI_SetBWT(BSP_U32 nBWT);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_SETBWT(nBWT)    I1_BSP_SCI_SetBWT(nBWT)
#else
#define DRV_USIMMSCI_SETBWT(nBWT)    BSP_SCI_SetBWT(nBWT)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_Protocol_Switch
*
* ��������  : ���ӿ�����֪ͨ�������T=0��T=1Э�������л�
*
* �������  : RESET_MODE enSIMProtocolMode:��Ҫ�л�Ϊ��Э������
*
* �������  : ��
*
* �� �� ֵ  : OK/ERROR
* �޸ļ�¼  : 2013��10��15��   liuyi  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_Protocol_Switch(PROTOCOL_MODE enSIMProtocolMode);
BSP_S32 I1_BSP_SCI_Protocol_Switch(PROTOCOL_MODE enSIMProtocolMode);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_PROTOCOL_SWITCH(enSIMProtocolMode)    I1_BSP_SCI_Protocol_Switch(enSIMProtocolMode)
#else
#define DRV_USIMMSCI_PROTOCOL_SWITCH(enSIMProtocolMode)    BSP_SCI_Protocol_Switch(enSIMProtocolMode)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_BLK_Receive
* ��������  : ���ӿ�������T=1ʱUSIM Manager��ȡ����SCI Driver�Ŀ��������ݣ�
*             �������BWT��ʱ�������ݶ�ȡʱ����һ����ʱ��ֵSCI_TIMEOUT
*
* �������  : ��
* �������  : BSP_U8 *pu8Data USIM Managerָ����Buffer��SCI Driver�����ݿ�������Buffer��
              BSP_U32 *pulLength SCI Driver���յ������ݳ��ȡ�

* �� �� ֵ  : OK : 0
              ERROR : -1
              SCI_TIMEOUT : 1
*
* �޸ļ�¼  : 2013��10��15��   liuyi  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_BLK_Receive(BSP_U8 *pu8Data,BSP_U32 *pulLength);
BSP_S32 I1_BSP_SCI_BLK_Receive(BSP_U8 *pu8Data,BSP_U32 *pulLength);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_BLK_RCV(pu8Data,pulLength)    I1_BSP_SCI_BLK_Receive(pu8Data,pulLength)
#else
#define DRV_USIMMSCI_BLK_RCV(pu8Data,pulLength)    BSP_SCI_BLK_Receive(pu8Data,pulLength)
#endif

/*****************************************************************************
* �� �� ��  : BSP_SCI_RecordLog_Read
* ��������  : ���ӿ����ڶ�ȡSIM��log���ݣ��������ݲμ��ṹ��SCI_RECORD_INFO
*
* �������  : BSP_U32 ulMaxLength ָ������buff����󳤶�(����С��SCI_RECORD_INFO����)
* �������  : BSP_U8 *pucDataBuff USIM Managerָ����Buffer��
*             SCI Driver��log��Ϣ��������Buffer��
*             BSP_U32 *pulLength SCI Driver������pucDataBuff�ĳ���
*
* �� �� ֵ  : OK : 0
*             ERROR : -1
*
*
* �޸ļ�¼  : 2014��02��24��   liuyi  creat
*
*****************************************************************************/
BSP_S32 BSP_SCI_RecordLog_Read(BSP_U8 *pucDataBuff,BSP_U32 *pulLength,BSP_U32 ulMaxLength);
BSP_S32 I1_BSP_SCI_RecordLog_Read(BSP_U8 *pucDataBuff,BSP_U32 *pulLength,BSP_U32 ulMaxLength);
#if defined(INSTANCE_1)
#define DRV_USIMMSCI_RECORDLOG_READ(pucDataBuff,pulLength,ulMaxLength)    I1_BSP_SCI_RecordLog_Read(pucDataBuff,pulLength,ulMaxLength)
#else
#define DRV_USIMMSCI_RECORDLOG_READ(pucDataBuff,pulLength,ulMaxLength)    BSP_SCI_RecordLog_Read(pucDataBuff,pulLength,ulMaxLength)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of drv_sci.h */

