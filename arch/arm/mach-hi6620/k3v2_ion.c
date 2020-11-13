/*
 * ION Initialization for K3V24.
 *
 * Copyright (C) 2011 Hisilicon
 *
 * Author: zhangniangao@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/io.h>
#include <linux/ata_platform.h>
#include <linux/amba/mmci.h>

#include <asm/clkdev.h>
#include <asm/system.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/mach-types.h>
#include <asm/hardware/arm_timer.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>

#include <asm/hardware/gic.h>

#include <mach/platform.h>
#include <mach/irqs.h>
#include <mach/early-debug.h>
#include <linux/bootmem.h>

#include <linux/ion.h>
#include <linux/memblock.h>
#include <linux/hisi_ion.h>
#include <mach/hisi_mem.h>
#include <hsad/config_mgr.h>

#ifdef CONFIG_CMA //added by qijiwen
#include <asm/dma-contiguous.h>
#endif //added by qijiwen end

//Ԥ���������ַ
unsigned long hisi_reserved_gpu_phymem;
unsigned long hisi_reserved_fb_phymem;
unsigned long hisi_reserved_codec_phymem;
unsigned long hisi_reserved_dumplog_phymem;
unsigned long hisi_reserved_camera_phymem;
unsigned long hisi_reserved_vpp_phymem;
unsigned long hisi_reserved_codec_fix_size;
EXPORT_SYMBOL(hisi_reserved_gpu_phymem);
EXPORT_SYMBOL(hisi_reserved_fb_phymem);
EXPORT_SYMBOL(hisi_reserved_codec_phymem);
EXPORT_SYMBOL(hisi_reserved_dumplog_phymem);
EXPORT_SYMBOL(hisi_reserved_camera_phymem);
EXPORT_SYMBOL(hisi_reserved_vpp_phymem);
EXPORT_SYMBOL(hisi_reserved_codec_fix_size);

#define HISI_LCD_SIZE_NAME 10
struct hisi_reserved_media_memory {
    unsigned char lcd_name[HISI_LCD_SIZE_NAME];
    unsigned long gpu_size;
    unsigned long codec_size;
    unsigned long camera_size;
    unsigned long gralloc_size;
	unsigned long vpp_size;
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    unsigned long overlay_size;
#endif
*/
	unsigned long fb_size;
};

extern int get_isfullhd(void);
void ion_config_set(void);
unsigned long HISI_FRAME_BUFFER_SIZE;
unsigned long PLAT_MEM_RESERVED_SIZE_NEW = 0;


extern unsigned int g_ddr_size;

static struct hisi_reserved_media_memory hisi_media_mem = {
    .lcd_name = "720p",
    .gpu_size = HISI_MEM_GPU_SIZE,
    .codec_size = HISI_MEM_CODEC_SIZE,
    .camera_size = HISI_PMEM_CAMERA_SIZE,
    .gralloc_size = HISI_PMEM_GRALLOC_SIZE,
    .vpp_size = HISI_MEM_VPP_SIZE,
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    .overlay_size = HISI_PMEM_OVERLAY_SIZE,
#endif
*/
    .fb_size = HISI_MEM_FB_SIZE,
};

static struct ion_platform_data k3v2_ion_data = {
    .nr = 5,
    .heaps = {
        {
            .type = ION_HEAP_TYPE_CARVEOUT,
            .id = HISI_ION_HEAP_CAMERA_ID,
            .name = "camera",
        },
	 {
            .type = ION_HEAP_TYPE_CARVEOUT,
            .id = HISI_ION_HEAP_GRALLOC_ID,
            .name = "gralloc",
//            .cached = 1,
        },
/*
        {
            .type = ION_HEAP_TYPE_CARVEOUT,
            .id = HISI_ION_HEAP_OVERLAY_ID,
            .name = "overlay",
//	    .cached = 1,
        },
*/
       {
            .type = ION_HEAP_TYPE_SYSTEM,
            .id = HISI_ION_HEAP_SYS_ID,
            .name = "sys_user",
        },
       {
            .type = ION_HEAP_TYPE_SYSTEM_CONTIG,
            .id = HISI_ION_HEAP_SYS_CONTIG_ID,
            .name = "sys_contig",
        },
       {
            .type = ION_HEAP_TYPE_DMA,
            .id = HISI_ION_HEAP_DMA_ID,
            .name = "cma_heap",
        },		
    },
};

