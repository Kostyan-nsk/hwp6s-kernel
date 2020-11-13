/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  icc_mntn.h
*
*   ��    �� :  zhaikuixiu
*
*   ��    �� :  ���ļ�����Ϊ"icc_mntn.h"
*
*   �޸ļ�¼ :  2011��12��07��  v1.00  zhaikuixiu����
*************************************************************************/
/*#include "omDrv.h"*/

#define DRV_MNTN_HEADER_FLAG       (0xAAAA)/*�Ƿ���Ҫ���������Ҫ������usb_mntn.h��ɾ��*/


/*ɾ��DRV_TRACE_IND_STRU�ṹ�����ĸ��ֽڣ�����DRV_TRACE_IND_STRU
       ֻ��ʾ��Ϣͷ���������κ���ʵ���͵�����*/
/*
#define ICC_MNTN_ERRLOG_SIZE   (sizeof(DRV_TRACE_IND_STRU)  \
                             + sizeof(icc_mntn_info_header) - 4)
*/

/*********om stub**********/
/***from omdrv.h  v3r2***/
/****Driverģ��ID��,��31�� *******/
/****�벻Ҫ���м����moduleId*****/
/**Ҫ����moduleId���������������*/
typedef enum
{
    DRV_MODULE_ID_AMTS   = 0x8101,
    DRV_MODULE_ID_AUDIO ,
    DRV_MODULE_ID_BSP ,
    DRV_MODULE_ID_CAMERA ,
    DRV_MODULE_ID_DM  ,
    DRV_MODULE_ID_DMA ,
    DRV_MODULE_ID_DSPC,
    DRV_MODULE_ID_END ,
    DRV_MODULE_ID_FDI,
    DRV_MODULE_ID_FLASH ,
    DRV_MODULE_ID_FM,
    DRV_MODULE_ID_GPIO,
    DRV_MODULE_ID_HICONTRL,
    DRV_MODULE_ID_HKADC,
    DRV_MODULE_ID_I2C,
    DRV_MODULE_ID_KEYPAD,
    DRV_MODULE_ID_LCD,
    DRV_MODULE_ID_LOGGER,
    /*DRV_MODULE_ID_MNTN,*/
    DRV_MODULE_ID_PM,
    DRV_MODULE_ID_RTC,
    DRV_MODULE_ID_SCI,
    DRV_MODULE_ID_SDMMC,
    DRV_MODULE_ID_SPI,
    DRV_MODULE_ID_USB,
    DRV_MODULE_ID_WDT,
    DRV_MODULE_ID_NVIM,
    DRV_MODULE_ID_SYSCTRL,
    DRV_MODULE_ID_MNTN ,
    DRV_MODULE_ID_TIMER,
    DRV_MODULE_ID_VIC,
    DRV_MODULE_ID_YAFFS,
    DRV_MODULE_ID_SDIO,
    DRV_MODULE_ID_WIFIEND,
    DRV_MODULE_ID_WIFIMVL,
    DRV_MODULE_ID_OLED,
    DRV_MODULE_ID_ONOFF,
#if(FEATURE_ICC_DEBUG == FEATURE_ON)
    DRV_MODULE_ID_ICC=0x812E,
#endif	
}LOG_MODULE_ID_EN;



/*********om stub*********/
typedef struct
{
    BSP_U8  ucFuncType;
    BSP_U8  ucReserve;
    BSP_U16 usLength;
    BSP_U32 ulSn;
    BSP_U32 ulTimeStamp;
    BSP_U16 usPrimId;
    BSP_U16 usReserve;
    BSP_U32 ulModuleId;
}DRV_TRACE_IND_STRU;


#define ICC_MNTN_ERRLOG_SIZE   (sizeof(DRV_TRACE_IND_STRU)  \
                             + sizeof(icc_mntn_info_header))

typedef enum
{
    DRV_PRIM_ICC_REQ= 0X9501,
    DRV_PRIM_ICC_CNF= 0X9502,
    DRV_PRIM_ICC_ERRLOG= 0X9503,

}ICC_PRIMID;

typedef struct
{
    BSP_U32 header_flag;
    BSP_U32 u32ChanID;
    BSP_S32 s32Errlog;
    BSP_U16 module_id;
    BSP_U16 prim_id;
    BSP_S32 s32Datesize;
    BSP_U32 time_stamp;
}icc_mntn_info_header;


typedef struct
{
    BSP_U32			u32ChanID;
    BSP_S32                      s32Errlog;
    BSP_S32                      s32Datesize;
}icc_mntn_info_t;


//extern BSP_U32 g_usb_mntn_func_type_on;
extern void    BSP_MNTN_IccErrlogInit(void);
extern void    BSP_MNTN_IccErrlogUnInit(void);
extern void    BSP_ICC_Errlog_Save( BSP_U16  module_id,
                                     BSP_U16  prim_id,
                                     void     *buffer);


