/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_modem_bsp.c
  �� �� ��   : ����
  ��    ��   : c61362
  ��������   : 2012��3��2��
  ����޸�   :
  ��������   : ������ϲ������װ�Ľӿڲ�
  �޸���ʷ   :
  1.��    ��   : 2012��3��2��
    ��    ��   : c61362
    �޸�����   : �½�Drvinterface.c

  2.��    ��   : 2013��2��19��
    ��    ��   : ��ϲ 220237
    �޸�����   : ��Drvinterface.c�������

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "BSP.h"
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#include <vxWorks.h>
#include <private/vmLibP.h>
#include <cacheLib.h>
#include "arm_pbxa9.h"
#include <cacheLib.h>
#include "BSP_TIMER.h"
#include "sys_config.h"
#include "soc_baseaddr_interface.h"
#include "string.h"
#include "stdio.h"
#include "BSP_LED.h"
#include "BSP_EMMC.h"
#include "drv_mailbox_cfg.h"
#endif

#if (defined BSP_CORE_APP)
#include "soc_irqs.h"
#include <linux/module.h>
#include "MemoryMap.h"
#include <linux/dma-mapping.h>
#endif

#include "soc_baseaddr_interface.h"
#include "soc_timer_interface.h"
#include "soc_sctrl_interface.h"
#include <soc_ao_sctrl_interface.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SOC_INVALID_BASE_ADDR 0

/* IP NUMBER DESC INFO */
typedef struct BSP_IPADDR_INFO_S
{
    BSP_IP_TYPE_E         ip_type;
    BSP_U32               ip_addr;
}BSP_IPADDR_INFO;

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
/* MEMORY ADDR INFO */
typedef struct BSP_MEMADDR_INFO_S
{
    BSP_DDR_TYPE_E         mem_type;
    BSP_U32                mem_base;
    BSP_U32                mem_size;
}BSP_MEMADDR_INFO;
#endif

/* INTERPUTER NUMBER DESC INFO */
typedef struct BSP_INT_NUM_INFO_S
{
    BSP_INT_TYPE_E         int_type;
    BSP_S32                int_num;
}BSP_INT_NUM_INFO;

/*****************************************************************************
  2 ��������
*****************************************************************************/

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)

/* ȫ�ֱ�������¼IP����ַ */
BSP_IPADDR_INFO g_ulIPBaseAddr[] = {
                {BSP_IP_TYPE_SOCP,      SOCP_REG_BASEADDR},
                {BSP_IP_TYPE_CICOM0,    CICOM0_REGBASE_ADDR},
                {BSP_IP_TYPE_CICOM1,    CICOM1_REGBASE_ADDR},
                {BSP_IP_TYPE_BBPMASTER, BBP_MASTER_REGBASE_ADDR},
                {BSP_IP_TYPE_AHB,       ZSP_UP_ADDR},
                {BSP_IP_TYPE_WBBP,      WBBP_BASE_ADDR},
                {BSP_IP_TYPE_WBBP_DRX,  WBBP_DRX_ADDR},
                {BSP_IP_TYPE_GBBP,      GBBP_BASE_ADDR},
                {BSP_IP_TYPE_GBBP_DRX,  GBBP_DRX_ADDR},
                {BSP_IP_TYPE_GBBP1,     GBBP1_BASE_ADDR},
                {BSP_IP_TYPE_GBBP1_DRX, GBBP1_DRX_ADDR},
                {BSP_IP_TYPE_SYSCTRL,   INTEGRATOR_SC_BASE},
                {BSP_IP_TYPE_CTU,       CTU_BASE_ADDR},
                {BSP_IP_TYPE_TDSSYS,       TDSSYS_BASE_ADDR},
                {BSP_IP_TYPE_BUTTOM,    SOC_INVALID_BASE_ADDR}
};

