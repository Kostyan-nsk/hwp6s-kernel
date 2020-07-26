
#ifndef _GENERAL_SRAM_MAP_INCLUDE_H_
#define _GENERAL_SRAM_MAP_INCLUDE_H_

#include "global_sram_map.h"

/*-------------------------------------------------------------------------------
                            SRAM �����־������
  -------------------------------------------------------------------------------
 
                          -----------------------
                         |  A��������־          |->64B
                          -----------------------
                         |  DICC��־             |->32B
                          -----------------------
                         |  ���־               |->256B
                          -----------------------
                         |  �±���               |->128B
                          -----------------------                                                   
                         |  CSHELL               |->4B
                          -----------------------
                         |  �������л�         |->4B
                          -----------------------
                         |  ICC                  |->1024B
                          -----------------------
                         |  �ڴ����             |->1024B
                          -----------------------                       
                         |  IFC                  |->32B
                          -----------------------
                         |  HIFI                 |->32B
                          -----------------------
                         |  DDR capacity         |->4B
                          -----------------------
                         |  ����                 |
                          -----------------------
                         
-------------------------------------------------------------------------------*/

#define GLOBAL_AXI_MEM_BASE_ADDR                (GENERAL_SRAM_ADDR)
#define GLOBAL_AXI_MEM_SPEC_SIZE                (GENERAL_SRAM_SIZE)

/* A��������־ */
#define MEMORY_AXI_ACPU_START_ADDR              (GLOBAL_AXI_MEM_BASE_ADDR)
#define MEMORY_AXI_ACPU_START_SIZE              (64)

/* DICC��ʶ */
#define MEMORY_AXI_DICC_ADDR                    (MEMORY_AXI_ACPU_START_ADDR + MEMORY_AXI_ACPU_START_SIZE)
#define MEMORY_AXI_DICC_SIZE                    (32)

#define MEMORY_AXI_SOFT_FLAG_ADDR               (MEMORY_AXI_DICC_ADDR + MEMORY_AXI_DICC_SIZE)
#define MEMORY_AXI_SOFT_FLAG_SIZE               (256)

/* �±��� */
#define MEMORY_AXI_TEMP_PROTECT_ADDR            (MEMORY_AXI_SOFT_FLAG_ADDR + MEMORY_AXI_SOFT_FLAG_SIZE)
#define MEMORY_AXI_TEMP_PROTECT_SIZE            (128)

/*CSHELL*/
#define MEMORY_AXI_USB_CSHELL_ADDR              (MEMORY_AXI_TEMP_PROTECT_ADDR + MEMORY_AXI_TEMP_PROTECT_SIZE)
#define MEMORY_AXI_USB_CSHELL_SIZE              (4)

/* ��������A/C shell���л� */
#define MEMORY_AXI_UART_INOUT_ADDR              (MEMORY_AXI_USB_CSHELL_ADDR + MEMORY_AXI_USB_CSHELL_SIZE)
#define MEMORY_AXI_UART_INOUT_SIZE              (4)

/* ICC��ʶ */
#define MEMORY_AXI_ICC_ADDR                     (MEMORY_AXI_UART_INOUT_ADDR + MEMORY_AXI_UART_INOUT_SIZE)
#define MEMORY_AXI_ICC_SIZE                     (1024+256)

/* �ڴ���� */
#define MEMORY_AXI_MEMMGR_FLAG_ADDR             (MEMORY_AXI_ICC_ADDR + MEMORY_AXI_ICC_SIZE)
#define MEMORY_AXI_MEMMGR_FLAG_SIZE             (1024)

/* IFC��ʶ */
#define MEMORY_AXI_IFC_ADDR                     (MEMORY_AXI_MEMMGR_FLAG_ADDR + MEMORY_AXI_MEMMGR_FLAG_SIZE)
#define MEMORY_AXI_IFC_SIZE                     (32)

/* HIFI������ */
#define MEMORY_AXI_HIFI_ADDR                    (MEMORY_AXI_IFC_ADDR + MEMORY_AXI_IFC_SIZE)
#define MEMORY_AXI_HIFI_SIZE                    (32)

/* CONFIG FLAG */
#define MEMORY_AXI_CONFIG_ADDR                  (MEMORY_AXI_HIFI_ADDR + MEMORY_AXI_HIFI_SIZE)
#define MEMORY_AXI_CONFIG_SIZE                  (32)

