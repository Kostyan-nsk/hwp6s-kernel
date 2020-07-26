/**********************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  icc_mntn.c
*
*   ��    �� :  zhaikuixiu
*
*   ��    �� :  ���ļ�����Ϊ"icc_mntn.c"
*
*   �޸ļ�¼ :  2011��12��07��  v1.00  zhaikuixiu����
************************************************************************************/

#include <linux/semaphore.h>    /*����mutex�����ͷ�ļ�*/
#include <linux/kernel.h>       /*   kmalloc,printk*/
#include <linux/slab.h>
#include "BSP.h"
#include <soc_ao_sctrl_interface.h>


#if(FEATURE_ICC_DEBUG == FEATURE_ON)
/*#include "omDrv.h"*/
#include "icc_mntn.h"
/*#include "excDrv.h"*/

/*extern OM_RSP_FUNC *g_pOmRspFuncPtr;*/
BSP_U32 g_icc_errlog_init_flag = 0;
BSP_U8   *g_drv_icc_buf    = NULL;
icc_mntn_info_header *g_icc_mntn_info_header  = NULL;
struct semaphore         g_drv_icc_mntn_mutex;
BSP_U32                  g_drv_icc_mntn_dbg_level      = 0x0;


/************om, now delete************/
/*extern unsigned int omTimerGet(void);
int icc_trace_pri_check(ICC_PRIMID traceType)
{
    printk("primdID:%d\n",traceType);
    return((0 == BSP_MNTN_DrvPrimCanSendOrNot((unsigned short)traceType)) ? 1 : 0);
}
*/

/*******************************************************************************
 �� �� ��:omTimerGet
 ��������:��ȡϵͳtimer�����sliceֵ
 �������:��
 �������:��
 �� �� ֵ:timer slice value
*******************************************************************************/
unsigned int omTimerGet(void)
{
    return (*(volatile unsigned int *)(SOC_AO_SCTRL_SC_ALWAYSON_SYS_STAT6_ADDR(IO_ADDRESS(SOC_SC_ON_BASE_ADDR))));
}


/***********************************************************************************
 Function:          BSP_ICC_Errlog_init
 Description:       ��ʼ��ICC������Ϣģ�飬������Դ
 Calls:
 Input:             NA
 Output:            NA
 Return:            NA
 ************************************************************************************/
 void BSP_MNTN_IccErrlogInit(void)
{
    BSP_U32 offset_mem = 0x0;

    printk("Enter BSP_MNTN_IccErrlogInit\n");

    if (1 == g_icc_errlog_init_flag)
    {
         printk("do not init again!\n");
         return;
    }

    /*�����е�Ҫ��SDT��ȥ���ڴ�ȫ�����������*/
    g_drv_icc_buf = (BSP_U8*)kmalloc(ICC_MNTN_ERRLOG_SIZE, GFP_KERNEL);
    if (NULL == g_drv_icc_buf)
    {
        printk("\n ICC Errlog kmalloc failed\n");
        goto ERR;
    }

    /*����USB��λ��Ϣ��ͷ*/

    /*ɾ��DRV_TRACE_IND_STRU�ṹ�����ĸ��ֽڣ�����DRV_TRACE_IND_STRU
	 ֻ��ʾ��Ϣͷ���������κ���ʵ���͵�����*/
    //offset_mem = (sizeof(DRV_TRACE_IND_STRU) - 4);
    offset_mem = (sizeof(DRV_TRACE_IND_STRU));

    g_icc_mntn_info_header = (icc_mntn_info_header*)(g_drv_icc_buf + offset_mem);

    g_icc_mntn_info_header->header_flag      = DRV_MNTN_HEADER_FLAG;
    g_icc_mntn_info_header->u32ChanID       = 0x0;
    g_icc_mntn_info_header->s32Errlog         = 0x0;
    g_icc_mntn_info_header->module_id         = 0x0;
    g_icc_mntn_info_header->prim_id             = 0x0;
    g_icc_mntn_info_header->s32Datesize      = 0x0;
    g_icc_mntn_info_header->time_stamp      =0x0;

    /*��ʼ�������ź���*/
    sema_init(&g_drv_icc_mntn_mutex, SEM_FULL);

    g_icc_errlog_init_flag = 1;
    g_drv_icc_mntn_dbg_level = 1;
    printk("MNTN_IccErrlogInit:header_flag:%d,u32ChanID:%d,s32Errlog:%d,module_id:%d,prim_id:%d,s32Datesize:%d,time_stamp:%d\n",g_icc_mntn_info_header->header_flag,\
        g_icc_mntn_info_header->u32ChanID,g_icc_mntn_info_header->s32Errlog,g_icc_mntn_info_header->module_id,g_icc_mntn_info_header->prim_id,g_icc_mntn_info_header->s32Datesize,g_icc_mntn_info_header->time_stamp );
    printk("g_icc_errlog_init_flag:%d,g_drv_icc_mntn_dbg_level:%d",g_icc_errlog_init_flag,g_drv_icc_mntn_dbg_level);
    return;

    ERR:
    BSP_MNTN_IccErrlogUnInit();
}

