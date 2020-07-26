/******************************************************************************

    Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : msp_diag.h
  Description     : msp_diag.h header file
  History         :
     1.w00182550       2012-11-20     Draft Enact
     2. 
******************************************************************************/

#ifndef __MSP_DIAG_H__
#define __MSP_DIAG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include Headfile                            
*****************************************************************************/
#include "vos.h"


#pragma pack(4)

/*****************************************************************************
  2 macro
*****************************************************************************/
/*���PS��MODID ��DIAG_AIR_MSG_LOG_ID�ĵط����滻��DIAG_ID*/
#define DIAG_ID(module_id, log_type)   (VOS_UINT32)(module_id | (log_type << 12))

#define MODID(module_id, log_type) DIAG_ID(module_id, log_type)

#define DIAG_AIR_MSG_LOG_ID(module_id, is_up_link)  DIAG_ID(module_id, is_up_link) /*module_id��ӦPID*/


/*is_up_linkȡֵ*/
#define OS_MSG_UL                (0x01)/* ��ʾ������Ϣ*/
#define OS_MSG_DL                (0x02)/* ��ʾ������Ϣ*/
#define LPS_OM_SPECIFY_FLG       (0x01)


#define DIAG_SIDE_UE             (0x1)  /* ��ʾUE���յĿտ���Ϣ��NET-->UE*/
#define DIAG_SIDE_NET            (0x2)  /* ��ʾNET���յĿտ���Ϣ��UE-->NET*/


/* �¼���Ϣ��ӡ������*/
#define LOG_TYPE_INFO            0x00000008UL
#define LOG_TYPE_AUDIT_FAILURE   0x00000004UL
#define LOG_TYPE_AUDIT_SUCCESS   0x00000002UL
#define LOG_TYPE_ERROR           0x00000001UL
#define LOG_TYPE_WARNING         0x00000010UL

/*DIAG��ϢID����*/
/*diag fw�����Լ��Ľ�����Ϣ��������*/
#define ID_MSG_DIAG_CMD_REQ_FW_TO_FW                (0x00010001)

/*diag fw����AGENTģ��������Լ���Ӧ�Ļظ���Ϣ*/
#define ID_MSG_DIAG_CMD_REQ_FW_TO_AGENT             (0x00010002)
#define ID_MSG_DIAG_CMD_CNF_AGENT_TO_FW             (0x00010003)
#define ID_MSG_DIAG_HSO_DISCONN_IND                 (0x00010004)

/*diag fw����APP AGENTģ��������Լ���Ӧ�Ļظ���Ϣ*/
#define ID_MSG_DIAG_CMD_REQ_FW_TO_APP_AGENT         (0x00010005)
#define ID_MSG_DIAG_CMD_CNF_APP_AGENT_TO_FW         (0x00010006)

/*diag AGENT����BBP AGENTģ�������*/
#define ID_MSG_REQ_DRX_DATA_SAMPLE_START            (0x00010009)
#define ID_MSG_REQ_DRX_DATA_SAMPLE_STOP             (0x0001000A)
#define ID_MSG_REQ_DRX_LOG_SAMPLE_START             (0x0001000B)
#define ID_MSG_REQ_DRX_LOG_SAMPLE_STOP              (0x0001000C)

#define ID_MSG_DIAG_CMD_IND                         (0x0001000D)

/*diag AGENT����PSģ���HSO�ط�����*/
#define ID_MSG_DIAG_CMD_REPLAY_TO_PS    			(0x00010100)


/*****************************************************************************
  3 Massage Declare 
*****************************************************************************/


/*****************************************************************************
  4 Enum 
*****************************************************************************/


/*****************************************************************************
   5 STRUCT 
*****************************************************************************/


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
 Function Name   : DIAG_PrintfV
 Description     : ��ӡ�ϱ��ӿڣ���PS����
 Input           :VOS_UINT32 id          
                VOS_CHAR* pszFileName  
                VOS_UINT32 ulLineNum   
                VOS_CHAR* pszFmt       
                ...                    
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/

/*��������ֽ���,����ǰ���ļ������кų��ȣ�-1��Ԥ��\0������*/
#define DIAG_PRINTF_MAX_LEN 	(1024-1)

extern VOS_UINT32 DIAG_PrintfV_TDS( VOS_CHAR* pszFmt, ...);

extern VOS_UINT32 DIAG_PrintfV_TDS1( VOS_UINT32 id ,VOS_CHAR* pszFmt, ...);

extern VOS_UINT32 DIAG_PrintfV(VOS_UINT32 id, VOS_CHAR* pszFileName, VOS_UINT32 ulLineNum, VOS_CHAR* pszFmt, ...);

/*****************************************************************************
 Function Name   : DIAG_ReportEventLog
 Description     : �¼��ϱ��ӿڣ���PS����
 Input           :VOS_UINT32 ulId       
                VOS_UINT32 ulEventID  
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
extern VOS_UINT32 DIAG_ReportEventLog(VOS_UINT32 ulId, VOS_UINT32 ulEventID);


/*****************************************************************************
 Function Name   : DIAG_ReportAirMessageLog
 Description     : L�տ��ϱ��ӿڣ���PS����
 Input           :DIAG_AIR_MSG_LOG_STRU* pRptMessage  
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
typedef struct
{
    VOS_UINT32 ulId;
    VOS_UINT32 ulMessageID;  /* Specify the message id.*/
    VOS_UINT32 ulSideId;     /* �տ���Ϣ�ķ���*/
    VOS_UINT32 ulDestMod;    /* Ŀ��ģ��*/
    VOS_UINT32 ulDataSize;   /* the data size in bytes.*/
    VOS_VOID* pData;      /* Pointer to the data buffer.*/
} DIAG_AIR_MSG_LOG_STRU;
extern VOS_UINT32 DIAG_ReportAirMessageLog(DIAG_AIR_MSG_LOG_STRU* pRptMessage);
typedef struct
{
    VOS_UINT32 ulId;
    VOS_UINT32 ulMessageID;  /* Specify the message id.*/
    VOS_UINT32 ulSideId;     /* VoLTE��Ϣ�ķ���,0x1:���ൽUE�� 0x2:UE�ൽ���� 0xFFFFFFFF:Ϊ��Чֵ,�����뷽���޹�*/
    VOS_UINT32 ulDestMod;    /* Ŀ��ģ��*/
    VOS_UINT32 ulDataSize;   /* the data size in bytes.*/
    VOS_VOID*  pData;        /* Pointer to the data buffer.*/
} DIAG_VoLTE_LOG_STRU;

extern VOS_UINT32 DIAG_ReportVoLTELog(DIAG_VoLTE_LOG_STRU* pRptMessage);
/*****************************************************************************
 Function Name   : DIAG_ReportGUAirMessageLog
 Description     : L�տ��ϱ��ӿڣ���PS����
 Input           :VOS_VOID* pRptMessage  
                VOS_UINT16 ulMsgLen    
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
typedef struct
{
    VOS_UINT8	    ucFuncType;
    VOS_UINT8       ucReserved1;
    VOS_UINT16      usLength;
    VOS_UINT32      ulSn;           /*Sequence Number for Trace, Event, OTA msg.*/
    VOS_UINT32      ulTimeStamp;    /*CPU time coming from ARM.*/
	VOS_UINT16      usPrimId;
    VOS_UINT16      usToolId;
	VOS_UINT16      usOtaMsgID;         /*�տ���Ϣ��ID��*/
    VOS_UINT8       ucUpDown;           /*�տ���Ϣ�ķ���*/
    VOS_UINT8       ucReserved;         /*�߼��ŵ�����*/
    VOS_UINT32      ulLengthASN;        /*�տ���Ϣ���ݵĳ���*/
	VOS_VOID*    pData;                 /* Pointer to the data buffer.*/
} DIAG_GU_AIR_MSG_LOG_STRU;
extern VOS_UINT32 DIAG_ReportGUAirMessageLog(VOS_VOID* pRptMessage,VOS_UINT16 ulMsgLen);


