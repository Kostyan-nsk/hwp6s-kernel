/*
 * drivers/mmc/host/k3v2_mmc_raw.c
 *
 * Copyright (C) 2011 Hisilicon, Inc.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/clk.h>
#include <linux/mmc/host.h>
#include <linux/mmc/core.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sd.h>
#include <linux/io.h>
#include <linux/semaphore.h>
#include <mach/platform.h>

#include "k3v2_mmc_raw.h"
#include <mach/boardid.h>

/*
 * emmc reg address offset
 */
#define EMMC_REG_OFF_CTRL               (0x0)
#define EMMC_REG_OFF_PWREN              (0x4)
#define EMMC_REG_OFF_CLKDIV             (0x8)
#define EMMC_REG_OFF_CLKSRC             (0xC)
#define EMMC_REG_OFF_CLKENA             (0x10)
#define EMMC_REG_OFF_TMOUT              (0x14)
#define EMMC_REG_OFF_CTYPE              (0x18)
#define EMMC_REG_OFF_BLKSIZ             (0x1C)
#define EMMC_REG_OFF_BYTCNT             (0x20)
#define EMMC_REG_OFF_INTMSK             (0x24)
#define EMMC_REG_OFF_CMDARG             (0x28)
#define EMMC_REG_OFF_CMD                (0x2C)
#define EMMC_REG_OFF_RESP0              (0x30)
#define EMMC_REG_OFF_RESP1              (0x34)
#define EMMC_REG_OFF_RESP2              (0x38)
#define EMMC_REG_OFF_RESP3              (0x3C)
#define EMMC_REG_OFF_MINTSTS            (0x40)
#define EMMC_REG_OFF_RINTSTS            (0x44)
#define EMMC_REG_OFF_STATUS             (0x48)
#define EMMC_REG_OFF_FIFOTH             (0x4C)
#define EMMC_REG_OFF_CDETECT            (0x50)
#define EMMC_REG_OFF_WRTPRT             (0x54)
#define EMMC_REG_OFF_GPIO               (0x58)
#define EMMC_REG_OFF_TCBCNT             (0x5C)
#define EMMC_REG_OFF_TBBCNT             (0x60)
#define EMMC_REG_OFF_DEBNCE             (0x64)
#define EMMC_REG_OFF_USRID              (0x68)
#define EMMC_REG_OFF_VERID              (0x6C)
#define EMMC_REG_OFF_HCON               (0x70)
#define EMMC_REG_OFF_UHS                (0x74)
#define EMMC_REG_OFF_BMOD               (0x80)
#define EMMC_REG_OFF_DBADDR             (0x88)
#define EMMC_REG_OFF_IDSTS              (0x8c)
#define EMMC_REG_OFF_IDINTEN            (0x90)
#define EMMC_REG_OFF_FIFO_START         (0x200)

#define BURST_SIZE                      (3)
#define BIT_FIFOTH_DMA_MSIZE            (BURST_SIZE<<28)

#define BIT_IDSTS_TI                    (1<<0)
#define BIT_IDSTS_RI                    (1<<1)

#define BIT_IDINTEN_AI                  (1<<9)
#define BIT_IDINTEN_CES                 (1<<5)
#define BIT_IDINTEN_DU                  (1<<4)
#define BIT_IDINTEN_FBE                 (1<<2)

#define BIT_BMOD_IDMAC_ENABLE           (1<<7)
#define BIT_BMOD_FIXED_BURST            (1<<1)
#define BIT_BMOD_SOFTWARE_RESET         (1<<0)

/* control */
#define BIT_FIFO_RESET                  (0x1<<1)
#define BIT_DMA_RESET                   (0x1<<2)
#define BIT_INT_ENABLE                  (0x1<<4)
#define BIT_DMA_ENABLE                  (0x1<<5)
#define BIT_INTERNAL_DMA                (0x1<<25)

/* cmd bit */
#define BIT_CMD_RESPONSE_EXPECT         (0x1<<6)
#define BIT_CMD_LONG_RESPONSE           (0x1<<7)
#define BIT_CMD_CHECK_RESPONSE_CRC      (0x1<<8)
#define BIT_CMD_DATA_EXPECTED           (0x1<<9)
#define BIT_CMD_READ                    (0x0<<10)
#define BIT_CMD_WRITE                   (0x1<<10)
#define BIT_CMD_BLOCK_TRANSFER          (0x0<<11)
#define BIT_CMD_STREAM_TRANSFER         (0x1<<11)
#define BIT_CMD_SEND_AUTO_STOP          (0x1<<12)
#define BIT_CMD_WAIT_PRVDATA_COMPLETE   (0x1<<13)
#define BIT_CMD_STOP_ABORT_CMD          (0x1<<14)
#define BIT_CMD_SEND_INIT               (0x1<<15)
#define BIT_CMD_UPDATE_CLOCK_ONLY       (0x1<<21)
#define BIT_CMD_READ_CEATA_DEVICE       (0x1<<22)
#define BIT_CMD_CCS_EXPECTED            (0x1<<23)
#define BIT_CMD_ENABLE_BOOT             (0x1<<24)
#define BIT_CMD_EXPECT_BOOT_ACK         (0x1<<25)
#define BIT_CMD_DISABLE_BOOT            (0x1<<26)
#define BIT_CMD_MANDATORY_BOOT          (0x0<<27)
#define BIT_CMD_ALTERNATE_BOOT          (0x1<<27)
#define BIT_CMD_VOLT_SWITCH             (0x1<<28)
#define BIT_CMD_START_CMD               (0x1<<31)

