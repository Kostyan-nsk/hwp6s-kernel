#ifndef _HI_GLOBAL_SRAM_MAP_INCLUDE_H_
#define _HI_GLOBAL_SRAM_MAP_INCLUDE_H_


#include "soc_baseaddr_interface.h"

/*******************************************
��˹���ͷ�ļ�
�������������ϵͳ���ͷ�ļ�
*******************************************/

#define SRAM_BASE_ADDR				(SOC_SRAM_OFF_BASE_ADDR)

/*��256KB���ֳ�������*/
#define SRAM_SIZE					(0x40000)

/*��һ����:64KB*/
#define SRAM_SECTION1_SIZE			(0x10000)

/*�ڶ�����:192KB*/
#define SRAM_SECTION2_SIZE			(0x30000)

/*-------------------------------------------------------------------------------
                            SRAM����(��ʱ����)
  -------------------------------------------------------------------------------
  onchiprom/fastboot�����׶�:
                          -----------------------
                         |  fastboot1��VRL       |->2KB
                          -----------------------
                         |  fastboot1��������	 |----
                          -----------------------     |->58KB
                         |  onchiprom��ջ        |----
                          -----------------------
                         |  �����־��           |->4KB
                          -----------------------
                         |  ��ȫOS               |->192KB
                          -----------------------
  linux/vxworks�׶�:
                          -----------------------
                         |  IPF				     |->30KB
                          -----------------------
                         |  ��������	 		 |->30KB
                          -----------------------
                         |  �����־��           |->4KB
                          -----------------------
                         |  ��ȫOS               |->192KB
                          -----------------------
                         
-------------------------------------------------------------------------------*/

/*ipf use section 30K*/
#define IPF_SRAM_ADDR				(SRAM_BASE_ADDR)
#define IPF_SRAM_SIZE				(0x000007800)

/*�ṩ��BBE16����TLģDRX�����б�����Ϣ��CCPU����ʣ���ַΪ���Ե�ַ���ʷ���ǰ��*/
#define SRAM_RTT_SLEEP_FLAG_ADDR    (IPF_SRAM_ADDR + IPF_SRAM_SIZE)    /*0xFFF07800,���иĶ���Ҫ֪ͨT/L DSP*/
#define SRAM_RTT_SLEEP_FLAG_SIZE    (32)

/*����LDSP˯�߻��ѱ�־*/
#define MEMORY_AXI_LDSP_AWAKE_ADDR  (SRAM_RTT_SLEEP_FLAG_ADDR + SRAM_RTT_SLEEP_FLAG_SIZE)
#define MEMORY_AXI_LDSP_AWAKE_SIZE  (4)

/*****************���ϱ��־���·������*********************/
/*reserved 30K*/
#define RESERVED_SRAM_ADDR		    (MEMORY_AXI_LDSP_AWAKE_ADDR + MEMORY_AXI_LDSP_AWAKE_SIZE)
#define RESERVED_SRAM_SIZE			(0x000007800 - SRAM_RTT_SLEEP_FLAG_SIZE - MEMORY_AXI_LDSP_AWAKE_SIZE)

/*general use section 4K*/
#define GENERAL_SRAM_ADDR			(RESERVED_SRAM_ADDR + RESERVED_SRAM_SIZE)
#define GENERAL_SRAM_SIZE			(0x000001000)

/*teeos use section 192K*/
#define TEEOS_SRAM_ADDR			    (SRAM_BASE_ADDR + SRAM_SECTION1_SIZE)
#define TEEOS_SRAM_SIZE			    (SRAM_SECTION2_SIZE)

/*******************************************
MODEM��456K SRAM�������
*******************************************/
/*GU DSP LOAD 388K*/
#define MEMORY_AXI_DSP_LOAD_ADDR      (SOC_SRAM_MDM_BASE_ADDR)
#define MEMORY_AXI_DSP_LOAD_SIZE      (388*1024)
/*BBPMASTER 56K*/
#define MEMORY_AXI_TTF_BBP_ADDR       (MEMORY_AXI_DSP_LOAD_ADDR + MEMORY_AXI_DSP_LOAD_SIZE)
#define MEMORY_AXI_TTF_BBP_SIZE       (56*1024)
/*TL DSP 12K*/
#define MEMORY_AXI_TDL_ADDR           (MEMORY_AXI_TTF_BBP_ADDR + MEMORY_AXI_TTF_BBP_SIZE)
#define MEMORY_AXI_TDL_SIZE           (12*1024)
#endif /*_HI_GLOBAL_MEM_MAP_INCLUDE_H_*/