BSP_MEMADDR_INFO g_stMemInfo[] = {
                {BSP_DDR_TYPE_DDR_GU,               GLOBAL_MEM_GU_RESERVED_ADDR,            GLOBAL_MEM_GU_RESERVED_SIZE},
                {BSP_DDR_TYPE_DDR_TLPHY_IMAGE,      GLOBAL_MEM_LT_IMAGE_ADDR,               GLOBAL_MEM_LT_IMAGE_SIZE},
                /*{BSP_DDR_TYPE_DDR_LPHY_SDR,       NO_USED,                                NO_USED},*/
                {BSP_DDR_TYPE_DDR_TLPHY_LCS,        GLOBAL_MEM_LCS_ADDR,                    GLOBAL_MEM_LCS_SIZE},
                {BSP_DDR_TYPE_DDR_TLPHY_BANDNV,     CORESHARE_MEM_TENCILICA_MULT_BAND_ADDR, CORESHARE_MEM_TENCILICA_MULT_BAND_SIZE},
                {BSP_DDR_TYPE_DDR_TPHY_TABLE,       GLOBAL_MEM_TDS_TABLE_ADDR,              GLOBAL_MEM_TDS_TABLE_SIZE},
                {BSP_DDR_TYPE_DDR_HIFI,             GLOBAL_MEM_HIFI_BASE_ADDR,              GLOBAL_MEM_HIFI_SIZE},
                {BSP_DDR_TYPE_SRAM_TLPHY,           MEMORY_AXI_TDL_ADDR,                    MEMORY_AXI_TDL_SIZE},
                {BSP_DDR_TYPE_SRAM_RTT_SLEEP_FLAG,  SRAM_RTT_SLEEP_FLAG_ADDR,               SRAM_RTT_SLEEP_FLAG_SIZE},
                {BSP_DDR_TYPE_SHM_TEMPERATURE,      MCU_HKADC_DDR_ADDR,                     MCU_HKADC_DDR_SIZE},
                {BSP_DDR_TYPE_SHM_BBE16_HIFI_HEAD,  MAILBOX_HEAD_ADDR(BBE16, HIFI, MSG),    MAILBOX_HEAD_LEN},
                {BSP_DDR_TYPE_SHM_HIFI_BBE16_HEAD,  MAILBOX_HEAD_ADDR(HIFI, BBE16, MSG),    MAILBOX_HEAD_LEN},
                {BSP_DDR_TYPE_SHM_BBE16_HIFI_QUEUE, MAILBOX_QUEUE_ADDR(BBE16, HIFI, MSG),   MAILBOX_QUEUE_SIZE(BBE16, HIFI, MSG)},
                {BSP_DDR_TYPE_SHM_HIFI_BBE16_QUEUE, MAILBOX_QUEUE_ADDR(HIFI, BBE16, MSG),   MAILBOX_QUEUE_SIZE(HIFI, BBE16, MSG)}
                /*{BSP_DDR_TYPE_SHM_TIMESTAMP,      NO_USED,                                NO_USED}*/
};