/*****************************************************************************
 Function Name   : DIAG_ReportLayerMessageLog
 Description     : �����Ϣ�ϱ��ӿڣ���PS����
 Input           :DIAG_LAYER_MSG_STRU* pRptMessage  
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
typedef DIAG_AIR_MSG_LOG_STRU DIAG_LAYER_MSG_STRU;

extern VOS_UINT32 DIAG_ReportLayerMessageLog(DIAG_LAYER_MSG_STRU* pRptMessage);

/*****************************************************************************
 Function Name   : DIAG_ReportUserPlaneMessageLog
 Description     : �û�����Ϣ�ϱ��ӿڣ���PS����
 Input           :DIAG_USERPLANE_MSG_STRU* pRptMessage  
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/

typedef struct
{
    VOS_UINT32 ulMessageID;    /* Specify the message id.*/
    VOS_UINT32 ulDataSize;     /* the data size in bytes.*/
    VOS_VOID* pData;        /* Pointer to the data buffer.*/
} DIAG_USERPLANE_MSG_STRU;

extern VOS_UINT32 DIAG_ReportUserPlaneMessageLog(DIAG_USERPLANE_MSG_STRU* pRptMessage);

/*****************************************************************************
 Function Name   : DIAG_ReportCommand
 Description     : �����ϱ��ӿڣ���PS��MSP����
 Input           :VOS_UINT16 ulID        
                VOS_UINT32 ulDataSize  
                VOS_VOID* pData        
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
extern VOS_UINT32 DIAG_ReportCommand(VOS_UINT16 ulID,VOS_UINT32 ulDataSize,VOS_VOID* pData);


/*****************************************************************************
 Function Name   : DIAG_ReportCommandPack
 Description     : MSP�ڲ������ϱ��ӿڣ���װ��ע��ʹ��
 Input           :VOS_UINT32 ulID        
                VOS_UINT32 ulDataSize  
                VOS_VOID* pData        
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-11-20  Draft Enact 

*****************************************************************************/

VOS_UINT32 DIAG_ReportCommandPack(VOS_UINT32 ulID, VOS_VOID* pData, VOS_UINT32 ulDataSize);
/*****************************************************************************
 Function Name   : DIAG_GtrRcvCallBackReg
 Description     :�Զ��������ϱ��ӿڣ���PS����
 Input           :DIAG_GTR_DATA_RCV_PFN pfnGtrRcv  
 Output          : None
 Return          : VOS_UINT32
 
 History         :
    1.w00182550      2012-12-26  Draft Enact 

*****************************************************************************/
typedef VOS_UINT32 (*DIAG_GTR_DATA_RCV_PFN)(VOS_UINT32 ulGtrDtaSize, VOS_VOID* pGtrData);

extern VOS_VOID DIAG_GtrRcvCallBackReg(DIAG_GTR_DATA_RCV_PFN pfnGtrRcv);

/*****************************************************************************
 Function Name   : diag_LogPortSwitch
 Description     :diag log port�л�����ATʹ��
 Input           :ulLogPort   0:usb   1: vcom
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.y00228784     2012-12-26  Draft Enact

*****************************************************************************/
/*�ṩ��AT����*/
VOS_UINT32 diag_LogPortSwitch(VOS_UINT32 ulLogPort);

/*****************************************************************************
 Function Name   : diag_getLogPort
 Description     :���ص�ǰ�˿ںţ���ATʹ��
 Input           :None
 Output          : None
 Return          : VOS_UINT32 0:usb   1: vcom

 History         :
    1.y00228784     2012-12-26  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_getLogPort(VOS_VOID);




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

#endif /* end of msp_diag.h */
