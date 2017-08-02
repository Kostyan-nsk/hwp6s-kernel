/*
 * drivers/gpu/exynos/exynos_ion.c
 *
 * Copyright (C) 2011 Samsung Electronics Co., Ltd.
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

#include <linux/err.h>
//#include <linux/ion.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
//#include <linux/cma.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/bitops.h>
#include <linux/pagemap.h>
#include <linux/dma-mapping.h>

#include <asm/pgtable.h>
#include <linux/hisi_ion.h>
#include "../ion_priv.h"

struct ion_device *k3v2_ion_device;

static int num_heaps;
static struct ion_heap **heaps;
static struct device *k3v2_ion_dev;

static int check_vaddr_bounds(unsigned long start, unsigned long end)
{
	struct mm_struct *mm = current->active_mm;
	struct vm_area_struct *vma;
	int ret = 1;

	if(end < start)
		goto out;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, start);
	if(vma && vma->vm_start < end){
		if(start < vma->vm_start)
			goto out_up;
		if(end > vma->vm_end)
			goto out_up;
		ret = 0;
	}
	out_up:
		up_read(&mm->mmap_sem);
	out:
		return ret;
}

static long k3v2_heap_ioctl(struct ion_client *client, unsigned int cmd,
				unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case ION_HISI_CUSTOM_PHYS:
	{
		struct ion_phys_data data;
		struct ion_handle *handle;

		if (copy_from_user(&data, (void __user *)arg,
				sizeof(data))) {
			return -EFAULT;
		}

		handle = ion_import_dma_buf(client, data.fd_buffer);

		if (IS_ERR(handle)) {
			pr_err("%s %d : handle 0x%p phy 0x%lx size 0x%x fd 0x%x\n", __FUNCTION__,
				__LINE__, handle, data.phys, data.size, data.fd_buffer);
			return PTR_ERR(handle);
		}
		ret = ion_phys(client, handle, &data.phys, &data.size);
		if (ret)
		{
			ion_free(client, handle);
			return ret;
        }
		if (copy_to_user((void __user *)arg, &data, sizeof(data)))
		{
			ion_free(client, handle);
			return -EFAULT;
		}
		ion_free(client, handle);

		break;
	}
        case ION_IOC_FLUSH_ALL_CACHES:
        {
            struct ion_flush_data data;
            unsigned long long start, end;
            if(copy_from_user(&data, (void __user*)arg,
                            sizeof(struct ion_flush_data)))
            return -EFAULT;

             start = (unsigned long ) data.vaddr;
             end = (unsigned long ) data.vaddr + data.length;

            if(check_vaddr_bounds(start, end)){
                pr_err("%s: virtual address %p is out of bounds\n",
                                    __func__, data.vaddr);
                return -EINVAL;
            }

            /*flush L1 cache */
            flush_all_cpu_caches();
            /*flush L2 cache*/
            outer_flush_all();
        }
        break;
	default:
		return -ENOTTY;
	}

	return ret;
}

static int k3v2_ion_probe(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;

	heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);
	if (!heaps)
		return -ENOMEM;

	k3v2_ion_device = ion_device_create(&k3v2_heap_ioctl);
	if (IS_ERR_OR_NULL(k3v2_ion_device)) {
		kfree(heaps);
		return PTR_ERR(k3v2_ion_device);
	}

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			err = PTR_ERR(heaps[i]);
			goto err;
		}
		ion_device_add_heap(k3v2_ion_device, heaps[i]);
		pr_info("%s: adding heap %s of type %d with %lx@%x\n",
            __func__, heap_data->name, heap_data->type,
            heap_data->base, heap_data->size);
	}
	platform_set_drvdata(pdev, k3v2_ion_device);

	k3v2_ion_dev = &pdev->dev;
    printk("k3v2_ion_probe success");
	return 0;
err:
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}
	kfree(heaps);
	return err;
}

static int k3v2_ion_remove(struct platform_device *pdev)
{
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	ion_device_destroy(idev);
	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);
	kfree(heaps);
	return 0;
}

static struct platform_driver ion_driver = {
	.probe = k3v2_ion_probe,
	.remove = k3v2_ion_remove,
	.driver = { .name = "ion-k3v2" }
};

static int __init ion_init(void)
{
    printk("============ion_init\n");
    return platform_driver_register(&ion_driver);
}

static void __exit ion_exit(void)
{
    platform_driver_unregister(&ion_driver);
}

module_init(ion_init);
module_exit(ion_exit);