/* ȫ�ֱ�������¼�жϺ� */
BSP_INT_NUM_INFO g_slIntNO[] = {
    {BSP_INT_TYPE_WDT,                      INT_VEC_WDT},
    {BSP_INT_TYPE_IPF_C,                    INT_VEC_IPF_C},
    {BSP_INT_TYPE_IPF_A,                    INT_VEC_IPF_A},
    {BSP_INT_TYPE_SOCP_A,                   INT_VEC_SOCP_A},
    {BSP_INT_TYPE_SOCP_C,                   INT_VEC_SOCP_C},
    {BSP_INT_TYPE_WBBP_BBPMST,              INT_VEC_WBBP_BBPMST_INTR},
    {BSP_INT_TYPE_GBBP,                     INT_VEC_GBBP_GSM1_NODRX_INTR},
    {BSP_INT_TYPE_GBBP_AWAKE,               INT_VEC_GBBP_GSM1_DRX_INTR},
    {BSP_INT_TYPE_WBBP_0MS,                 INT_VEC_WBBP_CARD1_0MS_INTR},
    {BSP_INT_TYPE_WBBP1_0MS,                INT_VEC_WBBP_CARD2_0MS_INTR},
    {BSP_INT_TYPE_WBBP_AWAKE,               INT_VEC_WDRX_SLP_AWK_SW_INTR},
    {BSP_INT_TYPE_WBBP_SWITCH,              INT_VEC_WBBP_CLK_SW_INTR},
    {BSP_INT_TYPE_UPACC_INTR,               INT_VEC_UPACC_ARM_INT},
    {BSP_INT_TYPE_UPACC_DSP,                INT_VEC_UPACC_DSP_INT},
    {BSP_INT_TYPE_CICOM_UL,                 INT_VEC_CICOM_UL},
    {BSP_INT_TYPE_CICOM_DL,                 INT_VEC_CICOM_DL},
    {BSP_INT_TYPE_CICOM1_UL,                INT_VEC_CICOM1_UL},
    {BSP_INT_TYPE_CICOM1_DL,                INT_VEC_CICOM1_DL},
    {BSP_INT_TYPE_GBBP_DSP,                 INT_VEC_GBBP_GSM1_RESERVED_INTR},
    {BSP_INT_TYPE_GBBP_GSML_RESERVED_INTR,  INT_VEC_GBBP_GSM1_DRX_WAKEUP_INTR},
    {BSP_INT_TYPE_GBBP_AWAKE_DSP,           INT_VEC_GBBP_GSM1_DRX_WAKEUP_RESERV},
    {BSP_INT_TYPE_GBBP1,                    INT_VEC_GBBP_GSM2_NODRX},
    {BSP_INT_TYPE_GBBP1_AWAKE,              INT_VEC_GBBP_GSM2_DRX_WAKEUP},
    {BSP_INT_TYPE_GBBP1_DSP,                INT_VEC_GBBP_GSM2_NODRX_RESERV},
    {BSP_INT_TYPE_GBBP1_AWAKE_DSP,          INT_VEC_GBBP_GSM2_DRX_WAKEUP_RESERV},
    {BSP_INT_TYPE_WBBP_SEARCH,              INT_VEC_WBBP_SEARCH_INTR},
    {BSP_INT_TYPE_WBBP_RAKE,                INT_VEC_WBBP_RAKE_INTR},
    {BSP_INT_TYPE_WBBP_DECODE,              INT_VEC_WBBP_DECODE_INTR},
    {BSP_INT_TYPE_WBBP_TIME,                INT_VEC_WBBP_CARD1_TIME_INTR},
    {BSP_INT_TYPE_WBBP1_TIME,               INT_VEC_WBBP_CARD2_TIME_INTR},
    {BSP_INT_TYPE_WBBP_MULTI_SEARCH,        INT_VEC_WBBP_MULTI_SEARCH_INTR},
    {BSP_INT_TYPE_CTU_INT_G,                INT_VEC_GBBP_CTU1_TO_ARM_INT},
    {BSP_INT_TYPE_CTU_INT_W,                INT_VEC_BBP_COMMON_CTU_W2ARM_INT},
    {BSP_INT_TYPE_INT_OSRTC,                INT_VEC_RTC},
    {BSP_INT_TYPE_INT_SMIM,                 INT_VEC_SMIM},
    {BSP_INT_TYPE_LTE_CIPHER,               INT_VEC_L_CIPHER_INTR},
    {BSP_INT_TYPE_LTE_APP_ARM_PUB,          INT_VEC_LTE_APP_ARM_PUB_INT},
    {BSP_INT_TYPE_LTE_ARM_POSITIONS,        INT_VEC_LTE_ARM_POSITION_INTS},
    {BSP_INT_TYPE_LTE_DL_DMA,               INT_VEC_LTE_DL_DMA_INTS},
    {BSP_INT_TYPE_LTE_ARM_POSITION,         INT_VEC_LTE_ARM_POSITION_INT},
    {BSP_INT_TYPE_LTE_ARM_DL_DMA,           INT_VEC_LTE_ARM_DL_DMA_INT},
    {BSP_INT_TYPE_LTE_ARM_CLK_SWITCH,       INT_VEC_LTE_ARM_CLK_SWITCH_INT},
    {BSP_INT_TYPE_LBBP_AWAKE,               INT_VEC_LTE_ARM_WAKEUP_INT},
    {BSP_INT_TYPE_LTE_PUB,                  INT_VEC_LTE_PUB_INT},
    {BSP_INT_TYPE_TDS_STU_ARM1,             INT_VEC_TDS_STU_ARM1_INT},
    {BSP_INT_TYPE_TBBP_AWAKE,               INT_VEC_TDS_DRX_ARM_INT1},
    {BSP_INT_TYPE_TDS_DRX_ARM2,             INT_VEC_TDS_DRX_ARM_INT2},
    {BSP_INT_TYPE_BBE16_GPIO1,              INT_VEC_BBE16_GPIO1_INT},
    {BSP_INT_TYPE_BUTTOM,                   0xFFFFFFFF}
};
#endif

