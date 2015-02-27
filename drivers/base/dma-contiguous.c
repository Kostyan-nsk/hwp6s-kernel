/*
 * Contiguous Memory Allocator for DMA mapping framework
 * Copyright (c) 2010-2011 by Samsung Electronics.
 * Written by:
 *	Marek Szyprowski <m.szyprowski@samsung.com>
 *	Michal Nazarewicz <mina86@mina86.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License or (at your optional) any later version of the license.
 */

#define pr_fmt(fmt) "cma: " fmt

#ifdef CONFIG_CMA_DEBUG
#ifndef DEBUG
#  define DEBUG
#endif
#endif

#include <asm/page.h>
#include <asm/dma-contiguous.h>

#include <linux/memblock.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/page-isolation.h>
#include <asm/sizes.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/mm_types.h>

struct cma {
	unsigned long	base_pfn;
	unsigned long	count;
	unsigned long	*bitmap;
};

struct cma *dma_contiguous_default_area;

#ifdef CONFIG_CMA_SIZE_MBYTES
#define CMA_SIZE_MBYTES CONFIG_CMA_SIZE_MBYTES
#else
#define CMA_SIZE_MBYTES 0
#endif

static struct cma_reserved {
	phys_addr_t start;
	unsigned long size;
	struct device *dev;
} cma_reserved[MAX_CMA_AREAS] __initdata;
static unsigned cma_reserved_count __initdata;

static DEFINE_MUTEX(cma_mutex);
/*
 * Default global CMA area size can be defined in kernel's .config.
 * This is usefull mainly for distro maintainers to create a kernel
 * that works correctly for most supported systems.
 * The size can be set in bytes or as a percentage of the total memory
 * in the system.
 *
 * Users, who want to set the size of global CMA area for their system
 * should use cma= kernel parameter.
 */
static const phys_addr_t size_bytes = CMA_SIZE_MBYTES * SZ_1M;
static phys_addr_t size_cmdline = -1;

atomic_long_t cma_stat[NR_CMA_ITEMS];
char *cma_name[NR_CMA_ITEMS] = {
	"total_cma",
	"total_ion_gralloc_cma",
	"total_codec_cma",
	"used_ion_gralloc_cma",
	"used_codec_cma"
};

void dump_cma_stat(void)
{
	printk("total_cma:0x%lx\n"
		"total_ion_gralloc_cma:0x%lx\n"
		"total_codec_cma:0x%lx\n"
		"used_ion_gralloc_cma:0x%lx\n"
		"used_codec_cma:0x%lx\n",
		cma_state(NR_TOTAL_CMA),
		cma_state(NR_TOTAL_ION_GRALLOC_CMA),
		cma_state(NR_TOTAL_CODEC_CMA),		
		cma_state(NR_USED_ION_GRALLOC_CMA),
		cma_state(NR_USED_CODEC_CMA));
}

static int __init early_cma(char *p)
{
	pr_debug("%s(%s)\n", __func__, p);
	size_cmdline = memparse(p, &p);
	return 0;
}
early_param("cma", early_cma);

#ifdef CONFIG_CMA_SIZE_PERCENTAGE

static phys_addr_t __init __maybe_unused cma_early_percent_memory(void)
{
	struct memblock_region *reg;
	unsigned long total_pages = 0;

	/*
	 * We cannot use memblock_phys_mem_size() here, because
	 * memblock_analyze() has not been called yet.
	 */
	for_each_memblock(memory, reg)
		total_pages += memblock_region_memory_end_pfn(reg) -
			       memblock_region_memory_base_pfn(reg);

	return (total_pages * CONFIG_CMA_SIZE_PERCENTAGE / 100) << PAGE_SHIFT;
}

#else

static inline __maybe_unused phys_addr_t cma_early_percent_memory(void)
{
	return 0;
}

#endif

/**
 * dma_contiguous_reserve() - reserve area for contiguous memory handling
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory from early allocator. It should be
 * called by arch specific code once the early allocator (memblock or bootmem)
 * has been activated and all other subsystems have already allocated/reserved
 * memory.
 */
