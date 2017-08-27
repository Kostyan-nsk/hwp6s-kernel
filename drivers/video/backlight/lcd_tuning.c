/*
 * LCD Lowlevel Tuning Abstraction
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/lcd_tuning.h>
#include <linux/lcd_ioctl.h>

#define PRIMARY_LCD "pri_lcd"

#define to_lcd_mdevice(obj) container_of(obj, struct lcd_mdevice, mdev)

static long lmdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int r = 0;
	struct miscdevice *mdev = (struct miscdevice *)filp->private_data;
	struct lcd_mdevice *lmdev = container_of(mdev, struct lcd_mdevice, mdev);
	struct lcd_tuning_dev *ltd = 0;
	void __user *ptr = (void __user *)arg;

	union lcd_param
	{
		u32 dgamma;
		u32 cabc;
		/* END:   Added by huohua, 2012/02/14 */
		u32 ctValue[9];
	}par;

	ltd = &lmdev->ltd;
	
	if(!ltd->ops)
		return -1;

	mutex_lock(&lmdev->ops_lock);

	switch(cmd)
	{
		case LCD_TUNING_DGAMMA:
		{
			if(ltd->ops->set_gamma)
			{
				r = copy_from_user((void *)&par, ptr, sizeof(par.dgamma));
				if(r)
					goto out;
					
				if(par.dgamma)
				{
					if(ltd->props.default_gamma == GAMMA25)
						ltd->ops->set_gamma(ltd, GAMMA22);
					else if(ltd->props.default_gamma == GAMMA22)
						ltd->ops->set_gamma(ltd, GAMMA19);
					else
					{
						r = -EINVAL;
						pr_debug("%s: Unrecognized default gamma value %d\n", __func__, ltd->props.default_gamma);
						goto out;
					}
				}
				else
				{
					ltd->ops->set_gamma(ltd, ltd->props.default_gamma);
				}
			}
		}
		break;

		case LCD_TUNING_CABC:
		{
			if(ltd->ops->set_cabc)
			{
				r = copy_from_user((void *)&par, ptr, sizeof(par.cabc));
				if(r)
					goto out;

				if(par.cabc)
				{
					ltd->ops->set_cabc(ltd, CABC_VID);
				}
				else
				{
					ltd->ops->set_cabc(ltd, CABC_UI);
				}
			}
		}
		break;
		/* END:   Added by huohua, 2012/02/14 */
        case LCD_TUNING_DCPR:
		{

			if(ltd->ops->set_color_temperature)
			{
				r = copy_from_user((void *)&par, ptr, sizeof(par.ctValue));
			}
			if(r)
			{
				goto out;
			}

			ltd->ops->set_color_temperature(ltd, par.ctValue);
		}
		break;

		default:
		{
			r = -EINVAL;
		}
		break;
	}
out:
	mutex_unlock(&lmdev->ops_lock);
	return r;
}


static int lmdev_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t lmdev_write(struct file * file, const char __user * buf, size_t count, loff_t * pos)
{
	u32 ctValue[9] = {0};
	char kernelbuf[12] = {0};
	int i, ret;
	struct miscdevice *mdev = (struct miscdevice *)file->private_data;
	struct lcd_mdevice *lmdev = container_of(mdev, struct lcd_mdevice, mdev);
	struct lcd_tuning_dev *ltd = 0;

	ltd = &lmdev->ltd;

	if(!ltd->ops) {
		pr_err("%s: no ops\n", __func__);
		return -EINVAL;
	}

	if (count > 12) {
		pr_err("%s: write count is too large\n", __func__);
		return -EINVAL;
	}

	ret = copy_from_user(kernelbuf, buf, count);
	if(ret != 0) {
		pr_err("%s: copy to kernel buffer failed\n", __func__);
		return -EINVAL;
	}

	ret = sscanf(kernelbuf, "%u %u %u", &ctValue[0], &ctValue[4], &ctValue[8]);
	if (ret != 3) {
		pr_err("%s: buffer value is invalid\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i < 9; i += 4) {
		if (ctValue[i] > 256)
			ctValue[i] = 256;
		else if (ctValue[i] < 1)
			 ctValue[i] = 1;
	}

	if(ltd->ops->set_color_temperature)
		ltd->ops->set_color_temperature(ltd, ctValue);

	return count;
}


static struct file_operations mdev_fops = 
{
	.owner		= THIS_MODULE,
	.open		= lmdev_open,
	.unlocked_ioctl = lmdev_ioctl,
	.write		= lmdev_write,
};


static struct lcd_mdevice lmdev = 
{
	.ops_lock = __MUTEX_INITIALIZER(lmdev.ops_lock),
	.mdev = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = PRIMARY_LCD,
		.mode = 0600,
		.fops = &mdev_fops,
	},
};

struct lcd_tuning_dev *lcd_tuning_dev_register(struct lcd_properities *props, const struct lcd_tuning_ops *lcd_ops, void *devdata)
{
	if(props)
	{
		lmdev.ltd.props = *props;
	}
	else	/* Default to TFT with gamma2.5 */
	{
		lmdev.ltd.props.type = TFT;
		lmdev.ltd.props.default_gamma = GAMMA25;
	}

	lmdev.ltd.ops = lcd_ops;
	lmdev.ltd.data = devdata;

	return &lmdev.ltd;
}
EXPORT_SYMBOL(lcd_tuning_dev_register);

void lcd_tuning_dev_unregister(struct lcd_tuning_dev *ltd)
{
	if (!ltd)
		return;

	mutex_lock(&lmdev.ops_lock);
	lmdev.ltd.ops = NULL;
	mutex_unlock(&lmdev.ops_lock);
}
EXPORT_SYMBOL(lcd_tuning_dev_unregister);



static void __exit lcd_mdevice_exit(void)
{
	memset((void *)&lmdev, 0, sizeof(struct lcd_mdevice));
	
}

static int __init lcd_mdevice_init(void)
{
	int ret;

	ret = misc_register(&lmdev.mdev);

	if (ret) {
		return -1;
	}
	return 0;
}

/*
 * if this is compiled into the kernel, we need to ensure that the
 * class is registered before users of the class try to register lcd's
 */
module_init(lcd_mdevice_init);
module_exit(lcd_mdevice_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD Low-level Tuning Abstraction");
