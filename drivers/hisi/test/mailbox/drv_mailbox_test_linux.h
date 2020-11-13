
#ifndef __TEST_DRV_MAILBOX_H__
#define __TEST_DRV_MAILBOX_H__

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TEST_MAILBOX_INIT_FLAG          (0x5aa55aa5)

#define TM_MAILBOX_BYTE_PER_KB          (1024)

#define Second_To_Millisecond           (1000)
 /*�������ĳ˷�����*/
#define MAILBOX_BOARDST_BOUNDWIDTH_MUL_LIMIT   30

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
 ʵ �� ��  : struct tm_mb_handle
 ��������  : ������ģ����ƾ��
*****************************************************************************/
struct tm_mb_handle
{
    void *          mutex;
    unsigned long   init_flag;
};

/*****************************************************************************
 ʵ �� ��  : struct tm_mb_cb
 ��������  : �����Ե��û��ص����
*****************************************************************************/
struct tm_mb_cb
{
    void *           finish_sema;
    void *           sync_sema;
    unsigned long    start_slice;       /*for calculate boundwidth*/
    unsigned long    end_slice;         /*for calculate boundwidth*/
    unsigned long    prob_slice;        /*for calculate latency*/
    unsigned long    back_slice;        /*for calculate latency*/
    unsigned long    msg_count;
    unsigned long    task_count;
    unsigned long    check_ret;
};

/*****************************************************************************
 ʵ �� ��  : test_mailbox_send_proc_func
 ��������  : �����Ե���ں���
*****************************************************************************/
typedef void (*test_mailbox_send_proc_func)(
                unsigned long mailcode,
                unsigned long recvcode,
                unsigned long sendsize,
                unsigned long msgnum,
                unsigned long delaytick,
                unsigned long delaynum,
                unsigned long trytimes,
                struct tm_mb_cb * cb);

/*****************************************************************************
 ʵ �� ��  : struct test_mailbox_proc
 ��������  : �����Ե�������νṹ��
*****************************************************************************/
struct test_mailbox_proc
{
    test_mailbox_send_proc_func proc_cb;
    unsigned long mail_code;
    unsigned long recv_code;
    unsigned long send_size;
    unsigned long msg_num;
    unsigned long delay_tick;
    unsigned long delay_num;
    unsigned long try_times;
    struct tm_mb_cb * mb_cb;
};


/*****************************************************************************
  7 UNION����
*****************************************************************************/

/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


extern unsigned long test_mailbox_msg_multi_send(
                unsigned long DstId,
                unsigned long CarrierID,
                unsigned long TaskNum,
                unsigned long MsgLen,
                unsigned long MsgNum,
                unsigned long Priority,
                unsigned long DelayTick,
                unsigned long DelayNum,
                unsigned long TryTimes
);

extern unsigned long test_mailbox_msg_multi_test(
                unsigned long DstId,
                unsigned long TaskNum,
                unsigned long MsgNum,
                unsigned long DelayNum);

extern unsigned long test_mailbox_msg_single_test(
                unsigned long DstId,
                unsigned long MsgLen,
                unsigned long MsgNum,
                unsigned long DelayNum);

extern void test_mailbox_msg_reg(unsigned long ChannelID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of test_drv_mailbox.h */

