/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mem_remap.c
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
#include "sys_config.h"
#include "string.h"
#endif

#if (defined BSP_CORE_APP)
#include "soc_irqs.h"
#include <linux/module.h>
#include "soc_baseaddr_interface.h"
#include "soc_timer_interface.h"
#include "soc_sctrl_interface.h"
#include "MemoryMap.h"
#include <linux/dma-mapping.h>
#endif

#include <soc_ao_sctrl_interface.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern int printf(const char *format, ...);

/*****************************************************************************
  2 ��������
*****************************************************************************/
#if defined(BSP_CORE_MODEM) || defined(PRODUCT_CFG_CORE_TYPE_MODEM)
/* ȫ�ֵĽṹ�壬��¼DDR�ڴ�η�����Ϣ */
BSP_DDR_SECT_INFO g_stDdrSectInfo[] = {
    {BSP_DDR_SECT_TYPE_TTF,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_TTF_BASE_ADDR ,              ECS_TTF_BASE_ADDR,            ECS_TTF_SIZE},
    {BSP_DDR_SECT_TYPE_ARMDSP,    BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_ARM_DSP_BUFFER_BASE_ADDR,    ECS_ARM_DSP_BUFFER_BASE_ADDR, ECS_ARM_DSP_BUFFER_SIZE},
    {BSP_DDR_SECT_TYPE_UPA,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_UPA_BASE_ADDR,               ECS_UPA_BASE_ADDR,            ECS_UPA_SIZE},
    {BSP_DDR_SECT_TYPE_CQI,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_CQI_BASE_ADDR,               ECS_CQI_BASE_ADDR,            ECS_CQI_SIZE},
    {BSP_DDR_SECT_TYPE_APT,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_APT_BASE_ADDR,               ECS_APT_BASE_ADDR,            ECS_APT_SIZE},
    {BSP_DDR_SECT_TYPE_ET,        BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_ET_BASE_ADDR,                ECS_ET_BASE_ADDR,             ECS_ET_SIZE},
    {BSP_DDR_SECT_TYPE_BBPMASTER, BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_BBP_MASTER_BASE_ADDR,        ECS_BBP_MASTER_BASE_ADDR,     ECS_BBP_MASTER_SIZE},
    {BSP_DDR_SECT_TYPE_NV,        BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(ECS_NV_BASE_ADDR),  ECS_NV_BASE_ADDR,             ECS_NV_SIZE},
    {BSP_DDR_SECT_TYPE_DICC,      BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(ECS_TTF_DICC_ADDR), ECS_TTF_DICC_ADDR,            ECS_TTF_DICC_SIZE},
    {BSP_DDR_SECT_TYPE_WAN,       BSP_DDR_SECT_ATTR_NONCACHEABLE, CORESHARE_MEM_WAN_ADDR,          CORESHARE_MEM_WAN_ADDR,       CORESHARE_MEM_WAN_SIZE},
    {BSP_DDR_SECT_TYPE_SHARE_MEM, BSP_DDR_SECT_ATTR_NONCACHEABLE, GLOBAL_MEM_CORE_SHARE_ADDR,      GLOBAL_MEM_CORE_SHARE_ADDR,   GLOBAL_MEM_CORE_SHARE_SIZE},
    {BSP_DDR_SECT_TYPE_EXCP,      BSP_DDR_SECT_ATTR_NONCACHEABLE, GLOBAL_MEM_EXCP_BASE_ADDR,       GLOBAL_MEM_EXCP_BASE_ADDR,    GLOBAL_MEM_EXCP_SIZE},
    {BSP_DDR_SECT_TYPE_HIFI,      BSP_DDR_SECT_ATTR_NONCACHEABLE, GLOBAL_MEM_HIFI_BASE_ADDR,       GLOBAL_MEM_HIFI_BASE_ADDR,    GLOBAL_MEM_HIFI_SIZE},
    {BSP_DDR_SECT_TYPE_HUTAF,     BSP_DDR_SECT_ATTR_NONCACHEABLE, HUTAF_HLT_MEM_ADDR,              HUTAF_HLT_MEM_ADDR,           HUTAF_HLT_MEM_SIZE},
    {BSP_DDR_SECT_TYPE_ZSP_UP,    BSP_DDR_SECT_ATTR_NONCACHEABLE, ZSP_UP_ADDR,                     ZSP_UP_ADDR,                  ZSP_UP_SIZE},
    {BSP_DDR_SECT_TYPE_TDS_LH2,   BSP_DDR_SECT_ATTR_NONCACHEABLE, GLOBAL_MEM_TDS_TABLE_ADDR,       GLOBAL_MEM_TDS_TABLE_ADDR,    GLOBAL_MEM_TDS_TABLE_SIZE},
    {BSP_DDR_SECT_TYPE_BUTTOM,    0,                              0,                               0,                            0}
};
    
