/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : pwrctrl_multi_sleep.h
  �� �� ��   : ����
  ��    ��   : ������ 56193
  ��������   : 2012��9��6��
  ����޸�   :
  ��������   : �͹��Ķ�˹�����ͷ�ļ���˯�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��6��
    ��    ��   : ������ 56193
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "pwrctrl_multi_def.h"
#include  "soc_baseaddr_interface.h"
#include  "soc_ao_sctrl_interface.h"


#ifndef __PWRCTRL_MULTI_SLEEP_H__
#define __PWRCTRL_MULTI_SLEEP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define PWC_VOTE_STAT_LOCK          (0xA1)   /*ͶƱ��������˯*/
#define PWC_VOTE_STAT_UNLOCK        (0xA0)   /*ȥͶƱ������˯*/
#define PWC_VOTE_MODULE_SLEEP_SOFT  (0x50)   /*���ͶƱ*/
#define PWC_VOTE_MODULE_SLEEP_HW    (0x51)   /*Ӳ��ͶƱ*/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum tagPWC_SC_RESERVED28_ID_E
{
    PWC_PD_SRAMBAK_RESTORE_ID_BIT      =    (0x1 << 0),

    PWC_SC_RESERVED28_ID_MAX           =    (0x1 << 31)
}PWC_SC_RESERVED28_ID_E;

typedef enum tagPWC_PD_MOUDLE_ID_E
{
    /*��ϸbit����ο�sc�Ĵ���SC_MTCMOS_STAT0��SC_MTCMOS_STAT1*/
    PWC_PD_ID_PERI          = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_0peri_START,
    PWC_PD_ID_G3D           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_1g3d_START,
    PWC_PD_ID_G2D           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_2g2d_START,
    PWC_PD_ID_EDC0          = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_3edc0_START,
    PWC_PD_ID_EDC1          = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_4edc1_START,
    PWC_PD_ID_ISP           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_5isp_START,
    PWC_PD_ID_ENC           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_6enc_START,
    PWC_PD_ID_DEC           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_7dec_START,

    PWC_PD_ID_HIFI          = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_8hifi_START,
    PWC_PD_ID_VPP           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_9vpp_START,
    PWC_PD_ID_BBE           = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_10dspbbe_START,
    PWC_PD_ID_DDRCTRL       = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_11ddrctrl_START,
    PWC_PD_ID_BBP2C         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_12bbpcommtop2c_gbbp1_START,
    PWC_PD_ID_BBP2D         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_13bbpcommtop2d_gbbp2_START,
    PWC_PD_ID_BBP3          = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_14ltebbptop_START,  /*Lbbp*/
    PWC_PD_ID_BBP2B         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_15bbpcommtop2b_bbpcomm2_START,
    PWC_PD_ID_BBP2A         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_16bbpcommtop2a_bbpcomm1_START,
    PWC_PD_ID_BBP1B         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_17bbpcommtop1b_wtdbbp2_START,
    PWC_PD_ID_BBP1A         = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_18bbpcommtop1a_wtdbbp1_START,
    PWC_PD_ID_TCXO0_AFC_MSK = SOC_AO_SCTRL_SC_ALWAYSON_SYS_CTRL4_tcxo0_afc_high_z_msk_START,
    PWC_PD_ID_TCXO1_AFC_MSK = SOC_AO_SCTRL_SC_ALWAYSON_SYS_CTRL4_tcxo1_afc_high_z_msk_START,
    PWC_PD_ID_TCXO0_AFC     = SOC_AO_SCTRL_SC_ALWAYSON_SYS_CTRL4_tcxo0_afc_high_z_START,
    PWC_PD_ID_TCXO1_AFC     = SOC_AO_SCTRL_SC_ALWAYSON_SYS_CTRL4_tcxo1_afc_high_z_START,
    PWC_PD_ID_ABBANA        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_19abbana_START,     /*û�и÷�����������һ����*/
    PWC_PD_ID_USBPHY        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_20usbphy_START,
    PWC_PD_ID_RESERVE1      = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_21reserved_START,
    PWC_PD_ID_CS0PHY        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_22cs0phy_START,
    PWC_PD_ID_RESERVE2      = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_23reserved_START,
    PWC_PD_ID_CS2PHY        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_24cs2phy_START,
    PWC_PD_ID_DSIPHY        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_25dsiphy_START,
    PWC_PD_ID_HDMIPHY       = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_26hdmiphy_START,
    PWC_PD_ID_DDRPHY        = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_27ddrphy_START,
    PWC_PD_ID_reserved      ,
    PWC_PD_ID_TSENSOR0      = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_29tsensor0_START,
    PWC_PD_ID_TSENSOR1      = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_30tsensor1_START,
    PWC_PD_ID_RESERVE3      = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_31reserved_START,

    PWC_PD_ID_ACPU0         = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_0acpu0_START,      /*��Ҫ������������������Ҫ��*/
    PWC_PD_ID_ACPU1         = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_1acpu1_START,
    PWC_PD_ID_ACPU2         = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_2acpu2_START,
    PWC_PD_ID_ACPU3         = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_3acpu3_START,
    PWC_PD_ID_CM3MEM        = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_4cm3mem_START,
    PWC_PD_ID_CCPU          = 32 + SOC_AO_SCTRL_SC_PW_EN1_pw_en1_5mcpu_START,       /*��Ҫ������������������Ҫ��*/

    PWC_PD_ID_BUTT          = 38

} PWC_PD_MOUDLE_ID;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct PWC_EXC_SLEEP_CORE_LOG_STRU_S
{
    u32_t  SleepCount;         //+0X0
    u32_t  WakeCount;          //+0X4
    u32_t  SleepSliceTime;     //+0X8
    u32_t  WakeSliceTime;      //+0Xc

    u32_t  WfiFailCount;       //+0X10
    u32_t  WfiFailSliceTime;   //+0X14
    u32_t  status;             //+0X18
    u32_t  wakesrcIPC;         //+0X1c

    u32_t  wakesrc0;           //+0x20
    u32_t  wakesrc1;           //+0x24

}PWC_MCU_EXC_SLEEP_CORE_LOG_STRU;

