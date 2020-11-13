
#ifdef __cplusplus
extern "C" {
#endif
#include "product_config.h"
#if (FEATURE_IPF_VERSION == IPF_V200)
#ifdef __VXWORKS__
#include <vxWorks.h>
#include <cacheLib.h>
#include <intLib.h>
#include <logLib.h>
#include <string.h>
#include <taskLib.h>
#include <memLib.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "BSP_DRV_IPF_V200.h"
#include "pwrctrl_multi_memcfg.h"
#include "BSP_MEM.h"
#include "soc_sctrl_interface.h"
#include "BSP_TIMER.h"
#include "osal.h"
#elif defined(__KERNEL__)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <mach/common/mem/bsp_mem_drv.h>
#include "bsp_drv_ipf_v200.h"
#endif

/* �������� */
IPF_UL_S g_stIpfUl = {0};
IPF_DL_S g_stIpfDl = {0};
#ifdef __BSP_IPF_DEBUG__
IPF_DEBUG_INFO_S* g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)BSP_NULL;
BSP_U32 g_32IpfDebugReg[IPF_REG_NUM] = {0};
#endif
BSP_U32* g_pbIPFInit = (BSP_U32*)BSP_NULL;
#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
IPF_MATCH_INFO_S* g_pstExFilterAddr;
IPF_ID_S* g_stIPFExtFreeList = (IPF_ID_S*)BSP_NULL;
IPF_ID_S* g_stIPFBasicFreeList = (IPF_ID_S*)BSP_NULL;
IPF_FILTER_INFO_S g_stIPFFilterInfo[IPF_FILTER_CHAIN_MAX_NUM];
OSAL_SEM_ID g_ipfSetFltSem ;
#endif
#ifdef _DRV_LLT_
BSP_CHAR g_ipfInfoStub[30*1024] = {0};
BSP_U32 g_ipfRegStub[200] = {0};
BSP_U32 g_ipfResoreFlagStub = 0;
#endif
/* �ڲ��������� */
#ifdef __VXWORKS__
BSP_VOID IPF_IntHandler(BSP_VOID);
BSP_VOID IPF_DeleteAll(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead);
#elif defined(__KERNEL__)
irqreturn_t  IPF_IntHandler (int irq, void* dev);
#endif
BSP_VOID IPF_Int_Connect(BSP_VOID);
BSP_S32 IPF_FilterList_Init(BSP_VOID);
BSP_S32 IPF_WaitResume(BSP_VOID);
#ifdef _DRV_LLT_
#define IPF_PWRCTRL_RESTORE_FLAG_ADDR   (&g_ipfResoreFlagStub)
#define IPF_PWRCTRL_RESTORE_FLAG_SIZE    4
#define SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(x) (&g_ipfResoreFlagStub)
#define SOC_SCTRL_SC_PERIPH_RSTDIS1_ADDR(x) (&g_ipfResoreFlagStub)
#else
#define IPF_PWRCTRL_RESTORE_FLAG_ADDR   (DDR_PHY_TO_VIRT(MEMORY_RAM_CORESHARE_IPF_FLAG_ADDR))
#define IPF_PWRCTRL_RESTORE_FLAG_SIZE   (MEMORY_RAM_CORESHARE_IPF_FLAG_SIZE)
#endif
#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
#ifdef _DRV_LLT_
BSP_U8 g_ipfRuleInfoStub[MEMORY_RAM_CORESHARE_IPF_RULE_SIZE] = {0};
#define IPF_PWRCTRL_RULE_INFO_ADDR      (g_ipfRuleInfoStub)
#define IPF_PWRCTRL_RULE_INFO_SIZE      (MEMORY_RAM_CORESHARE_IPF_RULE_SIZE)
#else
#define IPF_PWRCTRL_RULE_INFO_ADDR      (MEMORY_RAM_CORESHARE_IPF_RULE_ADDR)
#define IPF_PWRCTRL_RULE_INFO_SIZE      (MEMORY_RAM_CORESHARE_IPF_RULE_SIZE)
#endif
#endif
/*Modify_for_c_reset, l00212112,20130511, starts*/
/*�ı�������C�˸�λʱ������IPF���У�0:����1:������*/
static IPF_FORRESET_CONTROL_E g_forCcore_reset_Up_flag = 0;

/*****************************************************************************
* �� �� ��  : BSP_IPF_SetControlFLagForCcoreReset
*
* ��������  : ccore��λʱ������IPF��־�����ڿ���IPF����
*
* �������  : IPF_FORRESET_CONTROL_E flagvalue,0:����1:����������
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS    ��ʼ���ɹ�
*             IPF_ERROR      ��ʼ��ʧ��
*
* �޸ļ�¼  :2013��4��19��   ¬��ʤ ����
*   1.�޸����� :2013��9��12
*     �޸����� :��ɺ
*     �޸ļ�¼ :�����μ�顢forbidden֮��ȴ�idle
*****************************************************************************/
BSP_VOID BSP_IPF_SetControlFLagForCcoreReset(IPF_FORRESET_CONTROL_E flagvalue)
{
    BSP_U32 u32UlState = 0;
    BSP_U32 u32DlState = 0;
    BSP_U32 u32Cnt = 0;

    /* ��μ�� */
    if(IPF_FORRESET_CONTROL_MAX <= flagvalue)
    {
        IPF_DEBUG("BSP_IPF_SetControlFLagForCcoreReset:para error\n");
    }

    /* ���õ�ǰIPF״̬ */
    g_forCcore_reset_Up_flag = flagvalue;

    if(IPF_FORRESET_CONTROL_FORBID == flagvalue)
    {
        /* �ȴ�IPF����idle̬(���ͷ�CPU������ȴ���ʱ),����޷�����idle����ȴ�ʱ���ȷ��IPF���ٴ�������Ϊֹ */
        do{
            IPF_REG_READ(SOC_IPF_CH0_STATE_ADDR(IPF_REGBASE_ADR), u32UlState);
            IPF_REG_READ(SOC_IPF_CH1_STATE_ADDR(IPF_REGBASE_ADR), u32DlState);
            msleep(10);
            u32Cnt++;
        }while(((u32UlState != IPF_CH_IDLE) || (u32DlState != IPF_CH_IDLE)) && (u32Cnt <= IPF_IDLE_TIMEOUT_NUM));
        if(u32UlState != IPF_CH_IDLE)
        {
            IPF_DEBUG("BSP_IPF_SetControlFLagForCcoreReset:ul not idle\n");
        }
        if(u32DlState != IPF_CH_IDLE)
        {
            IPF_DEBUG("BSP_IPF_SetControlFLagForCcoreReset:dl not idle\n");
        }
    }
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_GetControlFLagForCcoreReset
*
* ��������  : ���ccore��λʱ�����õ�IPF��־�����ڿ���IPF����
*
* �������  : ��
* �������  : ��
* �� �� ֵ  : 0������1��������

*
* �޸ļ�¼  :2013��4��19��   ¬��ʤ ����
*   1.�޸����� :
*     �޸����� :
*     �޸ļ�¼ :
*****************************************************************************/
static IPF_FORRESET_CONTROL_E BSP_IPF_GetControlFLagForCcoreReset(BSP_VOID)
{
    return g_forCcore_reset_Up_flag;
}
/*Modify_for_c_reset, l00212112,20130511, ends*/

#ifdef __VXWORKS__
BSP_S32 IPF_Init(BSP_VOID)
#elif defined(__KERNEL__)
BSP_S32 IPF_Init(BSP_VOID)
#endif
{
#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
    BSP_U32 ret = 0;
    BSP_U32 u32ChanCtrl[IPF_CHANNEL_MAX] = {0,0};
    BSP_U32 u32IntStatus = 0;
    SOC_IPF_TIME_OUT_UNION unTimeout;
    /* IPF֧��BDQ�Ĺ�� */
    BSP_U32 u32BDSize[IPF_CHANNEL_MAX] = {IPF_ULBD_DESC_SIZE, IPF_DLBD_DESC_SIZE};
    /* IPF֧��RDQ�Ĺ�� */
    BSP_U32 u32RDSize[IPF_CHANNEL_MAX] = {IPF_ULRD_DESC_SIZE, IPF_DLRD_DESC_SIZE};
    /* IPF֧��ADQ�Ĺ�� */
    BSP_U32 u32ADCtrl[IPF_CHANNEL_MAX] = {(IPF_ULADQ_PLEN_TH<<16)|(IPF_ADSIZE_64<<4)|(IPF_ADSIZE_64<<2)|(IPF_BOTH_ADQ_EN),
                                          (IPF_DLADQ_PLEN_TH<<16)|( IPF_ADSIZE_64<<4)|(IPF_ADSIZE_64<<2)|(IPF_BOTH_ADQ_EN)};

    SOC_IPF_DMA_CTRL1_UNION unDMAOutstanding;

    /* IPF�ڴ�����Խ���� */
    if(IPF_MEM_USED_SIZE > IPF_AXI_MEM_SIZE)
    {
        IPF_DEBUG("IPF_Init:mem beyond error\n");
        return IPF_ERROR;
    }

    /* ����IPF hclkʱ�ӡ�axi����ʱ�� */
    IPF_REG_WRITE(SOC_SCTRL_SC_PERIPH_CLKEN2_ADDR(SOC_SC_OFF_BASE_ADDR), \
              (0x1 << SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_ipf_axi_START)
            | (0x1 << SOC_SCTRL_SC_PERIPH_CLKEN2_periph_clken2_ipf_hclk_START));
    /* ����IPF �⸴λ */
    IPF_REG_WRITE(SOC_SCTRL_SC_PERIPH_RSTDIS1_ADDR(SOC_SC_OFF_BASE_ADDR), \
              (0x1 << SOC_SCTRL_SC_PERIPH_RSTDIS1_periph_rstdis1_21reset_ipf_n_START));

    /* ��ʼ��ȫ�ֽṹ�� */
    memset((BSP_VOID*)IPF_AXI_MEM_ADDR, 0x0, IPF_AXI_MEM_SIZE);
    memset((BSP_VOID*)IPF_PWRCTRL_RESTORE_FLAG_ADDR, 0x0, IPF_PWRCTRL_RESTORE_FLAG_SIZE);
    memset((BSP_VOID*)IPF_PWRCTRL_RULE_INFO_ADDR, 0x0, IPF_PWRCTRL_RULE_INFO_SIZE);
    memset((BSP_VOID*)&g_stIPFFilterInfo[0], 0x0, IPF_FILTER_CHAIN_MAX_NUM * sizeof(IPF_FILTER_INFO_S));

    /* Ϊ����BD��RD����������һ�������������ַ */
    g_stIpfUl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_ULBD_MEM_ADDR;
    g_stIpfUl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_ULRD_MEM_ADDR;

    /* Ϊ����AD����������һ�������ڴ棨�׵�ַ8�ֽڶ��룩*/
    g_stIpfUl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_ULAD0_MEM_ADDR;
    g_stIpfUl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_ULAD1_MEM_ADDR;

    /* Ϊ����BD��RD����������һ�������������ַ */
    g_stIpfDl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_DLBD_MEM_ADDR;
    g_stIpfDl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_DLRD_MEM_ADDR;

    /* Ϊ����AD����������һ�������ڴ棨�׵�ַ8�ֽڶ��룩*/
    g_stIpfDl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_DLAD0_MEM_ADDR;
    g_stIpfDl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_DLAD1_MEM_ADDR;

    /* Ϊ����CD����������һ�������������ַ */
    g_stIpfDl.pstIpfCDQ = (IPF_CD_DESC_S*)IPF_DLCD_MEM_ADDR;

    /* IPF��ʼ����־ */
    g_pbIPFInit = (BSP_U32*)IPF_INIT_ADDR;

    /* ��¼IPF���п���BD���� */
    g_stIpfUl.pu32IdleBd = (BSP_U32*)IPF_ULBD_IDLENUM_ADDR;
    IPF_REG_WRITE(IPF_ULBD_IDLENUM_ADDR, IPF_ULBD_DESC_SIZE);

#ifdef __BSP_IPF_DEBUG__
    /* ��¼IPF debug��Ϣ */
    g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)IPF_DEBUG_INFO_ADDR;
    memset((void*)g_stIPFDebugInfo, 0x0, IPF_DEBUG_INFO_SIZE);

    /* ��¼IPF ����CDdebug��Ϣ */
    g_stIpfDl.pstIpfDebugCDQ = (IPF_CD_DESC_S*)IPF_DEBUG_DLCD_ADDR;
#endif

    /* ��¼IPF ����CD��дָ�� */
    g_stIpfDl.u32IpfCdRptr = (BSP_U32*)IPF_DLCD_PTR_ADDR;
    g_stIpfDl.u32IpfCdWptr = (BSP_U32*)(IPF_DLCD_PTR_ADDR+4);

    /* ��¼IPF���п���BD���� */
    g_stIpfDl.u32IdleBd = IPF_DLBD_DESC_SIZE;

    /* ������չ���������ڴ�, ������չ�������Ļ�ַ */
    g_pstExFilterAddr = (IPF_MATCH_INFO_S*)cacheDmaMalloc(IPF_EXFLITER_NUM * sizeof(IPF_MATCH_INFO_S));
    memset((BSP_VOID*)g_pstExFilterAddr, 0x0, IPF_EXFLITER_NUM * sizeof(IPF_MATCH_INFO_S));

    /* ������չ����������ʼ��ַ */
    IPF_REG_WRITE(SOC_IPF_EF_BADDR_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_pstExFilterAddr);

    /* ��ʼ������������ */
    if(IPF_FilterList_Init() != IPF_SUCCESS)
    {
        IPF_DEBUG("IPF_Init:malloc list error\n");
        return IPF_ERROR;
    }

    /* ���ó�ʱ���üĴ�����ʹ�ܳ�ʱ�жϣ����ó�ʱʱ�� */
    unTimeout.value = 0;
    unTimeout.reg.time_out_cfg = IPF_TIME_OUT_CFG;
    unTimeout.reg.time_out_valid = 1;
    IPF_REG_WRITE(SOC_IPF_TIME_OUT_ADDR(IPF_REGBASE_ADR), unTimeout.value);

    /* ������ж���RD��BD��дָ�� */
    IPF_REG_READ(SOC_IPF_CH0_CTRL_ADDR(IPF_REGBASE_ADR), u32ChanCtrl[IPF_CHANNEL_UP]);
    u32ChanCtrl[IPF_CHANNEL_UP] |= 0x30;
    IPF_REG_WRITE(SOC_IPF_CH0_CTRL_ADDR(IPF_REGBASE_ADR), u32ChanCtrl[IPF_CHANNEL_UP]);
    /* ��������ͨ����BD��RD��� */
    IPF_REG_WRITE(SOC_IPF_CH0_BDQ_SIZE_ADDR(IPF_REGBASE_ADR), u32BDSize[IPF_CHANNEL_UP]-1);
    IPF_REG_WRITE(SOC_IPF_CH0_RDQ_SIZE_ADDR(IPF_REGBASE_ADR), u32RDSize[IPF_CHANNEL_UP]-1);
    IPF_REG_WRITE(SOC_IPF_CH0_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32ADCtrl[IPF_CHANNEL_UP]);

    /* ������ж���RD��BD��дָ�� */
    IPF_REG_READ(SOC_IPF_CH1_CTRL_ADDR(IPF_REGBASE_ADR), u32ChanCtrl[IPF_CHANNEL_DOWN]);
    u32ChanCtrl[IPF_CHANNEL_DOWN] |= 0x30;
    IPF_REG_WRITE(SOC_IPF_CH1_CTRL_ADDR(IPF_REGBASE_ADR), u32ChanCtrl[IPF_CHANNEL_DOWN]);

    /* ��������ͨ����BD��RD��� */
    IPF_REG_WRITE(SOC_IPF_CH1_BDQ_SIZE_ADDR(IPF_REGBASE_ADR), u32BDSize[IPF_CHANNEL_DOWN]-1);
    IPF_REG_WRITE(SOC_IPF_CH1_RDQ_SIZE_ADDR(IPF_REGBASE_ADR), u32RDSize[IPF_CHANNEL_DOWN]-1);
    IPF_REG_WRITE(SOC_IPF_CH1_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32ADCtrl[IPF_CHANNEL_DOWN]);

    /* ���к�����ͨ����BD��RD�Լ�AD��ʼ��ַ*/
    IPF_REG_WRITE(SOC_IPF_CH0_BDQ_BADDR_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfUl.pstIpfBDQ);
    IPF_REG_WRITE(SOC_IPF_CH0_RDQ_BADDR_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfUl.pstIpfRDQ);
    IPF_REG_WRITE(SOC_IPF_CH0_ADQ0_BASE_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfUl.pstIpfADQ0);
    IPF_REG_WRITE(SOC_IPF_CH0_ADQ1_BASE_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfUl.pstIpfADQ1);

    IPF_REG_WRITE(SOC_IPF_CH1_BDQ_BADDR_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfDl.pstIpfBDQ);
    IPF_REG_WRITE(SOC_IPF_CH1_RDQ_BADDR_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfDl.pstIpfRDQ);
    IPF_REG_WRITE(SOC_IPF_CH1_ADQ0_BASE_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfDl.pstIpfADQ0);
    IPF_REG_WRITE(SOC_IPF_CH1_ADQ1_BASE_ADDR(IPF_REGBASE_ADR), (BSP_U32)g_stIpfDl.pstIpfADQ1);

    /* �����ж�����,ֻ��3���жϣ�����ϱ��������ʱ��RD���� */
    IPF_REG_WRITE(SOC_IPF_INT_MASK0_ADDR(IPF_REGBASE_ADR), IPF_INT_OPEN0);
    IPF_REG_WRITE(SOC_IPF_INT_MASK1_ADDR(IPF_REGBASE_ADR), IPF_INT_OPEN1);

    /* ���ж� */
    IPF_REG_READ(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), u32IntStatus);
    IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), u32IntStatus);

    /*��DMAͨ����Outstanding��дʹ��*/
    unDMAOutstanding.value = 0;
    unDMAOutstanding.reg.outstanding_w = 1;
    unDMAOutstanding.reg.outstanding_r = 1;
    IPF_REG_WRITE(SOC_IPF_DMA_CTRL1_ADDR(IPF_REGBASE_ADR), unDMAOutstanding.value);

    /* �ҽ�IPF�ж� */
    IPF_Int_Connect();

    /* ��ʼ��IPF���ݻָ���־����ʼ̬Ϊ�ѻָ� */
    *(volatile BSP_U32*)IPF_PWRCTRL_RESTORE_FLAG_ADDR = IPF_STATE_RESTORED;

    ret = OSAL_SemBCreate (&g_ipfSetFltSem, OSAL_SEM_FULL, OSAL_SEM_PRIORITY);
    if(OSAL_OK != ret)
    {
        IPF_DEBUG("IPF_Init:sem create fail\n");
        return IPF_ERROR;
    }

    /* IPF��ʼ����� */
    *g_pbIPFInit = BSP_TRUE;

    /* �ͷ�IPFͬ���ź� */
    BSP_SYNC_Give(SYNC_MODULE_IPF);

    IPF_DEBUG("IPF_Init:success\n");
    return IPF_SUCCESS;
