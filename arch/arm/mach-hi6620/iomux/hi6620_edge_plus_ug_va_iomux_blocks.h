/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
 
  This is an AUTO GENERATED file ! DON'T MODIFY MANUAL! 
  文 件 名   : hi6620_edge_plus_ug_va_iomux_blocks.h
  生成日期   : 2015年1月6日
  
******************************************************************************/


#ifndef __HI6620_EDGE_PLUS_UG_VA_IOMUX_BLOCKS_H__
#define __HI6620_EDGE_PLUS_UG_VA_IOMUX_BLOCKS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "iomux.h"
#include "hi6620_edge_plus_ug_va_iomux_pins.h"
#include <hsad/config_interface.h>

extern struct iomux_ops iomux_block_ops;

#define IOMUX_BLOCK(_iomux_block, _block_name, _block_func, _pins)   \
struct iomux_block _iomux_block = {\
    .block_name  = _block_name,\
    .block_func   =  _block_func,\
    .pins = _pins,\
    .ops = &iomux_block_ops,\
    .init = 0, \
};

/*TODO:every config of blocks*/
#define BLOCK_CONFIG(_name, _block, _configarray) \
{\
    .name = _name,\
    .block = _block,\
    .config_array = _configarray,\
},


/*charger pins */
static struct  iomux_pin *charger_pins_edge_plus_ug_va[] = {
    &GPIO_4_0_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: charger*/
IOMUX_BLOCK(block_charger_edge_plus_ug_va, "block_charger", NORMAL, charger_pins_edge_plus_ug_va)



/*CHARGER*/

static enum lowlayer_func charger_func_normal_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown charger_pullud_normal_edge_plus_ug_va[] = {NOPULL,  -INVALID,};
static enum drive_strength charger_drv_normal_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static enum lowlayer_func charger_func_lowpower_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown charger_pullud_lowpower_edge_plus_ug_va[] = {NOPULL,  -INVALID,};
static enum drive_strength charger_drv_lowpower_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static struct block_config charger_config_edge_plus_ug_va[] = {
    [NORMAL] = {charger_func_normal_edge_plus_ug_va, charger_pullud_normal_edge_plus_ug_va, charger_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {charger_func_lowpower_edge_plus_ug_va, charger_pullud_lowpower_edge_plus_ug_va, charger_drv_lowpower_edge_plus_ug_va},

};

/*flash pins */
static struct  iomux_pin *flash_pins_edge_plus_ug_va[] = {
    &ISP_GPIO5_EDGE_PLUS_UG_VA, &RF_GPIO_0_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: flash*/
IOMUX_BLOCK(block_flash_edge_plus_ug_va, "block_flash", NORMAL, flash_pins_edge_plus_ug_va)



/*FLASH*/

static enum lowlayer_func flash_func_normal_edge_plus_ug_va[] = {FUNC2, FUNC0,  -INVALID,};
static enum pull_updown flash_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength flash_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func flash_func_lowpower_edge_plus_ug_va[] = {FUNC2, FUNC0,  -INVALID,};
static enum pull_updown flash_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength flash_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config flash_config_edge_plus_ug_va[] = {
    [NORMAL] = {flash_func_normal_edge_plus_ug_va, flash_pullud_normal_edge_plus_ug_va, flash_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {flash_func_lowpower_edge_plus_ug_va, flash_pullud_lowpower_edge_plus_ug_va, flash_drv_lowpower_edge_plus_ug_va},

};

/*bt pins */
static struct  iomux_pin *bt_pins_edge_plus_ug_va[] = {
    &UART4_CTS_N_EDGE_PLUS_UG_VA, &UART4_RTS_N_EDGE_PLUS_UG_VA, &UART4_RXD_EDGE_PLUS_UG_VA, &UART4_TXD_EDGE_PLUS_UG_VA, &GPIO_15_5_EDGE_PLUS_UG_VA, &GPIO_15_7_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: bt*/
IOMUX_BLOCK(block_bt_edge_plus_ug_va, "block_bt", NORMAL, bt_pins_edge_plus_ug_va)



/*BT*/

static enum lowlayer_func bt_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown bt_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength bt_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func bt_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC4, FUNC0, FUNC4, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown bt_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, PULLUP, NOPULL, PULLUP, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength bt_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config bt_config_edge_plus_ug_va[] = {
    [NORMAL] = {bt_func_normal_edge_plus_ug_va, bt_pullud_normal_edge_plus_ug_va, bt_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {bt_func_lowpower_edge_plus_ug_va, bt_pullud_lowpower_edge_plus_ug_va, bt_drv_lowpower_edge_plus_ug_va},

};

/*scamera pins */
static struct  iomux_pin *scamera_pins_edge_plus_ug_va[] = {
    &ISP_GPIO1_EDGE_PLUS_UG_VA, &ISP_GPIO4_EDGE_PLUS_UG_VA, &ISP_RESETB1_EDGE_PLUS_UG_VA, &ISP_CCLK1_EDGE_PLUS_UG_VA, &ISP_SDA1_EDGE_PLUS_UG_VA, &ISP_SCL1_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: scamera*/
IOMUX_BLOCK(block_scamera_edge_plus_ug_va, "block_scamera", NORMAL, scamera_pins_edge_plus_ug_va)



/*SCAMERA*/

static enum lowlayer_func scamera_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC2, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown scamera_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength scamera_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func scamera_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC2, FUNC0, FUNC0, FUNC2, FUNC2,  -INVALID,};
static enum pull_updown scamera_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, PULLDOWN, PULLDOWN,  -INVALID,};
static enum drive_strength scamera_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config scamera_config_edge_plus_ug_va[] = {
    [NORMAL] = {scamera_func_normal_edge_plus_ug_va, scamera_pullud_normal_edge_plus_ug_va, scamera_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {scamera_func_lowpower_edge_plus_ug_va, scamera_pullud_lowpower_edge_plus_ug_va, scamera_drv_lowpower_edge_plus_ug_va},

};

/*codec pins */
static struct  iomux_pin *codec_pins_edge_plus_ug_va[] = {
    &PCM1_XCLK_EDGE_PLUS_UG_VA, &PCM1_XFS_EDGE_PLUS_UG_VA, &PCM1_DI_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: codec*/
IOMUX_BLOCK(block_codec_edge_plus_ug_va, "block_codec", NORMAL, codec_pins_edge_plus_ug_va)



/*CODEC*/

static enum lowlayer_func codec_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown codec_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength codec_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func codec_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown codec_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength codec_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config codec_config_edge_plus_ug_va[] = {
    [NORMAL] = {codec_func_normal_edge_plus_ug_va, codec_pullud_normal_edge_plus_ug_va, codec_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {codec_func_lowpower_edge_plus_ug_va, codec_pullud_lowpower_edge_plus_ug_va, codec_drv_lowpower_edge_plus_ug_va},

};

/*gpa1 pins */
static struct  iomux_pin *gpa1_pins_edge_plus_ug_va[] = {
    &RF_GPIO_22_EDGE_PLUS_UG_VA, &RF_GPIO_23_EDGE_PLUS_UG_VA, &RF_GPIO_26_EDGE_PLUS_UG_VA, &RF_GPIO_28_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: gpa1*/
IOMUX_BLOCK(block_gpa1_edge_plus_ug_va, "block_gpa1", NORMAL, gpa1_pins_edge_plus_ug_va)



/*GPA1*/

static enum lowlayer_func gpa1_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown gpa1_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength gpa1_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func gpa1_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown gpa1_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength gpa1_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config gpa1_config_edge_plus_ug_va[] = {
    [NORMAL] = {gpa1_func_normal_edge_plus_ug_va, gpa1_pullud_normal_edge_plus_ug_va, gpa1_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {gpa1_func_lowpower_edge_plus_ug_va, gpa1_pullud_lowpower_edge_plus_ug_va, gpa1_drv_lowpower_edge_plus_ug_va},

};

/*pmu pins */
static struct  iomux_pin *pmu_pins_edge_plus_ug_va[] = {
    &HKADC_SSI_EDGE_PLUS_UG_VA, &AUX_SSI0_EDGE_PLUS_UG_VA, &AUX_SSI1_EDGE_PLUS_UG_VA, &GPIO_5_0_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: pmu*/
IOMUX_BLOCK(block_pmu_edge_plus_ug_va, "block_pmu", NORMAL, pmu_pins_edge_plus_ug_va)



/*PMU*/

static enum lowlayer_func pmu_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC2,  -INVALID,};
static enum pull_updown pmu_pullud_normal_edge_plus_ug_va[] = {PULLDOWN, PULLDOWN, PULLDOWN, NOPULL,  -INVALID,};
static enum drive_strength pmu_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func pmu_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC2,  -INVALID,};
static enum pull_updown pmu_pullud_lowpower_edge_plus_ug_va[] = {PULLDOWN, PULLDOWN, PULLDOWN, NOPULL,  -INVALID,};
static enum drive_strength pmu_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config pmu_config_edge_plus_ug_va[] = {
    [NORMAL] = {pmu_func_normal_edge_plus_ug_va, pmu_pullud_normal_edge_plus_ug_va, pmu_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {pmu_func_lowpower_edge_plus_ug_va, pmu_pullud_lowpower_edge_plus_ug_va, pmu_drv_lowpower_edge_plus_ug_va},

};

/*emmc pins */
static struct  iomux_pin *emmc_pins_edge_plus_ug_va[] = {
    &EMMC_CLK_EDGE_PLUS_UG_VA, &EMMC_CMD_EDGE_PLUS_UG_VA, &EMMC_DATA0_EDGE_PLUS_UG_VA, &EMMC_DATA1_EDGE_PLUS_UG_VA, &EMMC_DATA2_EDGE_PLUS_UG_VA, &EMMC_DATA3_EDGE_PLUS_UG_VA, &EMMC_DATA4_EDGE_PLUS_UG_VA, &EMMC_DATA5_EDGE_PLUS_UG_VA, &EMMC_DATA6_EDGE_PLUS_UG_VA, &EMMC_DATA7_EDGE_PLUS_UG_VA, &GPIO_2_3_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: emmc*/
IOMUX_BLOCK(block_emmc_edge_plus_ug_va, "block_emmc", NORMAL, emmc_pins_edge_plus_ug_va)



/*EMMC*/

static enum lowlayer_func emmc_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC4,  -INVALID,};
static enum pull_updown emmc_pullud_normal_edge_plus_ug_va[] = {NOPULL, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, NOPULL,  -INVALID,};
static enum drive_strength emmc_drv_normal_edge_plus_ug_va[] = {LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL0,  -INVALID,};

static enum lowlayer_func emmc_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC4,  -INVALID,};
static enum pull_updown emmc_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength emmc_drv_lowpower_edge_plus_ug_va[] = {LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL1, LEVEL0,  -INVALID,};

static struct block_config emmc_config_edge_plus_ug_va[] = {
    [NORMAL] = {emmc_func_normal_edge_plus_ug_va, emmc_pullud_normal_edge_plus_ug_va, emmc_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {emmc_func_lowpower_edge_plus_ug_va, emmc_pullud_lowpower_edge_plus_ug_va, emmc_drv_lowpower_edge_plus_ug_va},

};

/*rf pins */
static struct  iomux_pin *rf_pins_edge_plus_ug_va[] = {
    &RF_TX_EN1_EDGE_PLUS_UG_VA, &RF_TCVR_ON0_EDGE_PLUS_UG_VA, &RF_TCVR_ON1_EDGE_PLUS_UG_VA, &RF_SSI0_EDGE_PLUS_UG_VA, &RF_SSI1_EDGE_PLUS_UG_VA, &RF_GPIO_2_EDGE_PLUS_UG_VA, &RF_GPIO_3_EDGE_PLUS_UG_VA, &RF_GPIO_4_EDGE_PLUS_UG_VA, &RF_GPIO_5_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: rf*/
IOMUX_BLOCK(block_rf_edge_plus_ug_va, "block_rf", NORMAL, rf_pins_edge_plus_ug_va)



/*RF*/

static enum lowlayer_func rf_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown rf_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, PULLDOWN, PULLDOWN, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength rf_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func rf_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown rf_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, PULLDOWN, PULLDOWN, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength rf_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config rf_config_edge_plus_ug_va[] = {
    [NORMAL] = {rf_func_normal_edge_plus_ug_va, rf_pullud_normal_edge_plus_ug_va, rf_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {rf_func_lowpower_edge_plus_ug_va, rf_pullud_lowpower_edge_plus_ug_va, rf_drv_lowpower_edge_plus_ug_va},

};

/*tp pins */
static struct  iomux_pin *tp_pins_edge_plus_ug_va[] = {
    &GPIO_18_7_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: tp*/
IOMUX_BLOCK(block_tp_edge_plus_ug_va, "block_tp", NORMAL, tp_pins_edge_plus_ug_va)



/*TP*/

static enum lowlayer_func tp_func_normal_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown tp_pullud_normal_edge_plus_ug_va[] = {NOPULL,  -INVALID,};
static enum drive_strength tp_drv_normal_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static enum lowlayer_func tp_func_lowpower_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown tp_pullud_lowpower_edge_plus_ug_va[] = {NOPULL,  -INVALID,};
static enum drive_strength tp_drv_lowpower_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static struct block_config tp_config_edge_plus_ug_va[] = {
    [NORMAL] = {tp_func_normal_edge_plus_ug_va, tp_pullud_normal_edge_plus_ug_va, tp_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {tp_func_lowpower_edge_plus_ug_va, tp_pullud_lowpower_edge_plus_ug_va, tp_drv_lowpower_edge_plus_ug_va},

};

/*switch pins */
static struct  iomux_pin *switch_pins_edge_plus_ug_va[] = {
    &GPIO_6_2_EDGE_PLUS_UG_VA, &GPIO_7_7_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: switch*/
IOMUX_BLOCK(block_switch_edge_plus_ug_va, "block_switch", NORMAL, switch_pins_edge_plus_ug_va)



/*SWITCH*/

static enum lowlayer_func switch_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown switch_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength switch_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func switch_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown switch_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength switch_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config switch_config_edge_plus_ug_va[] = {
    [NORMAL] = {switch_func_normal_edge_plus_ug_va, switch_pullud_normal_edge_plus_ug_va, switch_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {switch_func_lowpower_edge_plus_ug_va, switch_pullud_lowpower_edge_plus_ug_va, switch_drv_lowpower_edge_plus_ug_va},

};

/*rfpmu pins */
static struct  iomux_pin *rfpmu_pins_edge_plus_ug_va[] = {
    &RF_MIPI_CLK0_EDGE_PLUS_UG_VA, &RF_MIPI_DATA0_EDGE_PLUS_UG_VA, &APT_PDM0_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: rfpmu*/
IOMUX_BLOCK(block_rfpmu_edge_plus_ug_va, "block_rfpmu", NORMAL, rfpmu_pins_edge_plus_ug_va)



/*RFPMU*/

static enum lowlayer_func rfpmu_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown rfpmu_pullud_normal_edge_plus_ug_va[] = {NOPULL, PULLDOWN, NOPULL,  -INVALID,};
static enum drive_strength rfpmu_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func rfpmu_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown rfpmu_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, PULLDOWN, NOPULL,  -INVALID,};
static enum drive_strength rfpmu_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config rfpmu_config_edge_plus_ug_va[] = {
    [NORMAL] = {rfpmu_func_normal_edge_plus_ug_va, rfpmu_pullud_normal_edge_plus_ug_va, rfpmu_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {rfpmu_func_lowpower_edge_plus_ug_va, rfpmu_pullud_lowpower_edge_plus_ug_va, rfpmu_drv_lowpower_edge_plus_ug_va},

};

/*mmmpa pins */
static struct  iomux_pin *mmmpa_pins_edge_plus_ug_va[] = {
    &RF_GPIO_13_EDGE_PLUS_UG_VA, &RF_GPIO_14_EDGE_PLUS_UG_VA, &RF_GPIO_15_EDGE_PLUS_UG_VA, &RF_GPIO_16_EDGE_PLUS_UG_VA, &RF_GPIO_17_EDGE_PLUS_UG_VA, &RF_GPIO_18_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: mmmpa*/
IOMUX_BLOCK(block_mmmpa_edge_plus_ug_va, "block_mmmpa", NORMAL, mmmpa_pins_edge_plus_ug_va)



/*MMMPA*/

static enum lowlayer_func mmmpa_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown mmmpa_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength mmmpa_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func mmmpa_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown mmmpa_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength mmmpa_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config mmmpa_config_edge_plus_ug_va[] = {
    [NORMAL] = {mmmpa_func_normal_edge_plus_ug_va, mmmpa_pullud_normal_edge_plus_ug_va, mmmpa_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {mmmpa_func_lowpower_edge_plus_ug_va, mmmpa_pullud_lowpower_edge_plus_ug_va, mmmpa_drv_lowpower_edge_plus_ug_va},

};

/*i2c3 pins */
static struct  iomux_pin *i2c3_pins_edge_plus_ug_va[] = {
    &I2C3_SCL_EDGE_PLUS_UG_VA, &I2C3_SDA_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: i2c3*/
IOMUX_BLOCK(block_i2c3_edge_plus_ug_va, "block_i2c3", NORMAL, i2c3_pins_edge_plus_ug_va)



/*I2C3*/

static enum lowlayer_func i2c3_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c3_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c3_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func i2c3_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c3_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c3_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config i2c3_config_edge_plus_ug_va[] = {
    [NORMAL] = {i2c3_func_normal_edge_plus_ug_va, i2c3_pullud_normal_edge_plus_ug_va, i2c3_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {i2c3_func_lowpower_edge_plus_ug_va, i2c3_pullud_lowpower_edge_plus_ug_va, i2c3_drv_lowpower_edge_plus_ug_va},

};

/*usim1 pins */
static struct  iomux_pin *usim1_pins_edge_plus_ug_va[] = {
    &USIM1_CLK_EDGE_PLUS_UG_VA, &USIM1_DATA_EDGE_PLUS_UG_VA, &USIM1_RST_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: usim1*/
IOMUX_BLOCK(block_usim1_edge_plus_ug_va, "block_usim1", NORMAL, usim1_pins_edge_plus_ug_va)



/*USIM1*/

static enum lowlayer_func usim1_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown usim1_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength usim1_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func usim1_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown usim1_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength usim1_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config usim1_config_edge_plus_ug_va[] = {
    [NORMAL] = {usim1_func_normal_edge_plus_ug_va, usim1_pullud_normal_edge_plus_ug_va, usim1_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {usim1_func_lowpower_edge_plus_ug_va, usim1_pullud_lowpower_edge_plus_ug_va, usim1_drv_lowpower_edge_plus_ug_va},

};

/*usim0 pins */
static struct  iomux_pin *usim0_pins_edge_plus_ug_va[] = {
    &USIM0_CLK_EDGE_PLUS_UG_VA, &USIM0_DATA_EDGE_PLUS_UG_VA, &USIM0_RST_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: usim0*/
IOMUX_BLOCK(block_usim0_edge_plus_ug_va, "block_usim0", NORMAL, usim0_pins_edge_plus_ug_va)



/*USIM0*/

static enum lowlayer_func usim0_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown usim0_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength usim0_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func usim0_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown usim0_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength usim0_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config usim0_config_edge_plus_ug_va[] = {
    [NORMAL] = {usim0_func_normal_edge_plus_ug_va, usim0_pullud_normal_edge_plus_ug_va, usim0_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {usim0_func_lowpower_edge_plus_ug_va, usim0_pullud_lowpower_edge_plus_ug_va, usim0_drv_lowpower_edge_plus_ug_va},

};

/*mcamera pins */
static struct  iomux_pin *mcamera_pins_edge_plus_ug_va[] = {
    &ISP_GPIO0_EDGE_PLUS_UG_VA, &ISP_GPIO2_EDGE_PLUS_UG_VA, &ISP_GPIO3_EDGE_PLUS_UG_VA, &ISP_RESETB0_EDGE_PLUS_UG_VA, &ISP_CCLK0_EDGE_PLUS_UG_VA, &ISP_SDA0_EDGE_PLUS_UG_VA, &ISP_SCL0_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: mcamera*/
IOMUX_BLOCK(block_mcamera_edge_plus_ug_va, "block_mcamera", NORMAL, mcamera_pins_edge_plus_ug_va)



/*MCAMERA*/

static enum lowlayer_func mcamera_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC2, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown mcamera_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength mcamera_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func mcamera_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC2, FUNC0, FUNC0, FUNC2, FUNC2,  -INVALID,};
static enum pull_updown mcamera_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, PULLDOWN, PULLDOWN,  -INVALID,};
static enum drive_strength mcamera_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config mcamera_config_edge_plus_ug_va[] = {
    [NORMAL] = {mcamera_func_normal_edge_plus_ug_va, mcamera_pullud_normal_edge_plus_ug_va, mcamera_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {mcamera_func_lowpower_edge_plus_ug_va, mcamera_pullud_lowpower_edge_plus_ug_va, mcamera_drv_lowpower_edge_plus_ug_va},

};

/*i2c0 pins */
static struct  iomux_pin *i2c0_pins_edge_plus_ug_va[] = {
    &I2C0_SCL_EDGE_PLUS_UG_VA, &I2C0_SDA_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: i2c0*/
IOMUX_BLOCK(block_i2c0_edge_plus_ug_va, "block_i2c0", NORMAL, i2c0_pins_edge_plus_ug_va)



/*I2C0*/

static enum lowlayer_func i2c0_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c0_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c0_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func i2c0_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c0_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c0_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config i2c0_config_edge_plus_ug_va[] = {
    [NORMAL] = {i2c0_func_normal_edge_plus_ug_va, i2c0_pullud_normal_edge_plus_ug_va, i2c0_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {i2c0_func_lowpower_edge_plus_ug_va, i2c0_pullud_lowpower_edge_plus_ug_va, i2c0_drv_lowpower_edge_plus_ug_va},

};

/*i2c1 pins */
static struct  iomux_pin *i2c1_pins_edge_plus_ug_va[] = {
    &I2C1_SCL_EDGE_PLUS_UG_VA, &I2C1_SDA_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: i2c1*/
IOMUX_BLOCK(block_i2c1_edge_plus_ug_va, "block_i2c1", NORMAL, i2c1_pins_edge_plus_ug_va)



/*I2C1*/

static enum lowlayer_func i2c1_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c1_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c1_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func i2c1_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c1_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c1_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config i2c1_config_edge_plus_ug_va[] = {
    [NORMAL] = {i2c1_func_normal_edge_plus_ug_va, i2c1_pullud_normal_edge_plus_ug_va, i2c1_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {i2c1_func_lowpower_edge_plus_ug_va, i2c1_pullud_lowpower_edge_plus_ug_va, i2c1_drv_lowpower_edge_plus_ug_va},

};

/*i2c2 pins */
static struct  iomux_pin *i2c2_pins_edge_plus_ug_va[] = {
    &I2C2_SCL_EDGE_PLUS_UG_VA, &I2C2_SDA_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: i2c2*/
IOMUX_BLOCK(block_i2c2_edge_plus_ug_va, "block_i2c2", NORMAL, i2c2_pins_edge_plus_ug_va)



/*I2C2*/

static enum lowlayer_func i2c2_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c2_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c2_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func i2c2_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown i2c2_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength i2c2_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config i2c2_config_edge_plus_ug_va[] = {
    [NORMAL] = {i2c2_func_normal_edge_plus_ug_va, i2c2_pullud_normal_edge_plus_ug_va, i2c2_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {i2c2_func_lowpower_edge_plus_ug_va, i2c2_pullud_lowpower_edge_plus_ug_va, i2c2_drv_lowpower_edge_plus_ug_va},

};

/*gps pins */
static struct  iomux_pin *gps_pins_edge_plus_ug_va[] = {
    &GPIO_6_1_EDGE_PLUS_UG_VA, &RF_GPIO_1_EDGE_PLUS_UG_VA, &UART3_CTS_N_EDGE_PLUS_UG_VA, &UART3_RTS_N_EDGE_PLUS_UG_VA, &UART3_RXD_EDGE_PLUS_UG_VA, &UART3_TXD_EDGE_PLUS_UG_VA, &GPS_CLK_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: gps*/
IOMUX_BLOCK(block_gps_edge_plus_ug_va, "block_gps", NORMAL, gps_pins_edge_plus_ug_va)



/*GPS*/

static enum lowlayer_func gps_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown gps_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength gps_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func gps_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC2,  -INVALID,};
static enum pull_updown gps_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, PULLDOWN, PULLDOWN, PULLUP, PULLUP, NOPULL,  -INVALID,};
static enum drive_strength gps_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config gps_config_edge_plus_ug_va[] = {
    [NORMAL] = {gps_func_normal_edge_plus_ug_va, gps_pullud_normal_edge_plus_ug_va, gps_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {gps_func_lowpower_edge_plus_ug_va, gps_pullud_lowpower_edge_plus_ug_va, gps_drv_lowpower_edge_plus_ug_va},

};

/*tcxo pins */
static struct  iomux_pin *tcxo_pins_edge_plus_ug_va[] = {
    &TCXO0_AFC_EDGE_PLUS_UG_VA, &TCXO1_AFC_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: tcxo*/
IOMUX_BLOCK(block_tcxo_edge_plus_ug_va, "block_tcxo", NORMAL, tcxo_pins_edge_plus_ug_va)



/*TCXO*/

static enum lowlayer_func tcxo_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown tcxo_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL,  -INVALID,};
static enum drive_strength tcxo_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func tcxo_func_lowpower_edge_plus_ug_va[] = {FUNC4, FUNC4,  -INVALID,};
static enum pull_updown tcxo_pullud_lowpower_edge_plus_ug_va[] = {PULLDOWN, PULLDOWN,  -INVALID,};
static enum drive_strength tcxo_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config tcxo_config_edge_plus_ug_va[] = {
    [NORMAL] = {tcxo_func_normal_edge_plus_ug_va, tcxo_pullud_normal_edge_plus_ug_va, tcxo_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {tcxo_func_lowpower_edge_plus_ug_va, tcxo_pullud_lowpower_edge_plus_ug_va, tcxo_drv_lowpower_edge_plus_ug_va},

};

/*rfant pins */
static struct  iomux_pin *rfant_pins_edge_plus_ug_va[] = {
    &GPIO_7_5_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: rfant*/
IOMUX_BLOCK(block_rfant_edge_plus_ug_va, "block_rfant", NORMAL, rfant_pins_edge_plus_ug_va)



/*RFANT*/

static enum lowlayer_func rfant_func_normal_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown rfant_pullud_normal_edge_plus_ug_va[] = {PULLDOWN,  -INVALID,};
static enum drive_strength rfant_drv_normal_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static enum lowlayer_func rfant_func_lowpower_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown rfant_pullud_lowpower_edge_plus_ug_va[] = {PULLDOWN,  -INVALID,};
static enum drive_strength rfant_drv_lowpower_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static struct block_config rfant_config_edge_plus_ug_va[] = {
    [NORMAL] = {rfant_func_normal_edge_plus_ug_va, rfant_pullud_normal_edge_plus_ug_va, rfant_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {rfant_func_lowpower_edge_plus_ug_va, rfant_pullud_lowpower_edge_plus_ug_va, rfant_drv_lowpower_edge_plus_ug_va},

};

/*wifi pins */
static struct  iomux_pin *wifi_pins_edge_plus_ug_va[] = {
    &GPIO_2_2_EDGE_PLUS_UG_VA, &SDIO_DATA3_EDGE_PLUS_UG_VA, &SDIO_DATA2_EDGE_PLUS_UG_VA, &SDIO_DATA1_EDGE_PLUS_UG_VA, &SDIO_DATA0_EDGE_PLUS_UG_VA, &SDIO_CMD_EDGE_PLUS_UG_VA, &SDIO_CLK_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: wifi*/
IOMUX_BLOCK(block_wifi_edge_plus_ug_va, "block_wifi", NORMAL, wifi_pins_edge_plus_ug_va)



/*WIFI*/

static enum lowlayer_func wifi_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown wifi_pullud_normal_edge_plus_ug_va[] = {NOPULL, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, NOPULL,  -INVALID,};
static enum drive_strength wifi_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func wifi_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC2, FUNC2, FUNC2, FUNC2, FUNC2, FUNC2,  -INVALID,};
static enum pull_updown wifi_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, PULLUP, PULLUP, PULLUP, PULLUP, PULLUP, NOPULL,  -INVALID,};
static enum drive_strength wifi_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config wifi_config_edge_plus_ug_va[] = {
    [NORMAL] = {wifi_func_normal_edge_plus_ug_va, wifi_pullud_normal_edge_plus_ug_va, wifi_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {wifi_func_lowpower_edge_plus_ug_va, wifi_pullud_lowpower_edge_plus_ug_va, wifi_drv_lowpower_edge_plus_ug_va},

};

/*boot pins */
static struct  iomux_pin *boot_pins_edge_plus_ug_va[] = {
    &BOOT_SEL_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: boot*/
IOMUX_BLOCK(block_boot_edge_plus_ug_va, "block_boot", NORMAL, boot_pins_edge_plus_ug_va)



/*BOOT*/

static enum lowlayer_func boot_func_normal_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown boot_pullud_normal_edge_plus_ug_va[] = {PULLUP,  -INVALID,};
static enum drive_strength boot_drv_normal_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static enum lowlayer_func boot_func_lowpower_edge_plus_ug_va[] = {FUNC0,  -INVALID,};
static enum pull_updown boot_pullud_lowpower_edge_plus_ug_va[] = {PULLUP,  -INVALID,};
static enum drive_strength boot_drv_lowpower_edge_plus_ug_va[] = {LEVEL0,  -INVALID,};

static struct block_config boot_config_edge_plus_ug_va[] = {
    [NORMAL] = {boot_func_normal_edge_plus_ug_va, boot_pullud_normal_edge_plus_ug_va, boot_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {boot_func_lowpower_edge_plus_ug_va, boot_pullud_lowpower_edge_plus_ug_va, boot_drv_lowpower_edge_plus_ug_va},

};

/*lcd pins */
static struct  iomux_pin *lcd_pins_edge_plus_ug_va[] = {
    &PWM_OUT1_EDGE_PLUS_UG_VA, &GPIO_4_6_EDGE_PLUS_UG_VA, &GPIO_4_7_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: lcd*/
IOMUX_BLOCK(block_lcd_edge_plus_ug_va, "block_lcd", NORMAL, lcd_pins_edge_plus_ug_va)



/*LCD*/

static enum lowlayer_func lcd_func_normal_edge_plus_ug_va[] = {FUNC2, FUNC0, FUNC4,  -INVALID,};
static enum pull_updown lcd_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength lcd_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func lcd_func_lowpower_edge_plus_ug_va[] = {FUNC2, FUNC0, FUNC4,  -INVALID,};
static enum pull_updown lcd_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength lcd_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config lcd_config_edge_plus_ug_va[] = {
    [NORMAL] = {lcd_func_normal_edge_plus_ug_va, lcd_pullud_normal_edge_plus_ug_va, lcd_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {lcd_func_lowpower_edge_plus_ug_va, lcd_pullud_lowpower_edge_plus_ug_va, lcd_drv_lowpower_edge_plus_ug_va},

};

/*key pins */
static struct  iomux_pin *key_pins_edge_plus_ug_va[] = {
    &GPIO_4_2_EDGE_PLUS_UG_VA, &GPIO_4_3_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: key*/
IOMUX_BLOCK(block_key_edge_plus_ug_va, "block_key", NORMAL, key_pins_edge_plus_ug_va)



/*KEY*/

static enum lowlayer_func key_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown key_pullud_normal_edge_plus_ug_va[] = {PULLUP, PULLUP,  -INVALID,};
static enum drive_strength key_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func key_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown key_pullud_lowpower_edge_plus_ug_va[] = {PULLUP, PULLUP,  -INVALID,};
static enum drive_strength key_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config key_config_edge_plus_ug_va[] = {
    [NORMAL] = {key_func_normal_edge_plus_ug_va, key_pullud_normal_edge_plus_ug_va, key_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {key_func_lowpower_edge_plus_ug_va, key_pullud_lowpower_edge_plus_ug_va, key_drv_lowpower_edge_plus_ug_va},

};

/*test pins */
static struct  iomux_pin *test_pins_edge_plus_ug_va[] = {
    &UART0_RXD_EDGE_PLUS_UG_VA, &UART0_TXD_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: test*/
IOMUX_BLOCK(block_test_edge_plus_ug_va, "block_test", NORMAL, test_pins_edge_plus_ug_va)



/*TEST*/

static enum lowlayer_func test_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown test_pullud_normal_edge_plus_ug_va[] = {PULLUP, NOPULL,  -INVALID,};
static enum drive_strength test_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func test_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0,  -INVALID,};
static enum pull_updown test_pullud_lowpower_edge_plus_ug_va[] = {PULLUP, NOPULL,  -INVALID,};
static enum drive_strength test_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0,  -INVALID,};

static struct block_config test_config_edge_plus_ug_va[] = {
    [NORMAL] = {test_func_normal_edge_plus_ug_va, test_pullud_normal_edge_plus_ug_va, test_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {test_func_lowpower_edge_plus_ug_va, test_pullud_lowpower_edge_plus_ug_va, test_drv_lowpower_edge_plus_ug_va},

};

/*es305 pins */
static struct  iomux_pin *es305_pins_edge_plus_ug_va[] = {
    &I2S_XCLK_EDGE_PLUS_UG_VA, &I2S_XFS_EDGE_PLUS_UG_VA, &I2S_DI_EDGE_PLUS_UG_VA, &I2S_DO_EDGE_PLUS_UG_VA, &PCM0_XCLK_EDGE_PLUS_UG_VA, &PCM0_XFS_EDGE_PLUS_UG_VA, &PCM0_DI_EDGE_PLUS_UG_VA, &PCM0_DO_EDGE_PLUS_UG_VA, &UART1_TXD_EDGE_PLUS_UG_VA, &UART2_RXD_EDGE_PLUS_UG_VA, &UART2_TXD_EDGE_PLUS_UG_VA,  NULL,
};

/*define blocks: es305*/
IOMUX_BLOCK(block_es305_edge_plus_ug_va, "block_es305", NORMAL, es305_pins_edge_plus_ug_va)



/*ES305*/

static enum lowlayer_func es305_func_normal_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC2, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown es305_pullud_normal_edge_plus_ug_va[] = {NOPULL, NOPULL, PULLDOWN, NOPULL, NOPULL, NOPULL, PULLDOWN, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength es305_drv_normal_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static enum lowlayer_func es305_func_lowpower_edge_plus_ug_va[] = {FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC0, FUNC2, FUNC0, FUNC0,  -INVALID,};
static enum pull_updown es305_pullud_lowpower_edge_plus_ug_va[] = {NOPULL, NOPULL, PULLDOWN, NOPULL, NOPULL, NOPULL, PULLDOWN, NOPULL, NOPULL, NOPULL, NOPULL,  -INVALID,};
static enum drive_strength es305_drv_lowpower_edge_plus_ug_va[] = {LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0, LEVEL0,  -INVALID,};

static struct block_config es305_config_edge_plus_ug_va[] = {
    [NORMAL] = {es305_func_normal_edge_plus_ug_va, es305_pullud_normal_edge_plus_ug_va, es305_drv_normal_edge_plus_ug_va},
    [LOWPOWER] = {es305_func_lowpower_edge_plus_ug_va, es305_pullud_lowpower_edge_plus_ug_va, es305_drv_lowpower_edge_plus_ug_va},

};

struct iomux_block_table block_config_table_edge_plus_ug_va[] = {
    BLOCK_CONFIG("block_charger", &block_charger_edge_plus_ug_va, charger_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_flash", &block_flash_edge_plus_ug_va, flash_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_bt", &block_bt_edge_plus_ug_va, bt_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_scamera", &block_scamera_edge_plus_ug_va, scamera_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_codec", &block_codec_edge_plus_ug_va, codec_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_gpa1", &block_gpa1_edge_plus_ug_va, gpa1_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_pmu", &block_pmu_edge_plus_ug_va, pmu_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_emmc", &block_emmc_edge_plus_ug_va, emmc_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_rf", &block_rf_edge_plus_ug_va, rf_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_tp", &block_tp_edge_plus_ug_va, tp_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_switch", &block_switch_edge_plus_ug_va, switch_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_rfpmu", &block_rfpmu_edge_plus_ug_va, rfpmu_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_mmmpa", &block_mmmpa_edge_plus_ug_va, mmmpa_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_i2c3", &block_i2c3_edge_plus_ug_va, i2c3_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_usim1", &block_usim1_edge_plus_ug_va, usim1_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_usim0", &block_usim0_edge_plus_ug_va, usim0_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_mcamera", &block_mcamera_edge_plus_ug_va, mcamera_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_i2c0", &block_i2c0_edge_plus_ug_va, i2c0_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_i2c1", &block_i2c1_edge_plus_ug_va, i2c1_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_i2c2", &block_i2c2_edge_plus_ug_va, i2c2_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_gps", &block_gps_edge_plus_ug_va, gps_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_tcxo", &block_tcxo_edge_plus_ug_va, tcxo_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_rfant", &block_rfant_edge_plus_ug_va, rfant_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_wifi", &block_wifi_edge_plus_ug_va, wifi_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_boot", &block_boot_edge_plus_ug_va, boot_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_lcd", &block_lcd_edge_plus_ug_va, lcd_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_key", &block_key_edge_plus_ug_va, key_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_test", &block_test_edge_plus_ug_va, test_config_edge_plus_ug_va)
    BLOCK_CONFIG("block_es305", &block_es305_edge_plus_ug_va, es305_config_edge_plus_ug_va)

    {NULL, NULL, NULL}
};


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif