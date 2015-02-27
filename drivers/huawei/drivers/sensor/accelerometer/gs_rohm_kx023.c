/* drivers/input/accelerometer/gs_kxtik1004.c*/
/*
 * Copyright (C) 2012 HUAWEI, Inc.
 * Author: zhangmin/195861 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/board_sensors.h>
#include "gs_rohm_kx023.h"

#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <linux/hw_dev_dec.h>
#endif



#define GS_DEBUG(fmt, args...) printk(KERN_ERR fmt, ##args)

/*DBG */
/*This is the classcial Delay_time from framework and the units is ms*/
#define DELAY_FASTEST  10
#define DELAY_GAME     20
#define DELAY_UI       68
#define DELAY_NORMAL  200
#define DELAY_ERROR 10000

/*
 * The following table lists the maximum appropriate poll interval for each
 * available output data rate.
 */
static const struct {
	unsigned int cutoff;
	u8 mask;
} kxtik_odr_table[] = {
	{ DELAY_FASTEST,ODR200F},
	{ DELAY_GAME,   ODR100F},
	{ DELAY_UI,      ODR25F},
	{ DELAY_NORMAL,ODR12_5F},
	{ DELAY_ERROR, ODR12_5F},
};
#define ACCL_DATA_SIZE 6
static int kxtik_debug_mask ;
module_param_named(kxtik_debug, kxtik_debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP);

#define kxtik_DBG(x...) do {\
    if (kxtik_debug_mask) \
        printk(KERN_ERR x);\
    } while (0)
	
#define GS_POLLING   1

static struct workqueue_struct *gs_wq;
static  int sensor_status;
extern struct input_dev *sensor_dev;

struct gs_data {
    uint16_t addr; 
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct kx023_platform_data *pdata;
    int use_irq;
	int sub_type;
	struct mutex  mlock;
	struct hrtimer timer;
	struct work_struct  work;
	uint32_t flags;
	int (*power)(int on);
	struct early_suspend early_suspend;
};

static struct gs_data  *this_gs_data;
static int accel_delay = GS_KX_TIMRER;     /*1s*/
static atomic_t a_flag;
static atomic_t kxtik_status_flag;
struct input_dev *sensor_dev = NULL;
/*sunlibin added*/
static int gs_who_am_i = KIONIX_ACCEL_WHO_AM_I_KX023;
static int gs_ctrl_reg1 = KIONIX_ACCEL_WHO_AM_I_KX023;
static int gs_data_ctrl_reg = GRP7_DATA_CTRL;
static int gs_resolution_flag = KIONIX_ACCEL_RES_12BIT;



#ifdef CONFIG_HAS_EARLYSUSPEND
static void gs_early_suspend(struct early_suspend *h);
static void gs_late_resume(struct early_suspend *h);
#endif

static inline int reg_read(struct gs_data *gs , int reg)
{
    int val;
    mutex_lock(&gs->mlock);
    val = i2c_smbus_read_byte_data(gs->client, reg);
    if (val < 0)
    {
        printk(KERN_ERR "kxtik chip i2c %s failed! reg=0x%x, value=0x%x\n", __FUNCTION__, reg, val);
    }
    mutex_unlock(&gs->mlock);
    return val;
}

static inline int reg_write(struct gs_data *gs, int reg, uint8_t val)
{
    int ret;
    mutex_lock(&gs->mlock);
    ret = i2c_smbus_write_byte_data(gs->client, reg, val);
    if(ret < 0)
    {
        printk(KERN_ERR "kxtik chip i2c %s failed! reg=0x%x, value=0x%x, ret=%d\n", __FUNCTION__, reg, val, ret);
    }
    mutex_unlock(&gs->mlock);

    return ret;
}

static signed short gs_sensor_data[3];

