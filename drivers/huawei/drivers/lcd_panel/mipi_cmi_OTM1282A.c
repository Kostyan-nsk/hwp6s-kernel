/* Copyright (c) 2008-2011, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *	 * Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	 * Redistributions in binary form must reproduce the above
 *	   copyright notice, this list of conditions and the following
 *	   disclaimer in the documentation and/or other materials provided
 *	   with the distribution.
 *	 * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *	   contributors may be used to endorse or promote products derived
 *	   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/pwm.h>
#include <mach/platform.h>
#include <mach/gpio.h>
#include <mach/boardid.h>

#include "edc_reg.h"
#include "k3_fb.h"
#include "k3_fb_def.h"
#include "mipi_dsi.h"
#include "mipi_reg.h"
#include <linux/lcd_tuning.h>
#include "drv_regulator_user.h"
#include <linux/dsm_pub.h>

#define PWM_LEVEL 100
#define EDC_PIXCEL_CLK_RATE                  (CONFIG_EDC_PIXCEL_CLK_RATE)

#define LCD_HW_ID_STATUS_LOW      0x00
#define LCD_HW_ID_STATUS_HIGH     0x01
#define LCD_HW_ID_STATUS_FLOAT   0x02
#define CMI_OTM1282A        0x00
#define AUTO_OTM1282A     0x03

#define TM_NT35590     0x01

#define GPIO_LCD_RESET_NAME	"gpio_lcd_reset"
#define GPIO_LCD_ID0_NAME   "gpio_lcd_id0"
#define GPIO_LCD_ID1_NAME	"gpio_lcd_id1"
#define GPIO_LCD_ENP_NAME   "gpio_lcd_enp"
#define GPIO_LCD_ENN_NAME   "gpio_lcd_enn"
#define GPIO_LCD_PWM_NAME   "gpio_lcd_pwm_en"

#define GPIO_LCD_POWER_NAME	"gpio_lcd_power"
#define AUO_CABC_ENABLED 1
#define CMI_CABC_ENABLED 1
#define TM_CABC_ENABLED 1

extern struct dsm_client *lcd_dclient;

struct cmi_otm1282b_panel_data{
	struct regulator *lcd_vddio;
	struct iomux_block *lcd_gpio_block;
	struct block_config *lcd_gpio_block_config;
	int lcd_gpio_reset;
	int lcd_gpio_enp;
	int lcd_gpio_enn;
	int lcd_gpio_id0;
	int lcd_gpio_id1;
	int lcd_gpio_pwm_en;
};

static struct cmi_otm1282b_panel_data *g_panel_platform_data;
static volatile bool g_display_on;
static volatile bool backlight_log_once = true;
extern u32 frame_count;
static u16 g_panel_id;
/*******************************************************************************
** auto Power ON Sequence(sleep mode to Normal mode)
*/
static char  auto_addr_shift_1[] = {
	0x00,
	0x00,
};
static char  auto_command1[] = {
    0xff,
    0x12,0x82,0x01,
};
static char  auto_addr_shift_2[] = {
	0x00,
	0x80,
};
static char  auto_command2[] = {
    0xff,
    0x12,0x82,
};
static char  auto_addr_shift_3[] = {
	0x00,
	0x80,
};
static char  auto_command3[] = {
    0xc0,
    0x00,0x85,0x00,0x2C,0x2C,0x00,0x7B,0x2C,0x2C,0x00,0x7B,0x00,0x2C,0x2C,
};
static char  auto_addr_shift_4[] = {
	0x00,
	0xa0,
};
static char  auto_command4[] = {
    0xc0,
    0x00,0x00,0x00,0x07,0x00,0x19,0x09,
};
static char  auto_addr_shift_5[] = {
	0x00,
	0xd0,
};
static char  auto_command5[] = {
    0xc0,
    0x00,0x00,0x00,0x07,0x00,0x19,0x09,
};
static char  auto_addr_shift_6[] = {
	0x00,
	0x80,
};
static char  auto_command6[] = {
    0xc1,
    0x66,0x66,
};
static char  auto_addr_shift_7[] = {
	0x00,
	0x90,
};
static char  auto_command7[] = {
    0xc1,
    0x77,0x00,0x00,
};
static char  auto_addr_shift_8[] = {
	0x00,
	0x80,
};
static char  auto_command8[] = {
    0xc2,
    0x83,0x01,0x45,0x45,
};
static char  auto_addr_shift_9[] = {
	0x00,
	0x90,
};
static char  auto_command9[] = {
    0xc2,
    0xA9,0x2C,0x01,0x00,0x00,0xAB,0x2C,0x01,0x00,0x00,0xAA,0x2C,0x01,0x00,0x00,
};
static char  auto_addr_shift_10[] = {
	0x00,
	0xa0,
};
static char  auto_command10[] = {
    0xc2,
    0xA8,0x2C,0x01,0x00,0x00,
};
static char  auto_addr_shift_11[] = {
	0x00,
	0xec,
};
static char auto_command11[] = {
    0xc2,
    0x00,
};
static char  auto_addr_shift_12[] = {
	0x00,
	0xfa,
};
static char  auto_command12[] = {
    0xc2,
    0x00,0x80,0x01,
};
static char  auto_addr_shift_13[] = {
	0x00,
	0x80,
};
static char  auto_command13[] = {
    0xc3,
    0x83,0x01,0x45,0x45,
};
static char  auto_addr_shift_14[] = {
	0x00,
	0x90,
};
static char  auto_command14[] = {
    0xc3,
    0xA9,0x2C,0x01,0x00,0x00,0xAB,0x2C,0x01,0x00,0x00,0xAA,0x2C,0x01,0x00,0x00,
};
static char  auto_addr_shift_15[] = {
	0x00,
	0xa0,
};
static char  auto_command15[] = {
    0xc3,
    0xA8,0x2C,0x01,0x00,0x00,
};
static char  auto_addr_shift_16[] = {
	0x00,
	0xec,
};
static char  auto_command16[] = {
    0xc3,
    0x00,
};
static char  auto_addr_shift_17[] = {
	0x00,
	0x90,
};
static char  auto_command17[] = {
    0xcb,
    0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,
};
static char  auto_addr_shift_18[] = {
	0x00,
	0xa0,
};
static char  auto_command18[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  auto_addr_shift_19[] = {
	0x00,
	0xb0,
};
static char  auto_command19[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x50,0xC0,0x00,0x00,0x00,0x00,
};
static char  auto_addr_shift_20[] = {
	0x00,
	0xb4,
};
static char  auto_command20[] = {
    0xc0,
    0x40,
};
static char  auto_addr_shift_21[] = {
	0x00,
	0xc0,
};
static char  auto_command21[] = {
    0xcb,
    0xFF,0x28,0xEB,0x28,0xEB,0x14,0x14,0x14,0x14,0x00,0x14,0x14,0xD7,0x28,0x28,
};
static char  auto_addr_shift_22[] = {
	0x00,
	0xd0,
};
static char  auto_command22[] = {
    0xcb,
    0x28,0x28,0x14,0x14,0x14,0xD7,0x04,0xF7,0x04,0x14,0x14,0x14,0x14,0x04,0x04,
};
static char  auto_addr_shift_23[] = {
	0x00,
	0xe0,
};
static char  auto_command23[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x15,0x07,0x14,0x14,0x00,0x00,
};
static char  auto_addr_shift_24[] = {
	0x00,
	0xf0,
};
static char  auto_command24[] = {
    0xcb,
    0x30,0x03,0xFC,0x03,0xF0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  auto_addr_shift_25[] = {
	0x00,
	0x80,
};
static char  auto_command25[] = {
    0xcc,
    0x0F,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x10,
};
static char  auto_addr_shift_26[] = {
	0x00,
	0xb0,
};
static char  auto_command26[] = {
    0xcc,
    0x0F,0x06,0x05,0x04,0x03,0x07,0x08,0x09,0x0A,0x10,
};
static char  auto_addr_shift_27[] = {
	0x00,
	0x8a,
};
static char  auto_command27[] = {
    0xcd,
    0x0b,
};
static char  auto_addr_shift_28[] = {
	0x00,
	0xa0,
};
static char  auto_command28[] = {
    0xcd,
    0x12,0x14,0x15,0x04,0x05,0x01,0x0A,0x2D,0x2D,0x2D,0x2D,0x2D,0x26,0x25,0x24,
};
static char  auto_addr_shift_29[] = {
	0x00,
	0xb0,
};
static char  auto_command29[] = {
    0xcd,
    0x23,0x22,0x21,0x20,0x1F,0x2D,0x2D,0x2D,0x2D,0x13,0x0B,0x0C,0x2D,0x2D,0x2D,
};
static char  auto_addr_shift_30[] = {
	0x00,
	0xc0,
};
static char  auto_command30[] = {
    0xcd,
    0x2D,0x2D,0x2D,0x27,0x28,0x29,0x2A,0x2B,0x1D,0x2D,
};
static char  auto_addr_shift_31[] = {
	0x00,
	0xd0,
};
static char  auto_command31[] = {
    0xcd,
    0x12,0x14,0x15,0x02,0x03,0x01,0x0A,0x2D,0x2D,0x2D,0x2D,0x2D,0x26,0x25,0x24,
};
static char  auto_addr_shift_32[] = {
	0x00,
	0xe0,
};
static char  auto_command32[] = {
    0xcd,
    0x23,0x22,0x21,0x20,0x1F,0x2D,0x2D,0x2D,0x2D,0x13,0x0B,0x0C,0x2D,0x2D,0x2D,
};
static char  auto_addr_shift_33[] = {
	0x00,
	0xf0,
};
static char  auto_command33[] = {
    0xcd,
    0x2D,0x2D,0x2D,0x27,0x28,0x29,0x2A,0x2B,0x1D,0x2D,
};
static char  auto_addr_shift_34[] = {
	0x00,
	0x93,
};
static char  auto_command34[] = {
    0xf5,
    0x10,
};
static char  auto_addr_shift_35[] = {
	0x00,
	0x90,
};
static char  auto_command35[] = {
    0xc5,
    0x92,0xD6,0xAD,0xB0,
};
static char  auto_addr_shift_36[] = {
	0x00,
	0xa0,
};
static char  auto_command36[] = {
    0xc5,
    0x92,0xD6,0xAD,0xB0,
};
static char  auto_addr_shift_37[] = {
	0x00,
	0x00,
};
static char  auto_command37[] = {
    0xd8,
    0x34,0x34,
};
static char  auto_addr_shift_38[] = {
	0x00,
	0x00,
};
static char  auto_command38[] = {
    0xe1,
    0x55,0x57,0x5A,0x5E,0x62,0x67,0x6E,0x7A,0x83,0x8F,0x97,0x9C,0x60,0x5D,0x5A,0x50,0x41,0x32,0x28,0x22,0x1A,0x14,0x11,0x0F,
};
static char  auto_addr_shift_39[] = {
	0x00,
	0x00,
};
static char  auto_command39[] = {
    0xe2,
    0x55,0x57,0x5A,0x5E,0x62,0x67,0x6E,0x7A,0x83,0x8F,0x97,0x9C,0x60,0x5D,0x5A,0x50,0x41,0x32,0x28,0x22,0x1A,0x14,0x11,0x0F,
};
static char  auto_addr_shift_40[] = {
	0x00,
	0x00,
};
static char  auto_command40[] = {
    0xe3,
    0x55,0x57,0x5A,0x5E,0x62,0x67,0x6E,0x7A,0x83,0x8F,0x97,0x9C,0x60,0x5D,0x5A,0x50,0x41,0x32,0x28,0x22,0x1A,0x14,0x11,0x0F,
};
static char  auto_addr_shift_41[] = {
	0x00,
	0x00,
};
static char  auto_command41[] = {
    0xe4,
    0x55,0x57,0x5A,0x5E,0x62,0x67,0x6E,0x7A,0x83,0x8F,0x97,0x9C,0x60,0x5D,0x5A,0x50,0x41,0x32,0x28,0x22,0x1A,0x14,0x11,0x0F,
};
static char  auto_addr_shift_42[] = {
	0x00,
	0x00,
};
static char  auto_command42[] = {
    0xe5,
    0x2A,0x38,0x40,0x4A,0x52,0x59,0x64,0x74,0x7E,0x8D,0x96,0x9C,0x5F,0x5D,0x5A,0x50,0x41,0x33,0x2A,0x24,0x1D,0x16,0x11,0x0F,
};
static char  auto_addr_shift_43[] = {
	0x00,
	0x00,
};
static char  auto_command43[] = {
    0xe6,
    0x2A,0x38,0x40,0x4A,0x52,0x59,0x64,0x74,0x7E,0x8D,0x96,0x9C,0x5F,0x5D,0x5A,0x50,0x41,0x33,0x2A,0x24,0x1D,0x16,0x11,0x0F,
};
//Digital Gamma Red- setting
static char  auto_command_ec[] = {
    0xec,
    0x40,0x34,0x44,0x34,0x44,0x34,0x44,0x34,0x44,0x44,0x43,0x44,0x43,0x44,0x43,0x44,0x34,0x44,0x34,0x44,0x34,0x44,0x34,0x44,0x44,0x43,0x44,0x43,0x44,0x43,0x44,0x43,0x04,
};
//Digital Gamma Green- setting
static char  auto_command_ed[] = {
    0xed,
    0x40,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x44,0x43,0x34,0x44,0x34,0x44,0x43,0x34,0x04,
};
//Digital Gamma Blue- setting
static char  auto_command_ee[] = {
    0xee,
    0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x04,
};
static char  auto_addr_shift_44[] = {
	0x00,
	0xb0,
};
static char  auto_command44[] = {
    0xca,
    0x02,
};
static char  auto_addr_shift_45[] = {
	0x00,
	0x80,
};
static char  auto_command45[] = {
    0xff,
    0x00,0x00,
};
static char  auto_addr_shift_46[] = {
	0x00,
	0x00,
};
static char  auto_command46[] = {
    0xff,
    0x00,0x00,0x00,
};
static char  auto_bl_enable[] = {
    0x53,
    0x24,
};
static char  auto_bl_mode[] = {
    0x55,
//to optimize the CABC function
#if AUO_CABC_ENABLED    
    0x01,   //to enable CABC when LCD panel on
#else
    0x00,
#endif
};
static char  auto_te_enable[] = {
    0x35,
    0x00,
};
static char  auto_exit_sleep[] = {
    0x11,
    0x00,
};
static char  auto_display_on[] = {
    0x29,
    0x00,
};
static char  auto_all_pixels_off[] = {
	0x22,    
};
static char auto_normal_display_on[] = {
	0x13,  
};
/*******************************************************************************
** auto Power OFF Sequence(Normal to power off)
*/
static char auto_bl_level_0[] = {
	0x51,
	0x00,
};
static char auto_display_off[] = {
	0x28,
};
static char auto_enter_sleep[] = {
	0x10,
};

/*******************************************************************************
** auo CABC Sequence
*/
#if AUO_CABC_ENABLED
//enable cmd 2
static char  auto_cmd_enable_1[] = {
	0xff,
	0x12, 0x82, 0x01,
};
static char  auto_cmd_enable_2[] = {
	0xff,
	0x12, 0x82,
};
//disable cmd 2
static char  auto_cmd_disable_1[] = {
	0xff,
	0x00, 0x00,
};
static char  auto_cmd_disable_2[] = {
	0xff,
	0x00, 0x00, 0x00,
};
static char  auto_addr_shift_0[] = {
	0x00,
	0x00,
};
static char  auto_addr_shift_80[] = {
	0x00,
	0x80,
};
static char auto_cabc_ca[] = {
	0xCA,
	0x01,0x8e,0x95,0x9d,0xa4,
	0xab,0xb2,0xba,0xc1,0xc8,
	0xcf,0xd7,0xde,0xe5,0xec,
	0xf4,0xe8,0xff,0x87,0xff,
	0x87,0xff,0x53,0x53,0x53,
};
//0x00 0x00
static char auto_cabc_c6_G1[] = {
	0xc6,
	0x10,
};
static char auto_cabc_c7_G1[] = {
	0xC7,
	0x90,0xa9,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0x99,0x88,
	0x88,0x88,0x77,0x66,0x55,
	0x55,0x55,0x55,
};
//0x00 0x00
static char auto_cabc_c6_G2[] = {
	0xc6,
	0x11,
};
static char auto_cabc_c7_G2[] = {
	0xC7,
	0x90,0x99,0xaa,0xaa,0xaa,
	0xaa,0xaa,0x9a,0x99,0x88,
	0x88,0x88,0x77,0x66,0x66,
	0x55,0x55,0x55,
};
//0x00 0x00
static char auto_cabc_c6_G3[] = {
	0xc6,
	0x12,
};
static char auto_cabc_c7_G3[] = {
	0xC7,
	0x90,0x99,0xa9,0xaa,0xaa,
	0xaa,0xaa,0x99,0x99,0x88,
	0x88,0x88,0x77,0x66,0x66,
	0x66,0x55,0x55,
};
//0x00 0x00
static char auto_cabc_c6_G4[] = {
	0xc6,
	0x13,
};
static char auto_cabc_c7_G4[] = {
	0xC7,
	0x90,0x99,0x99,0xaa,0xaa,
	0xaa,0x9a,0x99,0x99,0x88,
	0x88,0x88,0x77,0x66,0x66,
	0x66,0x66,0x55,
};
//0x00 0x00
static char auto_cabc_c6_G5[] = {
	0xc6,
	0x14,
};
static char auto_cabc_c7_G5[] = {
	0xC7,
	0x90,0x99,0x99,0xa9,0xaa,
	0xaa,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x66,0x66,
	0x66,0x66,0x66,
};
//0x00 0x00
static char auto_cabc_c6_G6[] = {
	0xc6,
	0x15,
};
static char auto_cabc_c7_G6[] = {
	0xC7,
	0x90,0x99,0x99,0x99,0xaa,
	0x9a,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x77,0x66,
	0x66,0x66,0x66,
};
//0x00 0x00
static char auto_cabc_c6_G7[] = {
	0xc6,
	0x16,
};
static char auto_cabc_c7_G7[] = {
	0xC7,
	0x90,0x99,0x99,0x99,0xa9,
	0x99,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x77,0x77,
	0x66,0x66,0x66,
};
//0x00 0x00
static char auto_cabc_c6_G8[] = {
	0xc6,
	0x17,
};
static char auto_cabc_c7_G8[] = {
	0xC7,
	0x80,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x77,0x77,
	0x77,0x66,0x66,
};
//0x00 0x00
static char auto_cabc_c6_G9[] = {
	0xc6,
	0x18,
};
static char auto_cabc_c7_G9[] = {
	0xC7,
	0x80,0x98,0x99,0x99,0x99,
	0x98,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x77,0x77,
	0x77,0x77,0x66,
};
//0x00 0x00
static char auto_cabc_c6_G10[] = {
	0xc6,
	0x19,
};
static char auto_cabc_c7_G10[] = {
	0xC7,
	0x80,0x88,0x99,0x99,0x99,
	0x88,0x99,0x99,0x99,0x88,
	0x88,0x88,0x77,0x77,0x77,
	0x77,0x77,0x77,
};
//0x00 0x00
static char auto_cabc_c6_G11[] = {
	0xc6,
	0x1a,
};
static char auto_cabc_c7_G11[] = {
	0xC7,
	0x80,0x88,0x98,0x99,0x99,
	0x88,0x98,0x99,0x99,0x88,
	0x88,0x88,0x88,0x77,0x77,
	0x77,0x77,0x77,
};
//0x00 0x00
static char auto_cabc_c6_G12[] = {
	0xc6,
	0x1b,
};
static char auto_cabc_c7_G12[] = {
	0xC7,
	0x80,0x88,0x88,0x99,0x99,
	0x88,0x88,0x99,0x99,0x88,
	0x88,0x88,0x88,0x88,0x77,
	0x77,0x77,0x77,
};
//0x00 0x00
static char auto_cabc_c6_G13[] = {
	0xc6,
	0x1c,
};
static char auto_cabc_c7_G13[] = {
	0xC7,
	0x80,0x88,0x88,0x98,0x99,
	0x88,0x88,0x98,0x99,0x88,
	0x88,0x88,0x88,0x88,0x88,
	0x77,0x77,0x77,
};
//0x00 0x00
static char auto_cabc_c6_G14[] = {
	0xc6,
	0x1d,
};
static char auto_cabc_c7_G14[] = {
	0xC7,
	0x80,0x88,0x88,0x88,0x99,
	0x88,0x88,0x88,0x99,0x88,
	0x88,0x88,0x88,0x88,0x88,
	0x88,0x77,0x77,
};
//0x00 0x00
static char auto_cabc_c6_G15[] = {
	0xc6,
	0x1e,
};
static char auto_cabc_c7_G15[] = {
	0xC7,
	0x80,0x88,0x88,0x88,0x98,
	0x88,0x88,0x88,0x98,0x88,
	0x88,0x88,0x88,0x88,0x88,
	0x88,0x78,0x78,
};
//0x00 0x00
static char auto_cabc_c6_G16[] = {
	0xc6,
	0x1f,
};
static char auto_cabc_c7_G16[] = {
	0xC7,
	0x80,0x88,0x88,0x88,0x88,
	0x88,0x88,0x88,0x88,0x88,
	0x88,0x88,0x88,0x88,0x88,
	0x88,0x88,0x88,
};

static char auto_cabc_exit_seq_1[] = {
	0x00,
	0x00,
};
static char auto_cabc_exit_seq_2[] = {
	0xc0,
	0x00,
};
static char auto_cabc_exit_seq_3[] = {
	0x00,
	0x00,
};
#endif

/*******************************************************************************
** cmi Power ON Sequence(sleep mode to Normal mode)
*/
static char  cmi_addr_shift_1[] = {
	0x00,
	0x00,
};
static char  cmi_command1[] = {
    0xff,
    0x12,0x82,0x01,
};
static char  cmi_addr_shift_2[] = {
	0x00,
	0x80,
};
static char  cmi_command2[] = {
    0xff,
    0x12,0x82,
};
static char  cmi_addr_shift_3[] = {
	0x00,
	0xb0,
};
static char  cmi_command3[] = {
    0xca,
    0x02,
};
static char  cmi_addr_shift_4[] = {
	0x00,
	0x00,
};
static char  cmi_command4[] = {
    0x1c,
    0x00,
};
static char  cmi_addr_shift_5[] = {
	0x00,
	0xa0,
};
static char  cmi_command5[] = {
    0xc1,
    0x00,0xEE,
};
static char  cmi_addr_shift_6[] = {
	0x00,
	0x80,
};
static char  cmi_command6[] = {
    0xc1,
    0x66,0x66,
};
static char  cmi_addr_shift_7[] = {
	0x00,
	0x90,
};
static char  cmi_command7[] = {
    0xc1,
    0x77,
};
static char  cmi_addr_shift_8[] = {
	0x00,
	0x90,
};
static char  cmi_command8[] = {
    0xa5,
    0x00,
};
static char  cmi_addr_shift_9[] = {
	0x00,
	0xb4,
};
static char  cmi_command9[] = {
    0xc0,
    0x40,
};
static char  cmi_addr_shift_10[] = {
	0x00,
	0x82,
};
static char  cmi_command10[] = {
    0xa5,
    0x02,
};
static char  cmi_addr_shift_11[] = {
	0x00,
	0x80,
};
static char  cmi_command11[] = {
    0xc0,
    0x00,0x8B,0x00,0x0A,0x0A,0x00,0x8B,0x0A,0x0A,0x00,
};
static char  cmi_addr_shift_12[] = {
	0x00,
	0xa0,
};
static char  cmi_command12[] = {
    0xc0,
    0x00,0x00,0x00,0x03,0x00,0x1E,0x06,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_13[] = {
	0x00,
	0x80,
};
static char  cmi_command13[] = {
    0xc2,
    0x82,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_14[] = {
	0x00,
	0x90,
};
static char  cmi_command14[] = {
    0xc2,
    0x87,0x0C,0x01,0x07,0x06,0x86,0x0C,0x01,0x07,0x06,0x85,0x0D,0x01,0x07,0x06,
};
static char  cmi_addr_shift_15[] = {
	0x00,
	0xa0,
};
static char  cmi_command15[] = {
    0xc2,
    0x84,0x0D,0x01,0x07,0x06,0x87,0x0B,0x01,0x07,0x06,0x86,0x0B,0x01,0x07,0x06,
};
static char  cmi_addr_shift_16[] = {
	0x00,
	0xb0,
};
static char  cmi_command16[] = {
    0xc2,
    0x85,0x0D,0x01,0x07,0x06,0x84,0x0D,0x01,0x07,0x06,
};
static char  cmi_addr_shift_17[] = {
	0x00,
	0xea,
};
static char  cmi_command17[] = {
    0xc2,
    0x22,0x02,0x11,
};
static char  cmi_addr_shift_18[] = {
	0x00,
	0xf9,
};
static char  cmi_command18[] = {
    0xc2,
    0x08,0x00,0x0C,0x01,
};
static char  cmi_addr_shift_19[] = {
	0x00,
	0xd0,
};
static char  cmi_command19[] = {
    0xc0,
    0x00,0x00,0x00,0x03,0x00,0x1e,0x06,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_20[] = {
	0x00,
	0x80,
};
static char  cmi_command20[] = {
    0xc3,
    0x82,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_21[] = {
	0x00,
	0x90,
};
static char  cmi_command21[] = {
    0xc3,
    0x87,0x0c,0x01,0x07,0x06,0x86,0x0c,0x01,0x07,0x06,0x85,0x0d,0x01,0x07,0x06,
};
static char  cmi_addr_shift_22[] = {
	0x00,
	0xa0,
};
static char  cmi_command22[] = {
    0xc3,
    0x84,0x0d,0x01,0x07,0x06,0x87,0x0b,0x01,0x07,0x06,0x86,0x0b,0x01,0x07,0x06,
};
static char  cmi_addr_shift_23[] = {
	0x00,
	0xb0,
};
static char  cmi_command23[] = {
    0xc3,
    0x85,0x0d,0x01,0x07,0x06,0x84,0x0d,0x01,0x07,0x06,
};
static char  cmi_addr_shift_24[] = {
	0x00,
	0xea,
};
static char  cmi_command24[] = {
    0xc3,
    0x22,0x02,0x11,
};
static char  cmi_addr_shift_25[] = {
	0x00,
	0x90,
};
static char  cmi_command25[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xf0,
};
static char  cmi_addr_shift_26[] = {
	0x00,
	0xa0,
};
static char  cmi_command26[] = {
    0xcb,
    0xf0,0x00,0x00,0x00,0x00,0xc0,0x00,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_27[] = {
	0x00,
	0xb0,
};
static char  cmi_command27[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x50,0xc0,0x00,0x00,0xc0,0x00,
};
static char  cmi_addr_shift_28[] = {
	0x00,
	0xc0,
};
static char  cmi_command28[] = {
    0xcb,
    0x14,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x14,0x04,0x04,0x14,0x3b,0x3b,
};
static char  cmi_addr_shift_29[] = {
	0x00,
	0xd0,
};
static char  cmi_command29[] = {
    0xcb,
    0x3b,0x14,0x04,0x04,0x30,0x35,0x04,0xf7,0x04,0x14,0x14,0x14,0x14,0x04,0x04,
};
static char  cmi_addr_shift_30[] = {
	0x00,
	0xe0,
};
static char  cmi_command30[] = {
    0xcb,
    0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x15,0x15,0x07,0x14,0x14,0x00,0x00,
};
static char  cmi_addr_shift_31[] = {
	0x00,
	0xf0,
};
static char  cmi_command31[] = {
    0xcb,
    0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
static char  cmi_addr_shift_32[] = {
	0x00,
	0x80,
};
static char  cmi_command32[] = {
    0xcc,
    0x0f,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x10,
};
static char  cmi_addr_shift_33[] = {
	0x00,
	0xb0,
};
static char  cmi_command33[] = {
    0xcc,
    0x0f,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x10,
};
static char  cmi_addr_shift_34[] = {
	0x00,
	0x8a,
};
static char  cmi_command34[] = {
    0xcd,
    0x0b,
};
static char  cmi_addr_shift_35[] = {
	0x00,
	0xa0,
};
static char  cmi_command35[] = {
    0xcd,
    0x06,0x07,0x08,0x09,0x14,0x2d,0x14,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,
};
static char  cmi_addr_shift_36[] = {
	0x00,
	0xb0,
};
static char  cmi_command36[] = {
    0xcd,
    0x2d,0x2d,0x2d,0x15,0x12,0x2c,0x0c,0x0b,0x13,0x0e,0x0f,0x10,0x2d,0x2d,0x2d,
};
static char  cmi_addr_shift_37[] = {
	0x00,
	0xc0,
};
static char  cmi_command37[] = {
    0xcd,
    0x2d,0x2d,0x2d,0x27,0x28,0x29,0x2a,0x2b,0x1d,0x2d,
};
static char  cmi_addr_shift_38[] = {
	0x00,
	0xd0,
};
static char  cmi_command38[] = {
    0xcd,
    0x02,0x03,0x04,0x05,0x14,0x2d,0x14,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,0x2d,
};
static char  cmi_addr_shift_39[] = {
	0x00,
	0xe0,
};
static char  cmi_command39[] = {
    0xcd,
    0x2d,0x2d,0x2d,0x15,0x12,0x2c,0x0c,0x0b,0x13,0x0e,0x0f,0x10,0x2d,0x2d,0x2d,
};
static char  cmi_addr_shift_40[] = {
	0x00,
	0xf0,
};
static char  cmi_command40[] = {
    0xcd,
    0x2d,0x2d,0x2d,0x27,0x28,0x29,0x2a,0x2b,0x1d,0x2d,
};
static char  cmi_addr_shift_41[] = {
	0x00,
	0x00,
};
static char  cmi_command41[] = {
    0xd8,
    0x34,0x34,
};
static char  cmi_addr_shift_42[] = {
	0x00,
	0x00,
};
//CMI param ver2.0
//new parameters for CMI to make color temprature colder than previous version
//new para ver released on 4-14, so we add comment depends on date on which we recieved 
static char  cmi_command42[] = {
    0xe1,
    0x78,0x79,0x7b,0x7d,0x80,0x83,0x87,0x8e,0x90,0x98,0x9d,0xa0,0x5d,0x5a,0x54,0x46,0x3c,0x2f,0x26,0x23,0x1f,0x1d,0x1a,0x10,  //V1.1 modify 0515 depends 04-12 code from CMI  
};
static char  cmi_addr_shift_43[] = {
	0x00,
	0x00,
};
static char  cmi_command43[] = {
    0xe2,
    0x78,0x79,0x7b,0x7d,0x80,0x83,0x87,0x8e,0x90,0x98,0x9d,0xa0,0x5d,0x5a,0x54,0x46,0x3c,0x2f,0x26,0x24,0x1e,0x1d,0x1b,0x10,  //V1.1 modify 0515 depends 04-12 code from CMI  
};
static char  cmi_addr_shift_44[] = {
	0x00,
	0x00,
};
static char  cmi_command44[] = {
    0xe3,
    0x70,0x71,0x73,0x75,0x79,0x7b,0x7e,0x88,0x8b,0x93,0x99,0x9d,0x5f,0x5c,0x55,0x47,0x3d,0x2f,0x27,0x23,0x1f,0x1d,0x1a,0x10, //V1.1 modify 0515 depends 04-12 code from CMI  
};
static char  cmi_addr_shift_45[] = {
	0x00,
	0x00,
};
static char  cmi_command45[] = {
    0xe4,
    0x70,0x71,0x73,0x75,0x79,0x7b,0x7e,0x88,0x8b,0x93,0x99,0x9d,0x5f,0x5c,0x55,0x47,0x3d,0x2f,0x27,0x24,0x1e,0x1d,0x1b,0x10, //V1.1 modify 0515 depends 04-12 code from CMI  
};
static char  cmi_addr_shift_46[] = {
	0x00,
	0x00,
};
static char  cmi_command46[] = {
    0xe5,
    0x00,0x24,0x2b,0x38,0x3f,0x47,0x55,0x66,0x72,0x82,0x8e,0x96,0x65,0x63,0x5a,0x4c,0x41,0x31,0x27,0x23,0x1f,0x1d,0x1a,0x10, //V1.1 modify 0515 depends 04-12 code from CMI  
};
static char  cmi_addr_shift_47[] = {
	0x00,
	0x00,
};
static char  cmi_command47[] = {
    0xe6,
    0x00,0x24,0x2b,0x38,0x3f,0x48,0x55,0x65,0x72,0x82,0x8e,0x96,0x65,0x63,0x5a,0x4b,0x41,0x31,0x27,0x24,0x1e,0x1d,0x1b,0x10, //V1.1 modify 0515 depends 04-12 code from CMI   
};

//add digital RGB gamma settging
static char  cmi_addr_shift_00[] = {
	0x00,
	0x00,
};
// 0x00, 0x00
static char  cmi_command_ec[] = {
    0xec,
    0x40,0x44,0x43,0x44,0x44,0x43,0x44,0x44,0x34,0x44,0x44,0x34,0x44,0x44,0x34,0x44,0x44,0x44,0x43,0x44,0x44,0x43,0x44,0x44,0x34,0x44,0x44,0x34,0x44,0x44,0x34,0x44,0x04,
};
// 0x00, 0x00
static char  cmi_command_ed[] = {
    0xed,
    0x40,0x34,0x44,0x44,0x43,0x44,0x34,0x44,0x44,0x43,0x44,0x34,0x44,0x44,0x43,0x44,0x34,0x44,0x34,0x44,0x44,0x43,0x44,0x34,0x44,0x44,0x43,0x44,0x34,0x44,0x44,0x43,0x04,
};
// 0x00, 0x00
static char  cmi_command_ee[] = {
    0xee,
    0x40,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x04,
};

static char  cmi_addr_shift_48[] = {
	0x00,
	0x00,
};
static char  cmi_command48[] = {
    0xc6,
    0x10,
};
static char  cmi_addr_shift_49[] = {
	0x00,
	0x97,
};
static char  cmi_command49[] = {
    0xc5,
    0x22,0x20,
};
static char  cmi_addr_shift_50[] = {
	0x00,
	0xa7,
};
static char  cmi_command50[] = {
    0xc5,
    0x22,0x20,
};
static char  cmi_addr_shift_51[] = {
	0x00,
	0x83,
};
static char  cmi_command51[] = {
    0xc4,
    0x02,
};
static char  cmi_addr_shift_52[] = {
	0x00,
	0x80,
};
static char  cmi_command52[] = {
    0xa5,
    0x0c,
};
static char  cmi_addr_shift_53[] = {
	0x00,
	0xb3,
};
static char  cmi_command53[] = {
    0xc0,
    0x33,
};
static char  cmi_addr_shift_54[] = {
	0x00,
	0x80,
};
static char  cmi_command54[] = {
    0xc4,
    0x04,
};
static char  cmi_addr_shift_55[] = {
	0x00,
	0x81,
};
static char  cmi_command55[] = {
    0xc4,
    0x0f,
};
static char  cmi_addr_shift_56[] = {
	0x00,
	0x80,
};
static char  cmi_command56[] = {
    0xff,
    0x00,0x00,
};
static char  cmi_addr_shift_57[] = {
	0x00,
	0x00,
};
static char  cmi_command57[] = {
    0xff,
    0x00,0x00,0x00,
};
static char  cmi_bl_enable[] = {
    0x53,
    0x24,
};
static char  cmi_bl_mode[] = {
    0x55,
#if CMI_CABC_ENABLED    
    0x01,   //to enable CABC when LCD panel on
#else
    0x00,   
#endif
};
static char  cmi_te_enable[] = {
    0x35,
    0x00,
};
static char  cmi_exit_sleep[] = {
    0x11,
    0x00,
};
static char  cmi_display_on[] = {
    0x29,
    0x00,
};
static char  cmi_all_pixels_off[] = {
	0x22,
};
static char cmi_normal_display_on[] = {
	0x13, 
};
/*******************************************************************************
** cmi Power OFF Sequence(Normal to power off)
*/
static char cmi_bl_level_0[] = {
	0x51,
	0x00,
};
static char cmi_display_off[] = {
	0x28,
};
static char cmi_enter_sleep[] = {
	0x10,
};
/*******************************************************************************
** cmi CABC Sequence
*/
#if CMI_CABC_ENABLED
//enable cmd 2
static char  cmi_cmd_enable_1[] = {
	0xff,
	0x12, 0x82, 0x01,
};
static char  cmi_cmd_enable_2[] = {
	0xff,
	0x12, 0x82,
};
//disable cmd 2
static char  cmi_cmd_disable_1[] = {
	0xff,
	0x00, 0x00,
};
static char  cmi_cmd_disable_2[] = {
	0xff,
	0x00, 0x00, 0x00,
};
static char  cmi_addr_shift_0[] = {
	0x00,
	0x00,
};
static char  cmi_addr_shift_80[] = {
	0x00,
	0x80,
};
static char  cmi_addr_shift_90[] = {
	0x00,
	0x90,
};
static char  cmi_enable_cabc_setting[] = {
	0x59,
	0x03,
};
static char  cmi_disable_cabc_setting[] = {
	0x59,
	0x00,
};
static char cmi_cabc_ca[] = {
	0xCA,
	0xe5,0xff,0xd8,0xff,0xb2,
	0xff,0x53,0x53,0x53,
};
static char cmi_cabc_c6_CRV1[] = {
	0xc6,
	0x10,
};
static char cmi_cabc_c7_UI[] = {
	0xC7,
	0xa0,0x87,0x88,0x89,0x88,
	0x98,0x88,0x88,0x88,0x98,
	0x88,0x88,0x89,0x88,0x78,
	0x88,0x77,0x77,
};
static char cmi_cabc_c6_CRV2[] = {
	0xc6,
	0x11,
};
static char cmi_cabc_c7_STILL[] = {
	0xC7,
	0xb0,0x87,0x88,0x88,0x89,
	0x98,0x89,0x88,0x88,0x88,
	0x87,0x88,0x88,0x88,0x78,
	0x89,0x77,0x68,
};
static char cmi_cabc_c6_CRV3[] = {
	0xc6,
	0x12,
};
static char cmi_cabc_c7_MOV[] = {
	0xC7,
	0xf0,0x69,0x89,0x8a,0x8a,
	0x99,0x88,0x79,0x89,0x77,
	0x77,0x87,0x79,0x78,0x78,
	0x78,0x67,0x67,
};
static char cmi_cabc_c6_disable[] = {
	0xc6,
	0x00,
};
#endif
/*******************************************************************************
**tm_nt35590_tm_init_table
*/
/*************enter cmd1*************/
static char tm_cmd_ff[] = {
	0xFF,
	0x00,
};
static char tm_cmd_fb[] = {
	0xFB,
	0x01,
};
static char tm_cmd_3b[] = {
	0x3B,
	0x03,0x06,0x03,0x02,0x02,
};

/**************set pwm**************/
static char tm_cmd_ff4[] = {
	0xFF,
	0x04,
};
static char tm_cmd_0a[] = {
	0x0A,
	0x02,
};

/**************command mode & mipi 4plane**************/
/***command mode***/
static char tm_cmd_c2[] = {
	0xC2,
	0x08,
};
/***mipi 4plane***/
static char tm_cmd_mipi_4lane[] = {
	0xBA,
	0x03,
};

/**************cabc & te**************/
static char tm_bklt_53[] = {
	0x53,
	0x24,
};
static char tm_bklt_55[] = {
	0x55,
#if TM_CABC_ENABLED        
	0x01,  //set ui mode as default if cabc eanbled
#else
	0x00,
#endif
};
static char tm_te_enable[] = {
	0x35,
	0x00,
};

/**************Sleep out & Display ON**************/
static char tm_sleep_out[] = {
	0x11,
};
static char tm_display_on[] = {
	0x29,
};

/**************display off & Sleep Mode**************/
static char tm_bl_level_0[] = {
	0x51,
	0x00,
};
static char tm_display_off[] = {
	0x28,
};
static char tm_sleep_in[] = {
	0x10,
};

#if TM_CABC_ENABLED
static char tm_cmd_ff03[] = {
	0xff,
	0x03,
};
static char tm_cmd_fb01[] = {
	0xfb,
	0x01,
};
static char tm_cmd_1804[] = {
	0x18,
	0x04,
};
static char tm_cmd_190c[] = {
	0x19,
	0x0c,
};
static char tm_cmd_1a14[] = {
	0x1a,
	0x14,
};
static char tm_cmd_2526[] = {
	0x25,
	0x26,
};
static char tm_cmd_0000[] = {
	0x00,
	0x00,
};
static char tm_cmd_0104[] = {
	0x01,
	0x04,
};
static char tm_cmd_0208[] = {
	0x02,
	0x08,
};
static char tm_cmd_030c[] = {
	0x03,
	0x0c,
};
static char tm_cmd_0410[] = {
	0x04,
	0x10,
};
static char tm_cmd_0514[] = {
	0x05,
	0x14,
};
static char tm_cmd_0618[] = {
	0x06,
	0x18,
};
static char tm_cmd_071c[] = {
	0x07,
	0x1c,
};
static char tm_cmd_0820[] = {
	0x08,
	0x20,
};
static char tm_cmd_0924[] = {
	0x09,
	0x24,
};
static char tm_cmd_0a28[] = {
	0x0a,
	0x28,
};
static char tm_cmd_0b2c[] = {
	0x0b,
	0x2c,
};
static char tm_cmd_0c30[] = {
	0x0c,
	0x30,
};
static char tm_cmd_0d34[] = {
	0x0d,
	0x34,
};
static char tm_cmd_0e38[] = {
	0x0e,
	0x38,
};
static char tm_cmd_0f3c[] = {
	0x0f,
	0x3c,
};
static char tm_cmd_ff04[] = {
	0xff,
	0x04,
};
static char tm_cmd_13ff[] = {
	0x13,
	0xff,
};
static char tm_cmd_14f8[] = {
	0x14,
	0xf8,
};
static char tm_cmd_15f3[] = {
	0x15,
	0xf3,
};
static char tm_cmd_16da[] = {
	0x16,
	0xda,
};
static char tm_cmd_21ff[] = {
	0x21,
	0xff,
};
static char tm_cmd_22ff[] = {
	0x22,
	0xff,
};
static char tm_cmd_23ff[] = {
	0x23,
	0xff,
};
static char tm_cmd_24ff[] = {
	0x24,
	0xff,
};
static char tm_cmd_25f7[] = {
	0x25,
	0xf7,
};
static char tm_cmd_26f5[] = {
	0x26,
	0xf5,
};
static char tm_cmd_27f3[] = {
	0x27,
	0xf3,
};
static char tm_cmd_28f1[] = {
	0x28,
	0xf1,
};
static char tm_cmd_29ef[] = {
	0x29,
	0xef,
};
static char tm_cmd_2a90[] = {
	0x2a,
	0x90,
};


static char  tm_cabc_ui_on[] = {
    0x55,
    0x01,
};


static char  tm_cabc_vid_on[] = {
    0x55,
    0x03,
};


static struct dsi_cmd_desc tm_cabc_ui_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cmd_fb01), tm_cmd_fb01},
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cabc_ui_on), tm_cabc_ui_on},
};

static struct dsi_cmd_desc tm_cabc_vid_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cmd_fb01), tm_cmd_fb01},
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(tm_cabc_vid_on), tm_cabc_vid_on},
};
#endif


