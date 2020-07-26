/**
    @copyright: Huawei Technologies Co., Ltd. 2012-2012. All rights reserved.
    
    @file: srecorder_dmesg.c
    
    @brief: ��ȡ�ں�ѭ�������е����ݣ����ں�����printk��ӡ����Ϣ��
    
    @version: 1.0 
    
    @author: QiDechun ID: 216641
    
    @date: 2012-06-21
    
    @history:
*/

/*----includes-----------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/ctype.h>
#include <linux/highmem.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "../include/srecorder_dmesg.h"
#include "../include/srecorder_kernel_symbols.h"
#include "../include/srecorder_save_log.h"
#include "../include/srecorder_snprintf.h"
#include "../include/srecorder_memory.h"


#if defined(CONFIG_DUMP_DMESG)

/*----local macroes------------------------------------------------------------------*/


/*----local prototypes----------------------------------------------------------------*/


/*----local variables------------------------------------------------------------------*/


/*----global variables-----------------------------------------------------------------*/


/*----local function prototypes---------------------------------------------------------*/

static int srecorder_write_data(char *pdst, const char *psrc, int bytes_to_write);


/*----function definitions--------------------------------------------------------------*/

/**
    @function: static int srecorder_write_data(char *pdst, const char *psrc, int bytes_to_write)
    @brief: ��IO�ڴ�д����

    @param: pdst - IO��ַ
    @param: psrc - ԭʼ���ݵ�ַ
    @param: bytes_to_write - ��Ҫд����ֽ���
    
    @return: 0 - �ɹ���-1-ʧ��

    @note: 
*/
static int srecorder_write_data(char *pdst, const char *psrc, int bytes_to_write)
{
    int i = 0;
    int j = 0;
    
    if (NULL == pdst || NULL == psrc)
    {
        return 0; 
    }

    
    for (i = 0; i < bytes_to_write; i++)
    {     
        if (!isascii(psrc[i]) || psrc[i] == '\0')
        {
            continue;
        }
        
        *(pdst + j) = *(psrc + i);
        j++;
    }

    return j;
}


/**
    @function: int srecorder_get_dmesg(srecorder_reserved_mem_info_for_log_t *pmem_info)
    @brief: ��ȡ�ں�ѭ������������

    @param: pmem_info SRecorder�ı����ڴ���Ϣ
    
    @return: 0 - �ɹ���-1-ʧ��

    @note: 
*/
int srecorder_get_dmesg(srecorder_reserved_mem_info_t *pmem_info)
{
    int bytes_to_read = 0;
    int bytes_read1 = 0; /**/
    int bytes_read2 = 0; /**/
    int log_buf_len = 0;
    char *log_buf = NULL;         
    const char *start1 = NULL;
    const char *start2 = NULL;
    unsigned log_end = 0; /* Index into log_buf: most-recently-written-char + 1 */
    psrecorder_info_header_t pinfo_header = NULL;
    
    if (NULL == pmem_info 
        || INVALID_KSYM_ADDR == srecorder_get_log_buf()
        || INVALID_KSYM_ADDR == srecorder_get_log_buf_len()
        || INVALID_KSYM_ADDR == srecorder_get_log_end())
    {
        SRECORDER_PRINTK("File [%s] line [%d] invalid param or kernel symbol addr!\n", __FILE__, __LINE__);
        return -1;
    }

    if (srecorder_log_has_been_dumped(DMESG_BIT2))
    {
        SRECORDER_PRINTK("dmesg has been dumped successfully!\n");
        return 0;
    }
    
    if (0 != srecorder_write_info_header(pmem_info, DMESG_BIT2, &pinfo_header))
    {
        return -1;
    }
    
    log_buf = (char *)(*(srec_ksym_addr_t*)srecorder_get_log_buf());
    if (NULL == log_buf)
    {
        return -1;
    }

    /* ������жϲ����٣�����������ڴ�Խ���Ǳ�ڷ��գ������쳣Ƕ�ף�ϵͳ����������λ��ֻ�������ڴ��⸴λ */
    log_buf_len = (int)srecorder_get_log_buf_len();

    /* 
    * Theoretically, the log could move on after we do this, but
    * there's not a lot we can do about that. The new messages
    * will overwrite the start of what we dump. 
    */
    log_end = (*(srec_ksym_addr_t *)srecorder_get_log_end()) & LOG_BUF_MASK(log_buf_len);
    bytes_to_read = MIN((pmem_info->bytes_left - 1), MIN(log_buf_len, log_buf_len));

    if (log_end >= bytes_to_read)
    {
        start1 = log_buf + log_end - bytes_to_read;
        bytes_read1 = srecorder_write_data(pmem_info->start_addr + pmem_info->bytes_read, start1, bytes_to_read);
        srecorder_renew_meminfo(pmem_info, bytes_read1);
    }
    else
    {
        start1 = log_buf + log_buf_len - (bytes_to_read - log_end);
        bytes_read1 = srecorder_write_data(pmem_info->start_addr + pmem_info->bytes_read, start1, (bytes_to_read - log_end));
        srecorder_renew_meminfo(pmem_info, bytes_read1);
        start2 = log_buf;
        bytes_read2 = srecorder_write_data(pmem_info->start_addr + pmem_info->bytes_read, start2, log_end);
        srecorder_renew_meminfo(pmem_info, bytes_read2);
    } 

    /* д��"\n" */
    bytes_read1 = srecorder_write_data(pmem_info->start_addr + pmem_info->bytes_read, "\n", 1);
    srecorder_renew_meminfo(pmem_info, bytes_read1);
    
    /* ��д��Ϣͷ����֤��Ϣ */
    srecorder_validate_info_header(pinfo_header, pmem_info->bytes_per_type);
    
    return 0;
}


/**
    @function: int srecorder_init_dmesg(srecorder_module_init_params_t *pinit_params)
    @brief: ��ʼ��dmesgģ��

    @param: pinit_params ģ���ʼ������
    
    @return: 0 - �ɹ���-1-ʧ��

    @note: 
*/
int srecorder_init_dmesg(srecorder_module_init_params_t *pinit_params)
{
    srecorder_clear_log_dumped_bit(DMESG_BIT2);
    
    return 0;
}


/**
    @function: void srecorder_exit_dmesg(void)
    @brief: �˳�dmesgģ��

    @param: none
    
    @return: none

    @note: 
*/
void srecorder_exit_dmesg(void)
{
    srecorder_set_log_dumped_bit(DMESG_BIT2);
}
#else
int srecorder_get_dmesg(srecorder_reserved_mem_info_t *pmem_info)
{
    return 0;
}


int srecorder_init_dmesg(srecorder_module_init_params_t *pinit_params)
{
    return 0;
}


void srecorder_exit_dmesg(void)
{
    return;
}
#endif