/* ȫ�ֵĽṹ�壬��¼AXI�ڴ�η�����Ϣ */
BSP_AXI_SECT_INFO g_stAxiSectInfo[] = {
    {BSP_AXI_SECT_TYPE_IFC,               MEMORY_AXI_IFC_ADDR,               MEMORY_AXI_IFC_ADDR,               MEMORY_AXI_IFC_SIZE},
    {BSP_AXI_SECT_TYPE_ICC,               MEMORY_AXI_ICC_ADDR,               MEMORY_AXI_ICC_ADDR,               MEMORY_AXI_ICC_SIZE},
    {BSP_AXI_SECT_TYPE_MEMMGR_FLAG,       MEMORY_AXI_MEMMGR_FLAG_ADDR,       MEMORY_AXI_MEMMGR_FLAG_ADDR,       MEMORY_AXI_MEMMGR_FLAG_SIZE},
    {BSP_AXI_SECT_TYPE_DYNAMIC,           0,       0,       0},
    {BSP_AXI_SECT_TYPE_IPF,               MEMORY_AXI_IPF_ADDR,               MEMORY_AXI_IPF_ADDR,               MEMORY_AXI_IPF_SIZE},
    {BSP_AXI_SECT_TYPE_TEMPERATURE,       MEMORY_AXI_TEMP_PROTECT_ADDR,      MEMORY_AXI_TEMP_PROTECT_ADDR,      MEMORY_AXI_TEMP_PROTECT_SIZE},
    {BSP_AXI_SECT_TYPE_TTF_BBP,           MEMORY_AXI_TTF_BBP_ADDR,           MEMORY_AXI_TTF_BBP_ADDR,           MEMORY_AXI_TTF_BBP_SIZE},
    {BSP_AXI_SECT_TYPE_DICC,              MEMORY_AXI_DICC_ADDR,              MEMORY_AXI_DICC_ADDR,              MEMORY_AXI_DICC_SIZE},
    {BSP_AXI_SECT_TYPE_HIFI,              MEMORY_AXI_HIFI_ADDR,              MEMORY_AXI_HIFI_ADDR,              MEMORY_AXI_HIFI_SIZE},
    {BSP_AXI_SECT_TYPE_DDR_CAPACITY,      MEMORY_AXI_DDR_CAPACITY_ADDR,      MEMORY_AXI_DDR_CAPACITY_ADDR,      MEMORY_AXI_DDR_CAPACITY_SIZE},
	{BSP_AXI_SECT_TYPE_RESERVE,           MEMORY_AXI_RESERVE_ADDR,           MEMORY_AXI_RESERVE_ADDR,           MEMORY_AXI_RESERVE_SIZE},
    {BSP_AXI_SECT_TYPE_BUTTOM,            0,                                 0,                                 0}
};
#endif