typedef struct
{
    void* p;
    u32_t size;
} PWC_MCU_EXC_DFS_LOG_NODE_STRU;

typedef struct PWC_MCU_EXC_LOG_STRU_S
{

    PWC_MCU_EXC_SLEEP_CORE_LOG_STRU  ccpu;
    PWC_MCU_EXC_SLEEP_CORE_LOG_STRU  acpu0;
    PWC_MCU_EXC_SLEEP_CORE_LOG_STRU  hifi;
    PWC_MCU_EXC_SLEEP_CORE_LOG_STRU  peri;

    u32_t mcuNxtSchTimeId;
    u32_t mcuNxtSchTimeValue;

    u32_t mcuVoteMap;
    u32_t mcuHWVoteMap;
    s32_t mcuCanDeepSleepRet;

    u8_t wakesrcGPIO[8];

    PWC_MCU_EXC_DFS_LOG_NODE_STRU  dfs_log_node[64];

}PWC_MCU_EXC_LOG_STRU;

typedef struct PWRCTRL_CCPU_EXC_LOG_STRU_S
{

    u32_t  cSleepCount;         /*+0X0*/
    u32_t  cWakeCount;          /*+0X4*/

    u32_t  cSleepMgrSliceTime;  /*+0X8*/
    u32_t  cSleepSliceTime;       /*+0XC*/
    u32_t  cWakeSliceTime;      /*+0X10*/
    u32_t  cWakeMgrSliceTime;   /*+0X14*/

    u32_t  cSleepIpcCount;      /*+0X18*/
    u32_t  cSleepIpcSliceTime;  /*+0X1C*/
    u32_t  cWfiRetCount;        /*+0X20*/
    u32_t  cWfiRetSliceTime;    /*+0X24*/

    u32_t   cNxtSchTimeId;
    u32_t   cNxtSchTimeValue;

    u32_t   cVoteMap;
    u32_t   cHWVoteMap;
    s32_t   cCanDeepSleepRet;

    u32_t   reserved[29];

}PWRCTRL_CCPU_EXC_LOG_STRU;


typedef struct PWC_VOTE_LOG_STRU_S
{
    u32_t voteStat:8;     /*PWC_VOTE_STAT_LOCK ���� PWC_VOTE_STAT_UNLOCK*/
    u32_t voteModuleId:8; /*PWC_VOTE_MODULE_SLEEP_SOFT ���� PWC_VOTE_MODULE_SLEEP_HW*/
    u32_t reserved:8;     /*����*/
    u32_t clientId:8;     /*����ΪPWC_CLIENT_ID_E�ĵ�1���ֽ�*/
    u32_t voteMap;        /*���ͶƱֵ����Ӳ��ͶƱֵ*/
    u32_t taskId;         /*�˴�ͶƱ������ID*/
    u32_t slice;          /*�˴�ͶƱ��sliceʱ��ֵ*/
}PWC_VOTE_LOG_STRU;     /*�ýṹ���СΪ16���ֽ�*/


