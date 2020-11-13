
#if 0   /*only for pc-lint*/
#include <kernel.h>
#include <kthread.h>
#include <module.h>
#include <slab.h>
#include <reset.h>
#include <soc_sctrl_interface.h>
#include <soc_ao_sctrl_interface.h>
#include <soc_wdt_interface.h>
#include <soc_baseaddr_interface.h>
#include <soc_mddrc_axi_interface.h>
#include <io.h>
#include "rfilesystem_process.h"
#include "rfilesystem_interface.h"
#include "BSP_IPF.h"
#include "bsp_icc_drv.h"
#include "bsp_drv_ipc.h"
#include "bsp_mailbox.h"
#include "usb_shell.h"
#include "excDrv.h"
#define KERN_INFO   "LEVEL2"
#define GFP_KERNEL  0
#else
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <mach/reset.h>
#include <soc_sctrl_interface.h>
#include <soc_ao_sctrl_interface.h>
#include <soc_wdt_interface.h>
#include <soc_baseaddr_interface.h>
#include <soc_mddrc_axi_interface.h>
#include <linux/io.h>
#include "MemoryMap.h"
#include "../../rfile/rfilesystem_process.h"
#include "../../rfile/rfilesystem_interface.h"
#include "BSP_IPF.h"
#include "../../multicore/icc/bsp_icc_drv.h"
#include "../../multicore/ipcm/bsp_drv_ipc.h"
#include "bsp_mailbox.h"
#include "../usb/usb_shell.h"
#include "linux/etb.h"
#include "../../mntn/excDrv.h"
#include <linux/notifier.h>         /* For notifier... */
#include "drv_timer.h"
#include <mach/util.h>				/* For mach_call_usermoduleshell*/
#include <linux/spinlock.h>
#include <soc_pmctrl_interface.h>
#include <bsp_ao_sctrl.h>
#include <mach/platform.h>
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
���Լ���������
*****************************************************************************/
#define CCORE_RESET_FLAG_VALUE      0x87654321  /*Ccore��λ��־���Ĵ���ֵ*/
#define CCORE_RESET_OK_FROM_CCORE      0x12345678  /*Ccore��λ�ɹ���־*/

/*C�˹����ֵ*/
#define CCORE_WDT_REG_VALUE_UNLOCK        0x1ACCE551
#define CCORE_WDT_REG_VALUE_LOCK          0x00000000

#define RESET_CBFUNC_PRIO_LEVEL_LOWT    0
#define RESET_CBFUNC_PRIO_LEVEL_HIGH    49
#define RESET_WAIT_TIMEOUT_MAILMSG      5000   /*ms, time of wating mail msg reply from hifi/mcu*/
#define RESET_WAIT_CCPU_STARTUP_TIMEOUT 30000  /*ms, time of wating mail msg reply from ccpu*/
#define RESET_WAIT_CCPU_ACKDONE_TIMEOUT 100    /*100us, time of wating ack done from ccpu*/

#define RESET_CBFUN_IGNORE_NAME         "NONAME"
/*��Ϊb090�Ժ��Ѿ�֧�ְ�printk��ӡ��Ϣ�洢���ļ������Բ���Ҫ��ģ���Լ��洢*/
#define PRINK_TO_FILE

#ifndef NULL
#define NULL    0
#endif

#define CCORE_RESET_TASK_PHASE_SET(a)     (g_stResetDebugInfo.ulCcpuTaskPhase = (a))
#define CCORE_RESET_TASK_PHASE_GET()      (g_stResetDebugInfo.ulCcpuTaskPhase)

/*����Э����ɸ�λ����*/
sreset_mgr_assistant   g_reset_assistant;
/*Link used for c core reset*/
sreset_mgr_LLI  *g_pmgr_ccorereset_data = NULL;
/*Link used for hifi reset*/
sreset_mgr_LLI  *g_pmgr_hifireset_data = NULL;
/*single reset debug info*/
sreset_debug_info g_stResetDebugInfo;

/*****************************************************************************
��������
*****************************************************************************/
int ccorereset_enable_wdt_irq(void);

extern void reset_set_cpu_status(unsigned int iOff, unsigned int iflag);
extern int ccorereset_loadcbin_loadaddr(void);
int platform_ddr_protect_init(int flag);
extern void codec_btn_simu_func(int pressed_button);
#ifndef _DRV_LLT_
extern unsigned int g_modem_subsys_freeze_flag;
extern int modem_subsys_mdm_edma_access_get(void);
extern int modem_subsys_lcipher_access_get(void);
extern int modem_subsys_bbe_access_get(void);
extern int modem_subsys_bbp_access_get(void);
#define MODEM_SUBSYS_DEBUG_ADDR (0xF7600000)
unsigned int g_modem_subsys_freeze_flag = 0;
#endif
/*****************************************************************************
��������
*****************************************************************************/
IFC_GEN_CALL1(MAILBOX_IFC_ACPU_TO_CCPU_RST_MODEM, BSP_CCPU_Reset_Byhimself,
                IFC_INCNT, int, is_normal, 0)

/*****************************************************************************
 �� �� ��  : mailbox_recfun_mcu
 ��������  : ���ڽ���������MCU��MAILBOX��Ϣ
 �������  : void  *UserHandle, �û����
            void *MailHandle, ������
            unsigned long MailLen, ���ݳ���
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void mailbox_recfun_mcu(void  *UserHandle,void *MailHandle,unsigned long MailLen)
{
    unsigned long   ulresult = 0;
    unsigned long   msg_len = sizeof(int);
    int msg_mcu = 0;

    ulresult = mailbox_read_msg_data(MailHandle, (unsigned char *)(&msg_mcu), &msg_len);
    printk(KERN_INFO "%s: receive mail from mcu, msg_mcu = %d,result = %ld\n", __FUNCTION__, msg_mcu, ulresult);
    if (0 == msg_mcu)
    {
        up(&(g_reset_assistant.sem_wait_mcu_msg));
    }
    else
    {
        /*����log*/
        reset_no_ok_savelog("mcu", (int)msg_mcu, DRV_RESET_CALLCBFUN_RESET_BEFORE, BSP_RESET_MODULE_CCORE);

        /*��λϵͳ*/
        printk(KERN_ERR "%s: mcu give wrong msg, msg is %d\n", __FUNCTION__, msg_mcu);
        do_reset_system(RESET_TYPE_MSG_FROM_MCU_WRONG);
    }
}
/*****************************************************************************
 �� �� ��  : mailbox_recfun_mcu_hifireset
 ��������  : ���ڽ���������MCU��MAILBOX��Ϣ
 �������  : void  *UserHandle, �û����
            void *MailHandle, ������
            unsigned long MailLen, ���ݳ���
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void mailbox_recfun_mcu_hifireset(void  *UserHandle,void *MailHandle,unsigned long MailLen)
{
    unsigned long   ulresult = 0;
    unsigned long   msg_len = sizeof(int);
    int msg_mcu = 0;

    ulresult = mailbox_read_msg_data(MailHandle, (unsigned char *)(&msg_mcu), &msg_len);
    printk(KERN_INFO "%s: receive mail from mcu, msg_mcu = %d,result = %ld\n", __FUNCTION__, msg_mcu, ulresult);
    if (1 == msg_mcu)
    {
        up(&(g_reset_assistant.sem_wait_mcu_msg_hifireset));
    }
    else
    {
        /*��λϵͳ*/
        printk(KERN_INFO "%s: mcu give wrong msg, msg is %d\n", __FUNCTION__, msg_mcu);
        do_reset_system(RESET_TYPE_MSG_FROM_MCU_WRONG);
    }
}

/*****************************************************************************
 �� �� ��  : mailbox_recfun_hifi
 ��������  : ���ڽ���������HIFI��MAILBOX��Ϣ
 �������  : void  *UserHandle, �û����
            void *MailHandle, ������
            unsigned long MailLen, ���ݳ���
 �� �� ֵ  : void
*****************************************************************************/
void mailbox_recfun_hifi(void  *UserHandle,void *MailHandle,unsigned long MailLen)
{
    unsigned long   ulresult = 0;
    unsigned long   msg_len = sizeof(HIFI_AP_CCPU_RESET_CNF_STRU);
    HIFI_AP_CCPU_RESET_CNF_STRU msg_hifi = {0};

    ulresult = mailbox_read_msg_data(MailHandle, (unsigned char *)(&msg_hifi), &msg_len);
    printk(KERN_INFO "%s: receive mail from hifi, msg_hifi ret = %d,result = %ld\n", __FUNCTION__, msg_hifi.uhwResult, ulresult);

    if (ID_HIFI_AP_CCPU_RESET_CNF == msg_hifi.uhwMsgId
        && 0 == msg_hifi.uhwResult)
    {
        up(&(g_reset_assistant.sem_wait_hifi_msg));
    }
    else
    {
        /*��λϵͳ*/
        printk(KERN_ERR "%s: hifi's msg is wrong\n", __FUNCTION__);
        do_reset_system(RESET_TYPE_MSG_FROM_HIFI_WRONG);
    }
}
/*****************************************************************************
 �� �� ��  : reset_do_regcbfunc
 ��������  : �����������ע��ص�����������Modem��λǰ��������ݡ�
 �������  :
         sreset_mgr_LLI *plink,��������ע�⣬����Ϊ��.
            const char *pname, ���ע�������
         pdrv_reset_cbfun cbfun,    ���ע��Ļص�����
         int userdata,�����˽������
         Int Priolevel, �ص������������ȼ� 0-49������0-9 ������
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
sreset_mgr_LLI * reset_do_regcbfunc(sreset_mgr_LLI *plink, const char *pname, pdrv_reset_cbfun pcbfun, int userdata, int priolevel)
{
    sreset_mgr_LLI  *phead = plink;
    sreset_mgr_LLI  *pmgr_unit = NULL;

    /*�ж�����Ƿ�Ϸ������Ϸ����ش���*/
    if (NULL == pname
        || NULL == pcbfun
        || (priolevel < RESET_CBFUNC_PRIO_LEVEL_LOWT || priolevel > RESET_CBFUNC_PRIO_LEVEL_HIGH))
    {
        printk(KERN_ERR "%s: fail in ccore reset regcb,fail, name 0x%x, cbfun 0x%x, prio %d\n", __FUNCTION__, \
                        (unsigned int)pname, (unsigned int)pcbfun, priolevel);
        return NULL;
    }

    /*����ռ�*/
    pmgr_unit = (sreset_mgr_LLI*)kmalloc(sizeof(sreset_mgr_LLI), GFP_KERNEL);
    if (NULL != pmgr_unit)
    {
        memset((void*)pmgr_unit, 0, (sizeof(sreset_mgr_LLI)));
        /*��ֵ*/
        strncpy(pmgr_unit->cbfuninfo.name, pname, DRV_RESET_MODULE_NAME_LEN);
        pmgr_unit->cbfuninfo.priolevel = priolevel;
        pmgr_unit->cbfuninfo.userdata = userdata;
        pmgr_unit->cbfuninfo.cbfun = pcbfun;
    }

    /*��һ�ε��øú���������Ϊ��*/
    if (NULL == phead)
    {
        phead = pmgr_unit;
    }
    else
    {
    /*�������ȼ���������*/
        phead = reset_link_insert(phead, pmgr_unit);
    }
    return phead;
}

