
#include <linux/err.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/stddef.h>
#include <linux/sysfs.h>
#include <linux/types.h>
#include <linux/mutex.h>

#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/slab.h>
#include <linux/pm_qos_params.h>
#include <linux/delay.h>
#include <linux/timer.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#endif
#include "bsp_mailbox.h"

IFC_GEN_CALL2(MAILBOX_IFC_ACPU_TO_CCPU_GET_EFUSE_TEMP_ADJUST, get_efuse_temp_adjust_value,
              IFC_OUTVAR, unsigned char *, buf, 0,
              IFC_INCNT, unsigned int, len, 0)

int get_temp_adjust_value(unsigned int * buffer)
{

#if !defined(BOARD_ASIC)
    return 0;
#endif

    if(0 != get_efuse_temp_adjust_value((unsigned char *)buffer, 4, 1000))
    {
        printk("read temp adjust efuse error\n");
        return -1;
    }

    return 0;
}


