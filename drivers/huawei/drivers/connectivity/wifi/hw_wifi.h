#ifndef __HW_WIFI_H__
#define __HW_WIFI_H__

/************修改记录************************
版本:				日期:		解决问题:
HUAWEI-2014.005:       0609		solve scan_done panic.
HUAWEI-2014.006:       0626		scan_done panic patch.
HUAWEI-2014.007:       0725     use huawei customize country code.
HUAWEI-2014.008:       0926             softap cause system panic. 
HUAWEI-2014.011:       1015             scan retry for err -4.
HUAWEI-2014.012:       1030             p2p test cause system panic.
HUAWEI-2014.013:       1203             disable timestamps.
**********************************************/
#include <typedefs.h>
#include <linuxver.h>
#include <osl.h>
#include <bcmutils.h>
#include <dngl_stats.h>
#include <dhd.h>
#include <wlioctl.h>
#include <wl_iw.h>

#define        HUAWEI_VERSION_STR ", HUAWEI-2014.013"

extern void get_customized_country_code_for_hw(char *country_iso_code, wl_country_t *cspec);
#ifdef HW_PATCH_DISABLE_TCP_TIMESTAMPS
extern void hw_set_connect_status(struct net_device *ndev, int status);
#endif
extern void hw_dhd_check_and_disable_timestamps(void); /* called in ipv4/tcp_input.c */
#endif