/* interrupt */
#define BIT_INT_BAR                     (0x1<<8)
#define BIT_INT_BDS                     (0x1<<9)
#define BIT_INT_HLE                     (0x1<<12)
#define BIT_INT_ACD                     (0x1<<14)
#define BIT_INT_RXDR                    (0x1<<5)
#define BIT_INT_TXDR                    (0x1<<4)
#define BIT_INT_DTO                     (0x1<<3)
#define INT_DATA_FIFO_ERROR             (0xA280)
#define INT_BOOT_DATA_ERROR             (0xA200)

/* emmc status */
#define BIT_INT_DATA_BUSY               (0x1<<9)

/* emmc control */
#define EMMC_RESET_ALL                  (0x7)

/* clock update cmd */
#define CMD_UPDATE_CLK                  0xA0202000
#define CMD_START_BIT                   (0x1<<31)

/* time count */
#define MAX_COUNT_POLL_CMD              (100)
#define MAX_DELAY_POLL_CMD              (0xFF)

/* cmd defines */
#define CMD0                            0
#define CMD1                            1
#define CMD2                            2
#define CMD3                            3
#define CMD4                            4
#define CMD5                            5
#define CMD6                            6
#define CMD7                            7
#define CMD8                            8
#define CMD9                            9
#define CMD10                           10
#define CMD11                           11
#define CMD12                           12
#define CMD13                           13
#define CMD14                           14
#define CMD15                           15
#define CMD16                           16
#define CMD17                           17
#define CMD18                           18
#define CMD19                           19
#define CMD20                           20
#define CMD21                           21
#define CMD22                           22
#define CMD23                           23
#define CMD24                           24
#define CMD25                           25
#define CMD26                           26
#define CMD27                           27
#define CMD28                           28
#define CMD29                           29
#define CMD30                           30
#define CMD31                           31
#define CMD32                           32
#define CMD33                           33
#define CMD34                           34
#define CMD35                           35
#define CMD36                           36
#define CMD37                           37
#define CMD38                           38
#define CMD39                           39
#define CMD41                           41
#define CMD55                           55

#define EXTCSD_PARTITION_CONFIG         179
#define EXTCSD_BUS_WIDTH                183
#define EXTCSD_HS_TIMING                185

/* ext_csd partition config */
#define BOOT_ENABLE_BOOTPARTITION1      (1<<3)
#define RW_PARTITION_DEFAULT            0
#define RW_BOOTPARTITION1               1
#define BOOT_PARTITION                  BOOT_ENABLE_BOOTPARTITION1

#define EMMC_FIX_RCA                    6
#define EMMC_BLOCK_SIZE                 512

/*
 * ext_csd fields
 */
#define EXT_CSD_REV						192	/* RO */
#define EXT_CSD_CSD_STRUCTURE			194	/* RO */
#define EXT_CSD_SEC_CNT					212	/* RO, 4 bytes */

#define CURRENT_STATE_MASK				0x1E00
#define CURRENT_STATE_TRAN				0x800

#define ERASE_WR_BLK_START				32
#define ERASE_WR_BLK_END				33

