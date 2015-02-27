/******************************************************************************
修改以下文件需要保证下面文件的一致性：
hardware\libhardware\include\hardware\hisi\pwrctrl_multi_memcfg.h       ->vendor/hisi/modem/drv/hi6620/src/comm/inc/pwrctrl_multi_memcfg.h
hardware\libhardware\include\hardware\hisi\pwrctrl_multi_dfs_asm.h      ->vendor/hisi/modem/drv/hi6620/src/comm/inc/pwrctrl_multi_dfs_asm.h
hardware\libhardware\include\hardware\hisi\soc_baseaddr_interface.h     ->vendor/hisi/platform/hi6620cs_asic/soc_baseaddr_interface.h
hardware\libhardware\include\hardware\hisi\global_ddr_map.h             ->vendor/hisi/config/product/include/hi6620cs_oem_gutl_dsda/global_ddr_map.h
******************************************************************************/
#ifndef _HI_GLOBAL_MEM_MAP_INCLUDE_H_
#define _HI_GLOBAL_MEM_MAP_INCLUDE_H_


#include "soc_baseaddr_interface.h"

/*******************************************
多核共享头文件
不允许包含操作系统相关头文件
*******************************************/

/******************************************
V900R001 Dual-Channel DDR Details:
*******************************************/

#define DDR_BASE_ADDR					(SOC_DRAM_BASE_ADDR)
#define DDR_LAST_ADDR					(0x7FFFFFFF)

/******************************************************
         以下内存在DDR非交织区
********************************************************/
/******************************************************
         DDR非交织区 上半区 128M
********************************************************/
/*在这里添加新的内存区域后，需要在 ..\kernel\arch\arm\mach-hi6620\init_param.c
文件里面添加对应的检测项目-start*/
/*Modem系统内存 89M*/
#define MODEM_SYS_MEM_ADDR				(DDR_BASE_ADDR)                                 /*0x00000000*/
#define MODEM_SYS_MEM_SIZE				(0x04D00000)


/* when ADDR changed,please check the vendor\hisi\build\scripts\gen_img_vrl_tbl.py */

/*20130702,l00212112
请注意:
这段内存是从MODEM_SYS_MEM_SIZE 中最后39M划出来，在v9的c核单独复位功能中被用于加载
modem bin文件的临时存储区域，所以如果修改这段空间大小，
并且修改成小于modem bin的size的值，或者这段空间有新的用途，请同步修改c核单独复位!!!!*/
#define MODEM_SYS_MEM_RUN_AREA_SIZE				(0x02800000)    /*从这个大小的空间之后加载C镜像*/
#define MODEM_RESET_LOAD_CBIN_ADDR				(MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_RUN_AREA_SIZE)                /*0x02800000*/
#define MODEM_RESET_LOAD_CBIN_SIZE				(0x02500000)    /*暂存C镜像空间最大值，37M*/
#define MODEM_RESET_LOAD_CBIN_BOUNDARY          (MODEM_RESET_LOAD_CBIN_ADDR)/*最终加载镜像空间不能超过此值*/
/*请注意:
1.这段空间是给modem压缩镜像机制用的，是压缩镜像的搬运地址。
2.20140320定义生效，空间范围0x3C00000-0x4D00000，要求17M
3.在开机和C核单独复位过程中用到.
4.该值和gen_img_vrl_tbl.py中modem的加载地址对应，如果修改，要同时修改*/
#define MODEM_ZIP_LOAD_ADDR                     0x3BFFFF0
/*该地址是解压函数的入口地址，如果压缩镜像有头，则应该改为(MODEM_ZIP_LOAD_ADDR + 镜像头大小)*/
#define MODEM_UNZIP_RUN_ADDR                     0x3C00000

#define MODEM_ZIP_SIZE      ((MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_SIZE) - MODEM_ZIP_LOAD_ADDR)
/*请注意:
这个地址是modem镜像的加载地址。
*/
#define MODEM_LOAD_DEST_ADDR              0x3E00

/*HIFI系统内存 8M*/
#define HIFI_SYS_MEM_ADDR				(MODEM_SYS_MEM_ADDR + MODEM_SYS_MEM_SIZE)       /*0x04D00000*/
#define HIFI_SYS_MEM_SIZE				(0x00400000)

/*MCU系统内存 1M*/
#define MCU_SYS_MEM_ADDR			    (HIFI_SYS_MEM_ADDR + HIFI_SYS_MEM_SIZE)         /*0x05100000*/
#define MCU_SYS_MEM_SIZE				(0x00100000)

/*C核给GUPS预留的内存 13M*/
#define GU_RESERVED_MEM_ADDR		    (MCU_SYS_MEM_ADDR + MCU_SYS_MEM_SIZE)           /*0x05200000*/
#define GU_RESERVED_MEM_SIZE		    (0x00D00000)

/*C核给LPS预留的内存8M,若地址发生改变偏移，需要同步修改BBE16代码*/
#define LPS_RESERVED_MEM_ADDR		    (GU_RESERVED_MEM_ADDR + GU_RESERVED_MEM_SIZE)   /*0x05F00000*/
#define LPS_RESERVED_MEM_SIZE		    (0x00600000)

/*核间共享内存4M*/
#define IPC_SHARE_MEM_ADDR				(LPS_RESERVED_MEM_ADDR + LPS_RESERVED_MEM_SIZE) /*0x06500000*/
#define IPC_SHARE_MEM_SIZE				(0x00400000)

