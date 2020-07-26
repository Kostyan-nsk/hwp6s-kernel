#ifdef __cplusplus
extern "C" {
#endif
#include "product_config.h"
#if (FEATURE_IPF_VERSION == IPF_V200)
#include "ipf_test_v200.h"

BSP_BOOL g_u32IpfStAllInit = BSP_FALSE;
BSP_U32* g_ipftestdebug = (BSP_U32*)(IPF_INIT_ADDR + 4);
BSP_U32* g_pu32DlTaskCmp = (BSP_U32*)(IPF_INIT_ADDR + 12);
BSP_U8 *pu8ShortIpData = BSP_NULL;
BSP_U8 *pu8LongIpData = BSP_NULL;
IPF_CONFIG_ULPARAM_S *pstShortUlPara = BSP_NULL;
IPF_CONFIG_ULPARAM_S *pstLongUlPara = BSP_NULL;
BSP_U8 pu8TestData[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
IpfStResultCheck_S stResultCheck = {0};

int g_IpfUlTaskId = 0;
int g_IpfDlAdqNullTaskId = 0;
struct semaphore g_IpfDlTaskSemId;
struct semaphore g_IpfDlIntResultSemId;
struct semaphore g_IpfDlIntAdqNullSemId;
struct semaphore g_IpfDlIntAdq0NullSemId;
struct semaphore g_IpfDlIntAdq1NullSemId;
struct task_struct * g_IpfDlTaskId;
struct semaphore    g_ipfSyncSemId;

#define ipfTestDelay(a)     msleep((a)*10)
#define SHORT_IP_LEN        (400)
#define LONG_IP_LEN         (1000)
#define ADQ0_LEN            (448)
#define ADQ1_LEN            (1536)
#define IPF_TEST_ENTRY()  \
    do{ \
        pu8ShortIpData = (BSP_U8*)BSP_SMALLOC(SHORT_IP_LEN, MEM_ICC_DDR_POOL); \
        if(BSP_NULL == pu8ShortIpData) \
        { \
            IPF_PRINT("%s, %d   FAILED\n", __func__, __LINE__); \
            goto err; \
        } \
        pstShortUlPara = (IPF_CONFIG_ULPARAM_S*)kmalloc(IPF_ULBD_DESC_SIZE * sizeof(IPF_CONFIG_ULPARAM_S), GFP_KERNEL); \
        if(BSP_NULL == pstShortUlPara) \
        { \
            IPF_PRINT("%s, %d   FAILED\n", __func__, __LINE__); \
            goto err; \
        } \
        pu8LongIpData = (BSP_U8*)BSP_SMALLOC(LONG_IP_LEN, MEM_ICC_DDR_POOL); \
        if(BSP_NULL == pu8LongIpData) \
        { \
            IPF_PRINT("%s, %d   FAILED\n", __func__, __LINE__); \
            goto err; \
        } \
        pstLongUlPara = (IPF_CONFIG_ULPARAM_S*)kmalloc(IPF_ULBD_DESC_SIZE * sizeof(IPF_CONFIG_ULPARAM_S), GFP_KERNEL); \
        if(BSP_NULL == pstLongUlPara) \
        { \
            IPF_PRINT("%s, %d   FAILED\n", __func__, __LINE__); \
            goto err; \
        } \
    }while(0)   
#define IPF_TEST_LEAVE() \
    do{ \
        if(BSP_NULL != pu8ShortIpData) \
        { \
            BSP_SFREE(pu8ShortIpData); \
            pu8ShortIpData = BSP_NULL; \
        } \
        if(BSP_NULL != pu8LongIpData) \
        { \
            BSP_SFREE(pu8LongIpData); \
            pu8LongIpData = BSP_NULL; \
        } \
        if(BSP_NULL != pstShortUlPara) \
        { \
            kfree(pstShortUlPara); \
            pstShortUlPara = BSP_NULL; \
        } \
        if(BSP_NULL != pstLongUlPara) \
        { \
            kfree(pstLongUlPara); \
            pstLongUlPara = BSP_NULL; \
        } \
    }while(0)
extern BSP_CHAR *g_pSyncState;
/********************************************************************
* ��������
********************************************************************/
BSP_S32 IPF_ST_ConfigDlADQ0(BSP_U32 u32AdNum);
BSP_S32 IPF_ST_ConfigDlADQ1(BSP_U32 u32AdNum);

extern unsigned int omTimerGet(void);
/********************************************************************
* ����ʵ��
********************************************************************/
static void msDelay(unsigned int ms)
{
    unsigned int i =0;
    unsigned int Base=25165;
    unsigned int count;

    count = ms*Base;
    while(count--)
    {
        i++;
    }
}
BSP_S32 IpfDlIntResultCb(BSP_VOID)
{
    up(&g_IpfDlIntResultSemId);
    return OK;
}
BSP_S32 IpfDlIntAdq0NullCb(IPF_ADQ_EMPTY_E eAdqEmpty)
{
    if(IPF_EMPTY_ADQ0 == eAdqEmpty)
    {
        up(&g_IpfDlIntAdq0NullSemId);
        return OK;
    }
    IPF_PRINT("%s, %d\n", __func__, __LINE__);    
    return ERROR;
}
BSP_S32 IpfDlIntAdq1NullCb(IPF_ADQ_EMPTY_E eAdqEmpty)
{
    if(IPF_EMPTY_ADQ1 == eAdqEmpty)
    {
        up(&g_IpfDlIntAdq1NullSemId);
        return OK;
    }
    IPF_PRINT("%s, %d\n", __func__, __LINE__);        
    return ERROR;
}
BSP_S32 IpfDlIntAdqNullCb(BSP_VOID)
{
    up(&g_IpfDlIntAdqNullSemId);

    return OK;
}
void IPF_DL_AdqNull_Task(void)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;

    while(1)
    {
        down(&g_IpfDlIntAdqNullSemId);
        
        s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
        if(IPF_SUCCESS != s32Ret)
        {
            IPF_PRINT("TESTERROR    IPF_DL_AdqNull_Task:%d s32Ret = %d\n", __LINE__, s32Ret);
            return;
        }
        else
        {
            IPF_ST_ConfigDlADQ0(u32Adq0Num);
            IPF_ST_ConfigDlADQ1(u32Adq1Num);
        }
    }
}
BSP_VOID IPF_DL_Task(void *param)
{
    IpfStResultCheck_S *pstResultCheck = (IpfStResultCheck_S*)param;
    BSP_U32 u32Num = 0;
    IPF_RD_DESC_S stRd[IPF_DLRD_DESC_SIZE];
    TTF_MEM_ST* pstTtf = BSP_NULL;
    BSP_U32 i = 0;

    while(1)
    { 
        memset(stRd, 0x0, (sizeof(IPF_RD_DESC_S) * IPF_DLRD_DESC_SIZE));
        u32Num = IPF_DLRD_DESC_SIZE;
        
        down(&g_IpfDlIntResultSemId);
        if(*g_ipftestdebug)
        {
            IPF_PRINT("TESTERROR    IPF_DL_Task in \n");
        }
        BSP_IPF_GetDlRd(&u32Num, stRd);
        pstResultCheck->u32RdNum += u32Num;
        IPF_PRINT("%s:%d, u32Num = %d\n", __func__, __LINE__, u32Num); 
        
        for(i = 0; i < u32Num; i++)
        {
            if(BSP_NULL != (void*)(stRd[i].u32OutPtr))
            {
                pstTtf = (TTF_MEM_ST*)(DDR_PHY_TO_VIRT(stRd[i].u32OutPtr));
                if(*g_ipftestdebug)
                {
                    IPF_PRINT("%s:%d, u32Num = %d, i = %d \n",__func__, __LINE__, u32Num, i);
                    IPF_PRINT("pstTtf = 0x%x pData = 0x%x\n",(BSP_U32)pstTtf, (BSP_U32)(pstTtf->pData));
                }
                BSP_SFREE(pstTtf->pData);
                BSP_SFREE(pstTtf);   
            }
            else
            {
                IPF_PRINT("%s:%d, i = %d\n",__func__, __LINE__, i);    
                return;
            }
            
            if(stRd[i].u16Result != pstResultCheck->u16Result)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task:%d, u16Result = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u16Result, stRd[i].u16Result, i);
            }
            if(stRd[i].u16UsrField1 != pstResultCheck->u16UsrField1)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task:%d, u16UsrField1 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u16UsrField1, stRd[i].u16UsrField1, i);
            }    
            if(stRd[i].u32UsrField2 != pstResultCheck->u32UsrField2)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task:%d, u16UsrField2 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u32UsrField2, stRd[i].u32UsrField2, i);
            }       
            if(stRd[i].u32UsrField3 != pstResultCheck->u32UsrField3)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task:%d, u32UsrField3 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u32UsrField3, stRd[i].u32UsrField3, i);
            }                 
        }
        if(*g_ipftestdebug)
        {
            IPF_PRINT("TESTERROR    IPF_DL_Task out \n");
        }
        
        up(&g_IpfDlTaskSemId);
    }
}