/*******************************************************************************
**auto_display_on_cmds
*/
static struct dsi_cmd_desc auto_display_on_cmds[] = {
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_1), auto_addr_shift_1},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command1), auto_command1},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_2), auto_addr_shift_2},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command2), auto_command2},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_3), auto_addr_shift_3},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command3), auto_command3},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_4), auto_addr_shift_4},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command4), auto_command4},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_5), auto_addr_shift_5},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command5), auto_command5},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_6), auto_addr_shift_6},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command6), auto_command6},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_7), auto_addr_shift_7},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command7), auto_command7},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_8), auto_addr_shift_8},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command8), auto_command8},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_9), auto_addr_shift_9},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command9), auto_command9},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_10), auto_addr_shift_10},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command10), auto_command10},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_11), auto_addr_shift_11},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command11), auto_command11},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_12), auto_addr_shift_12},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command12), auto_command12},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_13), auto_addr_shift_13},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command13), auto_command13},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_14), auto_addr_shift_14},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command14), auto_command14},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_15), auto_addr_shift_15},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command15), auto_command15},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_16), auto_addr_shift_16},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command16), auto_command16},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_17), auto_addr_shift_17},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command17), auto_command17},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_18), auto_addr_shift_18},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command18), auto_command18},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_19), auto_addr_shift_19},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command19), auto_command19},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_20), auto_addr_shift_20},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command20), auto_command20},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_21), auto_addr_shift_21},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command21), auto_command21},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_22), auto_addr_shift_22},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command22), auto_command22},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_23), auto_addr_shift_23},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command23), auto_command23},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_24), auto_addr_shift_24},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command24), auto_command24},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_25), auto_addr_shift_25},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command25), auto_command25},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_26), auto_addr_shift_26},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command26), auto_command26},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_27), auto_addr_shift_27},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command27), auto_command27},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_28), auto_addr_shift_28},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command28), auto_command28},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_29), auto_addr_shift_29},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command29), auto_command29},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_30), auto_addr_shift_30},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command30), auto_command30},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_31), auto_addr_shift_31},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command31), auto_command31},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_32), auto_addr_shift_32},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command32), auto_command32},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_33), auto_addr_shift_33},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command33), auto_command33},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_34), auto_addr_shift_34},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command34), auto_command34},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_35), auto_addr_shift_35},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command35), auto_command35},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_36), auto_addr_shift_36},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command36), auto_command36},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_37), auto_addr_shift_37},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command37), auto_command37},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_38), auto_addr_shift_38},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		//sizeof(auto_command28), auto_command38},    //here just a mistake using wrong lenth of command28 for command38
		sizeof(auto_command38), auto_command38},    
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_39), auto_addr_shift_39},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command39), auto_command39},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_40), auto_addr_shift_40},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command40), auto_command40},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_41), auto_addr_shift_41},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command41), auto_command41},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_42), auto_addr_shift_42},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command42), auto_command42},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_43), auto_addr_shift_43},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command43), auto_command43},
//digital gamma setting 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command_ec), auto_command_ec},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command_ed), auto_command_ed},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command_ee), auto_command_ee},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_44), auto_addr_shift_44},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command44), auto_command44},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_45), auto_addr_shift_45},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command45), auto_command45},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_46), auto_addr_shift_46},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_command46), auto_command46},
//optimize cmi para
#if AUO_CABC_ENABLED
       //Enablle level 2 command
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cmd_enable_1), auto_cmd_enable_1},       
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_80), auto_addr_shift_80},       		
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cmd_enable_2), auto_cmd_enable_2},     
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_80), auto_addr_shift_80},   
	//CABC function setting
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_ca), auto_cabc_ca},     
      //Enable CABC Gamma 1
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G1), auto_cabc_c6_G1}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G1), auto_cabc_c7_G1}, 
      //Enable CABC Gamma 2
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G2), auto_cabc_c6_G2}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G2), auto_cabc_c7_G2}, 
      //Enable CABC Gamma 3
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G3), auto_cabc_c6_G3}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G3), auto_cabc_c7_G3}, 
      //Enable CABC Gamma 4
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G4), auto_cabc_c6_G4}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G4), auto_cabc_c7_G4}, 
      //Enable CABC Gamma 5
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G5), auto_cabc_c6_G5}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G5), auto_cabc_c7_G5}, 		
      //Enable CABC Gamma 6
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G6), auto_cabc_c6_G6}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G6), auto_cabc_c7_G6}, 			
      //Enable CABC Gamma 7
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G7), auto_cabc_c6_G7}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G7), auto_cabc_c7_G7}, 	
      //Enable CABC Gamma 8
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G8), auto_cabc_c6_G8}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G8), auto_cabc_c7_G8}, 	
      //Enable CABC Gamma 9
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G9), auto_cabc_c6_G9}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G9), auto_cabc_c7_G9}, 			
      //Enable CABC Gamma 10
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G10), auto_cabc_c6_G10}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G10), auto_cabc_c7_G10}, 
      //Enable CABC Gamma 11
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G11), auto_cabc_c6_G11}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G11), auto_cabc_c7_G11}, 
      //Enable CABC Gamma 12
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G12), auto_cabc_c6_G12}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G12), auto_cabc_c7_G12}, 
      //Enable CABC Gamma 13
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G13), auto_cabc_c6_G13}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G13), auto_cabc_c7_G13}, 
      //Enable CABC Gamma 13
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G13), auto_cabc_c6_G13}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G13), auto_cabc_c7_G13}, 
      //Enable CABC Gamma 14
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G14), auto_cabc_c6_G14}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G14), auto_cabc_c7_G14}, 
      //Enable CABC Gamma 15
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G15), auto_cabc_c6_G15}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G15), auto_cabc_c7_G15}, 
      //Enable CABC Gamma 16
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_0), auto_addr_shift_0},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c6_G16), auto_cabc_c6_G16}, 
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_c7_G16), auto_cabc_c7_G16}, 
      //Exit CABC setting
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_exit_seq_1), auto_cabc_exit_seq_1},  
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_exit_seq_2), auto_cabc_exit_seq_2}, 
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cabc_exit_seq_3), auto_cabc_exit_seq_3}, 
      //Disable level 2 command
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_80), auto_addr_shift_80},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cmd_disable_1), auto_cmd_disable_1},       
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(auto_addr_shift_80), auto_addr_shift_80},         		
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(auto_cmd_disable_2), auto_cmd_disable_2},     
#endif 
	{DTYPE_DCS_WRITE, 0, 100, WAIT_TYPE_US,
		sizeof( auto_all_pixels_off),  auto_all_pixels_off},		
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof( auto_bl_enable),  auto_bl_enable},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(auto_bl_mode), auto_bl_mode},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(auto_te_enable), auto_te_enable},
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(auto_exit_sleep), auto_exit_sleep},
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(auto_display_on), auto_display_on},
};
/*******************************************************************************
**auto_display_off_cmds
*/
static struct dsi_cmd_desc auto_display_off_cmds[] = {
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(auto_bl_level_0), auto_bl_level_0},	
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(auto_display_off), auto_display_off},		
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(auto_enter_sleep), auto_enter_sleep},
};
/*******************************************************************************
**cmi_display_on_cmds
*/
static struct dsi_cmd_desc cmi_display_on_cmds[] = {
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_1), cmi_addr_shift_1},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command1), cmi_command1},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_2), cmi_addr_shift_2},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command2), cmi_command2},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_3), cmi_addr_shift_3},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command3), cmi_command3},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_4), cmi_addr_shift_4},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command4), cmi_command4},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_5), cmi_addr_shift_5},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command5), cmi_command5},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_6), cmi_addr_shift_6},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command6), cmi_command6},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_7), cmi_addr_shift_7},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command7), cmi_command7},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_8), cmi_addr_shift_8},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command8), cmi_command8},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_9), cmi_addr_shift_9},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command9), cmi_command9},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_10), cmi_addr_shift_10},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command10), cmi_command10},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_11), cmi_addr_shift_11},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command11), cmi_command11},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_12), cmi_addr_shift_12},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command12), cmi_command12},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_13), cmi_addr_shift_13},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command13), cmi_command13},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_14), cmi_addr_shift_14},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command14), cmi_command14},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_15), cmi_addr_shift_15},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command15), cmi_command15},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_16), cmi_addr_shift_16},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command16), cmi_command16},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_17), cmi_addr_shift_17},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command17), cmi_command17},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_18), cmi_addr_shift_18},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command18), cmi_command18},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_19), cmi_addr_shift_19},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command19), cmi_command19},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_20), cmi_addr_shift_20},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command20), cmi_command20},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_21), cmi_addr_shift_21},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command21), cmi_command21},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_22), cmi_addr_shift_22},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command22), cmi_command22},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_23), cmi_addr_shift_23},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command23), cmi_command23},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_24), cmi_addr_shift_24},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command24), cmi_command24},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_25), cmi_addr_shift_25},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command25), cmi_command25},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_26), cmi_addr_shift_26},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command26), cmi_command26},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_27), cmi_addr_shift_27},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command27), cmi_command27},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_28), cmi_addr_shift_28},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command28), cmi_command28},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_29), cmi_addr_shift_29},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command29), cmi_command29},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_30), cmi_addr_shift_30},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command30), cmi_command30},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_31), cmi_addr_shift_31},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command31), cmi_command31},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_32), cmi_addr_shift_32},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command32), cmi_command32},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_33), cmi_addr_shift_33},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command33), cmi_command33},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_34), cmi_addr_shift_34},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command34), cmi_command34},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_35), cmi_addr_shift_35},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command35), cmi_command35},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_36), cmi_addr_shift_36},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command36), cmi_command36},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_37), cmi_addr_shift_37},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command37), cmi_command37},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_38), cmi_addr_shift_38},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command28), cmi_command38},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_39), cmi_addr_shift_39},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command39), cmi_command39},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_40), cmi_addr_shift_40},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command40), cmi_command40},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_41), cmi_addr_shift_41},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command41), cmi_command41},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_42), cmi_addr_shift_42},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command42), cmi_command42},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_43), cmi_addr_shift_43},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command43), cmi_command43},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_44), cmi_addr_shift_44},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command44), cmi_command44},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_45), cmi_addr_shift_45},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command45), cmi_command45},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_46), cmi_addr_shift_46},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command46), cmi_command46},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_47), cmi_addr_shift_47},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command47), cmi_command47},
//add for digital RGB Gamma
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_00), cmi_addr_shift_00},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command_ec), cmi_command_ec},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_00), cmi_addr_shift_00},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command_ed), cmi_command_ed},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_00), cmi_addr_shift_00},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command_ee), cmi_command_ee},			
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_48), cmi_addr_shift_48},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command48), cmi_command48},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_49), cmi_addr_shift_49},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command49), cmi_command49},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_50), cmi_addr_shift_50},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command50), cmi_command50},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_51), cmi_addr_shift_51},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command51), cmi_command51},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_52), cmi_addr_shift_52},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command52), cmi_command52},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_53), cmi_addr_shift_53},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command53), cmi_command53},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_54), cmi_addr_shift_54},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command54), cmi_command54},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_55), cmi_addr_shift_55},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command55), cmi_command55},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_56), cmi_addr_shift_56},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command56), cmi_command56},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_57), cmi_addr_shift_57},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_command57), cmi_command57},
//optimize cmi para, as use wrong data type before
#if CMI_CABC_ENABLED
       //Enablle level 2 command
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cmd_enable_1), cmi_cmd_enable_1},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_80), cmi_addr_shift_80},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cmd_enable_2), cmi_cmd_enable_2},
       //Enable CABC setting
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_90), cmi_addr_shift_90},
       //CABC function setting
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_ca), cmi_cabc_ca},
       //Enable CABC Curve 1
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c6_CRV1), cmi_cabc_c6_CRV1},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c7_UI), cmi_cabc_c7_UI},
       //Enable CABC Curve 2
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c6_CRV2), cmi_cabc_c6_CRV2},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c7_STILL), cmi_cabc_c7_STILL},
       //Enable CABC Curve 3
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},
	{DTYPE_GEN_WRITE2, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c6_CRV3), cmi_cabc_c6_CRV3},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c7_MOV), cmi_cabc_c7_MOV},
       //Disable CABC curve
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},		
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cabc_c6_disable), cmi_cabc_c6_disable},       
       //Disable CABC setting
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},		
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_disable_cabc_setting), cmi_disable_cabc_setting},         
       //Disable level 2
       //shift 80 missed before
       {DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_80), cmi_addr_shift_80},   
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cmd_disable_1), cmi_cmd_disable_1},
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_addr_shift_0), cmi_addr_shift_0},		
	{DTYPE_GEN_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_cmd_disable_2), cmi_cmd_disable_2},		