/* DDR capacity ��ʶ */
#define MEMORY_AXI_DDR_CAPACITY_ADDR            (MEMORY_AXI_CONFIG_ADDR + MEMORY_AXI_CONFIG_SIZE)
#define MEMORY_AXI_DDR_CAPACITY_SIZE   			(4)

/* USB_SHELL��־��*/
#define MEMORY_AXI_USB_SHELL_FLAG_ADDR            (MEMORY_AXI_DDR_CAPACITY_ADDR + MEMORY_AXI_DDR_CAPACITY_SIZE )
#define MEMORY_AXI_USB_SHELL_FLAG_SIZE              (4)

/*the switch flag of MCU WDT */
#define MEMORY_AXI_MCU_WDT_FLAG_ADDR            (MEMORY_AXI_USB_SHELL_FLAG_ADDR + MEMORY_AXI_USB_SHELL_FLAG_SIZE)
#define MEMORY_AXI_MCU_WDT_FLAG_SIZE             (4)

/*double backup the flag of reset*/
#define MEMORY_AXI_SRESET_FLAG_ADDR2            (MEMORY_AXI_MCU_WDT_FLAG_ADDR + MEMORY_AXI_MCU_WDT_FLAG_SIZE)
#define MEMORY_AXI_SRESET_FLAG_SIZE2            (4)

/* ���� */
#define MEMORY_AXI_RESERVE_ADDR                 (MEMORY_AXI_SRESET_FLAG_ADDR2 + MEMORY_AXI_SRESET_FLAG_SIZE2)
#define MEMORY_AXI_RESERVE_SIZE                 ((GLOBAL_AXI_MEM_BASE_ADDR + GLOBAL_AXI_MEM_SPEC_SIZE) - (MEMORY_AXI_RESERVE_ADDR+MEMORY_AXI_BBE_TICK_DEBUG_SIZE+MEMORY_AXI_MAILBOX_TICK_DEBUG_SIZE))

/*bbe16 ��� debug*/
#define MEMORY_AXI_BBE_TICK_DEBUG_ADDR           (MEMORY_AXI_MAILBOX_TICK_DEBUG_ADDR - MEMORY_AXI_BBE_TICK_DEBUG_SIZE)
#define MEMORY_AXI_BBE_TICK_DEBUG_SIZE           (24) 
/*mailbox ��� debug*/
#define MEMORY_AXI_MAILBOX_TICK_DEBUG_ADDR       (MEMORY_AXI_END_ADDR - MEMORY_AXI_MAILBOX_TICK_DEBUG_SIZE)
#define MEMORY_AXI_MAILBOX_TICK_DEBUG_SIZE       (40) 

#define MEMORY_AXI_END_ADDR                     (MEMORY_AXI_RESERVE_ADDR + MEMORY_AXI_RESERVE_SIZE + MEMORY_AXI_BBE_TICK_DEBUG_SIZE + MEMORY_AXI_MAILBOX_TICK_DEBUG_SIZE)


/*Memory size check*/
#if (MEMORY_AXI_END_ADDR != (GLOBAL_AXI_MEM_BASE_ADDR + GLOBAL_AXI_MEM_SPEC_SIZE))
#error AXI(MEMORY_AXI_END_ADDR) USED beyond the mark (GLOBAL_AXI_MEM_BASE_ADDR + GLOBAL_AXI_MEM_SPEC_SIZE)
#endif

/*Memory size check*/
#if (MEMORY_AXI_END_ADDR > (GLOBAL_AXI_MEM_BASE_ADDR + GLOBAL_AXI_MEM_SPEC_SIZE))
#error AXI(MEMORY_AXI_END_ADDR) USED beyond the mark (GLOBAL_AXI_MEM_BASE_ADDR + GLOBAL_AXI_MEM_SPEC_SIZE)
#endif
/*ICC ��̬�ڴ��� */
/*#define MEMORY_AXI_DYNAMIC_SEC_ADDR         (ICC_SRAM_ADDR)
 */
/*#define MEMORY_AXI_DYNAMIC_SEC_SIZE         (ICC_SRAM_SIZE)
 */

/*������A��������־�ľ������*/

/*�ȸ�λ��־*/
#define MEMORY_AXI_SRESET_FLAG_ADDR         (MEMORY_AXI_ACPU_START_ADDR + 0x0000)
#define MEMORY_AXI_SRESET_FLAG_SIZE         (4)

