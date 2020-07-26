/******************************************************************************

                 ��Ȩ���� (C), 2001-2013, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : bbp_comm_on_interface.h
  �� �� ��   : ����
  ��    ��   : Excel2Code
  ��������   : 2013-06-07 14:16:46
  ����޸�   :
  ��������   : �ӿ�ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��6��7��
    ��    ��   : w56557
    �޸�����   : �ӡ�Hi6620V100 BBP�Ĵ����ֲ�_COMM_ON.xml���Զ�����

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "soc_baseaddr_interface.h"

#ifndef __BBP_COMM_ON_INTERFACE_H__
#define __BBP_COMM_ON_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/***======================================================================***
                     (1/3) register_define_afc_mix
 ***======================================================================***/
/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      0����ʾ����AFCѡ��PDM�����
             1����ʾ����ѡ��PWM���
   UNION�ṹ:  BBPCOMM_AFC_PWM_SEL_MIX_UNION */
#define BBPCOMM_AFC_PWM_SEL_MIX_ADDR                  (SOC_BBP_COMM_ON_BASE_ADDR + 0x3000)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      1����ʾ����AFCʹ�ܣ�
             0����ʾ����AFC��ʹ��
   UNION�ṹ:  BBPCOMM_AFC_EN_MIX_UNION */
#define BBPCOMM_AFC_EN_MIX_ADDR                       (SOC_BBP_COMM_ON_BASE_ADDR + 0x3004)

/* �Ĵ���˵����
 bit[31:12]  ����
 bit[11:0]   AFC����ֵ
   UNION�ṹ:  BBPCOMM_AFC_CFG_WIDTH_MIX_UNION */
#define BBPCOMM_AFC_CFG_WIDTH_MIX_ADDR                (SOC_BBP_COMM_ON_BASE_ADDR + 0x3008)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      1����ʾѡ������AFCֵ
             0����ʾѡ��BBP������AFCֵ
   UNION�ṹ:  BBPCOMM_AFC_CFG_SEL_MIX_UNION */
#define BBPCOMM_AFC_CFG_SEL_MIX_ADDR                  (SOC_BBP_COMM_ON_BASE_ADDR + 0x300C)



/***======================================================================***
                     (2/3) register_define_afc_mix_ch1
 ***======================================================================***/
/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      0����ʾ����AFCѡ��PDM�����
             1����ʾ����ѡ��PWM���
   UNION�ṹ:  BBPCOMM_AFC_PWM_SEL_MIX_CH1_UNION */
#define BBPCOMM_AFC_PWM_SEL_MIX_CH1_ADDR              (SOC_BBP_COMM_ON_BASE_ADDR + 0x3200)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      1����ʾ����AFCʹ�ܣ�
             0����ʾ����AFC��ʹ��
   UNION�ṹ:  BBPCOMM_AFC_EN_MIX_CH1_UNION */
#define BBPCOMM_AFC_EN_MIX_CH1_ADDR                   (SOC_BBP_COMM_ON_BASE_ADDR + 0x3204)

/* �Ĵ���˵����
 bit[31:12]  ����
 bit[11:0]   AFC����ֵ
   UNION�ṹ:  BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_UNION */
#define BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_ADDR            (SOC_BBP_COMM_ON_BASE_ADDR + 0x3208)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      1����ʾѡ������AFCֵ
             0����ʾѡ��BBP������AFCֵ
   UNION�ṹ:  BBPCOMM_AFC_CFG_SEL_MIX_CH1_UNION */
#define BBPCOMM_AFC_CFG_SEL_MIX_CH1_ADDR              (SOC_BBP_COMM_ON_BASE_ADDR + 0x320C)



/***======================================================================***
                     (3/3) register_define_hkadc_ssi
 ***======================================================================***/
/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    �������
   UNION�ṹ:  BBPCOMM_DSP_START_UNION */
#define BBPCOMM_DSP_START_ADDR                        (SOC_BBP_COMM_ON_BASE_ADDR + 0x3800)