#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define DPRINTK(fmt, args...) \
	printk(fmt, ## args)
#else
#define DPRINTK(fmt, args...)
#endif

/*
 * io address
 */
#define IO_ADDRESS_EMMC					IO_ADDRESS(REG_BASE_MMC1)
										/* FIXME */
#define IO_ADDRESS_PMU					IO_ADDRESS(REG_BASE_PMUSPI)
#define IO_ADDRESS_IOC					IO_ADDRESS(REG_BASE_IOC)
#define IO_ADDRESS_SC					IO_ADDRESS(REG_BASE_SCTRL)
#define IO_ADDRESS_PC					IO_ADDRESS(REG_BASE_PCTRL)
/*
 * pmu related
 */
#define BIT_6421_LDO0_EN             	(0x1<<4)
#define BIT_6421_LDO0_V285             	(0x6)
#define BIT_6421_LDO0_DIS             	(0x0<<4)
#define BIT_6421_LDO0_ECO              	(0x1<<5)
#define REG_OFF_LDO_ADDR				(0x20*4)

#define BIT_PCTRL22_MMC1_CLKEN			(0x1<<12)
#define PMU_RST_CTRL                    (0x035<<2)

struct raw_k3v2_mmc_host {
	unsigned int		pmuioaddr;
	unsigned int		mmcioaddr;
	unsigned int		iocioaddr;
	unsigned int		scioaddr;
	unsigned int		pcioaddr;
	unsigned int		clock_base;
	unsigned int		f_min;
	unsigned int		f_max;
	unsigned int        bus_width;
	int 				type;
};

/* global mmc host */
static struct raw_k3v2_mmc_host emmc_host;
static struct raw_k3v2_mmc_host *kpanic_host;


static void k3v2_mmc_dumpregs(struct raw_k3v2_mmc_host *host)
{
	printk(KERN_ERR ": ============== REGISTER DUMP ==============\n");
	printk(KERN_ERR ": MSHCI_CTRL:	  0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CTRL));
	printk(KERN_ERR ": MSHCI_PWREN:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_PWREN));
	printk(KERN_ERR ": MSHCI_CLKDIV:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CLKDIV));
	printk(KERN_ERR ": MSHCI_CLKSRC:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CLKSRC));
	printk(KERN_ERR ": MSHCI_CLKENA:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CLKENA));
	printk(KERN_ERR ": MSHCI_TMOUT:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_TMOUT));
	printk(KERN_ERR ": MSHCI_CTYPE:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CTYPE));
	printk(KERN_ERR ": MSHCI_BLKSIZ:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_BLKSIZ));
	printk(KERN_ERR ": MSHCI_BYTCNT:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_BYTCNT));
	printk(KERN_ERR ": MSHCI_INTMSK:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_INTMSK));
	printk(KERN_ERR ": MSHCI_CMDARG:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CMDARG));
	printk(KERN_ERR ": MSHCI_CMD:	   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CMD));
	printk(KERN_ERR ": MSHCI_MINTSTS:   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_MINTSTS));
	printk(KERN_ERR ": MSHCI_RINTSTS:   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS));
	printk(KERN_ERR ": MSHCI_STATUS:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_STATUS));
	printk(KERN_ERR ": MSHCI_FIFOTH:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_FIFOTH));
	printk(KERN_ERR ": MSHCI_CDETECT:   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_CDETECT));
	printk(KERN_ERR ": MSHCI_WRTPRT:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_WRTPRT));
	printk(KERN_ERR ": MSHCI_GPIO:	  0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_GPIO));
	printk(KERN_ERR ": MSHCI_TCBCNT:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_TCBCNT));
	printk(KERN_ERR ": MSHCI_TBBCNT:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_TBBCNT));
	printk(KERN_ERR ": MSHCI_DEBNCE:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_DEBNCE));
	printk(KERN_ERR ": MSHCI_USRID:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_USRID));
	printk(KERN_ERR ": MSHCI_VERID:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_VERID));
	printk(KERN_ERR ": MSHCI_HCON:	  0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_HCON));
	printk(KERN_ERR ": MSHCI_UHS_REG:   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_UHS));
	printk(KERN_ERR ": MSHCI_BMOD:	  0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_BMOD));
	printk(KERN_ERR ": MSHCI_DBADDR:	0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_DBADDR));
	printk(KERN_ERR ": MSHCI_IDSTS:	 0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_IDSTS));
	printk(KERN_ERR ": MSHCI_IDINTEN:   0x%08x\n",
		readl(host->mmcioaddr + EMMC_REG_OFF_IDINTEN));
	/*
	printk(KERN_ERR DRIVER_NAME ": MSHCI_FIFODAT:   0x%08x\n",
		mshci_readl(host, MSHCI_FIFODAT));
	*/
	printk(KERN_ERR ": ===========================================\n");
}

static int k3v2_set_pmu_rst2n(struct raw_k3v2_mmc_host *host, unsigned int enable)
{
	if (enable) {
		writel(1, host->pmuioaddr + PMU_RST_CTRL);
	} else {
		writel(0, host->pmuioaddr + PMU_RST_CTRL);
	}
}

/*
 * config ldo0 power
 * 0 for power off and 1 for power on
 */
static int k3v2_sys_set_power(struct raw_k3v2_mmc_host *host, unsigned int enable)
{
	if (enable)	{
		writel(BIT_6421_LDO0_EN | BIT_6421_LDO0_V285, host->pmuioaddr + REG_OFF_LDO_ADDR);
	} else {
		writel(BIT_6421_LDO0_DIS | BIT_6421_LDO0_ECO, host->pmuioaddr + REG_OFF_LDO_ADDR);
	}

	return 0;
}

/*
 * config iomux
 * 0 for gpio and 1 for mmc io
 */
static int k3v2_sys_set_io(struct raw_k3v2_mmc_host *host, unsigned int mode)
{
	if (mode) {

		/* iomg: func2 is emmc mode */
		writel(2, host->iocioaddr + 0x02C);
		writel(2, host->iocioaddr + 0x030);

		/* iocg: 6mA and pull up*/
		writel(2<<0, host->iocioaddr + 0x88C);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x88C);
		writel(2<<0, host->iocioaddr + 0x890);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x890);
		writel(2<<0, host->iocioaddr + 0x894);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x894);
		writel(2<<0, host->iocioaddr + 0x898);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x898);
		writel(2<<0, host->iocioaddr + 0x89c);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x89c);
		writel(2<<0, host->iocioaddr + 0x8A0);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x8A0);
		writel(2<<0, host->iocioaddr + 0x8A4);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x8A4);
		writel(2<<0, host->iocioaddr + 0x8A8);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x8A8);

		writel(2<<0, host->iocioaddr + 0x8AC);
		writel((3<<4) | (1<<0), host->iocioaddr + 0x8AC);
		writel(2<<0, host->iocioaddr + 0x8B0);
		writel((3<<4) | (0<<0), host->iocioaddr + 0x8B0);	/* clk without pullup */

	} else {

		/* iomg: func2 is gpio mode */
		writel(1, host->iocioaddr + 0x02C);
		writel(1, host->iocioaddr + 0x030);

		/* set down out put signal */

		writel((3<<4) | (2<<0), host->iocioaddr + 0x88C);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x890);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x894);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x898);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x89c);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x8A0);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x8A4);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x8A8);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x8AC);
		writel((3<<4) | (2<<0), host->iocioaddr + 0x8B0);	/* clk without pullup */
	}

	return 0;
}

/*
 * config clock
 * 0 for disable and 1 for enable
 */
static int k3v2_sys_set_clock(struct raw_k3v2_mmc_host *host, unsigned int enable)
{
	unsigned int reg_val;
	unsigned int dwLoopCount = 0;

	if (enable)	{

		/* [3:  0]: 11 - 12 division */
		/* [4]  : 0 - periPLL */
		/* [5]  : 1 - PLL clock sourc */
		/* [8:6]: 0 - 1 division */
		/* [9]  : 1 - PLL clock sourc */
		/* [25  :16]: 0x3FF - mask */
//#define EMMC_SD_CLOCK_VAL	((11 << 0) | (0 << 4) | (1 << 5) | (0 << 6) | (1 << 9) | (0x3FF << 16))
#define SDSEL_VALUE (((1<<4)<<16) | (1<<4))
#define SDDIV_VALUE ((0x1E0F<<16) | (1<<12) | (1<<0) | (1<<9))

		if (DI_CHIP_ID == get_chipid()) {
			/* set sd clock to 100M and emmc clock 100M */
			reg_val = SDSEL_VALUE;
			writel(reg_val, host->scioaddr + 0x108);
			reg_val = SDDIV_VALUE;
			writel(reg_val, host->scioaddr + 0x108);
			host->clock_base = 80*1000*1000;
		} else {

			/* disable MMC1 clock */
			writel(0x1<<21, host->scioaddr + 0x54);

			/* choose MMC1_clk 26MHz */
			writel((0x0<<10) | (0x1<<26), host->scioaddr + 0x108);

			/* enable MMC1 clock */
			writel(0x1<<21, host->scioaddr + 0x50);

			// disable MMC1 clk_en
			reg_val = readl(host->pcioaddr + 0x1F8);
			reg_val = reg_val & (~BIT_PCTRL22_MMC1_CLKEN);
			writel( reg_val, host->pcioaddr + 0x1F8 );

			// set MMC1 2��Ƶ 180��delay
			reg_val = readl(host->pcioaddr + 0x1F8 );
			reg_val = (reg_val & 0xFF001FFF) | (0x1<<13) | (0x1<<16) | (0x3<<20);
			writel(reg_val, host->pcioaddr + 0x1F8);

			// enable MMC1 clk_en
			reg_val = readl(host->pcioaddr + 0x1F8);
			reg_val = reg_val | BIT_PCTRL22_MMC1_CLKEN;
			writel(reg_val, host->pcioaddr + 0x1F8);

			host->clock_base = 13*1000*1000;
		}


		/* reset */
		writel(0x1<<24, host->pcioaddr + 0x0A4);
		dwLoopCount = 0;
		while (dwLoopCount++ < 10) {
		}

		/* unreset */
		writel(0x1<<24, host->pcioaddr + 0x0A8);
		dwLoopCount = 0;
		while (dwLoopCount++ < 10) {
		}

	} else {
		/* FIXME */
	}

	return 0;
}

/*
 * send mmc command
 */
static int k3v2_mmc_send_cmd(struct raw_k3v2_mmc_host *host,
		unsigned char cmd_index, unsigned int cmd_arg, unsigned int *resp_buffer)
{
	unsigned int reg_val;
	unsigned int loop;

	DPRINTK(KERN_ERR "KPANIC_MMC: cmd index:%d, cmd arg:0x%x\n", cmd_index, cmd_arg);

    /* set up args, no check perform */
	writel(cmd_arg, host->mmcioaddr + EMMC_REG_OFF_CMDARG);

	/* clear interrupt */
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);

	switch (cmd_index) {
	case CMD0:
		reg_val = BIT_CMD_SEND_INIT;
		break;
	case CMD1:
		reg_val = BIT_CMD_RESPONSE_EXPECT;
		break;
	case CMD2:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_LONG_RESPONSE |
				BIT_CMD_CHECK_RESPONSE_CRC | BIT_CMD_SEND_INIT;
		break;
	case CMD3:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_SEND_INIT;
		break;
	case CMD8:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_DATA_EXPECTED | BIT_CMD_READ | BIT_CMD_WAIT_PRVDATA_COMPLETE;
		break;
	case CMD9:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_LONG_RESPONSE;
		break;
	case CMD12:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_STOP_ABORT_CMD;
		break;
	case CMD17:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_DATA_EXPECTED | BIT_CMD_READ | BIT_CMD_WAIT_PRVDATA_COMPLETE;
		break;
	case CMD18:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_DATA_EXPECTED | BIT_CMD_READ | BIT_CMD_WAIT_PRVDATA_COMPLETE;
		break;
	case CMD24:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_DATA_EXPECTED | BIT_CMD_WRITE | BIT_CMD_WAIT_PRVDATA_COMPLETE;
		break;
	case CMD25:
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
				BIT_CMD_DATA_EXPECTED | BIT_CMD_WRITE | BIT_CMD_WAIT_PRVDATA_COMPLETE;
		break;
	default:
		/* default val 0x80000140 */
		reg_val = BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC;
		break;
	}

	/* add index */
	reg_val |= cmd_index | BIT_CMD_START_CMD | (1<<29);

	/* write to register */
	writel(reg_val, host->mmcioaddr + EMMC_REG_OFF_CMD);

	/* wait command end */
	for (loop = 0; loop < 1000000; loop++) {

		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		DPRINTK(KERN_ERR "KPANIC_MMC: raw intr:0x%x\n", reg_val);
		if (reg_val & 0x4) {
			break;
		}
	}

	if (1000 == loop) {
		printk(KERN_ERR "KPANIC_MMC: wait cd timeout\n");
		return -1;
	}

	/* check if normal result */
	/* bit1:RE;bit6:RCRC;bit8:RTO;bit12:HLE;bit15:End bit error.*/
	if (reg_val & 0x9142) {
		printk(KERN_ERR "KPANIC_MMC: send cmd error 0x%x\n", reg_val);
		return -1;
	}

	/* get resp */
	if (resp_buffer) {

		resp_buffer[0] = readl(host->mmcioaddr + EMMC_REG_OFF_RESP0);
		if (cmd_index == 2 || cmd_index == 9) {
			resp_buffer[1] = readl(host->mmcioaddr + EMMC_REG_OFF_RESP1);
			resp_buffer[2] = readl(host->mmcioaddr + EMMC_REG_OFF_RESP2);
			resp_buffer[3] = readl(host->mmcioaddr + EMMC_REG_OFF_RESP3);
		}
	}

	return 0;
}

/*
 * cofind ext csd
 */
static int k3v2_mmc_update_ext_csd(struct raw_k3v2_mmc_host *host,
			unsigned int dwAccessIndex, unsigned int dwAccessValue)
{
	unsigned int cmd_reg;
	unsigned int resp;
	unsigned int reg_val;
	unsigned int loop;
	unsigned int timeout;

	cmd_reg = (0x3 << 24) | (dwAccessIndex << 16) | (dwAccessValue << 8) | 0x1;

	k3v2_mmc_send_cmd(host, CMD6, cmd_reg, NULL);

	/* wait busy de-assert */
	for (loop = 0; loop < 100000; loop++) {
		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_STATUS);
		if (!(reg_val & BIT_INT_DATA_BUSY)) {
			break;
		}
	}

	timeout = 1000;/* wait 1S */

	do {
		k3v2_mmc_send_cmd(host, CMD13, EMMC_FIX_RCA << 16, &resp);

		if (resp & (1<<7)) {
			printk(KERN_ERR "KPANIC_MMC: maybe switch eMMC mode error\n");
			return -1;
		} else {
			if (timeout) {
				timeout--;
				udelay(1);
			} else {
				printk(KERN_ERR "KPANIC_MMC: warring -- switch eMMC mode timeout\n");
				break;
			}
		}
	} while ((resp&0x00001E00)>>9 == 7);
	return 0;
}

/*
 * update mmc clock
 */
static int k3v2_mmc_update_clock(struct raw_k3v2_mmc_host *host)
{
	unsigned int retry_count;
	unsigned int reg_val;
	volatile unsigned int delay;

	writel(CMD_UPDATE_CLK, host->mmcioaddr + EMMC_REG_OFF_CMD);

	/* poll result */
	for (retry_count = 0; retry_count < MAX_COUNT_POLL_CMD; retry_count++) {

		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_CMD);

		if (0 == (reg_val & CMD_START_BIT)) {
			return 0;
		}

		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS);

		if (0 != (reg_val & BIT_INT_HLE)) {
			printk(KERN_ERR "KPANIC_MMC: MMC update clock HLE ERROR\n");
			return -1;
		}

		for (delay = 0; delay < MAX_DELAY_POLL_CMD; delay++) {
			/* just delay for a while */
		}
	}

	printk(KERN_ERR "KPANIC_MMC: MMC update clock time out ERROR\n");
	return -1;
}

/*
 * set mmc clock
 */
int k3v2_mmc_set_clock(struct raw_k3v2_mmc_host *host, unsigned int clock)
{
	unsigned int clock_div;


	if (clock) {
		clock_div = host->clock_base / (clock * 2);

		writel(clock_div, host->mmcioaddr + EMMC_REG_OFF_CLKDIV);
		writel(0, host->mmcioaddr + EMMC_REG_OFF_CLKSRC);
		writel(1, host->mmcioaddr + EMMC_REG_OFF_CLKENA);
	} else {
		writel(0, host->mmcioaddr + EMMC_REG_OFF_CLKDIV);
		writel(0, host->mmcioaddr + EMMC_REG_OFF_CLKSRC);
		writel(0, host->mmcioaddr + EMMC_REG_OFF_CLKENA);
	}

	return k3v2_mmc_update_clock(host);

}

/*
 * Set clock rate and bus width
 */
int k3v2_mmc_set_clock_and_width(struct raw_k3v2_mmc_host *host,
			unsigned int clock, unsigned int bus_width)
{
	int ret;
	int reg_val;

	switch (bus_width) {
	case 0:
		writel(0x0, host->mmcioaddr + EMMC_REG_OFF_CTYPE);
		ret = k3v2_mmc_update_ext_csd(host, EXTCSD_BUS_WIDTH, 0);
		break;
#if 0
	case 4:
		writel(0x1, EMMC_REG_OFF_CTYPE);
		ret = emmc_update_ext_csd(EXTCSD_BUS_WIDTH, 1);
		/* writel(0x1<<16, EMMC_REG_OFF_UHS); */
		break;
#endif
	case 8:
		writel(0x1<<16, host->mmcioaddr + EMMC_REG_OFF_CTYPE);
		ret = k3v2_mmc_update_ext_csd(host, EXTCSD_BUS_WIDTH, ( 2 + 4 ));
		writel( 0x1<<16, host->mmcioaddr + EMMC_REG_OFF_UHS );
		break;

	default:
		printk(KERN_ERR "KPANIC_MMC: default %s:%d\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if (ret) {
		printk(KERN_ERR "KPANIC_MMC:%s:%d\n", __FUNCTION__, __LINE__);
		return ret;
	}

	if (CS_CHIP_ID == get_chipid()) {
			reg_val = 0x07E00420;
			writel(reg_val, host->scioaddr + 0x108);

			// disable MMC1 clk_en
			reg_val = readl(host->pcioaddr + 0x1F8);
			reg_val = reg_val & (~BIT_PCTRL22_MMC1_CLKEN);
			writel(reg_val, host->pcioaddr + 0x1F8);

			// set MMC1 2��Ƶ 180��delay
			reg_val = readl(host->pcioaddr + 0x1F8);
			reg_val = (reg_val & 0xFF001FFF) | (0x6<<13) | (0x4<<16) | (0x6<<20);
			writel(reg_val, host->pcioaddr + 0x1F8);

			// enable MMC1 clk_en
			reg_val = readl(host->pcioaddr + 0x1F8);
			reg_val = reg_val | BIT_PCTRL22_MMC1_CLKEN;
			writel(reg_val, host->pcioaddr + 0x1F8);

			host->clock_base = 100*1000*1000;
		}

	/* Disable clk */
	ret = k3v2_mmc_set_clock(host, 0);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: disable clock failed\n");
		return ret;
	}

	/* set clock and enable clock */
	ret = k3v2_mmc_set_clock(host, clock);
	if (ret) {
		return -1;
	}


	return 0;
}

/*
 * check emmc mode is tran
 */
static int k3v2_mmc_check_tran_mode(struct raw_k3v2_mmc_host *host)
{
	int ret;
	unsigned int resp;
	unsigned int loop;

	writel(0xFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);

	for (loop = 0; loop < 1000; loop++) {
		ret =  k3v2_mmc_send_cmd(host, CMD13, EMMC_FIX_RCA << 16, &resp);
		DPRINTK("KPANIC_MMC: resp for cmd13:0x%x", resp);
		if (((resp >> 9) & 0xf) == 4)
		return 0;
	}

	printk(KERN_ERR "KPANIC_MMC: card mode %d\n", ((resp >> 9) & 0xf));
	return -1;
}

/*
 * reset emmc ip
 */
static int k3v2_mmc_reset_ip(struct raw_k3v2_mmc_host *host)
{
	volatile u32 reg_val;
	unsigned int loop;

	/* ip: reset all */
	reg_val = EMMC_RESET_ALL;

	writel(reg_val, host->mmcioaddr + EMMC_REG_OFF_CTRL);

	loop = 0;
	do {
		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_CTRL);
		if (loop <= 0x10000) {
			loop++;
		} else {
			printk(KERN_ERR "KPANIC_MMC: reset ip timeout\n");
			break;
		}
	} while (reg_val != 0);

	/* interrupt mask */
	writel(0x00000000, host->mmcioaddr + EMMC_REG_OFF_INTMSK);
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_IDINTEN);
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_IDSTS);


	/* config io */
	k3v2_sys_set_io(host, 1);

	/* power on */
	k3v2_sys_set_power(host, 1);
	mdelay(50);

	/* set rst2n high */
	k3v2_set_pmu_rst2n(host, 1);

	if (k3v2_mmc_set_clock(host, host->f_min)) {
		printk(KERN_ERR "KPANIC_MMC: k3v2_mmc_set_clock f_min failed\n");
	}

	/* bit mode: 1 bit */
	writel(0x0, host->mmcioaddr + EMMC_REG_OFF_CTYPE);

	/* non-DDR mode */
	writel(0x0, host->mmcioaddr + EMMC_REG_OFF_UHS);

	/* data time out */
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_TMOUT);

	/* debounce time out */
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_DEBNCE);

	/* clear bmod */
	writel(0, host->mmcioaddr + EMMC_REG_OFF_BMOD);


	/* fifo config */
	reg_val = (0x3F) << 16 | (0x40);
	writel(reg_val, host->mmcioaddr + EMMC_REG_OFF_FIFOTH);

	DPRINTK(KERN_ERR "KPANIC-MMC: EMMC_REG_OFF_FIFOTH:0x%x\n", reg_val);

	return 0;
}

/*
 * write function
 */
static int k3v2_mmc_write(char *buf, sector_t start_sect, sector_t nr_sects,
			unsigned int offset, unsigned int len)
{
	struct raw_k3v2_mmc_host *host;
	int ret = 0;
	unsigned int reg_val;
	unsigned int written_len;
	unsigned int total_len;
	unsigned int temp_len;
	unsigned int loop;

	host = &emmc_host;

	DPRINTK(KERN_ERR "KPANIC-MMC: %s : start_sect=%u, nr_sects=%u, "
		"offset=%u, len=%u\n", __func__, (unsigned int)start_sect,
		(unsigned int)nr_sects, offset, len);

	if (!len)
		return 0;

	if (offset + len > nr_sects * 512) {
		printk(KERN_ERR "KPANIC-MMC: writing buf too long for "
			"the partition\n");
		return 0;
	}

	if (offset % 512 != 0) {
		printk(KERN_ERR "KPANIC-MMC: writing offset not aligned to "
			"sector size\n");
		return 0;
	}

	writel(512, host->mmcioaddr + EMMC_REG_OFF_BLKSIZ);
	writel(len, host->mmcioaddr + EMMC_REG_OFF_BYTCNT);

	/* clear raw int */
	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);

	/* FIXME */
	/* how to disable dma */
	writel((readl(host->mmcioaddr+EMMC_REG_OFF_CTRL) & (~(BIT_DMA_ENABLE|BIT_INTERNAL_DMA))),
				host->mmcioaddr+EMMC_REG_OFF_CTRL);
	/* we assume emmc is addressed by block */
	ret = k3v2_mmc_send_cmd(host, CMD25, start_sect + (offset / 512), NULL);
	if (ret) {
		printk(KERN_ERR "KPANIC-MMC: send cmd25 failed");
		writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		return -1;
	}
	for (loop = 0; loop < 1000; loop++) {

		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		DPRINTK(KERN_ERR "KPANIC_MMC: cmd25 raw intr:0x%x\n", reg_val);
		if (reg_val & 0x4) {
			break;
		}
	}

	/* poll data */
	written_len = 0;
	total_len = len;
	while (written_len < total_len) {
		reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_STATUS);
		reg_val = (reg_val & (0x1FFF<<17))>>17;
		temp_len = 512 - reg_val * 4;
		if (temp_len > (total_len - written_len))
			temp_len = (total_len - written_len);
		for (loop = 0; loop < temp_len; loop += 4) {
			writel(*(unsigned int *)(buf + written_len + loop),
					host->mmcioaddr + EMMC_REG_OFF_FIFO_START);
		}
		written_len += temp_len;
	}

	/* poll dto */
	while (((reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS)) & BIT_INT_DTO) == 0) {
		/* got error */
	}


	/* check error */
	reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
	if (reg_val & 0xA280) {
		/* got error */
		printk(KERN_ERR "KPANIC-MMC: send CMD12 error:0x%x\n", reg_val);
		writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		return -1;
	}

	ret = k3v2_mmc_send_cmd(host, CMD12, EMMC_FIX_RCA<<16, NULL);
	if (ret) {
		printk(KERN_ERR "KPANIC-MMC: send CMD12 failed");
		writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		return -1;
	}

	/* check error 7,9,13,15, do not check crc error */
	reg_val = readl(host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
	if (reg_val & 0xA280) {
		/* got error */
		printk(KERN_ERR "KPANIC-MMC: send CMD12 error:0x%x\n", reg_val);
		writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);
		return -1;
	}

	while (1) {
		if (!(readl(host->mmcioaddr + EMMC_REG_OFF_STATUS) & BIT_INT_DATA_BUSY)) {
			break;
		}
	}

	writel(0xFFFFFFFF, host->mmcioaddr + EMMC_REG_OFF_RINTSTS);

	if (k3v2_mmc_check_tran_mode(host)) {
		printk(KERN_ERR "KPANIC-MMC: check_status failed\n");
		return -1;
	}

	return len;
}


/*
 * init mmc card
 */
static int k3v2_mmc_set_ready(struct raw_k3v2_mmc_host *host)
{
	unsigned int resp_buf[4];
	unsigned int ret;
	unsigned int loop;
	unsigned int reg;

	/* 1 bit mode */
	writel(0, host->mmcioaddr + EMMC_REG_OFF_CTYPE);

	/* cmd0 : IDLE */
	ret = k3v2_mmc_send_cmd(host, CMD0, 0, NULL);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: send cmd0 failed\n");
		return ret;
	}

	/* cmd1 : READY */
	for (loop = 0; loop < 1000; loop++) {

		ret = k3v2_mmc_send_cmd(host, CMD1, 0x40ff8000, resp_buf);
		if (ret) {
			printk(KERN_ERR "KPANIC_MMC: send cmd1 failed, loop:0x%x\n", loop);
			return ret;
		}

		/* wait for busy */
		if (resp_buf[0] & 0x80000000)
			break;
		mdelay(1);
	}

	if (1000 == loop) {
		printk(KERN_ERR "KPANIC_MMC: send cmd1 reach max time\n");
		return -1;
	}

	/* cmd2 : IDENT */
	ret = k3v2_mmc_send_cmd(host, CMD2, 0, resp_buf);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: send cmd2 failed\n");
		return ret;
	}

	/* CMD3 : STBY  */
	ret = k3v2_mmc_send_cmd(host, CMD3, EMMC_FIX_RCA<<16, resp_buf);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: send cmd3 failed\n");
		return ret;
	}

	/* CMD9 : get CSD */
	ret = k3v2_mmc_send_cmd(host, CMD9, EMMC_FIX_RCA<<16, resp_buf);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: send cmd9 failed\n");
		return ret;
	}

	/* CMD7 : TRAN */
	ret = k3v2_mmc_send_cmd(host, CMD7, EMMC_FIX_RCA<<16, NULL);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: send cmd7 failed\n");
		return ret;
	}

	/* END : Check */
	ret = k3v2_mmc_check_tran_mode(host);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: check_tran_mode failed\n");
		return ret;
	}

	/* high speed mode */
	ret = k3v2_mmc_update_ext_csd(host, EXTCSD_HS_TIMING, 1);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: EXTCSD_HS_TIMING failed\n");
		return ret;
	}

	if (DI_CHIP_ID == get_chipid()) {
		/* add for k3v2: set delay sel */
		writel(0xF0, host->scioaddr+0x064);
		writel(0x79, host->scioaddr+0x060);
	}

	ret = k3v2_mmc_set_clock_and_width(host, host->f_max, host->bus_width);
	if (ret) {
		printk(KERN_ERR "KPANIC_MMC: set_clock_and_width failed\n");
		return ret;
	}

	return 0;
}


