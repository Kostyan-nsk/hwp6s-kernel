

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/string.h>
#include "mntn_deal_with_mcu_msg.h"
#include "bsp_mailbox.h"
#include "drv_mailbox.h"
#include "drv_mailbox_ifc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


s32 mntn_deal_with_msg(u8* data, u32 enDataType, u32 ulTimeStemp)
{
    MCU_EVENT* pEvent = 0;

    switch (enDataType)
    {
        case ID_MCU_DATA_EVENT :
            pEvent = (MCU_EVENT*)data;
            printk(PRINT_LEV "TimeStemp:0x%x, EventType:%d, Module:%d, PreVAl:%d, CurVal:%d\r\n",
                pEvent->ulTimeStamp, pEvent->enEventType, pEvent->enModule, pEvent->ulPreVAl, pEvent->ulCurVal);
            break;
        case ID_MCU_DATA_TRACE :
            printk(PRINT_LEV "TimeStemp:0x%x, String:%s\r\n", ulTimeStemp, data);
            break;
        case ID_MCU_DATA_DEBUG:
            /* Ϊʵ���ڹ涨ʱ�������ٷ���һ�ε�������ݣ�ʲô������ */
            break;
        default:
            printk(PRINT_LEV "DataType Error! TimeStemp:0x%x, DataType:%d\r\n", ulTimeStemp, enDataType);
            return -1;
    }

    return 0;
}
void mntn_deal_with_mcu_mail(void * UserHandle, void * MailHandle, u32 ulMailLen)
{
    u32 ulMailSize = MNTN_BUFF_SIZE;    /* �ʼ�������512�ֽ� */
    u32 ulCount = 0;
    u8  ucBuf[MNTN_BUFF_SIZE] = {0};
    u8* pCur = ucBuf;
    MCU_TO_ACPU_MSG* pMsg = 0;

#if defined (MNTN_DEBUG)
        printk(PRINT_LEV "mntn_deal_with_mcu_mail entry.\r\n");
#endif

    if (MAILBOX_OK != mailbox_read_msg_data(MailHandle, ucBuf, ( unsigned long *)&ulMailSize))
    {
        printk(PRINT_LEV "ReadMail error!\r\n");
        return;
    }

    while(ulCount < ulMailSize)
    {
        pMsg = (MCU_TO_ACPU_MSG*)pCur;

        /* ������Ϣ��Ԫ */
        mntn_deal_with_msg(pCur + sizeof(MCU_TO_ACPU_MSG) - DATA_SIZE_IN_MSG_HEAD, pMsg->enDataType, pMsg->ulTimeStemp);

        /* pCurָ����һ����Ϣ */
        pCur += sizeof(MCU_TO_ACPU_MSG) - DATA_SIZE_IN_MSG_HEAD + pMsg->ulLength;

        /* ���㴦����Ϣ���ܳ��� */
        ulCount += sizeof(MCU_TO_ACPU_MSG) - DATA_SIZE_IN_MSG_HEAD + pMsg->ulLength;
    }
}

#ifndef _DRV_LLT_
/* ����Զ�̵��÷����� */
IFC_GEN_CALL1(MAILBOX_IFC_ACPU_TO_MCU_MNTN, mntn_set_on_off,
                IFC_INCNT, u8, ucMntnOnOff, 0)
IFC_GEN_CALL2(MAILBOX_IFC_ACPU_TO_MCU_RUN_CMD, mntn_mcu_run_cmd,
                IFC_INVAR, void*, cmd, 0,
                IFC_INCNT, int, len, 0)

/* ����Զ�̵���ִ�к����� */
IFC_GEN_EXEC2(aprintk, IFC_INVAR, void *, str, 0,
                      IFC_INCNT, int, len, 0)
#endif


void set_mcu_mntn_on_off(u8 ucMntnOnOff)
{
#if defined (MNTN_DEBUG)
        printk(PRINT_LEV "set_mcu_mntn_on_off ucMntnOnOff=%d.\r\n", ucMntnOnOff);
#endif

#ifndef _DRV_LLT_
    mntn_set_on_off(ucMntnOnOff, 0);/* ��0��IFC��Ҫ�� */
#endif
}
void RunMcuCmd(char* cmd)
{
#ifndef _DRV_LLT_
    (void)mntn_mcu_run_cmd(cmd, strlen(cmd) + 1, 0);/* ��0��ʾ���ȴ����� */
#endif
}

void tsRunMcuCmd(void)
{
    char* cmd = "ReadM 0xf711a35c 0x4";
    RunMcuCmd(cmd);
}
long aprintk(char* str, int len)
{
    printk("%s", str);
    return 0;
}


s32 mntn_init(void)
{
#if defined (MNTN_DEBUG)
        printk(PRINT_LEV "mntn_init entry!\r\n");
#endif
    mailbox_reg_msg_cb(BSP_MAILBOX_CHANNEL_MCU_TO_ACPU_MNTN_CH, (mb_msg_cb)mntn_deal_with_mcu_mail, 0);

#ifndef _DRV_LLT_
    mailbox_ifc_register(MAILBOX_IFC_MCU_TO_ACPU_PRINT, aprintk);
#endif

    return 0;
}


module_init(mntn_init);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif






