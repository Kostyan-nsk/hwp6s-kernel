#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/debugfs.h>//wangweitao
#include <mach/pmussi_drv.h>//wangweitao
#include <linux/poll.h>

#include "hisi_6401_spk.h"

#define LOG_TAG "HISI_6401_SPK"

#define PRINT_INFO  1
#define PRINT_WARN  0
#define PRINT_DEBUG 0
#define PRINT_ERR   1

#if PRINT_INFO
#define logi(fmt, ...) printk("[" LOG_TAG "][I]" fmt "\n", ##__VA_ARGS__)
#else
#define logi(fmt, ...) ((void)0)
#endif

#if PRINT_WARN
#define logw(fmt, ...) printk("[" LOG_TAG "][W]" fmt "\n", ##__VA_ARGS__)
#else
#define logw(fmt, ...) ((void)0)
#endif

#if PRINT_DEBUG
#define logd(fmt, ...) printk("[" LOG_TAG "][D]" fmt "\n", ##__VA_ARGS__)
#else
#define logd(fmt, ...) ((void)0)
#endif

#if PRINT_ERR
#define loge(fmt, ...) printk("[" LOG_TAG "][E]" fmt "\n", ##__VA_ARGS__)
#else
#define loge(fmt, ...) ((void)0)
#endif

#define HI6401_CLASSD_REGISTER 0x0c5
#define HI6401_CLASSD_ENABLE   0x10
#define Hi6401_CLASSD_DISABLE  0x0

static struct mutex hi6401_spk_lock;

static int hisi_6401_spk_open(struct inode *inode, struct file *file)
{
    logi("%s", __FUNCTION__);

	return 0;
}

static int hisi_6401_spk_release(struct inode *inode, struct file *file)
{
    logi("%s", __FUNCTION__);

	return 0;
}

static long hisi_6401_spk_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

    logi("%s, cmd=%d", __FUNCTION__, cmd);
	
	mutex_lock(&hi6401_spk_lock);
	switch (cmd) {
	case HISI_6401_SPK_ENABLE:
	     pmussi_reg_write(HI6401_CLASSD_REGISTER,HI6401_CLASSD_ENABLE);
		break;
	case HISI_6401_SPK_DISABLE:
	     pmussi_reg_write(HI6401_CLASSD_REGISTER,Hi6401_CLASSD_DISABLE);
		break;
	default:
		loge("%s: invalid command %d", __FUNCTION__, _IOC_NR(cmd));
		ret = -EINVAL;
		break;
	}

exit:
	mutex_unlock(&hi6401_spk_lock);

    logi("%s End, cmd=%d", __FUNCTION__, cmd);

	return ret;
}

static const struct file_operations hisi_6401_spk_misc_fops = {
	.owner = THIS_MODULE,
	.open = hisi_6401_spk_open,
	.release = hisi_6401_spk_release,
	.unlocked_ioctl = hisi_6401_spk_ioctl,
};

static struct miscdevice hi6401_spk_device = {
    .minor  = MISC_DYNAMIC_MINOR,
    .name   = "hisi_6401_spk",
    .fops   = &hisi_6401_spk_misc_fops,
};

static int hisi_6401_spk_probe(struct platform_device *pdev)
{

    int ret = 0;
    logi("%s", __FUNCTION__);

    ret = misc_register(&hi6401_spk_device);
   
#ifdef CONFIG_DEBUG_FS  
	if (!debugfs_create_file("hisi_6401_spk", 0644, NULL, NULL,
				 &hisi_6401_spk_misc_fops))
		pr_warn("PA: Failed to create hisi_6401_spk debugfs file\n");
#endif

	return ret;
}

static int hisi_6401_spk_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver hisi_6401_spk_driver = {
	.driver = {
		.name	= "hisi_6401_spk",
		.owner	= THIS_MODULE,
		//.of_match_table = hisi_6401_spk_of_match,
	},
	.probe	= hisi_6401_spk_probe,
	.remove	= hisi_6401_spk_remove,
};
//module_platform_driver(hisi_6401_spk_driver);

static int __init hi6401_spk_init(void)
{
    logi("%s", __FUNCTION__);
    mutex_init(&hi6401_spk_lock);
    return platform_driver_register(&hisi_6401_spk_driver);
}

static void __exit hi6401_spk_exit(void)
{
    logi("%s", __FUNCTION__);
    platform_driver_unregister(&hisi_6401_spk_driver);
}

module_init(hi6401_spk_init);
module_exit(hi6401_spk_exit);

MODULE_DESCRIPTION("hisi_6401_spk driver");
MODULE_LICENSE("GPL");