/* �Ĵ���˵����
 bit[31:20]  ����
 bit[19:15]  ����������Ҫ���á�
 bit[14:12]  master check��ͬ��λʱ�䣺
             000�� 1.5clk�� 
             001�� 2.0clk��
              ,,, 
             111�� 5.0 clk.
             Ĭ��ֵ����Ϊ2.
 bit[11:8]   SSI-IP��Ҫ�����ó�ʱ�ȴ�ʱ�䣬��λcycle�� 
             time+1��
 bit[7:3]    ����������Ҫ���á�
 bit[2:0]    ����������Ҫ���á�
   UNION�ṹ:  BBPCOMM_DSP_CFG_UNION */
#define BBPCOMM_DSP_CFG_ADDR                          (SOC_BBP_COMM_ON_BASE_ADDR + 0x3804)

/* �Ĵ���˵����
   ��      �������HKADC����ʱʱ�䳤�ȣ���λcycle��
   UNION�ṹ ���� */
#define BBPCOMM_DSP_WAIT_TIME_ADDR                    (SOC_BBP_COMM_ON_BASE_ADDR + 0x380C)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    ��ʱ��������źš�
   UNION�ṹ:  BBPCOMM_TIME_OUT_ERR_CLR_UNION */
#define BBPCOMM_TIME_OUT_ERR_CLR_ADDR                 (SOC_BBP_COMM_ON_BASE_ADDR + 0x3810)

/* �Ĵ���˵����
   ��      �����ض��������10λ��Ч��
            �������ֱ�Ӷ�ȡ�üĴ���ֵ������Ҫ�κδ���
   UNION�ṹ ���� */
#define BBPCOMM_DSP_RD_DATA_ADDR                      (SOC_BBP_COMM_ON_BASE_ADDR + 0x3814)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    ���HKADC����ʱ״ָ̬ʾ��ƽ�źš�
             �����ʱ�������Ҫ���ó�ʱ����Ĵ���hkadc_time_out_err_clr��
   UNION�ṹ:  BBPCOMM_TIME_OUT_ERR_UNION */
#define BBPCOMM_TIME_OUT_ERR_ADDR                     (SOC_BBP_COMM_ON_BASE_ADDR + 0x3818)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    ���
   UNION�ṹ:  BBPCOMM_DSP_START_CLR_UNION */
#define BBPCOMM_DSP_START_CLR_ADDR                    (SOC_BBP_COMM_ON_BASE_ADDR + 0x381C)

/* �Ĵ���˵���������д�Ĵ���01����ֵ
 bit[31:31]  HKADC
 bit[30:24]  HKADC
 bit[23:16]  HKADC
 bit[15:15]  HKADC
 bit[14:8]   HKADC
 bit[7:0]    HKADC
   UNION�ṹ:  BBPCOMM_WR01_DATA_UNION */
#define BBPCOMM_WR01_DATA_ADDR                        (SOC_BBP_COMM_ON_BASE_ADDR + 0x3820)

/* �Ĵ���˵���������д�Ĵ���34����ֵ
 bit[31:31]  HKADC
 bit[30:24]  HKADC
 bit[23:16]  HKADC
 bit[15:15]  HKADC
 bit[14:8]   HKADC
 bit[7:0]    HKADC
   UNION�ṹ:  BBPCOMM_WR34_DATA_UNION */
#define BBPCOMM_WR34_DATA_ADDR                        (SOC_BBP_COMM_ON_BASE_ADDR + 0x3824)

/* �Ĵ���˵���������д�Ĵ���ʱ��ֵ
 bit[31:16]  HKADC
 bit[15:0]   HKADC
   UNION�ṹ:  BBPCOMM_HKADC_DELAY01_UNION */
#define BBPCOMM_HKADC_DELAY01_ADDR                    (SOC_BBP_COMM_ON_BASE_ADDR + 0x3828)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    
   UNION�ṹ:  BBPCOMM_HKADC_DBG_IND_UNION */
