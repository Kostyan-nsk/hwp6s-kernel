#Android makefile to build kernel as a part of Android Build
# KERNEL_TOOLCHAIN_PREFIX overrides CROSS_COMPLE in 13.1
# And you cannot set it here...
# KERNEL_TOOLCHAIN_PREFIX= arm-eabi- #$(CROSS_COMPILE)

#ifeq ($(TARGET_PREBUILT_KERNEL),)

ifeq ($(TARGET_DEVICE), p6_u06)
OBB_PRODUCT_NAME := hi6620cs_sft
KERNEL_GEN_CONFIG_FILE := hisi_hi6620vpa_defconfig
endif
ifeq ($(TARGET_DEVICE), p7_l10)	
OBB_PRODUCT_NAME := hi6620cs_oem_gutl_dsda
KERNEL_GEN_CONFIG_FILE := hisi_hi6620oem_defconfig 
endif


TARGET_KERNEL_ARCH := $(strip $(TARGET_KERNEL_ARCH))
ifeq ($(TARGET_KERNEL_ARCH),)
KERNEL_ARCH := arm
else
KERNEL_ARCH := $(TARGET_KERNEL_ARCH)
endif

TARGET_KERNEL_HEADER_ARCH := $(strip $(TARGET_KERNEL_HEADER_ARCH))
ifeq ($(TARGET_KERNEL_HEADER_ARCH),)
KERNEL_HEADER_ARCH := $(KERNEL_ARCH)
else
$(warning Forcing kernel header generation only for '$(TARGET_KERNEL_HEADER_ARCH)')
KERNEL_HEADER_ARCH := $(TARGET_KERNEL_HEADER_ARCH)
endif

ifeq ($(OBB_PRINT_CMD), true)
KERNEL_OUT := vendor/hisi/build/delivery/$(OBB_PRODUCT_NAME)/obj/android
else
KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
endif
KERNEL_CONFIG := $(KERNEL_OUT)/.config
TARGET_PREBUILT_KERNEL := $(KERNEL_OUT)/arch/arm/boot/zImage

#include $(BALONG_TOPDIR)/build/scripts/make_base.mk



ifneq ($(COMMON_HEAD),)
BALONG_INC := $(patsubst %,-I%,$(COMMON_HEAD))
else
BALONG_INC := 
endif
 
export BALONG_INC
export TARGET_BOARD_PLATFORM

KERNEL_ARCH_ARM_CONFIGS := $(shell pwd)/kernel/hwp6s-kernel/arch/arm/configs

KERNEL_GEN_CONFIG_PATH := $(KERNEL_ARCH_ARM_CONFIGS)/$(KERNEL_GEN_CONFIG_FILE)

KERNEL_COMMON_DEFCONFIG := $(KERNEL_ARCH_ARM_CONFIGS)/$(KERNEL_DEFCONFIG)

GPIO_CONFIG_TARGET := $(KERNEL_OUT)/gpio_config

.PHONY: $(GPIO_CONFIG_TARGET)
$(GPIO_CONFIG_TARGET) : FORCE

ifeq ($(OBB_PRINT_CMD), true)
$(KERNEL_CONFIG): MAKEFLAGS := 
endif
$(KERNEL_CONFIG): $(KERNEL_GEN_CONFIG_PATH)
	+mkdir -p $(KERNEL_OUT)
	$(MAKE) -C kernel O=../$(KERNEL_OUT) ARCH=arm CROSS_COMPILE=/linaro-gcc-4.9-abe-gnueabbihf/bin/arm-linux-gnueabihf- $(KERNEL_GEN_CONFIG_FILE)
	@rm -frv $(KERNEL_GEN_CONFIG_PATH)

$(TARGET_PREBUILT_KERNEL): $(GPIO_CONFIG_TARGET) $(KERNEL_CONFIG)
ifeq ($(OBB_PRINT_CMD), true)
	$(hide) $(MAKE) -C kernel O=../$(KERNEL_OUT) ARCH=arm CROSS_COMPILE=/linaro-gcc-4.9-abe-gnueabbihf/bin/arm-linux-gnueabihf- vmlinux
	touch $@
else
	$(hide) $(MAKE) -C kernel O=../$(KERNEL_OUT) ARCH=arm CROSS_COMPILE=/linaro-gcc-4.9-abe-gnueabihf/bin/arm-linux-gnueabihf- # arm-linux-androideabi-
endif

KERNEL_HEADERS_INSTALL := $(KERNEL_OUT)/usr
KERNEL_MODULES_INSTALL := system
KERNEL_MODULES_OUT := $(TARGET_OUT)/lib/modules


kernelconfig: $(KERNEL_GEN_CONFIG_PATH)
	mkdir -p $(KERNEL_OUT)
	$(MAKE) -C kernel O=../$(KERNEL_OUT) ARCH=arm CROSS_COMPILE=/linaro-gcc-4.9-abe-gnueabihf/bin/arm-linux-gnueabihf- $(KERNEL_GEN_CONFIG_FILE) menuconfig
	@rm -frv $(KERNEL_GEN_CONFIG_PATH)

zImage:$(TARGET_PREBUILT_KERNEL)
	@mkdir -p $(dir $(INSTALLED_KERNEL_TARGET))
	@cp -fp $(TARGET_PREBUILT_KERNEL) $(INSTALLED_KERNEL_TARGET)

pclint_kernel: $(KERNEL_CONFIG)
	$(hide) $(MAKE) -C kernel O=../$(KERNEL_OUT) ARCH=arm CROSS_COMPILE=/linaro-gcc-4.9-abe-gnueabihf/bin/arm-linux-gnueabihf- pc_lint_all

