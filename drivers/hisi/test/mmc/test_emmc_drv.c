

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include "test_emmc_drv.h"
#include "test_acore.h"

MODULE_AUTHOR("huawei wanglei");
MODULE_DESCRIPTION("Kernel emmc file test function.");

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
//#define LENGTH 100
#define COUNT (100)

#define TESTFILE "/data/data/test.txt"

char *s_input="0123456789";

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


int test_emmc_file_create( void  )
{
    struct file *filp;

    filp=filp_open(TESTFILE, O_RDWR|O_CREAT, 0644);
    if(IS_ERR(filp)){
        printk("error occured while opening file%s,exsiting...\n",TESTFILE);
        return ERROR;
    }
    filp_close(filp,NULL);
    return OK;

}

int test_emmc_file_WR( void  )
{
    struct file *filp;
    struct inode *inode;
    mm_segment_t fs;
    off_t fsize;
    int i=COUNT;
    char *buf;

    /*MAKE THE FILE CONTENT null*/
    filp=filp_open(TESTFILE, O_RDWR|O_TRUNC, 0644);
    if(IS_ERR(filp)){
        printk("error occured while opening file%s,exsiting...\n",TESTFILE);
        return ERROR;
    }
    filp_close(filp,NULL);

    /*write first   |O_APPEND*/
    filp=filp_open(TESTFILE, O_RDWR|O_CREAT|O_SYNC, 0644);
    if(IS_ERR(filp)){
        printk("error occured while opening file%s,exsiting...\n",TESTFILE);
        return ERROR;
    }
    fs=get_fs();
    set_fs(KERNEL_DS);
    while(i--){
        filp->f_op->write(filp, s_input, strlen(s_input),&(filp->f_pos));
    }
    set_fs(fs);
    filp_close(filp,NULL);
    /*read to check the write result*/
    filp=filp_open(TESTFILE, O_RDONLY, 0644);
    if(IS_ERR(filp)){
        printk("error occured while opening file%s,exsiting...\n",TESTFILE);
        return ERROR;
    }
    inode=filp->f_dentry->d_inode;
    fsize=inode->i_size;
    printk("file size:%i \n",(int)fsize);
    if ((strlen(s_input)*COUNT) != (int)fsize)
    {
    	printk(KERN_ERR "the number written into the file is not ritht!");
    }
    buf=(unsigned char *)kmalloc(fsize+1,GFP_KERNEL);
    fs=get_fs();
    set_fs(KERNEL_DS);
    filp->f_op->read(filp,buf,fsize,&(filp->f_pos));
    set_fs(fs);
    buf[fsize]='\0';
    filp_close(filp,NULL);

    /*check the result*/
    for(i=0;i < fsize;i++){
        if((i%10) != (buf[i]-'0')){
            printk(KERN_ERR "what we read does not mach what we write!");
            kfree(buf);
            return ERROR;
        }
    }
    kfree(buf);
    return OK;
}

int emmc_test_init(void){
     printk(KERN_ERR "EMMC TEST MODULE LOAD\n ");
     return 0;
}
void emmc_test_exit(void){
     printk(KERN_ERR "EMMC TEST MODULE UNLOAD\n ");
}

module_init(emmc_test_init);
module_exit(emmc_test_exit);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