#else
    BSP_S32 s32Ret = 0;

    /* IPF�ڴ�����Խ���� */
    if(IPF_MEM_USED_SIZE > IPF_AXI_MEM_SIZE)
    {
        IPF_DEBUG("IPF_Init:mem beyond error\n");
        return IPF_ERROR;
    }

    /* Ϊ����BD��RD����������һ�������������ַ */
    g_stIpfUl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_ULBD_MEM_ADDR;
    g_stIpfUl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_ULRD_MEM_ADDR;

    /* Ϊ����AD����������һ�������ڴ棨�׵�ַ8�ֽڶ��룩*/
    g_stIpfUl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_ULAD0_MEM_ADDR;
    g_stIpfUl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_ULAD1_MEM_ADDR;

    /* Ϊ����BD��RD����������һ�������������ַ */
    g_stIpfDl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_DLBD_MEM_ADDR;
    g_stIpfDl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_DLRD_MEM_ADDR;

    /* Ϊ����AD����������һ�������ڴ棨�׵�ַ8�ֽڶ��룩*/
    g_stIpfDl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_DLAD0_MEM_ADDR;
    g_stIpfDl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_DLAD1_MEM_ADDR;

    /* Ϊ����CD����������һ�������������ַ */
    g_stIpfDl.pstIpfCDQ = (IPF_CD_DESC_S*)IPF_DLCD_MEM_ADDR;

    /* IPF��ʼ����־ */
    g_pbIPFInit = (BSP_U32*)IPF_INIT_ADDR;

    /* ��¼IPF���п���BD���� */
    g_stIpfUl.pu32IdleBd = (BSP_U32*)IPF_ULBD_IDLENUM_ADDR;

#ifdef __BSP_IPF_DEBUG__
    /* ��¼IPF debug��Ϣ */
    g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)IPF_DEBUG_INFO_ADDR;

    /* ��¼IPF ����CDdebug��Ϣ */
    g_stIpfDl.pstIpfDebugCDQ = (IPF_CD_DESC_S*)IPF_DEBUG_DLCD_ADDR;
#endif

    /* ��¼IPF ����CD��дָ�� */
    g_stIpfDl.u32IpfCdRptr = (BSP_U32*)IPF_DLCD_PTR_ADDR;
    g_stIpfDl.u32IpfCdWptr = (BSP_U32*)(IPF_DLCD_PTR_ADDR+4);

    /* �ҽ�IPF�ж� */
    IPF_Int_Connect();

    /* �ȴ�����һ��core ipf��ʼ��ͬ����� */
    s32Ret = BSP_SYNC_Wait(SYNC_MODULE_IPF, 5000);

    if(s32Ret != BSP_OK)
    {
        IPF_DEBUG("BSP_SYNC_Wait:timeout\n");
        return IPF_ERROR;
    }

    IPF_DEBUG("IPF_Init:success\n");
    return IPF_SUCCESS;

#endif
}

/*****************************************************************************
* �� �� ��  : IPF_Int_Connect
*
* ��������  : ��IPF�жϴ�����(���˶��ṩ)
*
* �������  : BSP_VOID
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��12��2��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_Int_Connect(BSP_VOID)
{
#ifdef __VXWORKS__
    /* ���ж�ʹ�� */
    (BSP_VOID)BSP_INT_Connect((VOIDFUNCPTR *)(INT_LVL_IPF), (VOIDFUNCPTR)IPF_IntHandler, 0);
    /* ʹ���жϺ� */
    (BSP_VOID)BSP_INT_Enable(INT_LVL_IPF);
#elif defined(__KERNEL__)
    if(request_irq(INT_LVL_IPF, (irq_handler_t)IPF_IntHandler, 0, "IPF_APP_IRQ", NULL) != 0)
    {
        IPF_DEBUG("IPF_Int_Connect:irequest_irq error\n");
    }
#endif
}