BSP_VOID IPF_DL_Task_Fn(void *param)
{
    BSP_U32 i = 0;
    IpfStResultCheck_S *pstResultCheck = (IpfStResultCheck_S*)param;
    BSP_U32 u32Num = 0;
    IPF_RD_DESC_S stRd[IPF_DLRD_DESC_SIZE];
    TTF_MEM_ST* pstTtf = BSP_NULL;
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;  
    
    while(1)
    { 
        memset(stRd, 0x0, (sizeof(IPF_RD_DESC_S) * IPF_DLRD_DESC_SIZE));
        u32Num = IPF_DLRD_DESC_SIZE;
        
        down(&g_IpfDlIntResultSemId);
        BSP_IPF_GetDlRd(&u32Num, stRd);
        pstResultCheck->u32RdNum += u32Num;
        IPF_PRINT("%s:%d, u32Num = %d\n", __func__, __LINE__, u32Num); 
        
        for(i = 0; i < u32Num; i++)
        {
            if(BSP_NULL != (void*)(stRd[i].u32OutPtr))
            {
                pstTtf = (TTF_MEM_ST*)(DDR_PHY_TO_VIRT(stRd[i].u32OutPtr));
                
                BSP_SFREE(pstTtf->pData);
                BSP_SFREE(pstTtf);   
            }
            else
            {
                IPF_PRINT("%s:%d, i = %d\n",__func__, __LINE__, i);    
                return;
            }
            
            if(stRd[i].u16Result != pstResultCheck->u16Result)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task_Fn:%d, u16Result = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u16Result, stRd[i].u16Result, i);
            }
            if(stRd[i].u16UsrField1 != pstResultCheck->u16UsrField1)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task_Fn:%d, u16UsrField1 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u16UsrField1, stRd[i].u16UsrField1, i);
            }    
            if(stRd[i].u32UsrField2 != pstResultCheck->u32UsrField2)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task_Fn:%d, u16UsrField2 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u32UsrField2, stRd[i].u32UsrField2, i);
            }       
            if(stRd[i].u32UsrField3 != pstResultCheck->u32UsrField3)
            {
                IPF_PRINT("TESTERROR    IPF_DL_Task_Fn:%d, u32UsrField3 = 0x%x(expect) 0x%x(fact), i = %d \n",__LINE__, 
                            pstResultCheck->u32UsrField3, stRd[i].u32UsrField3, i);
            }    
        }

        s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
        if(IPF_SUCCESS != s32Ret)
        {
            IPF_PRINT("TESTERROR    IPF_DL_Task_Fn:%s, %d, s32Ret = %d\n", __func__, __LINE__, s32Ret);    
        }   
        else
        {
            IPF_ST_ConfigDlADQ0(u32Adq0Num);
            IPF_ST_ConfigDlADQ1(u32Adq1Num);
        }       
        
        up(&g_IpfDlTaskSemId);
    }
}

