

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/types.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/list.h>

#include "BSP.h"
#include "acm_ldisc.h"
#include "bsp_udi_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* acm ldisc ops����ṹ�� */
static struct tty_ldisc_ops acm_ldisc = {
    .owner        = THIS_MODULE,
    .magic        = TTY_LDISC_MAGIC,
    .name         = "acm",
    .open         = acm_ldisc_open,
    .close        = acm_ldisc_close,
    .read         = acm_ldisc_read,
    .write        = acm_ldisc_write,
    .receive_buf  = acm_ldisc_receive_buf,
    .write_wakeup = acm_ldisc_write_wakeup,
    .ioctl        = acm_ldisc_ioctl,
    .hangup       = acm_ldisc_hangup,
};

extern struct acm_ctx g_acm_priv[];
struct acm_ldisc_priv g_acm_ldisc[USB_ACM_COM_UDI_NUM];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


void acm_free_list(struct list_head *plist)
{
    struct acm_mem_info   *acm_mem_node;

    while(!list_empty(plist)) {
        acm_mem_node = list_entry(plist->next,
                                  struct acm_mem_info,
                                  list);
        list_del(&acm_mem_node->list);

        if (NULL != acm_mem_node->mem) {
            kfree(acm_mem_node->mem);
        }

        kfree(acm_mem_node);
        acm_mem_node = NULL;
    }
}



int acm_ldisc_open(struct tty_struct *tty)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = NULL;
    int i = 0;

    for (i = 0; i < USB_ACM_COM_UDI_NUM; i++) {
        if (!g_acm_ldisc[i].tty || (0 == strncmp(tty->name,g_acm_ldisc[i].tty_name,strlen(g_acm_ldisc[i].tty_name)))) {
            acm_ldisc_ctx = &g_acm_ldisc[i];
            strncpy(g_acm_ldisc[i].tty_name, tty->name, sizeof(tty->name));
            break;
        }
    }

    if (NULL == acm_ldisc_ctx) {
        ACM_LD_DBG("device is not opened\n");
        return ERROR;
    }

    /* ��tty�ڵ�����acm ldisc˽�������� */
    acm_ldisc_ctx->tty = tty;

    ACM_LD_DBG("[%s] enter\n", tty->name);

    spin_lock_init(&acm_ldisc_ctx->recv_lock);
    spin_lock_init(&acm_ldisc_ctx->write_lock);
    sema_init(&acm_ldisc_ctx->recv_sema, 0);

    tasklet_init(&acm_ldisc_ctx->write_tsk, acm_write_task, (unsigned long)acm_ldisc_ctx);

    /* ��ʼ������д���� */
    INIT_LIST_HEAD(&acm_ldisc_ctx->recv_list);
    INIT_LIST_HEAD(&acm_ldisc_ctx->write_list);

    acm_ldisc_ctx->read_size          = 0;
    acm_ldisc_ctx->tty_recv_size      = 0;
    acm_ldisc_ctx->write_size         = 0;
    acm_ldisc_ctx->write_success_size = 0;
    acm_ldisc_ctx->last_read_left_mem = NULL;
    acm_ldisc_ctx->last_write_left_mem = NULL;

    /* ��ʼ��receive_room��С */
    tty->receive_room = ACM_LD_ROOM;

    /* ��acm ldisc˽����������tty->disc_data�� */
    tty->disc_data = (void *)acm_ldisc_ctx;

    return OK;
}