static int gs_data_to_compass(signed short accel_data [3])
{
       /* coverity: remove unuseful code to avoid coverity error */
	accel_data[0]=gs_sensor_data[0];
	accel_data[1]=gs_sensor_data[1];
	accel_data[2]=gs_sensor_data[2];
	return 0;
}
static void gs_kxtik_update_odr(struct gs_data  *gs)
{
	int i;
	int reg = 0;
	int ret = 0;
	short time_reg;
	for (i = 0; i < ARRAY_SIZE(kxtik_odr_table); i++) 
	{
		time_reg = kxtik_odr_table[i].mask;
		if (accel_delay <= kxtik_odr_table[i].cutoff)
		{
			accel_delay = kxtik_odr_table[i].cutoff;
			break;
		}
	}
	kxtik_DBG("%s:  accel_delay=%d,  time_reg=%d \n",__func__, accel_delay, time_reg);
	/*kxtik doesn't need to use mask,this register's fuction is independence*/
	reg  = reg_read(gs, gs_ctrl_reg1);
	ret  = reg_write(gs, gs_ctrl_reg1,0x00);
	ret |= reg_write(gs,DATA_CTRL,time_reg);
	ret |= reg_write(gs,gs_ctrl_reg1,reg);
	if(ret < 0)
	{
		printk("register write failed \n ");
	}
}
static int gs_init_reg(struct gs_data  *gs)
{
	int ret = 0;
	/* 0x00 for stand-by mode */
	ret  = reg_write(gs, gs_ctrl_reg1, 0x00);
	mdelay(10);
	/* 0xD0 for operating mode,16/12-bit valid,range +/-8g */
	ret |= reg_write(gs, gs_ctrl_reg1, 0xD0);
	if(ret < 0)
	{
		printk("register write failed \n ");
		return ret;
	}
	return ret;
}
#if 0
static int gs_kxtik_open(struct inode *inode, struct file *file)
{	
	gs_init_reg(this_gs_data);
	atomic_set(&kxtik_status_flag, GS_RESUME);
	if (this_gs_data->use_irq)
		enable_irq(this_gs_data->client->irq);
	else
	{
		/* when device start, we set the default dilay time 68ms */
		hrtimer_start(&this_gs_data->timer, ktime_set(0, DELAY_UI * 1000000), HRTIMER_MODE_REL);
	}
	return nonseekable_open(inode, file);
}

static int gs_kxtik_release(struct inode *inode, struct file *file)
{
	int ret;
	ret  = reg_write(this_gs_data, gs_ctrl_reg1, 0x00);
	atomic_set(&kxtik_status_flag, GS_SUSPEND);
	if (this_gs_data->use_irq)
		disable_irq(this_gs_data->client->irq);
	else
		hrtimer_cancel(&this_gs_data->timer);
	accel_delay = GS_KX_TIMRER;	
	return 0;
}

static long
gs_kxtik_ioctl(struct file *file, unsigned int cmd,
	   unsigned long arg)
{
	/*
	struct kxtik_data *tik = file->private_data;
	struct input_dev *input_dev = tik->input_dev;
	*/
	void __user *argp = (void __user *)arg;
	signed short accel_buf[3];
	short flag;
	switch (cmd) 
	{
		case ECS_IOCTL_APP_SET_AFLAG:     /*set open acceleration sensor flag*/
			if (copy_from_user(&flag, argp, sizeof(flag)))
				return -EFAULT;
				break;
		case ECS_IOCTL_APP_SET_DELAY:
			if (copy_from_user(&flag, argp, sizeof(flag)))
				return -EFAULT;
				break;
		default:
				break;
	}

	switch (cmd) 
	{
		case ECS_IOCTL_APP_SET_AFLAG:
			atomic_set(&a_flag, flag);
			break;
		case ECS_IOCTL_APP_GET_AFLAG:  /*get open acceleration sensor flag*/
			flag = atomic_read(&a_flag);
			break;
		case ECS_IOCTL_APP_SET_DELAY:
			if(flag)
				accel_delay = flag;
			else
				accel_delay = 10;   /*10ms*/
			/*
			 * Set current interval to the greater of the minimum interval or
			 * the requested interval
			 */	
			gs_kxtik_update_odr(this_gs_data);
			break;
		case ECS_IOCTL_APP_GET_DELAY:
			flag = accel_delay;
			break;
		case ECS_IOCTL_READ_ACCEL_XYZ:
			gs_data_to_compass(accel_buf);
			break;
		default:
			break;
	}

	switch (cmd) 
	{
		case ECS_IOCTL_APP_GET_AFLAG:
			if (copy_to_user(argp, &flag, sizeof(flag)))
				return -EFAULT;
			break;
		case ECS_IOCTL_APP_GET_DELAY:
			if (copy_to_user(argp, &flag, sizeof(flag)))
				return -EFAULT;
			break;
		case ECS_IOCTL_READ_ACCEL_XYZ:
			if (copy_to_user(argp, &accel_buf, sizeof(accel_buf)))
				return -EFAULT;
			break;
		case ECS_IOCTL_READ_DEVICEID:
			if (copy_to_user(argp, kxtik_device_id, sizeof(kxtik_device_id)))
				return -EFAULT;
			break;
		default:
			break;
	}
	return 0;
}