#if (defined BSP_CORE_APP)
/* ȫ�ֵĽṹ�壬��¼DDR�ڴ�η�����Ϣ */
BSP_DDR_SECT_INFO g_stDdrSectInfo[] = {
    {BSP_DDR_SECT_TYPE_TTF,       BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(ECS_TTF_BASE_ADDR) ,              ECS_TTF_BASE_ADDR,            ECS_TTF_SIZE},
    {BSP_DDR_SECT_TYPE_ARMDSP,    BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_ARM_DSP_BUFFER_BASE_ADDR,    ECS_ARM_DSP_BUFFER_BASE_ADDR, ECS_ARM_DSP_BUFFER_SIZE},
    {BSP_DDR_SECT_TYPE_UPA,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_UPA_BASE_ADDR,               ECS_UPA_BASE_ADDR,            ECS_UPA_SIZE},
    {BSP_DDR_SECT_TYPE_CQI,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_CQI_BASE_ADDR,               ECS_CQI_BASE_ADDR,            ECS_CQI_SIZE},
    {BSP_DDR_SECT_TYPE_APT,       BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_APT_BASE_ADDR,               ECS_APT_BASE_ADDR,            ECS_APT_SIZE},
    {BSP_DDR_SECT_TYPE_ET,        BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_ET_BASE_ADDR,                ECS_ET_BASE_ADDR,             ECS_ET_SIZE},
    {BSP_DDR_SECT_TYPE_BBPMASTER, BSP_DDR_SECT_ATTR_NONCACHEABLE, ECS_BBP_MASTER_BASE_ADDR,        ECS_BBP_MASTER_BASE_ADDR,     ECS_BBP_MASTER_SIZE},
    {BSP_DDR_SECT_TYPE_NV,        BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(ECS_NV_BASE_ADDR),  ECS_NV_BASE_ADDR,             ECS_NV_SIZE},
    {BSP_DDR_SECT_TYPE_DICC,      BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(ECS_TTF_DICC_ADDR), ECS_TTF_DICC_ADDR,            ECS_TTF_DICC_SIZE},
    {BSP_DDR_SECT_TYPE_WAN,       BSP_DDR_SECT_ATTR_NONCACHEABLE, CORESHARE_MEM_WAN_ADDR,          CORESHARE_MEM_WAN_ADDR,       CORESHARE_MEM_WAN_SIZE},
    {BSP_DDR_SECT_TYPE_SHARE_MEM, BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(GLOBAL_MEM_CORE_SHARE_ADDR),      GLOBAL_MEM_CORE_SHARE_ADDR,   GLOBAL_MEM_CORE_SHARE_SIZE},
    {BSP_DDR_SECT_TYPE_EXCP,      BSP_DDR_SECT_ATTR_NONCACHEABLE, VIRT_ADDRESS(GLOBAL_MEM_EXCP_BASE_ADDR),       GLOBAL_MEM_EXCP_BASE_ADDR,    GLOBAL_MEM_EXCP_SIZE},
    {BSP_DDR_SECT_TYPE_HIFI,      BSP_DDR_SECT_ATTR_NONCACHEABLE, GLOBAL_MEM_HIFI_BASE_ADDR,       GLOBAL_MEM_HIFI_BASE_ADDR,    GLOBAL_MEM_HIFI_SIZE},
    {BSP_DDR_SECT_TYPE_TDS_LH2,   BSP_DDR_SECT_ATTR_NONCACHEABLE, 0,                               0,                            0},
    {BSP_DDR_SECT_TYPE_BUTTOM,    0,                              (BSP_U32)NULL,                            (BSP_U32)NULL,                         0}
};

/* ȫ�ֵĽṹ�壬��¼AXI�ڴ�η�����Ϣ */
#define AXI_P2V(A) IO_ADDRESS(A)
BSP_AXI_SECT_INFO g_stAxiSectInfo[] = {
    {BSP_AXI_SECT_TYPE_IFC,               AXI_P2V(MEMORY_AXI_IFC_ADDR),               MEMORY_AXI_IFC_ADDR,               MEMORY_AXI_IFC_SIZE},
    {BSP_AXI_SECT_TYPE_ICC,               AXI_P2V(MEMORY_AXI_ICC_ADDR),               MEMORY_AXI_ICC_ADDR,               MEMORY_AXI_ICC_SIZE},
    {BSP_AXI_SECT_TYPE_MEMMGR_FLAG,       AXI_P2V(MEMORY_AXI_MEMMGR_FLAG_ADDR),       MEMORY_AXI_MEMMGR_FLAG_ADDR,       MEMORY_AXI_MEMMGR_FLAG_SIZE},
    {BSP_AXI_SECT_TYPE_DYNAMIC,           0,       0,       0},
    {BSP_AXI_SECT_TYPE_IPF,               AXI_P2V(MEMORY_AXI_IPF_ADDR),               MEMORY_AXI_IPF_ADDR,               MEMORY_AXI_IPF_SIZE},
    {BSP_AXI_SECT_TYPE_TEMPERATURE,       AXI_P2V(MEMORY_AXI_TEMP_PROTECT_ADDR),      MEMORY_AXI_TEMP_PROTECT_ADDR,      MEMORY_AXI_TEMP_PROTECT_SIZE},
    {BSP_AXI_SECT_TYPE_DICC,              AXI_P2V(MEMORY_AXI_DICC_ADDR),              MEMORY_AXI_DICC_ADDR,              MEMORY_AXI_DICC_SIZE},
    {BSP_AXI_SECT_TYPE_HIFI,              AXI_P2V(MEMORY_AXI_HIFI_ADDR),              MEMORY_AXI_HIFI_ADDR,              MEMORY_AXI_HIFI_SIZE},
    {BSP_AXI_SECT_TYPE_DDR_CAPACITY,      AXI_P2V(MEMORY_AXI_DDR_CAPACITY_ADDR),      MEMORY_AXI_DDR_CAPACITY_ADDR,      MEMORY_AXI_DDR_CAPACITY_SIZE},
    {BSP_AXI_SECT_TYPE_RESERVE,           AXI_P2V(MEMORY_AXI_RESERVE_ADDR),           MEMORY_AXI_RESERVE_ADDR,           MEMORY_AXI_RESERVE_SIZE},
    {BSP_AXI_SECT_TYPE_TTF_BBP,           AXI_P2V(MEMORY_AXI_TTF_BBP_ADDR),           MEMORY_AXI_TTF_BBP_ADDR,           MEMORY_AXI_TTF_BBP_SIZE},
    {BSP_AXI_SECT_TYPE_BUTTOM,            (BSP_U32)NULL,                                       (BSP_U32)NULL,                              0}
};
#endif


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : BSP_DDR_GetSectInfo
 ��������  : DDR�ڴ�β�ѯ�ӿ�
 �������  : pstSectQuery: ��Ҫ��ѯ���ڴ�����͡�����
 �������  : pstSectInfo:  ��ѯ�����ڴ����Ϣ
 ����ֵ    ��BSP_OK/BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_DDR_GetSectInfo(BSP_DDR_SECT_QUERY *pstSectQuery, BSP_DDR_SECT_INFO *pstSectInfo)
{
    int i = 0;
    int NUM_TMP = sizeof(g_stDdrSectInfo)/sizeof(g_stDdrSectInfo[0]);
    if((BSP_NULL == pstSectQuery) || (BSP_NULL == pstSectInfo))
    {
        return BSP_ERROR;
    }

    for (i = 0;i < NUM_TMP;i++)
    {

        if (pstSectQuery->enSectType == g_stDdrSectInfo[i].enSectType)
        {
            memcpy((void *)pstSectInfo, (const void *)(&g_stDdrSectInfo[i]), sizeof(BSP_DDR_SECT_INFO));
            return BSP_OK;
        }
    }

    return BSP_ERROR;
}

/*****************************************************************************
 �� �� ��  : BSP_DDR_ShowSectInfo
 ��������  : ��ӡDDR�ڴ����Ϣ
 �������  : ��
 �������  : ��
 ����ֵ    ����
*****************************************************************************/
BSP_VOID BSP_DDR_ShowSectInfo(BSP_VOID)
{   
    int enSectTypeIndex = 0;

    DRV_PRINTF("\ntype       paddr      vaddr      size       attr\n");
    for(; enSectTypeIndex < (sizeof(g_stDdrSectInfo) / sizeof(BSP_DDR_SECT_INFO)); enSectTypeIndex++)
    {
        DRV_PRINTF("0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x\n\n", \
          g_stDdrSectInfo[enSectTypeIndex].enSectType, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectPhysAddr, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectVirtAddr, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectSize, \
          g_stDdrSectInfo[enSectTypeIndex].enSectAttr);
    }
    
#if 0    
    BSP_DDR_SECT_TYPE_E     enSectTypeIndex = (BSP_DDR_SECT_TYPE_E)0;

    DRV_PRINTF("\ntype       paddr      vaddr      size       attr\n");
    for(; enSectTypeIndex < BSP_DDR_SECT_TYPE_BUTTOM; enSectTypeIndex++)
    {
        DRV_PRINTF("0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x\n\n", \
          g_stDdrSectInfo[enSectTypeIndex].enSectType, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectPhysAddr, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectVirtAddr, \
          g_stDdrSectInfo[enSectTypeIndex].ulSectSize, \
          g_stDdrSectInfo[enSectTypeIndex].enSectAttr);
    }
#endif
}