/*****************************************************************************
 �� �� ��  : ccorereset_regcbfunc
 ��������  : �����������ע��ص�����������Modem��λǰ��������ݡ�
 �������  : const char *pname, ���ע�������
         pdrv_reset_cbfun cbfun,    ���ע��Ļص�����
         int userdata,�����˽������
         Int Priolevel, �ص������������ȼ� 0-49������0-9 ������
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_regcbfunc(const char *pname, pdrv_reset_cbfun pcbfun, int userdata, int priolevel)
{
    g_pmgr_ccorereset_data = reset_do_regcbfunc(g_pmgr_ccorereset_data, pname, pcbfun, userdata, priolevel);
    printk(KERN_INFO "%s: %s registered a cbfun for ccore reset\n", __FUNCTION__, pname);
    return BSP_RESET_OK;
}


/*****************************************************************************
 �� �� ��  : hifireset_regcbfunc
 ��������  : �����������ע��ص�����������HIFI��λǰ��������ݡ�
 �������  : const char *pname, ���ע�������
         pdrv_reset_cbfun cbfun,    ���ע��Ļص�����
         int userdata,�����˽������
         Int Priolevel, �ص������������ȼ� 0-49������0-9 ������
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int hifireset_regcbfunc(const char *pname, pdrv_reset_cbfun pcbfun, int userdata, int priolevel)
{
    g_pmgr_hifireset_data = reset_do_regcbfunc(g_pmgr_hifireset_data, pname, pcbfun, userdata, priolevel);
    printk(KERN_INFO "%s: %s registered a cbfun for hifi reset\n", __FUNCTION__, pname);
    return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : ccorereset_task
 ��������  : ccpu��λ��Ϣ֪ͨ����cpu��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_notify_other_cpu(struct notifier_block *cb, unsigned long code, void *p)
{
        int iResult = BSP_RESET_OK;

        /*�����Ϣ����*/
        g_reset_assistant.smailboxmsg_mcu.iproctec = BSP_RESET_MAILBOX_MSG_PROTECT;
        g_reset_assistant.smailboxmsg_mcu.idata = BSP_RESET_MCU_MSG_CCORE_RESET_BEFORE;
        /*��ϢID*/
        g_reset_assistant.smailboxmsg_hifi.uhwMsgId = ID_AP_HIFI_CCPU_RESET_REQ;

        /*������Ϣ��HIFI*/
        printk(KERN_INFO "%s: msg id send to hifi is %d\n", __FUNCTION__, g_reset_assistant.smailboxmsg_hifi.uhwMsgId);
        iResult = ccorereset_tonotify(DRV_RESET_CALLCBFUN_RESET_BEFORE, BSP_RESET_MODULE_HIFI);
        if(BSP_RESET_OK != iResult)
        {
            printk(KERN_ERR "%s: fail send msg to hifi\n", __FUNCTION__);
            return BSP_RESET_ERROR;
        }
        iResult = ccorereset_tonotify(DRV_RESET_CALLCBFUN_RESET_BEFORE, BSP_RESET_MODULE_MCU);
        if(BSP_RESET_OK != iResult)
        {
            printk(KERN_ERR "%s: fail send msg to hifi\n", __FUNCTION__);
            return BSP_RESET_ERROR;
        }

        return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : ccorereset_task
 ��������  : �ȴ�����cpu������
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
void ccorereset_wait_other_cpu(void)
{
#ifndef _DRV_LLT_
        if (0 != down_timeout(&(g_reset_assistant.sem_wait_hifi_msg), msecs_to_jiffies(RESET_WAIT_TIMEOUT_MAILMSG)))
        {
            /*��λϵͳ*/
            printk(KERN_ERR "%s: fail to get mail from hifi,reset system\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_FAILGET_MSG_FROM_HIFI);
            return;
        }
#endif
        /*������Ϣ��MCU*/
        printk(KERN_INFO "%s: receive mail from hifi\n", __FUNCTION__);

#ifndef _DRV_LLT_
        if (0 != down_timeout(&(g_reset_assistant.sem_wait_mcu_msg), msecs_to_jiffies(RESET_WAIT_TIMEOUT_MAILMSG)))
        {
            /*��λϵͳ*/
            printk(KERN_ERR "%s: fail to get mail from mcu,reset system\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_FAILGET_MSG_FROM_MCU);
            return;
        }
#endif
        printk(KERN_INFO "%s: receive mail from mcu\n", __FUNCTION__);
        printk(KERN_INFO "%s: leave\n", __FUNCTION__);
}

/*****************************************************************************
 �� �� ��  : ccorereset_task
 ��������  : ���ڴ���Modem��λ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
#ifndef _DRV_LLT_
void modem_subsys_debug_init(void)
{
    unsigned int addr;
    g_modem_subsys_freeze_flag = 0;
    addr = (unsigned int)(ioremap(MODEM_SUBSYS_DEBUG_ADDR, (16 * 1024)));
    *(volatile unsigned int *)(addr + 0x3FFC) = 0;
    iounmap((void *)addr);
}

void modem_subsys_freeze_set(void)
{
    g_modem_subsys_freeze_flag = 1;
}

void modem_subsys_freeze_clr(void)
{
    g_modem_subsys_freeze_flag = 0;
}

int modem_subsys_freeze_get(void)
{
    return g_modem_subsys_freeze_flag;
}

void modem_subsys_flag_record(unsigned int pos)
{
    unsigned int addr;

    addr = (unsigned int)(ioremap(MODEM_SUBSYS_DEBUG_ADDR, (16 * 1024)));
    *(volatile unsigned int *)(addr + 0x3FFC) |= (1<<pos); 
    iounmap((void *)addr);
}

int modem_subsys_bbe_access_get(void)
{
	unsigned int reg_value0 = 0;
	unsigned int reg_value1 = 0;
	unsigned int reg_value3 = 0;
	unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);
	unsigned long sctrl_on = (unsigned long)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);

	reg_value0 = readl(SOC_SCTRL_SC_PERIPH_CLKSTAT9_ADDR(sctrl_off));
	reg_value1 = readl(SOC_SCTRL_SC_PERIPH_RSTSTAT6_ADDR(sctrl_off));
	reg_value3 = readl(SOC_AO_SCTRL_SC_PW_MTCMOS_TIMEOUT_STAT0_ADDR(sctrl_on));
	if((0x1 == (0x1 & reg_value0)) && (0x0 == (0x1 & reg_value1)) && (0x400 == (0x400 & reg_value3))) 
    {
		return 0;
	}
	else 
    {
		return -1;
	}
}

int modem_subsys_bbp_access_get(void)
{
	unsigned int reg_value0 = 0;
	unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);

	reg_value0 = readl(SOC_SCTRL_SC_PERIPH_CLKSTAT9_ADDR(sctrl_off));

	if(0x80000 == (0x80000 & reg_value0)) 
    {
		return 0;
	}
	else 
    {
		return -1;
	}
}

int modem_subsys_mdm_edma_access_get(void)
{
	unsigned int reg_value0 = 0;
	unsigned int reg_value1 = 0;
	unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);


	reg_value0 = readl(SOC_SCTRL_SC_PERIPH_CLKSTAT9_ADDR(sctrl_off));
	reg_value1 = readl(SOC_SCTRL_SC_PERIPH_RSTSTAT6_ADDR(sctrl_off));
	if((0x180 == (0x180 & reg_value0)) && (0x0 == (0x800 & reg_value1))) 
    {
		return 0;
	}
	else 
    {
		return -1;
	}
}
int modem_subsys_lcipher_access_get(void)
{
	unsigned int reg_value0 = 0;
	unsigned int reg_value1 = 0;
	unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);


	reg_value0 = readl(SOC_SCTRL_SC_PERIPH_CLKSTAT9_ADDR(sctrl_off));
	reg_value1 = readl(SOC_SCTRL_SC_PERIPH_RSTSTAT6_ADDR(sctrl_off));
	if((0x1C00 == (0x1C00 & reg_value0)) && (0x0 == (0x2000 & reg_value1))) 
    {
		return 0;
	}
	else 
    {
		return -1;
	}
}

int modem_subsys_msram_access_status_get(void)
{
    int reg_value0,reg_value1;
    unsigned long sctrl_off,pm_ctrl;
    
    sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);
    pm_ctrl   = (unsigned long)IO_ADDRESS(SOC_PMCTRL_BASE_ADDR);

	reg_value0 = readl(SOC_SCTRL_SC_PERIPH_CLKSTAT9_ADDR(sctrl_off));
    reg_value1 = readl(SOC_PMCTRL_MCPUPLLCTRL_ADDR(pm_ctrl));
    
    if(1 != (reg_value1&0x1))
    {
       modem_subsys_flag_record(18);
       return -1;
        
    }

    if(0xf80000 !=(reg_value0&0xf80000))
    {
        modem_subsys_flag_record(19);
        return -1;
    }
    return 0;
}

void modem_subsys_debug_bbe_save(void)
{
	unsigned int addr = 0;
	unsigned int reg_value = 0;

	if(-1 == modem_subsys_bbe_access_get()) 
    {
		modem_subsys_flag_record(0);
        return;
    }
    modem_subsys_flag_record(1);
    addr = (unsigned int)(ioremap(SOC_Tensilica_BBE16_DTCM_BASE_ADDR, 1024));
	reg_value = *(volatile unsigned int*)(addr);
    modem_subsys_flag_record(2);
	iounmap((void *)addr);
}

void modem_subsys_debug_bbp_save(void)
{
	unsigned int addr = 0;
	unsigned int reg_value = 0;


	if(-1 == modem_subsys_bbp_access_get()) 
    {
		modem_subsys_flag_record(3);
        return;
    }
    modem_subsys_flag_record(4);
    addr = (unsigned int)(ioremap(SOC_BBP_COMM_BASE_ADDR, 1024));
	reg_value = *(volatile unsigned int*)(addr+0xa00c);
    modem_subsys_flag_record(5);
	iounmap((void *)addr);
}
void modem_subsys_debug_mdm_edma_save(void)
{
	unsigned int addr = 0;
	unsigned int reg_value = 0;

	if(-1 == modem_subsys_mdm_edma_access_get()) 
    {
		modem_subsys_flag_record(6);
        return;
    }
    modem_subsys_flag_record(7);
    addr = (unsigned int)(ioremap(SOC_Modem_DMAC_BASE_ADDR, 1024));
	reg_value = *(volatile unsigned int*)(addr + 0x58);
    modem_subsys_flag_record(8);
	iounmap((void *)addr);
}
void modem_subsys_debug_lcipher_save(void)
{
	unsigned int addr = 0;
	unsigned int reg_value = 0;

	if(-1 == modem_subsys_lcipher_access_get()) 
    {
		modem_subsys_flag_record(9);
        return;
    }
    modem_subsys_flag_record(10);
    addr = (unsigned int)(ioremap(SOC_LCipher_BASE_ADDR, 1024));
	reg_value = *(volatile unsigned int*)(addr);

    modem_subsys_flag_record(11);
	iounmap((void *)addr);
}

void modem_subsys_debug_mdm_sram_save(void)
{
	unsigned int addr = 0;
	unsigned int reg_value = 0;

	if(-1 == modem_subsys_msram_access_status_get()) 
    {
        modem_subsys_flag_record(12);
		return;
	}
    modem_subsys_flag_record(13);
    addr = (unsigned int)(ioremap(SOC_SRAM_MDM_BASE_ADDR, 1024));
	reg_value = *(volatile unsigned int*)(addr);
    modem_subsys_flag_record(14);
	iounmap((void *)addr);
}
void modem_subsys_debug_info_save(void)
{
    printk(KERN_ERR"modem_subsys_debug_info_save in/n");
    if(!modem_subsys_freeze_get())
    {   
        modem_subsys_flag_record(15);
        return;
    } 
    modem_subsys_flag_record(16);
	modem_subsys_freeze_clr();
	modem_subsys_debug_bbe_save();
    modem_subsys_debug_bbp_save();
	modem_subsys_debug_mdm_edma_save();
	modem_subsys_debug_lcipher_save();
	modem_subsys_debug_mdm_sram_save();
    modem_subsys_flag_record(17);
}
#endif
int ccorereset_task(void *arg)
{
    int iresult = BSP_RESET_OK;

#ifdef _DRV_LLT_
#else
    for ( ; ; )
#endif
    {
        down(&(g_reset_assistant.sem_wait_ccorereset));
        printk(KERN_ERR "%s: enter\n", __FUNCTION__);
        CCORE_RESET_TASK_PHASE_SET(0);

        /*�ȴ�����cpu�Ļظ�,������Ҫָhifi��mcu*/
        ccorereset_wait_other_cpu();
        CCORE_RESET_TASK_PHASE_SET(1);

        /*���ø�λǰ�����ע��ص�����*/
        iresult = ccorereset_runcbfun(DRV_RESET_CALLCBFUN_RESET_BEFORE);
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to run callback fun before reset\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_RUNCB_STEP1_FAIL);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(2);
        printk(KERN_ERR"begin reset ccore/n");
        /*��λccpu��modem uart�Լ�ipf��*/
        iresult = ccorereset_doreset();
        printk(KERN_ERR"end reset ccore,ret:%d/n",iresult);
        if (BSP_RESET_OK != iresult)
        {
            #ifndef _DRV_LLT_
			modem_subsys_debug_info_save();
			#endif
			printk(KERN_ERR "%s:: fail to reset ccore\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_DORESET);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(3);

#ifdef _DRV_LLT_
#else
       /*���¼���BBE16��ӳ��*/
        iresult = ccorereset_loadbbebin();
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to load bbe16 bin\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_LOAD_BBE16_FAIL);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(4);

       /*���¼���C��ӳ��*/
        iresult = ccorereset_loadcbin();
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to load ccpu bin\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_LOAD_CCPU_FAIL);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(5);
#endif
        /*���ڱ�ʾccore�Ѿ���λ*/
        g_reset_assistant.iccore_reset = 1;

        /*���üĴ���ֵ,����ccpu���Ǹ�λ�󿪻�*/
        (void)ccorereset_setCresetFlag();
        CCORE_RESET_TASK_PHASE_SET(6);

        /*�⸴λccpu*/
        (void)ccorereset_dostart();
        CCORE_RESET_TASK_PHASE_SET(7);

        enable_etm_reset();
        CCORE_RESET_TASK_PHASE_SET(8);

        /*���÷��͸�mcu��mailbox����ֵ,������Ϣ��MCU*/
        g_reset_assistant.smailboxmsg_mcu.iproctec = BSP_RESET_MAILBOX_MSG_PROTECT;
        g_reset_assistant.smailboxmsg_mcu.idata = BSP_RESET_MCU_MSG_CCORE_RESET_AFTER;
        iresult = ccorereset_tonotify(DRV_RESET_CALLCBFUN_RESET_AFTER, BSP_RESET_MODULE_MCU);
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to notify mcu after reset\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_NOTIFY_AFTER);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(9);

        /*�ȴ�ccpu�����ɹ�֪ͨ*/
        printk(KERN_ERR "%s: wait for ccore startup finish\n", __FUNCTION__);
        if (0 != down_timeout(&(g_reset_assistant.sem_wait_ccorereset_ok), msecs_to_jiffies(RESET_WAIT_CCPU_STARTUP_TIMEOUT)))
        {
            printk(KERN_ERR "%s: wait for ccpu startup, timeout\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_CRUN_NO_OK);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(10);

        /*���ø�λ��ص�����*/
        iresult = ccorereset_runcbfun(DRV_RESET_CALLCBFUN_RESET_AFTER);
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to run after-cb fun\n", __FUNCTION__);
            /*��λϵͳ*/
            do_reset_system(RESET_TYPE_CRESET_RUNCB_STEP2_FAIL);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(11);

        /*���÷��͸�mcu��mailbox����ֵ,������Ϣ��MCU*/
        g_reset_assistant.smailboxmsg_mcu.iproctec = BSP_RESET_MAILBOX_MSG_PROTECT;
        g_reset_assistant.smailboxmsg_mcu.idata = BSP_RESET_MCU_MSG_CCORE_RESET_FINISH;
        iresult = ccorereset_tonotify(DRV_RESET_CALLCBFUN_RESET_AFTER, BSP_RESET_MODULE_MCU);
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to notify mcu finish reset\n", __FUNCTION__);
            do_reset_system(RESET_TYPE_CRESET_NOTIFY_FINISH);
            return BSP_RESET_ERROR;
        }
        CCORE_RESET_TASK_PHASE_SET(12);
        
        //mach_call_usermodeshell("/system/etc/log/crash_notice.sh",0);/*notice LogServer to handle log-bins*/
        
        printk(KERN_ERR "%s: leave\n", __FUNCTION__);
    }
}