static struct platform_device k3v2_ion_device = {
    .name = "ion-k3v2",
    .id = -1,
    .dev = {
        .platform_data = &k3v2_ion_data,
    },
};

static int __init k3v2_register_ion(void)
{
#ifdef CONFIG_CMA //added by qijiwen
	if (!get_cma_type()) {
		struct page *cma_pages;
		u32 size = (u32)hisi_get_reserve_mem_size();
		
		cma_pages = dma_alloc_from_contiguous(NULL, size>>PAGE_SHIFT, 0);
		cma_log(CMA_LOG_INFO, "%s: 101 pages:0x%08x phy:0x%08x\n", 
					__FUNCTION__, size>>PAGE_SHIFT, page_to_phys(cma_pages));
		if(cma_pages== NULL)
		{
			cma_log(CMA_LOG_INFO, "%s: error: dma_alloc_from_contiguous return NULL\n", __FUNCTION__);
			return -ENOMEM;
		}
	}
#endif //added by qijiwen end
    printk("=====ion device register");
    platform_device_register(&k3v2_ion_device);
    return 0;
}

arch_initcall(k3v2_register_ion);

#ifdef CONFIG_CMA //added by qijiwen
u64 gralloc_dma_mask = 0xffffffffULL;
struct device gralloc_cma_dev = {
	.dma_mask = &gralloc_dma_mask,
	.coherent_dma_mask = 0xffffffffULL,
};
u64 codec_dma_mask = 0xffffffffULL;
struct device codec_cma_dev = {
	.dma_mask = &codec_dma_mask,
	.coherent_dma_mask = 0xffffffffULL,
};
#endif //added by qijiwen end

u32 codec_cma_base;
u32 codec_cma_size;
u32 dma_cma_base = 0;
u32 dma_cma_size = SZ_16M; //qijiwen must be the multiple of pageblock(i.e. SZ_4M)

unsigned long hisi_get_reserve_cma_size(void)
{
    unsigned long reserved = 0;

    reserved += hisi_media_mem.gpu_size;
    reserved += hisi_media_mem.camera_size;
    reserved += hisi_media_mem.vpp_size;
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    reserved += hisi_media_mem.overlay_size;
#endif
*/
    reserved += hisi_media_mem.fb_size;
    reserved += CODEC_FIX_SIZE;
    reserved += GRALLOC_FIX_SIZE;
    //reserved += HISI_PMEM_DUMPLOG_SIZE;

    reserved = PAGE_ALIGN(reserved);

    printk("%s: reserved:0x%lx\n", __FUNCTION__, reserved);
    return reserved;
}

unsigned long hisi_get_reserve_info(void)
{
    unsigned long reserved = 0;

    reserved += hisi_media_mem.gpu_size;
    reserved += hisi_media_mem.codec_size;
    reserved += hisi_media_mem.vpp_size;
    reserved += hisi_media_mem.fb_size;
    return reserved;
}

