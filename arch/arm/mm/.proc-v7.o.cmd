cmd_arch/arm/mm/proc-v7.o := /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mm/.proc-v7.o.d  -nostdinc -isystem /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/define/hi6620cs_oem_gutl_dsda/config -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/nvim/include/gu -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/common/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/include/hisi -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/mm -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/taf -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv/MODULE_H -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/med -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/nv/gu/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/platform/balong -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/common -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/linux -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/nvim -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/keymaster/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/codesafe/src/secure_boot/boot_images_verifier -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/host/src/sbromlib -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../vendor/hisi/include/taf/ -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi6620/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float -gdwarf-2     -Wa,-march=armv7-a   -DBSP_CORE_APP -DBSP_COMPILE_ALLY -DBOARD_ASIC -c -o arch/arm/mm/proc-v7.o arch/arm/mm/proc-v7.S

source_arch/arm/mm/proc-v7.o := arch/arm/mm/proc-v7.S

deps_arch/arm/mm/proc-v7.o := \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/arm/errata/430973.h) \
    $(wildcard include/config/arm/errata/754322.h) \
    $(wildcard include/config/cpu/use/domains.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/arm/errata/458693.h) \
    $(wildcard include/config/arm/errata/460075.h) \
    $(wildcard include/config/arm/errata/742230.h) \
    $(wildcard include/config/arm/errata/742231.h) \
    $(wildcard include/config/arm/errata/743622.h) \
    $(wildcard include/config/arm/errata/751472.h) \
    $(wildcard include/config/arm/thumbee.h) \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/swp/emulate.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler_types.h \
  include/linux/linkage.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/linkage.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/hwcap.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/pgtable-hwdef.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/pgtable.h \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/highpte.h) \
  include/linux/const.h \
  include/asm-generic/4level-fixup.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/proc-fns.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/glue-proc.h \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm7tdmi.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm740t.h) \
    $(wildcard include/config/cpu/arm9tdmi.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm940t.h) \
    $(wildcard include/config/cpu/arm946e.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/mohawk.h) \
    $(wildcard include/config/cpu/v6.h) \
    $(wildcard include/config/cpu/v6k.h) \
    $(wildcard include/config/cpu/v7.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/glue.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  include/asm-generic/getorder.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/arm/patch/phys/virt/16bit.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  arch/arm/mach-hi6620/include/mach/memory.h \
  arch/arm/mach-hi6620/include/mach/ddr_map.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda/global_ddr_map.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic/soc_baseaddr_interface.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/sizes.h \
  include/asm-generic/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  arch/arm/mach-hi6620/include/mach/vmalloc.h \
  arch/arm/mm/proc-macros.S \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/thread_info.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \

arch/arm/mm/proc-v7.o: $(deps_arch/arm/mm/proc-v7.o)

$(deps_arch/arm/mm/proc-v7.o):
