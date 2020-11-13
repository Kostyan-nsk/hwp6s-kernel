

/************************************************************
                     ��������ģ���ͷ�ļ�
************************************************************/
/************************************************************
                               �궨��
************************************************************/

/************************************************************
                             ���ݽṹ����
************************************************************/

/************************************************************
                             �ӿں�������
 ************************************************************/

#ifndef __MEM_MAP_BASE_H__
#define __MEM_MAP_BASE_H__

#include "product_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#if (defined(MS_VC6_PLATFORM))

    #define MASTER_TCM_ADDR_OFFSET  (0x0)
    #define SLAVE_TCM_ADDR_OFFSET   MASTER_TCM_ADDR_OFFSET
    /*�������鶨��*/
    extern UINT32  gulArrMailxBox[];       /*400k�ֽڴ�С*/
    /*HARQ�����������С*/
    extern UINT32  gulArrHarqBuf[];        /*400k�ֽڴ�С*/

    /*ģ������*/
    #define LPHY_SHARE_MEM_BASE_ADDR                ((UINT32)&gulArrMailxBox[0])
    #define LPHY_DSP_ARM_RAM_BASE_OFFSET            (0x00000000UL)

    /*ģ��HARQ������*/
    #define LPHY_LMAC_HARQ_BUFF_BASE_ADDR           ((UINT32)&gulArrHarqBuf[0])

/* ����ַ����*/
#elif defined(BOARD_FPGA)

    #define MASTER_TCM_ADDR_OFFSET  (0x0 - 0xf000000)
    #define SLAVE_TCM_ADDR_OFFSET   (0x0 - 0xd000000)
    /*���乲����ض���*/
    #define LPHY_SHARE_MEM_BASE_ADDR                (0xE0FC0000UL)
    #define LPHY_DSP_ARM_RAM_BASE_OFFSET            (0x0F000000UL)

    /*HARQ����������ַ����*/
    #define LPHY_LMAC_HARQ_BUFF_BASE_ADDR           (0x52680000UL)
    #define LPHY_LMAC_TIMER_LOCK_ADDR               (0x5270E094UL)
    #define LPHY_LMAC_FRAME_ADDR                    (0x5270E014UL)
    #define LPHY_LMAC_SUB_FRAME_ADDR                (0x5270E018UL)

    /*���ص�ַ����BSP��*/
    #define LPHY_CORE0_BINARY_LOAD_ADDR             (0xE0FC0000UL) /*P500 bbe16 core0*/
    #define LPHY_CORE1_BINARY_LOAD_ADDR             (0xE2FC0000UL) /*P500 bbe16 core1*/

    /*ӳ���ַ����vxworks ӳ��ʹ��*/
    #define LPHY_CORE_TCM_MAP_BASE_ADDR             (LPHY_CORE0_BINARY_LOAD_ADDR)
    #define LPHY_CORE_TCM_MAP_SIZE                  (256*1024 + 256*1024) /*256K + 256K*/

#elif defined(TL_PHY_FPGA_P531)

	#define MASTER_TCM_ADDR_OFFSET  				(0x01000000)
	#define SLAVE_TCM_ADDR_OFFSET   				(0x02000000)

	/*���乲����ض���*/
	#define LPHY_SHARE_MEM_BASE_ADDR                (0x49f80000UL)
	#define LPHY_DSP_ARM_RAM_BASE_OFFSET            (0xFF000000UL)

	/*HARQ����������ַ����*/
	#define LPHY_LMAC_HARQ_BUFF_BASE_ADDR           (0x20680000UL)
	#define LPHY_LMAC_TIMER_LOCK_ADDR               (0x2070E094UL)
	#define LPHY_LMAC_FRAME_ADDR                    (0x2070E014UL)
	#define LPHY_LMAC_SUB_FRAME_ADDR                (0x2070E018UL)

	/*���ص�ַ����BSP��*/
	#define LPHY_CORE0_UNI_DTCM_LOAD_ADDR           (0x49f80000UL) /*P530 bbe16 UNIPHY DTCM*/
	#define LPHY_CORE0_UNI_DTCM_LOAD_SIZE           (0x00080000UL) /*P530 bbe16 UNIPHY DTCM*/
	#define LPHY_CORE0_PUB_DTCM_LOAD_ADDR           (0x49f80000UL) /*P530 bbe16 PUBPHY DTCM*/
	#define LPHY_CORE0_PUB_DTCM_LOAD_SIZE           (0x0003f000UL) /*P530 bbe16 PUBPHY DTCM*/
	#define LPHY_CORE0_MOD_DTCM_LOAD_ADDR           (0x49fbf000UL) /*P530 bbe16 MODPHY DTCM*/
	#define LPHY_CORE0_MOD_DTCM_LOAD_SIZE           (0x00040000UL) /*P530 bbe16 MODPHY DTCM*/

	#define LPHY_CORE0_UNI_ITCM_LOAD_ADDR           (0x4a000000UL) /*P530 bbe16 UNIPHY ITCM*/
	#define LPHY_CORE0_UNI_ITCM_LOAD_SIZE           (0x00080000UL) /*P530 bbe16 UNIPHY DTCM*/
	#define LPHY_CORE0_PUB_ITCM_LOAD_ADDR           (0x4afff000UL) /*P530 bbe16 PUBPHY ITCM*/
	#define LPHY_CORE0_PUB_ITCM_LOAD_SIZE           (0x00041000UL) /*P530 bbe16 PUBPHY DTCM*/
	#define LPHY_CORE0_MOD_ITCM_LOAD_ADDR           (0x4a040000UL) /*P530 bbe16 MODPHY ITCM*/
	#define LPHY_CORE0_MOD_ITCM_LOAD_SIZE           (0x00040000UL) /*P530 bbe16 MODPHY DTCM*/


	/*ӳ���ַ����vxworks ӳ��ʹ��*/
	#define LPHY_CORE_TCM_MAP_BASE_ADDR             (LPHY_CORE0_PUB_DTCM_LOAD_ADDR)
	#define LPHY_CORE_TCM_MAP_SIZE                  (512*1024 + 512*1024) /*256K + 256K*/

	#define LPHY_CHIP_BB_6930ES
	#define LPHY_RFIC_HI6361
#elif (defined(BOARD_ASIC) || defined(BOARD_ASIC_BIGPACK) ||defined(BOARD_SFT))

    #define MASTER_TCM_ADDR_OFFSET  (0x0)
    #define SLAVE_TCM_ADDR_OFFSET   MASTER_TCM_ADDR_OFFSET
    /*���乲����ض���*/
    //#if defined(CHIP_BB_6920ES)
    //    #define LPHY_SHARE_MEM_BASE_ADDR                (0x27FC0000UL) /*256K*/
    //#else
    //    #define LPHY_SHARE_MEM_BASE_ADDR                (0x27FB0000UL) /*64K+256K*/
    //#endif

    //V9R1_ASIC start
    #define LPHY_SHARE_MEM_BASE_ADDR                (0xFA880000UL) /*tensilica dtcm��ַ */
    #define LPHY_DSP_ARM_RAM_BASE_OFFSET            (0x00000000UL)

    /*HARQ����������ַ����*/
    #define LPHY_LMAC_HARQ_BUFF_BASE_ADDR           (0xFD680000UL)
    #define LPHY_LMAC_TIMER_LOCK_ADDR               (0xFD70E094UL)
    #define LPHY_LMAC_FRAME_ADDR                    (0xFD70E014UL)
    #define LPHY_LMAC_SUB_FRAME_ADDR                (0xFD70E018UL)
	//V9R1_ASIC end

    /*���ص�ַ����BSP��*/
    #define LPHY_CORE0_BINARY_LOAD_ADDR             (0xFA800000UL) /*tensilica���ص�ַ��itcm��ַ*/

    /*ӳ���ַ����vxworks ӳ��ʹ��*/
    #define LPHY_CORE_TCM_MAP_BASE_ADDR             (LPHY_CORE0_BINARY_LOAD_ADDR)
    #define LPHY_CORE_TCM_MAP_SIZE                  (64*1024 + 256*1024 + 256*1024 + 128*1024) /*64K + 256K + 256K + 128K*/