void __init dma_contiguous_reserve(phys_addr_t limit)
{
	phys_addr_t selected_size = 0;

	pr_debug("%s(limit %08lx)\n", __func__, (unsigned long)limit);

	if (size_cmdline != -1) {
		selected_size = size_cmdline;
	} else {
#ifdef CONFIG_CMA_SIZE_SEL_MBYTES
		selected_size = size_bytes;
#elif defined(CONFIG_CMA_SIZE_SEL_PERCENTAGE)
		selected_size = cma_early_percent_memory();
#elif defined(CONFIG_CMA_SIZE_SEL_MIN)
		selected_size = min(size_bytes, cma_early_percent_memory());
#elif defined(CONFIG_CMA_SIZE_SEL_MAX)
		selected_size = max(size_bytes, cma_early_percent_memory());
#endif
	}
	cma_log(CMA_LOG_INFO, "selected_size:0x%08x %s\n", selected_size, __FUNCTION__);
	if (selected_size) {
		pr_debug("%s: reserving %ld MiB for global area\n", __func__,
			 (unsigned long)selected_size / SZ_1M);

		dma_declare_contiguous(NULL, selected_size, 0, limit);
	}
};



static __init int cma_activate_area(unsigned long base_pfn, unsigned long count)
{
	unsigned long pfn = base_pfn;
	unsigned i = count >> pageblock_order;
	struct zone *zone;

	zone = page_zone(pfn_to_page(pfn));

	do {
		unsigned j;
		base_pfn = pfn;
		for (j = pageblock_nr_pages; j; --j, pfn++) {
			if (page_zone(pfn_to_page(pfn)) != zone)
				return -EINVAL;
		}
		init_cma_reserved_pageblock(pfn_to_page(base_pfn));
	} while (--i);
	return 0;
}

static __init struct cma *cma_create_area(unsigned long base_pfn,
				     unsigned long count)
{
	int bitmap_size = BITS_TO_LONGS(count) * sizeof(long);
	struct cma *cma;
	int ret = -ENOMEM;

	pr_debug("%s(base %08lx, count %lx)\n", __func__, base_pfn, count);

	cma = kmalloc(sizeof *cma, GFP_KERNEL);
	if (!cma)
		return ERR_PTR(-ENOMEM);

	cma->base_pfn = base_pfn;
	cma->count = count;
	cma->bitmap = kzalloc(bitmap_size, GFP_KERNEL);

	if (!cma->bitmap)
		goto no_mem;

	ret = cma_activate_area(base_pfn, count);
	if (ret)
		goto error;

	pr_debug("%s: returned %p\n", __func__, (void *)cma);
	return cma;

error:
	kfree(cma->bitmap);
no_mem:
	kfree(cma);
	return ERR_PTR(ret);
}

static int __init cma_init_reserved_areas(void)
{
	struct cma_reserved *r = cma_reserved;
	unsigned i = cma_reserved_count;

	pr_debug("%s()\n", __func__);

	for (; i; --i, ++r) {
		struct cma *cma;
		cma = cma_create_area(PFN_DOWN(r->start),
				      r->size >> PAGE_SHIFT);
		if (!IS_ERR(cma))
			dev_set_cma_area(r->dev, cma);
	}
	return 0;
}
core_initcall(cma_init_reserved_areas);

/**
 * dma_declare_contiguous() - reserve area for contiguous memory handling
 *			      for particular device
 * @dev:   Pointer to device structure.
 * @size:  Size of the reserved memory.
 * @base:  Start address of the reserved memory (optional, 0 for any).
 * @limit: End address of the reserved memory (optional, 0 for any).
 *
 * This function reserves memory for specified device. It should be
 * called by board specific code when early allocator (memblock or bootmem)
 * is still activate.
 */