/*****************************************************************************
 �� �� ��  : hifireset_task
 ��������  : ���ڴ���HIFI��λ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int hifireset_task(void *arg)
{
    int iresult = BSP_RESET_OK;

#ifdef _DRV_LLT_
#else
    for ( ; ; )
#endif
    {
        printk(KERN_INFO "%s: enter hifireset_task\n", __FUNCTION__);
        down(&(g_reset_assistant.sem_wait_hifireset));
        printk(KERN_INFO "%s: hifi reset int is coming!\n", __FUNCTION__);
        reset_for_savelog("\n=============hifi reset=============\n");

        /*���ûص�����*/
        iresult = hifireset_runcbfun(DRV_RESET_CALLCBFUN_RESET_BEFORE);
        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to run cb func before hifi reset\n", __FUNCTION__);

            /*��λϵͳ*/
            do_reset_system(RESET_TYPE_HIFIRESET_RUNCB_STEP1_FAIL);
            return BSP_RESET_ERROR;
        }
#ifdef _DRV_LLT_
#else
        if (0 != down_timeout(&(g_reset_assistant.sem_wait_mcu_msg_hifireset), msecs_to_jiffies(RESET_WAIT_TIMEOUT_MAILMSG)))
        {
            /*��λϵͳ*/
            printk(KERN_ERR "%s: fail to get mail from mcu,reset system\n", __FUNCTION__);
            /*do_reset_system(RESET_TYPE_FAILGET_MSG_FROM_MCU);*/
            return BSP_RESET_ERROR;
        }
        printk(KERN_INFO "%s: to load hifi bin!\n", __FUNCTION__);
        reset_for_savelog("To load hifi bin\n");
       /*���¼���HIFIӳ��*/
        iresult = hifireset_loadhifibin();
        if (BSP_RESET_OK != iresult)
        {
            reset_for_savelog("fail to load hifi bin\n");
            printk(KERN_ERR "%s: fail to load hifi bin! reset system\n", __FUNCTION__);
            /*��λϵͳ*/
            do_reset_system(RESET_TYPE_HIFIRESET_LOAD_BIN_FAIL);
            return BSP_RESET_ERROR;
        }