void acm_ldisc_close(struct tty_struct *tty)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct list_head *recv_list;
    struct list_head *write_list;

    if (NULL == acm_ldisc_ctx) {
        ACM_LD_DBG("device ldisc data is null\n");
        return;
    }

    ACM_LD_DBG("[%s] enter\n", tty->name);

    /* ��tty�ڵ��¼��ldisc˽�������ÿ� */
    tty->disc_data = NULL;

    recv_list  = &(acm_ldisc_ctx->recv_list);
    write_list = &(acm_ldisc_ctx->write_list);

    /* �ͷŶ�д�����ڴ� */
    spin_lock_bh(&acm_ldisc_ctx->recv_lock);
    acm_free_list(recv_list);
    spin_unlock_bh(&acm_ldisc_ctx->recv_lock);

    spin_lock_bh(&acm_ldisc_ctx->write_lock);
    acm_free_list(write_list);
    spin_unlock_bh(&acm_ldisc_ctx->write_lock);

    up(&acm_ldisc_ctx->recv_sema);

    /* killдtask_let */
    tasklet_kill(&acm_ldisc_ctx->write_tsk);

    /*kfree(acm_ldisc_ctx);*/

}


int acm_ldisc_read(struct tty_struct * tty, struct file * file,
                        unsigned char __user * buf, size_t nr)
{
    int ret       = 0;
    int read_size = 0;
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct list_head *recv_list;
    struct acm_mem_info *acm_mem_node = NULL;
    unsigned char *copy_start_addr;

    if (NULL == acm_ldisc_ctx) {
        return USB_NODATA;
    }

    recv_list = &acm_ldisc_ctx->recv_list;

    if (NULL == acm_ldisc_ctx->last_read_left_mem) {
        if (list_empty(recv_list)){
            /* �ȴ�receive_buf���ź��� */
            down(&acm_ldisc_ctx->recv_sema);
        }

        if (NULL == acm_ldisc_ctx) {
            return USB_NODATA;
        }

        spin_lock_bh(&acm_ldisc_ctx->recv_lock);
        if (list_empty(recv_list)) {
            spin_unlock_bh(&acm_ldisc_ctx->recv_lock);
            return USB_NODATA;
        }

        acm_mem_node = list_entry(recv_list->next, struct acm_mem_info, list);
        list_del_init(recv_list->next);
        spin_unlock_bh(&acm_ldisc_ctx->recv_lock);

        if (NULL == acm_mem_node) {
            ACM_LD_DBG("read buff is invalid\n");
            return ERROR;
        }

    } else {
        acm_mem_node = acm_ldisc_ctx->last_read_left_mem;
    }

    /* �˴���Ҫ��ȡ���ݵ��׵�ַ */
    copy_start_addr = (unsigned char *)acm_mem_node->current_pos;

    if (acm_mem_node->left_size > nr){
        read_size = nr;
        acm_mem_node->left_size -= nr;
        acm_mem_node->current_pos +=nr;
        acm_ldisc_ctx->last_read_left_mem = acm_mem_node;
    } else {
        read_size = acm_mem_node->left_size;
        acm_ldisc_ctx->last_read_left_mem = NULL;
    }

    if (read_size > 0){
        /* copy���ݵ�buf�� */
        if(copy_to_user((void *)buf, (void *)copy_start_addr, read_size))
        {
            return ERROR;
        }
        acm_ldisc_ctx->read_size += read_size;
        ret = read_size;
    }

    if (NULL == acm_ldisc_ctx->last_read_left_mem) {
        kfree(acm_mem_node->mem);
        acm_mem_node->mem = NULL;
        kfree(acm_mem_node);
        acm_mem_node      = NULL;
    }

    return ret;
}
int acm_ldisc_write(struct tty_struct *tty, struct file *file,
                       const unsigned char *buf, size_t nr)
{
    int ret  = 0;
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct acm_mem_info *acm_mem_node;

    acm_ldisc_ctx->write_size += nr;

    /* ���NONBLOCK��־λ,�첽д��֧ */
    if (file->f_flags & O_NONBLOCK){
        ret = tty->ops->write(tty,buf,nr);
        acm_ldisc_ctx->write_success_size += ret;

        return ret;
    }

    acm_mem_node = (struct acm_mem_info *)kmalloc(sizeof(struct acm_mem_info),GFP_KERNEL);

    /* ��buf��װ��acm_mem_info�ṹȻ��ӵ�д������ */
    acm_mem_node->mem         = (void *)buf;
    acm_mem_node->valid_size  = nr;
    acm_mem_node->left_size   = nr;
    acm_mem_node->current_pos = (unsigned int)buf;
    sema_init(&acm_mem_node->sema,0);

    /* д������ */
    spin_lock_bh(&acm_ldisc_ctx->write_lock);

    list_add_tail(&acm_mem_node->list, &acm_ldisc_ctx->write_list);

    /* д������� */
    spin_unlock_bh(&acm_ldisc_ctx->write_lock);

    /* ����дtask_let */
    tasklet_schedule(&acm_ldisc_ctx->write_tsk);

    /* �ȴ�д����ź��� */
    down(&acm_mem_node->sema);

    /*if (NULL != acm_mem_node->mem) {
        kfree(acm_mem_node->mem);
    }*/

    kfree(acm_mem_node);
    acm_mem_node = NULL;

    return nr;
}