/*Modem异常信息记录2M */
#define MODEM_DUMP_LOG_ADDR				(IPC_SHARE_MEM_ADDR + IPC_SHARE_MEM_SIZE)       /*0x06900000*/
#define MODEM_DUMP_LOG_SIZE				(0x00100000)

/*MCU系统内存 1M */
#define MCU_SHARE_MEM_ADDR				(MODEM_DUMP_LOG_ADDR + MODEM_DUMP_LOG_SIZE)     /*0x06A00000*/
#define MCU_SHARE_MEM_SIZE				(0x00100000)
/******************************************************
         DDR非交织区 下半区128M
********************************************************/

/*AP异常信息记录2M */
#define ANDROID_DUMP_LOG_ADDR			(MCU_SHARE_MEM_ADDR + MCU_SHARE_MEM_SIZE)       /*0x06BS00000*/
#define ANDROID_DUMP_LOG_SIZE			(0x00100000)

/*FASTBOOT 系统内存4M*/
#define FASTBOOT_SYS_MEM_ADDR			(ANDROID_DUMP_LOG_ADDR + ANDROID_DUMP_LOG_SIZE) /*0x06C00000*/
#define FASTBOOT_SYS_MEM_SIZE			(0x00400000)

/*Android 系统内存两部分共886M 为了A核Kernel使用方便 */
#define ANDROID_SYS_MEM_ADDR			(FASTBOOT_SYS_MEM_ADDR + FASTBOOT_SYS_MEM_SIZE) /*0x07000000*/
#define ANDROID_SYS_MEM_SIZE			(DDR_LAST_ADDR - ANDROID_SYS_MEM_ADDR + 1)

#if 0
/*FASTBOOT 系统内存32M 使用1G/2G最后32M空间 */
#define FASTBOOT_SYS_MEM_ADDR			(ANDROID_SYS_MEM_ADDR + ANDROID_SYS_MEM_SIZE - 0x02000000)
#define FASTBOOT_SYS_MEM_SIZE			(0x02000000)
#endif

#define SYS_DUMP_MEM_ADDR                (0x40000000)
#define SYS_DUMP_MEM_SIZE                (0x20000000)

/*在这里添加新的内存区域后，需要在 ..\kernel\arch\arm\mach-hi6620\init_param.c
文件里面添加对应的检测项目 end*/

/******************************************************
  以下是需要预留的虚地址区64MB(0xF0000000~0xF4000000)
********************************************************/
#define GLOBAL_VIRT_ADDR                (0xF0000000)
#define GLOBAL_VIRT_SIZE                (0x04000000)

/*C核给GUPS预留的内存虚地址*/
#define GU_RESERVED_MEM_VIRT_ADDR       (GLOBAL_VIRT_ADDR)                                  /*0xF0000000*/

/*C核给LPS预留的内存虚地址*/
#define LPS_RESERVED_MEM_VIRT_ADDR      (GU_RESERVED_MEM_VIRT_ADDR + GU_RESERVED_MEM_SIZE)  /*0xF1100000*/

/*核间共享内存虚地址*/
#define IPC_SHARE_MEM_VIRT_ADDR         (LPS_RESERVED_MEM_VIRT_ADDR + LPS_RESERVED_MEM_SIZE)/*0xF1700000*/

/*Modem异常信息记录虚地址 */
#define MODEM_DUMP_LOG_VIRT_ADDR        (IPC_SHARE_MEM_VIRT_ADDR + IPC_SHARE_MEM_SIZE)      /*0xF1B00000*/

/*温保核间共享内存虚地址*/
#define MCU_SHARE_MEM_VIRT_ADDR         (MODEM_DUMP_LOG_VIRT_ADDR + MODEM_DUMP_LOG_SIZE)    /*0xF1D00000*/





/*MCU 异常处理dump区*/
#define MCU_DUMP_LOG_ADDR				(MCU_SHARE_MEM_ADDR)
#define MCU_DUMP_LOG_SIZE               (0x000E0000)

/*mcu mntn使用共享区*/
#define MCU_MNTN_DDR_ADDR               (MCU_DUMP_LOG_ADDR + MCU_DUMP_LOG_SIZE)
#define MCU_MNTN_DDR_SIZE               (0x00008000)

/*MCU 系统温保共享内存区 MCU_HKADC_DDR_ADDR*/
#define MCU_HKADC_DDR_ADDR              (MCU_MNTN_DDR_ADDR + MCU_MNTN_DDR_SIZE)
#define MCU_HKADC_DDR_SIZE              (0x0004000)

#define MCU_DDR_LOAD_DDR_ADDR              (MCU_HKADC_DDR_ADDR + MCU_HKADC_DDR_SIZE)
#define MCU_DDR_LOAD_DDR_SIZE              (0x00010000)

#define MCU_SHARE_MEM_USED_SIZE         (MCU_DUMP_LOG_SIZE+MCU_MNTN_DDR_SIZE+MCU_HKADC_DDR_SIZE+MCU_DDR_LOAD_DDR_SIZE)


#if (MCU_SHARE_MEM_USED_SIZE > MCU_SHARE_MEM_SIZE)
#error mcu_share_memory(MCU_SHARE_MEM_USED_SIZE) used beyond (MCU_SHARE_MEM_SIZE)
#endif
#endif /*_HI_GLOBAL_MEM_MAP_INCLUDE_H_*/