#endif

        printk(KERN_INFO "%s: to run cb fun after hifi reset!\n", __FUNCTION__);
        reset_for_savelog("To run cb func after hifi reset\n");
        /*HIFI������Ϻ󣬵��ûص�����*/
        iresult = hifireset_runcbfun(DRV_RESET_CALLCBFUN_RESET_AFTER);

        /*�ָ��ж�ʹ��*/
        finish_reset_sub();

        if (BSP_RESET_OK != iresult)
        {
            printk(KERN_ERR "%s: fail to run cb fun after hifi reset! reset system\n", __FUNCTION__);
            /*��λϵͳ*/
            do_reset_system(RESET_TYPE_HIFIRESET_RUNCB_STEP2_FAIL);
            return BSP_RESET_ERROR;
        }
        printk(KERN_INFO "%s: reset hifi successfully!\n", __FUNCTION__);
        reset_for_savelog("hifi reset ok\n");
    }
}

/*****************************************************************************
 �� �� ��  : ccorereset_runcbfun
 ��������  : Modem��λǰ����ûص������ĺ�����
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam, 0 ��ʾmodem��λǰ�������ʾ��λ��

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int ccorereset_runcbfun (DRV_RESET_CALLCBFUN_MOMENT eparam)
{
    int  iresult = 0;

    sreset_mgr_LLI  *phead = g_pmgr_ccorereset_data;

    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        reset_set_cpu_status(1, RESET_CPU_CCORE_STATUS_OFF);
    }
    else
    {
        reset_set_cpu_status(0, RESET_CPU_CCORE_STATUS_OFF);
    }
    /*���ݻص��������ȼ������ûص�����*/
    while (NULL != phead)
    {
        if (NULL != phead->cbfuninfo.cbfun)
        {
            iresult = phead->cbfuninfo.cbfun(eparam, phead->cbfuninfo.userdata);
            if (BSP_RESET_OK != iresult)
            {
                /*�������ʧ�ܣ���¼���������,����ֵ*/
                printk(KERN_ERR "%s: fail to run cbfun of %s, at %d return %d\n", __FUNCTION__, phead->cbfuninfo.name, eparam, iresult);
                break;
            }
            printk(KERN_INFO "%s: run %s cb function 0x%x success\n", __FUNCTION__, phead->cbfuninfo.name, (unsigned int)phead->cbfuninfo.cbfun);
        }
        phead = phead->pnext;
    }
    printk(KERN_INFO "%s: leave, eparam = %d, iresult = %d\n", __FUNCTION__, eparam, iresult);
    return iresult;
}

/*****************************************************************************
 �� �� ��  :  hifireset_doruncbfun
 ��������  : HIFI��λǰ����ûص������ĺ���������Ȧ���Ӷȳ��꣬���������װ����
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam, 0 ��ʾHIFI��λǰ�������ʾ��λ��

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int hifireset_doruncbfun (const char *pname, DRV_RESET_CALLCBFUN_MOMENT eparam)
{
    int  iresult = BSP_RESET_OK;

    sreset_mgr_LLI  *phead = g_pmgr_hifireset_data;

    if (NULL != pname)
    {
        /*���ж�ģ������,��˳��ִ��*/
        if (strcmp(pname, RESET_CBFUN_IGNORE_NAME) == 0)
        {
            while (NULL != phead)
            {
                if (NULL != phead->cbfuninfo.cbfun)
                {
                    iresult = phead->cbfuninfo.cbfun(eparam, phead->cbfuninfo.userdata);
                    if (BSP_RESET_OK != iresult)
                    {
                        /*�������ʧ�ܣ���¼���������,����ֵ*/
                        reset_no_ok_savelog(phead->cbfuninfo.name, iresult, eparam, BSP_RESET_MODULE_CCORE);
                        printk(KERN_ERR "%s: fail to run cbfun of %s, at %d return %d\n", __FUNCTION__, phead->cbfuninfo.name, eparam, iresult);
                        break;
                    }
                    printk(KERN_INFO "%s: run %s cb function 0x%x\n", __FUNCTION__, phead->cbfuninfo.name, (unsigned int)phead->cbfuninfo.cbfun);
                }
                phead = phead->pnext;
            }
        }
        else/*��Ҫ�ж�ģ�����֣�ִ��ָ���Ļص�����*/
        {
            while (NULL != phead)
            {
                if (strcmp(pname, phead->cbfuninfo.name) == 0
                    && NULL != phead->cbfuninfo.cbfun)
                {
                    iresult  = phead->cbfuninfo.cbfun(eparam, phead->cbfuninfo.userdata);
                    printk(KERN_INFO "%s: run %s cb function 0x%x\n", __FUNCTION__, phead->cbfuninfo.name, (unsigned int)phead->cbfuninfo.cbfun);
                    break;
                }
                phead = phead->pnext;
            }
        }
    }
    else
    {
        iresult = BSP_RESET_ERROR;
    }
    if (BSP_RESET_OK != iresult)
    {
        if (NULL != phead)
        {
            reset_no_ok_savelog(phead->cbfuninfo.name, iresult, eparam, BSP_RESET_MODULE_HIFI);
            printk(KERN_ERR "%s: fail to run cbfun of %s, at %d, return %d\n", __FUNCTION__, phead->cbfuninfo.name, eparam ,iresult);

        }
        else
        {
            printk(KERN_ERR "%s: fail to run cbfun, but phead or pname is null\n", __FUNCTION__);
        }
    }
    return iresult;
}

/*****************************************************************************
 �� �� ��  :  hifireset _runcbfun
 ��������  : HIFI��λǰ����ûص������ĺ�����
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam, 0 ��ʾHIFI��λǰ�������ʾ��λ��

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int hifireset_runcbfun (DRV_RESET_CALLCBFUN_MOMENT eparam)
{
    int  iresult = 0;

    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        /*Mailbox ����*/
        reset_set_cpu_status(1, RESET_CPU_HIFI_STATUS_OFF);
        /*�����ص�������������NAS�Ļص�*/
        iresult = hifireset_doruncbfun("NAS_AT", eparam);
        if (BSP_RESET_OK != iresult)
        {
        /*�������ʧ�ܣ���¼���齨name, ����ֵ�����浽�ļ�*/
            goto return_error;
        }

        /*�����ص���������������Ƶ�Ļص�*/
        iresult = hifireset_doruncbfun("CODEC", eparam);
        if (BSP_RESET_OK != iresult)
        {
        /*�������ʧ�ܣ���¼���齨name, ����ֵ�����浽�ļ�*/
            goto return_error;
        }

        /*֪ͨMCU*/
        g_reset_assistant.smailboxmsg_mcu.iproctec = BSP_RESET_MAILBOX_MSG_PROTECT;
        g_reset_assistant.smailboxmsg_mcu.idata = BSP_RESET_MCU_MSG_HIFI_RESET_BEFORE;
        hifireset_tonotify(eparam, BSP_RESET_MODULE_MCU);
        /*�����ص�������������OM�Ļص�*/
        iresult = hifireset_doruncbfun("OAM", eparam);
        if (BSP_RESET_OK != iresult)
        {
        /*�������ʧ�ܣ���¼���齨name, ����ֵ�����浽�ļ�*/
            goto return_error;
        }
    }
    else
    {
        /*֪ͨMCU*/
        g_reset_assistant.smailboxmsg_mcu.idata = BSP_RESET_MCU_MSG_HIFI_RESET_AFTER;
        hifireset_tonotify(eparam, BSP_RESET_MODULE_MCU);

        /*MAILBOX����*/
        reset_set_cpu_status(0, RESET_CPU_HIFI_STATUS_OFF);

        /*�����ص������������ûص�����*/
        iresult = hifireset_doruncbfun(RESET_CBFUN_IGNORE_NAME, eparam);
        if (BSP_RESET_OK != iresult)
        {
            goto return_error;
        }
    }
    printk(KERN_INFO "%s: end of run cb functions for hifi reset at %d, %d\n", __FUNCTION__, eparam, iresult);
    return BSP_RESET_OK;