#endif
	{DTYPE_DCS_WRITE, 0, 100, WAIT_TYPE_US,
		sizeof( cmi_all_pixels_off),  cmi_all_pixels_off},		
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof( cmi_bl_enable),  cmi_bl_enable},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_bl_mode), cmi_bl_mode},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_te_enable), cmi_te_enable},
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(cmi_exit_sleep), cmi_exit_sleep},
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(cmi_display_on), cmi_display_on},
};
/*******************************************************************************
**cmi_display_off_cmds
*/
static struct dsi_cmd_desc cmi_display_off_cmds[] = {
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(cmi_bl_level_0), cmi_bl_level_0},	
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(cmi_display_off), cmi_display_off},		
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(cmi_enter_sleep), cmi_enter_sleep},
};

/*******************************************************************************
**CABC cmds
*/
static char  cmi_cabc_ui_on[] = {
    0x55,
    0x01,
};


static char  cmi_cabc_vid_on[] = {
    0x55,
    0x03,
};

static char  auo_cabc_ui_on[] = {
    0x55,
    0x01,
};


static char  auo_cabc_vid_on[] = {
    0x55,
    0x03,
};

static struct dsi_cmd_desc cmi_cabc_ui_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(cmi_cabc_ui_on), cmi_cabc_ui_on},
};