#else

    #error "sorry, this is not a supported board version, please include product_config.h"

#endif

/*HARQ��������ַ*/
#if defined(CHIP_BB_6920ES)     /*HI6920ES*/
    #define LPHY_LMAC_HARQ_BUFF_ADDR_BY_HARQID(ucHarqId) (LPHY_LMAC_HARQ_BUFF_BASE_ADDR + (ucHarqId*0x2000))
#elif defined(CHIP_BB_6620CS) /*HI6620CS*/
    #define LPHY_LMAC_HARQ_BUFF_ADDR_BY_HARQID(ucHarqId) (LPHY_LMAC_HARQ_BUFF_BASE_ADDR + (ucHarqId*0x2000))
#elif defined(CHIP_BB_6920CS) /*HI6920CS*/
    #define LPHY_LMAC_HARQ_BUFF_ADDR_BY_HARQID(ucHarqId) (LPHY_LMAC_HARQ_BUFF_BASE_ADDR + (ucHarqId*0x4000))
#else
    #error "sorry, this is not a supported baseband version, please include product_config.h"
#endif


/* ���������С����*/
#define LPHY_MAILBOX_SHARE_MEM_SIZE                 (0x00000400UL)  /*1k�ֽڣ�TBD*/
#define LPHY_MAILBOX_HDLC_BUF_SIZE                  (0x00001800UL)  /*6K�ֽ�, TBD*/
#define LPHY_MAILBOX_LHPA_UP_SIZE                   (0x00001000UL)  /*4K�ֽڣ�PS���������С*/
#define LPHY_MAILBOX_LHPA_DOWN_SIZE                 (0x00001C00UL)  /*change size of dl mbx from 6K to 7K*/		/*6k�ֽڣ�PS���������С*/
#define LPHY_MAILBOX_OM_UP_SIZE                     (0x00000000UL)  /*0k�ֽڣ�OM���������С*/
#define LPHY_MAILBOX_OM_DOWN_SIZE                   (0x00000800UL)  /*2k�ֽڣ�OM���������С*/
#define LPHY_MAILBOX_LMAC_UP_SIZE                   (0x00000400UL)  /*1k�ֽڣ�MACר�������С*/
#define LPHY_MAILBOX_LMAC_DOWN_SIZE                 (0x00000400UL)  /*1k�ֽڣ�MACר�������С*/
#define LPHY_MAILBOX_CT_UP_SIZE                     (0x00000000UL)  /*0k�ֽڣ�CT���������С*/
#define LPHY_MAILBOX_LPP_DOWN_SIZE                  (0x00000000UL)  /*0k�ֽڣ�LPP ��λ������Ϣ�����С*/
#define LPHY_MAILBOX_TOA_SIZE                       (0x00000000UL)  /*0k�ֽڣ�TOA�����С*/
#define LPHY_NV_CFG_SIZE                            (0x00002000UL)  /*8k�ֽڣ�NV����������С*/
#define TPHY_NV_CFG_SIZE                            (0x00002000UL)  /*8k�ֽڣ�NV����������С*/
#define LPHY_CSU_SDR_BUF_SIZE                       (0x00002800UL)  /*10k�ֽ�,CSU SDR��������С*/
#define TPHY_COMM_DATA_BUF_SIZE                     (0x00008000UL)  /*32k�ֽڣ�TDS COMM BUF��������С*/
#define LPHY_RSVD_DATA_BUF_SIZE                     (0x00000000UL)  /*change size of dl mbx from 1K to 0K*/		/*1k�ֽڣ�TDS COMM BUF��������С*/

