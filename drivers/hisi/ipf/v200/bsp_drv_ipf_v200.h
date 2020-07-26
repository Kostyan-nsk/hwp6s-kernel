

#ifndef    _BSP_DRV_IPF_H_
#define    _BSP_DRV_IPF_H_

#ifdef __cplusplus
extern "C" 
{ 
#endif /* __cplusplus */

/**************************************************************************
  ͷ�ļ�����                            
**************************************************************************/
#include "product_config.h"
#if (FEATURE_IPF_VERSION == IPF_V200)
#ifdef __VXWORKS__
#include <vxWorks.h>
#include <lstLib.h>
#include "BSP.h"
#include "BSP_GLOBAL.h"
#else
#include "BSP.h"
#include <mach/hardware.h>
#include "MemoryLayout.h"
#include "soc_irqs.h"
#endif
#include "BSP_IPF.h"
#include "soc_baseaddr_interface.h"
#include "soc_ipf_interface.h"
/**************************************************************************
  �궨�� 
**************************************************************************/
#define __BSP_IPF_DEBUG__

#ifdef _DRV_LLT_
#define __STATIC__      
#define __INLINE__ 
#define IPF_AXI_MEM_ADDR                ((BSP_U32)g_ipfInfoStub)         
#else
#define __STATIC__      static
#define __INLINE__      inline
#define IPF_AXI_MEM_ADDR                MEMORY_AXI_IPF_ADDR
#endif
#define IPF_AXI_MEM_SIZE                MEMORY_AXI_IPF_SIZE

/* IPF��ַ������Ϣ */
#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
#define IPF_ULBD_MEM_ADDR               IPF_AXI_MEM_ADDR
#else
#define IPF_ULBD_MEM_ADDR               (IO_ADDRESS(IPF_AXI_MEM_ADDR))
#endif
#define IPF_ULBD_MEM_SIZE               (IPF_ULBD_DESC_SIZE * sizeof(IPF_BD_DESC_S))

#define IPF_ULRD_MEM_ADDR               (IPF_ULBD_MEM_ADDR + IPF_ULBD_MEM_SIZE)
#define IPF_ULRD_MEM_SIZE               (IPF_ULRD_DESC_SIZE * sizeof(IPF_RD_DESC_S))

#define IPF_ULAD0_MEM_ADDR              (IPF_ULRD_MEM_ADDR + IPF_ULRD_MEM_SIZE)
#define IPF_ULAD0_MEM_SIZE              (IPF_ULAD0_DESC_SIZE * sizeof(IPF_AD_DESC_S))

#define IPF_ULAD1_MEM_ADDR              (IPF_ULAD0_MEM_ADDR + IPF_ULAD0_MEM_SIZE)
#define IPF_ULAD1_MEM_SIZE              (IPF_ULAD1_DESC_SIZE * sizeof(IPF_AD_DESC_S))

#define IPF_DLBD_MEM_ADDR               (IPF_ULAD1_MEM_ADDR + IPF_ULAD1_MEM_SIZE)
#define IPF_DLBD_MEM_SIZE               (IPF_DLBD_DESC_SIZE * sizeof(IPF_BD_DESC_S))

#define IPF_DLRD_MEM_ADDR               (IPF_DLBD_MEM_ADDR + IPF_DLBD_MEM_SIZE)
#define IPF_DLRD_MEM_SIZE               (IPF_DLRD_DESC_SIZE * sizeof(IPF_RD_DESC_S))

#define IPF_DLAD0_MEM_ADDR              (IPF_DLRD_MEM_ADDR + IPF_DLRD_MEM_SIZE)
#define IPF_DLAD0_MEM_SIZE              (IPF_DLAD0_DESC_SIZE * sizeof(IPF_AD_DESC_S))

#define IPF_DLAD1_MEM_ADDR              (IPF_DLAD0_MEM_ADDR + IPF_DLAD0_MEM_SIZE)
#define IPF_DLAD1_MEM_SIZE              (IPF_DLAD1_DESC_SIZE * sizeof(IPF_AD_DESC_S))

#define IPF_DLCD_MEM_ADDR               (IPF_DLAD1_MEM_ADDR  + IPF_DLAD1_MEM_SIZE)
#define IPF_DLCD_MEM_SIZE               (IPF_DLCD_DESC_SIZE * sizeof(IPF_CD_DESC_S))

#define IPF_INIT_ADDR                   (IPF_DLCD_MEM_ADDR + IPF_DLCD_MEM_SIZE)
#define IPF_INIT_SIZE                   16

#define IPF_ULBD_IDLENUM_ADDR           (IPF_INIT_ADDR + IPF_INIT_SIZE)
#define IPF_ULBD_IDLENUM_SIZE           16

#define IPF_DEBUG_INFO_ADDR             (IPF_ULBD_IDLENUM_ADDR + IPF_ULBD_IDLENUM_SIZE)
#define IPF_DEBUG_INFO_SIZE             sizeof(IPF_DEBUG_INFO_S)

#define IPF_DEBUG_DLCD_ADDR             (IPF_DEBUG_INFO_ADDR + IPF_DEBUG_INFO_SIZE)
#define IPF_DEBUG_DLCD_SIZE             (IPF_DLCD_DESC_SIZE * sizeof(IPF_CD_DESC_S))

/* ��������CD��дָ�� */
#define IPF_DLCD_PTR_ADDR               (IPF_DEBUG_DLCD_ADDR + IPF_DEBUG_DLCD_SIZE)
#define IPF_DLCD_PTR_SIZE               8

#define IPF_MEM_USED_SIZE               (IPF_DLCD_PTR_ADDR + IPF_DLCD_PTR_SIZE - IPF_ULBD_MEM_ADDR)

/* IPF����������Ϣ */
#define IPF_REG_NUM                         (172)         /* IPF�Ĵ������� */
#define IPF_BF_NUM                          FEATURE_IPF_BF_NUM          /* ������������Ŀ */
#define IPF_TOTAL_FILTER_NUM                (256)         /* ����������Ŀ��ʹ��һ�� */
#define IPF_EXFLITER_NUM                    (IPF_TOTAL_FILTER_NUM-IPF_BF_NUM) /* ��չ��������Ŀ */
#define IPF_TAIL_INDEX                      (511)         /* ��ʶ���һ��filter */
#define IPF_FILTERCHAIN_HEAD_INDEX          (0)           /* ����������ͷ��������λ�� */
#define IPF_FILTERTYPE_MASK                 (0xFFFF)      /* ���ֹ������ǻ���������չ�˲��� */
#define IPF_TIME_OUT_CFG                    (65)          /* RD��ʱʱ��0.1msʱ��Ƶ��166MHZ�� */
#define IPF_ULADQ_PLEN_TH                   (404)         /* ���а�����ֵ */
#define IPF_DLADQ_PLEN_TH                   (448)         /* ���а�����ֵ */
#define IPF_RESUME_COUNT                    (200)         /* ���˹���ָ���ʱ,��λ10us,��2ms */
#define IPF_OBLIGATE_AD_NUM                 (3)           /* ADQ����Ԥ��AD���� */
#define IPF_OBLIGATE_BD_NUM                 (3)           /* ADQ����Ԥ��BD���� */
#define IPF_CH_IDLE                         (0x14)        /* ͨ��IDLE״ֵ̬ */
#define IPF_IDLE_TIMEOUT_NUM                (30)          /* ����ȴ���ʱֵ,��λ10ms,ȷ����ʱ����IPF������ */
#define IPF_ADQ_BUF_EPT_MASK                0x00000004    /* ADQ BUFF��Ԥ��״̬mask */
#define IPF_ADQ_BUF_FULL_MASK               0x00000008    /* ADQ BUFF��Ԥ��״̬mask */

/* IPF�ж��ϱ������Ρ�״̬�Ĵ���λ����,�ϱ������Ρ�״̬����һһ��Ӧ */
#define IPF_UL_RPT_INT                     (1 << 0)    /* ���н���ϱ� */
#define IPF_UL_TIMEOUT_INT                 (1 << 1)    /* ���н���ϱ���ʱ */
#define IPF_UL_RDQ_DOWN_OVERFLOW_INT       (1 << 6)    /* ����RDQ���� */
#define IPF_UL_BDQ_UP_OVERFLOW_INT         (1 << 7)    /* ����BDQ���� */
#define IPF_UL_RDQ_FULL_INT                (1 << 8)    /* ����RDQ�� */
#define IPF_UL_BDQ_EMPTY_INT               (1 << 9)    /* ����ͨ��BDQ�� */
#define IPF_UL_ADQ0_EMPTY_INT              (1 << 10)   /* ����ͨ��ADQ0�� */
#define IPF_UL_ADQ1_EMPTY_INT              (1 << 11)   /* ����ͨ��ADQ1�� */

#define IPF_DL_RPT_INT                     (1 << 16)   /* ���н���ϱ� */
#define IPF_DL_TIMEOUT_INT                 (1 << 17)   /* ���н���ϱ���ʱ */
#define IPF_DL_RDQ_DOWN_OVERFLOW_INT       (1 << 22)   /* ����RDQ���� */
#define IPF_DL_BDQ_UP_OVERFLOW_INT         (1 << 23)   /* ����BDQ���� */
#define IPF_DL_RDQ_FULL_INT                (1 << 24)   /* ����RDQ�� */
#define IPF_DL_BDQ_EMPTY_INT               (1 << 25)   /* ����ͨ��BDQ�� */
#define IPF_DL_ADQ0_EMPTY_INT              (1 << 26)   /* ����ͨ��ADQ0�� */
#define IPF_DL_ADQ1_EMPTY_INT              (1 << 27)   /* ����ͨ��ADQ1�� */
/* INTO�ж��ϱ����: */
#define IPF_INT_OPEN0                       ( IPF_UL_RPT_INT \
                                            | IPF_UL_TIMEOUT_INT \
                                            | IPF_UL_ADQ0_EMPTY_INT \
                                            | IPF_UL_ADQ1_EMPTY_INT)   