/*
 * You never know current state of eMMC card when panic happens
 * So need a clear startup
 */
static int k3v2_mmc_probe(struct raw_hd_struct *rhd)
{
	/* need to check the start_sector and nr_sector value are valid */
	struct raw_k3v2_mmc_host *host;
	int ret = 0;

	DPRINTK(KERN_ERR "KPANIC-MMC: probe eMMC chip\n");

	kpanic_host = &emmc_host;	/* detect eMMC, then detect SD case */
	host = &emmc_host;
	memset(host, 0, sizeof(emmc_host));

	/* no more might_sleep after this point */
	local_irq_disable();

	/* init pmu address */
	host->pmuioaddr = IO_ADDRESS_PMU;
	host->mmcioaddr = IO_ADDRESS_EMMC;
	host->iocioaddr = IO_ADDRESS_IOC;
	host->scioaddr  = IO_ADDRESS_SC;
	host->pcioaddr  = IO_ADDRESS_PC;
	host->f_min		= 400*1000;			/* 400K */
	if (DI_CHIP_ID == get_chipid())
		host->f_max		= 40*1000*1000;		/* 25M */
	else
		host->f_max		= 50*1000*1000;		/* 25M */
	host->bus_width = 8;				/* 8bit */
	host->type		= MMC_TYPE_MMC;		/* mmc */

	/* config io */
	k3v2_sys_set_io(host, 0);

	/* set rst2n low */
	k3v2_set_pmu_rst2n(host, 0);
	mdelay(50);

	/* power off */
	k3v2_sys_set_power(host, 0);
	mdelay(40);

	/* enable clock */
	k3v2_sys_set_clock(host, 1);


	/* reset ip */
	k3v2_mmc_reset_ip(host);


	if(k3v2_mmc_set_ready(host)){
		k3v2_mmc_dumpregs(host);
	}

	return ret;
}