static struct dsi_cmd_desc cmi_cabc_vid_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(cmi_cabc_vid_on), cmi_cabc_vid_on},
};


static struct dsi_cmd_desc auo_cabc_ui_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(auo_cabc_ui_on), auo_cabc_ui_on},
};

static struct dsi_cmd_desc auo_cabc_vid_cmds[] = {
	{DTYPE_GEN_LWRITE, 0, 30, WAIT_TYPE_US,
		sizeof(auo_cabc_vid_on), auo_cabc_vid_on},
};
/*******************************************************************************

/*******************************************************************************
**tm_nt35590_tm_init_table
*/
static struct dsi_cmd_desc tm_display_on_cmds[] = {
#if TM_CABC_ENABLED
	/*************cabc cmd*************/
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff03), tm_cmd_ff03},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb01), tm_cmd_fb01},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_1804), tm_cmd_1804},
      {DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_190c), tm_cmd_190c},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_1a14), tm_cmd_1a14},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_2526), tm_cmd_2526},		
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0000), tm_cmd_0000},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0104), tm_cmd_0104},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0208), tm_cmd_0208},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_030c), tm_cmd_030c},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0410), tm_cmd_0410},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0514), tm_cmd_0514},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0618), tm_cmd_0618},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_071c), tm_cmd_071c},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0820), tm_cmd_0820},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0924), tm_cmd_0924},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0a28), tm_cmd_0a28},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0b2c), tm_cmd_0b2c},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0c30), tm_cmd_0c30},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0d34), tm_cmd_0d34},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0e38), tm_cmd_0e38},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0f3c), tm_cmd_0f3c},	

	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff04), tm_cmd_ff04},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb01), tm_cmd_fb01},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_13ff), tm_cmd_13ff},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_14f8), tm_cmd_14f8},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_15f3), tm_cmd_15f3},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_16da), tm_cmd_16da},	
		
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff04), tm_cmd_ff04},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb01), tm_cmd_fb01},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_21ff), tm_cmd_21ff},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_22ff), tm_cmd_22ff},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_23ff), tm_cmd_23ff},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_24ff), tm_cmd_24ff},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_25f7), tm_cmd_25f7},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_26f5), tm_cmd_26f5},			
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_27f3), tm_cmd_27f3},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_28f1), tm_cmd_28f1},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_29ef), tm_cmd_29ef},	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_2a90), tm_cmd_2a90},	
#endif
	/*************enter cmd1*************/
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb), tm_cmd_fb},
	{DTYPE_DCS_LWRITE, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_3b), tm_cmd_3b},

	/**************set pwm**************/
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff4), tm_cmd_ff4},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb), tm_cmd_fb},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_0a), tm_cmd_0a},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},

	/**************set command mode**************/
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb), tm_cmd_fb},
	
	
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_c2), tm_cmd_c2},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_mipi_4lane), tm_cmd_mipi_4lane},
	
	/**************cabc & te**************/
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_bklt_53), tm_bklt_53},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_bklt_55), tm_bklt_55},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_te_enable), tm_te_enable},
		
	
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(tm_sleep_out), tm_sleep_out},
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(tm_display_on), tm_display_on},
};