BSP_VOID IPF_DL_Task_Speed(void *param)
{
    IpfStResultCheck_S *pstResultCheck = (IpfStResultCheck_S*)param;
    BSP_U32 u32Num = 0;
    IPF_RD_DESC_S stRd[IPF_DLRD_DESC_SIZE];
    TTF_MEM_ST* pstTtf = BSP_NULL;
    BSP_U32 i = 0;

    while(1)
    { 
        memset(stRd, 0x0, (sizeof(IPF_RD_DESC_S) * IPF_DLRD_DESC_SIZE));
        u32Num = IPF_DLRD_DESC_SIZE;
        
        down(&g_IpfDlIntResultSemId);
        
        BSP_IPF_GetDlRd(&u32Num, stRd);
        pstResultCheck->u32RdNum += u32Num;
        
        for(i = 0; i < u32Num; i++)
        {
            if(BSP_NULL != (void*)(stRd[i].u32OutPtr))
            {
                pstTtf = (TTF_MEM_ST*)(DDR_PHY_TO_VIRT(stRd[i].u32OutPtr));
               
                BSP_SFREE(pstTtf->pData);
                BSP_SFREE(pstTtf);   
            }     
            else
            {
                IPF_PRINT("%s:%d, i = %d\n",__func__, __LINE__, i);    
                return;
            }            
        }
        
        up(&g_IpfDlTaskSemId);
    }
}
/* test BSP_IPF_ConfigTimeout */
BSP_S32 IPF_ST_ConfigTimeout(BSP_U32 u32Timeout)
{
    BSP_S32 s32ret = 0;
    s32ret = BSP_IPF_ConfigTimeout(u32Timeout);
    if(s32ret != IPF_SUCCESS)
    {
        IPF_PRINT("TESTERROR    IPF_ST_ConfigTimeout:%d, u32Timeout :%d\n",__LINE__, u32Timeout);
        return BSP_ERROR;
    }
    if(*(BSP_U32*)(SOC_IPF_TIME_OUT_ADDR(IPF_REGBASE_ADR)) != (0x10000 | u32Timeout))
    {
        IPF_PRINT("TESTERROR    IPF_ST_ConfigTimeout:%d, u32Timeout :%d\n",__LINE__, u32Timeout);
        return BSP_ERROR;
    }
    return BSP_OK;
}
BSP_S32 IPF_ST_TimeoutCheck(BSP_U32 u32Timeout)
{
    BSP_S32 s32ret = 0;
    s32ret = BSP_IPF_ConfigTimeout(u32Timeout);
    if(s32ret != BSP_ERR_IPF_INVALID_PARA)
    {
        IPF_PRINT("TESTERROR    IPF_ST_TimeoutCheck:%d, u32Timeout :%d\n",__LINE__, u32Timeout);
        return BSP_ERROR;
    }
    return BSP_OK;
}
BSP_S32 IPF_ST_ConfigUlShortPacket(IpfStModemId_E eModemId, BSP_U32 u32BdNum, BSP_BOOL bReportFlag)
{
    BSP_U32 i = 0;    
    StreamIPv4Header stIPHeader = {0};
    StreamUDP stUDP = {0};
    BSP_S32 s32Ret = 0;
    
    stIPHeader.Vers = 4;
    stIPHeader.HeaderLen = 5;
    stIPHeader.TOS = 3;
    stIPHeader.TOL = SHORT_IP_LEN;
    stIPHeader.ID = 1;
    stIPHeader.ReservFlag = 0;
    stIPHeader.DF = 0;
    stIPHeader.MF = 0;
    stIPHeader.offset = 0;
    stIPHeader.TTL = 128;
    stIPHeader.Prot = 17; /* UDP */
    stIPHeader.Checksum = 0xEC3A;
    stIPHeader.SourceAddr = 0xACA84649;
    stIPHeader.DestAddr = 0x0202014E;

    stUDP.SrcPort = 5088;
    stUDP.DstPort = 2923;
   
    memset(pu8ShortIpData, 0x0, SHORT_IP_LEN);
    memcpy(pu8ShortIpData, &stIPHeader, sizeof(StreamIPv4Header));
    memcpy(pu8ShortIpData+sizeof(StreamIPv4Header), &stUDP, sizeof(StreamUDP));
    memcpy(pu8ShortIpData+sizeof(StreamIPv4Header)+sizeof(StreamUDP), pu8TestData, (sizeof(pu8TestData)-1));
         
    pstShortUlPara[0].u16Len = SHORT_IP_LEN;
    pstShortUlPara[0].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x0;
    pstShortUlPara[0].u32Data = DDR_VIRT_TO_PHY(pu8ShortIpData);
    pstShortUlPara[0].u16Reserved = 0;
    pstShortUlPara[0].u16UsrField1 = eModemId;
    pstShortUlPara[0].u32UsrField2 = 0x5A5A5A5A;
    pstShortUlPara[0].u32UsrField3 = 0x55AA55AA;
    for(i = 1; i < u32BdNum; i++)
    {
        memcpy(&pstShortUlPara[i], &pstShortUlPara[0], sizeof(IPF_CONFIG_ULPARAM_S));
    }

    if(BSP_TRUE == bReportFlag)   
    {
        pstShortUlPara[--i].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x1;;
    }
    s32Ret = BSP_IPF_ConfigUpFilter(u32BdNum, pstShortUlPara);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }    
    return OK;    
}
BSP_S32 IPF_ST_ConfigUlLongPacket(IpfStModemId_E eModemId, BSP_U32 u32BdNum, BSP_BOOL bReportFlag)
{
    BSP_U32 i = 0;    
    StreamIPv4Header stIPHeader = {0};
    StreamUDP stUDP = {0};
    BSP_S32 s32Ret = 0;
    
    stIPHeader.Vers = 4;
    stIPHeader.HeaderLen = 5;
    stIPHeader.TOS = 3;
    stIPHeader.TOL = LONG_IP_LEN;
    stIPHeader.ID = 1;
    stIPHeader.ReservFlag = 0;
    stIPHeader.DF = 0;
    stIPHeader.MF = 0;
    stIPHeader.offset = 0;
    stIPHeader.TTL = 128;
    stIPHeader.Prot = 17; /* UDP */
    stIPHeader.Checksum = 0xEC3A;
    stIPHeader.SourceAddr = 0xACA84649;
    stIPHeader.DestAddr = 0x0202014E;

    stUDP.SrcPort = 5088;
    stUDP.DstPort = 2923;
 
    memset(pu8LongIpData, 0x0, LONG_IP_LEN);
    memcpy(pu8LongIpData, &stIPHeader, sizeof(StreamIPv4Header));
    memcpy(pu8LongIpData+sizeof(StreamIPv4Header), &stUDP, sizeof(StreamUDP));
    memcpy(pu8LongIpData+sizeof(StreamIPv4Header)+sizeof(StreamUDP), pu8TestData, (sizeof(pu8TestData)-1));

    pstLongUlPara[0].u16Len = LONG_IP_LEN;
    pstLongUlPara[0].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x0;
    pstLongUlPara[0].u32Data = DDR_VIRT_TO_PHY(pu8LongIpData);
    pstLongUlPara[0].u16Reserved = 0;    
    pstLongUlPara[0].u16UsrField1 = eModemId;
    pstLongUlPara[0].u32UsrField2 = 0x5A5A5A5A;
    pstLongUlPara[0].u32UsrField3 = 0x55AA55AA;
    for(i = 1; i < u32BdNum; i++)
    {
        memcpy(&pstLongUlPara[i], &pstLongUlPara[0], sizeof(IPF_CONFIG_ULPARAM_S));
    }

    if(BSP_TRUE == bReportFlag)   
    {    
        pstLongUlPara[--i].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x1;
    }
    s32Ret = BSP_IPF_ConfigUpFilter(u32BdNum, pstLongUlPara);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    return OK;    
}
BSP_S32 IPF_ST_ConfigDlADQ0(BSP_U32 u32AdNum)
{
    BSP_U32 i = 0;
    BSP_S32 s32Ret = 0;    
    IPF_AD_DESC_S *pstAdBuffer;
    TTF_MEM_ST* pstDstTtf;

    pstAdBuffer = (IPF_AD_DESC_S*)kmalloc(u32AdNum * sizeof(IPF_AD_DESC_S), GFP_KERNEL);
    for(i = 0; i < u32AdNum; i++)
    {
        pstDstTtf = BSP_SMALLOC(sizeof(TTF_MEM_ST), MEM_ICC_DDR_POOL);
        if(pstDstTtf == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);     
            return ERROR;
        }
        pstDstTtf->pData = BSP_SMALLOC(ADQ0_LEN, MEM_ICC_DDR_POOL);
        if(pstDstTtf->pData == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);   
            return ERROR;
        }    
        pstAdBuffer[i].u32OutPtr0 = DDR_VIRT_TO_PHY(pstDstTtf->pData);
        pstAdBuffer[i].u32OutPtr1 = DDR_VIRT_TO_PHY(pstDstTtf);
    }

    s32Ret = BSP_IPF_ConfigDlAd(IPF_AD_0, u32AdNum, pstAdBuffer);  
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("TESTERROR    func=%s, s32Ret=%d\n",__func__, s32Ret);
        return ERROR;
    }
    return OK;    
}

BSP_S32 IPF_ST_ConfigDlADQ1(BSP_U32 u32AdNum)
{
    BSP_U32 i = 0;
    BSP_S32 s32Ret = 0;        
    IPF_AD_DESC_S *pstAdBuffer;
    TTF_MEM_ST* pstDstTtf;

    pstAdBuffer = (IPF_AD_DESC_S*)kmalloc(u32AdNum * sizeof(IPF_AD_DESC_S), GFP_KERNEL);
    for(i = 0; i < u32AdNum; i++)
    {
        pstDstTtf = BSP_SMALLOC(sizeof(TTF_MEM_ST), MEM_ICC_DDR_POOL);
        if(pstDstTtf == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);       
            return ERROR;                        
        }
        pstDstTtf->pData = BSP_SMALLOC(ADQ1_LEN, MEM_ICC_DDR_POOL);
        if(pstDstTtf->pData == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);     
            return ERROR;            
        }    
        pstAdBuffer[i].u32OutPtr0 = DDR_VIRT_TO_PHY(pstDstTtf->pData);
        pstAdBuffer[i].u32OutPtr1 = DDR_VIRT_TO_PHY(pstDstTtf);
    }

    s32Ret= BSP_IPF_ConfigDlAd(IPF_AD_1, u32AdNum, pstAdBuffer);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("TESTERROR    func=%s, s32Ret=%d\n",__func__, s32Ret);
        return ERROR;
    }
    return OK;        
}
/****************************************
* 0   ���˸��Զ��õĲ�������
* 1   ��ִ��C�˺�ִ��A������
* 2   C�˵Ĳ�������
* 3   ��ִ��A�˺�ִ��C������
* 4   A/C��ͬʱ����ִ�е�����
****************************************/
/*��֤��ʱ����*/
BSP_S32 BSP_IPF_ST_001(BSP_VOID)
{
    BSP_U32 u32Timeout = 0;
    BSP_U32 u32OldTime = 0;
    BSP_S32 s32ret = 0;

    IPF_REG_READ(SOC_IPF_TIME_OUT_ADDR(IPF_REGBASE_ADR), u32OldTime);

    s32ret = IPF_ST_TimeoutCheck(u32Timeout);
   
    u32Timeout = 1;
    s32ret += IPF_ST_ConfigTimeout(u32Timeout);

    u32Timeout = 0xffff;
    s32ret += IPF_ST_ConfigTimeout(u32Timeout);

    u32Timeout = 0x10000;
    s32ret += IPF_ST_TimeoutCheck(u32Timeout);

    IPF_REG_WRITE(SOC_IPF_TIME_OUT_ADDR(IPF_REGBASE_ADR), u32OldTime);

    if(BSP_OK == s32ret)
    {
        IPF_PRINT("%s : SUCCESS!\n", __func__);
        return BSP_OK;
    }
    else
    {
        return BSP_ERROR;
    }
}