/*****************************************************************************
 �� �� ��  : BSP_AXI_GetSectInfo
 ��������  : AXI�ڴ�β�ѯ�ӿ�
 �������  : enSectType: ��Ҫ��ѯ���ڴ������
 �������  : pstSectInfo:  ��ѯ�����ڴ����Ϣ
 ����ֵ    ��BSP_OK/BSP_ERROR
*****************************************************************************/
BSP_S32 BSP_AXI_GetSectInfo(BSP_AXI_SECT_TYPE_E enSectType, BSP_AXI_SECT_INFO *pstSectInfo)
{
    int i = 0;
    int NUM_TMP = sizeof(g_stAxiSectInfo)/sizeof(g_stAxiSectInfo[0]);

    if(BSP_NULL == pstSectInfo)
    {
        return BSP_ERROR;
    }

    for (i = 0;i < NUM_TMP;i++)
    {
        if (enSectType == g_stAxiSectInfo[i].enSectType)
        {
            memcpy((void *)pstSectInfo, (const void *)(&g_stAxiSectInfo[i]), sizeof(BSP_AXI_SECT_INFO));
            return BSP_OK;
        }
    }

    return BSP_ERROR;
}

/*****************************************************************************
 �� �� ��  : BSP_AXI_ShowSectInfo
 ��������  : ��ӡAXI�ڴ����Ϣ
 �������  : ��
 �������  : ��
 ����ֵ    ����
*****************************************************************************/
BSP_VOID BSP_AXI_ShowSectInfo(BSP_VOID)
{       
    int enSectTypeIndex = 0;

    DRV_PRINTF("\ntype       paddr      vaddr      sizer\n");
    for(; enSectTypeIndex < (sizeof(g_stAxiSectInfo) / sizeof(BSP_AXI_SECT_INFO)); enSectTypeIndex++)
    {
        DRV_PRINTF("0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x\n\n", \
          g_stAxiSectInfo[enSectTypeIndex].enSectType, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectPhysAddr, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectVirtAddr, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectSize);
    }
    
#if 0   
    BSP_AXI_SECT_TYPE_E     enSectTypeIndex = 0;

    DRV_PRINTF("\ntype       paddr      vaddr      sizer\n");
    for(; enSectTypeIndex < BSP_AXI_SECT_TYPE_BUTTOM; enSectTypeIndex++)
    {
        DRV_PRINTF("0x%-8.8x 0x%-8.8x 0x%-8.8x 0x%-8.8x\n\n", \
          g_stAxiSectInfo[enSectTypeIndex].enSectType, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectPhysAddr, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectVirtAddr, \
          g_stAxiSectInfo[enSectTypeIndex].ulSectSize);
    }
#endif
}

/*************************�ڴ���ʵת�� start*********************************/
/*****************************************************************************
 �� �� ��  : DRV_DDR_VIRT_TO_PHY
 ��������  : DDR�ڴ����ַ��ʵ��ַת��
 �������  : ulVAddr�����ַ
 �������  : ��
 ����ֵ    ��ʵ��ַ
*****************************************************************************/
unsigned int DRV_DDR_VIRT_TO_PHY(unsigned int ulVAddr)
{
    if((ulVAddr < ECS_MDDR_RESERVE_BASE_ADDR_VIRT)
        || (ulVAddr > ECS_MDDR_RESERVE_BASE_ADDR_VIRT + GLOBAL_MEM_GU_RESERVED_SIZE
        + GLOBAL_MEM_CORE_SHARE_SIZE + GLOBAL_MEM_EXCP_SIZE))
    {
        DRV_PRINTF("DRV_DDR_VIRT_TO_PHY: ulVAddr is invalid!\n");
        return 0;
    }

    return (ulVAddr - ECS_MDDR_RESERVE_BASE_ADDR_VIRT + ECS_MDDR_RESERVE_BASE_ADDR);
}