int __init dma_declare_contiguous(struct device *dev, phys_addr_t size,
				  phys_addr_t base, phys_addr_t limit)
{
	struct cma_reserved *r = &cma_reserved[cma_reserved_count];
	phys_addr_t alignment;
	phys_addr_t temp;

	cma_log(CMA_LOG_INFO, "%s(size 0x%08x, base 0x%08x, limit 0x%08x)\n", __func__, size, base, limit);

	/* Sanity checks */
	if (cma_reserved_count == ARRAY_SIZE(cma_reserved)) {
		pr_err("Not enough slots for CMA reserved regions!\n");
		return -ENOSPC;
	}

	if (!size)
		return -EINVAL;

	/* Sanitise input arguments */
	alignment = PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order);
	limit &= ~(alignment - 1);

	/* Reserve memory */
	if (base) {
		if (memblock_is_region_reserved(base, size) ||
		    memblock_reserve(base, size) < 0) {
			cma_log(CMA_LOG_EMERG, "%s: memblock_remove return error\n", __FUNCTION__);
			base = -EBUSY;
			goto err;
		}
	} else {
		/*
		 * Use __memblock_alloc_base() since
		 * memblock_alloc_base() panic()s.
		 */
		phys_addr_t addr = __memblock_alloc_base(size, alignment, limit);
		if (!addr) {
			base = -ENOMEM;
			goto err;
		} else {
			base = addr;
		}
	}

	temp = base;
	base = ALIGN(base, alignment);
	size -= (base - temp);
	if(size%alignment)
		size = ALIGN(size, alignment) - alignment;

	/*
	 * Each reserved area must be initialised later, when more kernel
	 * subsystems (like slab allocator) are available.
	 */
	r->start = base;
	r->size = size;
	r->dev = dev;
	cma_reserved_count++;
	cma_log(CMA_LOG_INFO, "%s: reserved area: size 0x%08x, base 0x%08x, limit 0x%08x)\n", __func__, size, base, limit);
	mod_cma_state(size, NR_TOTAL_CMA);

	return 0;
err:
	cma_log(CMA_LOG_EMERG, "%s: failed to reserve area: size 0x%08x, base 0x%08x, limit 0x%08x)\n", __func__, size, base, limit);
	return base;
}

u64 cma_log_max_alloc_time = 0;
u32 cma_log_alloc_retries = 0;
u32 cma_log_alloc_errors = 0;
/**
 * dma_alloc_from_contiguous() - allocate pages from contiguous area
 * @dev:   Pointer to device for which the allocation is performed.
 * @count: Requested number of pages.
 * @align: Requested alignment of pages (in PAGE_SIZE order).
 *
 * This function allocates memory buffer for specified device. It uses
 * device specific contiguous memory area if available or the default
 * global one. Requires architecture specific get_dev_cma_area() helper
 * function.
 */