static struct dsi_cmd_desc tm_display_off_cmds[] = {
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_bl_level_0), tm_bl_level_0},
	{DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_MS,
		sizeof(tm_display_off), tm_display_off},
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS,
		sizeof(tm_sleep_in), tm_sleep_in},
};

static struct dsi_cmd_desc  tm_NT35590_cmd_page0[] = {
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_ff), tm_cmd_ff},
	{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
		sizeof(tm_cmd_fb), tm_cmd_fb},
};

/*******************************************************************************
** LCD VCC
*/
//to define macro for LCD VDDI,  please reffer PMU regulator declearation
#define SYNAPTICS_VBUS_LCD LCDIO_VDD
//lcd vddio
static int lcd_vddio_get(struct platform_device *pdev)
{
	BUG_ON(pdev==NULL);
	g_panel_platform_data->lcd_vddio = regulator_get(&pdev->dev,SYNAPTICS_VBUS_LCD);
	if (IS_ERR(g_panel_platform_data->lcd_vddio)) {
		printk( "%s: failed to get regulator lcd vddio\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static int lcd_vddio_put(void)
{
	if (IS_ERR(g_panel_platform_data->lcd_vddio)) {
		printk( "%s: failed to put regulator lcd vddio\n", __func__);
		return -EINVAL;
	}
	regulator_put(g_panel_platform_data->lcd_vddio);
	return 0;
}

static int lcd_vddio_enable(void)
{
	int error;
	if (IS_ERR(g_panel_platform_data->lcd_vddio)) {
		printk( "%s: failed to enable regulator lcd vddio 1\n", __func__);
		return -EINVAL;
	}
	error = regulator_enable(g_panel_platform_data->lcd_vddio);
	if (error < 0) {
		printk( "%s: failed to enable regulator lcd vddio 2\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static int lcd_vddio_disable(void)
{
	int error;
	if (IS_ERR(g_panel_platform_data->lcd_vddio)) {
		printk( "%s: failed to disable regulator lcd vddio 1\n", __func__);
		return -EINVAL;
	}

	error = regulator_disable(g_panel_platform_data->lcd_vddio);
	if (error < 0) {
		printk( "%s: failed to disable regulator lcd vddio 2\n", __func__);
		return -EINVAL;
	}
	return 0;
}

/*******************************************************************************
** LCD IOMUX
*/
static int lcd_iomux_get(void)
{
	g_panel_platform_data->lcd_gpio_block = iomux_get_block(IOMUX_LCD_NAME);
	if (!g_panel_platform_data->lcd_gpio_block) {
		k3fb_loge("failed to get iomux_lcd!\n");
		return PTR_ERR(g_panel_platform_data->lcd_gpio_block);
	}

	g_panel_platform_data->lcd_gpio_block_config = iomux_get_blockconfig(IOMUX_LCD_NAME);
	if (!g_panel_platform_data->lcd_gpio_block_config) {
		k3fb_loge("failed to get iomux_lcd config!\n");
		return PTR_ERR(g_panel_platform_data->lcd_gpio_block_config);
	}
	return 0;
}

static void lcd_iomux_set(int mode)
{
	if (blockmux_set(g_panel_platform_data->lcd_gpio_block, g_panel_platform_data->lcd_gpio_block_config, mode) != 0) {
		k3fb_loge("failed to set iomux_lcd normal mode!\n");
	}
}

/*******************************************************************************
** LCD GPIO
*/
static int lcd_get_one_gpio_resource(struct platform_device *pdev, char *name)
{
	struct resource *res =	NULL;
	int gpio = -1;
	res = platform_get_resource_byname(pdev, IORESOURCE_IO, name);
	if (res == NULL) {
		k3fb_loge("failed to get gpio resource:%s!\n", name);
		return -1;
	}

	gpio = res->start;
	if (!gpio_is_valid(gpio)) {
		k3fb_loge("gpio %d for %s is invalid.", gpio, name);
		return -1;
	}
	k3fb_loge("gpio %d for %s is valid.", gpio, name);
	return gpio;
}

static void lcd_gpio_request(void)
{
	if (gpio_request(g_panel_platform_data->lcd_gpio_reset, GPIO_LCD_RESET_NAME) != 0) {
		k3fb_loge("failed to request gpio reset!\n");
	}

	if (gpio_request(g_panel_platform_data->lcd_gpio_id0, GPIO_LCD_ID0_NAME) != 0) {
		k3fb_loge("failed to request gpio_lcd_id0!\n");
	}

	if (gpio_request(g_panel_platform_data->lcd_gpio_id1, GPIO_LCD_ID1_NAME) != 0) {
		k3fb_loge("failed to request gpio_lcd_id1!\n");
	}

	if (gpio_request(g_panel_platform_data->lcd_gpio_enp, GPIO_LCD_ENP_NAME) != 0){
	    k3fb_loge("failed to request enp!\n");
	}

	if (gpio_request(g_panel_platform_data->lcd_gpio_enn, GPIO_LCD_ENN_NAME) != 0){
	    k3fb_loge("failed to request enn!\n");
	}

	if (gpio_request(g_panel_platform_data->lcd_gpio_pwm_en, GPIO_LCD_PWM_NAME) != 0){
	    k3fb_loge("failed to request pwm name!\n");
	}
}

static void lcd_gpio_free(void)
{
	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_reset)) {
		gpio_free(g_panel_platform_data->lcd_gpio_reset);
	}

	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_id0)) {
		gpio_free(g_panel_platform_data->lcd_gpio_id0);
	}

	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_id1)) {
		gpio_free(g_panel_platform_data->lcd_gpio_id1);
	}

	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_enp)) {
		gpio_free(g_panel_platform_data->lcd_gpio_enp);
	}

	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_enn)) {
		gpio_free(g_panel_platform_data->lcd_gpio_enn);
	}

	if (gpio_is_valid(g_panel_platform_data->lcd_gpio_pwm_en)) {
		gpio_free(g_panel_platform_data->lcd_gpio_pwm_en);
	}
}

