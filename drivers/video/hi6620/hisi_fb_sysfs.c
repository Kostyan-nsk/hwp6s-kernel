/* Copyright (c) 2008-2011, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
#include "k3_fb.h"

extern wait_queue_head_t overlay_idle_wait_queue;
static ssize_t hisifb_overlay_idle_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret = 0;
	struct fb_info *fbi = NULL;
	struct k3_fb_data_type *hisifd = NULL;

	fbi = dev_get_drvdata(dev);
	BUG_ON(fbi == NULL);
	hisifd = (struct k3_fb_data_type *)fbi->par;
	BUG_ON(hisifd == NULL);

	ret = wait_event_interruptible(overlay_idle_wait_queue, hisifd->ovc_idle_flag);
	hisifd->ovc_idle_flag = false;

	return 0;
}

extern int isFHD;
extern struct mipi_dsi_phy_register phy_ctrl[5];
static ssize_t hisifb_set_dsi_freq_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t size)
{
	u32 dsi_freq_index = 0;
	struct fb_info *fbi = NULL;
	struct k3_fb_data_type *hisifd = NULL;

	fbi = dev_get_drvdata(dev);
	BUG_ON(fbi == NULL);
	hisifd = (struct k3_fb_data_type *)fbi->par;
	BUG_ON(hisifd == NULL);

	if ((!*buf)) {
		k3fb_loge("Input param is error!\n");
		return 0;
	}
	dsi_freq_index = simple_strtoul(buf, NULL, 10);
	if (isFHD) {
		switch(dsi_freq_index) {
			case 1:
				hisifd->phy_ctrl1 = phy_ctrl[0];
				hisifd->panel_info.mipi.dsi_bit_clk = DSI_BIT_CLK_473;
				break;
			case 10:
				hisifd->phy_ctrl1 = phy_ctrl[1];
				hisifd->panel_info.mipi.dsi_bit_clk = DSI_BIT_CLK_480;
				break;
			case 100:
				hisifd->phy_ctrl1 = phy_ctrl[2];
				hisifd->panel_info.mipi.dsi_bit_clk = DSI_BIT_CLK_486;
				break;
			case 1000:
				hisifd->phy_ctrl1 = phy_ctrl[3];
				hisifd->panel_info.mipi.dsi_bit_clk = DSI_BIT_CLK_492;
				break;
			case 10000:
				hisifd->phy_ctrl1 = phy_ctrl[4];
				hisifd->panel_info.mipi.dsi_bit_clk = DSI_BIT_CLK_499;
				break;
			default:
				k3fb_loge("dsi_freq_index = %d is invalid \n", dsi_freq_index);
				break;
		}
	}

	return size;
}

/*lint -e665*/
#if defined(CONFIG_OVERLAY_COMPOSE)
#ifndef PC_UT_TEST_ON
static DEVICE_ATTR(overlay_idle_state, (S_IRUGO), hisifb_overlay_idle_show, NULL);
static DEVICE_ATTR(lcd_mipi_freq, (S_IWUSR), NULL, hisifb_set_dsi_freq_store);
#endif
#endif //CONFIG_OVERLAY_COMPOSE
/*lint +e665*/

static struct attribute *hisi_fb_attrs[] = {
#if defined(CONFIG_OVERLAY_COMPOSE)
#ifndef PC_UT_TEST_ON
	&dev_attr_overlay_idle_state.attr,
	&dev_attr_lcd_mipi_freq.attr,
#endif
#endif //CONFIG_OVERLAY_COMPOSE
	NULL,
};
static struct attribute_group hisi_fb_attr_group = {
	.attrs = hisi_fb_attrs,
};

int hisi_fb_sysfs_create(struct platform_device *pdev)
{
	int ret = 0;
	struct k3_fb_data_type *hisifd = NULL;
	hisifd = platform_get_drvdata(pdev);
	BUG_ON(hisifd == NULL);

	ret = sysfs_create_group(&hisifd->fbi->dev->kobj, &hisi_fb_attr_group);
	if (ret) {
		k3fb_loge("create sysfs file failed, error=%d!\n", ret);
		return ret;
	}

	return ret;
}

void hisi_fb_sysfs_remove(struct platform_device *pdev)
{
	struct k3_fb_data_type *hisifd = NULL;
	hisifd = platform_get_drvdata(pdev);
	BUG_ON(hisifd == NULL);

	sysfs_remove_group(&hisifd->fbi->dev->kobj, &hisi_fb_attr_group);
}