void acm_write_task(unsigned long ldisc_priv)
{
    int len       = 0;
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)ldisc_priv;
    struct tty_struct *tty       = acm_ldisc_ctx->tty;
    struct list_head *write_list = &acm_ldisc_ctx->write_list;
    struct acm_mem_info *acm_mem_node;

    /* д������ */
    spin_lock(&acm_ldisc_ctx->write_lock);

    while ((NULL != acm_ldisc_ctx->last_write_left_mem)||
            (!list_empty(write_list))) {
        /* ����ϴ��Ƿ���δд������� */
        if (NULL == acm_ldisc_ctx->last_write_left_mem) {
            /* ��д������ȡ���ݲ��Ҹ������Բ�����mem_info�ֲ����� */
            acm_mem_node = list_entry(write_list->next, struct acm_mem_info, list);
            list_del_init(write_list->next);

            /* ��������Ƿ�Ϊ�� */
            if (NULL == acm_mem_node) {
                /*ACM_LD_DBG("write buff is empty\n");*/
                spin_unlock(&acm_ldisc_ctx->write_lock);
                return;
            }

        } else {
            /* δд������last_write_left_mem��ֵ��mem_info�ֲ����� */
            acm_mem_node= acm_ldisc_ctx->last_write_left_mem;
            clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
        }

        len = tty->ops->write(tty, (unsigned char*)acm_mem_node->current_pos, acm_mem_node->left_size);

        acm_ldisc_ctx->write_success_size += len;

        if (len < acm_mem_node->left_size) {
            /* ����mem_info->left_size,�ó�ʣ������ֽ�δд��  */
            acm_mem_node->left_size -= len;

            /* ����mem_info->current_pos,�ó���ǰдָ���λ��  */
            acm_mem_node->current_pos += len;

            /* ˢ��l_disc��last_write_left_mem����δд������ָ�� */
            acm_ldisc_ctx->last_write_left_mem = acm_mem_node;

            /* ����TTY_DO_WRITE_WAKEUP��־���õײ��пռ�ʱ֪ͨldisc�����·����� */
            set_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);
            break;
        } else {
            /* ���l_disc��last_write_left_mem */
            acm_ldisc_ctx->last_write_left_mem = NULL;

            /* �ͷŸ��ڴ�ڵ�д�ź��� */
            up(&acm_mem_node->sema);
        }
    }

    spin_unlock(&acm_ldisc_ctx->write_lock);

    return;
}


