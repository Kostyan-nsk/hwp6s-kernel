cmd_arch/arm/mach-hi6620/headsmp.o := /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mach-hi6620/.headsmp.o.d  -nostdinc -isystem /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/define/hi6620cs_oem_gutl_dsda/config -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/nvim/include/gu -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/common/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/include/hisi -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/mm -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/taf -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv/MODULE_H -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/med -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/nv/gu/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/platform/balong -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/common -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/linux -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/nvim -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/keymaster/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/codesafe/src/secure_boot/boot_images_verifier -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/host/src/sbromlib -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../vendor/hisi/include/taf/ -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi6620/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork  -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float -gdwarf-2        -DBSP_CORE_APP -DBSP_COMPILE_ALLY -DBOARD_ASIC -c -o arch/arm/mach-hi6620/headsmp.o arch/arm/mach-hi6620/headsmp.S

source_arch/arm/mach-hi6620/headsmp.o := arch/arm/mach-hi6620/headsmp.S

deps_arch/arm/mach-hi6620/headsmp.o := \
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
  arch/arm/mach-hi6620/include/mach/pwrctrl/pwrctrl_sleepasm.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic/soc_baseaddr_interface.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc/pwrctrl_multi_memcfg.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc/pwrctrl_multi_dfs_asm.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda/global_ddr_map.h \

arch/arm/mach-hi6620/headsmp.o: $(deps_arch/arm/mach-hi6620/headsmp.o)

$(deps_arch/arm/mach-hi6620/headsmp.o):
