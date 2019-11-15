cmd_arch/arm/mm/cache-v7.o := /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mm/.cache-v7.o.d  -nostdinc -isystem /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/define/hi6620cs_oem_gutl_dsda/config -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/nvim/include/gu -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/common/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/include/hisi -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/mm -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/taf -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv/MODULE_H -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/med -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/nv/gu/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/platform/balong -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/common -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/linux -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/nvim -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/keymaster/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/codesafe/src/secure_boot/boot_images_verifier -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/host/src/sbromlib -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../vendor/hisi/include/taf/ -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi6620/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float -gdwarf-2     -Wa,-march=armv7-a   -DBSP_CORE_APP -DBSP_COMPILE_ALLY -DBOARD_ASIC -c -o arch/arm/mm/cache-v7.o arch/arm/mm/cache-v7.S

source_arch/arm/mm/cache-v7.o := arch/arm/mm/cache-v7.S

deps_arch/arm/mm/cache-v7.o := \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/arm/errata/764369.h) \
    $(wildcard include/config/arm/errata/775420.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler_types.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/linkage.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/hwcap.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/unwind.h \
    $(wildcard include/config/arm/unwind.h) \
  arch/arm/mm/proc-macros.S \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \

arch/arm/mm/cache-v7.o: $(deps_arch/arm/mm/cache-v7.o)

$(deps_arch/arm/mm/cache-v7.o):