#if (defined(CHIP_BB_6920ES) || defined(BOARD_FPGA))
    #define LPHY_MAILBOX_LCS_BANKA_MEM_SIZE         (0x00000000UL)  /*LPHY_HI6920ES���� fpga����������ڴ�*/
#else
    #define LPHY_MAILBOX_LCS_BANKA_MEM_SIZE         (0x00000000UL)  /*24K�ֽ�, ����  pliot�汾lcs����ʹ��*/
#endif


/*�����ܴ�С*/
#define LPHY_TOTAL_RESERVED_MEM_SIZE    ( \
      LPHY_MAILBOX_SHARE_MEM_SIZE \
    + LPHY_MAILBOX_LCS_BANKA_MEM_SIZE \
    + LPHY_MAILBOX_HDLC_BUF_SIZE \
    + LPHY_MAILBOX_LHPA_UP_SIZE   \
    + LPHY_MAILBOX_LHPA_DOWN_SIZE \
    + LPHY_MAILBOX_OM_UP_SIZE    \
    + LPHY_MAILBOX_OM_DOWN_SIZE  \
    + LPHY_MAILBOX_LMAC_UP_SIZE  \
    + LPHY_MAILBOX_LMAC_DOWN_SIZE \
    + LPHY_MAILBOX_CT_UP_SIZE \
    + LPHY_MAILBOX_LPP_DOWN_SIZE \
    + LPHY_MAILBOX_TOA_SIZE \
    + LPHY_NV_CFG_SIZE\
    + TPHY_NV_CFG_SIZE\
    + LPHY_CSU_SDR_BUF_SIZE\
    + TPHY_COMM_DATA_BUF_SIZE\
    + LPHY_RSVD_DATA_BUF_SIZE)

/*�������ַ����*/
#if (defined(TENSILICA_PLATFORM) || defined(MS_VC6_PLATFORM))
    #define LPHY_MAILBOX_BASE_ADDR                  (LPHY_SHARE_MEM_BASE_ADDR + LPHY_DSP_ARM_RAM_BASE_OFFSET)
#else
    #define LPHY_MAILBOX_BASE_ADDR                  (LPHY_SHARE_MEM_BASE_ADDR)
#endif

/*�ⲿϵͳ���ʸ����������ַ*/
#define LPHY_MAILBOX_VERSION_ADDR                   (LPHY_MAILBOX_BASE_ADDR + 0x0200)   /*DSP�汾��ѯ��ַ*/
#define LPHY_MAILBOX_LOAD_MEMORY_ADDR               (LPHY_MAILBOX_BASE_ADDR + 0x020C)   /*������ʼ�����ֵ�ַ*/
#define LPHY_MAILBOX_RFIC_AD9361_VERSIOM_ADDR       (LPHY_MAILBOX_BASE_ADDR + 0x0210)   /*AD9361��ƵоƬ�汾��ѯ��ַ*/
#define LPHY_MAILBOX_IPCM_MUTEX_DSP_SEND            (LPHY_MAILBOX_BASE_ADDR + 0x0324)   /*P500��A9IPCͨѶ��������ַ*/
#define LPHY_MAILBOX_IPCM_MUTEX_DSP_RECEIVE         (LPHY_MAILBOX_BASE_ADDR + 0x0328)   /*P500��A9IPCͨѶ��������ַ*/
#define LPHY_MAILBOX_SLAVE_AWAKE_ADDR               (LPHY_MAILBOX_BASE_ADDR + 0x032C)   /*��ģPS IPCM����״̬��ַ*/
#define LPHY_MAILBOX_SLAVE_AWAKEMSP_ADDR            (LPHY_MAILBOX_BASE_ADDR + 0x0330)   /*��ģMSP IPCM����״̬��ַ*/