/*��֤��ʱ����*/
BSP_S32 BSP_IPF_ST_002(BSP_VOID)
{
    BSP_S32 s32Ret;
    BSP_U32 u32MaxLen = 0;
    BSP_U32 u32MinLen = 0;

    s32Ret = BSP_IPF_SetPktLen(u32MaxLen,u32MinLen);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("TESTERROR    BSP_IPF_SetPktLen:%d\n",__LINE__);
        return ERROR;
    }

    u32MaxLen = 0x5;
    u32MinLen = 0x6;
    s32Ret = BSP_IPF_SetPktLen(u32MaxLen,u32MinLen);
    if(s32Ret != BSP_ERR_IPF_INVALID_PARA)
    {
        IPF_PRINT("TESTERROR    BSP_IPF_SetPktLen:%d\n",__LINE__);
        return ERROR;
    }

    u32MaxLen = 0x345;
    u32MinLen = 0x123;
    s32Ret = BSP_IPF_SetPktLen(u32MaxLen,u32MinLen);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("TESTERROR    BSP_IPF_SetPktLen:%d\n",__LINE__);
        return ERROR;
    }
    if(*(BSP_U32*)(SOC_IPF_PKT_LEN_ADDR(IPF_REGBASE_ADR)) != 0x3450123)
    {
        IPF_PRINT("TESTERROR    BSP_IPF_SetPktLen:%d\n",__LINE__);
        return ERROR;
    }

    u32MaxLen = 1500;
    u32MinLen = 40;
    s32Ret = BSP_IPF_SetPktLen(u32MaxLen,u32MinLen);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("TESTERROR    BSP_IPF_SetPktLen:%d\n",__LINE__);
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*����ADQ��д��������*/
BSP_S32 BSP_IPF_ST_003(BSP_U32 u32Count)
{
    BSP_S32 s32Ret;
    BSP_U32 u32Adq0Num_1 = 0;
    BSP_U32 u32Adq1Num_1 = 0;   
    BSP_U32 u32Adq0Num_2 = 0;
    BSP_U32 u32Adq1Num_2 = 0;    
    BSP_U32 u32Adq0Num_3 = 0;
    BSP_U32 u32Adq1Num_3 = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num_1, &u32Adq1Num_1);
    IPF_PRINT("u32Adq0Num_1 = %d, u32Adq1Num_1 = %d\n", u32Adq0Num_1, u32Adq1Num_1);   
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }

    s32Ret = IPF_ST_ConfigDlADQ0(u32Count);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }  
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num_2, &u32Adq1Num_2);
    IPF_PRINT("u32Adq0Num_2 = %d, u32Adq1Num_2 = %d\n", u32Adq0Num_2, u32Adq1Num_2);       
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    if((u32Adq0Num_2 != (u32Adq0Num_1-u32Count)) || (u32Adq1Num_2 != u32Adq1Num_1))    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);  
        IPF_PRINT("u32Adq0Num_1 = %d, u32Adq0Num_2 = %d\n", u32Adq0Num_1, u32Adq0Num_2);   
        IPF_PRINT("u32Adq1Num_1 = %d, u32Adq1Num_2 = %d\n", u32Adq1Num_1, u32Adq1Num_2);                    
        return ERROR;       
    }

    s32Ret = IPF_ST_ConfigDlADQ1(u32Count);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num_3, &u32Adq1Num_3);
    IPF_PRINT("u32Adq0Num_3 = %d, u32Adq1Num_3 = %d\n", u32Adq0Num_3, u32Adq1Num_3);       
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }    
    if((u32Adq0Num_3 != u32Adq0Num_2) || (u32Adq1Num_3 != (u32Adq1Num_2-u32Count)))    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        IPF_PRINT("u32Adq0Num_2 = %d, u32Adq0Num_3 = %d\n", u32Adq0Num_2, u32Adq0Num_3);   
        IPF_PRINT("u32Adq1Num_2 = %d, u32Adq1Num_3 = %d\n", u32Adq1Num_2, u32Adq1Num_3);            
        return ERROR;       
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ADQ����*/
BSP_S32 BSP_IPF_ST_004(IPF_AD_TYPE_E eAdType)
{
    BSP_U32 i = 0;
    BSP_S32 s32Ret = 0;
    IPF_AD_DESC_S *pstAdBuffer;
    TTF_MEM_ST* pstDstTtf;

    pstAdBuffer = (IPF_AD_DESC_S*)kmalloc(30 * sizeof(IPF_AD_DESC_S), GFP_KERNEL);
    for(i = 0; i < 30; i++)
    {
        pstDstTtf = BSP_SMALLOC(sizeof(TTF_MEM_ST), MEM_ICC_DDR_POOL);
        if(pstDstTtf == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);  
            return ERROR;       
        }
        pstDstTtf->pData = BSP_SMALLOC(ADQ0_LEN, MEM_ICC_DDR_POOL);
        if(pstDstTtf->pData == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);   
            return ERROR;                   
        }    
        pstAdBuffer[i].u32OutPtr0 = (BSP_U32)pstDstTtf->pData;
        pstAdBuffer[i].u32OutPtr1 = (BSP_U32)pstDstTtf;
    }
    s32Ret = BSP_IPF_ConfigDlAd(eAdType, 30, pstAdBuffer);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("TESTERROR    func=%s, %d, s32Ret=%d\n",__func__, __LINE__, s32Ret);
        return ERROR;               
    }
    for(i = 0; i < 30; i++)
    {
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr0));
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr1));
    }
    kfree(pstAdBuffer);
    
    ipfTestDelay(2);
    pstAdBuffer = (IPF_AD_DESC_S*)kmalloc(30 * sizeof(IPF_AD_DESC_S), GFP_KERNEL);
    for(i = 0; i < 30; i++)
    {
        pstDstTtf = BSP_SMALLOC(sizeof(TTF_MEM_ST), MEM_ICC_DDR_POOL);
        if(pstDstTtf == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);    
            return ERROR;                           
        }
        pstDstTtf->pData = BSP_SMALLOC(ADQ0_LEN, MEM_ICC_DDR_POOL);
        if(pstDstTtf->pData == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);     
            return ERROR;                           
        }    
        pstAdBuffer[i].u32OutPtr0 = (BSP_U32)pstDstTtf->pData;
        pstAdBuffer[i].u32OutPtr1 = (BSP_U32)pstDstTtf;
    }
    s32Ret = BSP_IPF_ConfigDlAd(eAdType, 30, pstAdBuffer);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("TESTERROR    func=%s, %d, s32Ret=%d\n",__func__, __LINE__, s32Ret);
        return ERROR;                           
    }
    for(i = 0; i < 30; i++)
    {
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr0));
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr1));
    }
    kfree(pstAdBuffer);
    
    ipfTestDelay(2);
    pstAdBuffer = (IPF_AD_DESC_S*)kmalloc(20 * sizeof(IPF_AD_DESC_S), GFP_KERNEL);
    for(i = 0; i < 20; i++)
    {
        pstDstTtf = BSP_SMALLOC(sizeof(TTF_MEM_ST), MEM_ICC_DDR_POOL);
        if(pstDstTtf == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);   
            return ERROR;                           
        }
        pstDstTtf->pData = BSP_SMALLOC(ADQ0_LEN, MEM_ICC_DDR_POOL);
        if(pstDstTtf->pData == BSP_NULL)
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);  
            return ERROR;                           
        }    
        pstAdBuffer[i].u32OutPtr0 = (BSP_U32)pstDstTtf->pData;
        pstAdBuffer[i].u32OutPtr1 = (BSP_U32)pstDstTtf;
    }
    s32Ret = BSP_IPF_ConfigDlAd(eAdType, 20, pstAdBuffer);
    if(IPF_SUCCESS == s32Ret)
    {
        IPF_PRINT("TESTERROR    func=%s, %d, s32Ret=%d\n",__func__, __LINE__, s32Ret);
        return ERROR;                           
    }    
    for(i = 0; i < 20; i++)
    {
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr0));
        BSP_SFREE((BSP_VOID*)(pstAdBuffer[i].u32OutPtr1));
    }
    kfree(pstAdBuffer);
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ͨ��͸��������*/
BSP_S32 BSP_IPF_ST_101(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("BSP_IPF_ST_102, %d\n", __LINE__);    
        goto err;
    }    
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    
    
    err:
        IPF_TEST_LEAVE();
        return ERROR;
}
/*��֤����ͨ���������ж��ϱ�����*/
BSP_S32 BSP_IPF_ST_102(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, 1, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    
    
    err:
        IPF_TEST_LEAVE();
        return ERROR;    
}
/*��֤����ͨ����������ʱ�ж��ϱ�����*/
BSP_S32 BSP_IPF_ST_103(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();        
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, 1, BSP_FALSE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;  
    
    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}
