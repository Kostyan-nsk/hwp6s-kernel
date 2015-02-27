/*
 * sonyimx219 sensor driver
 *
 *Author: 	Hanjunhui (hanjh_hit@163.com)
 *  Date:  	2014/01/23
 *  Version:	1.0
 *  History:	2014/01/23      Frist add driver for sonyimx219
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include <linux/videodev2.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fb.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <asm/div64.h>
#include <mach/hisi_mem.h>
#include "mach/hardware.h"
#include <mach/boardid.h>
#include <mach/gpio.h>
#include "sensor_common.h"
#include "sonyimx219.h"
#include <asm/bug.h>
#include <linux/device.h>

#define LOG_TAG "SONYIMX219"
//#define DEBUG_DEBUG 1
#include "cam_log.h"
#include <cam_util.h>
#include "drv_regulator_user.h"
#include "hsad/config_interface.h"
#include <mach/pmussi_drv.h>
#include "effect.h"

#define SONYIMX219_ZSL                        (0x00)

/*add for set awb gain begin*/
#define DIG_GAIN_H                             0x0158
#define DIG_GAIN_L                             0x0159

#define SONYIMX219_SLAVE_ADDRESS                0x20
#define SONYIMX219_CHIP_ID                      (0x0219)

#define SONYIMX219_MIRROR_FLIP		            0x0172

#define SONYIMX219_EXPOSURE_REG_H	            0x015A
#define SONYIMX219_EXPOSURE_REG_L	            0x015B

#define SONYIMX219_GAIN_REG                     0x0157

#define SONYIMX219_VTS_REG_H		            0x0160
#define SONYIMX219_VTS_REG_L		            0x0161

#define SONYIMX219_APERTURE_FACTOR              200 //F2.0
//#define SONYIMX219_EQUIVALENT_FOCUS	       0

#define SONYIMX219_AP_WRITEAE_MODE
#define SONYIMX219_MAX_ANALOG_GAIN	            170//256/(256-232)*16

#define SONYIMX219_I2C_RETRY_TIMES              3

static u8 sensor_module;

#ifdef IMX219_OTP
//#define IMX219_OTP_READ_TIME_PRINT
#define OTP_SLAVE_ADDR1       0xa0
#define OTP_SLAVE_ADDR2       0xa2

#define OTP_ID_REG 		      0x00
#define OTP_AWB_REG 	      0x05
#define OTP_LSC_1_REG 	      0x0b
#define OTP_LSC_2_REG	      0x00
#define OTP_VCM_REG 	      0x23
#define OTP_CHECKSUM_REG      0x27

#define SONYIMX219_OTP_ID_READ				(1 << 0)
#define SONYIMX219_OTP_VCM_READ				(1 << 1)
#define SONYIMX219_OTP_LSC_READ				(1 << 2)
#define SONYIMX219_OTP_AWB_READ  			(1 << 3)
#define SONYIMX219_OTP_CHECKSUM_READ        (1 << 4)
#define SONYIMX219_OTP_CHECKSUM_ERR			(1 << 5)
#define SONYIMX219_OTP_FAIL_FLAG			(1 << 6)

#define SONYIMX219_OTP_LSC_SIZE             280
#define SONYIMX219_OTP_LSC_QUARTER_SIZE     70
#define OTP_INSURANCE_OFFSET 0x30
static u16 sonyimx219_vcm_start = 0;
static u16 sonyimx219_vcm_end   = 0;
static u32 OTPSUMVAL            = 0;
static u8  sonyimx219_otp_lsc_param[SONYIMX219_OTP_LSC_SIZE] ;

static u8 sonyimx219_otp_flag = 0;

extern int  ispv1_read_sensor_byte_addr8(i2c_index_t index, u8 i2c_addr, u16 reg, u16 *val, i2c_length length);//add by zhoujie
static bool sonyimx219_get_otp_from_sensor(void);
static void sonyimx219_otp_get_vcm(u16 *vcm_start, u16 *vcm_end);
static bool sonyimx219_otp_set_lsc(void);
#endif

static camera_capability sonyimx219_cap[] = {
	{V4L2_CID_FLASH_MODE, THIS_FLASH},
	{V4L2_CID_FOCUS_MODE, THIS_FOCUS_MODE},
	{V4L2_CID_FOCAL_LENGTH, 293},//2.93mm
	{V4L2_CID_ZSL,		  SONYIMX219_ZSL},
};

static awb_gain_t flash_platform_awb[] = {
	{0xbd, 0x80, 0x80, 0xdc}, /*EDGE*/
};

static vcm_info_s vcm_dw9714_Liteon = {
    #include "vcm_dw9714_Liteon.h"
};

static effect_params effect_imx219_liteon = {
    #include "effect_imx219_liteon_H30L.h"
};

static framesize_s sonyimx219_framesizes[] = {
	/* 1600*1200@30fps*/
	{0, 0, 1600, 1200, 3600, 1481, 30, 30, 0x1bc, 0x172, 0x100, VIEW_FULL, RESOLUTION_4_3, false, false,{ (sensor_reg_t *)sonyimx219_framesize_1600x1200, ARRAY_SIZE(sonyimx219_framesize_1600x1200)} },	

	/* 1920*1088@30fps*/
	{0, 0, 1920, 1088, 3600, 1481, 30, 30, 0x1bc, 0x172, 0x1c0, VIEW_CROP, RESOLUTION_16_9, false, false,{ (sensor_reg_t *)sonyimx219_framesize_1080p, ARRAY_SIZE(sonyimx219_framesize_1080p)} },	

    /* 3264*2448@30fps for ZSL only*/
    {0, 0, 3264, 2448, 3600, 2593, 30, 30, 0x309, 0x288, 0x100, VIEW_FULL, RESOLUTION_4_3, false, true, { (sensor_reg_t *)sonyimx219_framesize_ZSL_full, ARRAY_SIZE(sonyimx219_framesize_ZSL_full)} },
	
	/* 3264*2448@15fps*/
	{0, 0, 3264, 2448, 3600, 3630, 15, 15, 0x220, 0x1c5, 0x139, VIEW_FULL, RESOLUTION_4_3, false, false,{ (sensor_reg_t *)sonyimx219_framesize_full, ARRAY_SIZE(sonyimx219_framesize_full)} },

    /* 3280*1960@30fps */
    {0, 0, 3280, 1960, 3600, 2593, 30, 30, 0x309, 0x288, 0x1c0, VIEW_FULL, RESOLUTION_16_9, false, false, { (sensor_reg_t *)sonyimx219_framesize_3280x1960, ARRAY_SIZE(sonyimx219_framesize_3280x1960)} },	

    /* 3280*2464@30fps for ZSL only*/
    {0, 0, 3280, 2464, 3600, 2593, 30, 30, 0x309, 0x288, 0x100, VIEW_FULL, RESOLUTION_4_3, false, true, { (sensor_reg_t *)sonyimx219_framesize_3280_2464, ARRAY_SIZE(sonyimx219_framesize_3280_2464)} }	
};

static camera_sensor sonyimx219_sensor;

/*
 **************************************************************************
 * FunctionName: sonyimx219_read_reg;
 * Description : read sonyimx219 reg by i2c;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int sonyimx219_read_reg(u16 reg, u8 *val)
{
	return k3_ispio_read_reg(sonyimx219_sensor.i2c_config.index,
			 sonyimx219_sensor.i2c_config.addr, reg, (u16*)val, sonyimx219_sensor.i2c_config.val_bits,sonyimx219_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_write_reg;
 * Description : write sonyimx219 reg by i2c;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int sonyimx219_write_reg(u16 reg, u8 val, u8 mask)
{
	return k3_ispio_write_reg(sonyimx219_sensor.i2c_config.index,
			sonyimx219_sensor.i2c_config.addr, reg, val, sonyimx219_sensor.i2c_config.val_bits, mask,sonyimx219_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_write_seq;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int sonyimx219_write_seq(const struct _sensor_reg_t *seq, u32 size, u8 mask)
{
	print_debug("Enter %s, seq[%#x], size=%d", __func__, (int)seq, size);
	return k3_ispio_write_seq(sonyimx219_sensor.i2c_config.index,
			sonyimx219_sensor.i2c_config.addr, seq, size, sonyimx219_sensor.i2c_config.val_bits, mask,sonyimx219_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_write_isp_seq;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static void sonyimx219_write_isp_seq(const struct isp_reg_t *seq, u32 size)
{
	print_debug("Enter %s, seq[%#x], size=%d", __func__, (int)seq, size);
	k3_ispio_write_isp_seq(seq, size);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_enum_frame_intervals;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_enum_frame_intervals(struct v4l2_frmivalenum *fi)
{
    if(!fi) {
        return -EINVAL;
    }

	print_debug("enter %s", __func__);
	if (fi->index >= CAMERA_MAX_FRAMERATE) {
		return -EINVAL;
	}

	fi->type = V4L2_FRMIVAL_TYPE_DISCRETE;
	fi->discrete.numerator = 1;
	fi->discrete.denominator = (fi->index + 1);
	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_get_format;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_get_format(struct v4l2_fmtdesc *fmt)
{
	if (fmt->type == V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		fmt->pixelformat = sonyimx219_sensor.fmt[STATE_PREVIEW];
	} else {
		fmt->pixelformat = sonyimx219_sensor.fmt[STATE_CAPTURE];
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_get_capability;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_get_capability(u32 id, u32 *value)
{
	int i;
	for (i = 0; i < sizeof(sonyimx219_cap) / sizeof(sonyimx219_cap[0]); ++i) {
		if (id == sonyimx219_cap[i].id) {
			*value = sonyimx219_cap[i].value;
			break;
		}
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_enum_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_enum_framesizes(struct v4l2_frmsizeenum *framesizes)
{
	u32 max_index = ARRAY_SIZE(camera_framesizes) - 1;
	u32 this_max_index = ARRAY_SIZE(sonyimx219_framesizes) - 1;

    if(!framesizes) {
        return -EINVAL;
    }

	print_debug("enter %s; ", __func__);

	if (framesizes->index > max_index) {
		print_error("framesizes->index = %d error", framesizes->index);
		return -EINVAL;
	}

	if ((camera_framesizes[framesizes->index].width > sonyimx219_framesizes[this_max_index].width)
		|| (camera_framesizes[framesizes->index].height > sonyimx219_framesizes[this_max_index].height)) {
		print_error("framesizes->index = %d error", framesizes->index);
		return -EINVAL;
	}

	framesizes->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	framesizes->discrete.width = sonyimx219_framesizes[this_max_index].width;
	framesizes->discrete.height = sonyimx219_framesizes[this_max_index].height;

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_try_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_try_framesizes(struct v4l2_frmsizeenum *framesizes)
{
	int max_index = ARRAY_SIZE(sonyimx219_framesizes) - 1;

    if(!framesizes) {
        return -EINVAL;
    }

	print_debug("Enter Function:%s  ", __func__);

	if ((framesizes->discrete.width <= sonyimx219_framesizes[max_index].width)
	    && (framesizes->discrete.height <= sonyimx219_framesizes[max_index].height)) {
		print_debug("===========width = %d", framesizes->discrete.width);
		print_debug("===========height = %d", framesizes->discrete.height);
		return 0;
	}

	print_error("frame size too large, [%d,%d]",
    framesizes->discrete.width, framesizes->discrete.height);
	return -EINVAL;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_set_framesizes;
 * Description : NA;
 * Input       : flag: if 1, set framesize to sensor,
 *					   if 0, only store framesize to camera_interface;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_set_framesizes(camera_state state,
				 struct v4l2_frmsize_discrete *fs, int flag, camera_setting_view_type view_type,bool zsl_preview)
{
	int  i = 0;
	bool match = false;
	int  size = 0;
    if(!fs) {
        return -EINVAL;
    }

	print_info("Enter Function:%s State(%d), flag=%d, width=%d, height=%d",
		   __func__, state, flag, fs->width, fs->height);

	/*sonyimx219 not support scale down, so we use full view sequence 3280x1960*/
	if(fs->width == 1920){
		fs->width = 3280;
		fs->height = 1960;
	}

	print_info("Enter Function:%s resolution_type(%d), view_type=%d, width=%d, height=%d",
		   __func__, camera_get_resolution_type(fs->width, fs->height), view_type, fs->width, fs->height);

	size = ARRAY_SIZE(sonyimx219_framesizes);
	if (VIEW_FULL == view_type) {
		for (i = 0; i < size; i++) {
		    /*zsl only=true means is only for zsl, otherwise can used by zsl and not zsl*/
		    if (zsl_preview == false && sonyimx219_framesizes[i].zsl_only) {
		        continue;
		    }
			if ((sonyimx219_framesizes[i].width >= fs->width)
			    && (sonyimx219_framesizes[i].height >= fs->height)
			    && (VIEW_FULL == sonyimx219_framesizes[i].view_type)
			    && (camera_get_resolution_type(fs->width, fs->height)
			    <= sonyimx219_framesizes[i].resolution_type)) {
    				fs->width = sonyimx219_framesizes[i].width;
    				fs->height = sonyimx219_framesizes[i].height;
    				match = true;
					print_info("Enter Function:%s [%d] settings is sonyimx219_resolution_type(%d), sonyimx219_view_type=%d, sonyimx219_width=%d, ov8858_height=%d,ov8858_fps=%d fps",
				   		__func__, i,sonyimx219_framesizes[i].resolution_type, sonyimx219_framesizes[i].view_type, sonyimx219_framesizes[i].width, sonyimx219_framesizes[i].height,sonyimx219_framesizes[i].fps);
		    		break;
			}
			print_info("Enter Function:%s [%d] settings is sonyimx219_resolution_type(%d), sonyimx219_view_type=%d, sonyimx219_width=%d, ov8858_height=%d,ov8858_fps=%d fps",
			   __func__, i,sonyimx219_framesizes[i].resolution_type, sonyimx219_framesizes[i].view_type, sonyimx219_framesizes[i].width, sonyimx219_framesizes[i].height,sonyimx219_framesizes[i].fps);
	
		}
	}

	if (false == match) {
		for (i = 0; i < size; i++) {
		    /*zsl only=true means is only for zsl, otherwise can used by zsl and not zsl*/
		    if (zsl_preview == false && sonyimx219_framesizes[i].zsl_only) {
		        continue;
		    }

			if ((sonyimx219_framesizes[i].width >= fs->width)
				    && (sonyimx219_framesizes[i].height >= fs->height)
				    && (camera_get_resolution_type(fs->width, fs->height)
    				    <= sonyimx219_framesizes[i].resolution_type)
				    && (VIEW_HDR_MOVIE != sonyimx219_framesizes[i].view_type)){
					fs->width = sonyimx219_framesizes[i].width;
					fs->height = sonyimx219_framesizes[i].height;
					break;
			}
		}
	}

	if (i >= size) {
		print_error("request resolution larger than sensor's max resolution");
		return -EINVAL;
	}
	print_info("Enter Function:%s match=%d,final setting is sonyimx219_resolution_type(%d), sonyimx219_view_type=%d, sonyimx219_width=%d, ov8858_height=%d, ov8858_fps=%d fps",
		   __func__, match,sonyimx219_framesizes[i].resolution_type, sonyimx219_framesizes[i].view_type, sonyimx219_framesizes[i].width, sonyimx219_framesizes[i].height,sonyimx219_framesizes[i].fps);

	if (state == STATE_PREVIEW) {
		sonyimx219_sensor.preview_frmsize_index = i;
	} else {
		sonyimx219_sensor.capture_frmsize_index = i;
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_get_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_get_framesizes(camera_state state,
				     struct v4l2_frmsize_discrete *fs)
{
	int frmsize_index = 0;

    if(!fs) {
        return -EINVAL;
    }

	if (state == STATE_PREVIEW) {
		frmsize_index = sonyimx219_sensor.preview_frmsize_index;
	} else if (state == STATE_CAPTURE) {
		frmsize_index = sonyimx219_sensor.capture_frmsize_index;
	} else {
		return -EINVAL;
	}
	fs->width = sonyimx219_framesizes[frmsize_index].width;
	fs->height = sonyimx219_framesizes[frmsize_index].height;

	print_info("Enter funtion %s : fs->width =%d, fs->height = %d !!!\n",__func__, fs->width,fs->height);		

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_init_isp_reg;
 * Description : load initial seq for sensor init;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_init_isp_reg(void)
{
	int size = CAMERA_MAX_SETTING_SIZE;
	camera_sensor *sensor = &sonyimx219_sensor;
	   	
	sonyimx219_write_isp_seq(sensor->effect->isp_settings, size);

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_support_awb_otp;
 * Description : check sensor support awb otp or not;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_support_awb_otp(void)
{
#ifdef IMX219_OTP 
	if((sonyimx219_otp_flag & SONYIMX219_OTP_AWB_READ) == SONYIMX219_OTP_AWB_READ && (sonyimx219_otp_flag & SONYIMX219_OTP_FAIL_FLAG) != SONYIMX219_OTP_FAIL_FLAG)
    	return AWB_OTP_SUPPORT;
	else
		return AWB_OTP_NOT_SUPPORT;
#else
    	return AWB_OTP_NOT_SUPPORT;
#endif
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_init_reg;
 * Description : download initial seq for sensor init;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_init_reg(void)
{
	int size = 0;
		
	print_debug("Enter Function:%s , initsize=%d",__func__, sizeof(sonyimx219_init_regs));

	if (0 != k3_ispio_init_csi(sonyimx219_sensor.mipi_index,
			sonyimx219_sensor.mipi_lane_count, sonyimx219_sensor.lane_clk)) {
		return -EFAULT;
	}

	size = ARRAY_SIZE(sonyimx219_init_regs);
	if (0 != sonyimx219_write_seq(sonyimx219_init_regs, size, 0x00)) {
		print_error("line %d, fail to init sonyimx219 sensor",__LINE__);
		return -EFAULT;
	}

       /*
	if(E_CAMERA_SENSOR_FLIP_TYPE_H_V == get_primary_sensor_flip_type()) {//this is a valid branch
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, 0x00, 0x00);
		print_info("%s:change flip and mirror",__func__);
	}else if(E_CAMERA_SENSOR_FLIP_TYPE_H == get_primary_sensor_flip_type()){//both the branch and the below branch need to tuned with module IC rotation
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, 0x01, 0x00);
		print_info("%s:change mirror",__func__);
	}else if(E_CAMERA_SENSOR_FLIP_TYPE_V == get_primary_sensor_flip_type()){
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, 0x02, 0x00);
		print_info("%s:change flip",__func__);		
	}
	*/

#ifdef IMX219_OTP
	if((sonyimx219_otp_flag & SONYIMX219_OTP_LSC_READ) == SONYIMX219_OTP_LSC_READ && (sonyimx219_otp_flag & SONYIMX219_OTP_FAIL_FLAG) != SONYIMX219_OTP_FAIL_FLAG){
		sonyimx219_otp_set_lsc();
	}
#endif

	return 0;
}

static int sonyimx219_set_hflip(int flip)
{
	print_debug("enter %s flip=%d", __func__, flip);
	sonyimx219_sensor.hflip = flip;
	return 0;
}
static int sonyimx219_get_hflip(void)
{
	print_debug("enter %s", __func__);

	return sonyimx219_sensor.hflip;
}
static int sonyimx219_set_vflip(int flip)
{
	print_debug("enter %s flip=%d", __func__, flip);

	sonyimx219_sensor.vflip = flip;

	return 0;
}
static int sonyimx219_get_vflip(void)
{
	print_debug("enter %s", __func__);
	return sonyimx219_sensor.vflip;
}

static int sonyimx219_update_flip(u16 width, u16 height)
{
	u8 new_flip = ((sonyimx219_sensor.vflip << 1) | sonyimx219_sensor.hflip);
	
	print_debug("Enter %s  ", __func__);
	
	k3_ispio_update_flip(( sonyimx219_sensor.old_flip^new_flip) & 0x03, width, height, PIXEL_ORDER_CHANGED);

	sonyimx219_sensor.old_flip = new_flip;

	if(E_CAMERA_SENSOR_FLIP_TYPE_NONE == get_primary_sensor_flip_type()) {//this is a valid branch
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, sonyimx219_sensor.vflip ? 0x00 : 0x02, ~0x02);
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, sonyimx219_sensor.hflip ? 0x00 : 0x01, ~0x01);		
	}else if(E_CAMERA_SENSOR_FLIP_TYPE_H == get_primary_sensor_flip_type()){//both the branch and the below branch need to tuned with module IC rotation
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, sonyimx219_sensor.hflip ? 0x00 : 0x01, ~0x01);
	}else if(E_CAMERA_SENSOR_FLIP_TYPE_V == get_primary_sensor_flip_type()){
		sonyimx219_write_reg(SONYIMX219_MIRROR_FLIP, sonyimx219_sensor.vflip ? 0x00 : 0x02, ~0x02);
	}

	return 0;
}
/*
 **************************************************************************
 * FunctionName: sonyimx219_framesize_switch;
 * Description : switch frame size, used by preview and capture
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_framesize_switch(camera_state state)
{
	u8 next_frmsize_index;

	if (state == STATE_PREVIEW)
		next_frmsize_index = sonyimx219_sensor.preview_frmsize_index;
	else
		next_frmsize_index = sonyimx219_sensor.capture_frmsize_index;

	print_debug("Enter Function:%s frm index=%d", __func__, next_frmsize_index);
	
	if (next_frmsize_index >= ARRAY_SIZE(sonyimx219_framesizes)){
		print_error("Unsupport sensor setting index: %d",next_frmsize_index);
		return -ETIME;
	}

	if (0 != sonyimx219_write_seq(sonyimx219_sensor.frmsize_list[next_frmsize_index].sensor_setting.setting
		,sonyimx219_sensor.frmsize_list[next_frmsize_index].sensor_setting.seq_size, 0x00)) {
		print_error("fail to init sonyimx219 sensor");
		return -ETIME;
	}

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_stream_on;
 * Description : download preview seq for sensor preview;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_stream_on(camera_state state)
{
	print_debug("Enter Function:%s ", __func__);	    	
	
	return sonyimx219_framesize_switch(state);
}

/*  **************************************************************************
* FunctionName: sonyimx219_check_sensor;
* Description : NA;
* Input       : NA;
* Output      : NA;
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
static int sonyimx219_check_sensor(void)
{
	u8  idl = 0;
	u8  idh = 0;
	u16 id = 0;
	int pin_id = -1;
    int retry = 0;
    camera_sensor *sensor = &sonyimx219_sensor;
    bool retVal = false;

    for(retry = 0; retry < SONYIMX219_I2C_RETRY_TIMES; retry++) {
    	sonyimx219_read_reg(0x0000, &idh);
    	sonyimx219_read_reg(0x0001, &idl);
    	id = ((idh << 8) | idl);
        print_info("sonyimx219 product id:0x%x, retrytimes:%d", id, retry);
        if(SONYIMX219_CHIP_ID == id) {
            break;
        }
        udelay(100);
    }
		
	if (SONYIMX219_CHIP_ID != id) {
		print_error("Invalid product id ,Could not load sensor sonyimx219\n");
		return -ENODEV;
	}		
	
	pin_id = gpio_get_value(GPIO_18_3);//GPIO_18_3=DGND		
	if(pin_id != 0) {
		print_error("sonyimx219_check_sensor fail to get gpio value!!! set pin_id to 0 by default MODULE_LITEON !\n");
		return -ENODEV;		
	}

	sonyimx219_sensor.vcm = &vcm_dw9714_Liteon;
	sensor->effect = &effect_imx219_liteon;  
	snprintf(sonyimx219_sensor.info.name, sizeof(sonyimx219_sensor.info.name),"sonyimx219_liteon");

#ifdef IMX219_OTP
	retVal = sonyimx219_get_otp_from_sensor();
    if(true != retVal){
		print_error("%s~~~~~~sonyimx219_get_otp_from_sensor() return false for OTP FAIL\n",__func__);		
	}
	sonyimx219_sensor.vcm->get_vcm_otp = sonyimx219_otp_get_vcm;
#endif

	return 0;
}

/****************************************************************************
* FunctionName: sonyimx219_check_sensor;
* Description : NA;
* Input       : NA;
* Output      : NA;
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
int sonyimx219_power(camera_power_state power)
{
	int ret = 0;

	print_debug("Enter Function:%s\n ", __func__);

	if(0 == get_sensor_iopower()) {
		if (power == POWER_ON) {	
			k3_ispldo_power_sensor(power, M_CAMERA_ANALOG_VDD);
			//ret = camera_power_core_ldo(power);
			k3_ispldo_power_sensor(power, M_CAMERA_CORE_VDD);
			udelay(200);
			k3_ispldo_power_sensor(power, M_CAMERA_IO_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_VCM_VDD);
			udelay(1);
	        //k3_ispldo_power_sensor(power, "sec-cameralog-vcc");
			k3_ispldo_power_sensor(power, S_CAMERA_ANALOG_VDD);
			k3_ispldo_power_sensor(power, S_CAMERA_CORE_VDD);
	        k3_ispldo_power_sensor(power, S_BACKUP_CAMERA_CORE_VDD);
			//k3_ispldo_power_sensor(power, S_CAMERA_IO_VDD);

			k3_ispio_ioconfig(&sonyimx219_sensor, power);
			k3_ispgpio_power_sensor(&sonyimx219_sensor, power);
			msleep(3);
		} else {		
			k3_ispio_deinit_csi(sonyimx219_sensor.mipi_index);
			k3_ispgpio_power_sensor(&sonyimx219_sensor, power);
			k3_ispio_ioconfig(&sonyimx219_sensor, power);

			k3_ispldo_power_sensor(power, S_BACKUP_CAMERA_CORE_VDD);
			k3_ispldo_power_sensor(power, S_CAMERA_CORE_VDD);
	        k3_ispldo_power_sensor(power, S_CAMERA_ANALOG_VDD);
			//k3_ispldo_power_sensor(power, S_CAMERA_IO_VDD);

	        //k3_ispldo_power_sensor(power, "sec-cameralog-vcc");
			k3_ispldo_power_sensor(power, M_CAMERA_VCM_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_CORE_VDD);
			//camera_power_core_ldo(power);
			udelay(200);
			k3_ispldo_power_sensor(power, M_CAMERA_ANALOG_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_IO_VDD);

#ifdef DUMP_SCCTRL_ISP_REG
            dump_scctrl_isp_reg();
#endif
		}
	} else {
		if (power == POWER_ON) {	
			k3_ispldo_power_sensor(power, M_CAMERA_ANALOG_VDD);
			//ret = camera_power_core_ldo(power);
			k3_ispldo_power_sensor(power, M_CAMERA_CORE_VDD);
			udelay(200);
			k3_ispldo_power_sensor(power, M_CAMERA_IO_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_VCM_VDD);
			udelay(1);

			k3_ispio_ioconfig(&sonyimx219_sensor, power);
			k3_ispgpio_power_sensor(&sonyimx219_sensor, power);
			msleep(3);
		} else {			
			k3_ispio_deinit_csi(sonyimx219_sensor.mipi_index);
			k3_ispgpio_power_sensor(&sonyimx219_sensor, power);
			k3_ispio_ioconfig(&sonyimx219_sensor, power);

	        //k3_ispldo_power_sensor(power, "sec-cameralog-vcc");
			k3_ispldo_power_sensor(power, M_CAMERA_VCM_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_CORE_VDD);
			//camera_power_core_ldo(power);
			udelay(200);
			k3_ispldo_power_sensor(power, M_CAMERA_ANALOG_VDD);
			k3_ispldo_power_sensor(power, M_CAMERA_IO_VDD);

#ifdef DUMP_SCCTRL_ISP_REG
            dump_scctrl_isp_reg();
#endif
		}
	}

	return ret;
}

static int sonyimx219_get_sensor_aperture(void)
{
	return SONYIMX219_APERTURE_FACTOR;
}

/*
 * Here gain is in unit 1/16 of sensor gain,
 * y36721 todo, temporarily if sensor gain=0x10, ISO is 100
 * in fact we need calibrate an ISO-ET-gain table.
 */
u32 sonyimx219_gain_to_iso(int gain)
{
	return (gain * 100) / 0x10;
}

u32 sonyimx219_iso_to_gain(int iso)
{
    return (iso * 0x10) / 100;
}

void sonyimx219_set_gain(u32 gain)
{
	u32	  tmp_digital_gain = 0;
	u8    digital_ratio = 0;
	u8    digital_h = 0;
	u8    digital_l = 0;
	u32   analog_gain = 0;

	if (gain == 0)
		goto out;
	
    //isp2.2 gain = register_value/0x10
	if(gain > SONYIMX219_MAX_ANALOG_GAIN)
	{
        /*tmp_digital_gain*256 is keep the decimal part*/
		tmp_digital_gain = (gain*256) >> 4;
		tmp_digital_gain = (tmp_digital_gain*16)/SONYIMX219_MAX_ANALOG_GAIN;

		digital_ratio = gain/SONYIMX219_MAX_ANALOG_GAIN;
		digital_h = digital_ratio;
		digital_l = tmp_digital_gain - digital_ratio*256;

		analog_gain = SONYIMX219_MAX_ANALOG_GAIN;
	}
	else
	{
		analog_gain = gain;
		digital_h = 1;
		digital_l = 0;
	}

	analog_gain = 256 - (256*16)/analog_gain;
	sonyimx219_write_reg(SONYIMX219_GAIN_REG, analog_gain & 0xff, 0x00);
	
	sonyimx219_write_reg(DIG_GAIN_H, digital_h & 0xff, 0x00);
	sonyimx219_write_reg(DIG_GAIN_L, digital_l & 0xff, 0x00);

out:
	return;
}

void sonyimx219_set_exposure(u32 exposure)
{
    /*adapter isp2.2 register value*/
	exposure >>= 4; //isp2.2 exposure = register_value/0x10	
	sonyimx219_write_reg(SONYIMX219_EXPOSURE_REG_H, (exposure >> 8) & 0xff, 0x00);
	sonyimx219_write_reg(SONYIMX219_EXPOSURE_REG_L, exposure & 0xff, 0x00);

	return;
}

void sonyimx219_set_exposure_gain(u32 exposure, u32 gain)
{
	u32	  tmp_digital_gain = 0;
	u8    digital_ratio = 0;
	u8    digital_h = 0;
	u8    digital_l = 0;
	u32   analog_gain = 0;
	//sonyimx219_set_dpc_function(gain);//DPC effect of sonyimx219 sensor open by default

    /*adapter isp2.2 register value*/
	exposure >>= 4; //isp2.2 exposure = register_value/0x10	
	sonyimx219_write_reg(SONYIMX219_EXPOSURE_REG_H, (exposure >> 8) & 0xff, 0x00);
	sonyimx219_write_reg(SONYIMX219_EXPOSURE_REG_L, exposure & 0xff, 0x00);

	if (gain == 0)
		goto out;

    //isp2.2 gain = register_value/0x10
	if(gain > SONYIMX219_MAX_ANALOG_GAIN)
	{
        /*tmp_digital_gain*256 is keep the decimal part*/
		tmp_digital_gain = (gain*256) >> 4;
		tmp_digital_gain = (tmp_digital_gain*16)/SONYIMX219_MAX_ANALOG_GAIN;

		digital_ratio = gain/SONYIMX219_MAX_ANALOG_GAIN;
		digital_h = digital_ratio;
		digital_l = tmp_digital_gain - digital_ratio*256;

		analog_gain = SONYIMX219_MAX_ANALOG_GAIN;
	}
	else
	{
		analog_gain = gain;
		digital_h = 1;
		digital_l = 0;
	}

	analog_gain = 256 - (256*16)/analog_gain;
	sonyimx219_write_reg(SONYIMX219_GAIN_REG, analog_gain & 0xff, 0x00);
	
	sonyimx219_write_reg(DIG_GAIN_H, digital_h & 0xff, 0x00);
	sonyimx219_write_reg(DIG_GAIN_L, digital_l & 0xff, 0x00);
	
out:
	return;
}

void sonyimx219_set_vts(u16 vts)
{
	print_debug("Enter %s  ", __func__);
	sonyimx219_write_reg(SONYIMX219_VTS_REG_H, (vts >> 8) & 0xff, 0x00);
	sonyimx219_write_reg(SONYIMX219_VTS_REG_L, vts & 0xff, 0x00);
}

u32 sonyimx219_get_vts_reg_addr(void)
{
	return SONYIMX219_VTS_REG_H;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_get_gain;
 * Description :get gain from sensor
 * Input       	 NA;
 * Output      	 gain value from sensor
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
u32 sonyimx219_get_gain(void)
{
	u8  analog_gain = 0;
	u32 gain = 0;
	sonyimx219_read_reg(SONYIMX219_GAIN_REG, &analog_gain);
	gain = analog_gain;
	if(_IS_DEBUG_AE) {
		print_info("%s-0x0205=%x",__func__,analog_gain);
	}
	return gain;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_get_exposure;
 * Description :get exposure from sensor
 * Input       	 NA;
 * Output      	 exposure value from sensor
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
u32  sonyimx219_get_exposure(void)
{
	u32 expo = 0;
	u8  expo_h = 0;
	u8  expo_l = 0;
	
	sonyimx219_read_reg(SONYIMX219_EXPOSURE_REG_H, &expo_h);
	sonyimx219_read_reg(SONYIMX219_EXPOSURE_REG_L, &expo_l);

	if(_IS_DEBUG_AE) {
		print_info("sonyimx219_get_exposure-0x018A=%x,0x018B=%x",expo_h,expo_l);
	}
	expo = expo_h &0xff;
	expo = expo << 8 | expo_l;
	return expo;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_reset;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_reset(camera_power_state power_state)
{
	print_debug("%s  \n", __func__);
	sonyimx219_sensor.old_flip = 0;
	sonyimx219_sensor.sensor_hdr_movie.hdrInfo.hdr_on = HDR_MOVIE_OFF;

	if (POWER_ON == power_state) {
		k3_isp_io_enable_mclk(MCLK_ENABLE, sonyimx219_sensor.sensor_index);
		udelay(100);
		k3_ispgpio_reset_sensor(sonyimx219_sensor.sensor_index, power_state,
			      sonyimx219_sensor.power_conf.reset_valid);
		udelay(500);
	} else {
		k3_ispgpio_reset_sensor(sonyimx219_sensor.sensor_index, power_state,
			      sonyimx219_sensor.power_conf.reset_valid);
		udelay(10);
		k3_isp_io_enable_mclk(MCLK_DISABLE, sonyimx219_sensor.sensor_index);
	}

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_init;
 * Description : sonyimx219 init function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : Error code indicating success or failure;
 * Other       : NA;
 **************************************************************************
*/
static int sonyimx219_init(void)
{
	static bool sonyimx219_check = false;
    print_debug("%s  ", __func__);

	if (false == sonyimx219_check) {
        if (check_suspensory_camera("SONYIMX219") != 1) {
            return -ENODEV;     
        }       
        sonyimx219_check = true;
    }

	if (!camera_timing_is_match(0)) {
		print_error("%s: sensor timing don't match.\n", __func__);
		return -ENODEV;
	}

	if (sonyimx219_sensor.owner && !try_module_get(sonyimx219_sensor.owner)) {
		print_error("%s: try_module_get fail", __func__);
		return -ENOENT;
	}

    /* 1. M_CAMERA_IO_VDD and S_CAMERA_IO_VDD have the same power.
       2. Only power on one sensor cause leakage, so pri-sensor and sec-sensor
          power on at the same time. Then pull down sec-sensor's PWDN/RESET GPIO for low power.
    */
	if(E_CAMERA_SENSOR_FLIP_TYPE_H_V == get_primary_sensor_flip_type())
		sonyimx219_sensor.sensor_rgb_type = SENSOR_RGGB;
	else
		sonyimx219_sensor.sensor_rgb_type = SENSOR_BGGR;
	sonyimx219_sensor.old_flip = 0;
	if(0 == get_sensor_iopower()) {	
		k3_ispio_power_init(M_CAMERA_ANALOG_VDD, LDO_VOLTAGE_28V, LDO_VOLTAGE_28V);	/*analog 2.85V - pri camera*/
		k3_ispio_power_init(M_CAMERA_IO_VDD, LDO_VOLTAGE_18V, LDO_VOLTAGE_18V);		/*IO 1.8V - pri camera*/
		k3_ispio_power_init(M_CAMERA_VCM_VDD, LDO_VOLTAGE_28V, LDO_VOLTAGE_28V);	/*AF 2.85V - pri camera*/
		k3_ispio_power_init(M_CAMERA_CORE_VDD, LDO_VOLTAGE_12V, LDO_VOLTAGE_12V);	/*core 1.2V - pri camera*/
		k3_ispio_power_init(S_CAMERA_ANALOG_VDD, LDO_VOLTAGE_28V, LDO_VOLTAGE_28V);	/*analog 2.85V - sec camera*/
		k3_ispio_power_init(S_CAMERA_CORE_VDD, LDO_VOLTAGE_18V, LDO_VOLTAGE_18V);	/*core 1.8V - sec camera*/
		k3_ispio_power_init(S_BACKUP_CAMERA_CORE_VDD, LDO_VOLTAGE_18V, LDO_VOLTAGE_18V);
		//k3_ispio_power_init(S_CAMERA_IO_VDD, LDO_VOLTAGE_18V, LDO_VOLTAGE_18V);

	} else {		
		k3_ispio_power_init(M_CAMERA_ANALOG_VDD, LDO_VOLTAGE_28V, LDO_VOLTAGE_28V);	/*analog 2.85V - pri camera*/
		k3_ispio_power_init(M_CAMERA_IO_VDD, LDO_VOLTAGE_18V, LDO_VOLTAGE_18V);		/*IO 1.8V - pri camera*/
		k3_ispio_power_init(M_CAMERA_VCM_VDD, LDO_VOLTAGE_28V, LDO_VOLTAGE_28V);	/*AF 2.85V - pri camera*/
		k3_ispio_power_init(M_CAMERA_CORE_VDD, LDO_VOLTAGE_12V, LDO_VOLTAGE_12V);	/*core 1.2V - pri camera*/
	}

	return 0;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_exit;
 * Description : sonyimx219 exit function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void sonyimx219_exit(void)
{
	print_debug("enter %s", __func__);

	k3_ispio_power_deinit();

	if (sonyimx219_sensor.owner) {
		module_put(sonyimx219_sensor.owner);
	}
	print_debug("exit %s", __func__);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_shut_down;
 * Description : sonyimx219 shut down function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void sonyimx219_shut_down(void)
{
	print_debug("enter %s", __func__);
	k3_ispgpio_power_sensor(&sonyimx219_sensor, POWER_OFF);
}

void sonyimx219_get_flash_awb(flash_platform_t type, awb_gain_t *flash_awb)
{
	*flash_awb = flash_platform_awb[0];
	print_info("sonyimx219_get_flash_awb: type 0x%x,", type);
}

#ifdef IMX219_OTP
int sonyimx219_read_otp(u8 i2c_addr,u16 reg,u8 *buf,u16 count)
{
	u16 i   = 0;
	int ret = 0;
	u16 val = 0;
	
	for(;i<count;i++) {
		ret =ispv1_read_sensor_byte_addr8(sonyimx219_sensor.i2c_config.index,i2c_addr,reg+i,&val,sonyimx219_sensor.i2c_config.val_bits);
		if(ret !=0)
			print_error("%s fail to read otp with error code %d\n", __func__,ret);

		buf[i] = (val&0xff);
		OTPSUMVAL += buf[i];

		//print_info("######OTPPrint~~~%s::I2CAddr=0x%x, RegAddr=0x%x, RegVal=0x%x ######\n", __func__, i2c_addr, reg+i, buf[i]);		
	}
	
	return 0;
}

static bool sonyimx219_otp_read_id(void)
{
	u8 buf[5] = {0};
	u8 vendor_id = 0;

	print_debug("enter %s", __func__);

	sonyimx219_read_otp(OTP_SLAVE_ADDR1,OTP_ID_REG,buf,5);

	print_info("module info year 20%02d month %d day %d, SNO. 0x%x  vendor id&version 0x%x\n", buf[0],buf[1],buf[2],buf[3],buf[4]);

	vendor_id = buf[4]>>4;
	if(vendor_id == 0x03 && buf[3] == 0x8F) { //Liteon 0x03 & huaweiModuleCode is 23060143(0n143 = 0x8F)
		sonyimx219_otp_flag |= SONYIMX219_OTP_ID_READ;
		return true;		
	} else {
		print_error("%s OTP data is worng for with wrong vender id!!!\n",__func__);
		return false;
	}
}


static bool sonyimx219_otp_read_awb(void)
{
	u8  buf[6] = {0};
	u16 awbRG,awbBG,awbRB;

	sonyimx219_read_otp(OTP_SLAVE_ADDR1,OTP_AWB_REG,buf,6);	

	//print_info("%s OTP data are Rg_high=%x, Rg_low=%x, Bg_high=%x, Bg_low=%x, gbgr_high=%x, gbgr_low=%x!!!\n", __func__, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);	

	awbRG = buf[0];
	awbRG <<= 8;
	awbRG += buf[1];
	awbBG = buf[2];
	awbBG <<= 8; 
	awbBG += buf[3];
	awbRB = buf[4];
	awbRB <<= 8;
	awbRB += buf[5];
	print_info("%s OTP data are awbRG=%x, awbBG=%x, awbRB=%x!!!\n",
		__func__, awbRG, awbBG, awbRB);	
	
	if(0 == awbRG || 0 ==awbBG || 0 == awbRB){//if awb value read is error for zero, abnormal branch deal
		print_error("%s OTP data is worng!!!\n",__func__);		
		return false;
	}

	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.r_g_high = buf[0];
	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.r_g_low = buf[1];
	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.b_g_high = buf[2];
	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.b_g_low = buf[3];
	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.gb_gr_high = buf[4];
	sonyimx219_sensor.sensor_hw_3a.awb_otp_value.gb_gr_low = buf[5];

	sonyimx219_otp_flag |= SONYIMX219_OTP_AWB_READ;	
	
	return true;
}

static bool sonyimx219_otp_read_lsc(void)
{
	print_debug("enter %s", __func__);

	memset(sonyimx219_otp_lsc_param, 0, SONYIMX219_OTP_LSC_SIZE);
	//LSC 0xa0:0b--0xff & 0xa2:00--0x22  total = 280
	sonyimx219_read_otp(OTP_SLAVE_ADDR1,OTP_LSC_1_REG,sonyimx219_otp_lsc_param,0xff-0x0b+1);
	sonyimx219_read_otp(OTP_SLAVE_ADDR2,OTP_LSC_2_REG,&sonyimx219_otp_lsc_param[0xff-0x0b+1],0x22+1);

	print_info("%s LCS[0]= %x,LSC[244] = %x  LSC[245]=%x,LSC[279]=%d\n",__func__,
		sonyimx219_otp_lsc_param[0],sonyimx219_otp_lsc_param[244],sonyimx219_otp_lsc_param[245],sonyimx219_otp_lsc_param[279]);
	sonyimx219_otp_flag |= SONYIMX219_OTP_LSC_READ;

	return true;
}


/****************************************************************************
* FunctionName: sonyimx219_otp_set_lsc;
* Description : Set lens shading parameters to sensor registers.; cost time is 0.0341s on sunny module
* Input       : NA;
* Output      : NA;
* ReturnValue : bool;
* Other       : NA;
***************************************************************************/
static bool sonyimx219_otp_set_lsc(void)
{
	u8 *pval = NULL;
	int i = 0;

	print_info("enter %s\n", __func__);

	/* Lens shading parameters are burned OK,Write lens shading parameters to sensor registers. */	
	pval = sonyimx219_otp_lsc_param;
	for (i=0; i<SONYIMX219_OTP_LSC_QUARTER_SIZE; i++,pval++) {//target reg:0xD200 ~ 0xD245
		sonyimx219_write_reg(0xd200+i, *pval, 0x00);
		print_debug("LSC[%d] = 0x%x \n",i,*pval);
	}
	
	for (i=0; i<SONYIMX219_OTP_LSC_QUARTER_SIZE; i++,pval++) {//target reg:0xD248 ~ 0xD28D
		sonyimx219_write_reg(0xd248+i, *pval, 0x00);
		print_debug("LSC[%d] = 0x%x \n",70+i,*pval);
	}
	
	for (i=0; i<SONYIMX219_OTP_LSC_QUARTER_SIZE; i++,pval++) {//target reg:0xD290 ~ 0xD2D5
		sonyimx219_write_reg(0xd290+i, *pval, 0x00);
		print_debug("LSC[%d] = 0x%x \n",140+i,*pval);
	}
	
	for (i=0; i<SONYIMX219_OTP_LSC_QUARTER_SIZE; i++,pval++) {//target reg:0xD2D8 ~  0xD31D
		sonyimx219_write_reg(0xd2d8+i, *pval, 0x00);
		print_debug("LSC[%d] = 0x%x \n",210+i,*pval);
	}

	//Open OTP lsc mode
	sonyimx219_write_reg(0x0190, 0x01, 0x00);//LSC enable A
	sonyimx219_write_reg(0x0192, 0x00, 0x00);//LSC table 0		
	sonyimx219_write_reg(0x0191, 0x00, 0x00);//LCS color mode:4 color R/Gr/Gb/B
	sonyimx219_write_reg(0x0193, 0x00, 0x00);//LSC tuning disable
	sonyimx219_write_reg(0x01a4, 0x03, 0x00);//Knot point format A:u4.8

	print_info("%s, set OTP LSC to sensor OK.\n", __func__);

	return true;
}

/****************************************************************************
* FunctionName: sonyimx219_otp_read_vcm;
* Description : Get AF motor parameters from OTP.;
* Input       : NA;
* Output      : NA;
* ReturnValue : bool;
* Other       : NA;
***************************************************************************/
static bool sonyimx219_otp_read_vcm(void)
{
	u8 buf[4] = {0};
    u16 start_code,end_code;

	print_debug("enter %s", __func__);

	if((sonyimx219_otp_flag & SONYIMX219_OTP_VCM_READ) == SONYIMX219_OTP_VCM_READ) { //OTP data is read allready
		print_debug("%s OTP VCM data is read allready!!!\n",__func__);
		return true;
	}

	sonyimx219_read_otp(OTP_SLAVE_ADDR2,OTP_VCM_REG,buf,4);

	start_code = buf[0];
	start_code <<= 8;
	start_code += buf[1];
	end_code = buf[2];
	end_code <<= 8;
	end_code += buf[3];

	if((start_code != end_code) &&(end_code>start_code) && start_code != 0 && end_code != 0) {
		sonyimx219_otp_flag |= SONYIMX219_OTP_VCM_READ;//this is no need to use, just use sonyimx219_vcm_start/end 0 value to achieve
		sonyimx219_vcm_start = start_code;
		sonyimx219_vcm_end = end_code;
		print_info("%s sonyimx219_vcm_start= 0x%x, sonyimx219_vcm_end = 0x%x \n",__func__, sonyimx219_vcm_start,sonyimx219_vcm_end);
	} else {//Abnormal branch deal
		sonyimx219_vcm_start = 0;	
		sonyimx219_vcm_end = 0;	
		print_error("%s VCM OTP data is worng! sonyimx219_vcm_start= 0x%x, sonyimx219_vcm_end = 0x%x \n",__func__, sonyimx219_vcm_start,sonyimx219_vcm_end);
		return false;
	}

	return true;	
}

/****************************************************************************
* FunctionName: sonyimx219_otp_get_vcm;
* Description : Get vcm start and stop parameters read from OTP.;
* Input       : NA;
* Output      : vcm_start vcm_end
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
static void sonyimx219_otp_get_vcm(u16 *vcm_start, u16 *vcm_end)
{
	if (0 == sonyimx219_vcm_start) {
		*vcm_start = sonyimx219_sensor.vcm->infiniteDistance;
	} else {	
        if (sonyimx219_vcm_start > sonyimx219_sensor.vcm->startCurrentOffset)
            *vcm_start = sonyimx219_vcm_start - sonyimx219_sensor.vcm->startCurrentOffset - OTP_INSURANCE_OFFSET;
        else
            *vcm_start = 0;
	}

	if (0 == sonyimx219_vcm_end) {
		*vcm_end = sonyimx219_sensor.vcm->normalDistanceEnd;
	} else {
		*vcm_end = sonyimx219_vcm_end;
	}
	
	print_info("%s, vcm_start:%#x, vcm_end:%#x", __func__, *vcm_start, *vcm_end);
}
static bool sonyimx219_get_otp_from_sensor(void)
{
	u8 sum = 0;
    u8 otpCheckSumVal = 0;	
	bool retVal = false;
#ifdef IMX219_OTP_READ_TIME_PRINT	
	u16 otpReadUsed = 0;
	struct timeval otpReadBegin, otpReadEnd;
#endif

	print_info("%s enters!\n",__func__);		
	if((sonyimx219_otp_flag & SONYIMX219_OTP_FAIL_FLAG) == SONYIMX219_OTP_FAIL_FLAG) {//Just check OTP once whether success or not
		print_error("%s OTP data is worng!\n", __func__);
		return false;
	}else if((sonyimx219_otp_flag & SONYIMX219_OTP_CHECKSUM_READ) == SONYIMX219_OTP_CHECKSUM_READ){//branch for read success, no need read more
		print_info("%s OTP has been read success already!\n",__func__);
		return true;
	}
	
#ifdef IMX219_OTP_READ_TIME_PRINT	
	memset(&otpReadBegin,0,sizeof(otpReadBegin));
	memset(&otpReadEnd,0,sizeof(otpReadEnd));
	do_gettimeofday(&otpReadBegin);	
#endif

	retVal = sonyimx219_otp_read_id();
	if(false == retVal){	
		print_error("%s sonyimx219_otp_read_id() failed!\n",__func__);
		goto OTP_FAIL;	
	}

	retVal = sonyimx219_otp_read_awb();
	if(false == retVal){
		print_error("%s sonyimx219_otp_read_awb() failed!\n",__func__);
		goto OTP_FAIL;	
	}

	retVal = sonyimx219_otp_read_vcm();
	if(false == retVal){		
		print_error("%s sonyimx219_otp_read_vcm() failed!\n",__func__);
		goto OTP_FAIL;		
	}	
	
	sonyimx219_otp_read_lsc();	

	sonyimx219_read_otp(OTP_SLAVE_ADDR2,OTP_CHECKSUM_REG,&otpCheckSumVal,1);

	sum = (OTPSUMVAL - otpCheckSumVal)%0xff + 1;

#ifdef IMX219_OTP_READ_TIME_PRINT	
	do_gettimeofday(&otpReadEnd);
	otpReadUsed = (otpReadEnd.tv_sec - otpReadBegin.tv_sec) * 1000 + (otpReadEnd.tv_usec - otpReadBegin.tv_usec) / 1000;
	print_info("%s used time is %d ms\n", __func__,otpReadUsed);
#endif

	if(otpCheckSumVal == sum){
		sonyimx219_otp_flag |= SONYIMX219_OTP_CHECKSUM_READ;
		print_info("%s success, OTPSUMVAL: %d, otpCheckSumVal: %d ,sum:%d, sonyimx219_otp_flag=0x%x\n", __func__, OTPSUMVAL, otpCheckSumVal,sum,sonyimx219_otp_flag);
		return true;
	} else {
		sonyimx219_otp_flag |= SONYIMX219_OTP_CHECKSUM_ERR;	
		print_error("%s fail, OTPSUMVAL: %d, otpCheckSumVal: %d ,sum:%d \n", __func__, OTPSUMVAL, otpCheckSumVal,sum);
	}
	
OTP_FAIL:
	sonyimx219_otp_flag |= SONYIMX219_OTP_FAIL_FLAG;
	print_error("%s sonyimx219_otp_flag=0x%x \n", __func__,sonyimx219_otp_flag);
	return false;
}
#endif

/*
 **************************************************************************
 * FunctionName: sonyimx219_set_default;
 * Description : init sonyimx219_sensor;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void sonyimx219_set_default(void)
{
    sonyimx219_sensor.init = sonyimx219_init;
	sonyimx219_sensor.exit = sonyimx219_exit;
	sonyimx219_sensor.shut_down = sonyimx219_shut_down;
	sonyimx219_sensor.reset = sonyimx219_reset;
	sonyimx219_sensor.check_sensor = sonyimx219_check_sensor;
	sonyimx219_sensor.power = sonyimx219_power;
	sonyimx219_sensor.init_reg = sonyimx219_init_reg;
	sonyimx219_sensor.stream_on = sonyimx219_stream_on;

	sonyimx219_sensor.get_format = sonyimx219_get_format;
	sonyimx219_sensor.set_flash = NULL;
	sonyimx219_sensor.get_flash = NULL;
	sonyimx219_sensor.set_scene = NULL;
	sonyimx219_sensor.get_scene = NULL;

	sonyimx219_sensor.enum_framesizes = sonyimx219_enum_framesizes;
	sonyimx219_sensor.try_framesizes = sonyimx219_try_framesizes;
	sonyimx219_sensor.set_framesizes = sonyimx219_set_framesizes;
	sonyimx219_sensor.get_framesizes = sonyimx219_get_framesizes;

	sonyimx219_sensor.enum_frame_intervals = sonyimx219_enum_frame_intervals;
	sonyimx219_sensor.try_frame_intervals = NULL;
	sonyimx219_sensor.set_frame_intervals = NULL;
	sonyimx219_sensor.get_frame_intervals = NULL;

	sonyimx219_sensor.get_vts_reg_addr = sonyimx219_get_vts_reg_addr;

	sonyimx219_sensor.get_capability = sonyimx219_get_capability;

	sonyimx219_sensor.set_hflip = sonyimx219_set_hflip;
	sonyimx219_sensor.get_hflip = sonyimx219_get_hflip;
	sonyimx219_sensor.set_vflip = sonyimx219_set_vflip;
	sonyimx219_sensor.get_vflip = sonyimx219_get_vflip;
	sonyimx219_sensor.update_flip = sonyimx219_update_flip;

	sonyimx219_sensor.sensor_hw_3a.support_awb_otp = sonyimx219_support_awb_otp;

	strncpy(sonyimx219_sensor.info.name,"sonyimx219",sizeof(sonyimx219_sensor.info.name));
	sonyimx219_sensor.interface_type = MIPI1;
	sonyimx219_sensor.mipi_lane_count = CSI_LINES_4;
	sonyimx219_sensor.mipi_index = CSI_INDEX_0;
	sonyimx219_sensor.sensor_index = CAMERA_SENSOR_PRIMARY;
	sonyimx219_sensor.skip_frames = 2;

	sonyimx219_sensor.power_conf.pd_valid = LOW_VALID;
	sonyimx219_sensor.power_conf.reset_valid = LOW_VALID;
	sonyimx219_sensor.power_conf.vcmpd_valid = LOW_VALID;

	sonyimx219_sensor.i2c_config.index = I2C_PRIMARY;
	sonyimx219_sensor.i2c_config.speed = I2C_SPEED_400;
	sonyimx219_sensor.i2c_config.addr = SONYIMX219_SLAVE_ADDRESS;
	sonyimx219_sensor.i2c_config.addr_bits = I2C_16BIT;
	sonyimx219_sensor.i2c_config.val_bits = I2C_8BIT;

	sonyimx219_sensor.preview_frmsize_index = 0;
	sonyimx219_sensor.capture_frmsize_index = 0;
	sonyimx219_sensor.frmsize_list = sonyimx219_framesizes;
	sonyimx219_sensor.fmt[STATE_PREVIEW] = V4L2_PIX_FMT_RAW10;
	sonyimx219_sensor.fmt[STATE_CAPTURE] = V4L2_PIX_FMT_RAW10;

#ifdef SONYIMX219_AP_WRITEAE_MODE /* just an example and test case for AP write AE mode */
	sonyimx219_sensor.aec_addr[0] = 0;
	sonyimx219_sensor.aec_addr[1] = 0;
	sonyimx219_sensor.aec_addr[2] = 0;
	sonyimx219_sensor.agc_addr[0] = 0;
	sonyimx219_sensor.agc_addr[1] = 0;
	sonyimx219_sensor.ap_writeAE_delay = 6000; /* 5 expo and gain registers, 6000 us is enough */
#else//NOT USE MCU CONTROL, IF USED, SHOULD MODIFY ISP SETTING FOR IMX219
	sonyimx219_sensor.aec_addr[0] = 0x0000;
	sonyimx219_sensor.aec_addr[1] = 0x0160;
	sonyimx219_sensor.aec_addr[2] = 0x0161;
	sonyimx219_sensor.agc_addr[0] = 0x0000;
	sonyimx219_sensor.agc_addr[1] = 0x0167;
#endif
	sonyimx219_sensor.sensor_type = SENSOR_OV;//For no group function, gain:n+1;expo:n+2, instead of SENSOR_OV TYPE
	sonyimx219_sensor.sensor_rgb_type = SENSOR_BGGR;

	sonyimx219_sensor.set_gain = sonyimx219_set_gain;
	sonyimx219_sensor.set_exposure = sonyimx219_set_exposure;
	sonyimx219_sensor.set_exposure_gain = NULL;//sonyimx219_set_exposure_gain;
	sonyimx219_sensor.set_vts = sonyimx219_set_vts;

	sonyimx219_sensor.get_override_param = NULL;

	sonyimx219_sensor.sensor_gain_to_iso = sonyimx219_gain_to_iso;
	sonyimx219_sensor.sensor_iso_to_gain = sonyimx219_iso_to_gain;

	sonyimx219_sensor.get_sensor_aperture = sonyimx219_get_sensor_aperture;
	sonyimx219_sensor.get_equivalent_focus = NULL;

	sonyimx219_sensor.set_effect = NULL;

	sonyimx219_sensor.isp_location = CAMERA_USE_K3ISP;
	sonyimx219_sensor.sensor_tune_ops = NULL;

	sonyimx219_sensor.af_enable = 1;
	sonyimx219_sensor.get_flash_awb = sonyimx219_get_flash_awb;

	sonyimx219_sensor.image_setting.lensc_param = NULL;
	sonyimx219_sensor.image_setting.ccm_param = NULL;
	sonyimx219_sensor.image_setting.awb_param = NULL;
	sonyimx219_sensor.fps_max = 30;
	sonyimx219_sensor.fps_min = 15;
	sonyimx219_sensor.fps = 25;
	sonyimx219_sensor.owner = THIS_MODULE;

	sonyimx219_sensor.info.facing = CAMERA_FACING_BACK;
	sonyimx219_sensor.info.orientation = 270;
	sonyimx219_sensor.info.focal_length = 293;	/* 2.93mm */
	sonyimx219_sensor.info.h_view_angle = 75;	/* 75.2 degree */
	sonyimx219_sensor.info.v_view_angle = 75;
	sonyimx219_sensor.lane_clk = CLK_750M;
	sonyimx219_sensor.hflip = 0;
	sonyimx219_sensor.vflip = 0;
	sonyimx219_sensor.old_flip = 0;

	sonyimx219_sensor.get_gain     				= sonyimx219_get_gain;
	sonyimx219_sensor.get_exposure   			= sonyimx219_get_exposure;
	sonyimx219_sensor.init_isp_reg 				= sonyimx219_init_isp_reg;
	sonyimx219_sensor.support_summary = false;
    sonyimx219_sensor.get_ccm_pregain = NULL;
    sonyimx219_sensor.set_awb = NULL;
    sonyimx219_sensor.set_anti_banding = NULL;
    sonyimx219_sensor.update_framerate = NULL;
    sonyimx219_sensor.awb_dynamic_ccm_gain = NULL;
    sonyimx219_sensor.pclk = 0;
    sonyimx219_sensor.max_int_lines = 0;
    sonyimx219_sensor.min_int_lines = 0;
    sonyimx219_sensor.real_int_lines = 0;
    sonyimx219_sensor.min_gain = 0;
    sonyimx219_sensor.max_gain = 0;
    sonyimx219_sensor.real_gain = 0;
    sonyimx219_sensor.rcc_enable = false;
    sonyimx219_sensor.isp_idi_skip = false;
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_module_init;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static __init int sonyimx219_module_init(void)
{
	sonyimx219_set_default();
	return register_camera_sensor(sonyimx219_sensor.sensor_index, &sonyimx219_sensor);
}

/*
 **************************************************************************
 * FunctionName: sonyimx219_module_exit;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static void __exit sonyimx219_module_exit(void)
{
	unregister_camera_sensor(sonyimx219_sensor.sensor_index, &sonyimx219_sensor);
}

MODULE_AUTHOR("Hisilicon");
module_init(sonyimx219_module_init);
module_exit(sonyimx219_module_exit);
MODULE_LICENSE("GPL");