/* INT1�ж��ϱ����: */                                            
#define IPF_INT_OPEN1                       ( IPF_DL_RPT_INT \
                                            | IPF_DL_TIMEOUT_INT \
                                            | IPF_DL_ADQ0_EMPTY_INT \
                                            | IPF_DL_ADQ1_EMPTY_INT)  
#ifdef __VXWORKS__
#define IPF_PRINT   printf
#define IPF_DEBUG(enLogLevel, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
    BSP_TRACE(enLogLevel, BSP_MODU_IPF, fmt,arg1,arg2,arg3,arg4,arg5,arg6)
#ifdef _DRV_LLT_
#define IPF_REGBASE_ADR                 ((BSP_U32)g_ipfRegStub)
#else
#define IPF_REGBASE_ADR                 SOC_IPF_BASE_ADDR
#endif
#define INT_LVL_IPF                     INT_LVL_IPF_0
#define INT_VEC_IPF                     IVEC_TO_INUM(INT_LVL_IPF)
#elif defined(__KERNEL__)
#define IPF_PRINT   printk
#define IPF_DEBUG(fmt, arg...) \
    printk(KERN_ERR fmt, ##arg)
#ifdef _DRV_LLT_
#define IPF_REGBASE_ADR                 ((BSP_U32)g_ipfRegStub)
#else
#define IPF_REGBASE_ADR                 IO_ADDRESS(SOC_IPF_BASE_ADDR)
#endif
#define INT_LVL_IPF                     IRQ_IPF1
#define INT_VEC_IPF                     IVEC_TO_INUM(INT_LVL_IPF)
#endif

#define IPF_REG_WRITE(uwAddr,uwValue)   (*((volatile BSP_U32 *)(uwAddr)) = uwValue)
#define IPF_REG_READ(uwAddr,uwValue)    (uwValue = *((volatile BSP_U32 *)(uwAddr))) 
/**************************************************************************
  ö�ٶ���
**************************************************************************/

/**************************************************************************
  STRUCT����
**************************************************************************/
typedef struct tagIPF_UL_S
{
    IPF_BD_DESC_S* pstIpfBDQ;    
    IPF_RD_DESC_S* pstIpfRDQ;
    IPF_AD_DESC_S* pstIpfADQ0;
    IPF_AD_DESC_S* pstIpfADQ1;
#ifdef PRODUCT_CFG_CORE_TYPE_MODEM      
    BSP_IPF_WakeupUlCb pFnUlIntCb; /* �ж��л��ѵ�PS���� */
    BSP_IPF_AdqEmptyUlCb pAdqEmptyUlCb;
#endif   
    BSP_U32* pu32IdleBd; /* ��¼��һ�λ�ȡ�Ŀ���BD �� */
}IPF_UL_S;

typedef struct tagIPF_DL_S
{
    IPF_BD_DESC_S* pstIpfBDQ;    
    IPF_RD_DESC_S* pstIpfRDQ;
    IPF_AD_DESC_S* pstIpfADQ0;
    IPF_AD_DESC_S* pstIpfADQ1;
    IPF_CD_DESC_S* pstIpfCDQ;
    IPF_CD_DESC_S* pstIpfDebugCDQ;
#ifndef PRODUCT_CFG_CORE_TYPE_MODEM     
    BSP_IPF_WakeupDlCb pFnDlIntCb; /* �ж��л��ѵ�PS���� */
    BSP_IPF_AdqEmptyDlCb pAdqEmptyDlCb;
#endif    
    BSP_U32* u32IpfCdRptr; /* ��ǰ���Զ�����λ�� */
    BSP_U32* u32IpfCdWptr;
    BSP_U32  u32IdleBd; /* ��¼��һ�λ�ȡ�Ŀ���BD �� */
}IPF_DL_S;

typedef struct tagIPF_ID_S
{
    BSP_U32 u32PsID;
    BSP_U32 u32FilterID;
    struct tagIPF_ID_S* pstNext;
}IPF_ID_S;

typedef struct tagIPF_FILTER_INFO_S
{
    IPF_ID_S* pstUseList;
    BSP_U32 u32FilterNum;
}IPF_FILTER_INFO_S;

typedef struct tagIPF_DEBUG_INFO_S
{
    BSP_U32 u32IpfDebug;            /* ����debug��ʶ:1-���� 0-��ֹ */

    BSP_U32 u32UlResultTime;        /* �������һ���ϱ�����ж�ʱ�� */
    BSP_U32 u32UlGetRdTime;         /* �������һ�λ�ȡ������ʱ�� */
    BSP_U32 u32DlResultTime;        /* �������һ���ϱ�����ж�ʱ�� */
    BSP_U32 u32DlGetRdTime;         /* �������һ�λ�ȡ������ʱ�� */
    BSP_U32 u32UlResultCnt;         /* �����ϱ�����жϼ��� */
    BSP_U32 u32UlResultTimeoutCnt;  /* �����ϱ������ʱ�жϼ��� */
    BSP_U32 u32DlResultCnt;         /* �����ϱ�����жϼ��� */
    BSP_U32 u32DlResultTimeoutCnt;  /* �����ϱ������ʱ�жϼ��� */

    BSP_U32 u32UlBdqEmptyCnt;       /* ����BDQ�ռ��� */
    BSP_U32 u32UlRdqFullCnt;        /* ����RDQ������ */
    BSP_U32 u32DlBdqEmptyCnt;       /* ����BDQ�ռ��� */
    BSP_U32 u32DlRdqFullCnt;        /* ����RDQ������ */
    BSP_U32 u32UlBdqOverflow;       /* �ж��ϱ�����BD����������� */
    BSP_U32 u32DlBdqOverflow;       /* �ж��ϱ�����BD����������� */
    BSP_U32 u32UlBdNotEnough;       /* ��������BD,BD�����ô��� */
    BSP_U32 u32DlBdNotEnough;       /* ��������BD,BD�����ô��� */
    BSP_U32 u32UlAdq0Empty;         /* �ж��ϱ�����ADQ0�մ��� */
    BSP_U32 u32UlAdq1Empty;         /* �ж��ϱ�����ADQ1�մ��� */
    BSP_U32 u32DlAdq0Empty;         /* �ж��ϱ�����ADQ0�մ��� */
    BSP_U32 u32DlAdq1Empty;         /* �ж��ϱ�����ADQ1�մ��� */
    BSP_U32 u32UlAd0NotEnough;      /* ����ADQ0�������� */
    BSP_U32 u32UlAd1NotEnough;      /* ����ADQ0�������� */
    BSP_U32 u32DlAd0NotEnough;      /* ����ADQ0�������� */
    BSP_U32 u32DlAd1NotEnough;      /* ����ADQ0�������� */
    BSP_U32 u32DlCdNotEnough;       /* ��������CD,CD�����ô��� */
    BSP_U32 u32UlIPFBusyNum;        /* ����IPFæ���� */
    BSP_U32 u32DlIPFBusyNum;        /* ����IPFæ���� */
    BSP_U32 u32UlResumeTimeoutCnt;  /* ���й��˹���ָ���ʱ���� */
    BSP_U32 u32DlResumeTimeoutCnt;  /* ���й��˹���ָ���ʱ���� */

    BSP_U32 u32UlBdNum;             /* ���з���BD���� */
    BSP_U32 u32UlAdq0Num;           /* ��������AD0���� */
    BSP_U32 u32UlAdq1Num;           /* ��������AD1���� */
    BSP_U32 u32UlRdNum;             /* ���н���RD���� */
    BSP_U32 u32DlBdNum;             /* ���з���BD���� */
    BSP_U32 u32DlAdq0Num;           /* ��������AD1���� */
    BSP_U32 u32DlAdq1Num;           /* ��������AD1���� */
    BSP_U32 u32DlRdNum;             /* ���н���RD���� */
}IPF_DEBUG_INFO_S;

typedef struct tagIPF_ADQ_INFO_S
{
    BSP_U32 u32RptReg;      /* ��ָ��Ĵ��� */
    BSP_U32 u32StaReg;      /* ״̬�Ĵ��� */
    BSP_U32 u32AqMaxNum;    /* ���֧�ֵĸ��� */    
}IPF_ADQ_INFO_S;

/**************************************************************************
  UNION����
**************************************************************************/

/**************************************************************************
  OTHERS����
**************************************************************************/



/**************************************************************************
  ��������
**************************************************************************/
#endif

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End #define _BSP_DRV_IPF_H_ */