/***********************************************************************************
 Function:          BSP_MNTN_IccErrlogUnInit
 Description:       ��ʼ��ICC������Ϣģ�飬�ͷ���Դ
 Calls:
 Input:             NA
 Output:            NA
 Return:            NA
 ************************************************************************************/
void BSP_MNTN_IccErrlogUnInit(void)
{
    if (NULL != g_drv_icc_buf)
    {
        kfree(g_drv_icc_buf);
    }

   // g_usb_mntn_func_type_on = 0;
    g_icc_errlog_init_flag    = 0;
    g_drv_icc_mntn_dbg_level    = 0;

    return;
}
/***********************************************************************************
 Function:           BSP_ICC_Errlog_Save
 Description:        ��ICC���ͺͽ���ʱ��ʧ����Ϣ
                           ����OM�Ļص��������
 Calls:            BSP_MNTN_OmDrvTraceSend
 Input:           module_id
                     prim_id
                     buffer
 Output:             NA
 Return:             NA
 ************************************************************************************/
void BSP_ICC_Errlog_Save( BSP_U16 module_id, BSP_U16 prim_id, void  *buffer)
{
    BSP_U32        record_valid = 0x0;
    if(NULL == g_drv_icc_buf)
    {
        printk("#####not init#######\n");
        return;
    }




    if (!g_icc_errlog_init_flag)
    {
        printk("########drv icc mntn not init####### \n");
        return;
    }

    if (NULL == buffer)
    {
        printk("#######buffer is null#######\n");
        return;
    }

    down(&g_drv_icc_mntn_mutex);

    memset((void *)g_icc_mntn_info_header, 0, sizeof(icc_mntn_info_header));

	if (((ICC_PRIMID)prim_id >= DRV_PRIM_ICC_ERRLOG) && ((ICC_PRIMID)prim_id <= DRV_PRIM_ICC_ERRLOG))
	{
            g_icc_mntn_info_header->header_flag      = DRV_MNTN_HEADER_FLAG;
            g_icc_mntn_info_header->u32ChanID       = ((icc_mntn_info_t *)buffer)->u32ChanID;
            g_icc_mntn_info_header->s32Errlog         = ((icc_mntn_info_t *)buffer)->s32Errlog;
            g_icc_mntn_info_header->module_id         = module_id;
            g_icc_mntn_info_header->prim_id             =  prim_id;
            g_icc_mntn_info_header->s32Datesize      = ((icc_mntn_info_t *)buffer)->s32Datesize;
            g_icc_mntn_info_header->time_stamp      =   omTimerGet();

            printk("ICC_Errlog_Save:header_flag:%d,u32ChanID:%d,s32Errlog:%d,module_id:%d,prim_id:%d,s32Datesize:%d,time_stamp:%d\n",g_icc_mntn_info_header->header_flag,\
        g_icc_mntn_info_header->u32ChanID,g_icc_mntn_info_header->s32Errlog,g_icc_mntn_info_header->module_id,g_icc_mntn_info_header->prim_id,g_icc_mntn_info_header->s32Datesize,g_icc_mntn_info_header->time_stamp );//test
	    record_valid = 1;
        }

	else
	{
        printk("invalid prim_id\n");
	}

    printk("#######record_valid:%d#######\n",record_valid);

    /************om, now delete*************/
    /*
    if (record_valid)
    {
        trace_pri_check_flag=icc_trace_pri_check(prim_id);

	if(TRUE==trace_pri_check_flag)
	{
            printk("#######BSP_MNTN_OmDrvTraceSend#######\n");
            BSP_MNTN_OmDrvTraceSend(prim_id, module_id, (BSP_U8 *)g_icc_mntn_info_header,
            	                                            sizeof(icc_mntn_info_header));
	}
    }
    else
    {
         printk("error invalid record \n ");
    }
    */


    up(&g_drv_icc_mntn_mutex);
#ifdef MNTN_DBG
    printk("######GO out BSP_ICC_Errlog_Save#######\n");
#endif
}


#endif