return_error:
    return BSP_RESET_ERROR;
}
/*****************************************************************************
 �� �� ��  : reset_link_insert
 ��������  : �����ݲ�������
 �������  : sreset_mgr_LLI *plink, ����ָ��
             sreset_mgr_LLI *punit��������Ľڵ�ָ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
sreset_mgr_LLI * reset_link_insert(sreset_mgr_LLI *plink, sreset_mgr_LLI *punit)
{
    sreset_mgr_LLI    *phead = plink;
    sreset_mgr_LLI    *ppose = plink;
    sreset_mgr_LLI    *ptail = plink;

    if (NULL == plink || NULL == punit)
    {
        return NULL;
    }
    while (NULL != ppose)
    {
        /*�������ȼ����뵽������*/
        if (ppose->cbfuninfo.priolevel > punit->cbfuninfo.priolevel)
        {
            if (phead == ppose)
            {
                punit->pnext = ppose;
                phead = punit;
            }
            else
            {
                ptail->pnext = punit;
                punit->pnext = ppose;
            }
            break;
        }
        ptail = ppose;
        ppose = ppose->pnext;
    }
    if (NULL == ppose)
    {
        ptail->pnext = punit;
    }
    return phead;
}

/*****************************************************************************
 �� �� ��  : ccorereset_tonotify
 ��������  : CCORE��λʱ��֪ͨMCU����HIFI
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam,��λǰ���Ǹ�λ��
            int imodule, Ҫ֪ͨ��ģ�飬0��MCU;1,HIFI
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_tonotify(DRV_RESET_CALLCBFUN_MOMENT eparam, ereset_module emodule)
{
    unsigned long   result = 0;

    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        if (BSP_RESET_MODULE_MCU == emodule)
        {
            result = mailbox_send_msg(BSP_MAILBOX_CHANNEL_ACPU_TO_MCU_RST_CH,
                (void *)(&g_reset_assistant.smailboxmsg_mcu), sizeof(g_reset_assistant.smailboxmsg_mcu));
        }
        else if (BSP_RESET_MODULE_HIFI == emodule)
        {
            result = mailbox_send_msg(MAILBOX_MAILCODE_ACPU_TO_HIFI_CCORE_RESET_ID,
                (void *)(&g_reset_assistant.smailboxmsg_hifi), sizeof(g_reset_assistant.smailboxmsg_hifi));
        }
    }
    else
    {
        if (BSP_RESET_MODULE_MCU == emodule)
        {
            result = mailbox_send_msg(BSP_MAILBOX_CHANNEL_ACPU_TO_MCU_RST_CH,
                (void *)(&g_reset_assistant.smailboxmsg_mcu), sizeof(g_reset_assistant.smailboxmsg_mcu));
        }
        else
        {
            /*printk(KERN_INFO "%s: to notify none in ccorereset_tonotify\n", __FUNCTION__);*/
        }
    }
    /*printk(KERN_INFO "%s: send mail to %d, at %d, result=%d\n", __FUNCTION__, emodule, eparam, result);*/

    return ((MAILBOX_OK == result) ? BSP_RESET_OK : BSP_RESET_ERROR);
}

/*****************************************************************************
 �� �� ��  : ccorereset_tonotify
 ��������  : HIFI��λʱ��֪ͨMCU
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam,��λǰ���Ǹ�λ��
            int imodule, Ҫ֪ͨ��ģ�飬0��MCU;1,HIFI
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int hifireset_tonotify(DRV_RESET_CALLCBFUN_MOMENT eparam, ereset_module emodule)
{
    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        if (BSP_RESET_MODULE_MCU == emodule)
        {
            mailbox_send_msg(BSP_MAILBOX_CHANNEL_ACPU_TO_MCU_RST_CH,
                (void *)(&g_reset_assistant.smailboxmsg_mcu), sizeof(g_reset_assistant.smailboxmsg_mcu));
        }
        else
        {
            printk(KERN_ERR "%s: to notify none in hifireset_tonotify\n", __FUNCTION__);
        }
    }
    else
    {
        if (BSP_RESET_MODULE_MCU == emodule)
        {
            mailbox_send_msg(BSP_MAILBOX_CHANNEL_ACPU_TO_MCU_RST_CH,
                (void *)(&g_reset_assistant.smailboxmsg_mcu), sizeof(g_reset_assistant.smailboxmsg_mcu));
        }
        else
        {
            printk(KERN_ERR "%s: to notify none in hifireset_tonotify\n", __FUNCTION__);
        }
    }
    printk(KERN_INFO "%s: after send mail to %d, %d\n", __FUNCTION__, emodule, eparam);
    return BSP_RESET_OK;
}
/*****************************************************************************
 �� �� ��  : ccorereset_setCresetFlag
 ��������  : ���ø�λCCORE��־�Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_setCresetFlag(void)
{
#ifdef _DRV_LLT_
#else
    unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);
    unsigned long uvalue = 0;

    uvalue = readl(SOC_SCTRL_SC_RESERVED16_ADDR(sctrl_off));
    printk(KERN_INFO "%s: org value = 0x%lx\n", __FUNCTION__, uvalue);

    writel(CCORE_RESET_FLAG_VALUE, SOC_SCTRL_SC_RESERVED16_ADDR(sctrl_off));

#endif
    printk(KERN_INFO "%s: leave\n", __FUNCTION__);
    return 0;
}
/*****************************************************************************
 �� �� ��  : ccorereset_doreset
 ��������  : ��λCCORE
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_doreset(void)
{
#ifdef _DRV_LLT_
#else
    unsigned long sctrl_on  = (unsigned long)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);
    unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);
    unsigned long uvalue_jedge = 0;
    volatile unsigned long uvalue = 0;
    unsigned int ulAckWaitCnt = 0;

    uvalue = readl(SOC_SCTRL_SC_CARM_STAT0_ADDR(sctrl_off));
    uvalue = readl(SOC_SCTRL_SC_CARM_CTRL0_ADDR(sctrl_off));
    printk(KERN_INFO "%s: org value = 0x%lx\n", __FUNCTION__, uvalue);
    /*mcpu debug req*/
    writel((uvalue | BIT(SOC_SCTRL_SC_CARM_CTRL0_mcpu_debug_req_START)), SOC_SCTRL_SC_CARM_CTRL0_ADDR(sctrl_off));

    /*wait, dbgack & dbgdone*/
    uvalue_jedge = (BIT(SOC_SCTRL_SC_CARM_STAT0_carm_dbgack_START)
                    | BIT(SOC_SCTRL_SC_CARM_STAT0_carm_dbgcpudone_START));
    do{
        udelay(100);
        uvalue = readl(SOC_SCTRL_SC_CARM_STAT0_ADDR(sctrl_off));
        ulAckWaitCnt++;
    }while(((uvalue & uvalue_jedge) != uvalue_jedge) && (ulAckWaitCnt < RESET_WAIT_CCPU_ACKDONE_TIMEOUT));
    if((uvalue & uvalue_jedge) != uvalue_jedge)
    {
        return BSP_RESET_ERROR;
    }

    /*iso*/
    writel(BIT(SOC_AO_SCTRL_SC_PW_ISOEN1_pw_isoen1_5mcpu_START), SOC_AO_SCTRL_SC_PW_ISOEN1_ADDR(sctrl_on));

    /*mcpu���踴λ*/
    writel((BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_11mcpu_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_12mscu_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_13mp_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_14mwd_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_15mdbg_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_16mdapb_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_17matb_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_18mptm_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN0_periph_rsten0_19malone_START)), SOC_SCTRL_SC_PERIPH_RSTEN0_ADDR(sctrl_off));

    uvalue = readl(SOC_SCTRL_SC_CARM_CTRL0_ADDR(sctrl_off));
    printk(KERN_INFO "%s: new value = 0x%lx\n", __FUNCTION__, uvalue);
    /*�ָ�dbg req*/
    writel((uvalue & (~(BIT(SOC_SCTRL_SC_CARM_CTRL0_mcpu_debug_req_START)))), SOC_SCTRL_SC_CARM_CTRL0_ADDR(sctrl_off));

    /*HOLD BBE16*/
    uvalue = readl(SOC_SCTRL_SC_BBPABB_SUBSYS_CTRL0_ADDR(sctrl_off));
    uvalue = uvalue | BIT(SOC_SCTRL_SC_BBPABB_SUBSYS_CTRL0_ltedsp_runstall_START);
    writel(uvalue, SOC_SCTRL_SC_BBPABB_SUBSYS_CTRL0_ADDR(sctrl_off));
    /*wait for holding*/
    msleep(5);
    /*��λBBE16*/
    writel(BIT(SOC_AO_SCTRL_SC_PW_RSTEN0_pw_rsten0_14ltebbptop_START), SOC_AO_SCTRL_SC_PW_RSTEN0_ADDR(sctrl_on));

    /*��λmuart*/
    writel((BIT(SOC_SCTRL_SC_PERIPH_RSTEN6_periph_rsten6_16muart0_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTEN6_periph_rsten6_17muart1_START)), SOC_SCTRL_SC_PERIPH_RSTEN6_ADDR(sctrl_off));

    /* ��λIPF */
    writel(BIT(SOC_SCTRL_SC_PERIPH_RSTEN1_periph_rsten1_21reset_ipf_n_START), SOC_SCTRL_SC_PERIPH_RSTEN1_ADDR(sctrl_off));

    printk(KERN_INFO "%s: leave\n", __FUNCTION__);
#endif
    return BSP_RESET_OK;
}
/*****************************************************************************
 �� �� ��  : ccorereset_enable_wdt_irq
 ��������  : ͨ��д�Ĵ���ʵ���������жϵ�a��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_enable_wdt_irq(void)
{
#ifdef _DRV_LLT_
#else
    unsigned long sctrl_on  = (unsigned long)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);
    unsigned long uvalue = 0;

    uvalue = readl(SOC_AO_SCTRL_SC_MCU_WKUP_INT_EN1_ADDR(sctrl_on));
    printk(KERN_INFO "%s: org val = 0x%lx\n", __FUNCTION__, uvalue);

    writel((uvalue | (1 << 6)), SOC_AO_SCTRL_SC_MCU_WKUP_INT_EN1_ADDR(sctrl_on));
    uvalue = readl(SOC_AO_SCTRL_SC_MCU_WKUP_INT_EN1_ADDR(sctrl_on));
    printk(KERN_INFO "%s: new val = 0x%lx\n", __FUNCTION__, uvalue);

    writel(0x1f, SOC_AO_SCTRL_SC_INT_EN0_ADDR(sctrl_on));
    uvalue = readl(SOC_AO_SCTRL_SC_INT_EN0_ADDR(sctrl_on));
    printk(KERN_INFO "%s: new uvalue = 0x%lx\n", __FUNCTION__, uvalue);

#endif
    return 0;
}

#ifdef _DRV_LLT_
int platform_ddr_protect_init(int flag)
{
    return 0;
}
#endif

/*****************************************************************************
 �� �� ��  : ccorereset_dostart
 ��������  : �⸴λCCORE
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int ccorereset_dostart(void)
{
#ifdef _DRV_LLT_
#else

    unsigned long sctrl_on  = (unsigned long)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);
    unsigned long sctrl_off = (unsigned long)IO_ADDRESS(SOC_SC_OFF_BASE_ADDR);
    unsigned long   iviraddr_dest1 = 0;


    /* modem ����ʼ��ַ�̶�Ϊ���ַ�������Ҫ�����ַд��תָ�� */
    iviraddr_dest1 = (unsigned long)(ioremap_nocache(0x00000000, 4096));

    writel(0xE59F0004, iviraddr_dest1);            /*ldr  r0, 0xc*/
    writel(0xE59FD004, iviraddr_dest1 + 4);   /*ldr  r13, 0x10*/
    writel(0xE1A0F000, iviraddr_dest1 + 8);   /*mov  pc, r0*/
    writel(MODEM_UNZIP_RUN_ADDR, iviraddr_dest1 + 0xc);   /*д��modem ������ʼ��ַ*/
    writel(MODEM_ZIP_LOAD_ADDR, iviraddr_dest1 + 0x10);   /*д��ջ��ַ*/
    iounmap((void*)iviraddr_dest1);

    writel(BIT(SOC_AO_SCTRL_SC_PW_ISOEN1_pw_isoen1_5mcpu_START), SOC_AO_SCTRL_SC_PW_ISODIS1_ADDR(sctrl_on));
    writel((BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_11mcpu_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_12mscu_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_13mp_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_14mwd_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_15mdbg_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_16mdapb_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_17matb_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_18mptm_START)
            | BIT(SOC_SCTRL_SC_PERIPH_RSTDIS0_periph_rstdis0_19malone_START)), SOC_SCTRL_SC_PERIPH_RSTDIS0_ADDR(sctrl_off));
#endif

    printk(KERN_INFO "%s: leave\n", __FUNCTION__);
    return 0;
}


/*****************************************************************************
 �� �� ��  : ccorereset_icc_cok_notify
 ��������  : ����Modem��λ�ɹ�������Ա�֪ͨACPU��λ�ɹ���
 �������  : ��

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
unsigned int ccorereset_icc_cok_notify(unsigned int chenalId,signed int u32size)
{
    unsigned int readSize;
    int ireadvalue = 0;


    /*ICC��ȡ����*/
    readSize = udi_read(g_reset_assistant.ccorereset_IccFd,(void*)(&ireadvalue),u32size);
    printk(KERN_INFO "%s: get icc msg from c core, readsize = %d, wantsize = %d, ireadvalue = 0x%x\n", \
                    __FUNCTION__, readSize, u32size, ireadvalue);

    if (readSize != u32size)
    {
        printk(KERN_ERR "%s: udi_read 's size is error readSize:%d VS u32size:%d\n", __FUNCTION__, readSize, u32size);
        return (unsigned int)(BSP_RESET_ERROR);
    }
    if (1 == g_reset_assistant.iccore_reset)
    {
        if (CCORE_RESET_OK_FROM_CCORE == ireadvalue)
        {
            printk(KERN_INFO "%s: c tell me it's ok\n", __FUNCTION__);
            /*�ͷţ�������ɹ��ź���*/
            up(&(g_reset_assistant.sem_wait_ccorereset_ok));
        }
        else
        {
            printk(KERN_ERR "%s: fail to run c core!return %d\n", __FUNCTION__, ireadvalue);
            /*do nothing*/
            do_reset_system(RESET_TYPE_CRESET_CRUN_NO_OK);
        }
    }
    return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : drv_ccorereset_cbfun
 ��������  : ������Modem������λʱ���ڴ���IPF,ICC, FILEAGENT,CSHELL, IFC������ݡ�
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam, ��ʾ��λǰ���Ǹ�λ�����
            ��int userdata,�û����ݣ�

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int drv_ccorereset_cbfun(DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata)
{
    int     iresult = 0;

#ifdef _DRV_LLT_
#else

    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        /*��λICCͨ��*/
        BSP_ICC_ResetFun();

        /*���ô���IPF���ݵĺ���*/
        BSP_IPF_SetControlFLagForCcoreReset(IPF_FORRESET_CONTROL_FORBID);
        /*���c��ipc��*/
        BSP_IPC_SemGive_Ccore_All();
    }
    else
    {
        BSP_IPF_SetControlFLagForCcoreReset(IPF_FORRESET_CONTROL_ALLOW);

        /*ʹ�ܸ�λǰ�رյ��ж�*/
        finish_reset_sub();
    }
#endif

    /*����FILEAGENT�رվ��*/
    iresult = close_all_filehandle(eparam);
    if (0 != iresult)
    {
        printk(KERN_ERR "%s: drv_close file fail, return %d\n", __FUNCTION__, iresult);
        goto error_return;
    }

    /*���ô���CSHELL��صĺ���*/
    iresult = set_for_cshell(eparam);
    if (0 != iresult)
    {
        printk(KERN_ERR "%s: drv_cshell fail, return %d\n", __FUNCTION__, iresult);
        goto error_return;
    }

    return iresult;