/*****************************************************************************
 �� �� ��  : DRV_DDR_PHY_TO_VIRT
 ��������  : DDR�ڴ����ַ��ʵ��ַת��
 �������  : ulPAddr��ʵ��ַ
 �������  : ��
 ����ֵ    �����ַ
*****************************************************************************/
unsigned int DRV_DDR_PHY_TO_VIRT(unsigned int ulPAddr)
{
    if((ulPAddr < ECS_MDDR_RESERVE_BASE_ADDR)
        || (ulPAddr > ECS_MDDR_RESERVE_BASE_ADDR + GLOBAL_MEM_GU_RESERVED_SIZE
        + GLOBAL_MEM_CORE_SHARE_SIZE + GLOBAL_MEM_EXCP_SIZE + LPS_RESERVED_MEM_SIZE))
    {
        DRV_PRINTF("DRV_DDR_PHY_TO_VIRT: ulVAddr(0x%08x) is invalid!\n",ulPAddr);
        return 0;
    }

    return (ulPAddr - ECS_MDDR_RESERVE_BASE_ADDR + ECS_MDDR_RESERVE_BASE_ADDR_VIRT);
}

/*****************************************************************************
 �� �� ��  : TTF_VIRT_TO_PHY
 ��������  : TTF�ڴ����ַ��ʵ��ַת��
 �������  : ulVAddr�����ַ
 �������  : ��
 ����ֵ    ��ʵ��ַ
*****************************************************************************/
unsigned int TTF_VIRT_TO_PHY(unsigned int ulVAddr)
{
    return   (ulVAddr - ECS_TTF_BASE_ADDR_VIRT + ECS_TTF_BASE_ADDR);
}

/*****************************************************************************
 �� �� ��  : TTF_PHY_TO_VIRT
 ��������  : TTF�ڴ�ʵ��ַ�����ַת��
 �������  : ulPAddr��ʵ��ַ
 �������  : ��
 ����ֵ    �����ַ
*****************************************************************************/
unsigned int TTF_PHY_TO_VIRT(unsigned int ulPAddr)
{
    return  (ulPAddr - ECS_TTF_BASE_ADDR + ECS_TTF_BASE_ADDR_VIRT);
}

/*****************************************************************************
 �� �� ��  : IPF_VIRT_TO_PHY
 ��������  : IPF�Ĵ������ַ��ʵ��ַת��
 �������  : ulVAddr�����ַ
 �������  : ��
 ����ֵ    ��ʵ��ַ
*****************************************************************************/
unsigned int IPF_VIRT_TO_PHY(unsigned int ulVAddr)
{
    return (ulVAddr - IPF_BASE_ADDR_VIRT + IPF_PHY_BASE_ADDR);
}

/*****************************************************************************
 �� �� ��  : IPF_PHY_TO_VIRT
 ��������  : IPF�Ĵ���ʵ��ַ�����ַת��
 �������  : ulPAddr��ʵ��ַ
 �������  : ��
 ����ֵ    �����ַ
*****************************************************************************/
unsigned int IPF_PHY_TO_VIRT(unsigned int ulPAddr)
{
    return (ulPAddr - IPF_PHY_BASE_ADDR + IPF_BASE_ADDR_VIRT);
}

/*****************************************************************************
 �� �� ��  : DRV_AXI_VIRT_TO_PHY
 ��������  : AXI�����ַ��ʵ��ַת��
 �������  : ulVAddr�����ַ
 �������  : ��
 ����ֵ    �����ַ
*****************************************************************************/
unsigned int DRV_AXI_VIRT_TO_PHY(unsigned int ulVAddr)
{
    return (ulVAddr - AXI_BASE_ADDR_VIRT + AXI_PHY_BASE_ADDR);
}

/*****************************************************************************
 �� �� ��  : DRV_AXI_PHY_TO_VIRT
 ��������  : AXI��ʵ��ַ�����ַת��
 �������  : ulVAddr��ʵ��ַ
 �������  : ��
 ����ֵ    �����ַ
*****************************************************************************/
unsigned int DRV_AXI_PHY_TO_VIRT(unsigned int ulPAddr)
{
    return (ulPAddr - AXI_PHY_BASE_ADDR + AXI_BASE_ADDR_VIRT);
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