#define LPHY_MAILBOX_HDLC_BUF_BASE_ADDR             (LPHY_MAILBOX_BASE_ADDR             + LPHY_MAILBOX_SHARE_MEM_SIZE)
#define LPHY_MAILBOX_LCS_BANKA_BASE_ADDR            (LPHY_MAILBOX_HDLC_BUF_BASE_ADDR    + LPHY_MAILBOX_HDLC_BUF_SIZE)
#define LPHY_MAILBOX_LHPA_UP_BASE_ADDR              (LPHY_MAILBOX_LCS_BANKA_BASE_ADDR   + LPHY_MAILBOX_LCS_BANKA_MEM_SIZE)
#define LPHY_MAILBOX_LHPA_DOWN_BASE_ADDR            (LPHY_MAILBOX_LHPA_UP_BASE_ADDR     + LPHY_MAILBOX_LHPA_UP_SIZE)
#define LPHY_MAILBOX_OM_UP_BASE_ADDR                (LPHY_MAILBOX_LHPA_DOWN_BASE_ADDR   + LPHY_MAILBOX_LHPA_DOWN_SIZE)
#define LPHY_MAILBOX_OM_DOWN_BASE_ADDR              (LPHY_MAILBOX_OM_UP_BASE_ADDR       + LPHY_MAILBOX_OM_UP_SIZE)
#define LPHY_MAILBOX_LMAC_UP_BASE_ADDR              (LPHY_MAILBOX_OM_DOWN_BASE_ADDR     + LPHY_MAILBOX_OM_DOWN_SIZE)
#define LPHY_MAILBOX_LMAC_DOWN_BASE_ADDR            (LPHY_MAILBOX_LMAC_UP_BASE_ADDR     + LPHY_MAILBOX_LMAC_UP_SIZE)
#define LPHY_MAILBOX_CT_UP_MAIL_BASE_ADDR           (LPHY_MAILBOX_LMAC_DOWN_BASE_ADDR   + LPHY_MAILBOX_LMAC_DOWN_SIZE)
#define LPHY_MAILBOX_LPP_DOWN_BASE_ADDR             (LPHY_MAILBOX_CT_UP_MAIL_BASE_ADDR  + LPHY_MAILBOX_CT_UP_SIZE)
#define LPHY_MAILBOX_TOA_BASE_ADDR                  (LPHY_MAILBOX_LPP_DOWN_BASE_ADDR    + LPHY_MAILBOX_LPP_DOWN_SIZE)
#define LPHY_MAILBOX_NV_CFG_BASE_ADDR               (LPHY_MAILBOX_TOA_BASE_ADDR         + LPHY_MAILBOX_TOA_SIZE)
#define TPHY_MAILBOX_NV_CFG_BASE_ADDR               (LPHY_MAILBOX_NV_CFG_BASE_ADDR      + LPHY_NV_CFG_SIZE)
#define LPHY_CSU_SDR_BUF_ADDR                       (TPHY_MAILBOX_NV_CFG_BASE_ADDR      + TPHY_NV_CFG_SIZE) 
#define TPHY_COMM_DATA_BUF_ADDR                     (LPHY_CSU_SDR_BUF_ADDR              + LPHY_CSU_SDR_BUF_SIZE)
#define LPHY_RSVD_DATA_BUF_ADDR                     (TPHY_COMM_DATA_BUF_ADDR            + TPHY_COMM_DATA_BUF_SIZE)  /*1k�ֽڣ�TDS COMM BUF��������С*/


/* CSUʹ��LCS�ڴ�  V9R1_CSU */
#define GLOBAL_MEM_CSU_SDR_DDR_BASE		        (GLOBAL_MEM_LCS_ADDR)
#define GLOBAL_MEM_CSU_SDR_DDR_END		        (GLOBAL_MEM_LCS_ADDR+GLOBAL_MEM_LCS_SIZE)
/* CSUʹ��LCS�ڴ�  V9R1_CSU end */



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MEM_MAP_BASE_H__ */
