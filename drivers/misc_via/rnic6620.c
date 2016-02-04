/*
 *
 * Copyright (C) 2013 HUAWEI, Inc.
 * Author: wangliang <bill.wangliang@huawei.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/board_sensors.h>
#include <linux/device.h>
#include <linux/sysfs.h>

#include <asm/io.h>
#include <linux/mux.h>
#include <linux/wakelock.h>
#include <hsad/config_interface.h>
#include <linux/platform_device.h>
#include <hsad/config_mgr.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>
#include <linux/timer.h>
#include <linux/skbuff.h>

#include <RnicConfigInterface.h>

#define DRIVER_VERSION        "1.0.0"

#define LOG_ERR                0
#define LOG_INFO              1
#define LOG_NOTICE          2
#define LOG_NOTICE2        3
#define LOG_DEBUG           4

#define LOGPRT(lvl,x...)  do{ \
    if(lvl < (LOG_NOTICE + 1)) \
        printk("[RNIC_Adapter] " x); \
    }while(0)

#define RNIC_CONFIG_LENTH       (8)

static int rnic6620_pf_probe(struct platform_device *pdev);
static int rnic6620_pf_remove(struct platform_device *dev);
static ssize_t store_config(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count);

struct rnic6620_dev{
    struct platform_driver rnic6620_drv_pf;
    struct platform_device rnic6620_dev_pf;
};

static struct rnic6620_dev hw_rnic6620_dev = {
    .rnic6620_drv_pf = {
         .probe = rnic6620_pf_probe,
         .remove = rnic6620_pf_remove,
         .driver = {
                 .name = "rnic6620_interface",
                 .owner = THIS_MODULE,
         },
    },
    .rnic6620_dev_pf = {
        .name = "rnic6620_interface",
    },
};

static DEVICE_ATTR(rnic6620, S_IWUSR, NULL, store_config);

static struct attribute *rnic6620_attributes[] = {
    &dev_attr_rnic6620.attr,
    NULL
};

static const struct attribute_group rnic6620_attr_group = {
    .attrs = rnic6620_attributes,
};

static int rnic6620_pf_probe(struct platform_device *pdev)
{
    int ret = 0;
    LOGPRT(LOG_NOTICE, "RNIC__ called probe %s\n", __func__);

    ret =  sysfs_create_group(&hw_rnic6620_dev.rnic6620_dev_pf.dev.kobj, &rnic6620_attr_group);
    if (ret){
        dev_err(&hw_rnic6620_dev.rnic6620_dev_pf.dev, "sysfs create error %d\n", ret);
    }
    return ret;
};

static int rnic6620_pf_remove(struct platform_device *dev)
{
    return 0;
}

extern unsigned long SDIO_UL_SendPacket(struct sk_buff *pstData, unsigned char ucPDNId);

static ssize_t store_config(struct device *dev,
                struct device_attribute *attr, const char *buf, size_t count)
{
    RNIC_RMNET_CONFIG_STRU RnicConfig = {0};

    LOGPRT(LOG_NOTICE, "RNIC__ %s count = %d\n", __func__, count);

    if (count < RNIC_CONFIG_LENTH) {
        memcpy(&RnicConfig, buf, count);
    }
    else {
        memcpy(&RnicConfig, buf, RNIC_CONFIG_LENTH);
    }

    RNIC_ConfigRmnetStatus(&RnicConfig);
    return count;

}

static int rnic6620_init(void)
{
    int err = 0;

    err = platform_device_register(&hw_rnic6620_dev.rnic6620_dev_pf);
    if (err){
        dev_err(&hw_rnic6620_dev.rnic6620_dev_pf.dev, "rnic6620_dev_drv regiset error %d\n", err);
        goto rnic6620_pf_dev_fail;
    }

    err = platform_driver_register(&hw_rnic6620_dev.rnic6620_drv_pf);
    if (err){
        dev_err(&hw_rnic6620_dev.rnic6620_dev_pf.dev, "hw_rnic6620_dev.rnic6620_drv_pf regiset error %d\n", err);
        goto rnic6620_pf_drv_fail;
    }

    return err;

    rnic6620_pf_drv_fail:
        platform_device_unregister(&hw_rnic6620_dev.rnic6620_dev_pf);
    rnic6620_pf_dev_fail:

    return err;
}

static void __exit rnic6620_exit(void)
{
    platform_driver_unregister(&hw_rnic6620_dev.rnic6620_drv_pf);
    platform_device_unregister(&hw_rnic6620_dev.rnic6620_dev_pf);
}

module_init(rnic6620_init);
module_exit(rnic6620_exit);