static int lcd_resource(struct platform_device *pdev, struct resource *res)
{
	BUG_ON(pdev == NULL);
	g_panel_platform_data->lcd_gpio_reset = lcd_get_one_gpio_resource(pdev, GPIO_LCD_RESET_NAME);
	g_panel_platform_data->lcd_gpio_id0 = lcd_get_one_gpio_resource(pdev, GPIO_LCD_ID0_NAME);
	g_panel_platform_data->lcd_gpio_id1 = lcd_get_one_gpio_resource(pdev, GPIO_LCD_ID1_NAME);
	g_panel_platform_data->lcd_gpio_enp = lcd_get_one_gpio_resource(pdev, GPIO_LCD_ENP_NAME);
	g_panel_platform_data->lcd_gpio_enn = lcd_get_one_gpio_resource(pdev, GPIO_LCD_ENN_NAME);
	g_panel_platform_data->lcd_gpio_pwm_en = lcd_get_one_gpio_resource(pdev, GPIO_LCD_PWM_NAME);
	return 0;
}

static struct k3_fb_panel_data cmi_panel_data;
static struct lcd_tuning_dev *p_tuning_dev = NULL;
static int cabc_mode = 1; /* allow application to set cabc mode to ui mode */

static int cmi_set_gamma(struct lcd_tuning_dev *ltd, enum lcd_gamma gamma)
{
	int ret = 0;
	struct platform_device *pdev = NULL;
	struct k3_fb_data_type *k3fd = NULL;
	u32 edc_base = 0;
        
	BUG_ON(ltd == NULL);
	pdev = (struct platform_device *)(ltd->data);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);

	edc_base = k3fd->edc_base;

	/* Fix me: Implement it */

	return ret;
}
static int cmi_set_cabc(struct lcd_tuning_dev *ltd, enum  tft_cabc cabc)
{
	int ret = 0;
	struct platform_device *pdev = NULL;
	struct k3_fb_data_type *k3fd = NULL;
	u32 edc_base = 0;
      
	BUG_ON(ltd == NULL);
	pdev = (struct platform_device *)(ltd->data);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);

	edc_base = k3fd->edc_base;

	/* Fix me: Implement it */
      k3fb_logi("%s CABC_MODE %d\n", __func__, cabc);
	switch (cabc) {
	case CABC_UI:
        	if(AUTO_OTM1282A == g_panel_id)
        	{
        	     #if AUO_CABC_ENABLED
        		mipi_dsi_cmds_tx(auo_cabc_ui_cmds, \
        			ARRAY_SIZE(auo_cabc_ui_cmds));
                  #endif
        	}	
        	else if(CMI_OTM1282A == g_panel_id)
             {
                   #if CMI_CABC_ENABLED
        		mipi_dsi_cmds_tx(cmi_cabc_ui_cmds, \
        			ARRAY_SIZE(cmi_cabc_ui_cmds));             
                   #endif 
             }
        	else if(TM_NT35590 == g_panel_id)
             {
                 #if TM_CABC_ENABLED
        		mipi_dsi_cmds_tx(tm_cabc_ui_cmds, \
        			ARRAY_SIZE(tm_cabc_ui_cmds));             
                 #endif
             }            
		break;
	case CABC_VID:
        	if(AUTO_OTM1282A == g_panel_id)
        	{
                #if AUO_CABC_ENABLED
        		mipi_dsi_cmds_tx(auo_cabc_vid_cmds, \
        			ARRAY_SIZE(auo_cabc_vid_cmds));
                #endif
        	}	
        	else if(CMI_OTM1282A == g_panel_id)
             {
                #if CMI_CABC_ENABLED             
        		mipi_dsi_cmds_tx(cmi_cabc_vid_cmds, \
        			ARRAY_SIZE(cmi_cabc_vid_cmds));             
                #endif
             }
        	else if(TM_NT35590 == g_panel_id)
             {
                 #if TM_CABC_ENABLED             
        		mipi_dsi_cmds_tx(tm_cabc_vid_cmds, \
        			ARRAY_SIZE(tm_cabc_vid_cmds));             
                 #endif
             }
		break;
        
	case CABC_OFF:
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}

static unsigned int g_csc_value[9];
static unsigned int g_is_csc_set;
static struct semaphore ct_sem;

static void cmi_store_ct_cscValue(unsigned int csc_value[])
{
    down(&ct_sem);
	g_csc_value [0] = csc_value[0];
	g_csc_value [1] = csc_value[1];
	g_csc_value [2] = csc_value[2];
	g_csc_value [3] = csc_value[3];
	g_csc_value [4] = csc_value[4];
	g_csc_value [5] = csc_value[5];
	g_csc_value [6] = csc_value[6];
	g_csc_value [7] = csc_value[7];
	g_csc_value [8] = csc_value[8];
	g_is_csc_set = 1;
	up(&ct_sem);

	return;
}

static int cmi_set_ct_cscValue(struct k3_fb_data_type *k3fd)
{
	u32 edc_base = 0;
	edc_base = k3fd->edc_base;
	down(&ct_sem);
	if (1 == g_is_csc_set) {
		set_reg(edc_base + 0x400, 0x1, 1, 27);

		set_reg(edc_base + 0x408, g_csc_value[0], 13, 0);
		set_reg(edc_base + 0x408, g_csc_value[1], 13, 16);
		set_reg(edc_base + 0x40C, g_csc_value[2], 13, 0);
		set_reg(edc_base + 0x40C, g_csc_value[3], 13, 16);
		set_reg(edc_base + 0x410, g_csc_value[4], 13, 0);
		set_reg(edc_base + 0x410, g_csc_value[5], 13, 16);
		set_reg(edc_base + 0x414, g_csc_value[6], 13, 0);
		set_reg(edc_base + 0x414, g_csc_value[7], 13, 16);
		set_reg(edc_base + 0x418, g_csc_value[8], 13, 0);
	}
	up(&ct_sem);

	return 0;
}

static int cmi_set_color_temperature(struct lcd_tuning_dev *ltd, unsigned int csc_value[])
{

	int flag = 0;
	struct platform_device *pdev;
	struct k3_fb_data_type *k3fd;

	if (ltd == NULL) {
		return -1;
	}
	pdev = (struct platform_device *)(ltd->data);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);

	if (k3fd == NULL) {
		return -1;
	}

	cmi_store_ct_cscValue(csc_value);
	flag = cmi_set_ct_cscValue(k3fd);
	return flag;

}

static struct lcd_tuning_ops sp_tuning_ops = {
	.set_gamma = cmi_set_gamma,
	.set_cabc = cmi_set_cabc,
	.set_color_temperature = cmi_set_color_temperature,
};

static ssize_t cmi_lcd_info_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret = 0;
	struct k3_panel_info *pinfo = NULL;
	pinfo = cmi_panel_data.panel_info;
	
	if(AUTO_OTM1282A == g_panel_id)
	{
		snprintf(buf, PAGE_SIZE, "auo_OTM1282A 5.0' CMD TFT %d x %d\n",
			pinfo->xres, pinfo->yres);
	}	
	else if(CMI_OTM1282A == g_panel_id)
	{	
		snprintf(buf, PAGE_SIZE, "cmi_OTM1282A 5.0' CMD TFT %d x %d\n",
			pinfo->xres, pinfo->yres);	
	}
	else if(TM_NT35590 == g_panel_id)
	{
		snprintf(buf, PAGE_SIZE, "TM_NT35590 5.0' CMD TFT %d x %d\n",
			pinfo->xres, pinfo->yres);
	}
	else	//if there is no panel id matched, just display UNKOWN
	{
		snprintf(buf, PAGE_SIZE, "UNKOWN 5.0' CMD TFT %d x %d\n",
			pinfo->xres, pinfo->yres);			
	}
	
	ret = strlen(buf) + 1;
	return ret;
}

static ssize_t show_cabc_mode(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", cabc_mode);
}

static ssize_t store_cabc_mode(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int ret = 0;
	unsigned long val = 0;
      
	ret = strict_strtoul(buf, 0, &val);
	if (ret)
		return ret;

	if (val == 1) {
		/* allow application to set cabc mode to ui mode */
		cabc_mode =1;
		cmi_set_cabc(p_tuning_dev, CABC_UI);
	} else if (val == 2) {
		/* force cabc mode to video mode */
		cabc_mode =2;
		cmi_set_cabc(p_tuning_dev, CABC_VID);
	}

	return sprintf((char *)buf, "%d\n", cabc_mode);
}

static ssize_t cmi_frame_count_show(struct device *dev,
       struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", frame_count);
}

static DEVICE_ATTR(lcd_info, S_IRUGO, cmi_lcd_info_show, NULL);
static DEVICE_ATTR(cabc_mode, 0644, show_cabc_mode, store_cabc_mode);
static DEVICE_ATTR(frame_count, S_IRUGO, cmi_frame_count_show, NULL);

static struct attribute *cmi_attrs[] = {
	&dev_attr_lcd_info.attr,
	&dev_attr_cabc_mode.attr,
	&dev_attr_frame_count.attr,
	NULL,
};

static struct attribute_group cmi_attr_group = {
	.attrs = cmi_attrs,
};

static int cmi_sysfs_init(struct platform_device *pdev)
{
	int ret = 0;
	ret = sysfs_create_group(&pdev->dev.kobj, &cmi_attr_group);
	if (ret) {
		k3fb_loge("create sysfs file failed!\n");
		return ret;
	}
	return 0;
}

static void cmi_sysfs_deinit(struct platform_device *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &cmi_attr_group);
}

#ifdef CONFIG_FB_CHECK_MIPI_TR
static unsigned int runingtest_error_count = 0;
static int mipi_cmi_check_mipi_tr(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
	int err = 0; //ok
	u32 read_value;
	int ret = 0;

	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);

       //disable esd check to avoid confliction between esd_check and running test
       if(1 == k3fd->panel_info.esd_enable) //if esd check enabled, just to disable it when runing test
       {
            k3fd->panel_info.esd_enable = 0;
            pr_info("%s disable esd check when runing test \n", __func__);
       }

	//change cmd to page0, modify error in runing-test 
	if(TM_NT35590 == g_panel_id)
	{
		mipi_dsi_cmds_tx(tm_NT35590_cmd_page0, \
			ARRAY_SIZE(tm_NT35590_cmd_page0));
	}
	   
	//read 0x0a
	set_MIPIDSI_GEN_HDR((0x1 << 8) | 0x37);
	set_MIPIDSI_GEN_HDR((0x0a << 8) | 0x14);
	ret = mipi_dsi_wait_and_read(k3fd, &read_value, WAIT_TYPE_MS, 1);
	if (ret || (0x9c != (read_value&0xFF))) {
		err = 1;
		pr_info("%s read 0x0a success:%d, 0x0a value = 0x%x\n", __func__, ret, read_value);
		goto cmi_check_mipi_tr_error;
	}

	//read 0x0b
	set_MIPIDSI_GEN_HDR((0x1 << 8) | 0x37);
	set_MIPIDSI_GEN_HDR((0x0b << 8) | 0x14);
	ret = mipi_dsi_wait_and_read(k3fd, &read_value, WAIT_TYPE_MS, 1);
	if (ret || (0x00 != (read_value&0xFF))) {
		err = 1;
		pr_info("%s read 0x0b success:%d, 0x0b value = 0x%x\n", __func__, ret, read_value);
		goto cmi_check_mipi_tr_error;
	}

	//read 0x0c
	read_value = 0;
	set_MIPIDSI_GEN_HDR((0x1 << 8) | 0x37);
	set_MIPIDSI_GEN_HDR((0x0c << 8) | 0x14);
	ret = mipi_dsi_wait_and_read(k3fd, &read_value, WAIT_TYPE_MS, 1);
	//as NT35590 reg 0x0c returns 0x77 when LCD works well, so add new ret check here
	if(TM_NT35590 == g_panel_id)
	{	
		if (ret || (0x77 != (read_value&0xFF))) {
			err = 1;
			pr_info("%s read 0x0c success:%d, 0x0c value = 0x%x\n", __func__, ret, read_value);
			goto cmi_check_mipi_tr_error;
		}
	}
	else
	{	
		if (ret || (0x07 != (read_value&0xFF))) {
			err = 1;
			pr_info("%s read 0x0c success:%d, 0x0c value = 0x%x\n", __func__, ret, read_value);
			goto cmi_check_mipi_tr_error;
		}
	}
	
	//read 0x0d
	read_value = 0;
	set_MIPIDSI_GEN_HDR((0x1 << 8) | 0x37);
	set_MIPIDSI_GEN_HDR((0x0d << 8) | 0x14);
	ret = mipi_dsi_wait_and_read(k3fd, &read_value, WAIT_TYPE_MS, 1);
	if (ret || (0x00 != (read_value&0xFF))) {
		err = 1;
		pr_info("%s read 0x0d success:%d, 0x0d value = 0x%x\n", __func__, ret, read_value);
		goto cmi_check_mipi_tr_error;
	}