typedef struct ST_STORE_ADDR_INFO_S
{
    u32_t ulStartAddr;            /*��Ҫ���ݵļĴ�����ַ��Ϣ*/
    u32_t ulLength;               /*���ݳ��ȣ���Ҫ4�ֽڶ���,��λ32bit*/
} ST_STORE_REG_ADDR_INFO;

typedef struct ST_STORE_BIT_ADDR_INFO_S
{
    u32_t ulAddr;               /*��Ҫ���ݵļĴ�����ַ��Ϣ*/
    u32_t ulToAddr;             /*�ָ���ַ,��λ32bit*/
    u32_t ulMask;               /*�ָ�����,��λ32bit*/
    u32_t ulSetMask;            /*����дʹ��,��λ32bit*/
} ST_STORE_BIT_ADDR_INFO;


typedef struct ST_VOTE_HW_INFO_S
{
    u32_t voteEn;                   /*����ͶƱ������ֹ˯��*/
    u32_t voteDis;                  /*˯��ͶƱ��������˯��*/
    u32_t voteStat;                 /*ͶƱ״̬�Ĵ���������ǰ*/
    u32_t voteSleepMsk;             /*˯��ͶƱ����*/
    u32_t voteWakeMsk;              /*����ͶƱ����*/
    u32_t voteSleepStat;            /*ͶƱ״̬�Ĵ��������κ�*/
    u32_t voteWakeStat;             /*ͶƱ״̬�Ĵ��������κ�*/
} ST_VOTE_HW_INFO;

typedef struct ST_SYSCTRL_INT_INFO_S
{
    u32_t intEnStat;                /*����ͶƱ������ֹ˯��*/
    u32_t intStatR;                 /*˯��ͶƱ��������˯��*/
    u32_t intStatM;                 /*ͶƱ״̬�Ĵ���������ǰ*/
    u32_t intClr;                   /*˯��ͶƱ����*/
    u32_t intEn0;                   /*����ͶƱ����*/
    u32_t intDis0;                  /*ͶƱ״̬�Ĵ��������κ�*/
} ST_SYSCTRL_INT_HW_INFO;

/*w00176398 add begin*/
typedef struct ST_SET_BIT_ADDR_INFO_S
{
    u32_t ulAddr;               /*��Ҫ���õļĴ�����ַ��Ϣ*/
    u32_t ulMask;               /*����λ,��λ32bit*/
    u32_t ulvalue;              /*Ҫд���ֵ,��λ32bit*/
} ST_SET_BIT_ADDR_INFO;
/*w00176398 add end*/
/*****************************************************************************
  7 UNION����
*****************************************************************************/
typedef enum tagPWC_TELE_SLEEP_MODE_ID_E
{
    /*BIT0-7 HW*/
    PWRCTRL_TELE_INVALID = -1,
    PWRCTRL_TELE_CCPU      = 0,
    PWRCTRL_TELE_PERI      = 1,
    PWRCTRL_TELE_ACPU      = 2,
    PWRCTRL_TELE_REV0      = 3,
    PWRCTRL_TELE_REV1      = 4,
    PWRCTRL_TELE_REV2      = 5
} PWC_TELE_SLEEP_MODE_ID_E;

typedef enum tagPWC_TELE_SLEEP_CLIENT_ID_E
{
    /*BIT0-7 HW*/
    PWRCTRL_TELE_CLIENT_INVALID    = -1,
    PWRCTRL_TELE_SLEEP_CSIGHT0      = 0,
    PWRCTRL_TELE_SLEEP_CSIGHT1      = 1,
    PWRCTRL_TELE_SLEEP_IPF          = 2,
    PWRCTRL_TELE_SLEEP_SOCP         = 3,

    /* Test */
    PWRCTRL_TELE_CLIENT_BUTT        = 32
} PWC_TELE_SLEEP_CLIENT_ID_E;



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

#endif /* end of pwrctrl_multi_sleep.h */