/*��֤����ͨ��ADQ0�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_104(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, IPF_ULBD_DESC_SIZE, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 

    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;       
}
/*��֤����ͨ��ADQ1�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_105(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    
    IPF_TEST_ENTRY();            
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlLongPacket(IPF_MODEM_0, IPF_ULBD_DESC_SIZE, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 

    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK; 

    err:
        IPF_TEST_LEAVE();
        return ERROR;      
}
/*��֤����ͨ��ADQ0\ADQ1�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_106(BSP_VOID)
{
    /*���ֳ���Ӧ�ò�����*/
   return OK;
}
/*ģ����������BD����������Ϊmodem0����ҵ�������֤*/
BSP_S32 BSP_IPF_ST_107(BSP_U32 u32PacketNum)
{
    BSP_U32 i = 0;
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n",__func__, __LINE__);    
        goto err;
    }  
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");  
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    for(i = 0; i < 2; i++)
    {
        if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
        goto err;
    }

    up(&g_IpfDlTaskSemId);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;        
}
/*ģ����������BD����������Ϊmodem0����ҵ�������֤*/
BSP_S32 BSP_IPF_ST_108(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("BSP_IPF_ST_102, %d\n", __LINE__);    
        goto err;
    }    
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;         
}
/*ģ����������BD����������Ϊmodem0����ҵ����˰�����֤*/
BSP_S32 BSP_IPF_ST_109(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("BSP_IPF_ST_102, %d\n", __LINE__);    
        goto err;
    }    
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}
/*ģ����������BD����������Ϊmodem1����ҵ����˰�����֤*/
BSP_S32 BSP_IPF_ST_110(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("BSP_IPF_ST_102, %d\n", __LINE__);    
        goto err;
    }    
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_1, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;        
}
/*��֤����ͨ��͸��������*/
BSP_S32 BSP_IPF_ST_301(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
      
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;   
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      
    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ͨ���������ж��ϱ�����*/
BSP_S32 BSP_IPF_ST_302(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ͨ����������ʱ�ж��ϱ�����*/
BSP_S32 BSP_IPF_ST_303(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;      
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ͨ��ADQ0�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_304(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");  
    sema_init(&g_IpfDlIntAdq0NullSemId, 0);    
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb(IpfDlIntAdq0NullCb);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlIntAdq0NullSemId);
    IPF_ST_ConfigDlADQ0(IPF_DLAD0_DESC_SIZE - 1);

    while(IPF_DLBD_DESC_SIZE != stResultCheck.u32RdNum)
    {
        ipfTestDelay(2);            
    }
    ipfTestDelay(200);    
    
    IPF_PRINT("BSP_IPF_ST_308    u32RDNum = %d, %d\n", stResultCheck.u32RdNum, __LINE__);     
            
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}

/*��֤����ͨ��ADQ1�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_305(BSP_VOID)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    

    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ1(u32Adq1Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");  
    sema_init(&g_IpfDlIntAdq1NullSemId, 0);    
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb(IpfDlIntAdq1NullCb);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlIntAdq1NullSemId);
    IPF_ST_ConfigDlADQ1(IPF_DLAD1_DESC_SIZE - 1);

    while(IPF_DLBD_DESC_SIZE != stResultCheck.u32RdNum)
    {
        ipfTestDelay(2);            
    }
    ipfTestDelay(200);    
    
    IPF_PRINT("BSP_IPF_ST_305    u32RDNum = %d, %d\n", stResultCheck.u32RdNum, __LINE__);     
             
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��֤����ͨ��ADQ0\ADQ1�����ɷǿյ��յĳ���*/
BSP_S32 BSP_IPF_ST_306(BSP_VOID)
{
    IPF_PRINT("%s : SUCCESS!\n", __func__);

    /*���ֳ���Ӧ�ò�����*/
    return OK;	
}
/*ģ����������BD����������Ϊmodem0����ҵ�����(�ػ�)��֤*/
BSP_S32 BSP_IPF_ST_307(BSP_U32 u32PacketNum)
{
    if(u32PacketNum > (IPF_DLAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n",__func__, __LINE__);    
        return ERROR;
    }   
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

}
/*ģ����������BD����������Ϊmodem0����ҵ�������֤*/
BSP_S32 BSP_IPF_ST_308(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
      
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 0x3F;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      
    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*ģ����������BD����������Ϊmodem0����ҵ����˰�����֤*/
BSP_S32 BSP_IPF_ST_309(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
      
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      
    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*ģ����������BD����������Ϊmodem1����ҵ����˰�����֤*/
BSP_S32 BSP_IPF_ST_310(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
      
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_1;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;  
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      
    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*������������ģ���ϡ����г�ʱ�䲢�����д�����ݹ��˰��Ʋ�������ҵ����֤*/
BSP_S32 BSP_IPF_ST_401(BSP_U32 u32Count, BSP_U32 u32Tick)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32CountTmp = u32Count;

    IPF_TEST_ENTRY();                
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }  

    ipfTestDelay(500);
    while(u32Count--)
    {
        u32BDNum = BSP_IPF_GetUlBDNum();
        if(u32BDNum > 0)
        {
            if(OK != IPF_ST_ConfigUlLongPacket(IPF_MODEM_0, 32, BSP_TRUE))
            {
                IPF_PRINT("%s, %d\n", __func__, __LINE__);    
                goto err;
            } 
        }
        ipfTestDelay(u32Tick);
    }

    while(stResultCheck.u32RdNum != (u32CountTmp * 32))
    {
        IPF_PRINT("%s, %d, u32RDNum = %d\n", __func__, __LINE__, stResultCheck.u32RdNum);    
        ipfTestDelay(2);
    }      
    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }     
    IPF_TEST_LEAVE();    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}
/*������������ģ���ϡ����г�ʱ�䲢�����д�����ݹ��˰��Ʋ�������ҵ��ѹ����֤*/
BSP_S32 BSP_IPF_ST_401_Ex(BSP_U32 u32Count, BSP_U32 u32Tick)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32CountTmp = u32Count;
    BSP_U32 u32SendBdNum = 0;

    /*����ǰ׼������*/
    IPF_TEST_ENTRY();                
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    sema_init(&g_IpfDlIntAdqNullSemId, 0); 
    sema_init(&g_IpfDlTaskSemId, 0);        
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb((BSP_IPF_AdqEmptyDlCb)IpfDlIntAdqNullCb);

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  
    g_IpfDlAdqNullTaskId = (int)kthread_run((int (*)(void *))IPF_DL_AdqNull_Task, (void *)0, "IpfDLAdqNullTask");

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }  

    /*��ʱ�ȴ������˾��������*/
    ipfTestDelay(500);
    /*��ʼ��������*/    
    while(u32SendBdNum != (u32CountTmp*64))
    {
        for(;;)
        {
            u32BDNum = BSP_IPF_GetUlBDNum();
            if(0 != u32BDNum)
            break;
        }
        if((u32SendBdNum + u32BDNum) > (u32CountTmp * IPF_ULBD_DESC_SIZE)){
            u32BDNum = (u32CountTmp * IPF_ULBD_DESC_SIZE)-u32SendBdNum;
        }
        if(OK != IPF_ST_ConfigUlLongPacket(IPF_MODEM_0, u32BDNum, BSP_TRUE))
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);    
            goto err;
        } 
        u32SendBdNum += u32BDNum;
        msDelay(u32Tick);
    }

    while(stResultCheck.u32RdNum != (u32CountTmp * IPF_ULBD_DESC_SIZE))
    {
        IPF_PRINT("%s, %d, u32RDNum = %d\n", __func__, __LINE__, stResultCheck.u32RdNum);    
        ipfTestDelay(2);
    }      
    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }     
    IPF_TEST_LEAVE();    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}
