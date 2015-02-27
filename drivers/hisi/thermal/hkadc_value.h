
#include "drv_temp_cfg.h"

#if defined(BSP_CORE_MODEM) /*C核的系统适配*/
#include "stdio.h"
#define HKADC_DEBUG(p) printf p

/*CCPU实地址向虚拟地址映射的转换接口*/
#define THERMAL_CORE_SHARE_PHY2VIRT(phy)                (phy)

#elif defined (BSP_CORE_APP) /*A核的系统适配*/
#include <mach/hardware.h>

#define HKADC_DEBUG(p) printk p

/*ACPU实地址向虚拟地址映射的转换接口*/
#define THERMAL_CORE_SHARE_PHY2VIRT(phy)  \
        (((unsigned int)phy) - MCU_SHARE_MEM_ADDR + MCU_SHARE_MEM_VIRT_ADDR)

#elif defined (BSP_CORE_CM3)  /*M核的系统适配*/
#include "mcu_print.h"

#define HKADC_DEBUG(p) mcu_add_trace p

#define THERMAL_CORE_SHARE_PHY2VIRT(phy)         (phy - MCU_SYS_MEM_ADDR + M3_DDR_MEM_REMAP_START_ADDRESS)
#endif

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
extern struct thermal_data_area *g_thermal_data_area;

extern struct drv_hkadc_map g_hkadc_chan_map[];

extern int DRV_HKADC_GET_TEMP(HKADC_TEMP_PROTECT_E log_chan,
                unsigned short* volt,
                short* temp, HKADC_CONV_MODE_E ulMode);

extern struct drv_hkadc_map *bsp_hkadc_get_chan_map(int logic_chan);

extern int hkadc_fill_chan_map(unsigned short *p2l_tbl);
extern int hkadc_chan_map_renew(unsigned short *l2p_tbl);