static struct file_operations gs_kxtik_fops = {
	.owner = THIS_MODULE,
	.open = gs_kxtik_open,
	.release = gs_kxtik_release,
	.unlocked_ioctl = gs_kxtik_ioctl,
};

static struct miscdevice gsensor_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "accel",
	.fops = &gs_kxtik_fops,
};
#endif
static void gs_work_func(struct work_struct *work)
{
   s16 hw_d[3] = { 0 };
    s16 xyz[3] = {0};
    u8 u8xl, u8xh, u8yl, u8yh, u8zl, u8zh;
    int sesc = accel_delay / 1000;
    int nsesc = (accel_delay % 1000) * 1000000;
    struct gs_data *gs = container_of(work, struct gs_data, work);
    u8xl = reg_read(gs, XOUT_L_REG);
    u8xh = reg_read(gs, XOUT_H_REG);
    u8yl = reg_read(gs, YOUT_L_REG);
    u8yh = reg_read(gs, YOUT_H_REG);
    u8zl = reg_read(gs, ZOUT_L_REG);
    u8zh = reg_read(gs, ZOUT_H_REG);
    kxtik_DBG("%s:  xl:%d  xh:%d yl:%d yh:%d zl:%d zhl:%d \n",__func__, u8xl, u8xh, u8yl, u8yh, u8zl,u8zh);

	if(KIONIX_ACCEL_RES_16BIT == gs_resolution_flag)
	{
		hw_d[0] = (s16)((u8xh << 8) | u8xl);
		hw_d[1] = (s16)((u8yh << 8) | u8yl );
		hw_d[2] = (s16)((u8zh << 8) | u8zl );
		hw_d[0] = hw_d[0] /4;
		hw_d[1] = hw_d[1] /4;
		hw_d[2] = hw_d[2] /4;
		kxtik_DBG("%s:  x16:%+5d y16:%+5d z16:%+5d sec:%d nsec:%d\n",__func__, hw_d[0], hw_d[1], hw_d[2], sesc, nsesc);
	} else {
		hw_d[0] = (s16)((u8xh << 8) | u8xl) >> 2;
		hw_d[1] = (s16)((u8yh << 8) | u8yl )>> 2;
		hw_d[2] = (s16)((u8zh << 8) | u8zl )>> 2;
		/*hw_d[0] = hw_d[0] * acc->sensitivity;
		hw_d[1] = hw_d[1] * acc->sensitivity;
		hw_d[2] = hw_d[2] * acc->sensitivity;*/
		kxtik_DBG("%s:  x14:%+5d y14:%+5d z14:%+5d sec:%d nsec:%d\n",__func__, hw_d[0], hw_d[1], hw_d[2], sesc, nsesc);		
	}

	
    	kxtik_DBG("%s:  x:%+5d y:%+5d z:%+5d \n",__func__, hw_d[0], hw_d[1], hw_d[2]);
      
	

	xyz[0] = ((gs->pdata->negate_x) ? (-hw_d[gs->pdata->axis_map_x])
		   : (hw_d[gs->pdata->axis_map_x]));
	xyz[1] = ((gs->pdata->negate_y) ? (-hw_d[gs->pdata->axis_map_y])
		   : (hw_d[gs->pdata->axis_map_y]));
	xyz[2] = ((gs->pdata->negate_z) ? (-hw_d[gs->pdata->axis_map_z])
		   : (hw_d[gs->pdata->axis_map_z]));
    input_report_abs(gs->input_dev, ABS_X,  xyz[0]);
    input_report_abs(gs->input_dev, ABS_Y,  xyz[1]);
    input_report_abs(gs->input_dev, ABS_Z,  xyz[2]);
    input_sync(gs->input_dev);
    /*
     * There is a transform formula between ABS_X, ABS_Y, ABS_Z
     * and Android_X, Android_Y, Android_Z.
     *                        -          -
     *                        |  0 -1  0 |
     * [ABS_X ABS_Y ABS_Z] *  |  1  0  0 | = [Android_X, Android_Y, Android_Z]
     *                        |  0  0 -1 |
     *                        -          -
     * compass uses Android_X, Andorid_Y, Android_Z
     */
    memset(gs_sensor_data, 0, sizeof(gs_sensor_data));

    gs_sensor_data[0]= xyz[0];
    gs_sensor_data[1]= xyz[1];
    gs_sensor_data[2]= xyz[2];

    if (gs->use_irq)
    {
        enable_irq(gs->client->irq);
    }
    else
    {
        if(GS_RESUME == atomic_read(&kxtik_status_flag))
            if (0 != hrtimer_start(&gs->timer, ktime_set(sesc, nsesc), HRTIMER_MODE_REL) )
                printk(KERN_ERR "%s, line %d: hrtimer_start fail! sec=%d, nsec=%d\n", __func__, __LINE__, sesc, nsesc);
    }
}