/*
 * raw_mmc_panic_probe
 */
int raw_mmc_panic_probe(struct raw_hd_struct *rhd, int type)
{

	if (type == MMC_TYPE_MMC)
		return k3v2_mmc_probe(rhd);
#if 0
	else if (type == MMC_TYPE_SD)
		return raw_mmc_probe_usd(rhd);
#endif
	else
		return -1;
}

/*
 * raw_mmc_panic_write
 */
int raw_mmc_panic_write(struct raw_hd_struct *rhd, char *buf,
			unsigned int offset, unsigned int len)
{
	return k3v2_mmc_write(buf, rhd->start_sect, rhd->nr_sects,
			offset, len);
}

/*
 * offset and len should be aligned to 512
 */
int raw_mmc_panic_erase(struct raw_hd_struct *rhd, unsigned int offset,
			unsigned int len)
{
	int ret = -1;

	if (!kpanic_host) {
		printk(KERN_ERR "KPANIC_MMC: no card probed\n");
		return -1;
	}
	if ((offset % 512) || (len % 512)) {
		printk(KERN_ERR "KPANIC_MMC: non-aligned erase\n");
		return -1;
	}
	if ((offset + len) / 512 > rhd->nr_sects) {
		printk(KERN_ERR "KPANIC_MMC: out of range erase\n");
		return -1;
	}
	if (!len)
		return 0;

	if (kpanic_host->type == MMC_TYPE_MMC)
		ret = -1;
#if 0
	else if (kpanic_host->card.type == MMC_TYPE_SD)
		ret = raw_sd_erase(kpanic_host, rhd->start_sect +
			offset / 512, len / 512);
#endif
	else
		printk(KERN_ERR "KPANIC_MMC: card.type not recognized %d\n",
			kpanic_host->type);

	if (ret)
		printk(KERN_ERR "KPANIC_MMC: erase mmc/SD failed\n");

	return ret;
}