#if (defined BSP_CORE_APP)

/* ȫ�ֱ�������¼IP����ַ */
BSP_IPADDR_INFO g_ulIPBaseAddr[] = {
    {BSP_IP_TYPE_SOCP,      SOC_SOCP_BASE_ADDR},
    {BSP_IP_TYPE_CICOM0,    SOC_GUCipher0_BASE_ADDR},
    {BSP_IP_TYPE_AHB,       ZSP_UP_ADDR},
    {BSP_IP_TYPE_SYSCTRL,   SOC_SC_ON_BASE_ADDR},
    {BSP_IP_TYPE_BUTTOM,    SOC_INVALID_BASE_ADDR}
};

#define INT_LVL_INVALID (-1)
BSP_INT_NUM_INFO g_slIntNO[] = {
    {BSP_INT_TYPE_IPF_C,        IRQ_IPF0},
    {BSP_INT_TYPE_IPF_A,        IRQ_IPF1},
    {BSP_INT_TYPE_SOCP_A,       IRQ_SOCP0},
    {BSP_INT_TYPE_SOCP_C,       IRQ_SOCP1},
    {BSP_INT_TYPE_CTU_INT_W,    IRQ_TIMER2},
    {BSP_INT_TYPE_CTU_INT_TDS,  IRQ_WD1},
    {BSP_INT_TYPE_BUTTOM,       INT_LVL_INVALID}
};
#endif


#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
extern MMU_LIB_FUNCS mmuLibFuncs;
extern int BSP_DMR_ATAnalyze(void * pstMem,unsigned int uiSize, unsigned int * puiModIdSum);
extern int  max_freeblock_size_get(void);
extern BSP_VOID SendNMIInterrupt(unsigned int SocBitNO, unsigned int ZspBitNO);
extern unsigned int exchIntGetContext(void);
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


/*****************************************************************************
 �� �� ��  : BSP_GetIPBaseAddr
 ��������  : IP����ַ��ѯ
 �������  : enIPType: ��Ҫ��ѯ��IP����
 �������  : ��
 ����ֵ    ����ѯ����IP����ַ����鲻������NULL
*****************************************************************************/
BSP_U32 BSP_GetIPBaseAddr(BSP_IP_TYPE_E enIPType)
{
    int i = 0;
    int NUM_TMP = sizeof(g_ulIPBaseAddr)/sizeof(g_ulIPBaseAddr[0]);

    if(enIPType >= BSP_IP_TYPE_BUTTOM)
    {
        return SOC_INVALID_BASE_ADDR;
    }

    for (i = 0;i < NUM_TMP;i++)
    {

        if (enIPType == g_ulIPBaseAddr[i].ip_type)
        {
            return g_ulIPBaseAddr[i].ip_addr;
        }
    }

    return SOC_INVALID_BASE_ADDR;
}

#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
/*****************************************************************************
 �� �� ��  : BSP_GetMemAddrSize
 ��������  : ��ȡmem��ַ�ʹ�С��Ϣ
 �������  : enMemType: ��Ҫ��ѯ��mem����
 �������  : addr ��ַ
             size ��С
 ����ֵ    ��BSP_OK    ��ѯ�ɹ�
             BSP_ERROR ��ѯʧ��
*****************************************************************************/
BSP_U32 BSP_GetMemAddrSize(BSP_DDR_TYPE_E enDdrType, BSP_U32 *addr, BSP_U32 *size)
{
    BSP_U32 i = 0;
    BSP_U32 ulMemNum = sizeof(g_stMemInfo)/sizeof(g_stMemInfo[0]);

    /*��μ��*/
    if((!addr) || (!size) || (enDdrType >= BSP_DDR_TYPE_BUTTOM))
    {
        return BSP_ERROR;
    }

    for (i = 0;i < ulMemNum;i++)
    {
        if (enDdrType == g_stMemInfo[i].mem_type)
        {
            *addr = g_stMemInfo[i].mem_base;
            *size = g_stMemInfo[i].mem_size;
            return BSP_OK;
        }
    }

    return BSP_ERROR;
}
#endif

