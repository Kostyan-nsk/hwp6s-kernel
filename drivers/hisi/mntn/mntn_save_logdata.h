/*
 * head file of mntn_save_logdata.c
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef	__MNTN_SAVE_LOGDATA_H__
#define __MNTN_SAVE_LOGDATA_H__

#include <linux/types.h>


/*
 ***************************************************************************
 * Macro define here
 ***************************************************************************
*/
#define MNTN_AP_LOGDIR        "/data/ap-log/"
#define MNTN_CP_LOGDIR        "/data/cp-log/"
#define MNTN_HIFI_LOGDIR        "/data/hifi-log/"
#define MNTN_MCU_LOGDIR        MNTN_AP_LOGDIR

#define MNTN_SRAME_ON_FILE    "sram_on_memory.bin"
#define MNTN_SRAME_OFF_FILE    "sram_off_memory.bin"
#define MNTN_SRAME_MDM_FILE    "sram_mdm_memory.bin"
#define MNTN_SRAME_MCU_FILE    "sram_mcu_memory.bin"

#define MNTN_BBE16_LCS_FILE    "bbe16_lcs_memory.bin"
#define MNTN_BBE16_TDSTABLE_FILE    "bbe16_tds_table_memory.bin"
#define MNTN_BBE16_IMGDDR_FILE    "bbe16_img_ddr_memory.bin"
#define MNTN_BBE16_DTCM_FILE    "bbe16_dtcm_memory.bin"

#define MNTN_SC_OFF_FILE    "sc_off_memory.bin"
#define MNTN_SC_ON_FILE    "sc_on_memory.bin"
#define MNTN_PMCTRL_FILE    "pmctrl_memory.bin"

#define MNTN_MODEM_LOG_FILE    "modem_log.bin"
#define MNTN_MODEM_MEMORY_FILE    "modem_memory.bin"

#define MNTN_HIFI_MEMORY_FILE    "hifi_memory.bin"
#define MNTN_HIFI_6401_FILE    "hi6401.bin"

#define MNTN_MCU_MEMORY_FILE    "mcu_memory.bin"
#define MNTN_MCU_LOG_FILE    "mcu_log.bin"

#define MNTN_HISTORY_LOG	"history.log"

/*data/ property value*/
#define MNTN_HISILOG_DIR_PRO_VALUE    0770
/*data/ap_log property value*/
#define MNTN_AP_DIR_PRO_VALUE    0770
/*data/cp_log property value*/
#define MNTN_CP_DIR_PRO_VALUE    0770
/*data/hifi_log property value*/
#define MNTN_HIFI_DIR_PRO_VALUE    0770

#define MNTN_MCU_DIR_PRO_VALUE    MNTN_AP_DIR_PRO_VALUE
#define MNTN_HISTORY_LOG_PRO_VALUE    0660
#define MNTN_LOG_FILE_PRO_VALUE    0660

#define MNTN_LOG_DIR_OWNER_UID    0		/*root*/
#define MNTN_LOG_DIR_OWNER_GID    1000		/*system*/

#define MNTN_LOG_FILE_OWNER_UID    MNTN_LOG_DIR_OWNER_UID
#define MNTN_LOG_FILE_OWNER_GID    MNTN_LOG_DIR_OWNER_GID

#define MNTN_CP_LOG_MAX_NUMBER		5
#define MNTN_MCU_LOG_MAX_NUMBER		5
#define MNTN_HIFI_LOG_MAX_NUMBER		5
#define MNTN_MAX_APR_CMD_LEN		256
/*
 ***************************************************************************
 * Struct define here
 ***************************************************************************
*/
typedef enum MNTN_ACCESS_DOMAIN
{
    ACCESS_DOMAIN_NONE,
    ACCESS_DOMAIN_BBE16_DTCM,
    ACCESS_DOMAIN_MDM_SRAM
} eACCESS_DOMAIN_TYPE;

typedef enum eMNTN_MODULE
{
    BSP_MNTN_MODULE_CCORE,
    BSP_MNTN_MODULE_HIFI,
    BSP_MNTN_MODULE_MCU,
} emntn_module;
/*
 ***************************************************************************
 *extern functions here
 ***************************************************************************
*/
extern int mntn_mdm_reset_save_log(const char *preason);
extern int mntn_hifi_reset_save_log(const char *preason);
extern int mntn_mcu_reset_save_log(const char *preason);
#endif	/* End #define __MNTN_SAVE_LOGDATA_H__ */