unsigned long hisi_get_reserve_mem_size(void)
{
    unsigned long reserved = 0;
    unsigned long temp;

    ion_config_set();
    reserved += hisi_media_mem.gpu_size;
    reserved += hisi_media_mem.camera_size;
    reserved += hisi_media_mem.vpp_size;
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    reserved += hisi_media_mem.overlay_size;
#endif
*/
    reserved += hisi_media_mem.fb_size;
    //reserved += HISI_PMEM_DUMPLOG_SIZE;
    reserved += hisi_media_mem.codec_size;
#ifdef CONFIG_CMA //added by qijiwen should be here, but cause error
    if (get_cma_type()) {
		reserved += CODEC_FIX_SIZE;
		reserved -= CODEC_DEC_SIZE;
		reserved += GRALLOC_FIX_SIZE;
        temp = ALIGN(reserved, PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order));	
        hisi_media_mem.codec_size += (temp - reserved);
        reserved = temp;
    }
#endif //added by qijiwen end
    mod_cma_state(hisi_media_mem.codec_size, NR_TOTAL_CODEC_CMA);

    reserved += hisi_media_mem.gralloc_size;
    if (get_cma_type()) {
		reserved -= GRALLOC_DEC_SIZE;
    }
#ifdef CONFIG_CMA //added by qijiwen should be here, but cause error
        temp = ALIGN(reserved, PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order));	
        hisi_media_mem.gralloc_size += (temp - reserved);
        reserved = temp;
#endif //added by qijiwen end
    mod_cma_state(hisi_media_mem.gralloc_size, NR_TOTAL_ION_GRALLOC_CMA);

    if (get_cma_type()) {
        reserved += dma_cma_size;
    }

    return reserved;
}

void __init k3v2_allocate_memory_regions(void)
{
    unsigned long reserved_base = g_ddr_size;
    unsigned long reserved_size;
    unsigned long size;

    edb_trace(1);

    reserved_size = hisi_get_reserve_mem_size();

    reserved_base -= reserved_size;
    codec_cma_base = reserved_base;

    /* GPU memory */
    size = hisi_media_mem.gpu_size;
    hisi_reserved_gpu_phymem = reserved_base;
    printk("ion allocating 0x%lx bytes at (0x%lx physical) for gpu "
        "pmem area\n", size, reserved_base);

    reserved_base += size;
    
	/* VPP memory */
	size = hisi_media_mem.vpp_size;
	hisi_reserved_vpp_phymem = reserved_base;
	printk("vpp ion allocating 0x%lx bytes at (0x%lx physical) for vpp "
		"pmem area\n", size, reserved_base);

    reserved_base += size;

    /* CAMERA memory pmem */
    size = hisi_media_mem.camera_size;
    if (size) {
        k3v2_ion_data.heaps[0].base = reserved_base;
        k3v2_ion_data.heaps[0].size	= size;
        hisi_reserved_camera_phymem = k3v2_ion_data.heaps[0].base;
        printk("camera ion allocating 0x%lx bytes at (0x%lx physical) for camera pic\n",
            size, reserved_base);
    }

    reserved_base += size;
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    size = hisi_media_mem.overlay_size;
    if (size) {
        k3v2_ion_data.heaps[2].base = reserved_base;
        k3v2_ion_data.heaps[2].size	= size;
        printk("overlay ion allocating 0x%lx bytes at (0x%lx physical) for overlay "
            "pmem area\n", size, reserved_base);
    }

    reserved_base += size;
#endif
*/
	/*FB memory*/
	size = hisi_media_mem.fb_size;
	hisi_reserved_fb_phymem = reserved_base;
	printk("fb ion allocating 0x%lx bytes at (0x%lx physical) for fb "
		"pmem area\n",size,reserved_base);

    reserved_base += size;

    /* CODEC memory */
    size = hisi_media_mem.codec_size;
	if (get_cma_type()) {
		size += CODEC_FIX_SIZE;
		size -= CODEC_DEC_SIZE;
		printk("codec fix 0x%lx bytes dec 0x%x bytes\n", 
				CODEC_FIX_SIZE, CODEC_DEC_SIZE);
	}

    /*Revived by y44207 ,V200 64 byte align*/
    hisi_reserved_codec_phymem = reserved_base;
    printk("codec ion allocating 0x%lx bytes at (0x%lx physical) for codec "
           "pcodec area\n", size, reserved_base);
    reserved_base += size;

    codec_cma_size = reserved_base - codec_cma_base;

    /* video decoder display && camera preview + Thumbnail */
    size = hisi_media_mem.gralloc_size;
	if (get_cma_type()) {
		size += GRALLOC_FIX_SIZE;
		size -= GRALLOC_DEC_SIZE;
		printk("gralloc fix 0x%x bytes dec 0x%x bytes\n", 
				GRALLOC_FIX_SIZE, GRALLOC_DEC_SIZE);
	}
    if (size) {
        k3v2_ion_data.heaps[1].base = reserved_base;
        k3v2_ion_data.heaps[1].size	= size;
        printk("gralloc ion allocating %lu bytes at (%lx physical) for gralloc "
            "pmem area\n", size, reserved_base);
    }

    reserved_base += size;
    dma_cma_base = (u32)reserved_base;

    /* dumplog memory */
    //size = HISI_PMEM_DUMPLOG_SIZE;
    /*Revived by y44207 ,V200 64 byte align*/
    //hisi_reserved_dumplog_phymem = reserved_base;
    //printk("dumplog ion allocating %lu bytes at (%lx physical) for dumplog"
    //    "area\n", size, reserved_base);

    //reserved_base += size;

}