cmi_check_mipi_tr_error:

       if(1 == err) //increase count when there is some reg read error
       {
            runingtest_error_count++;
       }
       
       printk("%s: reg runingtest_error_count = %d\n", __func__, runingtest_error_count);
       if(runingtest_error_count >= 3) //report esd error when there are 3 times error in total
       {
          //clear error count 
          runingtest_error_count = 0;
          printk("%s: reg check result = %d\n", __func__, err);   
 	    return err;
       }
       else
       {
          err = 0;  //reset err to OK as error count less than 3 times
          return err;
       }
      
}
#endif

#ifdef CONFIG_FB_SET_INVERSION
static char orise_shift_0xb3[] = {
	0x00,
	0xb3,
};

static char inversion_mode_1dot[] = {
	0xc0,
	0x22,
};

static char inversion_mode_column[] = {
	0xc0,
	0x33,
};

static char tm_mode_ff_05[] = {
	0xff,
	0x05,
};
static char tm_mode_fb_01[] = {
	0xfb,
	0x01,
};
static char tm_mode_1dot_22[] = {
	0x22,
	0x00,
};
static char tm_mode_1dot_23[] = {	
	0x23,
	0x08,
};
static char tm_mode_column_22[] = {
	0x22,
	0x55,
};
static char tm_mode_column_23[] = {	
	0x23,
	0x0D,
};

static struct dsi_cmd_desc  otm1282a_lcd_inversion_type_1dot[] = {
	{DTYPE_GEN_LWRITE, 0, 200, WAIT_TYPE_US,
		sizeof(orise_shift_0xb3), orise_shift_0xb3},
	{DTYPE_GEN_WRITE2, 0, 200, WAIT_TYPE_US,
		sizeof(inversion_mode_1dot), inversion_mode_1dot},
};

static struct dsi_cmd_desc  otm1282a_lcd_inversion_type_column[] = {
	{DTYPE_GEN_LWRITE, 0, 200, WAIT_TYPE_US,
		sizeof(orise_shift_0xb3), orise_shift_0xb3},
	{DTYPE_GEN_WRITE2, 0, 200, WAIT_TYPE_US,
		sizeof(inversion_mode_column), inversion_mode_column},
};

static struct dsi_cmd_desc  tm_NT35590_lcd_inversion_type_1dot[] = {
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_ff_05), tm_mode_ff_05},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_fb_01), tm_mode_fb_01},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_1dot_22), tm_mode_1dot_22},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_1dot_23), tm_mode_1dot_23},
};
static struct dsi_cmd_desc  tm_NT35590_lcd_inversion_type_column[] = {
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_ff_05), tm_mode_ff_05},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_fb_01), tm_mode_fb_01},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_column_22), tm_mode_column_22},
	{DTYPE_DCS_WRITE1, 0, 200, WAIT_TYPE_US,
		sizeof(tm_mode_column_23), tm_mode_column_23},
};

static int mipi_cmi_set_inversion_type(struct platform_device *pdev, unsigned int inversion_mode)
{
	int ret = 0;
//add for inversion type set for tianm LCD (NT35590)
	switch (inversion_mode) {
		case INVERSION_TYPE_1DOT:
			if(TM_NT35590 == g_panel_id)
			{
				mipi_dsi_cmds_tx(tm_NT35590_lcd_inversion_type_1dot, \
					ARRAY_SIZE(tm_NT35590_lcd_inversion_type_1dot));
			}
			else
			{
				mipi_dsi_cmds_tx(otm1282a_lcd_inversion_type_1dot, \
					ARRAY_SIZE(otm1282a_lcd_inversion_type_1dot));

			}
			printk("%s INVERSION_TYPE_1DOT\n", __func__);			
			
			ret = 0;
			break;
		case INVERSION_TYPE_COLUMN:
			if(TM_NT35590 == g_panel_id)
			{
				mipi_dsi_cmds_tx(tm_NT35590_lcd_inversion_type_column, \
					ARRAY_SIZE(tm_NT35590_lcd_inversion_type_column));
			}
			else
			{			
				mipi_dsi_cmds_tx(otm1282a_lcd_inversion_type_column, \
					ARRAY_SIZE(otm1282a_lcd_inversion_type_column));
			}
			printk("%s INVERSION_TYPE_COLUMN\n", __func__);
			ret = 0;
			break;
		default:
			printk("%s: change LCD inversion type, fail.\n", __func__);
			ret = -EINVAL;
			break;
	}
	return ret;
}
#endif

static bool backlight_enabled_first_time = true;
static int mipi_cmi_panel_on(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
	struct k3_panel_info *pinfo = NULL;
	u32 edc_base = 0, int_st0 = 0, int_st1 = 0;

	BUG_ON(pdev == NULL);

	pr_info("%s enter succ!\n",__func__);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);
    
	edc_base = k3fd->edc_base;
	pinfo = &(k3fd->panel_info);
	if (pinfo->lcd_init_step == LCD_INIT_POWER_ON) {
		if (!g_display_on) {
			lcd_vddio_enable();
			lcd_iomux_set(NORMAL);
			lcd_gpio_request();
			
			//Unified Timing power down of TM, CMI, AUTO-screen 
			mdelay(5);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_pwm_en, 1);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 1);
			mdelay(10);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_enp, 1);
			mdelay(5);	//TM panel need more delay(5ms) here
			gpio_direction_output(g_panel_platform_data->lcd_gpio_enn, 1);
			mdelay(15);  //TM panel need more delay(15ms) here
			gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 0);
			mdelay(10);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 1);
			mdelay(20);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 0);
			mdelay(5);
			gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 1);
			mdelay(20);
			pinfo->lcd_init_step = LCD_INIT_SEND_SEQUENCE;
		}
        
	} else if (pinfo->lcd_init_step == LCD_INIT_SEND_SEQUENCE) {
		if (!g_display_on) {
			
			cmi_set_ct_cscValue(k3fd);
			
			 if(AUTO_OTM1282A == g_panel_id)
			 {
				mipi_dsi_cmds_tx(auto_display_on_cmds, ARRAY_SIZE(auto_display_on_cmds));
			 }
			 else if(CMI_OTM1282A == g_panel_id)
			 {
			 	mipi_dsi_cmds_tx(cmi_display_on_cmds, ARRAY_SIZE(cmi_display_on_cmds));
			 }
			 else if(TM_NT35590 == g_panel_id)
			 {
				mipi_dsi_cmds_tx(tm_display_on_cmds, ARRAY_SIZE(tm_display_on_cmds));
			 }
			 else
			 {
			 	mipi_dsi_cmds_tx(cmi_display_on_cmds, ARRAY_SIZE(cmi_display_on_cmds));
			 }
			g_display_on = true;
			backlight_log_once = true;
                   backlight_enabled_first_time = true;
		}
	} else {
		k3fb_loge("failed to init lcd!\n");
	}  
    
	int_st0 = inp32(edc_base +  MIPIDSI_INT_ST0_ADDR);
	int_st1 = inp32(edc_base +  MIPIDSI_INT_ST1_ADDR);
/*
	if ((0x00 != int_st0 || 0x00 != int_st1) && (!dsm_client_ocuppy(lcd_dclient))) {
		dsm_client_record(lcd_dclient, "LCD dsi status. int_st0:0x%x,\
int_st1:0x%x.\n", int_st0, int_st1);
		dsm_client_notify(lcd_dclient, DSM_LCD_STATUS_ERROR_NO);
	}
 */   
	return 0;
}

static int mipi_cmi_panel_off(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
	struct k3_panel_info *pinfo = NULL;
	u32 edc_base = 0;
    
	BUG_ON(pdev == NULL);
	pr_info("%s enter succ!\n",__func__);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);
	edc_base = k3fd->edc_base;
	pinfo = &(k3fd->panel_info);
    
#ifdef CONFIG_DEBUG_FS
	if ((g_fb_lowpower_debug_flag & DEBUG_LCD_LOWPOWER_DISABLE) == DEBUG_LCD_LOWPOWER_DISABLE) {
		k3fb_logi(" lcd off was disable");
		return 0;
	}
#endif
	if (g_display_on) {
		g_display_on = false;
        
             //clear flag
             backlight_enabled_first_time = false;

		if(AUTO_OTM1282A == g_panel_id)
		{
			mipi_dsi_cmds_tx(auto_display_off_cmds, ARRAY_SIZE(auto_display_off_cmds));
		}
		else if(CMI_OTM1282A == g_panel_id)
		{
			 mipi_dsi_cmds_tx(cmi_display_off_cmds, ARRAY_SIZE(cmi_display_off_cmds));
		}
		else if(TM_NT35590 == g_panel_id)
		{
			 mipi_dsi_cmds_tx(tm_display_off_cmds, ARRAY_SIZE(tm_display_off_cmds));
		}
		else
		{
			 mipi_dsi_cmds_tx(cmi_display_off_cmds, ARRAY_SIZE(cmi_display_off_cmds));
		}
	    //Unified Timing power down of TM, CMI, AUTO-screen 
		gpio_direction_output(g_panel_platform_data->lcd_gpio_reset, 0);
		mdelay(10);
		gpio_direction_output(g_panel_platform_data->lcd_gpio_pwm_en, 0);
		gpio_direction_output(g_panel_platform_data->lcd_gpio_enn, 0);
		mdelay(5);
		gpio_direction_output(g_panel_platform_data->lcd_gpio_enp, 0);
		mdelay(15);
		mdelay(1);
		lcd_gpio_free();
#ifdef CONFIG_MACH_HI6620OEM	
		lcd_iomux_set(LOWPOWER);
#endif
	    mdelay(1);
		lcd_vddio_disable();
	}
	return 0;
}

static int mipi_cmi_panel_remove(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;

	BUG_ON(pdev == NULL);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	/*BUG_ON(k3fd == NULL);*/
	if (!k3fd) {
		return 0;
	}

	if (k3fd->panel_info.bl_set_type & BL_SET_BY_PWM) {
		PWM_CLK_PUT(&(k3fd->panel_info));
	}
	lcd_vddio_put();
	cmi_sysfs_deinit(pdev);
	if (NULL != g_panel_platform_data) {
		kfree(g_panel_platform_data);
	}
	return 0;
}

static int mipi_cmi_panel_set_backlight(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
	u32 edc_base = 0;
	u32 level = 0;

	char bl_level_adjust[2] = {
		0x51,
		0x00,
	};
	
       char normal_display_on[1] = {
             0x13,
        };

	struct dsi_cmd_desc  cmi_bl_level_adjust[] = {
		{DTYPE_DCS_WRITE1, 0, 100, WAIT_TYPE_US,
			sizeof(bl_level_adjust), bl_level_adjust},
	};

	struct dsi_cmd_desc OTM1282A_normal_display_on[] = {
		{DTYPE_DCS_WRITE, 0, 10, WAIT_TYPE_US,
			sizeof(normal_display_on), normal_display_on},
	};

      
	BUG_ON(pdev == NULL);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);
	edc_base = k3fd->edc_base;

       if(true == backlight_enabled_first_time)
       {
             //to clear flag
             backlight_enabled_first_time = false;
            
       	if(CMI_OTM1282A == g_panel_id)
       	{
       		normal_display_on[0] = cmi_normal_display_on[0];
       	}
       	else if(AUTO_OTM1282A == g_panel_id)
       	{
       		normal_display_on[0] = auto_normal_display_on[0];
       	}
       	else
       	{
       		normal_display_on[0] = cmi_normal_display_on[0];
       	}
               
       	mipi_dsi_cmds_tx(OTM1282A_normal_display_on, \
       		ARRAY_SIZE(OTM1282A_normal_display_on));

            
       }    

