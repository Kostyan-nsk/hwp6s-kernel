

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/proc_fs.h>
#include <linux/clk.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>

#include "drv_mailbox.h"
#include "drv_mailbox_gut.h"
#include "drv_mailbox_cfg.h"

#include "drv_mailbox_platform.h"
#include "drv_mailbox_debug.h"
#include "bsp_mailbox.h"
#include "hifidrvinterface.h"
#include "test_hifi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
#undef  _HIFI_FILE_
#define _HIFI_FILE_   "th"

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ��������
*****************************************************************************/

int test_send_encap_msg(unsigned char  *buff, unsigned long len)
{
    int ret = OK;

    /*���ú˼�ͨ�Žӿڷ�������*/
    ret = mailbox_send_msg(MAILBOX_MAILCODE_ACPU_TO_HIFI_AUDIO, buff, len);
    if ( OK != ret )
    {
        printk("mailbox_send_msg error\n");
        return ERROR;
    }

    return ret;
}
int test_audioplayer_time_query(void)
{
    int ret = OK;

    TEST_HIFI_MSG_STRU                stHifiMsg;
    AUDIO_PLAYER_QUERY_TIME_REQ_STRU  stQuery;
    unsigned long                     ulSize   = 0;
    unsigned char                    *pucBuff  = NULL;

    /* ap��hifi����ͨ����ϢID */
    stHifiMsg.usMsgId       = ID_AP_HIFI_TEST_CMD;
    stHifiMsg.ulSenderPid   = DSP_PID_AUDIO;
    stHifiMsg.ulReceiverPid = DSP_PID_AUDIO_RT;
    stHifiMsg.ulChannel     = DRV_MAILBOX_CHANNEL_AUDIO_CH;
    stHifiMsg.ulMsgLen      = sizeof(AUDIO_PLAYER_QUERY_TIME_REQ_STRU);

    /* Query��Ϣ���� */
    stQuery.uhwMsgId        = ID_AP_AUDIO_PLAY_QUERY_TIME_REQ;

    ulSize = sizeof(TEST_HIFI_MSG_STRU) + stHifiMsg.ulMsgLen;


    /* �����ں�̬�ڴ� */
    pucBuff = kmalloc(ulSize, GFP_KERNEL);

    /* �ڴ�copy */
    memcpy(pucBuff, &stHifiMsg, sizeof(TEST_HIFI_MSG_STRU));
    memcpy(pucBuff+sizeof(TEST_HIFI_MSG_STRU), &stQuery, sizeof(AUDIO_PLAYER_QUERY_TIME_REQ_STRU));

    /* ��hifi��������Ϣ */
    ret = test_send_encap_msg(pucBuff,ulSize);

    if (NULL != pucBuff)
    {
        kfree(pucBuff);
    }

    return ret;
}

/*****************************************************************************
 �� �� ��  : test_hifi_help
 ��������  : show test function
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : ·���л�
    �޸�����   : �����ɺ���

*****************************************************************************/
int test_hifi_help( void  )
{
    printk("test_audioplayer_time_query\n");
    printk("test_hifi_mail_reg\n");
    printk("test_hifi_pcm_open\n");
    return 0;
}

/*****************************************************************************
 �� �� ��  : test_hifi_mail_handle
 ��������  : ����ص�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : ·���л�
    �޸�����   : �����ɺ���

*****************************************************************************/
void test_hifi_handle_mail( void *usr_para, void *mail_handle, unsigned int mail_len  )
{
    int ret_mail = OK;
    unsigned int *pmail_value;
    unsigned char *mail_buff;       /* �������ݽ��յ�buff */

    printk("test_hifi_handle_mail\n");

    mail_buff = (unsigned char *)kmalloc(mail_len, GFP_ATOMIC);

    /* ��ʣ������copy͸����buff�� */
    ret_mail = mailbox_read_msg_data(mail_handle, (unsigned char*)mail_buff, (unsigned long *)&mail_len);

    pmail_value =(unsigned int *) mail_buff;
    if (NULL != pmail_value)
    {
        printk("mail reture value %x \n",*pmail_value);

    }


    if (NULL != mail_buff)
    {
        kfree(mail_buff);
    }

}