void ion_config_set(void)
{
    /*0:gpu  1:codec   2:camera  3:gralloc
       4:vpp   5:overlay 6:fb         7:ram*/
    int media_sz[8];
    int ret=0;
    int camera_width;
    int camera_hight;

    memset(media_sz, 0, sizeof(media_sz));

    ret += get_hw_config_int("memory_config/gpu",     &media_sz[0], NULL);
    ret += get_hw_config_int("memory_config/codec",   &media_sz[1], NULL);
    ret += get_hw_config_int("memory_config/camera",  &media_sz[2], NULL);
    ret += get_hw_config_int("memory_config/gralloc", &media_sz[3], NULL);
    ret += get_hw_config_int("memory_config/vpp",     &media_sz[4], NULL);
    ret += get_hw_config_int("memory_config/overlay", &media_sz[5], NULL);
    ret += get_hw_config_int("memory_config/fb",      &media_sz[6], NULL);
    ret += get_hw_config_int("memory_config/ram",     &media_sz[7], NULL);
    if( ret != 8 )
        printk(KERN_ERR " %s read memery config from hw_config.xml failed !!!\n", __func__);

    hisi_media_mem.gpu_size     = PAGE_ALIGN(media_sz[0] * SZ_1K);
    hisi_media_mem.codec_size   = PAGE_ALIGN(media_sz[1] * SZ_1K);
    hisi_media_mem.camera_size  = PAGE_ALIGN(media_sz[2] * SZ_1K);
    hisi_media_mem.gralloc_size = PAGE_ALIGN(media_sz[3] * SZ_1K);
    hisi_media_mem.vpp_size     = PAGE_ALIGN(media_sz[4] * SZ_1K);
/*
#if defined(CONFIG_OVERLAY_COMPOSE)
    hisi_media_mem.overlay_size = PAGE_ALIGN(media_sz[5] * SZ_1K);
#endif
*/
    hisi_media_mem.fb_size      = PAGE_ALIGN(media_sz[6] * SZ_1K);

	HISI_FRAME_BUFFER_SIZE = PAGE_ALIGN(media_sz[6] * SZ_1K);
	get_hw_config_int("camera/primary_sensor_raw_w",     &camera_width, NULL);
	get_hw_config_int("camera/primary_sensor_raw_h",     &camera_hight, NULL);
	hisi_reserved_codec_fix_size = camera_width*camera_hight*3/2;
	hisi_reserved_codec_fix_size = PAGE_ALIGN(hisi_reserved_codec_fix_size);
	printk("hisi_reserved_codec_fix_size 0x%lx bytes width:%d,hight:%d\n", 
		hisi_reserved_codec_fix_size, camera_width, camera_hight);	
}


