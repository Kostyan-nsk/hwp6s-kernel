/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : pmic_reg.h
  �� �� ��   : ����
  ��    ��   : fansaihua
  ��������   : 2013��3��15��
  ����޸�   :
  ��������   : pmic_reg.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��3��15��
    ��    ��   : FANSAIHUA
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __PMIC_REG_H__
#define __PMIC_REG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "pmic_common.h"

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#ifndef BSP_REG
#define BSP_REG(base, reg) (*(volatile unsigned int *)((unsigned int)base + (reg)))
#endif

/* ��var��[pos, pos + bits-1]��������Ϊval,  pos��0��ʼ���
   e.g BDOM_SET_BITS(var, 4, 2, 2) ��ʾ��Bit5~4����Ϊb'10 */
#define BSP_REG_SETBITS(base, reg, pos, bits, val) (BSP_REG(base, reg) = (BSP_REG(base, reg) & (~((((BSP_U32)1 << (bits)) - 1) << (pos)))) \
                                                                         | ((BSP_U32)((val) & (((BSP_U32)1 << (bits)) - 1)) << (pos)))

/* ��ȡvar��[pos, pos + bits-1]����ֵ,  pos��0��ʼ��� */
#define BSP_REG_GETBITS(base, reg, pos, bits) ((BSP_REG(base, reg) >> (pos)) & (((unsigned int)1 << (bits)) - 1))

extern BSP_S32 pmic_reg_write(COMMON_REG_CFG* reg_cfg, BSP_U32 value);
extern BSP_S32 pmic_reg_read(COMMON_REG_CFG* reg_cfg, BSP_U32 *value);
extern BSP_S32 pmu_status_reg_read(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id);
extern BSP_S32 pmu_status_reg_clear(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id);
extern BSP_S32 pmic_status_reg_read(PMUIRQ_RES_CTRL* pmu_res_ctrl,BSP_U32 reg_type_id,BSP_U32* data_buf);
extern PMUIRQ_STAT* pmic_get_pmustat(void);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of BSP_SSI.h */