//as AUO LCD brightness exceeds criterion, here just to lower level set in chip by ratio
      if(AUTO_OTM1282A == g_panel_id)
        {
            level = (u32)(k3fd->bl_level*8 /10);
        }
      else
        {
        	level = k3fd->bl_level;
        }
 
//the solution below just to avoid backlight dithering as chip bug, now the bug fixed on chip, so disable it here 
#if 0
    //backlight may turn off when bl_level is below 6.
    /*Our eyes are more sensitive to small brightness.
	So we adjust the brightness of lcd following iphone4
	---the original description for >248*/
	if (level < 6 && level != 0) {
		level = 6;
	} else if (level >= 29 && level <= 34) {
		level = 35;
	} else if (level >= 59 && level <= 69) {
		level = 70;
	} else if (level >= 123 && level <= 133) {
		level = 134;
	} else if (level > 248) {
		level = 248;
	}
#endif    

	bl_level_adjust[1] = level;

	mipi_dsi_cmds_tx(cmi_bl_level_adjust, \
		ARRAY_SIZE(cmi_bl_level_adjust));

	if (backlight_log_once) {
		backlight_log_once = false;
		k3fb_loge("----k3fd->bl_level=%d,set backlight to level = %d\n",k3fd->bl_level, level);
	}
	return 0;
}

unsigned char which_lcd_module_triple(void)
{
    unsigned char  read0 = 0;
    unsigned char  read1 = 0;
    unsigned char  lcd_id0 = 0;
    unsigned char  lcd_id1 = 0;
    unsigned char  lcd_id = 0;
    unsigned int ret = 0;

    //ÉèÖÃÊäÈë
    ret = gpio_direction_input(g_panel_platform_data->lcd_gpio_id0);
    if(0 != ret)
    {
        printk("set_gpio_error1_%s\n", __func__);
    }
    ret = gpio_direction_input(g_panel_platform_data->lcd_gpio_id1);
    if(0 != ret)
    {
        printk("set_gpio_error2_%s\n", __func__);
    }
    //ÑÓÊ±
    msleep(100);
    //¶Á×´Ì¬
    read0 = gpio_get_value(g_panel_platform_data->lcd_gpio_id0);
    read1 = gpio_get_value(g_panel_platform_data->lcd_gpio_id1);

    if(LCD_HW_ID_STATUS_LOW == read0)
    {
        lcd_id0 = LCD_HW_ID_STATUS_LOW;
    }
    else if(LCD_HW_ID_STATUS_HIGH == read0)
    {
        lcd_id0 = LCD_HW_ID_STATUS_HIGH;
    }
    if(LCD_HW_ID_STATUS_LOW == read1)
    {
        lcd_id1 = LCD_HW_ID_STATUS_LOW;
    }
    else if(LCD_HW_ID_STATUS_HIGH == read1)
    {
        lcd_id1 = LCD_HW_ID_STATUS_HIGH;
    }
    lcd_id =  lcd_id0 | (lcd_id1 << 1);

    return lcd_id;
}

static int mipi_cmi_panel_set_fastboot(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
    
	BUG_ON(pdev == NULL);

	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);

	lcd_vddio_enable();
	lcd_iomux_set(NORMAL);
	lcd_gpio_request();

	if (k3fd->panel_info.bl_set_type & BL_SET_BY_PWM) {
		PWM_IOMUX_SET(&(k3fd->panel_info), NORMAL);
		PWM_GPIO_REQUEST(&(k3fd->panel_info));
	}

	g_display_on = true;

	g_panel_id = which_lcd_module_triple();

      if(CMI_OTM1282A == g_panel_id)
      {
          k3fb_logi("LCD Module is CMI_OTM1282A\n");
      }
      else if(AUTO_OTM1282A == g_panel_id)
      {
          k3fb_logi("LCD Module is AUTO_OTM1282A\n");
      }
      else if(TM_NT35590 == g_panel_id)
      {
          k3fb_logi("LCD Module is TM_NT35590\n");
		  //open esd function
      }	  
      else
      {
          k3fb_logi("LCD Module is UNKOWN\n");
      }
	return 0;
}

static int mipi_cmi_panel_set_cabc(struct platform_device *pdev, int value)
{
	u32 edc_base = 0;
	struct k3_fb_data_type *k3fd = NULL;

	BUG_ON(pdev == NULL);
	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);
	edc_base = k3fd->edc_base;

#if 0
	if (value) {
		outp32(edc_base + MIPIDSI_GEN_HDR_ADDR, 0x0dbb23);
	} else {
		outp32(edc_base + MIPIDSI_GEN_HDR_ADDR, 0x0cbb23);
	}
#endif

	return 0;
}

static int mipi_dsi_wait_and_read_reg(struct k3_fb_data_type *k3fd, u32 *read_value, int wait_time)
{
	bool is_timeout = 1;
	int delay_count = 0;
	u32 tmp = 0;
	int nRet = 0;

	/* check command fifo empty */
	while (1) {
		tmp = inp32(k3fd->dsi_base + MIPIDSI_CMD_PKT_STATUS_ADDR);
		if (((tmp & 0x00000001) == 0x00000001) || delay_count > 100) {
			is_timeout = (delay_count > 100) ? true : false;
			delay_count = 0;
			break;
		} else {
			udelay(wait_time);
			++delay_count;
		}
	}	
	
	/* check read command finish */
	while (1) {
		tmp = inp32(k3fd->dsi_base + MIPIDSI_CMD_PKT_STATUS_ADDR);
		if (((tmp & 0x00000040) != 0x00000040) || delay_count > 100) {
			is_timeout = (delay_count > 100) ? true : false;
			delay_count = 0;
			break;
		} else {
			udelay(wait_time);
			++delay_count;
		}
	}	
    
	if (false == is_timeout) {
		*read_value = inp32(k3fd->dsi_base + MIPIDSI_GEN_PLD_DATA_ADDR);
	} else {
		*read_value = 0;
		nRet = -1;
		pr_info("%s: read value error\n", __func__);
	}

	return nRet;
}


static int mipi_cmi_panel_check_esd(struct platform_device *pdev)
{
	struct k3_fb_data_type *k3fd = NULL;
	int err = 0; //ok
	u32 read_value;
	int ret = 0;

	k3fd = (struct k3_fb_data_type *)platform_get_drvdata(pdev);
	BUG_ON(k3fd == NULL);

	
	k3fb_logi("enter panel id = %d\n", g_panel_id);
	//change cmd to page0, open tianma LCD esd function 
	if(TM_NT35590 == g_panel_id)
	{
		mipi_dsi_cmds_tx(tm_NT35590_cmd_page0, \
			ARRAY_SIZE(tm_NT35590_cmd_page0));
	}

	//read 0x0a 	
	set_MIPIDSI_GEN_HDR((0x1 << 8) | 0x37);
	set_MIPIDSI_GEN_HDR((0x0a << 8) | 0x14);
	ret = mipi_dsi_wait_and_read_reg(k3fd, &read_value, 1);
    
/*	if (ret || (0x9c != (read_value&0xFF))) {
		err = 1;
            if (!dsm_client_ocuppy(lcd_dclient)) {
                dsm_client_record(lcd_dclient, "k3fb, %s: esd reg 0x0A check failed\
    			!!!\n", __func__);
                dsm_client_notify(lcd_dclient, DSM_LCD_ESD_ERROR_NO);
            }   
			     
		pr_info("%s read 0x0a success:%d, 0x0a value = 0x%x\n", __func__, ret, read_value);
		goto cmi_check_mipi_tr_error;
	}
*/

cmi_check_mipi_tr_error:
	return err;
}

static struct k3_panel_info cmi_panel_info = {0};
static struct k3_fb_panel_data cmi_panel_data = {
	.panel_info = &cmi_panel_info,
	.on = mipi_cmi_panel_on,
	.off = mipi_cmi_panel_off,
	.remove = mipi_cmi_panel_remove,
	.set_backlight = mipi_cmi_panel_set_backlight,
	.set_fastboot = mipi_cmi_panel_set_fastboot,
	.set_cabc = mipi_cmi_panel_set_cabc,
	.check_esd = mipi_cmi_panel_check_esd,
#ifdef CONFIG_FB_CHECK_MIPI_TR
	.check_mipi_tr = mipi_cmi_check_mipi_tr,
#endif
#ifdef CONFIG_FB_SET_INVERSION
	.set_inversion_type = mipi_cmi_set_inversion_type,
#endif
};

static int __devinit cmi_probe(struct platform_device *pdev)
{
	struct k3_panel_info *pinfo = NULL;
	struct resource *res = NULL;
	struct platform_device *reg_pdev = NULL;
	struct lcd_properities lcd_props;

	pr_info("%s enter succ!\n",__func__);
	g_panel_platform_data = kzalloc(sizeof(struct cmi_otm1282b_panel_data), GFP_KERNEL);
	if (NULL == g_panel_platform_data) {
		k3fb_loge("alloc driver data error\n");
		return -ENOMEM;
	}
	g_display_on = false;
	pinfo = cmi_panel_data.panel_info;

	/* init lcd panel info */
	pinfo->xres = 720;
	pinfo->yres = 1280;
	pinfo->width  = 68;  //mm
	pinfo->height = 136; //mm
	pinfo->type = PANEL_MIPI_CMD;
	pinfo->orientation = LCD_PORTRAIT;
	pinfo->bpp = EDC_OUT_RGB_888;
	pinfo->s3d_frm = EDC_FRM_FMT_2D;
	pinfo->bgr_fmt = EDC_RGB;
	pinfo->bl_set_type = BL_SET_BY_MIPI;
	pinfo->bl_max = 100;
	pinfo->bl_min = 1;
	pinfo->frc_enable = 1;
	if(NULL != strstr(saved_command_line, "androidboot.swtype=factory"))
		pinfo->esd_enable = 0;
	else
		pinfo->esd_enable = 1;      
	pr_info("esd_enable =%d\n", pinfo->esd_enable);
	pinfo->sbl_enable = 1;

	pinfo->sbl.bl_max = 0xff;
	pinfo->sbl.cal_a = 0x08;
	pinfo->sbl.cal_b = 0xd8;
	pinfo->sbl.str_limit = 0x40;

	pinfo->ldi.h_back_porch = 43;
	pinfo->ldi.h_front_porch = 97;
	pinfo->ldi.h_pulse_width = 57;
	pinfo->ldi.v_back_porch = 12;
	pinfo->ldi.v_front_porch = 14;
	pinfo->ldi.v_pulse_width = 2;

	pinfo->ldi.hsync_plr = 1;
	pinfo->ldi.vsync_plr = 0;
	pinfo->ldi.pixelclk_plr = 1;
	pinfo->ldi.data_en_plr = 0;

	pinfo->ldi.disp_mode = LDI_DISP_MODE_NOT_3D_FBF;

	/* Note: must init here */	
	pinfo->frame_rate = 60;
	pinfo->clk_rate = 76000000; // 72M = 1440M/20

	pinfo->mipi.lane_nums = DSI_4_LANES;
	pinfo->mipi.color_mode = DSI_24BITS_1;
	pinfo->mipi.vc = 0;

	pinfo->mipi.dsi_bit_clk = 241;


	lcd_vddio_get(pdev);
	lcd_iomux_get();
 
	/* lcd resource */
	lcd_resource(pdev, res);
	/* alloc panel device data */
	if (platform_device_add_data(pdev, &cmi_panel_data,
		sizeof(struct k3_fb_panel_data))) {
		k3fb_loge("platform_device_add_data failed!\n");
		platform_device_put(pdev);
		return -ENOMEM;
	}

	reg_pdev = k3_fb_add_device(pdev);

	sema_init(&ct_sem, 1);
	g_csc_value[0] = 0;
	g_csc_value[1] = 0;
	g_csc_value[2] = 0;
	g_csc_value[3] = 0;
	g_csc_value[4] = 0;
	g_csc_value[5] = 0;
	g_csc_value[6] = 0;
	g_csc_value[7] = 0;
	g_csc_value[8] = 0;
	g_is_csc_set = 0;
	/* for cabc */
	lcd_props.type = TFT;
	lcd_props.default_gamma = GAMMA25;
	p_tuning_dev = lcd_tuning_dev_register(&lcd_props, &sp_tuning_ops, (void *)reg_pdev);
	if (IS_ERR(p_tuning_dev)) {
		k3fb_loge("lcd_tuning_dev_register failed!\n");
		return -1;
	}

	cmi_sysfs_init(pdev);

	pr_info("%s exit succ!\n",__func__);
	return 0;
}

static struct platform_driver this_driver = {
	.probe = cmi_probe,
	.remove = NULL,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
	.driver = {
		.name = "mipi_cmi_OTM1282A",
	},
};

static int __init mipi_cmi_panel_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&this_driver);
	if (ret) {
		k3fb_loge("not able to register the driver\n");
		return ret;
	}
	return ret;
}

module_init(mipi_cmi_panel_init);