static enum hrtimer_restart gs_timer_func(struct hrtimer *timer)
{
	struct gs_data *gs = container_of(timer, struct gs_data, timer);		
	queue_work(gs_wq, &gs->work);
	return HRTIMER_NORESTART;
}

#ifndef   GS_POLLING 	
static irqreturn_t gs_irq_handler(int irq, void *dev_id)
{
	struct gs_data *gs = dev_id;
	disable_irq(gs->client->irq);
	queue_work(gs_wq, &gs->work);
	return IRQ_HANDLED;
}
/*modify Int_gpio for 8930*/
static int gs_config_int_pin(kx023_platform_data *pdata)
{
	/*differ platform different method config intertupt*/
	return 0;
}

static void gs_free_int(kx023_platform_data *pdata)
{

	gpio_free(pdata->int1_gpio);
	gpio_free(pdata->int2_gpio);

}
#endif

/*sunlibin added*/
#if 0
static compass_gs_position_type  get_compass_gs_position(int position)
{
	compass_gs_position_type compass_gs_position = COMPASS_TOP_GS_TOP;

	if((position < 0) || (position >= COMPASS_GS_POSITION_MAX))
	{
		pr_err("%s, %d: compass_gs_position is unknown \n", __func__, __LINE__);
		return -EFAULT;
	}
	
	switch(position)
	{
		case COMPASS_TOP_GS_TOP:
			compass_gs_position = COMPASS_TOP_GS_TOP;
			break;
			
		case COMPASS_TOP_GS_BOTTOM:
			compass_gs_position = COMPASS_TOP_GS_BOTTOM;
			break;
			
		case COMPASS_BOTTOM_GS_TOP:
			compass_gs_position = COMPASS_BOTTOM_GS_TOP;
			break;
			
		case COMPASS_BOTTOM_GS_BOTTOM:
			compass_gs_position = COMPASS_BOTTOM_GS_BOTTOM;
			break;
			
		case COMPASS_NONE_GS_BOTTOM:
			compass_gs_position = COMPASS_NONE_GS_BOTTOM;
			break;
			
		case COMPASS_NONE_GS_TOP:
			compass_gs_position = COMPASS_NONE_GS_TOP;
			break;
			
		default:
			break;
	}

	return compass_gs_position;
}
#endif
static ssize_t attr_kx023_set_polling_rate(struct device *dev, struct device_attribute *attr,
				const char *buf, size_t size)
{
	unsigned long val;
	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;
	printk("%s: val=%ld\n", __func__, val);
	if(val)
		accel_delay = val;
	else
		accel_delay = 10;   /*10ms*/
			/*
			 * Set current interval to the greater of the minimum interval or
			 * the requested interval
			 */	
	gs_kxtik_update_odr(this_gs_data);
	return size;
	
}
static ssize_t attr_kx023_get_polling_rate(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", accel_delay);
}
static ssize_t attr_kx023_get_accl_data(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	signed short accel_buf[3];
	gs_data_to_compass(accel_buf);
	*((int16_t *)&buf[0]) = accel_buf[0];
	*((int16_t *)&buf[2]) = accel_buf[1];
	*((int16_t *)&buf[4]) = accel_buf[2];
	return ACCL_DATA_SIZE;
}
static ssize_t attr_kx023_set_enable(struct device *dev, struct device_attribute *attr,
				const char *buf, size_t size)
{
	unsigned long val;
	int ret;
	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;
	GS_DEBUG("%s: val=%ld\n", __func__, val);
	if(val)
	{
		if(!atomic_cmpxchg(&a_flag, 0, 1))
		{
			gs_init_reg(this_gs_data);
	
			atomic_set(&kxtik_status_flag, GS_RESUME);
	
			if (this_gs_data->use_irq)
				enable_irq(this_gs_data->client->irq);
			else
			{	
		/* when device start, we set the default dilay time 68ms */
				hrtimer_start(&this_gs_data->timer, ktime_set(0, DELAY_UI * 1000000), HRTIMER_MODE_REL);
			}
			kxtik_DBG("%s: enable!!!!!\n",__func__);
			
		}
		else
		{
			GS_DEBUG("%s:already enable!!!!!\n",__func__);
		}
	}
	else
	{
		if(atomic_cmpxchg(&a_flag, 1, 0))
		{

			ret  = reg_write(this_gs_data, gs_ctrl_reg1, 0x00);
	
			atomic_set(&kxtik_status_flag, GS_SUSPEND);
	
			if (this_gs_data->use_irq)
				disable_irq(this_gs_data->client->irq);
			else
				hrtimer_cancel(&this_gs_data->timer);
			accel_delay = GS_KX_TIMRER;
			kxtik_DBG("%s: disable!!!!!\n",__func__);
		}
		else
		{
			GS_DEBUG("%s:already disable!!!!!\n",__func__);
		}
	}
	return size;
}
static ssize_t attr_kx023_get_enable(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int val = atomic_read(&a_flag);
	return sprintf(buf, "%d\n", val);
}
static ssize_t attr_kx023_get_accl_info(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Rohm kx023");
}
static struct device_attribute attributes[] = {