void acm_ldisc_receive_buf(struct tty_struct *tty, const unsigned char *buf,
                                char *cflags, int count)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct semaphore *sem = &acm_ldisc_ctx->recv_sema;
    struct acm_mem_info *acm_mem_node;
    unsigned char *buffer = NULL;
    unsigned long flags;
    int i;

    buffer = (unsigned char *)kmalloc(count, GFP_KERNEL);
    if (!buffer) {
        ACM_LD_DBG("kmalloc fail!\n");
        return;
    }

    memcpy((void *)buffer, (void *)buf, count);

    acm_mem_node = (struct acm_mem_info *)kmalloc(sizeof(struct acm_mem_info),GFP_ATOMIC);

    /* ��buf��װ��acm_mem_info�ṹȻ��ӵ��������� */
    acm_mem_node->mem         = buffer;
    acm_mem_node->valid_size  = count;
    acm_mem_node->left_size   = count;
    acm_mem_node->current_pos = (unsigned int)buffer;

    spin_lock(&acm_ldisc_ctx->recv_lock);
    list_add_tail(&acm_mem_node->list, &acm_ldisc_ctx->recv_list);
    acm_ldisc_ctx->tty_recv_size += count;
    spin_unlock(&acm_ldisc_ctx->recv_lock);

    /*�����C SHELL�����ö��ص���buff����*/
    for (i = 0; i < USB_ACM_COM_UDI_NUM; i++) {
        if (0 == strncmp(tty->name, g_acm_priv[i].tty_name, strlen(g_acm_priv[i].tty_name))) {
            if (g_acm_priv[i].readDoneCB) {
                g_acm_priv[i].readDoneCB();
                break;
            }
        }
    }

    raw_spin_lock_irqsave((raw_spinlock_t *)(&sem->lock), flags);
    if (0 == sem->count) {
        raw_spin_unlock_irqrestore((raw_spinlock_t *)(&sem->lock), flags);
        up(sem);
    } else {
        raw_spin_unlock_irqrestore((raw_spinlock_t *)(&sem->lock), flags);
    }

    return;
}


void acm_ldisc_write_wakeup(struct tty_struct *tty)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;

    /* ���TTY_DO_WRITE_WAKEUP��־λ */
    clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);

    /* ���ldisc˽�������Ƿ�Ϊ�� */
    if (NULL == acm_ldisc_ctx) {
       ACM_LD_DBG("acm_ldisc_priv is NULL!\n");
       return;
    }

    /* ����дtask_let */
    tasklet_schedule(&acm_ldisc_ctx->write_tsk);

    return;
}
int acm_ldisc_ioctl(struct tty_struct *tty, struct file *file, unsigned int cmd, unsigned long arg)
{
    ACM_WR_ASYNC_INFO *mem_info = NULL;
    unsigned long *buf = NULL;
    int ret = OK;

    /* ACM_LD_DBG("enter\n"); */

    if (0 == arg){
        ACM_LD_DBG("param invalid\n");
        return ERROR;
    }

    switch (cmd) {
        case LDISC_IOCTL_GET_RD_BUF:
            mem_info = (ACM_WR_ASYNC_INFO *)arg;
            ret = acm_ldisc_get_rd_buf(tty, mem_info);
            if (ERROR == ret){
                mem_info->pBuffer = NULL;
                mem_info->u32Size = 0;
                mem_info->pDrvPriv = NULL;
                ACM_LD_DBG("get read buf fail\n");
                return ERROR;
            }
            break;

        case LDISC_IOCTL_RETURN_BUF:
            mem_info = (ACM_WR_ASYNC_INFO *)arg;
            ret = acm_ldisc_return_buf(tty, mem_info);
            if (ERROR == ret){
                ACM_LD_DBG("return buf fail\n");
                return ERROR;
            }
            break;

        case LDISC_IOCTL_GET_TTY:
            buf = (unsigned long *)arg;
            *buf = (unsigned long)tty;
            ret = OK;
            break;

        default:
            ACM_LD_DBG("%s cmd[0x%x] unsupport\n", tty->name, cmd);
            break;
    }

    return ret;
}
int acm_ldisc_get_rd_buf(struct tty_struct * tty, ACM_WR_ASYNC_INFO *mem_info)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct acm_mem_info *acm_mem_node = NULL;
    struct list_head    *recv_list = NULL;


    /* ACM_LD_DBG("enter\n"); */

    recv_list = &acm_ldisc_ctx->recv_list;

    spin_lock_bh(&acm_ldisc_ctx->recv_lock);

    /*���������ǰ��δ��������ݣ���recv_listȡ�½ڵ�*/
    if (NULL == acm_ldisc_ctx->last_read_left_mem) {
        if (list_empty(recv_list)){
            spin_unlock_bh(&acm_ldisc_ctx->recv_lock);
            ACM_LD_DBG("no data in tty\n");
            return ERROR;
        }

        acm_mem_node = list_entry(recv_list->next, struct acm_mem_info, list);
        list_del_init(recv_list->next);

        if (NULL == acm_mem_node) {
            spin_unlock_bh(&acm_ldisc_ctx->recv_lock);
            ACM_LD_DBG("read buf is invalid\n");
            return ERROR;
        }

    } else {
        /*ȡδ���������*/
        acm_mem_node = acm_ldisc_ctx->last_read_left_mem;
    }

    mem_info->pBuffer = (char *)acm_mem_node->current_pos;
    mem_info->u32Size = acm_mem_node->left_size;
    mem_info->pDrvPriv = (void *)acm_mem_node;
    spin_unlock_bh(&acm_ldisc_ctx->recv_lock);
    return OK;
}
int acm_ldisc_return_buf(struct tty_struct * tty, ACM_WR_ASYNC_INFO *mem_info)
{
    struct acm_mem_info *acm_mem_node = NULL;

    /* ACM_LD_DBG("enter\n"); */

    acm_mem_node = (struct acm_mem_info *)mem_info->pDrvPriv;
    /* mem_nodeȫ��������ϣ����ͷŸ��ڴ� */
    if (((unsigned int)acm_mem_node->mem + acm_mem_node->valid_size) == ((unsigned int)mem_info->pBuffer + mem_info->u32Size)) {
        kfree(acm_mem_node->mem);
        acm_mem_node->mem = NULL;
        kfree(acm_mem_node);
        acm_mem_node      = NULL;
    }

    return OK;

}