error_return:
    printk(KERN_ERR "%s: drv cb fun run fail, return = %d\n", __FUNCTION__, iresult);
    return BSP_RESET_ERROR;
}
/*****************************************************************************
 �� �� ��  : reset_icc_open
 ��������  : ��icc
 �������  : ��

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int reset_icc_open(void)
{
    g_reset_assistant.attr.enChanMode  = ICC_CHAN_MODE_PACKET;
    g_reset_assistant.attr.u32Priority = 0;
    g_reset_assistant.attr.u32TimeOut  = 10000;
    g_reset_assistant.attr.event_cb    = NULL;
    g_reset_assistant.attr.read_cb     = ccorereset_icc_cok_notify;
    g_reset_assistant.attr.write_cb    = NULL;
    g_reset_assistant.attr.u32FIFOInSize  = 1024;
    g_reset_assistant.attr.u32FIFOOutSize = 1024;

    g_reset_assistant.ccorereset_IccParam.devid = UDI_ICC_GUOM7_ID;
    g_reset_assistant.ccorereset_IccParam.pPrivate = &(g_reset_assistant.attr);

    /*��ICCͨ��*/
    g_reset_assistant.ccorereset_IccFd = udi_open(&(g_reset_assistant.ccorereset_IccParam));

    if (g_reset_assistant.ccorereset_IccFd <= 0)
    {
        printk(KERN_ERR "%s: in reset_icc_open ICC failed:[0x%x]\n", __FUNCTION__, g_reset_assistant.ccorereset_IccFd);

        return BSP_RESET_ERROR;
    }
    printk(KERN_INFO "%s: in reset_icc_open ICC SUCCESS:[0x%x]\n", __FUNCTION__, g_reset_assistant.ccorereset_IccFd);
    return BSP_RESET_OK;
}
int  set_for_cshell(DRV_RESET_CALLCBFUN_MOMENT eparam)
{
#ifdef _DRV_LLT_
#else
    if(DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        /*cshell invalid*/
        bsp_ccpu_rest_cshell_handle(CSHELL_INVALID);
    }
    else
    {
        bsp_ccpu_rest_cshell_handle(CSHELL_VALID);
    }
#endif
    return BSP_RESET_OK;
}
int  close_all_filehandle(DRV_RESET_CALLCBFUN_MOMENT eparam)
{
#ifdef _DRV_LLT_
#else
    if(DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        bsp_ccpu_rest_rfile_handle();
    }
    else
    {
        ;
    }
#endif
    return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : do_reset_system
 ��������  : ���ڸ�λϵͳ
 �������  : int ireset_type,��λ����
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
void  do_reset_system(int ireset_type)
{
#ifdef _DRV_LLT_
#else

    systemError((int)BSP_MODU_DORESET, EXCH_S_DORESET, ireset_type, 0, 0);

#endif
}
/*****************************************************************************
 �� �� ��  : reset_set_cpu_status
 ��������  : ��hifi����ccpu��λʱ������״̬������mailbox��������hifi  ��mail.
 �������  : unsigned int iOff  1-��ʹ��;0-�ر�
             unsigned int iflag ��ʶcpu״̬��Ӧ��ƫ��
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
void reset_set_cpu_status(unsigned int iOff, unsigned int iflag)
{
    if (1 == iOff)
    {
        g_reset_assistant.icpustatus = (g_reset_assistant.icpustatus) | iflag;
    }
    else
    {
        g_reset_assistant.icpustatus = (g_reset_assistant.icpustatus) & (~iflag);
    }
    printk(KERN_INFO "%s: reset_set_cpu_status, ioff = %d, iflag = %d\n", __FUNCTION__, iOff, iflag);
}

/*****************************************************************************
 �� �� ��  : BSP_CPU_StateGet
 ��������  : �ṩ��mailboxģ�飬���ڻ�����cpu״̬
 �������  : int CpuID, MCU, HIFI,CCPU
 �������  : ��
 �� �� ֵ  : int,1:ʹ�ܣ�0:��ʹ��
*****************************************************************************/
int BSP_CPU_StateGet(int CpuID)
{
    unsigned int u32CpuStatusOffset = 0;

    switch (CpuID)
    {
    case MAILBOX_CPUID_HIFI:
        u32CpuStatusOffset = RESET_CPU_HIFI_STATUS_OFF;
        break;
    case MAILBOX_CPUID_CCPU:
        u32CpuStatusOffset = RESET_CPU_CCORE_STATUS_OFF;
        break;
    default:
        break;
    }

    return (g_reset_assistant.icpustatus & u32CpuStatusOffset) ? 0 : 1;
}

/*****************************************************************************
 �� �� ��  : reset_ccore_up_semaphone
 ��������  : �ͷ�ccore reset�ź���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void reset_ccore_up_semaphone(void)
{
#ifdef BSP_C_HIFI_RESET_ALONE_FEATURE
    up(&(g_reset_assistant.sem_wait_ccorereset));
    printk(KERN_INFO "%s: reset_ccore_up_semaphone\n", __FUNCTION__);
#endif
}

void reset_ccore_up_byhimself(int is_normal)
{
#ifdef BSP_C_HIFI_RESET_ALONE_FEATURE
    printk(KERN_INFO "%s: reset_ccore_up_byhimselfs\n", __FUNCTION__);
    BSP_CCPU_Reset_Byhimself(is_normal, 0);

#endif
}

/*****************************************************************************
 �� �� ��  : reset_hifi_up_semaphone
 ��������  : �ͷ�hifi reset�ź���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void reset_hifi_up_semaphone(void)
{
#ifdef BSP_C_HIFI_RESET_ALONE_FEATURE
    up(&(g_reset_assistant.sem_wait_hifireset));
    printk(KERN_INFO "%s: reset_hifi_up_semaphone\n", __FUNCTION__);
#endif
}

/*****************************************************************************
 �� �� ��  : reset_clean_wdt_int
 ��������  : �յ���λ�жϺ�����c��/hifi���ж�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void reset_clean_wdt_int(ereset_module emodule)
{
#ifdef _DRV_LLT_
#else
#ifdef BSP_C_HIFI_RESET_ALONE_FEATURE
    unsigned long sctrl_on  = (unsigned long)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);

    switch (emodule)
    {
    case BSP_RESET_MODULE_CCORE:
        writel(0x10, SOC_AO_SCTRL_SC_INT_CLEAR_ADDR(sctrl_on));
        break;
    case BSP_RESET_MODULE_HIFI:
        writel(0x4, SOC_AO_SCTRL_SC_INT_CLEAR_ADDR(sctrl_on));
        break;
    default:
        break;
    }
#endif
    printk(KERN_INFO "%s: reset_clean_wdt_int %d\n", __FUNCTION__, emodule);
#endif
}

/*****************************************************************************
 �� �� ��  : drv_hifireset_cbfun
 ��������  : ������hifi������λʱ���ڴ���mailbox������ݡ�
 �������  : DRV_RESET_CALLCBFUN_MOMENT eparam, ��ʾ��λǰ���Ǹ�λ�����
            ��int userdata,�û����ݣ�

 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int drv_hifireset_cbfun(DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata)
{
    int     iresult = 0;

#ifdef _DRV_LLT_
#else

    if (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
    {
        printk(KERN_INFO"RESET LOG: reset mediaserver task! before\n");

        mach_call_usermodeshell("/system/etc/log/mediaserver_reset.sh" , 0);

        printk(KERN_INFO"RESET LOG: reset mediaserver task! after\n");
    }
    else
    {
    }

    printk(KERN_INFO"RESET LOG: HIFI cb fun %d run ok(%d)\n", eparam, iresult);

#endif

    return iresult;
}

/*************************************************************************
��ά�ɲ�ӿ�
*************************************************************************/

/*****************************************************************************
 �� �� ��  : reset_no_ok_savelog
 ��������  : �ڵ��ûص�����ʱ����ص���������ʧ�ܣ����¼��ģ�����֣�����ֵ
 �������  : char *pname,���ע��ص�����ʱע������֣�
             int iresult,�ص������ķ���ֵ��
             ereset_module emodule, ��λ��ģ��,ccore or hifi
 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int reset_no_ok_savelog(char *pname, int iresult, DRV_RESET_CALLCBFUN_MOMENT eparam, ereset_module emodule)
{
    int     iwrite = 0;
    int     ilen = 0;
    char    *psavelog = NULL;
    char    *ptime = (DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam?"before":"after");
#ifdef PRINK_TO_FILE    /*���ʵ����printk��ӡ��Ϣ�����ļ����ܣ�����Ҫ�ú�������*/
    return BSP_RESET_OK;
#endif

    if (NULL == pname)
    {
        printk(KERN_ERR "[%s ]name is NULL, in savelog fun\n", __FUNCTION__);
        return BSP_RESET_ERROR;
    }

    psavelog = (char*)kmalloc(BSP_RESET_LOG_INFO_ITEM_LEN, GFP_KERNEL);
    if (NULL == psavelog)
    {
        printk(KERN_ERR "%s: fail to malloc, in savelog fun\n", __FUNCTION__);
        return BSP_RESET_ERROR;
    }
    memset((void*)psavelog, 0, BSP_RESET_LOG_INFO_ITEM_LEN);
    switch (emodule)
    {
    case BSP_RESET_MODULE_CCORE:
        sprintf(psavelog, "%s ccore reset, %s fail, return %d\n",ptime, pname, iresult);
        break;
    case BSP_RESET_MODULE_HIFI:
        sprintf(psavelog, "%s hifi reset, %s fail, return %d\n",ptime, pname, iresult);
        break;
    default:
        sprintf(psavelog, "valid module, %s fail, return %d\n",pname, iresult);
        printk(KERN_ERR "%s: module id %d invalid!!, in savelog fun\n", __FUNCTION__, emodule);
        break;

    }
    reset_for_savelog(psavelog);
    kfree(psavelog);
    psavelog = NULL;
    printk(KERN_INFO "%s: %s, iwrite/len %d/%d\n", __FUNCTION__, psavelog, iwrite, ilen);
    return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : reset_for_savelog
 ��������  : ����log
 �������  : char *pstr,log����
 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
int reset_for_savelog(char *pstr)
{
    FILE   *pfh = NULL;
    int     iwrite = 0;
    int     ilen = 0;


#ifdef PRINK_TO_FILE    /*���ʵ����printk��ӡ��Ϣ�����ļ����ܣ�����Ҫ�ú�������*/
    return BSP_RESET_OK;
#endif
#ifdef _DRV_LLT_
#else
    if (NULL == pstr)
    {
        printk(KERN_ERR "%s: pstr is NULL, in savelog fun\n", __FUNCTION__);
        return BSP_RESET_ERROR;
    }
    pfh = BSP_fopen(BSP_RESET_LOG_FILE, "a");
    if (NULL == pfh)
    {
        printk(KERN_ERR "%s: no log file, create one\n", __FUNCTION__);
        pfh = BSP_fopen(BSP_RESET_LOG_FILE, "w");
    }
    if (NULL != pfh)
    {
        ilen = strlen(pstr);
        iwrite = BSP_fwrite((const void*)pstr,ilen, 1, pfh);
        if (iwrite < 1)
        {
            printk(KERN_ERR "%s: can not write all: %d / %d\n", __FUNCTION__, iwrite, ilen);
        }
        BSP_fclose(pfh);
        pfh = NULL;
        printk(KERN_INFO "%s: iwrite len %d\n", __FUNCTION__, ilen);
    }
#endif
    return BSP_RESET_OK;
}

/*****************************************************************************
 �� �� ��  : reset_info_show
 ��������  : ��ʾ��ά�ɲ���Ϣ
 �������  :
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void reset_info_show(void)
{
    sreset_mgr_LLI  *phead = g_pmgr_ccorereset_data;

    while (NULL != phead)
    {
        printk(KERN_INFO "%s: name:%s cbfun:0x%8x data:%d priolevel:%d\n", __FUNCTION__, phead->cbfuninfo.name,
            (unsigned int)phead->cbfuninfo.cbfun, phead->cbfuninfo.userdata, phead->cbfuninfo.priolevel);
        phead = phead->pnext;
    }

    phead = g_pmgr_hifireset_data;
    while (NULL != phead)
    {
        printk(KERN_INFO "%s: hifi reset: name:%s cbfun:0x%8x data:%d priolevel:%d\n", __FUNCTION__, phead->cbfuninfo.name,
            (unsigned int)phead->cbfuninfo.cbfun, phead->cbfuninfo.userdata, phead->cbfuninfo.priolevel);
        phead = phead->pnext;
    }

    /*C�˵�����λ״̬*/
    printk(KERN_INFO "%s: ccore reset task process %d phase\n", __FUNCTION__, CCORE_RESET_TASK_PHASE_GET());
}
/*�����ýӿ�*/
#ifdef BSP_C_HIFI_RESET_ALONE_TESET_VALID
/*These functions are used for testing!!!only!!!, starts*/
extern int simulate_irq_c_freeze(void);
extern int simulate_irq_c_panic(void);
extern int simulate_irq_c_reset(int is_normal);
extern int simulate_irq_hifi_reset(void);
extern int simulate_irq_mcuwdt_reset(void);