struct page *dma_alloc_from_contiguous(struct device *dev, int count,
				       unsigned int align)
{
	unsigned long mask, pfn, pageno, start = 0;
	struct cma *cma = dev_get_cma_area(dev);
	struct page *page = NULL;
	int ret;
	unsigned long failed_pfn = 0;
	u64 start_nsec, end_nsec;

	start_nsec = cpu_clock(smp_processor_id());
	
	if (!cma || !cma->count)
		return NULL;

	if (align > CONFIG_CMA_ALIGNMENT)
		align = CONFIG_CMA_ALIGNMENT;

	pr_debug("%s(cma %p, count %d, align %d)\n", __func__, (void *)cma,
		 count, align);

	if (!count)
		return NULL;

	mask = (1 << align) - 1;

	mutex_lock(&cma_mutex);

	for (;;) {
		pageno = bitmap_find_next_zero_area(cma->bitmap, cma->count,
						    start, count, mask);
		if (pageno >= cma->count)
		{
			cma_log(CMA_LOG_EMERG, "%s: error: out of memory\n", __FUNCTION__);
			dump_cma_stat();
			cma_log_alloc_errors++;
			break;
		}

		pfn = cma->base_pfn + pageno;
		ret = alloc_contig_range(pfn, pfn + count, MIGRATE_CMA, &failed_pfn);
		if (ret == 0) {
			bitmap_set(cma->bitmap, pageno, count);
			page = pfn_to_page(pfn);
			break;
		} else if (ret != -EBUSY) {
			cma_log_alloc_errors++;
			cma_log(CMA_LOG_EMERG, "%s: error: ret(%d) != -EBUSY\n", __FUNCTION__, ret);
			break;
		}

		/* try again with a bit different memory target */
		cma_log_alloc_retries++;
		if (unlikely(failed_pfn)) {
			if(align >= pageblock_order)
				start = ALIGN(failed_pfn + 1 - cma->base_pfn, 1 << align);
			else
				start = ALIGN(failed_pfn + 1 - cma->base_pfn, 1 << pageblock_order);
			
			cma_log(CMA_LOG_WARNING, "%s: failed_pfn:0x%lx, start:0x%lx, align:%u, start_pfn:0x%lx\n", __FUNCTION__,
										failed_pfn, start, align, start + cma->base_pfn);
			failed_pfn = 0;
		}
		else
		{
			start = pageno + mask + 1;
			cma_log(CMA_LOG_WARNING, "%s: start:0x%lx, align:%u, start_pfn:0x%lx\n", __FUNCTION__,
										start, align, start + cma->base_pfn);
		}
	}

	cma_log(CMA_LOG_DEBUG, "lubing %s():phys:0x%x,size:0x%x,align:0x%x  \n", __func__, page_to_phys(page), count<<PAGE_SHIFT, align);
	mutex_unlock(&cma_mutex);
	end_nsec = cpu_clock(smp_processor_id());
	end_nsec -= start_nsec;
	if (end_nsec > cma_log_max_alloc_time)
		cma_log_max_alloc_time = end_nsec;
	return page;
}

/**
 * dma_release_from_contiguous() - release allocated pages
 * @dev:   Pointer to device for which the pages were allocated.
 * @pages: Allocated pages.
 * @count: Number of allocated pages.
 *
 * This function releases memory allocated by dma_alloc_from_contiguous().
 * It returns false when provided pages do not belong to contiguous area and
 * true otherwise.
 */
bool dma_release_from_contiguous(struct device *dev, struct page *pages,
				 int count)
{
	struct cma *cma = dev_get_cma_area(dev);
	unsigned long pfn;

	if (!cma || !pages)
		return false;

	pr_debug("%s(page %p)\n", __func__, (void *)pages);

	pfn = page_to_pfn(pages);

	if (pfn < cma->base_pfn || pfn >= cma->base_pfn + cma->count)
		return false;

	VM_BUG_ON(pfn + count > cma->base_pfn + cma->count);

	mutex_lock(&cma_mutex);
	bitmap_clear(cma->bitmap, pfn - cma->base_pfn, count);
	free_contig_range(pfn, count);
	mutex_unlock(&cma_mutex);

	return true;
}

void set_cma_log_level(unsigned int level)
{
	cma_log(CMA_LOG_EMERG, "%s: previous level is %d. level is set to %d.\n", __FUNCTION__, cma_log_ignore_level, level);
	cma_log_ignore_level = level;
}

#ifdef CMA_TEST
#include <linux/delay.h>
struct device test_cma_dev = {
	.coherent_dma_mask = 0xffffffffULL,
};
bool test_cma_free(struct page *cma_pages, int pages)
{
	bool ret;

	ret = dma_release_from_contiguous(&test_cma_dev, cma_pages, pages);
	if(ret == false)
	{
		cma_log(CMA_LOG_EMERG, "%s: error: dma_release_from_contiguous return false\n", __FUNCTION__);
		return false;
	}
	return ret;
}

