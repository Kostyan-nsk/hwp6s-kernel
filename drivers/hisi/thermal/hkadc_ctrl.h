
#include "drv_temp_cfg.h"
#include "bbp_comm_on_interface.h"
#include "soc_baseaddr_interface.h"
#include "soc_smart_interface.h"

#include <mach/hardware.h>
#include "linux/io.h"

#define HKADC_DEBUG(p) printk p

/*ACPU实地址向虚拟地址映射的转换接口*/
#define THERMAL_CORE_SHARE_PHY2VIRT(phy)  \
        (((unsigned int)phy) - MCU_SHARE_MEM_ADDR + MCU_SHARE_MEM_VIRT_ADDR)

/*通道属性和对应温度电压转换表*/
struct hkadc_table_data
{
    unsigned int logic_chan;
    unsigned int phy_chan;
    unsigned int ctrlflag;
    struct hkadc_nv_config hkadc_conf;
    unsigned int table_len;
    struct hkadc_tem_value temp_table[32];
};

/*==============================HKADC BBP control start==========================*/
/*Register define*/
#define HKADC_REGBASE_ADDR         (BBPCOMM_DSP_START_ADDR)
#define HKADC_START_OFFSET          (0x00)
#define HKADC_RD_DATA_OFFSET     (0x14)
#define HKADC_START_CLR_OFFSET          (0x1C)
#define HKADC_WR01_CFG_OFFSET   (0x20)
#define HKADC_WR34_CFG_OFFSET   (0x24)
#define HKADC_DELAY_OFFSET          (0x28)

#define PMU_ADDR_OFFSET0          (0x00)
#define PMU_ADDR_OFFSET1          (0x01)
#define PMU_ADDR_OFFSET2          (0x02)
#define PMU_ADDR_OFFSET3          (0x03)
#define PMU_ADDR_OFFSET4          (0x04)

#define WR01_VAL_START    ((0x0 << 31)  | (PMU_ADDR_OFFSET1 << 24) | (0x1 << 16)  | (0x0 << 15) | (PMU_ADDR_OFFSET0 << 8))
#define WR34_VAL_START    ((0x1 << 31)  | (PMU_ADDR_OFFSET4 << 24) | (0x1 << 15) | (PMU_ADDR_OFFSET3 << 8))

#define DELAY_VAL_START   ((0x05dc << 16) | (0x01c2 << 0))

#define WR01_VAL_END        (0x01000080)/*((PMU_ADDR_OFFSET0 << 24) | (0x80 << 16) | (PMU_ADDR_OFFSET0 << 8) | (0x80 << 0))*/
#define WR34_VAL_END        (0x82008100)/*((PMU_ADDR_OFFSET0 << 24) | (0x80 << 16) | (PMU_ADDR_OFFSET0 << 8) | (0x80 << 0))*/
#define DELAY_VAL_END       (0x012c012c)/*(0x00000000)*/
#define HKADC_VALID_VALUE       (0x3FF) /*|?§???ìa10????ì?DD??§?*/

#define START_DELAY_TIMEOUT     (20000)

/*BBP2??ì????§?é?????ì????ì2*/
//#define HKADC_REG(addr) (*(volatile unsigned int *)addr)

#define HKADC_BSP_REG_READ(addr, result) \
    ((result) = readl(addr))

#define HKADC_BSP_REG_WRITE(addr, data) \
    (writel(data, addr))


#define HKADC_REG_WRITE(reg,data) \
        HKADC_BSP_REG_WRITE(reg, data)
#define HKADC_REG_READ(reg,result) \
        HKADC_BSP_REG_READ(reg, result)

/*HKADC REF voltage*/
#define HKADC_VREF_25V      2500


/*hkadc???ì?¨¨1?o??ê?è??ìoy??¨¨|?§????|?§?*/
#define HKADC_OK  (0)
#define HKADC_ERROR (-1)


/*==============================HKADC BBP control end==========================*/

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

extern int thermal_data_init(void);
extern int hkadc_debug_init(int chanid);
extern int hkadc_debug_uninit(int chanid);
extern int hkadc_get_shared_value(int chanid, unsigned short* volt);

int hkadc_quick_access_init(void);
int hkadc_quick_access_uninit(void);
extern int hkadc_quick_bbp_convert(unsigned char enchan, unsigned short *pusvalue);