#ifdef __VXWORKS__
BSP_VOID IPF_IntHandler(BSP_VOID)
#elif defined(__KERNEL__)
irqreturn_t  IPF_IntHandler (int irq, void* dev)
#endif
{
    BSP_U32 u32IpfAdqInt = 0;

#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
    SOC_IPF_INT0_UNION unIpfInt;

    unIpfInt.value = 0;
    /* ��ȡ�ж�״̬ */
    IPF_REG_READ(SOC_IPF_INT0_ADDR(IPF_REGBASE_ADR), unIpfInt.value);
    /* ���н���ϱ��ͽ����ʱ�ϱ� */
    if((unIpfInt.reg.ul_rpt_int0) || (unIpfInt.reg.ul_timeout_int0))
    {
        /* д1������н���ͽ����ʱ�ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), (IPF_UL_RPT_INT | IPF_UL_TIMEOUT_INT));
    #ifdef __BSP_IPF_DEBUG__
        if(unIpfInt.reg.ul_rpt_int0)
        {
            g_stIPFDebugInfo->u32UlResultCnt++;
        }
        else
        {
            g_stIPFDebugInfo->u32UlResultTimeoutCnt++;
        }
    #endif
        /* ����ps�������� */
        if(g_stIpfUl.pFnUlIntCb != BSP_NULL)
        {
            (BSP_VOID)g_stIpfUl.pFnUlIntCb();
        }
        else
        {
            IPF_DEBUG("IPF_IntHandler:ul task null\n");
        }
    }
    /* ����ADQ0��ADQ1��ָʾ */
    if(unIpfInt.reg.ul_adq0_epty_int0)
    {
        /* д1�������ADQ0���ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), IPF_UL_ADQ0_EMPTY_INT);
    #ifdef __BSP_IPF_DEBUG__
        g_stIPFDebugInfo->u32UlAdq0Empty++;
    #endif
        u32IpfAdqInt += IPF_EMPTY_ADQ0;
    }

    if(unIpfInt.reg.ul_adq1_epty_int0)
    {
        /* д1�������ADQ1���ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), IPF_UL_ADQ1_EMPTY_INT);
    #ifdef __BSP_IPF_DEBUG__
        g_stIPFDebugInfo->u32UlAdq1Empty++;
    #endif
        u32IpfAdqInt += IPF_EMPTY_ADQ1;
    }

    if(u32IpfAdqInt)
    {
        if(g_stIpfUl.pAdqEmptyUlCb != BSP_NULL)
        {
            (BSP_VOID)g_stIpfUl.pAdqEmptyUlCb((IPF_ADQ_EMPTY_E)u32IpfAdqInt);
        }
        else
        {
            IPF_DEBUG("IPF_IntHandler:ul adq null\n");
        }
    }
#else
    SOC_IPF_INT1_UNION unIpfInt;

    unIpfInt.value = 0;
    /* ��ȡ�ж�״̬ */
    IPF_REG_READ(SOC_IPF_INT1_ADDR(IPF_REGBASE_ADR), unIpfInt.value);
    /* ���н���ϱ��ͽ����ʱ�ϱ� */
    if((unIpfInt.reg.dl_rpt_int1) || (unIpfInt.reg.dl_timeout_int1))
    {
        /* д1������н���ͽ����ʱ�ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), (IPF_DL_RPT_INT | IPF_DL_TIMEOUT_INT));
    #ifdef __BSP_IPF_DEBUG__
        if(unIpfInt.reg.dl_rpt_int1)
        {
            g_stIPFDebugInfo->u32DlResultCnt++;
        }
        else
        {
            g_stIPFDebugInfo->u32DlResultTimeoutCnt++;
        }
    #endif
        /* ����ps�������� */
        if(g_stIpfDl.pFnDlIntCb != BSP_NULL)
        {
            (BSP_VOID)g_stIpfDl.pFnDlIntCb();
        }
        else
        {
            IPF_DEBUG("IPF_IntHandler:dl task null\n");
        }
    }
    /* ����ADQ0��ADQ1��ָʾ */
    if(unIpfInt.reg.dl_adq0_epty_int1)
    {
        /* д1�������ADQ0���ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), IPF_DL_ADQ0_EMPTY_INT);
    #ifdef __BSP_IPF_DEBUG__
        g_stIPFDebugInfo->u32DlAdq0Empty++;
    #endif
        u32IpfAdqInt += IPF_EMPTY_ADQ0;
    }

    if(unIpfInt.reg.dl_adq1_epty_int1)
    {
        /* д1�������ADQ1���ж� */
        IPF_REG_WRITE(SOC_IPF_INT_STATE_ADDR(IPF_REGBASE_ADR), IPF_DL_ADQ1_EMPTY_INT);
    #ifdef __BSP_IPF_DEBUG__
        g_stIPFDebugInfo->u32DlAdq1Empty++;
    #endif
        u32IpfAdqInt += IPF_EMPTY_ADQ1;
    }

    if(u32IpfAdqInt)
    {
        if(g_stIpfDl.pAdqEmptyDlCb != BSP_NULL)
        {
            (BSP_VOID)g_stIpfDl.pAdqEmptyDlCb(u32IpfAdqInt);
        }
        else
        {
            IPF_DEBUG("IPF_IntHandler:dl adq null\n");
        }
    }
    return IRQ_HANDLED;
#endif
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_ConfigTimeout
*
* ��������  : ����ʹ�ã����ó�ʱʱ��ӿ�
*
* �������  : BSP_U32 u32Timeout ���õĳ�ʱʱ��
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS                   �ɹ�
*             BSP_ERR_IPF_INVALID_PARA      ������Ч
*
* ˵��      : 1����256��ʱ������
*
* �޸ļ�¼   : 2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigTimeout(BSP_U32 u32Timeout)
{
    SOC_IPF_TIME_OUT_UNION unTimeout;

    unTimeout.value = 0;
    if((u32Timeout == 0) || (u32Timeout > 0xFFFF))
    {
        IPF_DEBUG("BSP_IPF_ConfigTimeout:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    unTimeout.reg.time_out_cfg = u32Timeout;
    unTimeout.reg.time_out_valid = 1;

    IPF_REG_WRITE(SOC_IPF_TIME_OUT_ADDR(IPF_REGBASE_ADR), unTimeout.value);

    return IPF_SUCCESS;
}


BSP_S32 BSP_IPF_ConfigUpFilter(BSP_U32 u32Num, IPF_CONFIG_ULPARAM_S* pstUlPara)
{
    BSP_U32 i = 0;
    BSP_U32 u32BdqWptr = 0;
    IPF_CONFIG_ULPARAM_S* pstUlParam = pstUlPara;
#ifdef __BSP_IPF_DEBUG__
    SOC_IPF_CH0_ADQ_CTRL_UNION unCh0AdqCtrl;
#endif
/*Modify_for_c_reset, l00212112,20130511, starts*/
    /*���C�˸�λ���򷵻�*/
    if (IPF_FORRESET_CONTROL_FORBID == BSP_IPF_GetControlFLagForCcoreReset())
    {
        IPF_DEBUG("BSP_IPF_ConfigUpFilter:ccore is resetting\n");
        return BSP_ERR_IPF_CCORE_RESETTING;
    }
/*Modify_for_c_reset, l00212112,20130511, ends*/
    /* ������� */
    if((NULL == pstUlPara))
    {
        IPF_DEBUG("BSP_IPF_ConfigUpFilter:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_ConfigUpFilter:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    /* �쳣��ά�ɲ� */
#ifdef __BSP_IPF_DEBUG__
    if(u32Num > *(g_stIpfUl.pu32IdleBd))
    {
        g_stIPFDebugInfo->u32UlBdNotEnough++;
        return IPF_ERROR;
    }

    IPF_REG_READ(SOC_IPF_CH0_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), unCh0AdqCtrl.value);
    if(unCh0AdqCtrl.reg.ul_adq_en != IPF_BOTH_ADQ_EN)
    {
        IPF_DEBUG("BSP_IPF_ConfigUpFilter:adq not support\n");
        return IPF_ERROR;
    }

    g_stIPFDebugInfo->u32UlBdNum += u32Num;
#endif

    /* ȷ��IPF�ѻָ� */
    if(IPF_ERROR == IPF_WaitResume())
    {
        IPF_DEBUG("IPF_WaitResume:timeout\n");
        g_stIPFDebugInfo->u32UlResumeTimeoutCnt++;
        return  BSP_ERR_IPF_RESUME_TIMEOUT;
    }

    /* ����BDдָ��,��u32BdqWptr��Ϊ��ʱдָ��ʹ�� */
    IPF_REG_READ(SOC_IPF_CH0_BDQ_WPTR_ADDR(IPF_REGBASE_ADR), u32BdqWptr);

    for(i = 0; i < u32Num; i++)
    {
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u16Attribute = pstUlParam[i].u16Attribute;
        if(0 == pstUlParam[i].u32Data)
        {
            IPF_DEBUG("BSP_IPF_ConfigUpFilter:null point\n");
            return BSP_ERR_IPF_INVALID_PARA;
        }
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u32InPtr = pstUlParam[i].u32Data;
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u16PktLen = pstUlParam[i].u16Len;
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u16UsrField1 = pstUlParam[i].u16UsrField1;
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u32UsrField2= pstUlParam[i].u32UsrField2;
        g_stIpfUl.pstIpfBDQ[u32BdqWptr].u32UsrField3 = pstUlParam[i].u32UsrField3;
    #ifdef __BSP_IPF_DEBUG__
        if(g_stIPFDebugInfo->u32IpfDebug)
        {
            IPF_DEBUG("BSP_IPF_ConfigUpFilter u32BdqWptr=%d  u32UsrField2=%x, u32UsrField3=%x\n", u32BdqWptr,
                g_stIpfUl.pstIpfBDQ[u32BdqWptr].u32UsrField2,g_stIpfUl.pstIpfBDQ[u32BdqWptr].u32UsrField3);
        }
    #endif
        u32BdqWptr = ((u32BdqWptr + 1) < IPF_ULBD_DESC_SIZE)? (u32BdqWptr + 1) : 0;
    }

    /* ����BDдָ��*/
    IPF_REG_WRITE(SOC_IPF_CH0_BDQ_WPTR_ADDR(IPF_REGBASE_ADR), u32BdqWptr);

    return IPF_SUCCESS;
}
BSP_U32 BSP_IPF_GetUlDescNum(BSP_VOID)
{
    BSP_U32 u32UlAd0Num = 0;
    BSP_U32 u32UlAd1Num = 0;
    BSP_U32 u32UlBdNum = 0;
    BSP_U32 u32UlAdwptr = 0;
    BSP_U32 u32UlAdrptr = 0;
    BSP_U32 u32UlBdDepth = 0;
    SOC_IPF_CH0_DQ_DEPTH_UNION unDQDepth;

    /* ����BD��� */
    unDQDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH0_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unDQDepth.value);
    u32UlBdDepth = unDQDepth.reg.ul_bdq_depth;

    /* �������AD0���� */
    IPF_REG_READ(SOC_IPF_CH0_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32UlAdwptr);
    IPF_REG_READ(SOC_IPF_CH0_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), u32UlAdrptr);

    /*дָ����ǰ������˳��*/
    if (u32UlAdwptr >= u32UlAdrptr)
    {
        u32UlAd0Num = u32UlAdwptr - u32UlAdrptr;
    }
    else
    {
        u32UlAd0Num = IPF_ULAD0_DESC_SIZE - (u32UlAdrptr -u32UlAdwptr);
    }
    if(u32UlAd0Num > u32UlBdDepth)
    {
        u32UlAd0Num -= u32UlBdDepth;
    }
    else
    {
        u32UlAd0Num = 0;
        g_stIPFDebugInfo->u32UlAd0NotEnough++;
    }

    /* �������AD1���� */
    IPF_REG_READ(SOC_IPF_CH0_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32UlAdwptr);
    IPF_REG_READ(SOC_IPF_CH0_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), u32UlAdrptr);

    if (u32UlAdwptr >= u32UlAdrptr)/*дָ����ǰ������˳��*/
    {
        u32UlAd1Num = u32UlAdwptr - u32UlAdrptr;
    }
    else
    {
        u32UlAd1Num =  IPF_ULAD1_DESC_SIZE - (u32UlAdrptr -u32UlAdwptr);
    }

    if(u32UlAd1Num > u32UlBdDepth)
    {
        u32UlAd1Num -= u32UlBdDepth;
    }
    else
    {
        u32UlAd1Num = 0;
        g_stIPFDebugInfo->u32UlAd1NotEnough++;
    }

    u32UlBdNum = BSP_IPF_GetUlBDNum();
    if(0 == u32UlBdNum)
    {
        g_stIPFDebugInfo->u32UlBdNotEnough++;
    }

    if(u32UlBdNum > u32UlAd0Num)
    {
        u32UlBdNum = u32UlAd0Num;
    }

    if(u32UlBdNum > u32UlAd1Num)
    {
        u32UlBdNum = u32UlAd1Num;
    }

    return u32UlBdNum;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_GetUlBDNum
*
* ��������  : �ýӿ����ڻ�ȡ���п���BD ��Ŀ
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : ����BD��Ŀ
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetUlBDNum(BSP_VOID)
{
    SOC_IPF_CH0_DQ_DEPTH_UNION unUlBdDepth;
    BSP_U32 u32IdleBd = 0;

    unUlBdDepth.value = 0;
    /* �������BD���� */
    IPF_REG_READ(SOC_IPF_CH0_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unUlBdDepth.value);
    u32IdleBd = IPF_ULBD_DESC_SIZE - unUlBdDepth.reg.ul_bdq_depth;
    *(g_stIpfUl.pu32IdleBd) = u32IdleBd;

    return u32IdleBd;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_UlStateIdle
*
* ��������  : �ýӿ����ڻ�ȡ����ͨ���Ƿ�Ϊ����
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS  ��ʾ���п��У������л�ģʽ
*             IPF_ERROR      ��ʾ���зǿ��У��������л�ģʽ
*
* �޸ļ�¼  :2011��12��9��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_UlStateIdle(BSP_VOID)
{
    BSP_U32 u32UlState = 0;

    IPF_REG_READ(SOC_IPF_CH0_STATE_ADDR(IPF_REGBASE_ADR), u32UlState);

    if(u32UlState != IPF_CH_IDLE)
    {
        return IPF_ERROR;
    }

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_PktLen
*
* ��������  : �ýӿ��������ù�������������С����
*
* �������  : BSP_U32 MaxLen   ������
*             BSP_U32 MinLen   ��С����
*
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS                �ɹ�
*             BSP_ERR_IPF_INVALID_PARA   ��������(����������С����С)
*
* �޸ļ�¼  :2011��2��17��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_SetPktLen(BSP_U32 u32MaxLen, BSP_U32 u32MinLen)
{
    SOC_IPF_PKT_LEN_UNION unPktLen;

    unPktLen.value = 0;
    /* ������� */
    if(u32MaxLen < u32MinLen)
    {
        IPF_DEBUG("BSP_IPF_SetPktLen:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    unPktLen.reg.max_pkt_len = u32MaxLen;
    unPktLen.reg.min_pkt_len = u32MinLen;

    IPF_REG_WRITE(SOC_IPF_PKT_LEN_ADDR(IPF_REGBASE_ADR), unPktLen.value);
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_GetStat
*
* ��������  : �ýӿڻ�ȡͳ�Ƽ�������ֵ
*
* �������  : IPF_FILTER_STAT_S *pstFilterStat ͳ�Ƽ����ṹ��ָ��
*
*
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS                �ɹ�
*             BSP_ERR_IPF_INVALID_PARA   ��������
*
* �޸ļ�¼  : 2011��2��17��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_GetStat(IPF_FILTER_STAT_S *pstFilterStat)
{
    /* ��׮ */
    return IPF_SUCCESS;
}
BSP_S32 IPF_WaitResume(BSP_VOID)
{
    BSP_U32 u32Times = 0;
    IPF_RESTORE_STATE_E IPF_Restore_flag = (IPF_RESTORE_STATE_E)0;


    IPF_Restore_flag= (IPF_RESTORE_STATE_E)(*(volatile BSP_U32*)IPF_PWRCTRL_RESTORE_FLAG_ADDR);

    /* ��ȡIPF״̬��ʶ */
    if(IPF_STATE_RESTORED == IPF_Restore_flag)
    {
        return IPF_SUCCESS;
    }

#ifdef PRODUCT_CFG_CORE_TYPE_MODEM
    /* ͨ������IPC�жϸ�֪MCU */
    if(IPF_STATE_UNRESTORE == IPF_Restore_flag)
    {
        BSP_IPC_IntSend(IPC_CORE_MCU, IPC_MCU_INT_SRC_CCPU_IPF);
    }
#endif

    /* �ȴ�IPF_Restore_flag����IPF_STATE_RESTORED������2ms��ʱ���� */
    do{
        IPF_Restore_flag = (IPF_RESTORE_STATE_E)(*(volatile BSP_U32*)IPF_PWRCTRL_RESTORE_FLAG_ADDR);
        udelay(10);
        u32Times++;
    }while((IPF_STATE_RESTORED != IPF_Restore_flag) && (u32Times < IPF_RESUME_COUNT));

    if(IPF_RESUME_COUNT == u32Times)
    {
        return IPF_ERROR;
    }

    return IPF_SUCCESS;
}
BSP_S32 IPF_ObligateAdqRpt(IPF_CHANNEL_TYPE_E eChId, IPF_AD_TYPE_E eType, BSP_U32 *pu32Rpt)
{
    BSP_U32 u32Index = 0;
    BSP_U32 u32StateValue = 0;
    BSP_U32 u32RptValue = 0;
    BSP_U32 u32RptOffsetValue = 0; 
    /* ADQ��ָ���״̬�Ĵ����� */
    IPF_ADQ_INFO_S stIpfAdqReg[4] = {
        {SOC_IPF_CH0_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), SOC_IPF_CH0_ADQ0_STAT_ADDR(IPF_REGBASE_ADR), IPF_ULAD0_DESC_SIZE},/*����/AD0*/
        {SOC_IPF_CH0_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), SOC_IPF_CH0_ADQ1_STAT_ADDR(IPF_REGBASE_ADR), IPF_ULAD1_DESC_SIZE},/*����/AD1*/
        {SOC_IPF_CH1_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), SOC_IPF_CH1_ADQ0_STAT_ADDR(IPF_REGBASE_ADR), IPF_DLAD0_DESC_SIZE},/*����/AD0*/
        {SOC_IPF_CH1_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), SOC_IPF_CH1_ADQ1_STAT_ADDR(IPF_REGBASE_ADR), IPF_DLAD1_DESC_SIZE},/*����/AD1*/    
    };

    /*  ��μ�� */
    if((IPF_CHANNEL_MAX <= eChId) || (IPF_AD_MAX <= eType) || (BSP_NULL == pu32Rpt))
    {
        IPF_DEBUG("IPF_ObligateAdqRpt:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ����ͨ��ID��AD���ͻ�ȡ�����Ĵ�������indexֵ */
    switch (eChId) {
        case IPF_CHANNEL_UP:
            u32Index = (IPF_AD_0 == eType) ? 0 : 1;
            break;
        case IPF_CHANNEL_DOWN:
            u32Index = (IPF_AD_0 == eType) ? 2 : 3;
            break;
        default:
            IPF_DEBUG("IPF_ObligateAdqRpt:para error\n");
            u32Index = 0;
            break;
    }

    /* ��ȡADQ״̬�Ĵ���/��ָ��Ĵ��� */        
    IPF_REG_READ(stIpfAdqReg[u32Index].u32StaReg, u32StateValue);
    IPF_REG_READ(stIpfAdqReg[u32Index].u32RptReg, u32RptValue);   

    /* ���ݶ�ȡ��״̬�Ĵ���/��ָ��Ĵ���ֵ���¼���Rpt */        
    if(u32StateValue & IPF_ADQ_BUF_EPT_MASK)
    {
        u32RptOffsetValue = 0;       
    }
    else if(u32StateValue & IPF_ADQ_BUF_FULL_MASK)
    {
        u32RptOffsetValue = 2; 
    }
    else
    {
        u32RptOffsetValue = 1;    
    }
    u32RptValue = (u32RptValue >= u32RptOffsetValue) ? (u32RptValue - u32RptOffsetValue) \
            : (stIpfAdqReg[u32Index].u32AqMaxNum + u32RptValue - u32RptOffsetValue);

    /* ����ADQ��ָ�� */
    *pu32Rpt = u32RptValue;    
    return IPF_SUCCESS;
}


BSP_VOID IPF_EXC_DUMP(BSP_U32 u32ErrNo)       
{
    /* ��¼IPF���мĴ��������� */
    memcpy((BSP_VOID*)g_32IpfDebugReg, (BSP_VOID*)IPF_REGBASE_ADR, sizeof(g_32IpfDebugReg));

    /* ����λ */
    BSP_MNTN_SystemError(BSP_MODU_IPF, u32ErrNo, 0, 0, 0);

    return;
}

#ifdef PRODUCT_CFG_CORE_TYPE_MODEM

BSP_S32 IPF_UsedList_Init(BSP_VOID)
{
    BSP_U32 i = 0;

    for(i = 0; i < IPF_FILTER_CHAIN_MAX_NUM; i++)
    {
        g_stIPFFilterInfo[i].u32FilterNum = 0;
        g_stIPFFilterInfo[i].pstUseList = NULL;
        IPF_DeleteAll((IPF_FILTER_CHAIN_TYPE_E)i);
    }

    return IPF_SUCCESS;
}

BSP_S32 IPF_BasicFreeList_Init(BSP_VOID)
{
    BSP_U32 i = 0;
    IPF_ID_S* prev = (IPF_ID_S*)BSP_NULL;
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;
    IPF_ID_S* tmp = (IPF_ID_S*)BSP_NULL;

    for(i = IPF_FILTER_CHAIN_MAX_NUM; i < IPF_BF_NUM; i++)
    {
        current = (IPF_ID_S*)malloc(sizeof(IPF_ID_S));
        if(BSP_NULL == current)
        {
            goto err;
        }

        if(g_stIPFBasicFreeList != BSP_NULL)
        {
            if(BSP_NULL != prev)
            {
                prev->pstNext = current;
            }
            else
            {
                free(current);
                current = (IPF_ID_S *)BSP_NULL;
                goto err;
            }
        }
        else /* basicfreelistͷ��� */
        {
            g_stIPFBasicFreeList = current;
        }
        current->u32FilterID = i;
        current->u32PsID = 0;
        current->pstNext = (IPF_ID_S*)BSP_NULL;
        prev = current;
    }

    return IPF_SUCCESS;

err:
    IPF_DEBUG("IPF_BasicFreeList_Init:error\n");
    while(g_stIPFBasicFreeList!= BSP_NULL)
    {
        tmp = g_stIPFBasicFreeList->pstNext;
        free(g_stIPFBasicFreeList);
        g_stIPFBasicFreeList = tmp;
    }

    return IPF_ERROR;
}
BSP_S32 IPF_ExtFreeList_Init(BSP_VOID)
{
    BSP_U32 i = 0;
    IPF_ID_S* prev = (IPF_ID_S*)BSP_NULL;
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;
    IPF_ID_S* tmp = (IPF_ID_S*)BSP_NULL;

    for(i =  IPF_BF_NUM; i < IPF_TOTAL_FILTER_NUM; i++)
    {
        current = (IPF_ID_S*)malloc(sizeof(IPF_ID_S));
        if(BSP_NULL == current)
        {
            goto err;
        }
        if(g_stIPFExtFreeList != BSP_NULL)
        {
            if(BSP_NULL != prev)
            {
                prev->pstNext = current;/*lint !e613*/
            }
            else
            {
                free(current);
                current = (IPF_ID_S *)BSP_NULL;
                goto err;
            }
        }
        else /* freelistͷ��� */
        {
            g_stIPFExtFreeList = current;
        }
        current->u32FilterID = i;
        current->u32PsID = 0;
        current->pstNext = (IPF_ID_S*)BSP_NULL;
        prev = current;
    }

    return IPF_SUCCESS;

err:
    IPF_DEBUG("IPF_ExtFreeList_Init:error\n");
    while(g_stIPFExtFreeList!= BSP_NULL)
    {
        tmp = g_stIPFExtFreeList->pstNext;
        free(g_stIPFExtFreeList);
        g_stIPFExtFreeList = tmp;
    }

    return IPF_ERROR;
}
/*****************************************************************************
* �� �� ��  : IPF_FilterList_Init
*
* ��������  : IPF�����������ʼ��     �ڲ�ʹ�ã�����Ϊ�ӿں���
*
* �������  : BSP_VOID
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS    �����������ʼ���ɹ�
*             IPF_ERROR      �����������ʼ��ʧ��
*
* �޸ļ�¼  :2011��11��17��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_FilterList_Init(BSP_VOID)
{
    /* ��ʼ������ͷ�ڵĵĹ�������������uselist����0-7*/
    if(IPF_ERROR == IPF_UsedList_Init())
        return IPF_ERROR;

    /* ��ʼ��basiclist  ��8�ſ�ʼ��0-7������ͷ������*/
    if(IPF_ERROR == IPF_BasicFreeList_Init())
        return IPF_ERROR;

    /* ��ʼ��extfreelist  ��8�ſ�ʼ��0-7������ͷ������*/
    if(IPF_ERROR == IPF_ExtFreeList_Init())
        return IPF_ERROR;

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : IPF_AddToFreeList
*
* ��������  : �����ӵ���Ӧ��freelist�ĺ���λ��
*
* �������  : IPF_ID_S** pstFreeList        ������Ľ��ָ��
*             IPF_ID_S* stNode             ������Ľ��ָ��
*
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_AddToFreeList(IPF_ID_S** pstFreeList, IPF_ID_S* stNode)
{
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;
    IPF_ID_S* prev = (IPF_ID_S*)BSP_NULL;

    if((BSP_NULL == stNode) || (BSP_NULL == pstFreeList))
    {
        IPF_DEBUG("IPF_AddToFreeList:para error\n");
        return;
    }

    current = *pstFreeList;
    /* ���freelistΪ�� */
    if(BSP_NULL == current)
    {
        *pstFreeList = stNode;
        (*pstFreeList)->pstNext = (IPF_ID_S*)BSP_NULL;
        return;
    }

    /* ����ȵ�һ������ID��С */
    if(stNode->u32FilterID < current->u32FilterID)
    {
        stNode->pstNext = current;
        (*pstFreeList) = stNode;
        return;
    }

    prev = (*pstFreeList);
    current = (*pstFreeList)->pstNext;

    while(current != BSP_NULL)
    {
        if(stNode->u32FilterID < current->u32FilterID)
        {
            prev->pstNext = stNode;
            stNode->pstNext = current;
            return;
        }
        prev = current;
        current = current->pstNext;
    }
    /* ������� */
    prev->pstNext = stNode;
    stNode->pstNext = (IPF_ID_S*)BSP_NULL;

}
BSP_VOID IPF_BackupMatchInfo(BSP_U32 u32FilterID)
{
    BSP_U32 u32RegAddr = SOC_IPF_FLT_LOCAL_ADDR0_ADDR(IPF_REGBASE_ADR);
    BSP_U32 u32RegNum = sizeof(IPF_MATCH_INFO_S) >> 2;
    BSP_U32* pu32MatchInfo = (BSP_U32*)(IPF_PWRCTRL_RULE_INFO_ADDR + u32FilterID * sizeof(IPF_MATCH_INFO_S));
    BSP_U32 i = 0;

    /*  ��μ�� */
    if((u32FilterID >= IPF_BF_NUM) || (IPF_NULL == pu32MatchInfo))
    {
        return;
    }

    /* ����������--�����������򱸷ݵ�DDR */
    IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), u32FilterID);
#if 0
    memcpy((BSP_VOID*)pu32MatchInfo, (BSP_VOID*)u32RegAddr, sizeof(IPF_MATCH_INFO_S));
#else
    for(i = 0;i < u32RegNum; i++)
    {
        IPF_REG_READ((u32RegAddr + (4 * i)), *pu32MatchInfo);
        pu32MatchInfo ++;
    }
#endif
}

/*****************************************************************************
* �� �� ��  : IPF_DeleteAll
*
* ��������  : ɾ�������е����н��
*
* �������  : IPF_FILTER_CHAIN_TYPE_E eFilterChainhead���������׵�ַ
*
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_DeleteAll(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead)
{
    IPF_ID_S* pstMove = (IPF_ID_S*)BSP_NULL;
    SOC_IPF_FLT_CHAIN_UNION unFltChain;
    SOC_IPF_FLT_RULE_CTRL_UNION unFltCtrl;

    while(IPF_FILTERCHAIN_HEAD_INDEX < g_stIPFFilterInfo[eFilterChainhead].u32FilterNum)
    {
        pstMove = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
        if(BSP_NULL == pstMove)
        {
            IPF_DEBUG("IPF_DeleteAll:null point\n");
            break;
        }

        /* ������uselistɾ�� */
        g_stIPFFilterInfo[eFilterChainhead].pstUseList  = pstMove->pstNext;

        /* ��ɾ���Ľ����ӵ�freelist�� */
        if(IPF_BF_NUM > pstMove->u32FilterID)
        {
            IPF_AddToFreeList(&g_stIPFBasicFreeList, pstMove);
        }
        else
        {
            IPF_AddToFreeList(&g_stIPFExtFreeList, pstMove);
        }
        g_stIPFFilterInfo[eFilterChainhead].u32FilterNum--;
    }

    /*���׹�������������Ϊ511��ʹ������Ϊ0*/
    IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), eFilterChainhead);
    unFltChain.value = 0x0;
    unFltChain.reg.flt_next_index = IPF_TAIL_INDEX;
    IPF_REG_WRITE(SOC_IPF_FLT_CHAIN_ADDR(IPF_REGBASE_ADR), unFltChain.value);
    unFltCtrl.value = 0x0;
    unFltCtrl.reg.flt_en = 0;
    IPF_REG_WRITE(SOC_IPF_FLT_RULE_CTRL_ADDR(IPF_REGBASE_ADR), unFltCtrl.value);
    IPF_BackupMatchInfo(eFilterChainhead);
}

/*****************************************************************************
* �� �� ��  : IPF_MallocOneFilter
*
* ��������  : ��freelist�з���һ�����(ȡ����һ�����)
*
* �������  : ��
*
* �������  : ��

* �� �� ֵ  : ���ָ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*
* ˵��      : �ɵ��ú�������֤һ���ܷ��䵽���
*****************************************************************************/
IPF_ID_S* IPF_MallocOneFilter(IPF_ID_S** g_stIPFFreeList)
{
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;

    if(BSP_NULL == g_stIPFFreeList)
    {
        IPF_DEBUG("IPF_MallocOneFilter:null point 1\n");
        return (IPF_ID_S*)BSP_NULL;
    }

    current = *g_stIPFFreeList;
    if(BSP_NULL == current)
    {
        IPF_DEBUG("IPF_MallocOneFilter:null point 2\n");
        return current;
    }
    *g_stIPFFreeList = (*g_stIPFFreeList)->pstNext;
    return current;
}

/*****************************************************************************
* �� �� ��  : IPF_AddTailUsedFilter
*
* ��������  : �����ӵ�uselist�Ľ�β
*
* �������  : BSP_U8 eFilterChainhead ͨ������
*             IPF_ID_S* stNode        ������Ľ��ָ��
*
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
                             2012��11��25�ճ¶����޸�
*****************************************************************************/
BSP_VOID IPF_AddTailUsedFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead,IPF_ID_S* stNode)
{
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;

    if(BSP_NULL == stNode)
    {
        IPF_DEBUG("IPF_AddTailUsedFilter:null point\n");
        return;
    }

    current = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
    if(BSP_NULL == current)
    {
        g_stIPFFilterInfo[eFilterChainhead].pstUseList = stNode;
        g_stIPFFilterInfo[eFilterChainhead].pstUseList->pstNext = (IPF_ID_S*)BSP_NULL;
        g_stIPFFilterInfo[eFilterChainhead].u32FilterNum = 1;
        return;
    }

    /* �����ӵ������β */
    while(current->pstNext != BSP_NULL)
    {
        current = current->pstNext;
    }
    current->pstNext = stNode;
    stNode->pstNext = (IPF_ID_S*)BSP_NULL;

    g_stIPFFilterInfo[eFilterChainhead].u32FilterNum++;

}

/*****************************************************************************
* �� �� ��  : IPF_AddTailFilterChain
*
* ��������  :�����������õ�Ӳ����
*
* �������  : BSP_U32 u32LastFilterID, BSP_U32 u32FilterID, IPF_MATCH_INFO_S* pstMatchInfo
*
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_AddTailFilterChain(BSP_U32 u32LastFilterID, BSP_U32 u32FilterID, IPF_MATCH_INFO_S* pstMatchInfo)
{
    SOC_IPF_FLT_CHAIN_UNION unValue;
    IPF_MATCH_INFO_S* pstLastMatchInfo = (IPF_MATCH_INFO_S*)BSP_NULL;
    BSP_U32 u32Spi = 0;

    if(BSP_NULL == pstMatchInfo)
    {
        IPF_DEBUG("IPF_AddTailFilterChain:null point\n");
        return;
    }

    unValue.value = 0;
    u32Spi = pstMatchInfo->u32FltSpi;
    pstMatchInfo->u32FltSpi = ((u32Spi&0xff000000)>>24) |
                              ((u32Spi&0x00ff0000)>>8) |
                              ((u32Spi&0x0000ff00)<<8) |
                              ((u32Spi&0x000000ff)<<24);
    /* ����������--���µĹ������������õ������� */
    if(u32FilterID < IPF_BF_NUM)
    {
        IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), u32FilterID);
        memcpy((BSP_VOID*)SOC_IPF_FLT_LOCAL_ADDR0_ADDR(IPF_REGBASE_ADR), pstMatchInfo, sizeof(IPF_MATCH_INFO_S));
        IPF_BackupMatchInfo(u32FilterID);
    }
    /* ��չ������ */
    else if((u32FilterID >= IPF_BF_NUM) && (u32FilterID < IPF_TOTAL_FILTER_NUM))
    {
        memcpy((BSP_VOID*)(g_pstExFilterAddr + u32FilterID - IPF_BF_NUM), pstMatchInfo, sizeof(IPF_MATCH_INFO_S));/* [false alarm]:��zhoushanȷ��û�����⣬���˵�  */
    }
    else
    {
        IPF_DEBUG("IPF_AddTailFilterChain:u32FilterID = %d error\n", u32FilterID);
        return;
    }

    /* ���ǵ�һ��filter����Ҫ������һ��filter��nextindex�� */
    if(u32LastFilterID != IPF_TAIL_INDEX)
    {
        /* ���ϴ��������һ����������nextIndex���������� */
        if(u32LastFilterID < IPF_BF_NUM)
        {
            IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), u32LastFilterID);
            IPF_REG_READ(SOC_IPF_FLT_CHAIN_ADDR(IPF_REGBASE_ADR), unValue.value);
            unValue.reg.flt_next_index = u32FilterID;
            IPF_REG_WRITE(SOC_IPF_FLT_CHAIN_ADDR(IPF_REGBASE_ADR), unValue.value);
            IPF_BackupMatchInfo(u32LastFilterID);
        }
        else if((u32LastFilterID >= IPF_BF_NUM) && (u32LastFilterID < IPF_TOTAL_FILTER_NUM))
        {
            pstLastMatchInfo = g_pstExFilterAddr + u32LastFilterID - IPF_BF_NUM;
            pstLastMatchInfo->unFltChain.Bits.u16NextIndex = u32FilterID;
        }
        else
        {
            IPF_DEBUG("IPF_AddTailFilterChain:u32LastFilterID = %d error\n", u32LastFilterID);
            return;
        }
    }

}

