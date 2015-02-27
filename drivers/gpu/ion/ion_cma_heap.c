/*
 * drivers/gpu/ion/ion_cma_heap.c
 *
 * Copyright (C) Linaro 2012
 * Author: <benjamin.gaignard@linaro.org> for ST-Ericsson.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/device.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/dma-mapping.h>
#include <linux/dma-contiguous.h>
#include <linux/hisi_ion.h>
#include <asm/mach/map.h>
 
#include <linux/ion.h>
#include "ion_priv.h"

#define ION_CMA_ALLOCATE_FAILED -1
//#define DEBUG
struct ion_cma_heap {
	struct ion_heap heap;
	struct device *dev;
};

#define to_cma_heap(x) container_of(x, struct ion_cma_heap, heap)

struct ion_cma_buffer_info {
	void *cpu_addr;
	dma_addr_t handle;
	struct sg_table *table;
};

/*
 * Create scatter-list for the already allocated DMA buffer.
 * This function could be replaced by dma_common_get_sgtable
 * as soon as it will avalaible.
 */
static int ion_cma_get_sgtable(struct device *dev, struct sg_table *sgt,
			       void *cpu_addr, dma_addr_t handle, size_t size)
{
	//struct page *page = virt_to_page(cpu_addr);
	struct page *page = phys_to_page(handle);
	int ret;

	ret = sg_alloc_table(sgt, 1, GFP_KERNEL);
	if (unlikely(ret))
		return ret;

	sg_set_page(sgt->sgl, page, PAGE_ALIGN(size), 0);
	return 0;
}

/* ION CMA heap operations functions */
static int ion_cma_allocate(struct ion_heap *heap, struct ion_buffer *buffer,
			    unsigned long len, unsigned long align,
			    unsigned long flags)
{
	struct ion_cma_heap *cma_heap = to_cma_heap(heap);
	struct device *dev = cma_heap->dev;
	struct ion_cma_buffer_info *info;
	struct page *cma_pages = NULL;

	printk(KERN_ERR "%s: enter\n", __FUNCTION__);
	dev_dbg(dev, "Request buffer allocation len %ld\n", len);

	if (align > PAGE_SIZE)
	{
		pr_err("%s error: return EINVAL!\n", __func__);
		BUG();
		return -EINVAL;
	}
	
	info = kzalloc(sizeof(struct ion_cma_buffer_info), GFP_KERNEL);
	if (!info) {
		dev_err(dev, "Can't allocate buffer info\n");
		return ION_CMA_ALLOCATE_FAILED;
	}

	
		
	cma_pages = dma_alloc_from_contiguous(&gralloc_cma_dev,(PAGE_ALIGN(len)>>PAGE_SHIFT), align);

	if(cma_pages== NULL)
	{
		dev_err(dev, "handle Fail to allocate buffer\n");
		goto err;
	}	
	info->handle = page_to_phys(cma_pages);

	printk(KERN_INFO "%s: pages:0x%08lx phy:0x%08x\n", 
							__FUNCTION__, len>>PAGE_SHIFT, info->handle);	

	mod_cma_state(PAGE_ALIGN(len), NR_USED_ION_GRALLOC_CMA);
	info->table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!info->table) {
		dev_err(dev, "Fail to allocate sg table\n");
		goto free_mem;
	}

	if (ion_cma_get_sgtable
	    (dev, info->table, info->cpu_addr, info->handle, len))
		goto free_table;
	/* keep this for memory release */
	buffer->priv_virt = info;
	dev_dbg(dev, "Allocate buffer %p\n", buffer);
	return 0;

free_table:
	kfree(info->table);
free_mem:
	dma_release_from_contiguous(&gralloc_cma_dev, cma_pages,(PAGE_ALIGN(len)>>PAGE_SHIFT));
	mod_cma_state(-PAGE_ALIGN(len), NR_USED_ION_GRALLOC_CMA);
err:
	kfree(info);
	return ION_CMA_ALLOCATE_FAILED;
}

