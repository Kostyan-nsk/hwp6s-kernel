/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
 
  This is an AUTO GENERATED file ! DON'T MODIFY MANUAL! 
  文 件 名   : regulator_desc.h
  生成日期   : 2015年11月11日
  
******************************************************************************/


#ifndef __REGULATOR_DESC_H__
#define __REGULATOR_DESC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#include "balong_lpddr2_udp_regulator_desc.h"
#include "balong_lpddr2_udp_dcm_rf_regulator_desc.h"
#include "balong_lpddr2_udp_6361s_regulator_desc.h"
#include "balong_lpddr2_udp_oscar_rf_regulator_desc.h"
#include "balong_ddr3cap_udp_regulator_desc.h"
#include "balong_ddr3cap_udp_dcm_rf_regulator_desc.h"
#include "balong_ddr3cap_udp_6361s_regulator_desc.h"
#include "balong_mhlcap_udp_regulator_desc.h"
#include "balong_mhl_udp_regulator_desc.h"
#include "balong_lpddr3_udp_regulator_desc.h"
#include "balong_lpddr3_udp_dcm_rf_regulator_desc.h"
#include "balong_lpddr3_udp_6361s_regulator_desc.h"
#include "balong_mhlcap_udp_fhd_regulator_desc.h"
#include "balong_ddr3cap_udp_fhd_regulator_desc.h"
#include "edge_plus_ug_va_regulator_desc.h"
#include "edge_plus_ug_vc_regulator_desc.h"
#include "edge_plus_ug_vb_regulator_desc.h"
#include "oscar_cg_vb_regulator_desc.h"
#include "oscar_3g_vb_regulator_desc.h"
#include "oscar_cg_vc_regulator_desc.h"
#include "oscar_cg_vd_regulator_desc.h"
#include "edge_plus_ul_vb_regulator_desc.h"
#include "edge_plus_ul_vx_regulator_desc.h"
#include "edge_plus_ul_vy_regulator_desc.h"
#include "oscar_ul_vb_regulator_desc.h"
#include "oscar_4g_vc_regulator_desc.h"
#include "oscar_vul_vc_regulator_desc.h"
#include "oscar_vtl_vc_regulator_desc.h"
#include "oscar_tl_vb_regulator_desc.h"
#include "oscar_4g_vd_regulator_desc.h"
#include "oscar_vul_vd_regulator_desc.h"
#include "oscar_vtl_vd_regulator_desc.h"
#include "oscar_4g_ve_regulator_desc.h"
#include "oscar_vul_ve_regulator_desc.h"
#include "oscar_vtl_ve_regulator_desc.h"
#include "sophia_ul_va_regulator_desc.h"
#include "sophia_ul_vb_regulator_desc.h"
#include "sophia_ul_vc_regulator_desc.h"
#include "sophia_ul_vd_regulator_desc.h"
#include "sophia_ulh_va_regulator_desc.h"
#include "sophia_ul_vy_regulator_desc.h"
#include "sophia_tl_va_regulator_desc.h"
#include "sophia_tl_vb_regulator_desc.h"
#include "sophia_tl_vc_regulator_desc.h"
#include "sophia_tl_vd_regulator_desc.h"
#include "sophia_ulg_va_regulator_desc.h"
#include "sophia_ulg_vb_regulator_desc.h"
#include "sophia_ulg_vc_regulator_desc.h"
#include "sophia_ulg_vd_regulator_desc.h"
#include "sophia_ulg_ve_regulator_desc.h"
#include "sophia_ulgh_va_regulator_desc.h"
#include "sophia_clg_va_regulator_desc.h"
#include "sophia_clg_vb_regulator_desc.h"
#include "sophia_clg_vc_regulator_desc.h"
#include "sophia_clgh_va_regulator_desc.h"
#include "sophia_ul_vu_regulator_desc.h"
#include "h30_l01_va_regulator_desc.h"
#include "h30_l01_vb_regulator_desc.h"
#include "h30_l11_va_regulator_desc.h"
#include "h30_l11_vb_regulator_desc.h"
#include "h30_l02_va_regulator_desc.h"
#include "h30_l02_vb_regulator_desc.h"
#include "h30_l12_va_regulator_desc.h"
#include "h30_l12_vb_regulator_desc.h"

