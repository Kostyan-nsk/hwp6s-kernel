cmd_arch/arm/mach-hi6620/fastchg.o := /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mach-hi6620/.fastchg.o.d  -nostdinc -isystem /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/../lib/gcc/arm-eabi/4.8/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/product/define/hi6620cs_oem_gutl_dsda/config -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/config/nvim/include/gu -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/platform/hi6620cs_asic -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/hi6620/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/modem/drv/common/src/comm/inc -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/include/hisi -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/mm -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/taf -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/drv/MODULE_H -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/med -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/vendor/hisi/include/nv/gu/drv -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/platform/balong -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/common -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/linux -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/gpu/mali/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/nvim -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/modem/include/external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/drivers/hisi/keymaster/include -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/efipartition -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/codesafe/src/secure_boot/boot_images_verifier -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../external/dx/host/src/sbromlib -I/home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/../vendor/hisi/include/taf/ -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi6620/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -DBSP_CORE_APP -DBSP_COMPILE_ALLY -DBOARD_ASIC -O2 -mcpu=cortex-a9 -mtune=cortex-a9 -marm -fno-dwarf2-cfi-asm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -DBSP_CORE_APP -DBSP_COMPILE_ALLY -DBOARD_ASIC -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(fastchg)"  -D"KBUILD_MODNAME=KBUILD_STR(fastchg)" -c -o arch/arm/mach-hi6620/fastchg.o arch/arm/mach-hi6620/fastchg.c

source_arch/arm/mach-hi6620/fastchg.o := arch/arm/mach-hi6620/fastchg.c

deps_arch/arm/mach-hi6620/fastchg.o := \
  include/linux/kobject.h \
    $(wildcard include/config/hotplug.h) \
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
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler_types.h \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/smp.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/posix_types.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  include/linux/sysfs.h \
    $(wildcard include/config/debug/lock/alloc.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/errno.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/kobject_ns.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/system.h \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/linux/linkage.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/linkage.h \
  include/linux/irqflags.h \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/irqflags.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/hwcap.h \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/atomic-long.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  include/linux/bitops.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/bitops.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/sunniva/dev/huawei-p7/los13.1-src/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/lib/gcc/arm-eabi/4.8/include/stdarg.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
  include/linux/dynamic_debug.h \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/spinlock_types.h \
  include/linux/rwlock_types.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/spinlock.h \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/arm/errata/754327.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/hw_breakpoint.h \
  include/asm-generic/processor.h \
  include/asm-generic/relaxed.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/kref.h \
  include/linux/wait.h \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/current.h \
  include/linux/fastchg.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/sunniva/dev/huawei-p7/threader/git/hwp6s-kernel/arch/arm/include/asm/string.h \

arch/arm/mach-hi6620/fastchg.o: $(deps_arch/arm/mach-hi6620/fastchg.o)

$(deps_arch/arm/mach-hi6620/fastchg.o):