#ifdef _DRV_LLT_
#else
extern BSP_S32 BSP_IPF_ST_101(BSP_U32 u32PacketNum);
#endif

void test_clean_mgr_ccore_link(void)
{
    sreset_mgr_LLI  *phead = g_pmgr_ccorereset_data;
    sreset_mgr_LLI  *ptail = g_pmgr_ccorereset_data;

    while (NULL != phead)
    {
        ptail = phead->pnext;
        kfree(phead);
        phead = ptail;
    }
    g_pmgr_ccorereset_data = NULL;
}
void test_clean_mgr_hifi_link(void)
{
    sreset_mgr_LLI  *phead = g_pmgr_hifireset_data;
    sreset_mgr_LLI  *ptail = g_pmgr_hifireset_data;

    while (NULL != phead)
    {
        ptail = phead->pnext;
        kfree(phead);
        phead = ptail;
    }
    g_pmgr_hifireset_data = NULL;
}

/*ends*/

/*****************************************************************************
 �� �� ��  : reset_only_for_test
 ��������  : �ú���ֻ���ڲ���!!
 �������  : int iparam, ����case:
             0,����modem ����λ
             1,����modem ����쳣��λ
             2,����modem ������λ
             3,����hifi��λ
             4,����mcu����λ
             5,���Ե�����ccore��λʱ�����Ĵ�����ipf,icc,fileagent,cshell��Ӧ�����Ƿ���ȷ
 �������  : ��
 �� �� ֵ  : int
        0, �ɹ�����0��ʧ��
*****************************************************************************/
void reset_only_for_test(int iparam)
{
    int     iresult = 0;
#ifdef _DRV_LLT_
#else
    switch (iparam)
    {
    case 0:
        up(&(g_reset_assistant.sem_wait_hifi_msg));
        up(&(g_reset_assistant.sem_wait_mcu_msg));

        simulate_irq_c_freeze();
        break;
    case 1:
        up(&(g_reset_assistant.sem_wait_hifi_msg));
        up(&(g_reset_assistant.sem_wait_mcu_msg));

        simulate_irq_c_panic();
        break;
    case 2:
        up(&(g_reset_assistant.sem_wait_hifi_msg));
        up(&(g_reset_assistant.sem_wait_mcu_msg));

        simulate_irq_c_reset(1);
        break;
    case 3:
        simulate_irq_hifi_reset();
        break;
    case 4:
        simulate_irq_mcuwdt_reset();
        break;
    case 5:
        /*send ip*/
        iresult = BSP_IPF_ST_101(20);

        drv_ccorereset_cbfun(DRV_RESET_CALLCBFUN_RESET_BEFORE, 0);
        /*send ip*/
        iresult = BSP_IPF_ST_101(20);
        break;
    case 6:
       reset_no_ok_savelog("drv", -2, 0, BSP_RESET_MODULE_HIFI);
       do_reset_system(RESET_TYPE_FOR_TEST);
        break;
    case 7:
        ccorereset_regcbfunc("drv", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL);
        ccorereset_regcbfunc("OAM", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL+1);
        ccorereset_regcbfunc("TTF", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL-1);
        ccorereset_regcbfunc("NAS", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL+1);

        break;
    case 8:
        hifireset_regcbfunc("OAM", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL+1);
        hifireset_regcbfunc("CODEC", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL+1);
        hifireset_regcbfunc("NAS", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL+1);


        break;
    case 9:
        test_clean_mgr_ccore_link();
        break;
    case 10:
        test_clean_mgr_hifi_link();
        break;
    case 11:
        ccorereset_runcbfun(DRV_RESET_CALLCBFUN_RESET_BEFORE);
        ccorereset_runcbfun(DRV_RESET_CALLCBFUN_RESET_AFTER);
        break;
    case 12:
        hifireset_runcbfun(DRV_RESET_CALLCBFUN_RESET_BEFORE);
        hifireset_runcbfun(DRV_RESET_CALLCBFUN_RESET_AFTER);

        break;
    case 13:
        up(&(g_reset_assistant.sem_wait_hifi_msg));

        break;
    case 14:
        up(&(g_reset_assistant.sem_wait_mcu_msg));
        break;
    case 15:
        up(&(g_reset_assistant.sem_wait_ccorereset_ok));
        break;
    case 16:
        up(&(g_reset_assistant.sem_wait_hifireset));
        break;
    case 17:
        break;
    default:
        break;
    }
#endif
}

#endif


/*Added by l00212112,functions for c reset alone are defined, ends*/
#ifdef BSP_C_HIFI_RESET_ALONE_FEATURE
/*****************************************************************************
 �� �� ��  : reset_sub_mgr_init
 ��������  : ����C��,hifi������λ���Ƶĳ�ʼ��,��reset_module_init�е���,���ڴ����ź���,��������Ȳ���.
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
int reset_sub_mgr_init (void)
{
    struct task_struct    *pCCoreTask = NULL;
    struct task_struct    *pHifiTask = NULL;
    int     iret = BSP_RESET_OK;

    /*Ĭ��HIFI/CCPUʹ��*/
    reset_set_cpu_status(0, RESET_CPU_HIFI_STATUS_OFF);
    reset_set_cpu_status(0, RESET_CPU_CCORE_STATUS_OFF);

    /*������Ҫ���ź���*/

    printk(KERN_INFO "%s: enter\n", __FUNCTION__);
    memset(&g_stResetDebugInfo, 0, sizeof(sreset_debug_info));
    memset(&g_reset_assistant, 0, sizeof(sreset_mgr_assistant));
    sema_init(&(g_reset_assistant.sem_wait_ccorereset), SEM_EMPTY);
    sema_init(&(g_reset_assistant.sem_wait_hifireset), SEM_EMPTY);
    sema_init(&(g_reset_assistant.sem_wait_ccorereset_ok), SEM_EMPTY);
    sema_init(&(g_reset_assistant.sem_wait_hifi_msg), SEM_EMPTY);
    sema_init(&(g_reset_assistant.sem_wait_mcu_msg), SEM_EMPTY);
    sema_init(&(g_reset_assistant.sem_wait_mcu_msg_hifireset), SEM_EMPTY);

    /*����C��λ�����߳�*/
    pCCoreTask = kthread_run(ccorereset_task,  NULL, "ccorereset_task");

    printk(KERN_INFO "%s: create ccorereset_task, return %p\n", __FUNCTION__, pCCoreTask);

    /*����hifi��λ�����߳�*/
    pHifiTask = kthread_run(hifireset_task,  NULL, "hifireset_task");

    printk(KERN_INFO "%s: create hifireset_task, return %p\n", __FUNCTION__, pHifiTask);

    /*ע��C�˸�λ�ص�����*/
    ccorereset_regcbfunc("DRV", drv_ccorereset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL);

    /*ע��HIFI��λ�ص�����*/
    hifireset_regcbfunc("CODEC", drv_hifireset_cbfun, 0, BSP_DRV_CBFUN_PRIOLEVEL);

    /*ע��C�˸�λ�ص�����*/
    reset_icc_open();

    ccorereset_enable_wdt_irq();
    /*ע��ACPU��MCU֮���MAILBOX*/
    iret = mailbox_reg_msg_cb(MAILBOX_MAILCODE_MCU_TO_ACPU_CCORE_RESET_ID, mailbox_recfun_mcu, NULL);
    iret = mailbox_reg_msg_cb(MAILBOX_MAILCODE_MCU_TO_ACPU_HIFI_RESET_ID, mailbox_recfun_mcu_hifireset, NULL);
    printk(KERN_INFO"RESET LOG: LEAVE reset_sub_mgr_init0! iret = %d\n", iret);


    /*ע��ACPU��HIFI֮���MAILBOX*/
    iret = mailbox_reg_msg_cb(MAILBOX_MAILCODE_HIFI_TO_ACPU_CCORE_RESET_ID, mailbox_recfun_hifi, NULL);

    printk(KERN_INFO "%s: leave, iret = %d\n", __FUNCTION__, iret);
    return BSP_RESET_OK;
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