#define BBPCOMM_HKADC_DBG_IND_ADDR                    (SOC_BBP_COMM_ON_BASE_ADDR + 0x38D0)

/* �Ĵ���˵����
   ��      ��������
   UNION�ṹ ���� */
#define BBPCOMM_DBG_INFO_ADDR                         (SOC_BBP_COMM_ON_BASE_ADDR + 0x38D4)





/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/



/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/



/*****************************************************************************
  6 STRUCT����
*****************************************************************************/



/*****************************************************************************
  7 UNION����
*****************************************************************************/

/***======================================================================***
                     (1/3) register_define_afc_mix
 ***======================================================================***/
/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_PWM_SEL_MIX_UNION
 �ṹ˵��  : AFC_PWM_SEL_MIX �Ĵ����ṹ���塣��ַƫ����:0x00����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_pwm_sel_mix_reg;
    struct
    {
        unsigned long  afc_pwm_sel : 1;  /* bit[0]   : 0����ʾ����AFCѡ��PDM�����
                                                       1����ʾ����ѡ��PWM��� */
        unsigned long  reserved    : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_PWM_SEL_MIX_UNION;
#define BBPCOMM_AFC_PWM_SEL_MIX_afc_pwm_sel_START  (0)
#define BBPCOMM_AFC_PWM_SEL_MIX_afc_pwm_sel_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_EN_MIX_UNION
 �ṹ˵��  : AFC_EN_MIX �Ĵ����ṹ���塣��ַƫ����:0x04����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_en_mix_reg;
    struct
    {
        unsigned long  afc_en   : 1;  /* bit[0]   : 1����ʾ����AFCʹ�ܣ�
                                                    0����ʾ����AFC��ʹ�� */
        unsigned long  reserved : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_EN_MIX_UNION;
#define BBPCOMM_AFC_EN_MIX_afc_en_START    (0)
#define BBPCOMM_AFC_EN_MIX_afc_en_END      (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_CFG_WIDTH_MIX_UNION
 �ṹ˵��  : AFC_CFG_WIDTH_MIX �Ĵ����ṹ���塣��ַƫ����:0x08����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_cfg_width_mix_reg;
    struct
    {
        unsigned long  afc_cfg_width : 12; /* bit[0-11] : AFC����ֵ */
        unsigned long  reserved      : 20; /* bit[12-31]: ���� */
    } reg;
} BBPCOMM_AFC_CFG_WIDTH_MIX_UNION;
#define BBPCOMM_AFC_CFG_WIDTH_MIX_afc_cfg_width_START  (0)
#define BBPCOMM_AFC_CFG_WIDTH_MIX_afc_cfg_width_END    (11)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_CFG_SEL_MIX_UNION
 �ṹ˵��  : AFC_CFG_SEL_MIX �Ĵ����ṹ���塣��ַƫ����:0x0C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_cfg_sel_mix_reg;
    struct
    {
        unsigned long  afc_cfg_sel : 1;  /* bit[0]   : 1����ʾѡ������AFCֵ
                                                       0����ʾѡ��BBP������AFCֵ */
        unsigned long  reserved    : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_CFG_SEL_MIX_UNION;
#define BBPCOMM_AFC_CFG_SEL_MIX_afc_cfg_sel_START  (0)
#define BBPCOMM_AFC_CFG_SEL_MIX_afc_cfg_sel_END    (0)


/***======================================================================***
                     (2/3) register_define_afc_mix_ch1
 ***======================================================================***/
/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_PWM_SEL_MIX_CH1_UNION
 �ṹ˵��  : AFC_PWM_SEL_MIX_CH1 �Ĵ����ṹ���塣��ַƫ����:0x00����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_pwm_sel_mix_ch1_reg;
    struct
    {
        unsigned long  afc_pwm_sel_ch1 : 1;  /* bit[0]   : 0����ʾ����AFCѡ��PDM�����
                                                           1����ʾ����ѡ��PWM��� */
        unsigned long  reserved        : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_PWM_SEL_MIX_CH1_UNION;
#define BBPCOMM_AFC_PWM_SEL_MIX_CH1_afc_pwm_sel_ch1_START  (0)
#define BBPCOMM_AFC_PWM_SEL_MIX_CH1_afc_pwm_sel_ch1_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_EN_MIX_CH1_UNION
 �ṹ˵��  : AFC_EN_MIX_CH1 �Ĵ����ṹ���塣��ַƫ����:0x04����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_en_mix_ch1_reg;
    struct
    {
        unsigned long  afc_en_ch1 : 1;  /* bit[0]   : 1����ʾ����AFCʹ�ܣ�
                                                      0����ʾ����AFC��ʹ�� */
        unsigned long  reserved   : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_EN_MIX_CH1_UNION;
#define BBPCOMM_AFC_EN_MIX_CH1_afc_en_ch1_START  (0)
#define BBPCOMM_AFC_EN_MIX_CH1_afc_en_ch1_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_UNION
 �ṹ˵��  : AFC_CFG_WIDTH_MIX_CH1 �Ĵ����ṹ���塣��ַƫ����:0x08����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_cfg_width_mix_ch1_reg;
    struct
    {
        unsigned long  afc_cfg_width_ch1 : 12; /* bit[0-11] : AFC����ֵ */
        unsigned long  reserved          : 20; /* bit[12-31]: ���� */
    } reg;
} BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_UNION;
#define BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_afc_cfg_width_ch1_START  (0)
#define BBPCOMM_AFC_CFG_WIDTH_MIX_CH1_afc_cfg_width_ch1_END    (11)


/*****************************************************************************
 �ṹ��    : BBPCOMM_AFC_CFG_SEL_MIX_CH1_UNION
 �ṹ˵��  : AFC_CFG_SEL_MIX_CH1 �Ĵ����ṹ���塣��ַƫ����:0x0C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      afc_cfg_sel_mix_ch1_reg;
    struct
    {
        unsigned long  afc_cfg_sel_ch1 : 1;  /* bit[0]   : 1����ʾѡ������AFCֵ
                                                           0����ʾѡ��BBP������AFCֵ */
        unsigned long  reserved        : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_AFC_CFG_SEL_MIX_CH1_UNION;
#define BBPCOMM_AFC_CFG_SEL_MIX_CH1_afc_cfg_sel_ch1_START  (0)
#define BBPCOMM_AFC_CFG_SEL_MIX_CH1_afc_cfg_sel_ch1_END    (0)


/***======================================================================***
                     (3/3) register_define_hkadc_ssi
 ***======================================================================***/
/*****************************************************************************
 �ṹ��    : BBPCOMM_DSP_START_UNION
 �ṹ˵��  : DSP_START �Ĵ����ṹ���塣��ַƫ����:0x00����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      dsp_start_reg;
    struct
    {
        unsigned long  dsp_start : 1;  /* bit[0-0] : ������� */
        unsigned long  reserved  : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_DSP_START_UNION;
#define BBPCOMM_DSP_START_dsp_start_START  (0)
#define BBPCOMM_DSP_START_dsp_start_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_DSP_CFG_UNION
 �ṹ˵��  : DSP_CFG �Ĵ����ṹ���塣��ַƫ����:0x04����ֵ:0x000C1F3E�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      dsp_cfg_reg;
    struct
    {
        unsigned long  ass        : 3;  /* bit[0-2]  : ����������Ҫ���á� */
        unsigned long  dss        : 5;  /* bit[3-7]  : ����������Ҫ���á� */
        unsigned long  wait_time  : 4;  /* bit[8-11] : SSI-IP��Ҫ�����ó�ʱ�ȴ�ʱ�䣬��λcycle�� 
                                                       time+1�� */
        unsigned long  mst_ck_cfg : 3;  /* bit[12-14]: master check��ͬ��λʱ�䣺
                                                       000�� 1.5clk�� 
                                                       001�� 2.0clk��
                                                        ,,, 
                                                       111�� 5.0 clk.
                                                       Ĭ��ֵ����Ϊ2. */
        unsigned long  rw_bit     : 5;  /* bit[15-19]: ����������Ҫ���á� */
        unsigned long  reserved   : 12; /* bit[20-31]: ���� */
    } reg;
} BBPCOMM_DSP_CFG_UNION;
#define BBPCOMM_DSP_CFG_ass_START         (0)
#define BBPCOMM_DSP_CFG_ass_END           (2)
#define BBPCOMM_DSP_CFG_dss_START         (3)
#define BBPCOMM_DSP_CFG_dss_END           (7)
#define BBPCOMM_DSP_CFG_wait_time_START   (8)
#define BBPCOMM_DSP_CFG_wait_time_END     (11)
#define BBPCOMM_DSP_CFG_mst_ck_cfg_START  (12)
#define BBPCOMM_DSP_CFG_mst_ck_cfg_END    (14)
#define BBPCOMM_DSP_CFG_rw_bit_START      (15)
#define BBPCOMM_DSP_CFG_rw_bit_END        (19)


/*****************************************************************************
 �ṹ��    : BBPCOMM_TIME_OUT_ERR_CLR_UNION
 �ṹ˵��  : TIME_OUT_ERR_CLR �Ĵ����ṹ���塣��ַƫ����:0x10����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      time_out_err_clr_reg;
    struct
    {
        unsigned long  time_out_err_clr : 1;  /* bit[0-0] : ��ʱ��������źš� */
        unsigned long  reserved         : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_TIME_OUT_ERR_CLR_UNION;
#define BBPCOMM_TIME_OUT_ERR_CLR_time_out_err_clr_START  (0)
#define BBPCOMM_TIME_OUT_ERR_CLR_time_out_err_clr_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_TIME_OUT_ERR_UNION
 �ṹ˵��  : TIME_OUT_ERR �Ĵ����ṹ���塣��ַƫ����:0x18����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      time_out_err_reg;
    struct
    {
        unsigned long  time_out_err : 1;  /* bit[0-0] : ���HKADC����ʱ״ָ̬ʾ��ƽ�źš�
                                                        �����ʱ�������Ҫ���ó�ʱ����Ĵ���hkadc_time_out_err_clr�� */
        unsigned long  reserved     : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_TIME_OUT_ERR_UNION;
#define BBPCOMM_TIME_OUT_ERR_time_out_err_START  (0)
#define BBPCOMM_TIME_OUT_ERR_time_out_err_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_DSP_START_CLR_UNION
 �ṹ˵��  : DSP_START_CLR �Ĵ����ṹ���塣��ַƫ����:0x1C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      dsp_start_clr_reg;
    struct
    {
        unsigned long  dsp_start_clr : 1;  /* bit[0-0] : ��� */
        unsigned long  reserved      : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_DSP_START_CLR_UNION;
#define BBPCOMM_DSP_START_CLR_dsp_start_clr_START  (0)
#define BBPCOMM_DSP_START_CLR_dsp_start_clr_END    (0)


/*****************************************************************************
 �ṹ��    : BBPCOMM_WR01_DATA_UNION
 �ṹ˵��  : WR01_DATA �Ĵ����ṹ���塣��ַƫ����:0x20����ֵ:0x00000000�����:32
 �Ĵ���˵��: �����д�Ĵ���01����ֵ
*****************************************************************************/
typedef union
{
    unsigned long      wr01_data_reg;
    struct
    {
        unsigned long  dsp_wr0_cfg_data : 8;  /* bit[0-7]  : HKADC */
        unsigned long  dsp_wr0_cfg_addr : 7;  /* bit[8-14] : HKADC */
        unsigned long  dsp_wr0_cfg_wr   : 1;  /* bit[15-15]: HKADC */
        unsigned long  dsp_wr1_cfg_data : 8;  /* bit[16-23]: HKADC */
        unsigned long  dsp_wr1_cfg_addr : 7;  /* bit[24-30]: HKADC */
        unsigned long  dsp_wr1_cfg_wr   : 1;  /* bit[31-31]: HKADC */
    } reg;
} BBPCOMM_WR01_DATA_UNION;
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_data_START  (0)
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_data_END    (7)
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_addr_START  (8)
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_addr_END    (14)
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_wr_START    (15)
#define BBPCOMM_WR01_DATA_dsp_wr0_cfg_wr_END      (15)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_data_START  (16)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_data_END    (23)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_addr_START  (24)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_addr_END    (30)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_wr_START    (31)
#define BBPCOMM_WR01_DATA_dsp_wr1_cfg_wr_END      (31)


/*****************************************************************************
 �ṹ��    : BBPCOMM_WR34_DATA_UNION
 �ṹ˵��  : WR34_DATA �Ĵ����ṹ���塣��ַƫ����:0x24����ֵ:0x00000000�����:32
 �Ĵ���˵��: �����д�Ĵ���34����ֵ
*****************************************************************************/
typedef union
{
    unsigned long      wr34_data_reg;
    struct
    {
        unsigned long  dsp_wr3_cfg_data : 8;  /* bit[0-7]  : HKADC */
        unsigned long  dsp_wr3_cfg_addr : 7;  /* bit[8-14] : HKADC */
        unsigned long  dsp_wr3_cfg_wr   : 1;  /* bit[15-15]: HKADC */
        unsigned long  dsp_wr4_cfg_data : 8;  /* bit[16-23]: HKADC */
        unsigned long  dsp_wr4_cfg_addr : 7;  /* bit[24-30]: HKADC */
        unsigned long  dsp_wr4_cfg_wr   : 1;  /* bit[31-31]: HKADC */
    } reg;
} BBPCOMM_WR34_DATA_UNION;
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_data_START  (0)
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_data_END    (7)
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_addr_START  (8)
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_addr_END    (14)
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_wr_START    (15)
#define BBPCOMM_WR34_DATA_dsp_wr3_cfg_wr_END      (15)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_data_START  (16)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_data_END    (23)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_addr_START  (24)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_addr_END    (30)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_wr_START    (31)
#define BBPCOMM_WR34_DATA_dsp_wr4_cfg_wr_END      (31)


/*****************************************************************************
 �ṹ��    : BBPCOMM_HKADC_DELAY01_UNION
 �ṹ˵��  : HKADC_DELAY01 �Ĵ����ṹ���塣��ַƫ����:0x28����ֵ:0x00C0003C�����:32
 �Ĵ���˵��: �����д�Ĵ���ʱ��ֵ
*****************************************************************************/
typedef union
{
    unsigned long      hkadc_delay01_reg;
    struct
    {
        unsigned long  dsp_delay1 : 16; /* bit[0-15] : HKADC */
        unsigned long  dsp_delay2 : 16; /* bit[16-31]: HKADC */
    } reg;
} BBPCOMM_HKADC_DELAY01_UNION;
#define BBPCOMM_HKADC_DELAY01_dsp_delay1_START  (0)
#define BBPCOMM_HKADC_DELAY01_dsp_delay1_END    (15)
#define BBPCOMM_HKADC_DELAY01_dsp_delay2_START  (16)
#define BBPCOMM_HKADC_DELAY01_dsp_delay2_END    (31)


/*****************************************************************************
 �ṹ��    : BBPCOMM_HKADC_DBG_IND_UNION
 �ṹ˵��  : HKADC_DBG_IND �Ĵ����ṹ���塣��ַƫ����:0xD0����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      hkadc_dbg_ind_reg;
    struct
    {
        unsigned long  ind_invalid : 1;  /* bit[0-0] :  */
        unsigned long  reserved    : 31; /* bit[1-31]: ���� */
    } reg;
} BBPCOMM_HKADC_DBG_IND_UNION;
#define BBPCOMM_HKADC_DBG_IND_ind_invalid_START  (0)
#define BBPCOMM_HKADC_DBG_IND_ind_invalid_END    (0)




/*****************************************************************************
  8 OTHERS����
*****************************************************************************/



/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of bbp_comm_on_interface.h */