	__ATTR(pollrate_ms, 0664, attr_kx023_get_polling_rate, attr_kx023_set_polling_rate),
	//__ATTR(range, 0664, attr_kx023_get_range, attr_kx023_set_range),
	__ATTR(enable, 0664, attr_kx023_get_enable, attr_kx023_set_enable),
	__ATTR(accl_data, 0664, attr_kx023_get_accl_data, NULL),
	__ATTR(accl_info, 0664, attr_kx023_get_accl_info, NULL),
};

static int create_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		if (device_create_file(dev, attributes + i))
			goto error;
	return 0;

error:
	for ( ; i >= 0; i--)
		device_remove_file(dev, attributes + i);
	dev_err(dev, "%s:Unable to create interface\n", __func__);
	return -1;
}

static int remove_sysfs_interfaces(struct device *dev)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(attributes); i++)
		device_remove_file(dev, attributes + i);
	return 0;
}
static int gs_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;
	struct gs_data *gs;
	struct kx023_platform_data *pdata = NULL;
	pdata = kzalloc(sizeof(struct kx023_platform_data), GFP_KERNEL);
	if (!pdata) 
	{
		ret = -ENOMEM;
		goto err_exit;
	}
	
	if (client->dev.platform_data) 
	{
		memcpy(pdata, client->dev.platform_data, sizeof(struct kx023_platform_data));
		
	} 
	else 
	{
		GS_DEBUG("%s:platform data is NULL. exiting.\n", __func__);
		ret = -ENODEV;
		goto err_exit_data;
	}
	GS_DEBUG("my gs_probe_kxtik023\n");
    
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		GS_DEBUG("gs_probe: need I2C_FUNC_I2C\n");
		ret = -ENODEV;
		goto err_check_functionality_failed;
	}
	if (pdata){
		if(pdata->gs_power != NULL)
		{
			ret = pdata->gs_power(IC_PM_ON);
			if(ret < 0 )
			{
				goto err_check_functionality_failed;
			}
		}
		
		/*sunlibin added*/
		//compass_gs_position = get_compass_gs_position(pdata->compass_gs_position);
		gs_resolution_flag = pdata->accel_res;
	}
#ifndef   GS_POLLING 	
	ret = gs_config_int_pin(pdata);
	if(ret <0)
	{
		GS_DEBUG("%s, %d: gs_config_int_pin function failed because ret= %d\n", __func__, __LINE__,ret);
		goto err_power_failed;
	}
#endif
	gs = kzalloc(sizeof(*gs), GFP_KERNEL);
	if (gs == NULL) {
		GS_DEBUG("%s, %d: kzalloc function failed because gs=null\n", __func__, __LINE__);
		ret = -ENOMEM;
		goto err_alloc_data_failed;
	}
	sensor_status=0;
	mutex_init(&gs->mlock);
	INIT_WORK(&gs->work, gs_work_func);
	gs->client = client;
	gs->pdata= pdata;
	i2c_set_clientdata(client, gs);

	GS_DEBUG("mode is %x\n", reg_read(gs, 0x20));
	ret = reg_read(gs,WHO_AM_I);
	if(ret < 0)
	{
		pr_err("%s, %d: read who_am_i fail!\n", __func__, __LINE__);
		goto err_detect_failed;
	}else{
		gs->sub_type = ret;
		GS_DEBUG("%s, %d: read who_am_i success!gs->sub_type=%d\n", __func__, __LINE__,gs->sub_type);
	}

	switch (gs->sub_type) {
		case KIONIX_ACCEL_WHO_AM_I_KXTE9:
			ret = set_sensor_chip_info(ACC, "ROHM KXTE9");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTE9.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTF9:
			ret = set_sensor_chip_info(ACC, "ROHM KXTF9");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTF9.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTI9_1001:
			ret = set_sensor_chip_info(ACC, "ROHM KXTI9_1001");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTI9-1001.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTIK_1004:
			ret = set_sensor_chip_info(ACC, "ROHM KXTIK_1004");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTIK-1004.\n");
			gs_ctrl_reg1 = CTRL_REG1;
			gs_data_ctrl_reg = DATA_CTRL;
			/* 0x50 for stand-by mode,12-bit valid,range +/-8g */
			ret  = reg_write(gs, gs_ctrl_reg1, 0x50);
			if(ret <0)
			{
				pr_err("%s, %d: write ctrl_reg1 fail, ret= %d\n", __func__, __LINE__,ret);
			}
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTJ9_1005:
			ret = set_sensor_chip_info(ACC, "ROHM KXTJ9_1005");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTJ9-1005.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTJ9_1007:
			ret = set_sensor_chip_info(ACC, "ROHM KXTJ9_1007");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTJ9-1007.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXCJ9_1008:
			ret = set_sensor_chip_info(ACC, "ROHM KXCJ9_1008");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXCJ9-1008.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXTJ2_1009:
			ret = set_sensor_chip_info(ACC, "ROHM KXTJ2_1009");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXTJ2-1009.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KXCJK_1013:
			ret = set_sensor_chip_info(ACC, "ROHM KXCJK_1013");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KXCJK-1013.\n");
			break;
		case KIONIX_ACCEL_WHO_AM_I_KX023:
			ret = set_sensor_chip_info(ACC, "ROHM KX023");
			if (ret < 0)/*failed to add app_info*/
			{
				dev_err(&gs->client->dev, "%s %d:faile to add app_info\n", __func__, __LINE__);
			}
			dev_info(&gs->client->dev, "this accelerometer is a KX023.\n");
			gs_ctrl_reg1 = GRP7_CTRL_REG1;
			gs_data_ctrl_reg = GRP7_DATA_CTRL;
			/* 0xC0 for operating mode,16-bit valid. Bandwidth (Hz) = ODR/2,range +/-2g */
			ret  = reg_write(gs, gs_ctrl_reg1, 0xC0);
			if(ret <0)
			{
				pr_err("%s, %d: write ctrl_reg1 fail, ret= %d\n", __func__, __LINE__,ret);
			}
			break;
		default:
			dev_info(&gs->client->dev, "this accelerometer is unknown.\n");
			goto err_detect_failed;
	}

	gs_who_am_i = gs->sub_type;

	atomic_set(&kxtik_status_flag, GS_SUSPEND);
	atomic_set(&a_flag, 0);
	/*registe input device*/
	if (sensor_dev == NULL)
	{
		gs->input_dev = input_allocate_device();
		if (gs->input_dev == NULL) {
			ret = -ENOMEM;
			printk(KERN_ERR "gs_probe: Failed to allocate input device\n");
			goto err_input_dev_alloc_failed;
		}
		
		gs->input_dev->name = ACCL_INPUT_DEV_NAME;
		sensor_dev = gs->input_dev;
		
	}else{

		gs->input_dev = sensor_dev;
	}
	gs->input_dev->open = NULL;
	gs->input_dev->close = NULL;
	
	gs->input_dev->id.bustype = BUS_I2C;
	gs->input_dev->dev.parent = &gs->client->dev;
	gs->input_dev->id.vendor = GS_KXTIK1004;

	set_bit(EV_ABS,gs->input_dev->evbit);
	//provide by company
	input_set_abs_params(gs->input_dev, ABS_X, -11520, 11520, 3, 3);
	input_set_abs_params(gs->input_dev, ABS_Y, -11520, 11520, 3, 3);
	input_set_abs_params(gs->input_dev, ABS_Z, -11520, 11520, 3, 3);
	
	set_bit(EV_SYN,gs->input_dev->evbit);
	input_set_drvdata(gs->input_dev, gs);
	ret = input_register_device(gs->input_dev);
	if (ret) {
		printk(KERN_ERR "gs_probe: Unable to register %s input device\n", gs->input_dev->name);
		goto err_input_register_device_failed;
	}
	
	/*ret = misc_register(&gsensor_device);
	if (ret) {
		printk(KERN_ERR "gs_probe: gsensor_device register failed\n");
		goto err_misc_device_register_failed;
	}*/
	ret = create_sysfs_interfaces(&client->dev);
	if (ret < 0) {
		dev_err(&client->dev,
		   "device KX023_ACC_DEV_NAME sysfs register failed\n");
		goto err_input_register_device_failed;
	}