static int __init k3v2_cma_reserve(void)
{
	unsigned long reserved_size;
	struct page *cma_pages;

 	if (get_cma_type()) {
		reserved_size = hisi_get_reserve_cma_size();		
		cma_pages = dma_alloc_from_contiguous(&codec_cma_dev, reserved_size>>PAGE_SHIFT, 0);
		printk(KERN_INFO "%s: pages:0x%08lx phy:0x%08x\n", 
						__FUNCTION__, reserved_size>>PAGE_SHIFT, page_to_phys(cma_pages));
		if(cma_pages== NULL)
		{
			printk(KERN_INFO "%s: error: dma_alloc_from_contiguous return NULL\n", __FUNCTION__);
		}
 	}
	return 0;
}
core_initcall_sync(k3v2_cma_reserve);

static char cma_type = '0';
int get_cma_type(void)
{
    if (cma_type == '0')
        return 0;
    else
        return 1;
}

static int __init early_parse_cma_cmdline(char *cma_cmdline)
{
    if (strlen(cma_cmdline) != 1)
    {
        printk(KERN_ERR "error: invalid cma cmdline size!\n");
        return -1;
    }
    cma_type = cma_cmdline[0];
    pr_err("%s:cma_type:%c\n", __func__, cma_type);

    return 0;
}
early_param("cma", early_parse_cma_cmdline);

void get_cma_config(struct device *dev, u32 *size, u32 *base)
{
	if(dev == NULL)
	{
		*base = dma_cma_base;
		*size = dma_cma_size;
		printk(KERN_EMERG "%s global CMA device base:0x%x size:0x%x\n", __FUNCTION__, *base, *size);
	}
	else if(dev == &gralloc_cma_dev)
	{
		*base = (u32)k3v2_ion_data.heaps[1].base;
		*size = (u32)k3v2_ion_data.heaps[1].size;
		printk(KERN_EMERG "%s gralloc CMA device base:0x%x size:0x%x\n", __FUNCTION__, *base, *size);
	}
	else if(dev == &codec_cma_dev)
	{
		*base = codec_cma_base;
		*size = codec_cma_size;
		printk(KERN_EMERG "%s codec CMA device base:0x%x size:0x%x\n", __FUNCTION__, *base, *size);
	}
	else
	{
		printk(KERN_EMERG "%s invalid CMA device:%p\n", __FUNCTION__, dev);
	}	
}

#ifdef CMA_TEST
extern struct device test_cma_dev;
extern int test_cma_size;
#endif

void ion_mem_reserve(void)
{
	u32 size, base;
	unsigned long reserved_size;

	k3v2_allocate_memory_regions();

#ifdef CONFIG_CMA //added by qijiwen
	if (get_cma_type()) {
		get_cma_config(NULL, &size, &base);
		if(size != 0)
		{
			dma_declare_contiguous(NULL, size, base, 0);
		}
		get_cma_config(&gralloc_cma_dev, &size, &base);
		if(size != 0)
		{
			dma_declare_contiguous(&gralloc_cma_dev, size, base, 0);
		}
		get_cma_config(&codec_cma_dev, &size, &base);
		if(size != 0)
		{
			dma_declare_contiguous(&codec_cma_dev, size, base, 0);
		}
	} else {
		reserved_size = hisi_get_reserve_mem_size();
		base = g_ddr_size - reserved_size;
		printk(KERN_INFO "%s the reserved memor region base:0x%08lx size:0x%08lx\n", __func__, (unsigned long)base, reserved_size);
		dma_declare_contiguous(NULL, reserved_size, base, 0);
	}
#else
	memblock_remove(base, reserved_size);
#endif //added by qijiwen end
#ifdef CMA_TEST
	dma_declare_contiguous(&test_cma_dev, test_cma_size, 0, 0);
#endif
}