/*kernel�д�CPU��������ַ*/
#define MEMORY_AXI_SECOND_CPU_BOOT_ADDR     (MEMORY_AXI_ACPU_START_ADDR + 0x0004)
#define MEMORY_AXI_SECOND_CPU_BOOT_SIZE     (4)

/*onchiprom�����ڴӺ��ж�fastboot2�Ƿ��Ѽ�װ��*/
#define MEMORY_AXI_READY_FLAG_ADDR          (MEMORY_AXI_ACPU_START_ADDR + 0x0008)
#define MEMORY_AXI_READY_FLAG_SIZE          (4)

/*onchiprom�����ڼ�¼fastboot2����ת��ڵ�ַ*/
#define MEMORY_AXI_FASTBOOT_ENTRY_ADDR      (MEMORY_AXI_ACPU_START_ADDR + 0x000C)
#define MEMORY_AXI_FASTBOOT_ENTRY_SIZE      (4)

/*�ػ�����־*/
#define MEMORY_AXI_PD_CHARGE_ADDR           (MEMORY_AXI_ACPU_START_ADDR + 0x0010)
#define MEMORY_AXI_PD_CHARGE_SIZE           (4)

/*������*/
#define MEMORY_AXI_DBG_ALARM_ADDR           (MEMORY_AXI_ACPU_START_ADDR + 0x0014)
#define MEMORY_AXI_DBG_ALARM_SIZE           (4)

#define MEMORY_AXI_CPU1_STACK_ADDR          (MEMORY_AXI_ACPU_START_ADDR + 0x0018)
#define MEMORY_AXI_CPU1_STACK_SIZE          (4)

#define MEMORY_AXI_CPU2_STACK_ADDR          (MEMORY_AXI_ACPU_START_ADDR + 0x001C)
#define MEMORY_AXI_CPU2_STACK_SIZE          (4)

#define MEMORY_AXI_CPU3_STACK_ADDR          (MEMORY_AXI_ACPU_START_ADDR + 0x0020)
#define MEMORY_AXI_CPU3_STACK_SIZE          (4)

#if 0
#define MEMORY_AXI_NV_RESTORE_FLAG_ADDR     (MEMORY_AXI_ACPU_START_ADDR + 0x0024)
#define MEMORY_AXI_NV_RESTORE_FLAG_SIZE     (4)
#endif

#define MEMORY_AXI_COLD_START_ADDR          (MEMORY_AXI_ACPU_START_ADDR + 0x0028)
#define MEMORY_AXI_COLD_START_SIZE          (4)

/*Added by y65256 for wdg log save On 2013-6-15 Begin*/
#define MEMORY_AXI_ANDROID_REBOOT_FLAG_ADDR	(MEMORY_AXI_ACPU_START_ADDR + 0x002C)
#define MEMORY_AXI_ANDROID_REBOOT_FLAG_SIZE	4

#define MEMORY_AXI_ACPU_WDTRST_REBOOT_FLAG_ADDR	(MEMORY_AXI_ACPU_START_ADDR + 0x0030)
#define MEMORY_AXI_ACPU_WDTRST_REBOOT_FLAG_SIZE	4

#define MEMORY_AXI_ABNRST_BITMAP_ADDR	(MEMORY_AXI_ACPU_START_ADDR + 0x0034)
#define MEMORY_AXI_ABNRST_BITMAP_SIZE	4
/*Added by y65256 for wdg log save On 2013-6-15 End*/

#define AXI_MODEM_PANIC_FLAG_ADDR  		    (MEMORY_AXI_ACPU_START_ADDR + 0x38)
#define AXI_MODEM_PANIC_FLAG_SIZE 	        4
#define AXI_MODEM_PANIC_FLAG                0x68697369

#define MEMORY_AXI_ACPU_START_END_ADDR      (AXI_MODEM_PANIC_FLAG_ADDR + AXI_MODEM_PANIC_FLAG_SIZE)

/*Memory size check*/
#if (MEMORY_AXI_ACPU_START_END_ADDR > (MEMORY_AXI_ACPU_START_ADDR + MEMORY_AXI_ACPU_START_SIZE))
#error AXI(MEMORY_AXI_ACPU_START_END_ADDR) USED beyond the mark (MEMORY_AXI_ACPU_START_ADDR + MEMORY_AXI_ACPU_START_SIZE)
#endif


#define NVUPDATE_SUCCESS    0x5555AAAA
#define NVUPDATE_FAILURE    0xAAAA5555
#endif /*_GENERAL_SRAM_MAP_INCLUDE_H_*/