#ifndef   GS_POLLING 
	if (client->irq) {
		ret = request_irq(client->irq, gs_irq_handler, 0, client->name, gs);
		
		if (ret == 0)
			gs->use_irq = 1;
		else
			dev_err(&client->dev, "request_irq failed\n");
	}
#endif 

	if (!gs->use_irq) {
		hrtimer_init(&gs->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		gs->timer.function = gs_timer_func;
		
	}
	
#ifdef CONFIG_HAS_EARLYSUSPEND
	gs->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	gs->early_suspend.suspend = gs_early_suspend;
	gs->early_suspend.resume = gs_late_resume;
	register_early_suspend(&gs->early_suspend);
#endif

    gs_wq = create_singlethread_workqueue("gs_wq");
    if (!gs_wq)
    {
        ret = -ENOMEM;
        printk(KERN_ERR "%s, line %d: create_singlethread_workqueue fail!\n", __func__, __LINE__);
        goto err_create_workqueue_failed;
    }
    this_gs_data =gs;
   #ifdef CONFIG_HUAWEI_HW_DEV_DCT  
   /* detect current device successful, set the flag as present */  
    set_hw_dev_flag(DEV_I2C_G_SENSOR);  
   #endif
    GS_DEBUG("My G-sensor is KXTIK\n");
    return 0;

err_create_workqueue_failed:
#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&gs->early_suspend);
#endif

    if (gs->use_irq)
    {
        free_irq(client->irq, gs);
    }
    else
    {
        hrtimer_cancel(&gs->timer);
    }
    remove_sysfs_interfaces(&client->dev);
err_input_register_device_failed:
	input_free_device(gs->input_dev);
err_input_dev_alloc_failed:
err_detect_failed:
	kfree(gs);
err_alloc_data_failed:
#ifndef   GS_POLLING 
	gs_free_int(pdata);
#endif
/*8930 doesn't have to turn off the power*/
//err_power_failed:
err_check_functionality_failed:
err_exit_data:
	kfree(pdata);
err_exit:/*sunlibin added*/

	return ret;
}