/*****************************************************************************
 �� �� ��  : test_hifi_mail_reg
 ��������  : ����ע��
 �������  : ��
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��5��25��
    ��    ��   : ·���л�
    �޸�����   : �����ɺ���

*****************************************************************************/
int test_hifi_mail_reg( void  )
{

    unsigned long RecvMailCode = MAILBOX_MAILCODE_CHANNEL(MAILBOX_CPUID_HIFI, MAILBOX_CPUID_ACPU, 0);

    printk("test_hifi_mail_reg %x\n", (unsigned int)RecvMailCode);

    /*ע��˫��ͨ�Ŵ�����*/
    mailbox_reg_msg_cb( RecvMailCode, (mb_msg_cb)test_hifi_handle_mail, NULL );

    return 0;
}
void test_hifi_bit_read(
                unsigned int            Addr,
                unsigned int            StartBit,
                unsigned int            EndBit)
{
    unsigned int                          Content;
    unsigned int                          TmpMask;
    unsigned int                          TmpBit;

    Addr = (unsigned int)(ioremap(Addr, 4));

    /*�����쳣ʱ���ȡ����32bit����*/
    if ((EndBit < StartBit)
        || (StartBit > 31)
        || (EndBit > 31))
    {
        StartBit = 0;
        EndBit   = 31;
    }

    /*������ʼ�ͽ���������������*/
    TmpBit        = 31 - EndBit;
    TmpMask       = 0xffffffff << TmpBit;
    TmpMask       = TmpMask >> ( StartBit + TmpBit );
    TmpMask       = TmpMask << StartBit;

    Content       = readl(Addr);
    Content      &= TmpMask;
    Content       = Content >> StartBit;

    printk("test_hifi_bit_read %x\n",Content);

}

int test_hifi_bit_write(
                unsigned int              Addr,
                unsigned int              StartBit,
                unsigned int              EndBit,
                unsigned int              Content)
{
    unsigned int                          SetValue;
    unsigned int                          RegContent;
    unsigned int                          TmpMask;
    unsigned int                          TmpBit;

    Addr = (unsigned int)(ioremap(Addr, 4));

    /*���������쳣ʱ�����쳣����*/
    if ((EndBit < StartBit)
        || (StartBit > 31)
        || (EndBit > 31))
    {
        return -1;
    }

    /*���ݴ�д�����ݺ���ʼ������Ϣ������д�Ĵ���������*/
    SetValue      = Content;
    SetValue      = SetValue << StartBit;

    /*������ʼ�ͽ���������������*/
    TmpBit        = 31 - EndBit;
    TmpMask       = 0xffffffff << TmpBit;
    TmpMask       = TmpMask >> ( StartBit + TmpBit);
    TmpMask       = TmpMask << StartBit;

    /*�ȶ�ȡWBBP�Ĵ�����ַ�е����ݣ���д��ָ��bit����*/
    RegContent    = readl(Addr);
    RegContent   &= (~TmpMask);
    SetValue     &= TmpMask;
    writel((RegContent | SetValue),Addr);

    printk("test_hifi_bit_write %x\n",(RegContent | SetValue));
    return 0;
}


void test_hifi_read(unsigned int Addr)
{
    unsigned int                          Content;
    Addr = (unsigned int)(ioremap(Addr, 4));

    Content = readl(Addr);

    printk("test_hifi_read %x\n",Content);

}

int test_hifi_write(
                unsigned int              uwAddr,
                unsigned int              uwContent)
{
    uwAddr = (unsigned int)(ioremap(uwAddr, 4));

    writel(uwContent,uwAddr);

    printk("test_hifi_write %x\n",uwContent);
    return 0;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