static void ion_cma_free(struct ion_buffer *buffer)
{
	struct ion_cma_heap *cma_heap = to_cma_heap(buffer->heap);
	struct device *dev = cma_heap->dev;
	struct ion_cma_buffer_info *info = buffer->priv_virt;
	
	dev_dbg(dev, "Release buffer %p\n", buffer);
	printk(KERN_INFO "%s: len:0x%x len_a:0x%x phy:0x%08x\n", 
							__FUNCTION__, buffer->size, PAGE_ALIGN(buffer->size), info->handle);
	/* release memory */
	dma_release_from_contiguous(&gralloc_cma_dev, phys_to_page(info->handle), (PAGE_ALIGN(buffer->size)>>PAGE_SHIFT));
	mod_cma_state(-PAGE_ALIGN(buffer->size), NR_USED_ION_GRALLOC_CMA);
	/* release sg table */
	sg_free_table(info->table);
	kfree(info->table);
	kfree(info);
}

/* return physical address in addr */
static int ion_cma_phys(struct ion_heap *heap, struct ion_buffer *buffer,
			ion_phys_addr_t *addr, size_t *len)
{
	struct ion_cma_heap *cma_heap = to_cma_heap(buffer->heap);
	struct device *dev = cma_heap->dev;
	struct ion_cma_buffer_info *info = buffer->priv_virt;

	dev_dbg(dev, "Return buffer %p physical address 0x%pa\n", buffer,
		&info->handle);
	
	*addr = info->handle;
	*len = buffer->size;

	return 0;
}

static struct sg_table *ion_cma_heap_map_dma(struct ion_heap *heap,
					     struct ion_buffer *buffer)
{
	struct ion_cma_buffer_info *info = buffer->priv_virt;

	return info->table;
}

static void ion_cma_heap_unmap_dma(struct ion_heap *heap,
				   struct ion_buffer *buffer)
{
	return;
}

static int ion_cma_mmap(struct ion_heap *mapper, struct ion_buffer *buffer,
			struct vm_area_struct *vma)
{
	struct ion_cma_buffer_info *info = buffer->priv_virt;

	if((buffer->flags  & ION_FLAG_CACHED) == 0)
	{
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	}

	return remap_pfn_range(vma, vma->vm_start,
				   __phys_to_pfn(info->handle) + vma->vm_pgoff,
				   vma->vm_end - vma->vm_start,
				   vma->vm_page_prot);
}

static void *ion_cma_map_kernel(struct ion_heap *heap,
				struct ion_buffer *buffer)
{
	struct ion_cma_buffer_info *info = buffer->priv_virt;
	
	printk(KERN_ERR "%s: enter\n", __FUNCTION__);
	/* kernel memory mapping has been done at allocation time */

	if (buffer->flags & ION_FLAG_CACHED) {
		return mem_remap(info->handle, buffer->size);
	} else {
		return mem_remap_nocache(info->handle, buffer->size);;
	}
}

static void ion_cma_unmap_kernel(struct ion_heap *heap,
					struct ion_buffer *buffer)
{
	printk(KERN_ERR "%s: enter\n", __FUNCTION__);
	mem_unmap(buffer->vaddr);
	buffer->vaddr = NULL;
	return;
}

static struct ion_heap_ops ion_cma_ops = {
	.allocate = ion_cma_allocate,
	.free = ion_cma_free,
	.map_dma = ion_cma_heap_map_dma,
	.unmap_dma = ion_cma_heap_unmap_dma,
	.phys = ion_cma_phys,
	.map_user = ion_cma_mmap,
	.map_kernel = ion_cma_map_kernel,
	.unmap_kernel = ion_cma_unmap_kernel,
};

struct ion_heap *ion_cma_heap_create(struct ion_platform_heap *data)
{
	struct ion_cma_heap *cma_heap;
	
	cma_heap = kzalloc(sizeof(struct ion_cma_heap), GFP_KERNEL);

	if (!cma_heap)
		return ERR_PTR(-ENOMEM);

	cma_heap->heap.ops = &ion_cma_ops;
	/* get device from private heaps data, later it will be
	 * used to make the link with reserved CMA memory */
	//cma_heap->dev = data->priv;
	cma_heap->dev = NULL;
	cma_heap->heap.type = ION_HEAP_TYPE_DMA;
	return &cma_heap->heap;
}

void ion_cma_heap_destroy(struct ion_heap *heap)
{
	struct ion_cma_heap *cma_heap = to_cma_heap(heap);

	kfree(cma_heap);
}