/*****************************************************************************
* �� �� ��  : IPF_FindFilterID
*
* ��������  : ��������������Ѱ����PS ID ƥ���Filter ID
*
* �������  : IPF_FILTER_CHAIN_TYPE_E eFilterChainhead    �����������
*                           BSP_U32 u32PsID                PS ID
*
* �������  : BSP_U32* u32FilterID   ��ѯ����Filter ID

* �� �� ֵ  : IPF_SUCCESS                ��ѯ�ɹ�
*             IPF_ERROR                  ��ѯʧ��
*
* �޸ļ�¼  :2011��1��11��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_FindFilterID(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32PsID, BSP_U32* u32FilterID)
{
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;

    if(BSP_NULL == u32FilterID)
    {
        IPF_DEBUG("IPF_FindFilterID:null point\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    current = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
    while(current != BSP_NULL)
    {
        if(current->u32PsID != u32PsID)
        {
            current = current->pstNext;
        }
        else
        {
            *u32FilterID = current->u32FilterID;
            return IPF_SUCCESS;
        }
    }

    return IPF_ERROR;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_Init
*
* ��������  : IPF����������ʼ��
*
* �������  : IPF_COMMON_PARA_S *pstCommPara
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS    ���óɹ�
*             BSP_ERR_IPF_NOT_INIT     δ��ʼ��
*             BSP_ERR_IPF_INVALID_PARA  ��������
*
* ˵��      : ����IPFȫ�ֿ������üĴ��� ����ѡ�����ȼ���������
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_Init(IPF_COMMON_PARA_S *pstCommPara)
{
    SOC_IPF_CTRL_UNION unIPFCtrl;

    /* ������� */
    if(BSP_NULL == pstCommPara)
    {
        IPF_DEBUG("BSP_IPF_Init:null point\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_Init:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    unIPFCtrl.value = 0;
    /* ����IPFȫ�ֿ������üĴ��� */
    unIPFCtrl.reg.filter_seq = pstCommPara->bFilterSeq;
    unIPFCtrl.reg.flt_addr_reverse = pstCommPara->bFltAddrReverse;
    unIPFCtrl.reg.sp_cfg = pstCommPara->bSpPriSel;
    unIPFCtrl.reg.sp_wrr_sel = pstCommPara->bSpWrrModeSel;
    unIPFCtrl.reg.max_burst_len = pstCommPara->eMaxBurst;
    unIPFCtrl.reg.ipv6_nh_sel = pstCommPara->bIpv6NextHdSel;
    unIPFCtrl.reg.ah_esp_sel = pstCommPara->bEspAhSel;
    unIPFCtrl.reg.ah_disable = pstCommPara->bAhSpiDisable;
    unIPFCtrl.reg.esp_disable = pstCommPara->bEspSpiDisable;
    unIPFCtrl.reg.mfc_en = pstCommPara->bMultiFilterChainEn;
    unIPFCtrl.reg.mm_en = pstCommPara->bMultiModeEn;
    unIPFCtrl.reg.add_rep = pstCommPara->bAdReport;

    IPF_REG_WRITE(SOC_IPF_CTRL_ADDR(IPF_REGBASE_ADR), unIPFCtrl.value);

    return  IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_ConfigUlChannel
*
* ��������  : ��������ͨ�����Ʋ���
*
* �������  : IPF_CHL_CTRL_S *pstCtrl
* �������  : ��
* �� �� ֵ  : ��
*
* ˵��      : ��������IPFͨ�����ƼĴ���
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigUlChannel(IPF_CHL_CTRL_S *pstCtrl)
{
    SOC_IPF_CH0_CTRL_UNION unChanCtrl;

    /* ������� */
    if(BSP_NULL == pstCtrl)
    {
        IPF_DEBUG("BSP_IPF_ConfigUlChannel:null point\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    unChanCtrl.value = 0;
    /*��������ͨ�����ƼĴ��� */
    unChanCtrl.reg.ul_mod = pstCtrl->eIpfMode;
    unChanCtrl.reg.ul_endian = pstCtrl->bEndian;
    unChanCtrl.reg.ul_data_chain = pstCtrl->bDataChain;
    unChanCtrl.reg.ul_wrr_value = pstCtrl->u32WrrValue;

    IPF_REG_WRITE(SOC_IPF_CH0_CTRL_ADDR(IPF_REGBASE_ADR), unChanCtrl.value);

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_ConfigDlChannel
*
* ��������  : ��������ͨ�����Ʋ���
*
* �������  : IPF_CHL_CTRL_S *pstCtrl
* �������  : ��
* �� �� ֵ  : ��
*
* ˵��      : ��������IPFͨ�����ƼĴ���
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigDlChannel(IPF_CHL_CTRL_S *pstCtrl)
{
    SOC_IPF_CH1_CTRL_UNION unChanCtrl;

    /* ������� */
    if(BSP_NULL == pstCtrl)
    {
        IPF_DEBUG("BSP_IPF_ConfigDlChannel:null point\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    unChanCtrl.value = 0;
    /*��������ͨ�����ƼĴ��� */
    unChanCtrl.reg.dl_mod = pstCtrl->eIpfMode;
    unChanCtrl.reg.dl_endian = pstCtrl->bEndian;
    unChanCtrl.reg.dl_data_chain = pstCtrl->bDataChain;
    unChanCtrl.reg.dl_wrr_value = pstCtrl->u32WrrValue;

    IPF_REG_WRITE(SOC_IPF_CH1_CTRL_ADDR(IPF_REGBASE_ADR), unChanCtrl.value);

    return IPF_SUCCESS;
}


BSP_S32 BSP_IPF_SetFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, IPF_FILTER_CONFIG_S *pstFilterInfo, BSP_U32 u32FilterNum)
{
    BSP_U32 i = 0;
    IPF_ID_S* current = (IPF_ID_S*)BSP_NULL;
    BSP_U32 u32used_filter_num = 0;
    BSP_U32 u32FirstFilterID = IPF_TAIL_INDEX;
    BSP_U32 u32LastFilterID = IPF_TAIL_INDEX;
    SOC_IPF_FLT_CHAIN_UNION unFltChain;

    /* ������� */
    if((eFilterChainhead >= IPF_FILTER_CHAIN_MAX_NUM) || (NULL == pstFilterInfo))
    {
        IPF_DEBUG("BSP_IPF_SetFilter:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_SetFilter:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    (BSP_VOID)OSAL_SemTake(g_ipfSetFltSem, OSAL_WAIT_FOREVER);

    /* ����üĴ������׵�ַ��������н�� */
    IPF_DeleteAll(eFilterChainhead);

    /* ��ʱ10us�ȴ�ipf������� */
    udelay(10);

    /* ���㱻ʹ�õĹ����������ܺͣ����ж��Ƿ����㹻�Ĺ����� */
    for(i = 0; i < IPF_FILTER_CHAIN_MAX_NUM; i++)
    {
        u32used_filter_num += g_stIPFFilterInfo[i].u32FilterNum;
    }
    if(u32FilterNum > (IPF_TOTAL_FILTER_NUM - IPF_FILTER_CHAIN_MAX_NUM - u32used_filter_num))
    {
        IPF_DEBUG("BSP_IPF_SetFilter:not enough\n");
        OSAL_SemGive(g_ipfSetFltSem);
        return BSP_ERR_IPF_FILTER_NOT_ENOUGH;
    }

    /* ȷ��IPF�ѻָ� */
    if(IPF_ERROR == IPF_WaitResume())
    {
        IPF_DEBUG("IPF_WaitResume:timeout\n");
        g_stIPFDebugInfo->u32UlResumeTimeoutCnt++;
        g_stIPFDebugInfo->u32DlResumeTimeoutCnt++;
        OSAL_SemGive(g_ipfSetFltSem);
        return  BSP_ERR_IPF_RESUME_TIMEOUT;
    }

    /* ��������� */
    for(i = 0; i < u32FilterNum; i++)
    {
        /* ��freelist��ȡ��δ���õĹ����� */
        if(IPF_FILTERTYPE_MASK > pstFilterInfo->u32FilterID)
        {
            /* �ȴӻ����������б������� */
            current = IPF_MallocOneFilter(&g_stIPFBasicFreeList);
            /* �������������㣬�����չ���������� */
            if(BSP_NULL == current)
            {
                current = IPF_MallocOneFilter(&g_stIPFExtFreeList);
            }
        }
        else
        {
            current = IPF_MallocOneFilter(&g_stIPFExtFreeList);
        }
        if(current == BSP_NULL)
        {
            IPF_DEBUG("IPF_MallocOneFilter:error\n");
            OSAL_SemGive(g_ipfSetFltSem);
            return IPF_ERROR;
        }

        /*���ֵ�һ��������ID*/
        if(0 == i)
        {
            u32FirstFilterID = current->u32FilterID;
        }
        /* ��ȡPS ID */
        current->u32PsID = (pstFilterInfo->u32FilterID)&IPF_FILTERTYPE_MASK;
        /* �Ƚ�NextIndex����Ϊ511 */
        pstFilterInfo->stMatchInfo.unFltChain.Bits.u16NextIndex = IPF_TAIL_INDEX;
        /* ����������Ϣ���õ�Ӳ���� */
        IPF_AddTailFilterChain(u32LastFilterID, current->u32FilterID, &pstFilterInfo->stMatchInfo);
        /* ���¸ù������������usrlist��Ϣ */
        IPF_AddTailUsedFilter(eFilterChainhead, current);
        /* ������һ�������õĹ�������Ϣ���������� */
        u32LastFilterID = current->u32FilterID;
        pstFilterInfo++;
    }

    /* �����úõĹ��������ҵ������� */
#ifndef _DRV_LLT_
    IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), eFilterChainhead);
    unFltChain.value = 0x0;
    unFltChain.reg.flt_next_index = u32FirstFilterID;
    IPF_REG_WRITE(SOC_IPF_FLT_CHAIN_ADDR(IPF_REGBASE_ADR), unFltChain.value);
	IPF_BackupMatchInfo(eFilterChainhead);
#endif
    OSAL_SemGive(g_ipfSetFltSem);
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_GetFilter
*
* ��������  : �ṩ��PS��ѯĳ��Filter������Ϣ
*
* �������  : BSP_U32 u32FilterID   Filter ID��
*
* �������  : IPF_FILTER_CONFIG_S * pFilterInfo  ��ѯ����Filter��Ϣ

* �� �� ֵ  : IPF_SUCCESS                ��ѯ�ɹ�
*             IPF_ERROR                  ��ѯʧ��
*             BSP_ERR_IPF_NOT_INIT       ģ��δ��ʼ��
*             BSP_ERR_IPF_INVALID_PARA   ��������
*
* �޸ļ�¼  : 2011��1��11��   ³��  ����
                             2011��3��30��   ³��  �޸�
                             2011    11��30��   ³��  �޸�
*****************************************************************************/
BSP_S32 BSP_IPF_GetFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32FilterID, IPF_FILTER_CONFIG_S *pstFilterInfo)
{
    BSP_U32 u32FindID = 0;

    /* ������� */
    if(NULL == pstFilterInfo)
    {
        IPF_DEBUG("BSP_IPF_GetFilter:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_GetFilter:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    /* ����PS ID ��Ӧ��FilterID*/
    if(IPF_FindFilterID(eFilterChainhead, u32FilterID, &u32FindID) != IPF_SUCCESS)
    {
        return IPF_ERROR;
    }
    pstFilterInfo->u32FilterID = u32FilterID;

    /* ����ǻ��������� */
    if(u32FindID < IPF_BF_NUM)
    {
        /* д���˱������ַ */
        IPF_REG_WRITE(SOC_IPF_BFLT_INDEX_ADDR(IPF_REGBASE_ADR), u32FindID);

        /* ���������������� */
        memcpy(&pstFilterInfo->stMatchInfo, (BSP_VOID*)SOC_IPF_FLT_LOCAL_ADDR0_ADDR(IPF_REGBASE_ADR), sizeof(IPF_MATCH_INFO_S));
    }
    /* ��չ������ */
    else
    {
        /* ���������������� */
        memcpy(&pstFilterInfo->stMatchInfo, (BSP_VOID*)(g_pstExFilterAddr + u32FindID - IPF_BF_NUM), sizeof(IPF_MATCH_INFO_S));
    }

    return IPF_SUCCESS;

}

/*****************************************************************************
* �� �� ��  : IPF_ConfigCD
*
* ��������  : ����CD
*
* �������  : TTF_MEM_ST *pstTtf, BSP_U16* pu16TotalDataLen, BSP_U32* pu32BdInPtr
* �������  : ��
* �� �� ֵ  :  ��
*
* �޸ļ�¼  :2011��1��24��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_ConfigCD(TTF_MEM_ST *pstTtf, BSP_U16* pu16TotalDataLen, BSP_U32* pu32BdInPtr)
{
    BSP_U32 u32DlCDWptr = 0;
    BSP_U32 u32DlCDRptr = 0;
    BSP_U16 u16TotalDataLen = 0;
    TTF_MEM_ST* p = (TTF_MEM_ST*)BSP_NULL;

    if((BSP_NULL == pstTtf) || (BSP_NULL == pu16TotalDataLen) || (BSP_NULL == pu32BdInPtr))
    {
        IPF_DEBUG("IPF_ConfigCD:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }        

    /* Ϊ��ֹBD �����ߺ�������Ŀ�ĵ�ַ���ɹ���������ʱcd��λ�� */
    u32DlCDWptr = *(g_stIpfDl.u32IpfCdWptr);
    u32DlCDRptr = *(g_stIpfDl.u32IpfCdRptr);
    *pu32BdInPtr = (BSP_U32)(g_stIpfDl.pstIpfCDQ + (u32DlCDWptr));

    /* ����CD */
    p = pstTtf;
    do
    {
        /* �ж��Ƿ��п���CD */
        if(u32DlCDRptr != (u32DlCDWptr+1)%IPF_DLCD_DESC_SIZE)
        {
            if(BSP_NULL == p)
            {
                IPF_DEBUG("IPF_ConfigCD:null point\n");
                return BSP_ERR_IPF_INVALID_PARA;
            }        
            g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u32Ptr = (BSP_U32)p->pData;
            g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16PktLen = p->usUsed;
        #ifdef __BSP_IPF_DEBUG__
            g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u32Ptr = (BSP_U32)p->pData;
            g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16PktLen = p->usUsed;
            if(g_stIPFDebugInfo->u32IpfDebug)
            {
                IPF_DEBUG("IPF_ConfigCD:usUsed = %d  pData = 0x%x\n", p->usUsed, p->pData);
            }
        #endif
            u16TotalDataLen += p->usUsed;

            /* ��ʶ�Ƿ���� */
            if(p->pNext != BSP_NULL)
            {
                g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16Attribute = 0;
            #ifdef __BSP_IPF_DEBUG__
                g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16Attribute = 0;
            #endif
                if(u32DlCDWptr+1 < IPF_DLCD_DESC_SIZE)
                {
                    u32DlCDWptr += 1;
                    p = p->pNext;
                }
                /* δ����ʱ��CD���ܷ�ת�����Ǽ�¼CD��β����ͬʱ����0��ʼ����CD */
                else
                {
                    u32DlCDWptr = 0;
                    u16TotalDataLen = 0;
                    p = pstTtf;
                    *pu32BdInPtr = (BSP_U32)g_stIpfDl.pstIpfCDQ;
                #ifdef __BSP_IPF_DEBUG__
                    if(g_stIPFDebugInfo->u32IpfDebug)
                    {
                        IPF_DEBUG("IPF_ConfigCD:CD ��ת\n");
                    }
                #endif
                }
            }
            else
            {
                g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16Attribute = 1;
            #ifdef __BSP_IPF_DEBUG__
                g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16Attribute = 1;
            #endif

                /* ����ʱCD���Է�ת */
                u32DlCDWptr = (u32DlCDWptr+1 < IPF_DLCD_DESC_SIZE)?(u32DlCDWptr+1):0;
            #ifdef __BSP_IPF_DEBUG__
                if(g_stIPFDebugInfo->u32IpfDebug)
                {
                    IPF_DEBUG("IPF_ConfigCD:u32DlCDWptr  = %d\n", u32DlCDWptr);
                }
            #endif
                break;
            }
        }
        else
        {
            IPF_DEBUG("IPF_ConfigCD:DownFilter CD FULL ! u32DlCDRptr=%d u32DlCDWptr=%d\n", u32DlCDRptr,u32DlCDWptr);
            return BSP_ERR_IPF_CDQ_NOT_ENOUGH;
        }
    }while(p != BSP_NULL);

    *(g_stIpfDl.u32IpfCdWptr) = u32DlCDWptr;
    *pu16TotalDataLen = u16TotalDataLen;

    return IPF_SUCCESS;

}

BSP_S32 BSP_IPF_ConfigDownFilter(BSP_U32 u32Num, IPF_CONFIG_DLPARAM_S* pstDlPara)
{
    BSP_U32 i = 0;
    BSP_U32 u32BdqWptr = 0;
    BSP_U32 u32BdInPtr = 0;
    BSP_U16 u16TotalDataLen = 0;
#ifdef __BSP_IPF_DEBUG__
    SOC_IPF_CH1_ADQ_CTRL_UNION unCh1AdqCtrl;
#endif

    if(BSP_NULL == pstDlPara)
    {
        IPF_DEBUG("BSP_IPF_ConfigDownFilter:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_ConfigDownFilter:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

#ifdef __BSP_IPF_DEBUG__
    if(u32Num > g_stIpfDl.u32IdleBd)
    {
        g_stIPFDebugInfo->u32DlBdNotEnough++;
        return IPF_ERROR;
    }

    IPF_REG_READ(SOC_IPF_CH1_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), unCh1AdqCtrl.value);
    if(unCh1AdqCtrl.reg.dl_adq_en != IPF_BOTH_ADQ_EN)
    {
        IPF_DEBUG("BSP_IPF_ConfigDownFilter:adq not support\n");
        return IPF_ERROR;
    }

    g_stIPFDebugInfo->u32DlBdNum += u32Num;            
#endif

    /* ȷ��IPF�ѻָ� */
    if(IPF_ERROR == IPF_WaitResume())
    {
        IPF_DEBUG("IPF_WaitResume:timeout\n");
        g_stIPFDebugInfo->u32DlResumeTimeoutCnt++;
        return  BSP_ERR_IPF_RESUME_TIMEOUT;
    }

    /* ����BDдָ��,��u32BdqWptr��Ϊ��ʱдָ��ʹ�� */
    IPF_REG_READ(SOC_IPF_CH1_BDQ_WPTR_ADDR(IPF_REGBASE_ADR), u32BdqWptr);

    for(i = 0; i < u32Num; i++)
    {
        /* ����CD */
        if(IPF_ConfigCD((TTF_MEM_ST *)pstDlPara[i].u32Data, &u16TotalDataLen, &u32BdInPtr) != IPF_SUCCESS)
        {
        #ifdef __BSP_IPF_DEBUG__
            g_stIPFDebugInfo->u32DlCdNotEnough++;
        #endif
            return BSP_ERR_IPF_CDQ_NOT_ENOUGH;
        }
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u16Attribute = pstDlPara[i].u16Attribute;
        if(0 == u32BdInPtr)
        {
            IPF_DEBUG("BSP_IPF_ConfigDownFilter:u32BdInPtr null point\n");
            return BSP_ERR_IPF_INVALID_PARA;
        }        
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u32InPtr =  u32BdInPtr;
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u16PktLen = u16TotalDataLen;
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u16UsrField1 = pstDlPara[i].u16UsrField1;
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u32UsrField2 = pstDlPara[i].u32UsrField2;
        g_stIpfDl.pstIpfBDQ[u32BdqWptr].u32UsrField3 = pstDlPara[i].u32UsrField3;
    #ifdef __BSP_IPF_DEBUG__
        if(g_stIPFDebugInfo->u32IpfDebug)
        {
            IPF_DEBUG("BSP_IPF_ConfigDownFilter:%d  %x, %d\n", u32BdqWptr,
                g_stIpfDl.pstIpfBDQ[u32BdqWptr].u32UsrField2,g_stIpfDl.pstIpfBDQ[u32BdqWptr].u32UsrField3);
        }
    #endif
        u32BdqWptr = ((u32BdqWptr + 1) < IPF_DLBD_DESC_SIZE)? (u32BdqWptr + 1) : 0;
    }

    /* ����BDдָ�� */
    IPF_REG_WRITE(SOC_IPF_CH1_BDQ_WPTR_ADDR(IPF_REGBASE_ADR), u32BdqWptr);

    return IPF_SUCCESS;
}
/*****************************************************************************
* �� �� ��  : BSP_IPF_ChannelEnable
*
* ��������  : �ýӿ�����ʹ�ܻ�λIP������������ͨ��
*
* �������  : IPF_CHANNEL_TYPE_E eChanType      ������ͨ����ʶ
*             BSP_BOOL bFlag   ʹ�ܸ�λ��ʶ
*
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS                ʹ�ܸ�λ�ɹ�
*             IPF_ERROR                  ʹ�ܸ�λʧ��
*             BSP_ERR_IPF_INVALID_PARA   ��������
*
* �޸ļ�¼  :2011��1��11��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ChannelEnable(IPF_CHANNEL_TYPE_E eChanType, BSP_BOOL bFlag)
{
    BSP_U32 u32ChanEnable = 0;
    BSP_U32 u32ChanState = 0;
    BSP_U32 u32Times = 0;
    BSP_U32 u32ChCtrl = 0;
    BSP_U32 u32ChCtrlReg = 0;

    /* ������� */
    if((eChanType >= IPF_CHANNEL_MAX) || (eChanType < IPF_CHANNEL_UP))
    {
        IPF_DEBUG("BSP_IPF_ChannelEnable:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ͨ��ʹ�� */
    if(bFlag)
    {
        IPF_REG_READ(SOC_IPF_CH_EN_ADDR(IPF_REGBASE_ADR), u32ChanEnable);
        u32ChanEnable |= 0x1<<eChanType;
        IPF_REG_WRITE(SOC_IPF_CH_EN_ADDR(IPF_REGBASE_ADR), u32ChanEnable);
    }
    /* ͨ��ȥʹ�� */
    else
    {
        IPF_REG_READ(SOC_IPF_CH_EN_ADDR(IPF_REGBASE_ADR), u32ChanEnable);
        u32ChanEnable &= 0xFFFFFFFF ^ (0x1<<eChanType);
        IPF_REG_WRITE(SOC_IPF_CH_EN_ADDR(IPF_REGBASE_ADR), u32ChanEnable);

        /* ����ֱ��ͨ��״̬Ϊ0 */
        do
        {
            /*(BSP_VOID)taskDelay(1);*/
            IPF_REG_READ(SOC_IPF_EN_STATE_ADDR(IPF_REGBASE_ADR), u32ChanState);
            u32ChanState &= 0x1<<eChanType;
            u32Times++;
        }while((u32ChanState) && (u32Times < 100));

        if(100 == u32Times)
        {
            IPF_DEBUG("BSP_IPF_ChannelEnable:timeout\n");
            return IPF_ERROR;
        }

        /* ��λ������к�������еĶ�дָ�� */
        if((IPF_CHANNEL_UP == eChanType))
            u32ChCtrlReg = SOC_IPF_CH0_CTRL_ADDR(IPF_REGBASE_ADR);
        else
            u32ChCtrlReg = SOC_IPF_CH1_CTRL_ADDR(IPF_REGBASE_ADR);
        IPF_REG_READ(u32ChCtrlReg, u32ChCtrl);
        u32ChCtrl |= 0x30; /* ��4��5bit����Ϊ1 */
        IPF_REG_WRITE(u32ChCtrlReg, u32ChCtrl);
    }


    return IPF_SUCCESS;
}
BSP_U32 BSP_IPF_GetDlDescNum(BSP_VOID)
{
    BSP_U32 u32DlAd0Num = 0;
    BSP_U32 u32DlAd1Num = 0;
    BSP_U32 u32DlBdNum = 0;
    BSP_U32 u32DlAdwptr = 0;
    BSP_U32 u32DlAdrptr = 0;
    BSP_U32 u32DlBdDepth = 0;
    BSP_U32 u32DlCdNum = 0;
    SOC_IPF_CH1_DQ_DEPTH_UNION unDQDepth;

    /* ����BD��� */
    unDQDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH1_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unDQDepth.value);
    u32DlBdDepth = unDQDepth.reg.dl_bdq_depth;

    /* �������AD0���� */
    IPF_REG_READ(SOC_IPF_CH1_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32DlAdwptr);
    IPF_REG_READ(SOC_IPF_CH1_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), u32DlAdrptr);

    /*дָ����ǰ������˳��*/
    if (u32DlAdwptr >= u32DlAdrptr)
    {
        u32DlAd0Num = u32DlAdwptr - u32DlAdrptr;
    }
    else
    {
        u32DlAd0Num = IPF_DLAD0_DESC_SIZE - (u32DlAdrptr - u32DlAdwptr);
    }
    if(u32DlAd0Num > u32DlBdDepth)
    {
        u32DlAd0Num -= u32DlBdDepth;
    }
    else
    {
        u32DlAd0Num = 0;
        g_stIPFDebugInfo->u32DlAd0NotEnough++;
    }

    /* �������AD1���� */
    IPF_REG_READ(SOC_IPF_CH1_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32DlAdwptr);
    IPF_REG_READ(SOC_IPF_CH1_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), u32DlAdrptr);

    /*дָ����ǰ������˳��*/
    if (u32DlAdwptr >= u32DlAdrptr)
    {
        u32DlAd1Num = u32DlAdwptr - u32DlAdrptr;
    }
    else
    {
        u32DlAd1Num =  IPF_DLAD1_DESC_SIZE - (u32DlAdrptr - u32DlAdwptr);
    }

    if(u32DlAd1Num > u32DlBdDepth)
    {
        u32DlAd1Num -= u32DlBdDepth;
    }
    else
    {
        u32DlAd1Num = 0;
        g_stIPFDebugInfo->u32DlAd1NotEnough++;
    }

    u32DlBdNum = BSP_IPF_GetDlBDNum(&u32DlCdNum);
    if(0 == u32DlBdNum)
    {
        g_stIPFDebugInfo->u32DlBdNotEnough++;
    }

    if(u32DlBdNum > u32DlAd0Num)
    {
        u32DlBdNum = u32DlAd0Num;
    }

    if(u32DlBdNum > u32DlAd1Num)
    {
        u32DlBdNum = u32DlAd1Num;
    }

    return u32DlBdNum;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_GetDlBDNum
*
* ��������  : �ýӿ����ڻ�ȡ���п���BD��CD ��Ŀ
*             BD ��Χ: 0~64      CD��Χ: 0~64*4
* �������  : ��
*
* �������  : ����CD��Ŀ
* �� �� ֵ  : ����BD��Ŀ
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetDlBDNum(BSP_U32* pu32CDNum)
{
    SOC_IPF_CH1_DQ_DEPTH_UNION unDQDepth;
    BSP_U32 u32DlCDWptr = 0;
    BSP_U32 u32DlCDRptr = 0;
    BSP_U32 u32IdleBd = 0;
    BSP_U32 u32IdleCd = 0;
    BSP_U32 u32IdleCdDown = 0;
    BSP_U32 u32IdleCdUp = 0;

    if(BSP_NULL == pu32CDNum)
    {
        IPF_DEBUG("BSP_IPF_GetDlBDNum:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    u32DlCDWptr = *(g_stIpfDl.u32IpfCdWptr);
    u32DlCDRptr = *(g_stIpfDl.u32IpfCdRptr);
    /* �������BD���� */
    unDQDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH1_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unDQDepth.value);
    u32IdleBd = IPF_DLBD_DESC_SIZE - unDQDepth.reg.dl_bdq_depth;

    /* �������CD ���� */
    if(u32DlCDRptr > u32DlCDWptr)
    {
        u32IdleCd = u32DlCDRptr - u32DlCDWptr - 1;
    }
    else
    {
        u32IdleCdUp = (u32DlCDRptr > 1)?(u32DlCDRptr - 1):0;
        u32IdleCdDown = IPF_DLCD_DESC_SIZE -  u32DlCDWptr - 1;
        u32IdleCd = (u32IdleCdUp > u32IdleCdDown)? u32IdleCdUp:u32IdleCdDown;
    }
    g_stIpfDl.u32IdleBd = u32IdleBd;
    *pu32CDNum = u32IdleCd;

    return u32IdleBd;
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_DlStateIdle
*
* ��������  : �ýӿ����ڻ�ȡ����ͨ���Ƿ�Ϊ����
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS  ��ʾ���п��У������л�ģʽ
*             IPF_ERROR    ��ʾ���зǿ��У��������л�ģʽ
*
* �޸ļ�¼  :2011��12��9��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_DlStateIdle(BSP_VOID)
{
    BSP_U32 u32DlState = 0;

    IPF_REG_READ(SOC_IPF_CH1_STATE_ADDR(IPF_REGBASE_ADR), u32DlState);

    if(u32DlState != IPF_CH_IDLE)
    {
        return IPF_ERROR;
    }
    return IPF_SUCCESS;
}
/*****************************************************************************
* �� �� ��  : BSP_IPF_RegisterWakeupUlCb
*
* ��������  : �ýӿ�����ע������PS����ص�����
*
* �������  : BSP_IPF_WakeupUlCb *pFnWakeupUl
*
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterWakeupUlCb(BSP_IPF_WakeupUlCb pFnWakeupUl)
{
    /* ������� */
    if(BSP_NULL == pFnWakeupUl)
    {
        IPF_DEBUG("BSP_IPF_RegisterWakeupUlCb:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfUl.pFnUlIntCb = pFnWakeupUl;
    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_RegisterAdqEmptyUlCb(BSP_IPF_AdqEmptyUlCb pAdqEmptyUl)
{
    /* ������� */
    if(BSP_NULL == pAdqEmptyUl)
    {
        IPF_DEBUG("BSP_IPF_RegisterAdqEmptyUlCb:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfUl.pAdqEmptyUlCb = pAdqEmptyUl;
    return IPF_SUCCESS;
}
/*****************************************************************************
* �� �� ��  : BSP_IPF_GetUlRd
*
* ��������  : �ýӿ����ڶ�ȡ����RD, ֧��һ�ζ�ȡ���RD
*
* �������  : BSP_U32* pu32Num
*             IPF_RD_DESC_S *pstRd
*
* �������  : BSP_U32* pu32Num    ʵ�ʶ�ȡ��RD��Ŀ
*
* �� �� ֵ  : IPF_SUCCESS               �����ɹ�
*             IPF_ERROR                 ����ʧ��
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_VOID BSP_IPF_GetUlRd(BSP_U32* pu32Num, IPF_RD_DESC_S *pstRd)
{
    SOC_IPF_CH0_DQ_DEPTH_UNION unDqDepth;
    BSP_U32 u32RdqRptr = 0;
    BSP_U32 u32RdqDepth = 0;
    BSP_U32 u32Num = 0;
    BSP_U32 i = 0;

    /* ������� */
    if((BSP_NULL == pu32Num) || (BSP_NULL == pstRd))
    {
        IPF_DEBUG("BSP_IPF_GetUlRd:para error\n");
        return;
    }

    /* ��ȡRD��� */
    unDqDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH0_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unDqDepth.value);
    u32RdqDepth = unDqDepth.reg.ul_rdq_depth;

    u32Num = (u32RdqDepth < *pu32Num)?u32RdqDepth:*pu32Num;

    if(u32Num > 0)
    {
        g_stIPFDebugInfo->u32UlRdNum += u32Num;            
        /* ��ȡRD��ָ�� */
        IPF_REG_READ(SOC_IPF_CH0_RDQ_RPTR_ADDR(IPF_REGBASE_ADR), u32RdqRptr);
        if(u32RdqRptr >= IPF_ULRD_DESC_SIZE)
        {
            IPF_DEBUG("BSP_IPF_GetUlRd:u32RdqRptr = %d\n", u32RdqRptr);
        }
        for(i = 0; i < u32Num; i++)
        {
            /* ��ȡRD */
            pstRd[i].u16Attribute = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16Attribute;
            pstRd[i].u16PktLen = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16PktLen;
            pstRd[i].u16Result = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16Result;
            pstRd[i].u32InPtr = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32InPtr;
            pstRd[i].u32OutPtr = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32OutPtr;
            if((BSP_NULL == pstRd[i].u32OutPtr)/* || (0x55555555 == pstRd[i].u32OutPtr) || (0xAAAAAAAA == pstRd[i].u32OutPtr)*/)
            {
                IPF_DEBUG("BSP_IPF_GetUlRd:invalid ul rd\n");
                IPF_EXC_DUMP(BSP_ERR_IPF_INVALID_ULRD);
            }                        
            pstRd[i].u16UsrField1 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16UsrField1;
            pstRd[i].u32UsrField2 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32UsrField2;
            pstRd[i].u32UsrField3 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32UsrField3;

            u32RdqRptr = ((u32RdqRptr+1) < IPF_ULRD_DESC_SIZE)?(u32RdqRptr+1):0;
        }
        /* ����RD��ָ�� */
        IPF_REG_WRITE(SOC_IPF_CH0_RDQ_RPTR_ADDR(IPF_REGBASE_ADR), u32RdqRptr);
    }
    *pu32Num = u32Num;
}

BSP_S32 BSP_IPF_ConfigADThr(BSP_U32 u32UlADThr,BSP_U32 u32DlADThr)
{
    SOC_IPF_CH0_ADQ_CTRL_UNION u32UlADCtrl;
    SOC_IPF_CH1_ADQ_CTRL_UNION u32DlADCtrl;

    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_ConfigADThr:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    if((u32UlADThr == 0)  || (u32UlADThr > 0xFFFF) || (u32DlADThr == 0)  || (u32DlADThr > 0xFFFF))
    {
        IPF_DEBUG("BSP_IPF_ConfigADThr:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ��������ͨ����AD��ֵ*/
    u32UlADCtrl.value = 0;
    IPF_REG_READ(SOC_IPF_CH0_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32UlADCtrl.value);
    u32UlADCtrl.reg.ul_adq_plen_th = u32UlADThr;
    IPF_REG_WRITE(SOC_IPF_CH0_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32UlADCtrl.value);

    /* ��������ͨ����AD��ֵ*/
    u32DlADCtrl.value = 0;
    IPF_REG_READ(SOC_IPF_CH1_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32DlADCtrl.value);
    u32DlADCtrl.reg.dl_adq_plen_th = u32DlADThr;
    IPF_REG_WRITE(SOC_IPF_CH1_ADQ_CTRL_ADDR(IPF_REGBASE_ADR), u32DlADCtrl.value);

    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_GetUlAdNum(BSP_U32* pu32AD0Num, BSP_U32* pu32AD1Num)
{
    BSP_U32 u32UlAd0Depth = 0;
    BSP_U32 u32UlAd1Depth = 0;
    BSP_U32 u32UlAd0wptr = 0;
    BSP_U32 u32UlAd0rptr = 0;
    BSP_U32 u32UlAd1wptr = 0;
    BSP_U32 u32UlAd1rptr = 0;
    BSP_U32 u32AD0Num = 0;
    BSP_U32 u32AD1Num = 0;    

    /*��μ��*/
    if((NULL == pu32AD0Num)||(NULL == pu32AD1Num))
    {
        IPF_DEBUG("BSP_IPF_GetUlAdNum:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* �������AD0���� */
    IPF_REG_READ(SOC_IPF_CH0_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32UlAd0wptr);
    IPF_REG_READ(SOC_IPF_CH0_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), u32UlAd0rptr);
    if (u32UlAd0wptr >= u32UlAd0rptr)/*дָ����ǰ������˳��*/
        u32UlAd0Depth = IPF_ULAD0_DESC_SIZE - (u32UlAd0wptr - u32UlAd0rptr+1);
    else
        u32UlAd0Depth = u32UlAd0rptr - u32UlAd0wptr - 1;

    u32AD0Num = (u32UlAd0Depth > IPF_OBLIGATE_AD_NUM) ? (u32UlAd0Depth - IPF_OBLIGATE_AD_NUM) : 0;
    if(u32AD0Num > IPF_ULAD0_DESC_SIZE)
    {
        IPF_DEBUG("BSP_IPF_GetUlAdNum:ad0 num error\n");
        return BSP_ERR_IPF_INVALID_PARA;    
    }
    *pu32AD0Num = u32AD0Num;

    /* �������AD1���� */
    IPF_REG_READ(SOC_IPF_CH0_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32UlAd1wptr);
    IPF_REG_READ(SOC_IPF_CH0_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), u32UlAd1rptr);
    if (u32UlAd1wptr >= u32UlAd1rptr)/*дָ����ǰ������˳��*/
        u32UlAd1Depth = IPF_ULAD1_DESC_SIZE - (u32UlAd1wptr - u32UlAd1rptr+1);
    else
        u32UlAd1Depth = u32UlAd1rptr - u32UlAd1wptr - 1;

    u32AD1Num = (u32UlAd1Depth > IPF_OBLIGATE_AD_NUM) ? (u32UlAd1Depth - IPF_OBLIGATE_AD_NUM) : 0;
    if(u32AD1Num > IPF_ULAD1_DESC_SIZE)
    {
        IPF_DEBUG("BSP_IPF_GetUlAdNum:ad1 num error\n");
        return BSP_ERR_IPF_INVALID_PARA;    
    }
    *pu32AD1Num = u32AD1Num;

    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_ConfigUlAd(IPF_AD_TYPE_E eAdType, BSP_U32 u32AdNum, IPF_AD_DESC_S *pstAdDesc)
{
    BSP_U32 i = 0;
    BSP_U32 u32ADQ0wptr;
    BSP_U32 u32ADQ1wptr;

    /* ��μ�� */
    if((IPF_AD_MAX <= eAdType) || (NULL == pstAdDesc))
    {
        IPF_DEBUG("BSP_IPF_ConfigUlAd:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_ConfigUlAd:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }
    if(IPF_AD_0 == eAdType)
    {
        g_stIPFDebugInfo->u32UlAdq0Num += u32AdNum;    
        /* ��ȡADQ0дָ�� */
        IPF_REG_READ(SOC_IPF_CH0_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ0wptr);

        /* ��ȡĿ�Ľṹ��ָ���Ŀ�Ľṹ������ָ�룬���뵽ADQ�� */
        for(i=0; i < u32AdNum; i++)
        {
            if((BSP_NULL == pstAdDesc[i].u32OutPtr0) || (BSP_NULL == pstAdDesc[i].u32OutPtr1))
            {
                IPF_DEBUG("BSP_IPF_ConfigUlAd:ad0 null point\n");
                return BSP_ERR_IPF_INVALID_PARA;
            }
            g_stIpfUl.pstIpfADQ0[u32ADQ0wptr].u32OutPtr0 = pstAdDesc[i].u32OutPtr0;
            g_stIpfUl.pstIpfADQ0[u32ADQ0wptr].u32OutPtr1 = pstAdDesc[i].u32OutPtr1;
            u32ADQ0wptr = ((u32ADQ0wptr + 1) < IPF_ULAD0_DESC_SIZE)? (u32ADQ0wptr + 1) : 0;
        }
        /* ����ADQ0дָ�� */
        IPF_REG_WRITE(SOC_IPF_CH0_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ0wptr);
    }
    else
    {
        g_stIPFDebugInfo->u32UlAdq1Num += u32AdNum;        
        /* ��ȡADQ1дָ�� */
        IPF_REG_READ(SOC_IPF_CH0_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ1wptr);

        /* ��ȡĿ�Ľṹ��ָ���Ŀ�Ľṹ������ָ�룬���뵽ADQ�� */
        for(i=0; i < u32AdNum; i++)
        {
            if((BSP_NULL == pstAdDesc[i].u32OutPtr0) || (BSP_NULL == pstAdDesc[i].u32OutPtr1))
            {
                IPF_DEBUG("BSP_IPF_ConfigUlAd:ad1 null point\n");
                return BSP_ERR_IPF_INVALID_PARA;
            }        
            g_stIpfUl.pstIpfADQ1[u32ADQ1wptr].u32OutPtr0 = pstAdDesc[i].u32OutPtr0;
            g_stIpfUl.pstIpfADQ1[u32ADQ1wptr].u32OutPtr1 = pstAdDesc[i].u32OutPtr1;
            u32ADQ1wptr = ((u32ADQ1wptr + 1) < IPF_ULAD1_DESC_SIZE)? (u32ADQ1wptr + 1) : 0;
        }
        /* ����ADQ1дָ�� */
        IPF_REG_WRITE(SOC_IPF_CH0_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ1wptr);
    }
    return IPF_SUCCESS;
}
#else

BSP_VOID BSP_IPF_DlRegReInit(BSP_VOID)
{
    /*��׮*/
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_RegisterWakeupDlCb
*
* ��������  : �ýӿ�����ע������PS����ص�����
*
* �������  : BSP_IPF_WakeupDlkCb *pFnWakeupDl
*
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterWakeupDlCb(BSP_IPF_WakeupDlCb pFnWakeupDl)
{
    /* ������� */
    if(BSP_NULL == pFnWakeupDl)
    {
        IPF_DEBUG("BSP_IPF_RegisterWakeupDlCb:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfDl.pFnDlIntCb = pFnWakeupDl;
    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_RegisterAdqEmptyDlCb(BSP_IPF_AdqEmptyDlCb pAdqEmptyDl)
{
    /* ������� */
    if(BSP_NULL == pAdqEmptyDl)
    {
        IPF_DEBUG("BSP_IPF_RegisterAdqEmptyDlCb:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfDl.pAdqEmptyDlCb = pAdqEmptyDl;
    return IPF_SUCCESS;
}
BSP_U32 BSP_IPF_GetUlRdNum(BSP_VOID)
{
    SOC_IPF_CH0_DQ_DEPTH_UNION unRdqDepth;

    /* ��ȡ����RD��ȼĴ��� */
    unRdqDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH0_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unRdqDepth.value);

    return unRdqDepth.reg.ul_rdq_depth;
}
BSP_U32 BSP_IPF_GetDlRdNum(BSP_VOID)
{
    SOC_IPF_CH1_DQ_DEPTH_UNION unRdqDepth;

    /* ��ȡ����RD��ȼĴ��� */
    unRdqDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH1_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unRdqDepth.value);

    return unRdqDepth.reg.dl_rdq_depth;
}
BSP_VOID BSP_IPF_GetDlRd(BSP_U32* pu32Num, IPF_RD_DESC_S *pstRd)
{
    SOC_IPF_CH1_DQ_DEPTH_UNION unDqDepth;
    BSP_U32 u32CdqRptr = 0;
    BSP_U32 u32RdqRptr = 0;
    BSP_U32 u32RdqDepth = 0;
    BSP_U32 u32Num = 0;
    BSP_U32 i = 0;

    if((BSP_NULL == pu32Num) || (BSP_NULL == pstRd))
    {
        IPF_DEBUG("BSP_IPF_GetDlRd:para error\n");
        return;
    }                        

    /* ��ȡRD��� */
    unDqDepth.value = 0;
    IPF_REG_READ(SOC_IPF_CH1_DQ_DEPTH_ADDR(IPF_REGBASE_ADR), unDqDepth.value);
    u32RdqDepth = unDqDepth.reg.dl_rdq_depth;

    u32Num = (u32RdqDepth < *pu32Num)?u32RdqDepth:*pu32Num;
    if(u32Num > 0)
    {
        g_stIPFDebugInfo->u32DlRdNum += u32Num;        
        for(i = 0; i < u32Num; i++)
        {
            /* ��ȡRD��ָ�� */
            IPF_REG_READ(SOC_IPF_CH1_RDQ_RPTR_ADDR(IPF_REGBASE_ADR), u32RdqRptr);
            if(u32RdqRptr >= IPF_DLRD_DESC_SIZE)
            {
                IPF_DEBUG("BSP_IPF_GetDlRd:u32RdqRptr = %d\n", u32RdqRptr);
            }
                
            /* ��ȡRD */
            pstRd[i].u16Attribute = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16Attribute;
            pstRd[i].u16PktLen = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16PktLen;
            pstRd[i].u16Result = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16Result;
            pstRd[i].u32InPtr = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32InPtr;
            pstRd[i].u32OutPtr = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32OutPtr;
            if((BSP_NULL == (void *)pstRd[i].u32OutPtr)/* || (0x55555555 == pstRd[i].u32OutPtr) || (0xAAAAAAAA == pstRd[i].u32OutPtr)*/)
            {
                IPF_DEBUG("BSP_IPF_GetDlRd:invalid dl rd\n");
                IPF_EXC_DUMP(BSP_ERR_IPF_INVALID_DLRD);
            }                        
            pstRd[i].u16UsrField1 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16UsrField1;
            pstRd[i].u32UsrField2 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32UsrField2;
            pstRd[i].u32UsrField3 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32UsrField3;

            u32RdqRptr = ((u32RdqRptr+1) < IPF_DLRD_DESC_SIZE)?(u32RdqRptr+1):0;

            /* ����RD��ָ�� */
            IPF_REG_WRITE(SOC_IPF_CH1_RDQ_RPTR_ADDR(IPF_REGBASE_ADR), u32RdqRptr);
            /* ����CD��ָ�� */
            u32CdqRptr = (IO_ADDRESS(pstRd[i].u32InPtr)- (BSP_U32)g_stIpfDl.pstIpfCDQ)/sizeof(IPF_CD_DESC_S);
            while(g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16Attribute != 1)
            {
                /* ���ͷŵ�CD��0 */
                g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16Attribute = 0;
                g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16PktLen = 0;
                g_stIpfDl.pstIpfCDQ[u32CdqRptr].u32Ptr = 0;
                u32CdqRptr = ((u32CdqRptr+1) < IPF_DLCD_DESC_SIZE)?(u32CdqRptr+1):0;
            }
            u32CdqRptr = ((u32CdqRptr+1) < IPF_DLCD_DESC_SIZE)?(u32CdqRptr+1):0;
            *(g_stIpfDl.u32IpfCdRptr) = u32CdqRptr;
        }
    }
    *pu32Num = u32Num;
}
BSP_S32 BSP_IPF_GetDlAdNum(BSP_U32* pu32AD0Num, BSP_U32* pu32AD1Num)
{
    BSP_U32 u32DlAd0Depth = 0;
    BSP_U32 u32DlAd1Depth = 0;
    BSP_U32 u32DlAd0wptr = 0;
    BSP_U32 u32DlAd0rptr = 0;
    BSP_U32 u32DlAd1wptr = 0;
    BSP_U32 u32DlAd1rptr = 0;
    BSP_U32 u32AD0Num = 0;
    BSP_U32 u32AD1Num = 0;    
    /*��μ��*/
    if((NULL == pu32AD0Num)||(NULL == pu32AD1Num))
    {
        IPF_DEBUG("BSP_IPF_GetDlAdNum:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* �������AD0���� */
    IPF_REG_READ(SOC_IPF_CH1_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32DlAd0wptr);
    IPF_REG_READ(SOC_IPF_CH1_ADQ0_RPTR_ADDR(IPF_REGBASE_ADR), u32DlAd0rptr);
    if (u32DlAd0wptr >= u32DlAd0rptr)/*дָ����ǰ������˳��*/
        u32DlAd0Depth = IPF_DLAD0_DESC_SIZE - (u32DlAd0wptr - u32DlAd0rptr+1);
    else
        u32DlAd0Depth = u32DlAd0rptr - u32DlAd0wptr - 1;
    u32AD0Num = (u32DlAd0Depth > IPF_OBLIGATE_AD_NUM) ? (u32DlAd0Depth - IPF_OBLIGATE_AD_NUM) : 0;
    if(u32AD0Num > IPF_DLAD0_DESC_SIZE)
    {
        IPF_DEBUG("BSP_IPF_GetDlAdNum:ad0 num error\n");
        return BSP_ERR_IPF_INVALID_PARA;    
    }
    *pu32AD0Num = u32AD0Num;

    /* �������AD1���� */
    IPF_REG_READ(SOC_IPF_CH1_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32DlAd1wptr);
    IPF_REG_READ(SOC_IPF_CH1_ADQ1_RPTR_ADDR(IPF_REGBASE_ADR), u32DlAd1rptr);
    if (u32DlAd1wptr >= u32DlAd1rptr)/*дָ����ǰ������˳��*/
        u32DlAd1Depth = IPF_DLAD1_DESC_SIZE - (u32DlAd1wptr - u32DlAd1rptr+1);
    else
        u32DlAd1Depth = u32DlAd1rptr - u32DlAd1wptr - 1;

    u32AD1Num = (u32DlAd1Depth > IPF_OBLIGATE_AD_NUM) ? (u32DlAd1Depth - IPF_OBLIGATE_AD_NUM) : 0;
    if(u32AD1Num > IPF_DLAD1_DESC_SIZE)
    {
        IPF_DEBUG("BSP_IPF_GetDlAdNum:ad1 num error\n");
        return BSP_ERR_IPF_INVALID_PARA;    
    }
    *pu32AD1Num = u32AD1Num;

    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_ConfigDlAd(IPF_AD_TYPE_E eAdType, BSP_U32 u32AdNum, IPF_AD_DESC_S *pstAdDesc  )
{
    BSP_U32 i;
    BSP_U32 u32ADQ0wptr;
    BSP_U32 u32ADQ1wptr;

/*Modify_for_c_reset, l00212112,20130511, starts*/
    /*���C�˸�λ���򷵻�*/
    if (IPF_FORRESET_CONTROL_FORBID == BSP_IPF_GetControlFLagForCcoreReset())
    {
        IPF_DEBUG("BSP_IPF_ConfigUpFilter:ccore is resetting\n");
        return BSP_ERR_IPF_CCORE_RESETTING;
    }
/*Modify_for_c_reset, l00212112,20130511, ends*/

    /* ��μ�� */
    if((IPF_AD_MAX <= eAdType) || (NULL == pstAdDesc))
    {
        IPF_DEBUG("BSP_IPF_ConfigDlAd:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    /* ���ģ���Ƿ��ʼ�� */
    if(!(*g_pbIPFInit))
    {
        IPF_DEBUG("BSP_IPF_ConfigDlAd:not init\n");
        return BSP_ERR_IPF_NOT_INIT;
    }
    if(IPF_AD_0 == eAdType)
    {
        g_stIPFDebugInfo->u32DlAdq0Num += u32AdNum;
        /* ��ȡADQ0дָ�� */
        IPF_REG_READ(SOC_IPF_CH1_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ0wptr);
        /* ��ȡĿ�Ľṹ��ָ���Ŀ�Ľṹ������ָ�룬���뵽ADQ�� */
        for(i=0; i < u32AdNum; i++)
        {
            if((BSP_NULL == (void *)pstAdDesc[i].u32OutPtr0) || (BSP_NULL == (void *)pstAdDesc[i].u32OutPtr1))
            {
                IPF_DEBUG("BSP_IPF_ConfigDlAd:ad0 null point\n");
                return BSP_ERR_IPF_INVALID_PARA;
            }            
            g_stIpfDl.pstIpfADQ0[u32ADQ0wptr].u32OutPtr0 = pstAdDesc[i].u32OutPtr0;
            g_stIpfDl.pstIpfADQ0[u32ADQ0wptr].u32OutPtr1 = pstAdDesc[i].u32OutPtr1;
            u32ADQ0wptr = ((u32ADQ0wptr + 1) < IPF_DLAD0_DESC_SIZE)? (u32ADQ0wptr + 1) : 0;
        }
        /* ����ADQ0дָ�� */
        IPF_REG_WRITE(SOC_IPF_CH1_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ0wptr);
    }
    else
    {
        g_stIPFDebugInfo->u32DlAdq1Num += u32AdNum;    
        /* ��ȡADQ1дָ�� */
        IPF_REG_READ(SOC_IPF_CH1_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ1wptr);
        /* ��ȡĿ�Ľṹ��ָ���Ŀ�Ľṹ������ָ�룬���뵽ADQ�� */
        for(i=0; i < u32AdNum; i++)
        {
            if((BSP_NULL == (void *)pstAdDesc[i].u32OutPtr0) || (BSP_NULL == (void *)pstAdDesc[i].u32OutPtr1))
            {
                IPF_DEBUG("BSP_IPF_ConfigDlAd:ad1 null point\n");
                return BSP_ERR_IPF_INVALID_PARA;
            }        
            g_stIpfDl.pstIpfADQ1[u32ADQ1wptr].u32OutPtr0 = pstAdDesc[i].u32OutPtr0;
            g_stIpfDl.pstIpfADQ1[u32ADQ1wptr].u32OutPtr1 = pstAdDesc[i].u32OutPtr1;
            u32ADQ1wptr = ((u32ADQ1wptr + 1) < IPF_DLAD1_DESC_SIZE)? (u32ADQ1wptr + 1) : 0;
        }
        /* ����ADQ1дָ�� */
        IPF_REG_WRITE(SOC_IPF_CH1_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32ADQ1wptr);
    }
    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_GetUsedDlAd(IPF_AD_TYPE_E eAdType, BSP_U32 *pu32AdNum, IPF_AD_DESC_S *pstAdDesc)
{
    BSP_U32 i = 0;
    BSP_U32 u32IpfAdqRpt = 0;
    BSP_U32 u32IpfAdqWpt = 0;
    BSP_U32 u32IpfAdqSize = 0;
    IPF_AD_DESC_S *pstIpfADQ = (IPF_AD_DESC_S*)BSP_NULL;
    BSP_U32 u32AdNum = 0;

    IPF_DEBUG("BSP_IPF_GetUsedDlAd\n");

    /*  ��μ�� */
    if((IPF_AD_MAX <= eAdType)|| (BSP_NULL == pu32AdNum) || (BSP_NULL == pstAdDesc))
    {
        IPF_DEBUG("BSP_IPF_GetUsedDlAd:para error\n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ��ȡ��ʵ�Ķ�ָ�� */
    if(IPF_SUCCESS != IPF_ObligateAdqRpt(IPF_CHANNEL_DOWN, eAdType, &u32IpfAdqRpt))
    {
        IPF_DEBUG("IPF_ObligateAdqRpt:error\n");
        return IPF_ERROR;
    }

    /* ����ͨ��ID��AD���ͻ�ȡ�����Ĵ�������(дָ�롢��С������) */
    switch (eAdType) {
        case IPF_AD_0:
                IPF_REG_READ(SOC_IPF_CH1_ADQ0_WPTR_ADDR(IPF_REGBASE_ADR), u32IpfAdqWpt);
                u32IpfAdqSize = IPF_DLAD0_DESC_SIZE;
                pstIpfADQ = g_stIpfDl.pstIpfADQ0;
            break;
        case IPF_AD_1:
                IPF_REG_READ(SOC_IPF_CH1_ADQ1_WPTR_ADDR(IPF_REGBASE_ADR), u32IpfAdqWpt);
                u32IpfAdqSize = IPF_DLAD1_DESC_SIZE;
                pstIpfADQ = g_stIpfDl.pstIpfADQ1;
            break;
        default:
                IPF_DEBUG("BSP_IPF_GetUsedDlAd:para error\n");
            break;
    }

    /* ���ݻ�ȡ����ADQ����(��Ӧ����)���Ѿ�������ʹ�õ�AD��Ϣ��������pu32AdNum��pstAdDesc */
    for(i = u32IpfAdqRpt; i != u32IpfAdqWpt; i %= u32IpfAdqSize)
    {
        if(BSP_NULL == pstAdDesc)
        {
            IPF_DEBUG("BSP_IPF_GetUsedDlAd:null point\n");
            return BSP_ERR_IPF_INVALID_PARA;
        }
        pstAdDesc->u32OutPtr0 = pstIpfADQ[i].u32OutPtr0;
        pstAdDesc->u32OutPtr1 = pstIpfADQ[i].u32OutPtr1;
        u32AdNum++;
        pstAdDesc++;
        i++;
    }
    *pu32AdNum = u32AdNum;

    return IPF_SUCCESS;
}
#endif

#ifdef __KERNEL__

EXPORT_SYMBOL(BSP_IPF_ConfigTimeout);
EXPORT_SYMBOL(BSP_IPF_ConfigUpFilter);
EXPORT_SYMBOL(BSP_IPF_GetUlBDNum);
EXPORT_SYMBOL(BSP_IPF_UlStateIdle);
EXPORT_SYMBOL(BSP_IPF_SetPktLen);
EXPORT_SYMBOL(BSP_IPF_GetStat);
EXPORT_SYMBOL(BSP_IPF_RegisterWakeupDlCb);
EXPORT_SYMBOL(BSP_IPF_GetDlRd);
MODULE_LICENSE("GPL");
#endif
#endif

#ifdef __cplusplus
}
#endif