int acm_ldisc_hangup(struct tty_struct *tty)
{
    struct acm_ldisc_priv *acm_ldisc_ctx = (struct acm_ldisc_priv *)tty->disc_data;
    struct semaphore *sem = &acm_ldisc_ctx->recv_sema;
    unsigned long flags;

    /* ACM_LD_DBG("tty[%p]\n", tty); */

    raw_spin_lock_irqsave((raw_spinlock_t *)(&sem->lock), flags);
    if (0 == sem->count) {
        raw_spin_unlock_irqrestore((raw_spinlock_t *)(&sem->lock), flags);
        up(sem);
    } else {
        raw_spin_unlock_irqrestore((raw_spinlock_t *)(&sem->lock), flags);
    }
    return OK;
}
int __init acm_ldisc_init(void)
{
    int ret = 0;
    int i = 0;

    for (i = 0; i < USB_ACM_COM_UDI_NUM; i++) {
        g_acm_ldisc[i].tty = NULL;
        g_acm_ldisc[i].last_read_left_mem = NULL;
        g_acm_ldisc[i].last_write_left_mem = NULL;
    }

    ret = tty_register_ldisc(N_USB_COM, &acm_ldisc);
    if (ret != OK) {
       ACM_LD_DBG("acm_ldisc register failed!\n");
       ret = ERROR;
    }

    return ret;
}


void __exit acm_ldisc_exit(void)
{
    int ret = 0;

    /* ����tty_unregister_ldiscȥע��acm_ldisc�����ҵõ�����ֵret */
    ret = tty_unregister_ldisc(N_USB_COM);
    if (ret != OK) {
       ACM_LD_DBG("acm_ldisc unregister failed! unregister's ret is %d\n",ret);
    }
}

module_init(acm_ldisc_init);
module_exit(acm_ldisc_exit);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

