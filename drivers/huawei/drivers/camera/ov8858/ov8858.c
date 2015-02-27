/*
 * ov8858 sensor driver
 *
 *Author: 	Hanjunhui (hanjh_hit@163.com)
 *  Date:  	2014/02/12
 *  Version:	1.0
 *  History:	2014/02/12      Frist add driver for ov8858
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
#include "ov8858.h"
#include <asm/bug.h>
#include <linux/device.h>

#define LOG_TAG "OV8858"
//#define DEBUG_DEBUG 1
#include "cam_log.h"
#include <cam_util.h>
#include "drv_regulator_user.h"
#include "hsad/config_interface.h"
#include <mach/pmussi_drv.h>
#include "effect.h"

#define OV8858_ZSL                         (0x00)

#define OV8858_SLAVE_ADDRESS                0x20 //SID=DOVDD
#define OV8858_CHIP_ID                      (0x8858)

#define OV8858_MIRROR		                0x3821
#define OV8858_FLIP		                    0x3820

#define OV8858_EXPOSURE_REG_H	            0x3500
#define OV8858_EXPOSURE_REG_M	            0x3501
#define OV8858_EXPOSURE_REG_L	            0x3502
#define OV8858_GAIN_REG_H		            0x3508
#define OV8858_GAIN_REG_L		            0x3509

#define OV8858_VTS_REG_H		            0x380e
#define OV8858_VTS_REG_L		            0x380f

#define OV8858_APERTURE_FACTOR              200 //F2.0
//#define OV8858_EQUIVALENT_FOCUS	            0

#define OV8858_AP_WRITEAE_MODE
#define OV8858_MAX_ANALOG_GAIN	            16

#define OV8858_I2C_RETRY_TIMES              3

enum sensor_module_type
{
	MODULE_FOXCONN,
	MODULE_SUNNY,
	MODULE_UNSUPPORT
};

static u8 sensor_module;

#ifdef OV8858_OTP //this otp is just for foxconn module, if sunny module is being used, this may need tune
//#define OV8858_OTP_READ_TIME_PRINT

static u16 RG_Ratio_Typical = 0x233;//the average of 5 Golden samples' RG ratio under D50
static u16 BG_Ratio_Typical = 0x2b0;//the average of 5 Golden samples' BG ratio under D50

typedef enum otp_group_index_type
{
	OTP_GROUP_ONE_VALID,
	OTP_GROUP_TWO_VALID,
	OTP_GROUP_THREE_VALID,
	OTP_GROUP_INVALID
}otp_group_index;

#define OV8858_OTP_LSC_SIZE             110

typedef struct ov8858_otp_struct_type {
	u16 rg_ratio;
	u16 bg_ratio;
	u16 grgb;
	u16 vcm_start;
	u16 vcm_end;
	u8  lsc[OV8858_OTP_LSC_SIZE];
}ov8858_otp_struct;

static ov8858_otp_struct ov8858_otp;

#define OV8858_OTP_ID_VALID				(1 << 0)
#define OV8858_OTP_VCM_VALID			(1 << 1)
#define OV8858_OTP_LSC_VALID			(1 << 2)
#define OV8858_OTP_AWB_VALID			(1 << 3)
#define OV8858_OTP_SUCCESS_FLAG  		(1 << 4)
#define OV8858_OTP_FAIL_FLAG			(1 << 5)

static otp_group_index id_otp_group_index  = OTP_GROUP_INVALID;
static otp_group_index awb_otp_group_index = OTP_GROUP_INVALID;
static otp_group_index vcm_otp_group_index = OTP_GROUP_INVALID;
static otp_group_index lsc_otp_group_index = OTP_GROUP_INVALID;

static u8 ov8858_otp_flag = 0;

static bool ov8858_get_otp_from_sensor(void);
static bool ov8858_check_otp_validation(void);
static void ov8858_otp_read_id(void);
static void ov8858_otp_read_vcm(void);
static void ov8858_otp_get_vcm(u16 *vcm_start, u16 *vcm_end);
static void ov8858_otp_read_awb(void);
static void ov8858_otp_set_awb(void);
static void ov8858_otp_read_lsc(void);
static void ov8858_otp_set_lsc(void);
#endif

static camera_capability ov8858_cap[] = {
	{V4L2_CID_FLASH_MODE, THIS_FLASH},
	{V4L2_CID_FOCUS_MODE, THIS_FOCUS_MODE},
	{V4L2_CID_FOCAL_LENGTH, 287},//2.872mm
	{V4L2_CID_ZSL,		  OV8858_ZSL},
};

static awb_gain_t flash_platform_awb[] = {
	{0xbd, 0x80, 0x80, 0xdc}, /*EDGE*/
};

static vcm_info_s vcm_dw9714_Sunny = {
    #include "vcm_dw9714_Sunny.h"
};

static vcm_info_s vcm_dw9714_Foxconn = {
    #include "vcm_dw9714_Foxconn.h"
};

static effect_params effect_ov8858_sunny = {
    #include "effect_ov8858_sunny_H30L.h"
};

static effect_params effect_ov8858_foxconn = {
    #include "effect_ov8858_foxconn_H30L.h"
};

static framesize_s ov8858_framesizes[] = {      
	/* 1600x1200@30fps binning */
	{0, 0, 1600, 1200, 1776, 1350, 30, 30, 0x195, 0x152, 0x100, VIEW_FULL, RESOLUTION_4_3, false, false,{ (sensor_reg_t *)ov8858_framesize_1600x1200, ARRAY_SIZE(ov8858_framesize_1600x1200)} },

	/* 1920x1088@30fps for 1080P video not use by H30L*/
	{0, 0, 1920, 1088, 3880, 2148, 30, 30, 0x294, 0x226, 0x1a0, VIEW_CROP, RESOLUTION_16_9, false, false, { (sensor_reg_t *)ov8858_framesize_1080p, ARRAY_SIZE(ov8858_framesize_1080p)} },

    /* 3264*2448@30fps for ZSL only*/
    {0, 0, 3264, 2448, 3880, 2528, 30, 30, 0x45a, 0x3a0, 0x100, VIEW_FULL, RESOLUTION_4_3, false, true, { (sensor_reg_t *)ov8858_framesize_zsl_full, ARRAY_SIZE(ov8858_framesize_zsl_full)} },

	/* 3264*2448@15fps */
	{0, 0, 3264, 2448, 3880, 2528, 15, 15, 0x17d, 0x13e, 0xf0, VIEW_FULL, RESOLUTION_4_3, false, false,{ (sensor_reg_t *)ov8858_framesize_full, ARRAY_SIZE(ov8858_framesize_full)} },
	
	/* 3280x1960@30fps for 1080p recording*/
    {0, 0, 3280, 1960, 3880, 2148, 30, 30, 0x294, 0x226, 0x1a0, VIEW_FULL, RESOLUTION_16_9, false, false, { (sensor_reg_t *)ov8858_framesize_3280x1960, ARRAY_SIZE(ov8858_framesize_3280x1960)} },

	/* 3280*2464@30fps for ZSL only*/
	{0, 0, 3280, 2464, 3880, 2528, 30, 30, 0x45a, 0x3a0, 0x1e0, VIEW_FULL, RESOLUTION_4_3, false, true,{ (sensor_reg_t *)ov8858_framesize_3280x2464, ARRAY_SIZE(ov8858_framesize_3280x2464)} }
};

static camera_sensor ov8858_sensor;