/*****************************************************************************
 �� �� ��  : BSP_GetIntNO
 ��������  : �жϺŲ�ѯ
 �������  : enIntType: ��Ҫ��ѯ���ж�����
 �������  : ��
 ����ֵ    ����ѯ�����жϺţ���鲻������BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_GetIntNO(BSP_INT_TYPE_E enIntType)
{
    int i = 0;
    int NUM_TMP = sizeof(g_slIntNO)/sizeof(g_slIntNO[0]);
    for (i = 0;i < NUM_TMP;i++)
    {

        if (enIntType == g_slIntNO[i].int_type)
        {
            return g_slIntNO[i].int_num;
        }
    }

    return BSP_ERROR;
}

/************************************************************************
 * FUNCTION
 *       vmEnable
 * DESCRIPTION
 *       �ڴ��д��������ʹ�ܺ���
 * INPUTS
 *       NONE
 * OUTPUTS
 *       NONE
 *************************************************************************/
int vmEnable(int enable)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return (MMU_ENABLE(enable));
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_DMR_ATANALYZE
 ��������  : ��NAS��ѯ��̬�ڴ�ռ����Ϣ��ͨ��AT�����ѯ
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : �ɹ�0��ʧ��-1
*****************************************************************************/
int BSP_DMR_ATANALYZE(void * pstMem,unsigned int uiSize, unsigned int * puiModIdSum)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
#if( FEATURE_MEM_MONITOR == FEATURE_ON )
    return BSP_DMR_ATAnalyze(pstMem, uiSize, puiModIdSum);
#else
    DRV_PRINTF("Function BSP_DMR_ATANALYZE unsupport!\n");
    return BSP_ERROR;
#endif
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : DRV_GET_FREE_BLOCK_SIZE
 ��������  : get memory max free block size
 �������  : �ޡ�
 �������  : �ޡ�
 �� �� ֵ  : max free block size
*****************************************************************************/
int DRV_GET_FREE_BLOCK_SIZE(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return max_freeblock_size_get();
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_SendNMIInterrupt
 ��������  : ����һ��NMI�ж�
 �������  : NMI�ж�Bitλ
 �������  : ��
 ����ֵ    ����
*****************************************************************************/
BSP_VOID BSP_SendNMIInterrupt(unsigned int SocBitNO, unsigned int ZspBitNO)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    SendNMIInterrupt(SocBitNO, ZspBitNO);
#endif

#if (defined BSP_CORE_APP)
    return;   /* ��׮ */
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_CACHE_DataCacheFlush
 ��������  :
 �������  :
 �������  : None
 �� �� ֵ  : void
*****************************************************************************/
void BSP_CACHE_DATA_FLUSH(void * addr, int size)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    /*BSP_CACHE_DataCacheFlush(addr, size);*/
    cacheFlush(DATA_CACHE,addr, size);
#endif

#if (defined BSP_CORE_APP)
    return;   /* ��׮ */
#endif
}

/*****************************************************************************
 ������: DRV_INT_GET_CONTEXT
 �������� : �жϵ�ǰ�����Ƿ����ж�������
 ������� : ��
 ������� : ��
 ����ֵ   �� 0 ������ж�
           ��1  �����ж�
*****************************************************************************/
unsigned int DRV_INT_GET_CONTEXT(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return exchIntGetContext();
#endif

#if (defined BSP_CORE_APP)
    return 0;   /* ��׮ */
#endif
}

BSP_S32 BSP_NANDF_Erase(BSP_U32 u32BlockID)
{
    return 0;
}

BSP_S32 BSP_NANDF_Read(BSP_VOID* pRamAddr, BSP_U32 u32FlashAddr, BSP_U32 u32NumByte)
{
    return 0;
}

BSP_S32 BSP_NANDF_Write(BSP_U32 u32FlashAddr, BSP_VOID *pRamAddr,  BSP_U32 u32NumByte)
{
    return 0;
}

/*stub,Ϊ����������������룬��׮����������������޸ĺ�ɾ��*/
unsigned long free_mem_size_get(void)
{
#if defined (BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
    return 0;   /* ��׮ */
#endif

#if (defined BSP_CORE_APP)
    return 0;
#endif
}

#if (defined BSP_CORE_APP)
EXPORT_SYMBOL(free_mem_size_get);
#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