void test_cma_alloc(int pages)
{
	
	struct page *cma_pages;

	cma_log(CMA_LOG_INFO, "%s: pages:0x%08x\n", __FUNCTION__, pages);
	cma_pages = dma_alloc_from_contiguous(&test_cma_dev, pages, 0);
	if(cma_pages== NULL)
	{
		cma_log(CMA_LOG_EMERG, "%s: error: dma_alloc_from_contiguous return NULL\n", __FUNCTION__);
		return;
	}
	cma_log(CMA_LOG_INFO, "%s: dma_alloc_from_contiguous return 0x%p\n", __FUNCTION__, cma_pages);
	cma_log(CMA_LOG_INFO, "%s: page:0x%p pfn:0x%08lx pa:0x%08x\n", __FUNCTION__, cma_pages,
		page_to_pfn(cma_pages), page_to_phys(cma_pages));

	test_cma_free(cma_pages, pages);
}

unsigned int test_cma_alloc_long_pressure_flag = 1;
unsigned int test_cma_alloc_long_pressure_loops = 0;
unsigned int test_cma_alloc_long_pressure_inverval = 10;
u32 test_cma_delay_msecs = 2000;
int test_cma_size = SZ_256M;
int test_cma_alloc_long_pressure_pages = (SZ_128M)>>PAGE_SHIFT;

void set_test_cma_alloc_long_pressure_parameters(unsigned int flag, unsigned int interval,  int pages, u32 delay_msecs)
{
	cma_log(CMA_LOG_INFO, "%s: flag:%u interval:%u pages:0x%08x\n", __FUNCTION__, flag, interval, pages);
	test_cma_alloc_long_pressure_flag = flag;
	if(interval)
	{
		test_cma_alloc_long_pressure_inverval = interval;
	}
	if(pages)
	{
		test_cma_alloc_long_pressure_pages = pages;
	}

	test_cma_delay_msecs = delay_msecs;
}

void test_cma_dump_info(void)
{
	cma_log(CMA_LOG_INFO, "%s: loops:%u\n", __FUNCTION__, test_cma_alloc_long_pressure_loops);
}

int test_cma_alloc_long_pressure(void *not_used)
{
	int pages = test_cma_alloc_long_pressure_pages;
	struct page *cma_pages;

	cma_log(CMA_LOG_INFO, "%s 026: alloc pages(0x%08x)\n", __FUNCTION__, pages);
	
	while(test_cma_alloc_long_pressure_flag)
	{
		cma_log(CMA_LOG_INFO, "%s: loops:%u\n", __FUNCTION__, test_cma_alloc_long_pressure_loops);
		cma_pages = dma_alloc_from_contiguous(&test_cma_dev, pages, 0);
		if(cma_pages== NULL)
		{
			cma_log(CMA_LOG_EMERG, "%s: error: dma_alloc_from_contiguous return NULL\n",  __FUNCTION__);
			test_cma_dump_info();
			return 0;
		}

		msleep(100);

		if(dma_release_from_contiguous(&test_cma_dev, cma_pages, pages) == false)
		{
			cma_log(CMA_LOG_EMERG, "%s: error: dma_release_from_contiguous return false\n",  __FUNCTION__);
			test_cma_dump_info();
			return 0;
		}
		msleep(test_cma_delay_msecs);

		test_cma_alloc_long_pressure_loops++;
	}
	return 0;
}

static int create_cma_test_thread(void)
{
	pid_t pid;
	test_cma_alloc_long_pressure_pages = test_cma_size>>PAGE_SHIFT;
	pid = kernel_thread(test_cma_alloc_long_pressure, (void *)&test_cma_alloc_long_pressure_pages, CLONE_FS | CLONE_SIGHAND);
}

module_init(create_cma_test_thread);

void test_alloc_mem(u32 order, int count)
{
	s32 i;
	struct page *page;

	cma_log(CMA_LOG_EMERG, "%s: order:%u count:%d total:0x%lx\n",  __FUNCTION__, order, count, count*(PAGE_SIZE<<order));	
	for(i = 0; i < count; i++)
	{
		page = alloc_pages(GFP_HIGHUSER_MOVABLE, order);
		if(page == NULL)
		{
			cma_log(CMA_LOG_EMERG, "%s: error:alloc_pages return NULL\n",  __FUNCTION__);		
			return;
		}
	}	
}
#endif

