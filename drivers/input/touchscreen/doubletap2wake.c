/*
 * drivers/input/touchscreen/doubletap2wake.c
 *
 *
 * Copyright (c) 2013, Dennis Rassmann <showp1984@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/input/doubletap2wake.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#include <linux/wakelock.h>
#ifndef CONFIG_HAS_EARLYSUSPEND
#include <linux/lcd_notify.h>
#else
#include <linux/earlysuspend.h>
#endif
#include <linux/hrtimer.h>
#include <asm-generic/cputime.h>
#include <hsad/config_mgr.h>

/* uncomment since no touchscreen defines android touch, do that here */
//#define ANDROID_TOUCH_DECLARED

/* if Sweep2Wake is compiled it will already have taken care of this */
#ifdef CONFIG_TOUCHSCREEN_SWEEP2WAKE
#define ANDROID_TOUCH_DECLARED
#endif

/* Version, author, desc, etc */
#define DRIVER_AUTHOR "Dennis Rassmann <showp1984@gmail.com>"
#define DRIVER_DESCRIPTION "Doubletap2wake for almost any device\nAdjusted for Huawei Ascend P6S-U06 by Kostyan_nsk"
#define DRIVER_VERSION "1.2"
#define LOGTAG "[doubletap2wake]: "

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPLv2");

/* Tuneables */
#define DT2W_DEBUG		0
#define DT2W_DEFAULT		0
#define DT2W_DUR_DEFAULT	60

#define DT2W_PWRKEY_DUR		60
#define DT2W_FEATHER		100
#define DT2W_TIME		500

/* Regions */
#define DT2W_TOP_HALF		2
#define DT2W_BOTTOM_HALF	3
#define DT2W_NAVBAR		4

struct touch {
	s64 touch_time;
	s64 untouch_time;
	int touch_x;
	int touch_y;
	bool touch_x_called;
	bool touch_y_called;
	bool touched;
	bool untouched;
};

struct tap {
	s64 tap_time;
	int x;
	int y;
	int touch_nr;
};

/* Resources */
int dt2w_switch = DT2W_DEFAULT;
int s2s_switch =  DT2W_DEFAULT;
static int dt2w_prev_switch, y_res, s2s_length = INT_MAX, x0 = -1, x_pre;
static unsigned int dt2w_duration = DT2W_DUR_DEFAULT, s2s_height, s2s_feather;
static struct touch tch;
static struct tap tp;
static bool scr_suspended = false, exec = false;
#ifndef CONFIG_HAS_EARLYSUSPEND
static struct notifier_block dt2w_lcd_notif;
#endif
static struct input_dev * doubletap2wake_pwrdev;
static DEFINE_MUTEX(pwrkeyworklock);
static struct workqueue_struct *dt2w_input_wq;
static struct work_struct dt2w_input_work;
static struct work_struct s2s_input_work;
static struct wake_lock dt2w_wakelock;

/* Read cmdline for dt2w */
static int __init read_dt2w_cmdline(char *dt2w)
{
	if (strcmp(dt2w, "1") == 0) {
		pr_info("[cmdline_dt2w]: DoubleTap2Wake enabled. | dt2w='%s'\n", dt2w);
		dt2w_switch = 1;
	} else if (strcmp(dt2w, "0") == 0) {
		pr_info("[cmdline_dt2w]: DoubleTap2Wake disabled. | dt2w='%s'\n", dt2w);
		dt2w_switch = 0;
	} else {
		pr_info("[cmdline_dt2w]: No valid input found. Going with default: | dt2w='%u'\n", dt2w_switch);
	}
	return 1;
}
__setup("dt2w=", read_dt2w_cmdline);

/* PowerKey work func */
static void doubletap2wake_presspwr(struct work_struct * doubletap2wake_presspwr_work) {

	if (!mutex_trylock(&pwrkeyworklock))
		return;

	input_event(doubletap2wake_pwrdev, EV_KEY, KEY_POWER, 1);
	input_event(doubletap2wake_pwrdev, EV_SYN, 0, 0);
	msleep(DT2W_PWRKEY_DUR);
	input_event(doubletap2wake_pwrdev, EV_KEY, KEY_POWER, 0);
	input_event(doubletap2wake_pwrdev, EV_SYN, 0, 0);
	pr_info("dt2w_presspwr\n");
	exec = false;
        mutex_unlock(&pwrkeyworklock);
}
static DECLARE_WORK(doubletap2wake_presspwr_work, doubletap2wake_presspwr);

/* init a new touch */
static void new_touch(int x, int y) {

	tp.tap_time = ktime_to_ms(ktime_get());
	tp.x = x;
	tp.y = y;
	tp.touch_nr++;
}

/* Doubletap2wake main function */
static void detect_doubletap2wake(int x, int y)
{
#if DT2W_DEBUG
        pr_info(LOGTAG "x, y(%4d, %4d)", x, y);
#endif
	if (!dt2w_switch || exec)
		return;

	if (tp.touch_nr == 0)
		new_touch(x, y);
	else
		if (tp.touch_nr == 1) {
			if (abs(x - tp.x) < DT2W_FEATHER &&
			    abs(y - tp.y) < DT2W_FEATHER &&
			    (ktime_to_ms(ktime_get()) - tp.tap_time) < DT2W_TIME)
				tp.touch_nr++;
			else {
				tp.touch_nr = 0;
				new_touch(x, y);
			}
		}
	if (tp.touch_nr > 1) {
		pr_info(LOGTAG "ON\n");
		exec = true;
		schedule_work(&doubletap2wake_presspwr_work);
		tp.touch_nr = 0;
	}
}

static void dt2w_input_callback(struct work_struct *unused) {

	detect_doubletap2wake(tch.touch_x, tch.touch_y);
	memset(&tch, 0, sizeof(struct touch));
}

static void detect_sweep2sleep(int x, int y) {

	if (!s2s_switch || exec)
		return;

	if (y < (y_res - s2s_height - s2s_feather)) {
		x0 = -1;
		return;
	}

	if (x0 == -1) {
		if (y < y_res - s2s_height) {
		return;
	    }
	    x0 = x_pre = x;
	    return;
	}

	if ((x0 <= x_pre && x_pre <= x) || (x0 >= x_pre && x_pre >= x)) {
		if (abs(x0 - x) >= s2s_length && y >= y_res - s2s_height) {
			exec = true;
			schedule_work(&doubletap2wake_presspwr_work);
			memset(&tch, 0, sizeof(struct touch));
			x0 = -1;
			return;
		}
		x_pre = x;
	}
	else
		x0 = -1;
}

static void s2s_input_callback(struct work_struct *unused) {

	detect_sweep2sleep(tch.touch_x, tch.touch_y);
}

static void dt2w_input_event(struct input_handle *handle, unsigned int type,
				unsigned int code, int value) {
#if DT2W_DEBUG
	pr_info("doubletap2wake: code: %s|%u, val: %i type: %d\n",
		code == ABS_MT_POSITION_X ? "X" :
		code == ABS_MT_POSITION_Y ? "Y" :
		code == BTN_TOUCH ? "TOUCH" :
		"undef", code, value, type);
#endif
	if (!scr_suspended && s2s_switch > 0) {
		if (code == BTN_TOUCH) {
			if (value == 1)
				tch.touched = true;
			else {
				tch.touched = false;
				x0 = -1;
			}
			return;
		}

		if (!tch.touched)
			return;

		if (code == ABS_MT_POSITION_X) {
			tch.touch_x = value;
			tch.touch_x_called = true;
		}
		if (code == ABS_MT_POSITION_Y) {
			tch.touch_y = value;
			tch.touch_y_called = true;
		}
		if (tch.touch_x_called && tch.touch_y_called && tch.touched) {
			tch.touch_x_called = tch.touch_y_called = false;
			queue_work_on(0, dt2w_input_wq, &s2s_input_work);
		}
	}

	if (!scr_suspended || dt2w_switch == 0)
		return;

	if (code == BTN_TOUCH) {
		if (value == 1) {
			tch.touch_time = ktime_to_ms(ktime_get());
			tch.touched = true;
		}
		else {
			tch.untouch_time = ktime_to_ms(ktime_get());
			tch.untouched = true;
		}
	}

	if (code == ABS_MT_POSITION_X) {
		tch.touch_x = value;
		tch.touch_x_called = true;
	}

	if (code == ABS_MT_POSITION_Y) {
		switch (dt2w_switch) {
		    case DT2W_TOP_HALF:
			if (value > y_res / 2)
			    return;
		    case DT2W_BOTTOM_HALF:
			if (value < y_res / 2)
			    return;
		    case DT2W_NAVBAR:
			switch (y_res) {
			    case 1280:
				if (value < y_res - 100)
				    return;
			    case 1920:
				if (value < y_res - 145)
				    return;
			}
		}
		tch.touch_y = value;
		tch.touch_y_called = true;
	}

	if (tch.touch_x_called && tch.touch_y_called && tch.touched && tch.untouched &&
	    (tch.untouch_time - tch.touch_time) < DT2W_TIME)
		queue_work_on(0, dt2w_input_wq, &dt2w_input_work);
}

static int dt2w_input_connect(struct input_handler *handler,
				struct input_dev *dev, const struct input_device_id *id) {
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "dt2w";

	error = input_register_handle(handle);
	if (error)
		goto err2;

	error = input_open_device(handle);
	if (error)
		goto err1;

	return 0;
err1:
	input_unregister_handle(handle);
err2:
	kfree(handle);
	return error;
}

static void dt2w_input_disconnect(struct input_handle *handle) {
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id dt2w_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.evbit = { BIT_MASK(EV_ABS) },
		.absbit = { [BIT_WORD(ABS_MT_POSITION_X)] =
			    BIT_MASK(ABS_MT_POSITION_X) |
			    BIT_MASK(ABS_MT_POSITION_Y) },
	}, /* multi-touch touchscreen */
	{
		.flags = INPUT_DEVICE_ID_MATCH_KEYBIT |
			 INPUT_DEVICE_ID_MATCH_ABSBIT,
		.keybit = { [BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH) },
		.absbit = { [BIT_WORD(ABS_X)] =
			    BIT_MASK(ABS_X) | BIT_MASK(ABS_Y) },
	}, /* touchpad */
	{ },
};

static struct input_handler dt2w_input_handler = {
	.event		= dt2w_input_event,
	.connect	= dt2w_input_connect,
	.disconnect	= dt2w_input_disconnect,
	.name		= "dt2w_inputreq",
	.id_table	= dt2w_ids,
};

#ifndef CONFIG_HAS_EARLYSUSPEND
static int lcd_notifier_callback(struct notifier_block *this,
				unsigned long event, void *data)
{
	switch (event) {
	case LCD_EVENT_ON_END:
		scr_suspended = false;
		break;
	case LCD_EVENT_OFF_END:
		scr_suspended = true;
		break;
	default:
		break;
	}

	return 0;
}
#else
static void dt2w_early_suspend(struct early_suspend *h) {
	scr_suspended = true;
	dt2w_prev_switch = dt2w_switch;
	if (dt2w_switch > 0) {
	    wake_lock_timeout(&dt2w_wakelock, dt2w_duration * HZ);
	}
}

static void dt2w_late_resume(struct early_suspend *h) {
	scr_suspended = false;
	dt2w_switch = dt2w_prev_switch;
	if (wake_lock_active(&dt2w_wakelock))
	    wake_unlock(&dt2w_wakelock);
}

static struct early_suspend dt2w_early_suspend_handler = {
	.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN,
	.suspend = dt2w_early_suspend,
	.resume = dt2w_late_resume,
};
#endif

/*
 * SYSFS stuff below here
 */
static ssize_t dt2w_doubletap2wake_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	size_t count = 0;

	if (scr_suspended)
	    count = sprintf(buf, "%u\n", dt2w_prev_switch);
	else
	    count = sprintf(buf, "%u\n", dt2w_switch);

	return count;
}

static ssize_t dt2w_doubletap2wake_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
	    return -EINVAL;
	if ((input > 1 && y_res == 0) || input > 4)
	    input = 1;
	if (scr_suspended) {
	    dt2w_prev_switch = input;
	    if (!input && wake_lock_active(&dt2w_wakelock))
		wake_unlock(&dt2w_wakelock);
	}
	else
	    dt2w_switch = input;
	return count;
}

static DEVICE_ATTR(doubletap2wake, (0666),
	dt2w_doubletap2wake_show, dt2w_doubletap2wake_store);

static ssize_t s2s_sweep2sleep_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	size_t count;
	    count = sprintf(buf, "%u\n", s2s_switch);

	return count;
}

static ssize_t s2s_sweep2sleep_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1 || y_res == 0)
	    return -EINVAL;

	s2s_switch = input;
	return count;
}

static DEVICE_ATTR(sweep2sleep, (0666),
	s2s_sweep2sleep_show, s2s_sweep2sleep_store);

static ssize_t dt2w_version_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	size_t count = 0;

	count += sprintf(buf, "%s\n", DRIVER_VERSION);

	return count;
}

static DEVICE_ATTR(dt2w_version, (S_IRUGO),
	dt2w_version_show, NULL);

static ssize_t dt2w_duration_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	size_t count = 0;

	count += sprintf(buf, "%u\n", dt2w_duration);

	return count;
}

static ssize_t dt2w_duration_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
	    return -EINVAL;
	dt2w_duration = input;
	return count;
}

static DEVICE_ATTR(dt2w_duration, (0666),
	dt2w_duration_show, dt2w_duration_store);

static ssize_t s2s_length_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	size_t count;
	    count = sprintf(buf, "%u\n", s2s_length);

	return count;
}

static ssize_t s2s_length_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int input;
	int ret;
	ret = sscanf(buf, "%u", &input);
	if (ret != 1)
	    return -EINVAL;
	if ((y_res == 1280 && input > 720) || (y_res == 1920 && input > 1080))
	    return -EINVAL;

	s2s_length = input;
	return count;
}

static DEVICE_ATTR(s2s_length, (0666),
	s2s_length_show, s2s_length_store);

static struct attribute *dt2w_attrs[] = {
    &dev_attr_doubletap2wake.attr,
    &dev_attr_sweep2sleep.attr,
    &dev_attr_dt2w_version.attr,
    &dev_attr_dt2w_duration.attr,
    &dev_attr_s2s_length.attr,
    NULL
};

static struct attribute_group dt2w_attr_group = {
    .attrs = dt2w_attrs
};

/*
 * INIT / EXIT stuff below here
 */
#ifdef ANDROID_TOUCH_DECLARED
extern struct kobject *android_touch_kobj;
#else
struct kobject *android_touch_kobj;
EXPORT_SYMBOL_GPL(android_touch_kobj);
#endif
static int __init doubletap2wake_init(void)
{
	int rc = 0;

	doubletap2wake_pwrdev = input_allocate_device();
	if (!doubletap2wake_pwrdev) {
		pr_err("Can't allocate suspend autotest power button\n");
		goto err_alloc_dev;
	}

	input_set_capability(doubletap2wake_pwrdev, EV_KEY, KEY_POWER);
	doubletap2wake_pwrdev->name = "dt2w_pwrkey";
	doubletap2wake_pwrdev->phys = "dt2w_pwrkey/input0";

	rc = input_register_device(doubletap2wake_pwrdev);
	if (rc) {
		pr_err("%s: input_register_device err=%d\n", __func__, rc);
		goto err_input_dev;
	}

	dt2w_input_wq = create_workqueue("dt2wiwq");
	if (!dt2w_input_wq) {
		pr_err("%s: Failed to create dt2wiwq workqueue\n", __func__);
		return -EFAULT;
	}

	memset(&tch, 0, sizeof(struct touch));
	memset(&tp, 0, sizeof(struct tap));
	INIT_WORK(&dt2w_input_work, dt2w_input_callback);
	INIT_WORK(&s2s_input_work, s2s_input_callback);
	wake_lock_init(&dt2w_wakelock, WAKE_LOCK_SUSPEND, "dt2w_wakelock");
	rc = input_register_handler(&dt2w_input_handler);
	if (rc)
		pr_err("%s: Failed to register dt2w_input_handler\n", __func__);

	if (!get_hw_config_int("synaptics/y_res", &y_res, NULL))
	    y_res = 0;
	else
	    switch (y_res) {
		case 1280:
		    s2s_length  = 180;
		    s2s_height  = 100;
		    s2s_feather = 50;
		    break;
		case 1920:
		    s2s_length  = 270;
		    s2s_height  = 145;
		    s2s_feather = 70;
		    break;
	    }

#ifndef CONFIG_HAS_EARLYSUSPEND
	dt2w_lcd_notif.notifier_call = lcd_notifier_callback;
	if (lcd_register_client(&dt2w_lcd_notif) != 0) {
		pr_err("%s: Failed to register lcd callback\n", __func__);
	}
#else
	register_early_suspend(&dt2w_early_suspend_handler);
#endif

#ifndef ANDROID_TOUCH_DECLARED
	android_touch_kobj = kobject_create_and_add("android_touch", NULL) ;
	if (android_touch_kobj == NULL) {
		pr_warn("%s: android_touch_kobj create_and_add failed\n", __func__);
	}
#endif
	rc = sysfs_create_group(android_touch_kobj, &dt2w_attr_group);
	if (rc) {
	    pr_warn("%s: sysfs_create_group failed\n", __func__);
	}

err_input_dev:
	input_free_device(doubletap2wake_pwrdev);
err_alloc_dev:
	pr_info(LOGTAG"%s done\n", __func__);

	return 0;
}

static void __exit doubletap2wake_exit(void)
{
#ifndef ANDROID_TOUCH_DECLARED
	kobject_del(android_touch_kobj);
#endif
#ifndef CONFIG_HAS_EARLYSUSPEND
	lcd_unregister_client(&dt2w_lcd_notif);
#else
	unregister_early_suspend(&dt2w_early_suspend_handler);
#endif
	input_unregister_handler(&dt2w_input_handler);
	destroy_workqueue(dt2w_input_wq);
	wake_lock_destroy(&dt2w_wakelock);
	input_unregister_device(doubletap2wake_pwrdev);
	input_free_device(doubletap2wake_pwrdev);
	return;
}

module_init(doubletap2wake_init);
module_exit(doubletap2wake_exit);