/*
 **************************************************************************
 * FunctionName: ov8858_read_reg;
 * Description : read ov8858 reg by i2c;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int ov8858_read_reg(u16 reg, u8 *val)
{
	return k3_ispio_read_reg(ov8858_sensor.i2c_config.index,
			 ov8858_sensor.i2c_config.addr, reg, (u16*)val, ov8858_sensor.i2c_config.val_bits,ov8858_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: ov8858_write_reg;
 * Description : write ov8858 reg by i2c;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int ov8858_write_reg(u16 reg, u8 val, u8 mask)
{
	return k3_ispio_write_reg(ov8858_sensor.i2c_config.index,
			ov8858_sensor.i2c_config.addr, reg, val, ov8858_sensor.i2c_config.val_bits, mask,ov8858_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: ov8858_write_seq;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static int ov8858_write_seq(const struct _sensor_reg_t *seq, u32 size, u8 mask)
{
	print_debug("Enter %s, seq[%#x], size=%d", __func__, (int)seq, size);
	return k3_ispio_write_seq(ov8858_sensor.i2c_config.index,
			ov8858_sensor.i2c_config.addr, seq, size, ov8858_sensor.i2c_config.val_bits, mask,ov8858_sensor.i2c_config.addr_bits);
}

/*
 **************************************************************************
 * FunctionName: ov8858_write_isp_seq;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static void ov8858_write_isp_seq(const struct isp_reg_t *seq, u32 size)
{
	print_debug("Enter %s, seq[%#x], size=%d", __func__, (int)seq, size);
	k3_ispio_write_isp_seq(seq, size);
}

/*
 **************************************************************************
 * FunctionName: ov8858_enum_frame_intervals;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_enum_frame_intervals(struct v4l2_frmivalenum *fi)
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
 * FunctionName: ov8858_get_format;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_get_format(struct v4l2_fmtdesc *fmt)
{
	if (fmt->type == V4L2_BUF_TYPE_VIDEO_OVERLAY) {
		fmt->pixelformat = ov8858_sensor.fmt[STATE_PREVIEW];
	} else {
		fmt->pixelformat = ov8858_sensor.fmt[STATE_CAPTURE];
	}
	return 0;
}

static int ov8858_get_capability(u32 id, u32 *value)
{
	int i;
	for (i = 0; i < sizeof(ov8858_cap) / sizeof(ov8858_cap[0]); ++i) {
		if (id == ov8858_cap[i].id) {
			*value = ov8858_cap[i].value;
			break;
		}
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_enum_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_enum_framesizes(struct v4l2_frmsizeenum *framesizes)
{
	u32 max_index = ARRAY_SIZE(camera_framesizes) - 1;
	u32 this_max_index = ARRAY_SIZE(ov8858_framesizes) - 1;

    if(!framesizes) {
        return -EINVAL;
    }

	print_debug("enter %s; ", __func__);

	if (framesizes->index > max_index) {
		print_error("framesizes->index = %d error", framesizes->index);
		return -EINVAL;
	}

	if ((camera_framesizes[framesizes->index].width > ov8858_framesizes[this_max_index].width)
		|| (camera_framesizes[framesizes->index].height > ov8858_framesizes[this_max_index].height)) {
		print_error("framesizes->index = %d error", framesizes->index);
		return -EINVAL;
	}

	framesizes->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	framesizes->discrete.width = ov8858_framesizes[this_max_index].width;
	framesizes->discrete.height = ov8858_framesizes[this_max_index].height;

	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_try_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_try_framesizes(struct v4l2_frmsizeenum *framesizes)
{
	int max_index = ARRAY_SIZE(ov8858_framesizes) - 1;

    if(!framesizes) {
        return -EINVAL;
    }

	print_debug("Enter Function:%s  ", __func__);

	if ((framesizes->discrete.width <= ov8858_framesizes[max_index].width)
	    && (framesizes->discrete.height <= ov8858_framesizes[max_index].height)) {
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
 * FunctionName: ov8858_set_framesizes;
 * Description : NA;
 * Input       : flag: if 1, set framesize to sensor,
 *					   if 0, only store framesize to camera_interface;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_set_framesizes(camera_state state,
				 struct v4l2_frmsize_discrete *fs, int flag, camera_setting_view_type view_type,bool zsl_preview)
{
	int i = 0;
	bool match = false;
	int size = 0;
    if(!fs) {
        return -EINVAL;
    }

	print_info("Enter Function:%s State(%d), flag=%d, width=%d, height=%d",
		   __func__, state, flag, fs->width, fs->height);

	/*ov8858 not support scale down, so we use full view sequence 3280x1960*/
	if(fs->width == 1920){
		fs->width = 3280;
		fs->height = 1960;
	}

	print_info("Enter Function:%s resolution_type(%d), view_type=%d, width=%d, height=%d",
		   __func__, camera_get_resolution_type(fs->width, fs->height), view_type, fs->width, fs->height);

	size = ARRAY_SIZE(ov8858_framesizes);

	if (VIEW_FULL == view_type) {
		for (i = 0; i < size; i++) {
		    /*zsl only=true means is only for zsl, otherwise can used by zsl and not zsl*/
		    if (zsl_preview == false && ov8858_framesizes[i].zsl_only) {
		        continue;
		    }

			if ((ov8858_framesizes[i].width >= fs->width)
			    && (ov8858_framesizes[i].height >= fs->height)
			    && (VIEW_FULL == ov8858_framesizes[i].view_type)
			    && (camera_get_resolution_type(fs->width, fs->height)
			    <= ov8858_framesizes[i].resolution_type)) {
    				fs->width = ov8858_framesizes[i].width;
    				fs->height = ov8858_framesizes[i].height;
    				match = true;
					print_info("Enter Function:%s [%d] settings are OV8858_resolution_type(%d), OV8858_view_type=%d, ov8858_width=%d, ov8858_height=%d, OV8858_fps = %d fps",
					   __func__, i, ov8858_framesizes[i].resolution_type, ov8858_framesizes[i].view_type, ov8858_framesizes[i].width, ov8858_framesizes[i].height,ov8858_framesizes[i].fps);
    				break;
			}
			print_info("Enter Function:%s [%d] settings are OV8858_resolution_type(%d), OV8858_view_type=%d, ov8858_width=%d, ov8858_height=%d, OV8858_fps = %d fps",
			   __func__, i, ov8858_framesizes[i].resolution_type, ov8858_framesizes[i].view_type, ov8858_framesizes[i].width, ov8858_framesizes[i].height,ov8858_framesizes[i].fps);
		}
	}

	if (false == match) {
		for (i = 0; i < size; i++) {
		    /*zsl only=true means is only for zsl, otherwise can used by zsl and not zsl*/
		    if (zsl_preview == false && ov8858_framesizes[i].zsl_only) {
		        continue;
		    }

			if ((ov8858_framesizes[i].width >= fs->width)
				    && (ov8858_framesizes[i].height >= fs->height)
				    && (camera_get_resolution_type(fs->width, fs->height)
    				    <= ov8858_framesizes[i].resolution_type)
				    && (VIEW_HDR_MOVIE != ov8858_framesizes[i].view_type)){
					fs->width = ov8858_framesizes[i].width;
					fs->height = ov8858_framesizes[i].height;
					break;
			}
		}
	}

	if (i >= size) {
		print_error("request resolution larger than sensor's max resolution");
		return -EINVAL;
	}

	print_info("Enter Function:%s match=%d,final settings are OV8858_resolution_type(%d), OV8858_view_type=%d, ov8858_width=%d, ov8858_height=%d,ov8858_fps=%d fps",
		   __func__, match, ov8858_framesizes[i].resolution_type, ov8858_framesizes[i].view_type, ov8858_framesizes[i].width, ov8858_framesizes[i].height,ov8858_framesizes[i].fps);
	
	if (state == STATE_PREVIEW) {
		ov8858_sensor.preview_frmsize_index = i;
	} else {
		ov8858_sensor.capture_frmsize_index = i;
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_get_framesizes;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_get_framesizes(camera_state state,
				     struct v4l2_frmsize_discrete *fs)
{
	int frmsize_index;

    if(!fs) {
        return -EINVAL;
    }

	if (state == STATE_PREVIEW) {
		frmsize_index = ov8858_sensor.preview_frmsize_index;
	} else if (state == STATE_CAPTURE) {
		frmsize_index = ov8858_sensor.capture_frmsize_index;
	} else {
		return -EINVAL;
	}
	fs->width = ov8858_framesizes[frmsize_index].width;
	fs->height = ov8858_framesizes[frmsize_index].height;

	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_init_isp_reg;
 * Description : load initial seq for sensor init;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_init_isp_reg(void)
{
	int size = 0;
	camera_sensor *sensor = &ov8858_sensor;

    size = CAMERA_MAX_SETTING_SIZE;
	ov8858_write_isp_seq(sensor->effect->isp_settings, size);

	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_support_awb_otp;
 * Description : check sensor support awb otp or not;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_support_awb_otp(void)
{
	return AWB_OTP_NOT_SUPPORT;//the awb OTP is achieved by sensor IC self for ov8858, does not need ISP deal
}

/*
 **************************************************************************
 * FunctionName: ov8858_init_reg;
 * Description : download initial seq for sensor init;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_init_reg(void)
{
	int size = 0;

	print_debug("Enter Function:%s  , initsize=%d",
		    __func__, sizeof(ov8858_init_regs));

	if (0 != k3_ispio_init_csi(ov8858_sensor.mipi_index,
			ov8858_sensor.mipi_lane_count, ov8858_sensor.lane_clk)) {
		return -EFAULT;
	}

	mdelay(5);
	size = ARRAY_SIZE(ov8858_init_regs);
	if (0 != ov8858_write_seq(ov8858_init_regs, size, 0x00)) {
		print_error("line %d, fail to init ov8858 sensor",__LINE__);
		return -EFAULT;
	}

#ifdef OV8858_OTP
	if(sensor_module == MODULE_FOXCONN){//H30L just support ov8858_foxconn, does not use sunny module, so sunny otp does not achieve
		if((ov8858_otp_flag & OV8858_OTP_LSC_VALID) == OV8858_OTP_LSC_VALID && (ov8858_otp_flag & OV8858_OTP_SUCCESS_FLAG) == OV8858_OTP_SUCCESS_FLAG){
			ov8858_otp_set_lsc();
		}
		if((ov8858_otp_flag & OV8858_OTP_AWB_VALID) == OV8858_OTP_AWB_VALID && (ov8858_otp_flag & OV8858_OTP_SUCCESS_FLAG) == OV8858_OTP_SUCCESS_FLAG){
			ov8858_otp_set_awb();
		}
	}
#endif

	return 0;
}

static int ov8858_set_hflip(int flip)
{
	print_debug("enter %s flip=%d", __func__, flip);

	ov8858_sensor.hflip = flip;

	return 0;
}
static int ov8858_get_hflip(void)
{
	print_debug("enter %s", __func__);

	return ov8858_sensor.hflip;
}
static int ov8858_set_vflip(int flip)
{
	print_debug("enter %s flip=%d", __func__, flip);

	ov8858_sensor.vflip = flip;

	return 0;
}
static int ov8858_get_vflip(void)
{
	print_debug("enter %s", __func__);
	return ov8858_sensor.vflip;
}

static int ov8858_update_flip(u16 width, u16 height)
{
	u8 new_flip = ((ov8858_sensor.vflip << 1) | ov8858_sensor.hflip);

	print_debug("Enter %s  ", __func__);

	k3_ispio_update_flip((ov8858_sensor.old_flip ^ new_flip) & 0x03, width, height, PIXEL_ORDER_NO_CHANGED);

	ov8858_sensor.old_flip = new_flip;

	if(E_CAMERA_SENSOR_FLIP_TYPE_NONE == get_primary_sensor_flip_type()){
		ov8858_write_reg(OV8858_FLIP, ov8858_sensor.vflip ? 0x00 : 0x06, ~0x06);
		ov8858_write_reg(OV8858_MIRROR, ov8858_sensor.hflip ? 0x06 : 0x00, ~0x06);
	}else if (E_CAMERA_SENSOR_FLIP_TYPE_V == get_primary_sensor_flip_type()){
		ov8858_write_reg(OV8858_FLIP, ov8858_sensor.vflip ? 0x00 : 0x06, ~0x06);
	}else if (E_CAMERA_SENSOR_FLIP_TYPE_H == get_primary_sensor_flip_type()){
		ov8858_write_reg(OV8858_MIRROR, ov8858_sensor.hflip ? 0x00 : 0x06, ~0x06);
	}

	return 0;
}
/*
 **************************************************************************
 * FunctionName: ov8858_framesize_switch;
 * Description : switch frame size, used by preview and capture
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_framesize_switch(camera_state state)
{
	u8 next_frmsize_index;

	if (state == STATE_PREVIEW)
		next_frmsize_index = ov8858_sensor.preview_frmsize_index;
	else
		next_frmsize_index = ov8858_sensor.capture_frmsize_index;

	print_debug("Enter Function:%s frm index=%d", __func__, next_frmsize_index);

	if (next_frmsize_index >= ARRAY_SIZE(ov8858_framesizes)){
		print_error("Unsupport sensor setting index: %d",next_frmsize_index);
		return -ETIME;
	}

	if (0 != ov8858_write_seq(ov8858_sensor.frmsize_list[next_frmsize_index].sensor_setting.setting
		,ov8858_sensor.frmsize_list[next_frmsize_index].sensor_setting.seq_size, 0x00)) {
		print_error("fail to init ov8858 sensor");
		return -ETIME;
	}
	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_stream_on;
 * Description : download preview seq for sensor preview;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_stream_on(camera_state state)
{
	print_debug("Enter Function:%s ", __func__);

	return ov8858_framesize_switch(state);
}

/*  **************************************************************************
* FunctionName: ov8858_check_sensor;
* Description : NA;
* Input       : NA;
* Output      : NA;
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
static int ov8858_check_sensor(void)
{
	u8 idl = 0;
	u8 idm = 0;
	u8 idh = 0;
	u32 id = 0;
	int pin_id = 0;
    int retry = 0;
    camera_sensor *sensor = &ov8858_sensor;
	bool retVal=false;

    for(retry = 0; retry < OV8858_I2C_RETRY_TIMES; retry++) {
		ov8858_read_reg(0x300A, &idh);
		ov8858_read_reg(0x300B, &idm);
    	ov8858_read_reg(0x300C, &idl);
    	id = ((idh << 16) | (idm << 8) | idl);
        print_info("ov8858 product id:0x%x, retrytimes:%d", id, retry);
        if(OV8858_CHIP_ID == id) {
            break;
        }
        udelay(100);
    }
	if (OV8858_CHIP_ID != id) {
		print_error("Invalid product id ,Could not load sensor ov8858\n");
		return -ENODEV;
	}
	pin_id = gpio_get_value(GPIO_18_3);
	if(pin_id < 0) {
		pin_id = 0;
		print_error("ov8858_check_sensor fail to get gpio value!!! set pin_id to 0 by default MODULE_FOXCONN!\n");
	}
	sensor_module = pin_id>0 ? MODULE_SUNNY:MODULE_FOXCONN;//module_ID for foxconn is 0, for sunny is dovdd

	if(sensor_module ==  MODULE_SUNNY) {
		ov8858_sensor.vcm = &vcm_dw9714_Sunny;
        sensor->effect = &effect_ov8858_sunny;
		snprintf(ov8858_sensor.info.name, sizeof(ov8858_sensor.info.name),"ov8858_sunny");
	}else {
		ov8858_sensor.vcm = &vcm_dw9714_Foxconn;
        sensor->effect = &effect_ov8858_foxconn;
		snprintf(ov8858_sensor.info.name, sizeof(ov8858_sensor.info.name),"ov8858_foxconn");
	}

#ifdef OV8858_OTP
	ov8858_write_reg(0x0100, 0x01, 0x00);
	if(sensor_module == MODULE_FOXCONN){//H30L just support ov8858_foxconn, does not use sunny module, so sunny otp does not achieve
		retVal=ov8858_get_otp_from_sensor();
		if(false == retVal){
			print_error("%s ov8858_get_otp_from_sensor() fails!\n", __func__);
		}

		if((ov8858_otp_flag & OV8858_OTP_VCM_VALID) == OV8858_OTP_VCM_VALID && (ov8858_otp_flag & OV8858_OTP_SUCCESS_FLAG) == OV8858_OTP_SUCCESS_FLAG){
			print_info("%s register ov8858_sensor.vcm->get_vcm_otp ",__func__);
			ov8858_sensor.vcm->get_vcm_otp = ov8858_otp_get_vcm;
		}
	}
	ov8858_write_reg(0x0100, 0x00, 0x00);
#endif
	return 0;
}

/****************************************************************************
* FunctionName: ov8858_power;
* Description : NA;
* Input       : NA;
* Output      : NA;
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
int ov8858_power(camera_power_state power)
{
	int ret = 0;

	print_debug("Enter Function:%s\n ", __func__);

	if(0 == get_sensor_iopower()) {
		if (power == POWER_ON) {
			k3_ispldo_power_sensor(power, M_CAMERA_ANALOG_VDD);
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

			k3_ispio_ioconfig(&ov8858_sensor, power);
			k3_ispgpio_power_sensor(&ov8858_sensor, power);
			msleep(3);
		} else {		
			k3_ispio_deinit_csi(ov8858_sensor.mipi_index);
			k3_ispgpio_power_sensor(&ov8858_sensor, power);
			k3_ispio_ioconfig(&ov8858_sensor, power);

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

			k3_ispio_ioconfig(&ov8858_sensor, power);
			k3_ispgpio_power_sensor(&ov8858_sensor, power);
			msleep(3);
		} else {
			k3_ispio_deinit_csi(ov8858_sensor.mipi_index);
			k3_ispgpio_power_sensor(&ov8858_sensor, power);
			k3_ispio_ioconfig(&ov8858_sensor, power);

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

static int ov8858_get_sensor_aperture(void)
{
	return OV8858_APERTURE_FACTOR;
}

/*
 * Here gain is in unit 1/16 of sensor gain,
 * y36721 todo, temporarily if sensor gain=0x10, ISO is 100
 * in fact we need calibrate an ISO-ET-gain table.
 */
u32 ov8858_gain_to_iso(int gain)
{
	return (gain * 100) / 0x10;
}

u32 ov8858_iso_to_gain(int iso)
{
    return (iso * 0x10) / 100;
}

void ov8858_set_gain(u32 gain)
{
	u16 analog_gain = 0;

	gain <<=3;
	if(gain >= OV8858_MAX_ANALOG_GAIN*128)// write gain, 128 = 1x, max gain = 16x
	{
		analog_gain  = 0x7ff;
	}else{
		analog_gain  = gain & 0x7ff;
	}

	ov8858_write_reg(OV8858_GAIN_REG_L, analog_gain&0xff, 0x00);
	ov8858_write_reg(OV8858_GAIN_REG_H, (analog_gain>>8)&0xff, 0x00);
}

void ov8858_set_exposure(u32 exposure)
{
	ov8858_write_reg(OV8858_EXPOSURE_REG_L, exposure&0xf0, 0x00);
	ov8858_write_reg(OV8858_EXPOSURE_REG_M, (exposure>>8)&0xff, 0x00);
	ov8858_write_reg(OV8858_EXPOSURE_REG_H, (exposure>>16)&0x0f, 0x00);
}

void ov8858_set_exposure_gain(u32 exposure, u32 gain)
{
	u16 analog_gain = 0;

	ov8858_write_reg(OV8858_EXPOSURE_REG_L, exposure&0xf0, 0x00);
	ov8858_write_reg(OV8858_EXPOSURE_REG_M, (exposure>>8)&0xff, 0x00);
	ov8858_write_reg(OV8858_EXPOSURE_REG_H, (exposure>>16)&0x0f, 0x00);

	gain <<=3;
	if(gain >= OV8858_MAX_ANALOG_GAIN*128)// write gain 128 = 1x, max gain = 16x
	{
		analog_gain  = 0x7ff;
	}
	else
	{
		analog_gain  = gain & 0x7ff;
	}

	ov8858_write_reg(OV8858_GAIN_REG_L, analog_gain&0xff, 0x00);
	ov8858_write_reg(OV8858_GAIN_REG_H, (analog_gain>>8)&0xff, 0x00);

	return;
}

void ov8858_set_vts(u16 vts)
{
	print_debug("Enter %s  ", __func__);
	ov8858_write_reg(OV8858_VTS_REG_L, vts & 0xff, 0x00);
	ov8858_write_reg(OV8858_VTS_REG_H, (vts >> 8) & 0xff, 0x00);
}

u32 ov8858_get_vts_reg_addr(void)
{
	return OV8858_VTS_REG_H;
}

/*
 **************************************************************************
 * FunctionName: ov8858_get_gain;
 * Description :get gain from sensor
 * Input       	 NA;
 * Output      	 gain value from sensor
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
u32 ov8858_get_gain(void)
{
	u32 analog_gain = 0;
	u8  gain_l = 0;
	u8  gain_h = 0;
	ov8858_read_reg(OV8858_GAIN_REG_L, &gain_l);
	ov8858_read_reg(OV8858_GAIN_REG_H, &gain_h);
	analog_gain = gain_h<<8 + gain_l;
	if(_IS_DEBUG_AE) {
		print_info("%s-0x3508& 0x3509 =%x",__func__,analog_gain);
	}

	return analog_gain;
}

/*
 **************************************************************************
 * FunctionName: ov8858_get_exposure;
 * Description :get exposure from sensor
 * Input       	 NA;
 * Output      	 exposure value from sensor
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
u32  ov8858_get_exposure(void)
{
	u32 expo = 0;
	u8  expo_h = 0;
	u8  expo_m = 0;
	u8  expo_l = 0;
	
	ov8858_read_reg(OV8858_EXPOSURE_REG_H, &expo_h);
	ov8858_read_reg(OV8858_EXPOSURE_REG_M, &expo_m);
	ov8858_read_reg(OV8858_EXPOSURE_REG_L, &expo_l);
	expo = expo_h<<12 + expo_m<<4 + expo_l>>4;

	if(_IS_DEBUG_AE) {
		print_info("ov8858_get_exposure-0x3500&0x3501&0x3502=%x",expo);
	}

	return expo;
}

/*
 **************************************************************************
 * FunctionName: ov8858_reset;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_reset(camera_power_state power_state)
{
	print_debug("%s  \n", __func__);
	ov8858_sensor.old_flip = 0;
	ov8858_sensor.sensor_hdr_movie.hdrInfo.hdr_on = HDR_MOVIE_OFF;

	if (POWER_ON == power_state) {
		k3_isp_io_enable_mclk(MCLK_ENABLE, ov8858_sensor.sensor_index);
		udelay(100);
		k3_ispgpio_reset_sensor(ov8858_sensor.sensor_index, power_state,
			      ov8858_sensor.power_conf.reset_valid);
		udelay(500);
	} else {
		k3_ispgpio_reset_sensor(ov8858_sensor.sensor_index, power_state,
			      ov8858_sensor.power_conf.reset_valid);
		udelay(10);
		k3_isp_io_enable_mclk(MCLK_DISABLE, ov8858_sensor.sensor_index);
	}

	return 0;
}

/*
 **************************************************************************
 * FunctionName: ov8858_init;
 * Description : ov8858 init function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : Error code indicating success or failure;
 * Other       : NA;
 **************************************************************************
*/
static int ov8858_init(void)
{
	static bool ov8858_check = false;
    print_debug("%s  ", __func__);

	if (false == ov8858_check) {
        if (check_suspensory_camera("OV8858") != 1) {
            return -ENODEV;
        }
        ov8858_check = true;
    }

	if (!camera_timing_is_match(0)) {
		print_error("%s: sensor timing don't match.\n", __func__);
		return -ENODEV;
	}

	if (ov8858_sensor.owner && !try_module_get(ov8858_sensor.owner)) {
		print_error("%s: try_module_get fail", __func__);
		return -ENOENT;
	}

    /* 1. M_CAMERA_IO_VDD and S_CAMERA_IO_VDD have the same power.
       2. Only power on one sensor cause leakage, so pri-sensor and sec-sensor
          power on at the same time. Then pull down sec-sensor's PWDN/RESET GPIO for low power.
    */
	if(E_CAMERA_SENSOR_FLIP_TYPE_H_V == get_primary_sensor_flip_type())
		ov8858_sensor.sensor_rgb_type = SENSOR_RGGB;
	else
		ov8858_sensor.sensor_rgb_type =  SENSOR_BGGR;
	ov8858_sensor.old_flip = 0;
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
 * FunctionName: ov8858_exit;
 * Description : ov8858 exit function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void ov8858_exit(void)
{
	print_debug("enter %s", __func__);

	k3_ispio_power_deinit();

	if (ov8858_sensor.owner) {
		module_put(ov8858_sensor.owner);
	}
	print_debug("exit %s", __func__);
}

/*
 **************************************************************************
 * FunctionName: ov8858_shut_down;
 * Description : ov8858 shut down function;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void ov8858_shut_down(void)
{
	print_debug("enter %s", __func__);
	k3_ispgpio_power_sensor(&ov8858_sensor, POWER_OFF);
}

void ov8858_get_flash_awb(flash_platform_t type, awb_gain_t *flash_awb)
{
	*flash_awb = flash_platform_awb[0];
	print_info("ov8858_get_flash_awb: type 0x%x,", type);
}

#ifdef OV8858_OTP
otp_group_index ov8858_check_otp_item(u16 reg_addr)
{
	otp_group_index group_index = OTP_GROUP_INVALID;
	u8 buf  = 0xff;
	u8 flag = 0;

	ov8858_write_reg(0x3d84, 0xc0, 0x00); //program disable, manual mode
	ov8858_write_reg(0x3d88, (reg_addr>>8),0x00);//partial mode OTP write start address
	ov8858_write_reg(0x3d89, (reg_addr&0xff),0x00);
	ov8858_write_reg(0x3d8A, (reg_addr>>8),0x00);//partial mode OTP write end address
	ov8858_write_reg(0x3d8B, (reg_addr&0xff),0x00);
	ov8858_write_reg(0x3d81, 0x01,0x00); //read otp func enable
	mdelay(5);

	ov8858_read_reg(reg_addr, &buf);

	flag = (buf>>2) & 0x03;
	if(0x01 == flag){
		group_index = OTP_GROUP_THREE_VALID;
		goto GROUP_SUCCESS;
	}else if(0x10 == flag || 0x11 == flag){
		goto GROUP_FAIL;
	}

	flag = (buf>>4) & 0x03;
	if(0x01 == flag){
		group_index = OTP_GROUP_TWO_VALID;
		goto GROUP_SUCCESS;
	}else if(0x10 == flag || 0x11 == flag){
		goto GROUP_FAIL;
	}

	flag = (buf>>6) & 0x03;
	if(0x01 == flag){
		group_index = OTP_GROUP_ONE_VALID;
		goto GROUP_SUCCESS;
	}else{
		goto GROUP_FAIL;
	}

GROUP_SUCCESS:
	print_info("ov8858_get_otp_from_sensor() ~ %s reg_addr=0x%x, regVal=0x%x, group_index=%d", __func__, reg_addr, buf, group_index);
	return group_index;

GROUP_FAIL:
	print_info("ov8858_get_otp_from_sensor() ~ %s fail  reg_addr=0x%x, regVal=0x%x, group_index=%d", __func__, reg_addr, buf, group_index);
	return OTP_GROUP_INVALID;
}

static bool ov8858_check_otp_validation()
{
	otp_group_index group_index = OTP_GROUP_INVALID;

	/*This part for ID validation check*/
	group_index = ov8858_check_otp_item(0x7010);
	if(OTP_GROUP_ONE_VALID == group_index){
		id_otp_group_index = OTP_GROUP_ONE_VALID;
	}else if(OTP_GROUP_TWO_VALID == group_index){
		id_otp_group_index = OTP_GROUP_TWO_VALID;
	}else if(OTP_GROUP_THREE_VALID == group_index){
		id_otp_group_index = OTP_GROUP_THREE_VALID;
	}else{
		goto OTP_FAIL;
	}

	/*This part for AWB validation check*/
	group_index = ov8858_check_otp_item(0x7020);
	if(OTP_GROUP_ONE_VALID == group_index){
		awb_otp_group_index = OTP_GROUP_ONE_VALID;
	}else if(OTP_GROUP_TWO_VALID == group_index){
		awb_otp_group_index = OTP_GROUP_TWO_VALID;
	}else if(OTP_GROUP_THREE_VALID == group_index){
		awb_otp_group_index = OTP_GROUP_THREE_VALID;
	}else{
		goto OTP_FAIL;
	}

	/*This part for LSC validation check*/
	group_index = ov8858_check_otp_item(0x7033);
	if(OTP_GROUP_ONE_VALID == group_index){
		lsc_otp_group_index = OTP_GROUP_ONE_VALID;
	}else if(OTP_GROUP_TWO_VALID == group_index){
		lsc_otp_group_index = OTP_GROUP_TWO_VALID;
	}else if(OTP_GROUP_THREE_VALID == group_index){
		lsc_otp_group_index = OTP_GROUP_THREE_VALID;
	}else{
		goto OTP_FAIL;
	}

	/*This part for VCM validation check*/
	group_index = ov8858_check_otp_item(0x717e);
	if(OTP_GROUP_ONE_VALID == group_index){
		vcm_otp_group_index = OTP_GROUP_ONE_VALID;
	}else if(OTP_GROUP_TWO_VALID == group_index){
		vcm_otp_group_index = OTP_GROUP_TWO_VALID;		
	}else if(OTP_GROUP_THREE_VALID == group_index){
		vcm_otp_group_index = OTP_GROUP_THREE_VALID;
	}else{
		goto OTP_FAIL;
	}

	return true;//all have been checked and valid

OTP_FAIL:
	ov8858_otp_flag |= OV8858_OTP_FAIL_FLAG;
	return false;//eithe or all is invalid
}

static void ov8858_otp_read_id(void)
{
	u16 i = 0;
	u16 address_start = 0;
	u16 address_end = 0;
	u8  buf[5]= {0};
	u8  vendor_id =0;
	u8  module_type = MODULE_UNSUPPORT;

	print_info("%s enters",__func__);
	//read otp into buffer
	ov8858_write_reg(0x3d84, 0xc0, 0x00);// program disable, manual mode

	//select group
	if(OTP_GROUP_ONE_VALID == id_otp_group_index){
		address_start = 0x7011;
		address_end   = 0x7015;
	}
	else if(OTP_GROUP_TWO_VALID == id_otp_group_index){
		address_start = 0x7016;
		address_end   = 0x701a;
	}else{//OTP_GROUP_THREE_VALID == id_otp_group_index
		address_start = 0x701b;
		address_end   = 0x701f;
	}

	ov8858_write_reg(0x3d88, (address_start>>8), 0x00);	//partial mode OTP write start address
	ov8858_write_reg(0x3d89, (address_start & 0xff), 0x00);
	ov8858_write_reg(0x3d8A, (address_end>>8), 0x00);	// partial mode OTP write end address
	ov8858_write_reg(0x3d8B, (address_end & 0xff), 0x00);
	ov8858_write_reg(0x3d81, 0x01, 0x00);
	mdelay(5);

	for(i=0; i<=address_end-address_start; i++){//read reg value
		ov8858_read_reg(address_start+i,&buf[i]);
	}

	for (i=address_start; i<=address_end; i++){//clear otp buffer
		ov8858_write_reg(i, 0x00,0x00);
	}

	print_info("ov8858_get_otp_from_sensor() ~%s module info year 20%02d month %d day %d, SNO. 0x%x  vendor id&version 0x%x\n",__func__,
		buf[0],buf[1],buf[2],buf[3],buf[4]);

	vendor_id = buf[4]>>4 & 0x0F;
	if((vendor_id==0x02) && (buf[3]==0x8F)){//FOXCONN
		ov8858_otp_flag |= OV8858_OTP_ID_VALID;	
	}

	return;
}

static void ov8858_otp_read_awb(void)
{
	u16  i = 0;
	u16 address_start = 0;
	u16 address_end = 0;
	u8  buf[6] = {0};

	//read otp into buffer
	ov8858_write_reg(0x3d84, 0xc0, 0x00);// program disable, manual mode

	//select group
	if(OTP_GROUP_ONE_VALID == awb_otp_group_index){
		address_start = 0x7021;
		address_end   = 0x7026;
	}else if(OTP_GROUP_TWO_VALID == awb_otp_group_index){
		address_start = 0x7027;
		address_end   = 0x702C;
	}else{//OTP_GROUP_THREE_VALID == awb_otp_group_index
		address_start = 0x702D;
		address_end   = 0x7032;
	}

	ov8858_write_reg(0x3d88, (address_start>>8), 0x00);//partial mode OTP write start address
	ov8858_write_reg(0x3d89, (address_start & 0xff), 0x00);
	ov8858_write_reg(0x3d8A, (address_end>>8), 0x00);// partial mode OTP write end address
	ov8858_write_reg(0x3d8B, (address_end & 0xff), 0x00);
	ov8858_write_reg(0x3d81, 0x01, 0x00);
	mdelay(5);

	for(i=0; i<=address_end-address_start; i++){//read reg value
		ov8858_read_reg(address_start+i,&buf[i]);
	}

	for (i=address_start; i<=address_end; i++){//clear otp buffer
		ov8858_write_reg(i, 0x00,0x00);
	}

    ov8858_otp.rg_ratio = buf[0];
	ov8858_otp.rg_ratio <<= 8;
	ov8858_otp.rg_ratio += buf[1];
	ov8858_otp.bg_ratio = buf[2];
	ov8858_otp.bg_ratio <<= 8;
	ov8858_otp.bg_ratio += buf[3];
	ov8858_otp.grgb     = buf[4];
	ov8858_otp.grgb     <<= 8;
	ov8858_otp.grgb     += buf[5];

	print_info("ov8858_get_otp_from_sensor() ~ %s rg_ratio=0x%x, bg_ratio=0x%x, grgb=0x%x\n",__func__, ov8858_otp.rg_ratio, ov8858_otp.bg_ratio, ov8858_otp.grgb);

	if(0 == ov8858_otp.rg_ratio || 0 == ov8858_otp.bg_ratio || 0 == ov8858_otp.grgb){
		ov8858_otp_flag |= OV8858_OTP_FAIL_FLAG;
	}

	ov8858_otp_flag |= OV8858_OTP_AWB_VALID;

	return;
}


static void ov8858_otp_set_awb(void)
{
	u16 R_gain,G_gain,B_gain,G_gain_B,G_gain_R;//0x400 = 1x gain

	if(ov8858_otp.bg_ratio < BG_Ratio_Typical) {
		if (ov8858_otp.rg_ratio < RG_Ratio_Typical) {
			G_gain = 0x400;
			B_gain = 0x400 * BG_Ratio_Typical / ov8858_otp.bg_ratio;
			R_gain = 0x400 * RG_Ratio_Typical / ov8858_otp.rg_ratio;
		}else{
			R_gain = 0x400;
			G_gain = 0x400 * ov8858_otp.rg_ratio / RG_Ratio_Typical;
			B_gain = G_gain * BG_Ratio_Typical / ov8858_otp.bg_ratio;
		}
	}else{
		if (ov8858_otp.rg_ratio < RG_Ratio_Typical) {
			B_gain = 0x400;
			G_gain = 0x400 * ov8858_otp.bg_ratio / BG_Ratio_Typical;
			R_gain = G_gain * RG_Ratio_Typical / ov8858_otp.rg_ratio;
		}else {
			G_gain_B = 0x400 * ov8858_otp.bg_ratio / BG_Ratio_Typical;
			G_gain_R = 0x400 * ov8858_otp.rg_ratio / RG_Ratio_Typical;
			if(G_gain_B > G_gain_R ){
				B_gain = 0x400;
				G_gain = G_gain_B;
				R_gain = G_gain * RG_Ratio_Typical / ov8858_otp.rg_ratio;
			}else {
				R_gain = 0x400;
				G_gain = G_gain_R;
				B_gain = G_gain * BG_Ratio_Typical / ov8858_otp.bg_ratio;
			}
		}
	}

	print_info("ov8858_get_otp_from_sensor() ~ %s R_gain=0x%x, B_gain=0x%x, G_gain=0x%x\n",__func__, R_gain, B_gain, G_gain);

	if (R_gain>0x400){//R_gain, sensor red gain of AWB, 0x400 =1
		ov8858_write_reg(0x5032, R_gain>>8, 0x00);
		ov8858_write_reg(0x5033, R_gain & 0x00ff, 0x00);
	}
	if (G_gain>0x400){//G_gain, sensor green gain of AWB, 0x400 =1
		ov8858_write_reg(0x5034, G_gain>>8, 0x00);
		ov8858_write_reg(0x5035, G_gain & 0x00ff, 0x00);
	}
	if (B_gain>0x400){//B_gain, sensor blue gain of AWB, 0x400 =1
		ov8858_write_reg(0x5036, B_gain>>8, 0x00);
		ov8858_write_reg(0x5037, B_gain & 0x00ff, 0x00);
	}

	return;
}

static void ov8858_otp_read_lsc(void)
{
	u16  i = 0;
	u16 address_start = 0;
	u16 address_end = 0;

	memset(ov8858_otp.lsc, 0, sizeof(ov8858_otp.lsc));

	//read otp into buffer
	ov8858_write_reg(0x3d84, 0xc0, 0x00);// program disable, manual mode

	//select group
	if(OTP_GROUP_ONE_VALID == lsc_otp_group_index){
		address_start = 0x7034;
		address_end   = 0x70a1;
	}
	else if(OTP_GROUP_TWO_VALID == lsc_otp_group_index){
		address_start = 0x70a2;
		address_end   = 0x710f;
	}else{//OTP_GROUP_THREE_VALID == lsc_otp_group_index
		address_start = 0x7110;
		address_end   = 0x717d;
	}

	ov8858_write_reg(0x3d88, (address_start>>8), 0x00);//partial mode OTP write start address
	ov8858_write_reg(0x3d89, (address_start & 0xff), 0x00);
	ov8858_write_reg(0x3d8A, (address_end>>8), 0x00);// partial mode OTP write end address
	ov8858_write_reg(0x3d8B, (address_end & 0xff), 0x00);
	ov8858_write_reg(0x3d81, 0x01, 0x00);
	mdelay(5);

	for(i=0; i<=address_end-address_start; i++){//read reg value
		ov8858_read_reg(address_start+i,&ov8858_otp.lsc[i]);
	}

	for (i=address_start; i<=address_end; i++){//clear otp buffer
		ov8858_write_reg(i, 0x00,0x00);
	}

	ov8858_otp_flag |= OV8858_OTP_LSC_VALID;

	print_info("ov8858_get_otp_from_sensor() ~ %s lsc_start=0x%x, lsc_end=0x%x \n",__func__, ov8858_otp.lsc[0], ov8858_otp.lsc[address_end-address_start]);	

	return;	
}

static void ov8858_otp_set_lsc(void)
{
	u8 i = 0;
	u8 temp = 0;

	ov8858_read_reg(0x5000, &temp);
	temp = 0x80 | temp;
	ov8858_write_reg(0x5000, temp, 0x00);

	print_info("ov8858_get_otp_from_sensor() ~ %s reg0x5000val|0x80=0x%x \n",__func__, temp);

	for(;i<OV8858_OTP_LSC_SIZE;i++) {
		ov8858_write_reg(0x5800+i, ov8858_otp.lsc[i], 0x00);	
	}

	return;
}

static void ov8858_otp_read_vcm(void)
{
	u16 i = 0;
	u16 address_start = 0;
	u16 address_end = 0;
	u8  buf[4] = {0};

	//read otp into buffer
	ov8858_write_reg(0x3d84, 0xc0, 0x00);// program disable, manual mode

	//select group
	if(OTP_GROUP_ONE_VALID == vcm_otp_group_index){
		address_start = 0x717f;
		address_end   = 0x7182;
	}
	else if(OTP_GROUP_TWO_VALID == vcm_otp_group_index){
		address_start = 0x7183;
		address_end   = 0x7186;
	}else{//OTP_GROUP_THREE_VALID == vcm_otp_group_index
		address_start = 0x7187;
		address_end   = 0x718a;
	}

	ov8858_write_reg(0x3d88, (address_start>>8), 0x00);//partial mode OTP write start address
	ov8858_write_reg(0x3d89, (address_start & 0xff), 0x00);
	ov8858_write_reg(0x3d8A, (address_end>>8), 0x00);// partial mode OTP write end address
	ov8858_write_reg(0x3d8B, (address_end & 0xff), 0x00);
	ov8858_write_reg(0x3d81, 0x01, 0x00);
	mdelay(5);

	for(i=0; i<=address_end-address_start; i++){//read reg value
		ov8858_read_reg(address_start+i,&buf[i]);
	}

	for (i=address_start; i<=address_end; i++){//clear otp buffer
		ov8858_write_reg(i, 0x00,0x00);
	}

    ov8858_otp.vcm_start= buf[0];
	ov8858_otp.vcm_start<<= 2;
	ov8858_otp.vcm_start+= (buf[1]&0xc0)>>6;
	ov8858_otp.vcm_end= buf[2];
	ov8858_otp.vcm_end<<= 2;
	ov8858_otp.vcm_end+= (buf[3]&0xc0)>>6;

	//print_info("%s, buf[0]=0x%x, buf[1]=0x%x,buf[2]=0x%x,buf[3]=0x%x \n",__func__, buf[0], buf[1],buf[2],buf[3]);
	print_info("ov8858_get_otp_from_sensor() ~ %s vcm_start=0x%x, vcm_end=0x%x \n",__func__, ov8858_otp.vcm_start, ov8858_otp.vcm_end);	
	
	if((ov8858_otp.vcm_start != ov8858_otp.vcm_end) &&(ov8858_otp.vcm_end>ov8858_otp.vcm_start) &&(ov8858_otp.vcm_start!=0) &&(ov8858_otp.vcm_end!=0)) {
		ov8858_otp_flag |= OV8858_OTP_VCM_VALID;//just use ov8858_otp.vcm_start/ov8858_otp.vcm_end value 0 to achieve
	} else {//abnormal branch deal
		print_error("%s VCM OTP data is worng!!!\n",__func__);
		ov8858_otp_flag |= OV8858_OTP_FAIL_FLAG;
		ov8858_otp.vcm_start = 0;
		ov8858_otp.vcm_end = 0;
	}

	return;
}

/****************************************************************************
* FunctionName: ov8858_otp_get_vcm;
* Description : Get vcm start and stop parameters read from OTP.;
* Input       : NA;
* Output      : vcm_start vcm_end
* ReturnValue : NA;
* Other       : NA;
***************************************************************************/
static void ov8858_otp_get_vcm(u16 *vcm_start, u16 *vcm_end)
{
	if (0 == ov8858_otp.vcm_start){
		*vcm_start = ov8858_sensor.vcm->infiniteDistance;
	} else {
        if (ov8858_otp.vcm_start > ov8858_sensor.vcm->startCurrentOffset)
            *vcm_start = ov8858_otp.vcm_start - ov8858_sensor.vcm->startCurrentOffset;
        else
            *vcm_start = 0;
	}

	if (0 == ov8858_otp.vcm_end){
		*vcm_end = ov8858_sensor.vcm->normalDistanceEnd;
	} else {
		*vcm_end = ov8858_otp.vcm_end;
	}

	print_info("ov8858_get_otp_from_sensor() ~ %s vcm_start: %#x, vcm_end: %#x", __func__, *vcm_start, *vcm_end);
}

static bool ov8858_get_otp_from_sensor(void)
{
	bool retVal = false;
	int tmp = 0x00;
#ifdef OV8858_OTP_READ_TIME_PRINT
	u16 otpReadUsed = 0;
	struct timeval otpReadBegin, otpReadEnd;
#endif

	print_info("Enter %s \n", __func__);
	if((ov8858_otp_flag & OV8858_OTP_FAIL_FLAG) == OV8858_OTP_FAIL_FLAG) {
		print_error("%s fails to read OTP already\n", __func__);	
		return false;
	}else if((ov8858_otp_flag & OV8858_OTP_SUCCESS_FLAG) == OV8858_OTP_SUCCESS_FLAG){//otp just excute once, no need repeat
		print_info("%s has been read success already!\n", __func__);	
		return true;
	}

#ifdef OV8858_OTP_READ_TIME_PRINT
	memset(&otpReadBegin,0,sizeof(otpReadBegin));
	memset(&otpReadEnd,0,sizeof(otpReadEnd));
	do_gettimeofday(&otpReadBegin);
#endif

	memset(&ov8858_otp, 0, sizeof(ov8858_otp));

	//set 0x5002[3] to 0, OTP_DPC function disable
       ov8858_read_reg(0x5002, &tmp);
	ov8858_write_reg(0x5002,((0x00 & 0x08) | tmp & (~0x08)), 0x00);

	retVal = ov8858_check_otp_validation();
	if((false == retVal) || (ov8858_otp_flag & OV8858_OTP_FAIL_FLAG) == OV8858_OTP_FAIL_FLAG){
		print_error("%s ov8858_check_otp_validation() fails!\n", __func__);
		goto Error;
	}

	ov8858_otp_read_id();
	if((ov8858_otp_flag & OV8858_OTP_ID_VALID) != OV8858_OTP_ID_VALID){
		print_error("%s ov8858_otp_read_id() id mismatch\n", __func__);	
		goto Error;
	}

	ov8858_otp_read_awb();
	if((ov8858_otp_flag & OV8858_OTP_FAIL_FLAG) == OV8858_OTP_FAIL_FLAG){
		print_error("%s ov8858_otp_read_awb() awb error with zero!!!\n", __func__);	
		goto Error;
	}
	
	ov8858_otp_read_lsc();//no method to check data validation

	ov8858_otp_read_vcm();
	if((ov8858_otp_flag & OV8858_OTP_FAIL_FLAG) == OV8858_OTP_FAIL_FLAG){
		print_error("%s ov8858_otp_read_vcm() vcm error!!!\n", __func__);	
		goto Error;
	}

	ov8858_otp_flag |= OV8858_OTP_SUCCESS_FLAG;

#ifdef OV8858_OTP_READ_TIME_PRINT
	do_gettimeofday(&otpReadEnd);
	otpReadUsed = (otpReadEnd.tv_sec - otpReadBegin.tv_sec) * 1000 + (otpReadEnd.tv_usec - otpReadBegin.tv_usec) / 1000;
	print_info("%s used time is %d ms\n", __func__,otpReadUsed);
#endif

       //set 0x5002[3] to 1, OTP_DPC function enable
       ov8858_read_reg(0x5002, &tmp);
	ov8858_write_reg(0x5002,((0x08 & 0x08) | tmp & (~0x08)), 0x00);

	print_info("%s ov8858_otp_flag=0x%x \n", __func__,ov8858_otp_flag);
	return true;

Error:
	//set 0x5002[3] to 1, OTP_DPC function enable
       ov8858_read_reg(0x5002, &tmp);
	ov8858_write_reg(0x5002,((0x08 & 0x08) | tmp & (~0x08)), 0x00);

	return false;
}
#endif
/*
 **************************************************************************
 * FunctionName: ov8858_set_default;
 * Description : init ov8858_sensor;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
*/
static void ov8858_set_default(void)
{
    ov8858_sensor.init = ov8858_init;
	ov8858_sensor.exit = ov8858_exit;
	ov8858_sensor.shut_down = ov8858_shut_down;
	ov8858_sensor.reset = ov8858_reset;
	ov8858_sensor.check_sensor = ov8858_check_sensor;
	ov8858_sensor.power = ov8858_power;
	ov8858_sensor.init_reg = ov8858_init_reg;
	ov8858_sensor.stream_on = ov8858_stream_on;

	ov8858_sensor.get_format = ov8858_get_format;
	ov8858_sensor.set_flash = NULL;
	ov8858_sensor.get_flash = NULL;
	ov8858_sensor.set_scene = NULL;
	ov8858_sensor.get_scene = NULL;

	ov8858_sensor.enum_framesizes = ov8858_enum_framesizes;
	ov8858_sensor.try_framesizes = ov8858_try_framesizes;
	ov8858_sensor.set_framesizes = ov8858_set_framesizes;
	ov8858_sensor.get_framesizes = ov8858_get_framesizes;

	ov8858_sensor.enum_frame_intervals = ov8858_enum_frame_intervals;
	ov8858_sensor.try_frame_intervals = NULL;
	ov8858_sensor.set_frame_intervals = NULL;
	ov8858_sensor.get_frame_intervals = NULL;

	ov8858_sensor.get_vts_reg_addr = ov8858_get_vts_reg_addr;

	ov8858_sensor.get_capability = ov8858_get_capability;

	ov8858_sensor.set_hflip = ov8858_set_hflip;
	ov8858_sensor.get_hflip = ov8858_get_hflip;
	ov8858_sensor.set_vflip = ov8858_set_vflip;
	ov8858_sensor.get_vflip = ov8858_get_vflip;
	ov8858_sensor.update_flip = ov8858_update_flip;

	ov8858_sensor.sensor_hw_3a.support_awb_otp = ov8858_support_awb_otp;

	strncpy(ov8858_sensor.info.name,"ov8858",sizeof(ov8858_sensor.info.name));
	ov8858_sensor.interface_type = MIPI1;
	ov8858_sensor.mipi_lane_count = CSI_LINES_4;
	ov8858_sensor.mipi_index = CSI_INDEX_0;
	ov8858_sensor.sensor_index = CAMERA_SENSOR_PRIMARY;
	ov8858_sensor.skip_frames = 2;

	ov8858_sensor.power_conf.pd_valid = LOW_VALID;
	ov8858_sensor.power_conf.reset_valid = LOW_VALID;
	ov8858_sensor.power_conf.vcmpd_valid = LOW_VALID;

	ov8858_sensor.i2c_config.index = I2C_PRIMARY;
	ov8858_sensor.i2c_config.speed = I2C_SPEED_400;
	ov8858_sensor.i2c_config.addr = OV8858_SLAVE_ADDRESS;
	ov8858_sensor.i2c_config.addr_bits = I2C_16BIT;
	ov8858_sensor.i2c_config.val_bits = I2C_8BIT;

	ov8858_sensor.preview_frmsize_index = 0;
	ov8858_sensor.capture_frmsize_index = 0;
	ov8858_sensor.frmsize_list = ov8858_framesizes;
	ov8858_sensor.fmt[STATE_PREVIEW] = V4L2_PIX_FMT_RAW10;
	ov8858_sensor.fmt[STATE_CAPTURE] = V4L2_PIX_FMT_RAW10;

#ifdef OV8858_AP_WRITEAE_MODE /* just an example and test case for AP write AE mode */
	ov8858_sensor.aec_addr[0] = 0;
	ov8858_sensor.aec_addr[1] = 0;
	ov8858_sensor.aec_addr[2] = 0;
	ov8858_sensor.agc_addr[0] = 0;
	ov8858_sensor.agc_addr[1] = 0;
	ov8858_sensor.ap_writeAE_delay = 6000; /* 5 expo and gain registers, 6000 us is enough */
#else
	ov8858_sensor.aec_addr[0] = 0x3500; 
	ov8858_sensor.aec_addr[1] = 0x3501;
	ov8858_sensor.aec_addr[2] = 0x3502;
	ov8858_sensor.agc_addr[0] = 0x3508;
	ov8858_sensor.agc_addr[1] = 0x3509;
#endif
	ov8858_sensor.sensor_type = SENSOR_OV;
	ov8858_sensor.sensor_rgb_type = SENSOR_BGGR;
	ov8858_sensor.set_gain = ov8858_set_gain;
	ov8858_sensor.set_exposure = ov8858_set_exposure;
	ov8858_sensor.set_exposure_gain = NULL;//ov8858_set_exposure_gain
	ov8858_sensor.set_vts = ov8858_set_vts;

	ov8858_sensor.get_override_param = NULL;

	ov8858_sensor.sensor_gain_to_iso = ov8858_gain_to_iso;
	ov8858_sensor.sensor_iso_to_gain = ov8858_iso_to_gain;

	ov8858_sensor.get_sensor_aperture = ov8858_get_sensor_aperture;
	ov8858_sensor.get_equivalent_focus = NULL;

	ov8858_sensor.set_effect = NULL;

	ov8858_sensor.isp_location = CAMERA_USE_K3ISP;
	ov8858_sensor.sensor_tune_ops = NULL;

	ov8858_sensor.af_enable = 1;
	ov8858_sensor.get_flash_awb = ov8858_get_flash_awb;

	ov8858_sensor.image_setting.lensc_param = NULL;
	ov8858_sensor.image_setting.ccm_param = NULL;
	ov8858_sensor.image_setting.awb_param = NULL;
	ov8858_sensor.fps_max = 30;
	ov8858_sensor.fps_min = 15;
	ov8858_sensor.fps = 25;
	ov8858_sensor.owner = THIS_MODULE;

	ov8858_sensor.info.facing = CAMERA_FACING_BACK;
	ov8858_sensor.info.orientation = 270;
	ov8858_sensor.info.focal_length = 287;/* 2.872mm for sunny, foxconn cannot achieve by module schematic*/
	ov8858_sensor.info.h_view_angle = 75;
	ov8858_sensor.info.v_view_angle = 75;
	ov8858_sensor.lane_clk = CLK_750M;
	ov8858_sensor.hflip = 0;
	ov8858_sensor.vflip = 0;
	ov8858_sensor.old_flip = 0;

	ov8858_sensor.get_gain     				= ov8858_get_gain;
	ov8858_sensor.get_exposure   			= ov8858_get_exposure;
	ov8858_sensor.init_isp_reg 				= ov8858_init_isp_reg;
	ov8858_sensor.support_summary = false;
    ov8858_sensor.get_ccm_pregain = NULL;
    ov8858_sensor.set_awb = NULL;
    ov8858_sensor.set_anti_banding = NULL;
    ov8858_sensor.update_framerate = NULL;
    ov8858_sensor.awb_dynamic_ccm_gain = NULL;
    ov8858_sensor.pclk = 0;
    ov8858_sensor.max_int_lines = 0;
    ov8858_sensor.min_int_lines = 0;
    ov8858_sensor.real_int_lines = 0;
    ov8858_sensor.min_gain = 0;
    ov8858_sensor.max_gain = 0;
    ov8858_sensor.real_gain = 0;
    ov8858_sensor.rcc_enable = false;
    ov8858_sensor.isp_idi_skip = false;
}

/*
 **************************************************************************
 * FunctionName: ov8858_module_init;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static __init int ov8858_module_init(void)
{
	ov8858_set_default();
	return register_camera_sensor(ov8858_sensor.sensor_index, &ov8858_sensor);
}

/*
 **************************************************************************
 * FunctionName: ov8858_module_exit;
 * Description : NA;
 * Input       : NA;
 * Output      : NA;
 * ReturnValue : NA;
 * Other       : NA;
 **************************************************************************
 */
static void __exit ov8858_module_exit(void)
{
	unregister_camera_sensor(ov8858_sensor.sensor_index, &ov8858_sensor);
}

MODULE_AUTHOR("Hisilicon");
module_init(ov8858_module_init);
module_exit(ov8858_module_exit);
MODULE_LICENSE("GPL");