/*������������ģ���ϡ����г�ʱ�䲢������С�����ݹ��˰��Ʋ�������ҵ����֤*/
BSP_S32 BSP_IPF_ST_402(BSP_U32 u32Count, BSP_U32 u32Tick)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32CountTmp = u32Count;

    IPF_TEST_ENTRY();                    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;   
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }  

    ipfTestDelay(500);
    while(u32Count--)
    {
        u32BDNum = BSP_IPF_GetUlBDNum();
        if(u32BDNum > 0)
        {
            if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, 32, BSP_TRUE))
            {
                IPF_PRINT("%s, %d\n", __func__, __LINE__);    
                goto err;
            } 
        }
        ipfTestDelay(u32Tick);
    }

    while(stResultCheck.u32RdNum != (u32CountTmp * 32))
    {
        IPF_PRINT("%s, %d, u32RDNum = %d\n", __func__, __LINE__, stResultCheck.u32RdNum);    
        ipfTestDelay(2);
    }      
    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }     
    IPF_TEST_LEAVE();        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

    err:
        IPF_TEST_LEAVE();
        return ERROR;         
}
/*������������ģ���ϡ����г�ʱ�䲢������С�����ݹ��˰��Ʋ�������ҵ��ѹ����֤*/
BSP_S32 BSP_IPF_ST_402_Ex(BSP_U32 u32Count, BSP_U32 u32Tick)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32CountTmp = u32Count;
    BSP_U32 u32SendBdNum = 0;

    /*����ǰ׼������*/
    IPF_TEST_ENTRY();                
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    sema_init(&g_IpfDlIntAdqNullSemId, 0); 
    sema_init(&g_IpfDlTaskSemId, 0);        
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb((BSP_IPF_AdqEmptyDlCb)IpfDlIntAdqNullCb);

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  
    g_IpfDlAdqNullTaskId = (int)kthread_run((int (*)(void *))IPF_DL_AdqNull_Task, (void *)0, "IpfDLAdqNullTask");

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }  

    /*��ʱ�ȴ������˾��������*/
    ipfTestDelay(500);
    /*��ʼ��������*/    
    while(u32SendBdNum != (u32CountTmp*64))
    {
        for(;;)
        {
            u32BDNum = BSP_IPF_GetUlBDNum();
            if(0 != u32BDNum)
            break;
        }
        if((u32SendBdNum + u32BDNum) > (u32CountTmp * IPF_ULBD_DESC_SIZE)){
            u32BDNum = (u32CountTmp * IPF_ULBD_DESC_SIZE)-u32SendBdNum;
        }
        if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32BDNum, BSP_TRUE))
        {
            IPF_PRINT("%s, %d\n", __func__, __LINE__);    
            goto err;
        } 
        u32SendBdNum += u32BDNum;
        msDelay(u32Tick);
    }

    while(stResultCheck.u32RdNum != (u32CountTmp * IPF_ULBD_DESC_SIZE))
    {
        IPF_PRINT("%s, %d, u32RDNum = %d\n", __func__, __LINE__, stResultCheck.u32RdNum);    
        ipfTestDelay(2);
    }      
    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }     
    IPF_TEST_LEAVE();    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}

/*�������д�����ݹ��˰������ݵ�����*/
BSP_S32 BSP_IPF_ST_501(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32SendTime = 0;

    IPF_TEST_ENTRY();                    
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }   
    
    u32SendTime = omTimerGet();
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlLongPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 

    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();            
    IPF_PRINT("u32SendTime = 0x%x\n", u32SendTime);           
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;      
}
/*��������С�����ݹ��˰������ݵ�����*/
BSP_S32 BSP_IPF_ST_502(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32SendTime = 0;

    IPF_TEST_ENTRY();                    
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }   
    
    u32SendTime = omTimerGet();
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 

    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();            
    IPF_PRINT("u32SendTime = 0x%x\n", u32SendTime);           
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;      
}
/*�������д�����ݹ��˰������ݵ�����*/
BSP_S32 BSP_IPF_ST_503(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32RecvTime = 0;

    if(u32PacketNum > (IPF_DLAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ1(u32Adq1Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Speed, &stResultCheck, "IpfDlTask");  
    sema_init(&g_IpfDlIntAdq0NullSemId, 0);    
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb(IpfDlIntAdq0NullCb);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    u32RecvTime = (BSP_S32)omTimerGet();
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("BSP_IPF_ST_503, %d\n", __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("s32RecvTime = 0x%x\n", u32RecvTime);        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*��������С�����ݹ��˰������ݵ�����*/
BSP_S32 BSP_IPF_ST_504(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32RecvTime = 0;

    if(u32PacketNum > (IPF_DLAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Speed, &stResultCheck, "IpfDlTask");  
    sema_init(&g_IpfDlIntAdq0NullSemId, 0);    
    s32Ret = BSP_IPF_RegisterAdqEmptyDlCb(IpfDlIntAdq0NullCb);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }   

    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    u32RecvTime = omTimerGet();
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("u32RecvTime = 0x%x\n", u32RecvTime);        
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*�����ϡ����д�����ݹ��˰������ݵ�����*/
BSP_S32 ipf_sync_IntHandle()
{
    up(&g_ipfSyncSemId);

    return 0;
}
BSP_S32 BSP_IPF_ST_505_Step1(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    

    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
BSP_S32 BSP_IPF_ST_505_Step2(BSP_U32 u32PacketNum)
{
    BSP_U32 i = 0;    
    StreamIPv4Header stIPHeader = {0};
    StreamUDP stUDP = {0};
    BSP_U8* pu8IpData = BSP_NULL;
    IPF_CONFIG_ULPARAM_S *stUlPara = BSP_NULL;
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32UlSendTime = 0;
    BSP_U32 u32DlRecvTime = 0;    

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }  

    IPF_PRINT("CCPU ���������������\n");

    /* �ȴ�����һ��coreͬ����� */    
    g_pSyncState[SYNC_MODULE_IPF] = 0;
    s32Ret = BSP_SYNC_Wait(SYNC_MODULE_IPF, 10000);    
    if(s32Ret != BSP_OK)
    {
        IPF_PRINT("\r IPF BSP_SYNC_Wait error! \n");
        return ERROR;
    }
    u32UlSendTime = omTimerGet();
    
    stIPHeader.Vers = 4;
    stIPHeader.HeaderLen = 5;
    stIPHeader.TOS = 3;
    stIPHeader.TOL = LONG_IP_LEN;
    stIPHeader.ID = 1;
    stIPHeader.ReservFlag = 0;
    stIPHeader.DF = 0;
    stIPHeader.MF = 0;
    stIPHeader.offset = 0;
    stIPHeader.TTL = 128;
    stIPHeader.Prot = 17; /* UDP */
    stIPHeader.Checksum = 0xEC3A;
    stIPHeader.SourceAddr = 0xACA84649;
    stIPHeader.DestAddr = 0x0202014E;

    stUDP.SrcPort = 5088;
    stUDP.DstPort = 2923;

    pu8IpData = (BSP_U8*)BSP_SMALLOC(LONG_IP_LEN, MEM_ICC_DDR_POOL);
    if(pu8IpData == BSP_NULL)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }    
    memset(pu8IpData, 0x0, LONG_IP_LEN);
    memcpy(pu8IpData, &stIPHeader, sizeof(StreamIPv4Header));
    memcpy(pu8IpData+sizeof(StreamIPv4Header), &stUDP, sizeof(StreamUDP));
    memcpy(pu8IpData+sizeof(StreamIPv4Header)+sizeof(StreamUDP), pu8TestData, (sizeof(pu8TestData)-1));

    stUlPara = (IPF_CONFIG_ULPARAM_S*)kmalloc(u32PacketNum * sizeof(IPF_CONFIG_ULPARAM_S), GFP_KERNEL);
    if (!stUlPara)
    {
        IPF_PRINT("%s  %d  FAILED\n", __func__, __LINE__);
        return ERROR;
    } 
    stUlPara[0].u16Len = LONG_IP_LEN;
    stUlPara[0].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x0;
    stUlPara[0].u32Data = DDR_VIRT_TO_PHY(pu8IpData);
    stUlPara[0].u16Reserved = 0;    
    stUlPara[0].u16UsrField1 = IPF_MODEM_0;
    stUlPara[0].u32UsrField2 = 0x5A5A5A5A;
    stUlPara[0].u32UsrField3 = 0x55AA55AA;
    for(i = 1; i < u32PacketNum; i++)
    {
        memcpy(&stUlPara[i], &stUlPara[0], sizeof(IPF_CONFIG_ULPARAM_S));
    }
    stUlPara[--i].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x1;

    s32Ret = BSP_IPF_ConfigUpFilter(u32PacketNum, stUlPara);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        BSP_SFREE(pu8IpData);
        return ERROR;
    }
    
    down(&g_IpfDlTaskSemId);
    u32DlRecvTime = (BSP_S32)omTimerGet();
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("BSP_IPF_ST_503, %d\n", __LINE__);    
        return ERROR;
    }
    BSP_SFREE(pu8IpData);   
    pu8IpData = BSP_NULL;
    kfree(stUlPara);
    stUlPara = BSP_NULL;      
    IPF_PRINT("s32UlSendTime = 0x%x, s32DlRecvTime = 0x%x\n", u32UlSendTime, u32DlRecvTime);    

    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/*�����ϡ�����С�����ݹ��˰������ݵ�����*/
BSP_S32 BSP_IPF_ST_506_Step1(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    

    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;      
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
BSP_S32 BSP_IPF_ST_506_Step2(BSP_U32 u32PacketNum)
{
    BSP_U32 i = 0;    
    StreamIPv4Header stIPHeader = {0};
    StreamUDP stUDP = {0};
    BSP_U8* pu8IpData = BSP_NULL;
    IPF_CONFIG_ULPARAM_S *stUlPara = BSP_NULL;
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32UlSendTime = 0;
    BSP_U32 u32DlRecvTime = 0;    

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }  

    IPF_PRINT("CCPU ���������������\n");

    /* �ȴ�����һ��coreͬ����� */    
    g_pSyncState[SYNC_MODULE_IPF] = 0;    
    s32Ret = BSP_SYNC_Wait(SYNC_MODULE_IPF, 10000);    
    if(s32Ret != BSP_OK)
    {
        IPF_PRINT("\r IPF BSP_SYNC_Wait error! \n");
        return ERROR;
    }
    u32UlSendTime = omTimerGet();
    
    stIPHeader.Vers = 4;
    stIPHeader.HeaderLen = 5;
    stIPHeader.TOS = 3;
    stIPHeader.TOL = SHORT_IP_LEN;
    stIPHeader.ID = 1;
    stIPHeader.ReservFlag = 0;
    stIPHeader.DF = 0;
    stIPHeader.MF = 0;
    stIPHeader.offset = 0;
    stIPHeader.TTL = 128;
    stIPHeader.Prot = 17; /* UDP */
    stIPHeader.Checksum = 0xEC3A;
    stIPHeader.SourceAddr = 0xACA84649;
    stIPHeader.DestAddr = 0x0202014E;

    stUDP.SrcPort = 5088;
    stUDP.DstPort = 2923;

    pu8IpData = (BSP_U8*)BSP_SMALLOC(SHORT_IP_LEN, MEM_ICC_DDR_POOL);
    if(pu8IpData == BSP_NULL)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }    
    memset(pu8IpData, 0x0, SHORT_IP_LEN);
    memcpy(pu8IpData, &stIPHeader, sizeof(StreamIPv4Header));
    memcpy(pu8IpData+sizeof(StreamIPv4Header), &stUDP, sizeof(StreamUDP));
    memcpy(pu8IpData+sizeof(StreamIPv4Header)+sizeof(StreamUDP), pu8TestData, (sizeof(pu8TestData)-1));

    stUlPara = (IPF_CONFIG_ULPARAM_S*)kmalloc(u32PacketNum * sizeof(IPF_CONFIG_ULPARAM_S), GFP_KERNEL);
    if (!stUlPara)
    {
        IPF_PRINT("%s  %d  FAILED\n", __func__, __LINE__);
        return ERROR;
    } 
    stUlPara[0].u16Len = SHORT_IP_LEN;
    stUlPara[0].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x0;
    stUlPara[0].u32Data = DDR_VIRT_TO_PHY(pu8IpData);
    stUlPara[0].u16Reserved = 0;    
    stUlPara[0].u16UsrField1 = IPF_MODEM_0;
    stUlPara[0].u32UsrField2 = 0x5A5A5A5A;
    stUlPara[0].u32UsrField3 = 0x55AA55AA;
    for(i = 1; i < u32PacketNum; i++)
    {
        memcpy(&stUlPara[i], &stUlPara[0], sizeof(IPF_CONFIG_ULPARAM_S));
    }
    stUlPara[--i].u16Attribute = (IPF_MODEM0_ULFC <<4) | 0x1;

    s32Ret = BSP_IPF_ConfigUpFilter(u32PacketNum, stUlPara);
    if(s32Ret != IPF_SUCCESS)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        BSP_SFREE(pu8IpData);
        return ERROR;
    }
    
    down(&g_IpfDlTaskSemId);
    u32DlRecvTime = (BSP_S32)omTimerGet();
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("BSP_IPF_ST_503, %d\n", __LINE__);    
        return ERROR;
    }
    BSP_SFREE(pu8IpData);   
    pu8IpData = BSP_NULL;
    kfree(stUlPara);
    stUlPara = BSP_NULL;      
    IPF_PRINT("s32UlSendTime = 0x%x, s32DlRecvTime = 0x%x\n", u32UlSendTime, u32DlRecvTime);    

    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/* ���µ����һ: �Ĵ������ݻָ����� */
BSP_S32 BSP_IPF_ST_601(BSP_VOID)
{
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

}

/* ���µ���Զ�: ���˹��򱸷ݻָ����� */
BSP_S32 BSP_IPF_ST_602(BSP_VOID)
{
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

}

/* ���µ������: �������� */
/* ���в��� */
BSP_S32 BSP_IPF_ST_109_603(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;

    IPF_TEST_ENTRY();            
    if(u32PacketNum > (IPF_ULAD0_DESC_SIZE - 1))
    {
        IPF_PRINT("BSP_IPF_ST_102, %d\n", __LINE__);    
        goto err;
    }    
    
    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }    

    if(OK != IPF_ST_ConfigUlShortPacket(IPF_MODEM_0, u32PacketNum, BSP_TRUE))
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    ipfTestDelay(200);    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    } 
    IPF_TEST_LEAVE();
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;    

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}
/* ���в��� */
BSP_S32 BSP_IPF_ST_309_604(BSP_U32 u32PacketNum)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;        
    }
      
    IPF_ST_ConfigDlADQ0(u32Adq0Num);
    
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;    
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task, &stResultCheck, "IpfDlTask");      
    IPF_PRINT("CCPU ���������������\n");

    down(&g_IpfDlTaskSemId);
    if(u32PacketNum != stResultCheck.u32RdNum)
    {
        IPF_PRINT("TESTERROR    u32RDNum = %d, u32PacketNum = %d, %d\n", stResultCheck.u32RdNum, u32PacketNum, __LINE__);     
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        return ERROR;
    }
    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;
}
/* �����в������� */
BSP_S32 BSP_IPF_ST_401_605(BSP_U32 u32Count, BSP_U32 u32Tick)
{
    BSP_S32 s32Ret = 0;
    BSP_U32 u32BDNum = 0;
    BSP_U32 u32Adq0Num = 0;
    BSP_U32 u32Adq1Num = 0;    
    BSP_U32 u32CountTmp = u32Count;

    IPF_TEST_ENTRY();                
    s32Ret = BSP_IPF_GetDlAdNum(&u32Adq0Num, &u32Adq1Num);
    if(IPF_SUCCESS != s32Ret)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }
    else
    {
        IPF_ST_ConfigDlADQ0(u32Adq0Num);
        IPF_ST_ConfigDlADQ1(u32Adq1Num);        
    }
    sema_init(&g_IpfDlIntResultSemId, 0);
    s32Ret = BSP_IPF_RegisterWakeupDlCb(IpfDlIntResultCb);
    sema_init(&g_IpfDlTaskSemId, 0);    

    stResultCheck.u16Result = 5;
    stResultCheck.u16UsrField1 = IPF_MODEM_0;
    stResultCheck.u32UsrField2 = 0x5A5A5A5A;
    stResultCheck.u32UsrField3 = 0x55AA55AA;     
    stResultCheck.u32RdNum = 0;    
    g_IpfDlTaskId = kthread_run((int (*)(void *))IPF_DL_Task_Fn, &stResultCheck, "IpfDlTaskFn");  

    u32BDNum = BSP_IPF_GetUlBDNum();
    if(u32BDNum != IPF_ULBD_DESC_SIZE)
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }  

    ipfTestDelay(500);
    while(u32Count--)
    {
        u32BDNum = BSP_IPF_GetUlBDNum();
        if(u32BDNum > 0)
        {
            if(OK != IPF_ST_ConfigUlLongPacket(IPF_MODEM_0, 32, BSP_TRUE))
            {
                IPF_PRINT("%s, %d\n", __func__, __LINE__);    
                goto err;
            } 
        }
        ipfTestDelay(u32Tick);
    }

    while(stResultCheck.u32RdNum != (u32CountTmp * 32))
    {
        IPF_PRINT("%s, %d, u32RDNum = %d\n", __func__, __LINE__, stResultCheck.u32RdNum);    
        ipfTestDelay(2);
    }      
    
    s32Ret = BSP_IPF_UlStateIdle();
    if(s32Ret != IPF_SUCCESS)    
    {
        IPF_PRINT("%s, %d\n", __func__, __LINE__);    
        goto err;
    }     
    IPF_TEST_LEAVE();    
    IPF_PRINT("%s : SUCCESS!\n", __func__);
    return OK;

    err:
        IPF_TEST_LEAVE();
        return ERROR;     
}