static int gs_remove(struct i2c_client *client)
{
	struct gs_data *gs = i2c_get_clientdata(client);
#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&gs->early_suspend);
#endif
	if (gs->use_irq)
		free_irq(client->irq, gs);
	else
		hrtimer_cancel(&gs->timer);
	//misc_deregister(&gsensor_device);
	input_unregister_device(gs->input_dev);
	remove_sysfs_interfaces(&client->dev);

	kfree(gs);
	return 0;
}

#ifdef CONFIG_PM /*sunlibin added*/
static int gs_suspend(struct i2c_client *client, pm_message_t mesg)
{
	int ret;
	struct gs_data *gs = i2c_get_clientdata(client);
	ret  = reg_write(gs, gs_ctrl_reg1, 0x00);
	if(ret < 0)
	{
		printk("register write failed \n ");
		return ret;
	}
	atomic_set(&kxtik_status_flag, GS_SUSPEND);
	if (gs->use_irq)
		disable_irq(client->irq);
	else
		hrtimer_cancel(&gs->timer);
	ret = cancel_work_sync(&gs->work);
	if (ret && gs->use_irq) 
		enable_irq(client->irq);
	if (gs->power) {
		ret = gs->power(0);
		if (ret < 0)
			printk(KERN_ERR "gs_resume power off failed\n");
	}
	return 0;
}

static int gs_resume(struct i2c_client *client)
{
	struct gs_data *gs = i2c_get_clientdata(client);
	int sesc = accel_delay / 1000;
    	int nsesc = (accel_delay % 1000) * 1000000;
	GS_DEBUG("gs_resume a_flag=%d",(atomic_read(&a_flag)));
	atomic_set(&kxtik_status_flag, GS_RESUME);
	if(1==(atomic_read(&a_flag)))
	{
		gs_init_reg(gs);
		if (!gs->use_irq)
			hrtimer_start(&gs->timer, ktime_set(sesc, nsesc), HRTIMER_MODE_REL);
		else
			enable_irq(client->irq);
	}
	return 0;
}
#else /*sunlibin added*/
#define gs_suspend	NULL
#define gs_resume	NULL
#endif /* CONFIG_PM */

#ifdef CONFIG_HAS_EARLYSUSPEND
static void gs_early_suspend(struct early_suspend *h)
{
	struct gs_data *gs;
	gs = container_of(h, struct gs_data, early_suspend);
	gs_suspend(gs->client, PMSG_SUSPEND);
}

static void gs_late_resume(struct early_suspend *h)
{
	struct gs_data *gs;
	gs = container_of(h, struct gs_data, early_suspend);
	gs_resume(gs->client);
}
#endif

static const struct i2c_device_id gs_id[] = {
	{ /*GS_I2C_NAME*/"Rohm_accel", 0 },
	{ },
};

static struct of_device_id kx023_match_table[] = {
	{ .compatible = "gs,Rohm_accel", },
	{ },
};


static struct i2c_driver gs_driver = {
	.probe		= gs_probe,
	.remove		= gs_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= gs_suspend,
	.resume		= gs_resume,
#endif
	.id_table	= gs_id,
	.driver = {
		.name	= "Rohm_accel",
		.of_match_table = kx023_match_table,
	},
};

static int __devinit gs_kxtik_init(void)
{
	printk("gs_kxtik_init!!!!\n");
	return i2c_add_driver(&gs_driver);
}

static void __exit gs_kxtik_exit(void)
{
	i2c_del_driver(&gs_driver);
	if (gs_wq)
		destroy_workqueue(gs_wq);
}



module_init(gs_kxtik_init);
module_exit(gs_kxtik_exit);

MODULE_DESCRIPTION("accessor  Driver");
MODULE_LICENSE("GPL");