#define REGULATOR_CONFIG_DESC_TBL_END_SYMBOL     (-1)

struct regulator_config_desc_table {
    unsigned int board_id;
    struct regulator_desc *table;
};

struct regulator_config_desc_table regulators_total[] = {
    {0, regulators_balong_lpddr2_udp},
    {1, regulators_balong_lpddr2_udp_dcm_rf},
    {2, regulators_balong_lpddr2_udp_6361s},
    {9, regulators_balong_lpddr2_udp_oscar_rf},
    {10, regulators_balong_ddr3cap_udp},
    {11, regulators_balong_ddr3cap_udp_dcm_rf},
    {12, regulators_balong_ddr3cap_udp_6361s},
    {20, regulators_balong_mhlcap_udp},
    {30, regulators_balong_mhl_udp},
    {40, regulators_balong_lpddr3_udp},
    {41, regulators_balong_lpddr3_udp_dcm_rf},
    {42, regulators_balong_lpddr3_udp_6361s},
    {96, regulators_balong_mhlcap_udp_fhd},
    {97, regulators_balong_ddr3cap_udp_fhd},
    {700, regulators_edge_plus_ug_va},
    {708, regulators_edge_plus_ug_vc},
    {709, regulators_edge_plus_ug_vb},
    {732, regulators_oscar_cg_vb},
    {733, regulators_oscar_3g_vb},
    {734, regulators_oscar_cg_vc},
    {735, regulators_oscar_cg_vd},
    {750, regulators_edge_plus_ul_vb},
    {758, regulators_edge_plus_ul_vx},
    {759, regulators_edge_plus_ul_vy},
    {760, regulators_oscar_ul_vb},
    {761, regulators_oscar_4g_vc},
    {762, regulators_oscar_vul_vc},
    {764, regulators_oscar_vtl_vc},
    {770, regulators_oscar_tl_vb},
    {771, regulators_oscar_4g_vd},
    {772, regulators_oscar_vul_vd},
    {774, regulators_oscar_vtl_vd},
    {781, regulators_oscar_4g_ve},
    {782, regulators_oscar_vul_ve},
    {784, regulators_oscar_vtl_ve},
    {800, regulators_sophia_ul_va},
    {801, regulators_sophia_ul_vb},
    {802, regulators_sophia_ul_vc},
    {803, regulators_sophia_ul_vd},
    {805, regulators_sophia_ulh_va},
    {810, regulators_sophia_ul_vy},
    {820, regulators_sophia_tl_va},
    {821, regulators_sophia_tl_vb},
    {822, regulators_sophia_tl_vc},
    {823, regulators_sophia_tl_vd},
    {830, regulators_sophia_ulg_va},
    {831, regulators_sophia_ulg_vb},
    {832, regulators_sophia_ulg_vc},
    {833, regulators_sophia_ulg_vd},
    {834, regulators_sophia_ulg_ve},
    {835, regulators_sophia_ulgh_va},
    {840, regulators_sophia_clg_va},
    {841, regulators_sophia_clg_vb},
    {842, regulators_sophia_clg_vc},
    {845, regulators_sophia_clgh_va},
    {860, regulators_sophia_ul_vu},
    {900, regulators_h30_l01_va},
    {901, regulators_h30_l01_vb},
    {910, regulators_h30_l11_va},
    {911, regulators_h30_l11_vb},
    {920, regulators_h30_l02_va},
    {921, regulators_h30_l02_vb},
    {930, regulators_h30_l12_va},
    {931, regulators_h30_l12_vb},

    {REGULATOR_CONFIG_DESC_TBL_END_SYMBOL, NULL}
};


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif