/*
 * Copyright (c) 2012-2013 Huawei Ltd.
 *
 * Author: 
 *
 * This	program	is free	software; you can redistribute it and/or modify
 * it under	the	terms of the GNU General Public	License	version	2 as
 * published by	the	Free Software Foundation.
 */

/* dev_wifi.c
 */

/*=========================================================================
 *
 * histoty
 *
 *=========================================================================
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/err.h>
#include <linux/random.h>
#include <linux/skbuff.h>
#include <linux/wifi_tiwlan.h>
#include <linux/gpio.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#include <linux/regulator/consumer.h>
#include <linux/ctype.h>
#include <linux/mmc/dw_mmc.h>
#ifdef CONFIG_HWCONNECTIVITY
#include <linux/huawei/hw_connectivity.h>
#endif
#include <generated/mach-types.h>
#include <asm/mach-types.h>
#include <linux/mux.h>
#include <linux/clk.h>
#include <linux/mtd/nve_interface.h>
#include <hisi/drv_regulator_user.h>
#include <mach/clk_name_interface.h>
#include <mach/iomux_blocks.h>
#include <reg_ops.h>

#include "dev_wifi.h"

extern int hsad_get_wifi_enable(void);
extern int hsad_get_wifi_irq(void);
struct wifi_host_s {
	struct regulator *vdd;
	struct clk *clk;
	struct iomux_block *block;
	struct block_config *config;
	bool bEnable;
};

static int gpio_wifi_en = -1;
static int gpio_wifi_irq = -1;
typedef struct wifi_mem_prealloc_struct {
	void *mem_ptr;
	unsigned long size;
} wifi_mem_prealloc_t;


struct wifi_host_s *wifi_host;
static struct sk_buff *wlan_static_skb[WLAN_SKB_BUF_NUM];
unsigned char g_wifimac[WLAN_MAC_LEN] = {0x00,0x00,0x00,0x00,0x00,0x00};
static wifi_mem_prealloc_t wifi_mem_array[PREALLOC_WLAN_NUMBER_OF_SECTIONS] = {
	{ NULL, (WLAN_SECTION_SIZE_0 + PREALLOC_WLAN_SECTION_HEADER) },
	{ NULL, (WLAN_SECTION_SIZE_1 + PREALLOC_WLAN_SECTION_HEADER) },
	{ NULL, (WLAN_SECTION_SIZE_2 + PREALLOC_WLAN_SECTION_HEADER) },
	{ NULL, (WLAN_SECTION_SIZE_3 + PREALLOC_WLAN_SECTION_HEADER) }
};

/*kmalloc memory for wifi*/
void *wifi_mem_prealloc(int section, unsigned long size)
{
	if (section == PREALLOC_WLAN_NUMBER_OF_SECTIONS)
		return wlan_static_skb;
	if ((section < 0) || (section > PREALLOC_WLAN_NUMBER_OF_SECTIONS)) {
		pr_err("%s: is not prealloc(section:%d), use dynamic memory.\n", __func__, section);
		return NULL;
	}
	if (wifi_mem_array[section].size < size) {
		pr_err("%s: section:%d prealloc is not enough(size:%lu), use dynamic memory.\n", __func__, section, size);
		return NULL;
	}
	return wifi_mem_array[section].mem_ptr;
}

/*init wifi buf*/
int init_wifi_mem(void)
{
	int i = 0;

	pr_info("init_wifi_mem.\n");
	for (i = 0; i < WLAN_SKB_BUF_NUM; i++) {
		if (i < (WLAN_SKB_BUF_NUM / 2))
			wlan_static_skb[i] = dev_alloc_skb(WLAN_SKB_BUF_MIN);
		else
			wlan_static_skb[i] = dev_alloc_skb(WLAN_SKB_BUF_MAX);
		if (wlan_static_skb[i] == NULL) {
			pr_err("%s: dev_alloc_skb is error(%d).\n", __func__, i);
			return -ENOMEM;
		}
	}

	for	(i = 0; i < PREALLOC_WLAN_NUMBER_OF_SECTIONS; i++) {
		wifi_mem_array[i].mem_ptr = kzalloc(wifi_mem_array[i].size,
			GFP_KERNEL);
		if (wifi_mem_array[i].mem_ptr == NULL) {
			pr_err("%s: alloc mem_ptr is error(%d).\n", __func__, i);
			return -ENOMEM;
		}
	}
	return 0;
}

/*deinit wifi buf*/
int deinit_wifi_mem(void)
{
	int i = 0;

	pr_info("deinit_wifi_mem.\n");
	for (i = 0; i < WLAN_SKB_BUF_NUM; i++) {
		if (wlan_static_skb[i] != NULL) {
			dev_kfree_skb(wlan_static_skb[i]);
			wlan_static_skb[i] = NULL;
		} else
			break;
	}
	for	(i = 0;	i < PREALLOC_WLAN_NUMBER_OF_SECTIONS; i++) {
		if (wifi_mem_array[i].mem_ptr != NULL) {
			kfree(wifi_mem_array[i].mem_ptr);
			wifi_mem_array[i].mem_ptr = NULL;
		} else
			break;
	}
	return 0;
}

static void read_from_global_buf(unsigned char * buf)
{
	memcpy(buf,g_wifimac,WLAN_MAC_LEN);
	printk("get MAC from g_wifimac: mac=%02x:%02x:%02x:%02x:%02x:%02x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	return;
}

static int char2_byte( char* strori, char* outbuf )
{
	int i = 0;
	int temp = 0;
	int sum = 0;
	char *ptemp;
	char tempchar[20]={0};

	ptemp = tempchar;

	for (i = 0; i< WLAN_VALID_SIZE;i++){
		if(strori[i]!=':'){
			*ptemp = strori[i];
			 ptemp++;
		}
	}

	for( i = 0; i < NV_WLAN_VALID_SIZE; i++ ){

		switch (tempchar[i]) {
			case '0' ... '9':
				temp = tempchar[i] - '0';
				break;
			case 'a' ... 'f':
				temp = tempchar[i] - 'a' + 10;
				break;
			case 'A' ... 'F':
				temp = tempchar[i] - 'A' + 10;
				break;
			default:
				return 0;
		}
		sum += temp;
		if( i % 2 == 0 ){
			outbuf[i/2] |= temp << 4;
		}
		else{
			outbuf[i/2] |= temp;
		}
	}
	return sum;
}


static int read_from_mac_file(unsigned char * buf)
{
	struct file* filp = NULL;
	mm_segment_t old_fs;
	int result = 0;
	int sum = 0;
	char buf1[20] = {0};
	char buf2[6] = {0};

	if (NULL == buf) {
		pr_err("%s: buf is NULL\n", __func__);
		return -1;
	}

	filp = filp_open(MAC_ADDRESS_FILE, O_RDONLY,0);
	if(IS_ERR(filp)){
		pr_err("open mac address file error\n");
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	filp->f_pos = 0;
	result = filp->f_op->read(filp,buf1,WLAN_VALID_SIZE,&filp->f_pos);
	if(WLAN_VALID_SIZE != result){
		pr_err("read mac address file error\n");
		set_fs(old_fs);
		filp_close(filp,NULL);
		return -1;
	}

	sum = char2_byte(buf1,buf2);
	if (0 != sum){
		pr_err("get MAC from file: mac=%02x:%02x:%02x:%02x:%02x:%02x\n",buf2[0],buf2[1],buf2[2],buf2[3],buf2[4],buf2[5]);
		memcpy(buf,buf2,WLAN_MAC_LEN);
	}else{
		set_fs(old_fs);
		filp_close(filp,NULL);
		return -1;
	}

	set_fs(old_fs);
	filp_close(filp,NULL);
	return 0;
}

int bcm_wifi_get_mac_addr(unsigned char *buf)
{
	int ret = -1;

	if (NULL == buf) {
		pr_err("%s: k3v2_wifi_get_mac_addr failed\n", __func__);
		return -1;
	}

	memset(buf, 0, WLAN_MAC_LEN);
	if (0 != g_wifimac[0] || 0 != g_wifimac[1] || 0 != g_wifimac[2] || 0 != g_wifimac[3]|| 0 != g_wifimac[4] || 0 != g_wifimac[5]){
		read_from_global_buf(buf);
		return 0;
	}

	ret = read_from_mac_file(buf);
	if(0 == ret){
		pr_err("%s:read from mac addr file success \n",__func__);
		memcpy(g_wifimac,buf,WLAN_MAC_LEN);
		return 0;
	}else{
		get_random_bytes(buf,WLAN_MAC_LEN);
		buf[0] = 0x0;
		pr_err("get MAC from Random: mac=%02x:%02x:%02x:%02x:%02x:%02x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
		memcpy(g_wifimac,buf,WLAN_MAC_LEN);
	}

	return 0;
}


int bcm_wifi_power(int on)
{
	int ret = 0;
	pr_info("%s: on:%d\n", __func__, on);

	if (NULL == wifi_host) {
		pr_err("%s: wifi_host is null\n", __func__);
		return -1;
	}

	if (on) {
		if (wifi_host->bEnable) {
			pr_err("%s: wifi had power on.\n", __func__);
			return ret;
		}

		ret = blockmux_set(wifi_host->block, wifi_host->config, LOWPOWER);
		if (ret < 0)
		{
			pr_err("%s: blockmux_set LOWPOWER failed, ret:%d\n", __func__, ret);
			return ret;
		}
		pr_info("%s: power up\n", __func__);
		ret = clk_enable(wifi_host->clk);
		if (ret < 0) {
			pr_err("%s: clk_enable failed, ret:%d\n", __func__, ret);
			return ret;
		}

		msleep(10);
		gpio_set_value(gpio_wifi_en, 0);
		msleep(10);
		gpio_set_value(gpio_wifi_en, 1);
		msleep(100);

		ret = blockmux_set(wifi_host->block, wifi_host->config, NORMAL);
		if (ret < 0) {
			pr_err("%s: blockmux_set NORMAL failed, ret:%d\n", __func__, ret);
			clk_disable(wifi_host->clk);
			return ret;
		}

		hi_sdio_set_power(on);
		wifi_host->bEnable = true;
	} else {
		if (!wifi_host->bEnable) {
			pr_err("%s: wifi had power off\n", __func__);
			return ret;
		}

		ret = blockmux_set(wifi_host->block, wifi_host->config, LOWPOWER);
		if (ret < 0)
		{
			pr_err("%s: blockmux_set LOWPOWER failed, ret:%d\n", __func__, ret);
			return ret;
		}

		msleep(10);
		hi_sdio_set_power(on);
		gpio_set_value(gpio_wifi_en, 0);
		clk_disable(wifi_host->clk);
		wifi_host->bEnable = false;
	}

	return ret;
}



static int bcm_wifi_reset(int on)
{
	pr_info("%s: on:%d.\n", __func__, on);
	if (on)
		gpio_set_value(gpio_wifi_en, 1);
	else
		gpio_set_value(gpio_wifi_en, 0);
	return 0;
}


extern int dhd_msg_level;

static ssize_t show_wifi_debug_level(struct device *dev,
        struct device_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", dhd_msg_level);
}
static ssize_t restore_wifi_debug_level(struct device *dev, struct device_attribute *attr,
        const char *buf, size_t size) {
    int value;
    if (sscanf(buf, "%d\n", &value) == 1) {
#ifdef WL_CFG80211
        pr_err("restore_wifi_debug_level\n");
		/* Enable DHD and WL logs in oneshot */
		if (value & DHD_WL_VAL2)
			wl_cfg80211_enable_trace(TRUE, value & (~DHD_WL_VAL2));
		else if (value & DHD_WL_VAL)
			wl_cfg80211_enable_trace(FALSE, WL_DBG_DBG);
		if (!(value & DHD_WL_VAL2))
#endif /* WL_CFG80211 */
        dhd_msg_level = value;
        return size;
    }
    return -1;
}

extern int wl_get_wrong_action_flag(void);
extern int wl_trigger_disable_nmode(void);

static ssize_t show_wifi_wrong_action_flag(struct device *dev,
        struct device_attribute *attr, char *buf) {
    int has_wrong_action = wl_get_wrong_action_flag();
    printk(KERN_INFO "%s has wrong action %d\n", __func__, has_wrong_action);
    return sprintf(buf, "%d\n", has_wrong_action);
}

static ssize_t restore_wifi_arp_timeout(struct device *dev, struct device_attribute *attr,
        const char *buf, size_t size) {
    int value;
    if (sscanf(buf, "%d\n", &value) == 1) {
        if(value == 1) {
            printk(KERN_INFO "%s enter should invoke wrong action handler\n", __func__);
            wl_trigger_disable_nmode();
        }
        return size;
    }
    return -1;
}

static ssize_t restore_wifi_wrong_action_debug(struct device *dev, struct device_attribute *attr,
        const char *buf, size_t size) {
    int value;
    if (sscanf(buf, "%d\n", &value) == 1) {
        if(value == 1) {
            printk(KERN_INFO "%s enter should invoke wrong action handler\n", __func__);
            wl_trigger_disable_nmode();
        }
        return size;
    }
    return -1;
}

static DEVICE_ATTR(wifi_debug_level, S_IRUGO | S_IWUSR | S_IWGRP,
        show_wifi_debug_level, restore_wifi_debug_level);

static DEVICE_ATTR(wifi_wrong_action_flag, S_IRUGO,
        show_wifi_wrong_action_flag, NULL);

static DEVICE_ATTR(wifi_arp_timeout, S_IWUSR | S_IWGRP,
        NULL, restore_wifi_arp_timeout);

static DEVICE_ATTR(wifi_wrong_action_debug, S_IWUSR | S_IWGRP,
        NULL, restore_wifi_wrong_action_debug);

static struct attribute *attr_debug_attributes[] = {
    &dev_attr_wifi_debug_level.attr,
    NULL
};

static struct attribute *attr_arp_attributes[] = {
    &dev_attr_wifi_wrong_action_flag.attr,
    &dev_attr_wifi_arp_timeout.attr,
    &dev_attr_wifi_wrong_action_debug.attr,
    NULL
};

static const struct attribute_group attrgroup_debug_level = {
    .attrs = attr_debug_attributes,
};

static const struct attribute_group attrgroup_arp_timeout = {
	.attrs = attr_arp_attributes,
};

static int wifi_open_err_code = 0;
static ssize_t show_wifi_open_state(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", wifi_open_err_code);
}
static ssize_t restore_wifi_open_state(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	int value;
	if (sscanf(buf, "%d\n", &value) == 1) {
		wifi_open_err_code = value;
		return size;
	}
	return -1;
}

void set_wifi_open_err_code(int err_num)
{
    wifi_open_err_code = err_num;
}

static DEVICE_ATTR(wifi_open_state, S_IRUGO | S_IWUSR,
				   show_wifi_open_state, restore_wifi_open_state);

static struct attribute *wifi_state_attributes[] = {
	&dev_attr_wifi_open_state.attr,
	NULL
};
static const struct attribute_group wifi_state = {
	.attrs = wifi_state_attributes,
};

struct wifi_platform_data bcm_wifi_control = {
	.set_power = bcm_wifi_power,
	.set_reset = bcm_wifi_reset,
	.set_carddetect = hi_sdio_detectcard_to_core,
	.get_mac_addr = bcm_wifi_get_mac_addr,
	.mem_prealloc = wifi_mem_prealloc,
};

static struct resource bcm_wifi_resources[] = {
	[0] = {
	.name  = "bcmdhd_wlan_irq",
	.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL
			| IRQF_NO_SUSPEND,
	},
};

static struct platform_device bcm_wifi_device = {
	.name = "bcmdhd_wlan",
	.id = 1,
	.num_resources = ARRAY_SIZE(bcm_wifi_resources),
	.resource = bcm_wifi_resources,
	.dev = {
		.platform_data = &bcm_wifi_control,
	},
};

static int init_wifi_gpio(void)
{
    gpio_wifi_en = hsad_get_wifi_enable();
    if (gpio_wifi_en < 0){
        pr_err("%s: wifi enable gpio get failed gpio_wifi_en=%d\n", __func__, gpio_wifi_en);
        return -1;
    }

    gpio_wifi_irq = hsad_get_wifi_irq();
    if (gpio_wifi_irq < 0){
        pr_err("%s: wifi irq gpio get failed gpio_wifi_irq=%d\n", __func__, gpio_wifi_irq);
        return -1;
    }
    pr_err("init_wifi_gpio: wifi_en:%d, wifi_irq:%d====\n", gpio_wifi_en, gpio_wifi_irq); //debug

    bcm_wifi_device.resource[0].start = gpio_to_irq(gpio_wifi_irq);
    bcm_wifi_device.resource[0].end = gpio_to_irq(gpio_wifi_irq);

    return 0;
}


int  wifi_power_probe()
{
	int ret = 0;
	int i = 0;
	//struct timeval s_time_rec;

	ret = init_wifi_mem();
	if (ret) {
		pr_err("%s: init_wifi_mem failed.\n", __func__);
		goto err_malloc_wifi_host;
	}
	ret = init_wifi_gpio();
	if(ret){
		pr_err("%s: init_wifi_gpio failed.\n", __func__);
		goto err_malloc_wifi_host;
	}

	wifi_host = kzalloc(sizeof(struct wifi_host_s), GFP_KERNEL);
	if (!wifi_host)	{
		pr_err("%s: malloc wifi_host failed.\n", __func__);
		ret = -ENOMEM;
		goto err_malloc_wifi_host;
	}

	wifi_host->bEnable = false;

	/* get 32kb clock */
	wifi_host->clk = clk_get(NULL, CLK_PMU32KB);
	if (IS_ERR(wifi_host->clk)) {
		pr_err("%s: clk_get failed\n", __func__);
		ret = -ENXIO;
		goto err_clk_get;
	}

	/* get wifiio vdd */
	wifi_host->vdd = regulator_get(NULL, WIFI_IO_VDD);
	if (IS_ERR(wifi_host->vdd)) {
		pr_err("%s: regulator_get failed.\n", __func__);
		ret = -ENXIO;
		goto err_regulator_get;
	}
/* Regulator cannot be controlled*/
#if 0
	ret = regulator_set_voltage(wifi_host->vdd,
		K3V2_WIFI_VDD_VOLTAGE, K3V2_WIFI_VDD_VOLTAGE);
	if (ret < 0) {
		pr_err("%s: regulator_set_voltage failed, ret:%d.\n",
			__func__, ret);
		ret = -ENXIO;
		goto err_regulator_set_voltage;
	}
#endif
	/* set io mux*/
	wifi_host->block = iomux_get_block(BLOCK_WIFI);
	if (!wifi_host->block) {
		pr_err("%s: iomux_lookup_block failed.\n", __func__);
		ret = -ENXIO;
		goto err_put_regulator;
	}

	wifi_host->config = iomux_get_blockconfig(BLOCK_WIFI);
	if (!wifi_host->config) {
		pr_err("%s: iomux_get_blockconfig failed.\n", __func__);
		ret = -ENXIO;
		goto err_put_regulator;
	}

	ret	= blockmux_set(wifi_host->block, wifi_host->config, LOWPOWER);
	if (ret < 0) {
		pr_err("%s: blockmux_set failed, ret.\n", __func__);
		goto err_put_regulator;
	}

#if defined CONFIG_MACH_HI6620SFT
	/* fpga VDD open forver,if can not request other driver maybe has open*/
	ret = gpio_request(K3V2_WIFI_VDD_GPIO, NULL);
	if (ret < 0) {
		pr_err("%s: gpio_request %d failed, ret:%d.\n", __func__,
			K3V2_WIFI_VDD_GPIO,ret);
	} else {
		gpio_direction_output(K3V2_WIFI_VDD_GPIO, 1);
    }
#endif
	 /*set power gpio*/
	ret = gpio_request(gpio_wifi_en, "wifi_en_gpio");
	if (ret < 0) {
		pr_err("%s: gpio_request %d  failed, ret:%d .\n", __func__,gpio_wifi_en,ret);
		goto err_put_regulator;
	}
	gpio_direction_output(gpio_wifi_en, 0);
	/* set apwake gpio */
	ret = gpio_request(gpio_wifi_irq, "wifi_irq_gpio");
	if (ret < 0) {
		pr_err("%s: gpio_request %d failed, ret:%d.\n", __func__,gpio_wifi_irq,ret);
		goto err_irq_gpio_request;
	}
	gpio_direction_input(gpio_wifi_irq);
#if 0
	phy_reg_writel(REG_BASE_IOC_ON,
                     0xF800182C/* IOCG028*/,
                     0,
                      1,
                      0x00/*no pull, disable pull down*/);
#endif
#ifdef CONFIG_WIFI_CONTROL_FUNC
	ret = platform_device_register(&bcm_wifi_device);
	if (ret) {
		pr_err("%s: platform_device_register failed, ret:%d.\n",
			__func__, ret);
		goto err_platform_device_register;
	}
	ret = sysfs_create_group(&bcm_wifi_device.dev.kobj, &attrgroup_debug_level);
	if (ret) {
		pr_err("wifi_power_probe create debug level error ret =%d", ret);
	}

	ret = sysfs_create_group(&bcm_wifi_device.dev.kobj, &attrgroup_arp_timeout);
	if (ret) {
		pr_err("wifi_power_probe create arp trigger error ret =%d", ret);
	}

	ret = sysfs_create_group(&bcm_wifi_device.dev.kobj, &wifi_state);
	if (ret) {
		pr_err("wifi_power_probe sysfs_create_group error ret =%d", ret);
	}
#endif

	return 0;

err_platform_device_register:
	gpio_free(gpio_wifi_irq);
err_irq_gpio_request:
	gpio_free(gpio_wifi_en);
err_put_regulator:
	regulator_put(wifi_host->vdd);
err_regulator_get:
	clk_put(wifi_host->clk);
err_clk_get:
	kfree(wifi_host);
	wifi_host = NULL;
	//gpio_free(K3V2_WIFI_VDD_GPIO);
err_malloc_wifi_host:
	deinit_wifi_mem();
	return ret;
}

#ifdef _DRV_LLT_
int __init wifi_power_init(void)
#else
static int __init wifi_power_init(void)
#endif
{
	int ret;

	pr_err("enter: %s, %d.\n", __FUNCTION__, __LINE__);
	wifi_power_probe();
	pr_err("enter: %s, %d.\n", __FUNCTION__, __LINE__);
	return ret;
}

device_initcall(wifi_power_init); 