EXPORT_SYMBOL(BSP_IPF_ST_001);
EXPORT_SYMBOL(BSP_IPF_ST_002);
EXPORT_SYMBOL(BSP_IPF_ST_003);
EXPORT_SYMBOL(BSP_IPF_ST_004);
EXPORT_SYMBOL(BSP_IPF_ST_101);
EXPORT_SYMBOL(BSP_IPF_ST_102);
EXPORT_SYMBOL(BSP_IPF_ST_103);
EXPORT_SYMBOL(BSP_IPF_ST_104);
EXPORT_SYMBOL(BSP_IPF_ST_105);
EXPORT_SYMBOL(BSP_IPF_ST_106);
EXPORT_SYMBOL(BSP_IPF_ST_107);
EXPORT_SYMBOL(BSP_IPF_ST_108);
EXPORT_SYMBOL(BSP_IPF_ST_109);
EXPORT_SYMBOL(BSP_IPF_ST_110);
EXPORT_SYMBOL(BSP_IPF_ST_301);
EXPORT_SYMBOL(BSP_IPF_ST_302);
EXPORT_SYMBOL(BSP_IPF_ST_303);
EXPORT_SYMBOL(BSP_IPF_ST_304);
EXPORT_SYMBOL(BSP_IPF_ST_305);
EXPORT_SYMBOL(BSP_IPF_ST_306);
EXPORT_SYMBOL(BSP_IPF_ST_307);
EXPORT_SYMBOL(BSP_IPF_ST_308);
EXPORT_SYMBOL(BSP_IPF_ST_309);
EXPORT_SYMBOL(BSP_IPF_ST_310);
EXPORT_SYMBOL(BSP_IPF_ST_401);
EXPORT_SYMBOL(BSP_IPF_ST_402);
EXPORT_SYMBOL(BSP_IPF_ST_501);
EXPORT_SYMBOL(BSP_IPF_ST_502);
EXPORT_SYMBOL(BSP_IPF_ST_503);
EXPORT_SYMBOL(BSP_IPF_ST_504);
EXPORT_SYMBOL(BSP_IPF_ST_505_Step1);
EXPORT_SYMBOL(BSP_IPF_ST_505_Step2);
EXPORT_SYMBOL(BSP_IPF_ST_506_Step1);
EXPORT_SYMBOL(BSP_IPF_ST_506_Step2);
EXPORT_SYMBOL(BSP_IPF_ST_601);
MODULE_AUTHOR("zhoushan@212992");
MODULE_DESCRIPTION("IPF TEST MODULE");
MODULE_LICENSE("GPL");
#endif

#ifdef __cplusplus
}
#endif

