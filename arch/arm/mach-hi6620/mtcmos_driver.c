/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : mtcmos_driver.c
  �� �� ��   : ����
  ��    ��   : ����ΰ 176398
  ��������   : 2012��12��22��
  ����޸�   :
  ��������   : mtcmos����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��12��22��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/io.h>
#include "soc_baseaddr_interface.h"
#include "soc_ao_sctrl_interface.h"
#include "mtcmos_driver.h"
#include "mtcmos_driver_inner.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*����IP�Ͷ�Ӧϵͳ��������bit��ϵ*/
unsigned int ip_mtcmos_switch_pos[] = {
    /*MTCMOS Block 1*/
    [VCC_G2D] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_2g2d_START,/*G2D*/
    [VCC_EDC0] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_3edc0_START,/*EDC0*/
    [VCC_EDC1] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_4edc1_START,/*EDC1*/
    [VCC_ISP] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_5isp_START,/*ISP*/
    [VCC_ENC] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_6enc_START,/*ENC*/
    [VCC_DEC] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_7dec_START,/*DEC*/
    [VCC_HIFI] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_8hifi_START,/*HIFI*/
    [VCC_VPP] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_9vpp_START,/*VPP*/
    [VCC_DSPBBE] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_10dspbbe_START,/*DSPBBE*/
    [VCC_GBBP1] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_12bbpcommtop2c_gbbp1_START,/*GBBP1*/
    [VCC_GBBP2] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_13bbpcommtop2d_gbbp2_START,/*GBBP2*/
    [VCC_LTEBBP] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_14ltebbptop_START,/*LTEBBP*/
    [VCC_BBPCOMM2] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_15bbpcommtop2b_bbpcomm2_START,/*BBPCOMM2*/
    [VCC_WTDBBP1] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_18bbpcommtop1a_wtdbbp1_START,/*WTDBBP1*/
    [VCC_WTDBBP2] = SOC_AO_SCTRL_SC_PW_EN0_pw_en0_17bbpcommtop1b_wtdbbp2_START,/*WTDBBP2*/
    /*MTCMOS Block 1*/
    [VCC_ACPU1] = SOC_AO_SCTRL_SC_PW_EN1_pw_en1_1acpu1_START,/*ACPU1*/
    [VCC_ACPU2] = SOC_AO_SCTRL_SC_PW_EN1_pw_en1_2acpu2_START,/*ACPU2*/
    [VCC_ACPU3] = SOC_AO_SCTRL_SC_PW_EN1_pw_en1_3acpu3_START,/*ACPU3*/
    [VCC_CM3MEM] = SOC_AO_SCTRL_SC_PW_EN1_pw_en1_4cm3mem_START,/*CM3MEM*/
    [VCC_MCPU] = SOC_AO_SCTRL_SC_PW_EN1_pw_en1_5mcpu_START,/*MCPU*/
};

struct mtcmos_pwup_time_reg_stru pwup_time_reg_info[] = {
    /*MTCMOS Block 1*/
    [VCC_G2D] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME1_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_15g2d_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_15g2d_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_15g2d_START,
    },/*G2D*/
    [VCC_EDC0] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME1_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_31edc0_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_31edc0_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME1_pwup_time1_31edc0_START,
    },/*EDC0*/
    [VCC_EDC1] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME2_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_15edc1_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_15edc1_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_15edc1_START,
    },/*EDC1*/
    [VCC_ISP] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME2_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_31isp_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_31isp_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME2_pwup_time2_31isp_START,
    },/*ISP*/
    [VCC_ENC] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME3_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_15enc_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_15enc_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_15enc_START,
    },/*ENC*/
    [VCC_DEC] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME3_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_31dec_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_31dec_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME3_pwup_time3_31dec_START,
    },/*DEC*/
    [VCC_HIFI] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME4_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_15hifi_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_15hifi_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_15hifi_START,
    },/*HIFI*/
    [VCC_VPP] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME4_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_31vpp_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_31vpp_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME4_pwup_time4_31vpp_START,
    },/*VPP*/
    [VCC_DSPBBE] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME5_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME5_pwup_time5_15dspbbe_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME5_pwup_time5_15dspbbe_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME5_pwup_time5_15dspbbe_START,
    },/*DSPBBE*/
    [VCC_GBBP1] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME6_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_15bbpcommtop2c_gbbp1_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_15bbpcommtop2c_gbbp1_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_15bbpcommtop2c_gbbp1_START,
    },/*GBBP1*/
    [VCC_GBBP2] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME6_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_31bbpcommtop2d_gbbp2_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_31bbpcommtop2d_gbbp2_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME6_pwup_time6_31bbpcommtop2d_gbbp2_START,
    },/*GBBP2*/
    [VCC_LTEBBP] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME7_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_15ltebbptop_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_15ltebbptop_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_15ltebbptop_START,
    },/*LTEBBP*/
    [VCC_BBPCOMM2] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME7_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_31bbpcommtop2b_bbpcomm2_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_31bbpcommtop2b_bbpcomm2_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME7_pwup_time7_31bbpcommtop2b_bbpcomm2_START,
    },/*BBPCOMM2*/
    [VCC_WTDBBP1] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME9_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_15bbpcommtop1a_wtdbbp1_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_15bbpcommtop1a_wtdbbp1_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_15bbpcommtop1a_wtdbbp1_START,
    },/*WTDBBP1*/
    [VCC_WTDBBP2] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME8_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME8_pwup_time8_31bbpcommtop1b_wtdbbp2_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME8_pwup_time8_31bbpcommtop1b_wtdbbp2_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME8_pwup_time8_31bbpcommtop1b_wtdbbp2_START,
    },/*WTDBBP2*/
    /*MTCMOS Block 1*/
    [VCC_ACPU1] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME10_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME10_pwup_time10_31acpu1_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME10_pwup_time10_31acpu1_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME10_pwup_time10_31acpu1_START,
    },/*ACPU1*/
    [VCC_ACPU2] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME11_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_15acpu2_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_15acpu2_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_15acpu2_START,
    },/*ACPU2*/
    [VCC_ACPU3] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME9_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_31acpu3_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_31acpu3_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME9_pwup_time9_31acpu3_START,
    },/*ACPU3*/
    [VCC_CM3MEM] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME12_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME12_pwup_time12_cm3mem_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME12_pwup_time12_cm3mem_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME12_pwup_time12_cm3mem_START,
    },/*CM3MEM*/
    [VCC_MCPU] = {
        .pwup_time_reg = SOC_AO_SCTRL_SC_PWUP_TIME12_ADDR(CALC_REG_OFFSET_BASE_ADDR),
        .pwup_time_mask = CALC_REG_MSK(SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_31mcpu_START, \
                                       SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_31mcpu_END),
        .pwup_time_start_bit = SOC_AO_SCTRL_SC_PWUP_TIME11_pwup_time11_31mcpu_START,
    },/*MCPU*/

};

/*ƽ̨mtcmos id��mtcmos����ʹ�õ�id�Ķ�Ӧ��ϵ*/
int platform_mtcmos_id_2_actual_id[] = {
    [5] = (int)VCC_G2D,
    [4] = (int)VCC_EDC0,
    [3] = (int)VCC_EDC1,
    [6] = (int)VCC_ISP,
    [9] = (int)VCC_ENC,
    [7] = (int)VCC_DEC,
    [10] = (int)VCC_HIFI,
    [8] = (int)VCC_VPP,
    [11] = (int)VCC_DSPBBE,
    [15] = (int)VCC_GBBP1,
    [16] = (int)VCC_GBBP2,
    [17] = (int)VCC_LTEBBP,
    [18] = (int)VCC_BBPCOMM2,
    [13] = (int)VCC_WTDBBP1,
    [14] = (int)VCC_WTDBBP2,
/*MTCMOS Block 2*/
    [0] = (int)VCC_ACPU1,
    [1] = (int)VCC_ACPU2,
    [2] = (int)VCC_ACPU3,
    [19] = (int)VCC_CM3MEM,
#ifdef _DRV_LLT_
    [12] = (int)VCC_ENUM_END + 1,
#else
    [12] = (int)VCC_MCPU,
#endif
};

/*ao system ctrl register base address*/
unsigned int ao_sctrl_base_addr;

static DEFINE_MUTEX(reg_lock_mutex);

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : mtcmos_power_is_on
 ��������  : ��鵱ǰmtcoms�Ƿ��Ǵ�״̬
 �������  : int id //mtcmos id
 �������  : ��
 �� �� ֵ  : int    //0:�ɹ�  ����ֵ:ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��12��25��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ɺ���

*****************************************************************************/
int mtcmos_power_is_on( int id )
{
    unsigned int val;

    /*printk("%s id:%d\n", __func__, id);*/
    if ((id < (int)VCC_BLOCK1_END) && (id >= (int)VCC_BLOCK1_START)) {
        val = readl(ao_sctrl_base_addr + SC_MTCMOS_STAT0);
    } else if ((id < (int)VCC_BLOCK2_END) && (id >= (int)VCC_BLOCK2_START)) {
        val = readl(ao_sctrl_base_addr + SC_MTCMOS_STAT1);
    } else {
    	printk("%s invalid id:%d\n", __func__, id);
        return MTCMOS_MODULE_ERROR;
    }

    val &= (0x1 << ip_mtcmos_switch_pos[id]);
    val = val >> ip_mtcmos_switch_pos[id];

    if (MTCMOS_ON_STATUS == val) {
        return MTCMOS_MODULE_OK;
    } else {
    	printk("%s mtcmos status no ready id:%d\n", __func__, id);
        return MTCMOS_MODULE_ERROR;
    }
}


/*****************************************************************************
 �� �� ��  : mtcmos_power_on
 ��������  : ��mtcmos
 �������  : int id //mtcmos id
 �������  : ��
 �� �� ֵ  : int    //0:�ɹ�  ����ֵ:ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��12��25��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ɺ���

*****************************************************************************/
int mtcmos_power_on( int id )
{
    int id_index;
    int actual_id;
    int i;

    /*printk("%s id:%d, mtcmos begin id:%d\n", __func__, id, PLATFORM_MTCMOS_BEGIN_ID);*/
    id_index = id - PLATFORM_MTCMOS_BEGIN_ID;

    if((id_index < (int)VCC_ENUM_BEGIN) || (id_index >= (int)VCC_ENUM_END)) {
    	printk("%s invalid index_id:%d\n", __func__, id_index);
        return MTCMOS_MODULE_ERROR;
    }

    actual_id = platform_mtcmos_id_2_actual_id[id_index];

    lock(&reg_lock_mutex);
    /*open mtcmos*/
    if ((actual_id < (int)VCC_BLOCK1_END) && (actual_id >= (int)VCC_BLOCK1_START)) {
        writel((0x1 << ip_mtcmos_switch_pos[actual_id]), (ao_sctrl_base_addr + SC_PW_EN0));
    } else if ((actual_id < (int)VCC_BLOCK2_END) && (actual_id >= (int)VCC_BLOCK2_START)) {
        writel((0x1 << ip_mtcmos_switch_pos[actual_id]), (ao_sctrl_base_addr + SC_PW_EN1));
    } else {
    	printk("%s invalid actual_id:%d\n", __func__, actual_id);
        unlock(&reg_lock_mutex);
        return MTCMOS_MODULE_ERROR;
    }

    /*��ʱ10us,��ѯmtcmos״̬*/
    (void)udelay(MTCMOS_PWON_TIME);

    for(i = 0; i < MTCMOS_PWON_STATE_CHECK_COUNTER; i++)
    {
        if (MTCMOS_MODULE_OK == mtcmos_power_is_on(actual_id)) {
            break;
        }
    }

    if (MTCMOS_PWON_STATE_CHECK_COUNTER == i) {
    	printk("%s mtcmos status no ready actual_id:%d\n", __func__, actual_id);
        unlock(&reg_lock_mutex);
        return MTCMOS_MODULE_ERROR;
    }

    unlock(&reg_lock_mutex);

    return MTCMOS_MODULE_OK;
}

/*****************************************************************************
 �� �� ��  : mtcmos_power_off
 ��������  : �ر�mtcmos
 �������  : int id //mtcmos id
 �������  : ��
 �� �� ֵ  : int    //0:�ɹ�  ����ֵ:ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��12��25��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ɺ���

*****************************************************************************/
int mtcmos_power_off( int id )
{
    int id_index;
    int actual_id;

    printk("%s id:%d, mtcmos begin id:%d\n", __func__, id, PLATFORM_MTCMOS_BEGIN_ID);
    id_index = id - PLATFORM_MTCMOS_BEGIN_ID;

    if((id_index < (int)VCC_ENUM_BEGIN) || (id_index >= (int)VCC_ENUM_END)) {
    	printk("%s invalid index_id:%d\n", __func__, id_index);
        return MTCMOS_MODULE_ERROR;
    }

    actual_id = platform_mtcmos_id_2_actual_id[id_index];

    lock(&reg_lock_mutex);
    /*close mtcmos*/
    if ((actual_id < (int)VCC_BLOCK1_END) && (actual_id >= (int)VCC_BLOCK1_START)) {
        writel((0x1 << ip_mtcmos_switch_pos[actual_id]), (ao_sctrl_base_addr + SC_PW_DIS0));
    } else if ((actual_id < (int)VCC_BLOCK2_END) && (actual_id >= (int)VCC_BLOCK2_START)) {
        writel((0x1 << ip_mtcmos_switch_pos[actual_id]), (ao_sctrl_base_addr + SC_PW_DIS1));
    } else {
    	printk("%s invalid actual_id:%d\n", __func__, actual_id);
        unlock(&reg_lock_mutex);
        return MTCMOS_MODULE_ERROR;
    }

    /*��ʱ10us,��ѯmtcmos״̬*/
    udelay(MTCMOS_PWON_TIME);
    unlock(&reg_lock_mutex);

    return MTCMOS_MODULE_OK;
}

/*****************************************************************************
 �� �� ��  : mtcmos_power_status
 ��������  : mtcoms��ǰ״̬���򿪻�رգ�
 �������  : int id //mtcmos id
 �������  : ��
 �� �� ֵ  : int    //1:��  2:�ر�  ����ֵ:ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��12��25��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ɺ���

*****************************************************************************/
int mtcmos_power_status( int id )
{
    int id_index;
    int actual_id;
    int ret;

    /*printk("%s id:%d, mtcmos begin id:%d\n", __func__, id, PLATFORM_MTCMOS_BEGIN_ID);*/
    id_index = id - PLATFORM_MTCMOS_BEGIN_ID;

    if((id_index < (int)VCC_ENUM_BEGIN) || (id_index >= (int)VCC_ENUM_END)) {
    	printk("%s invalid index_id:%d\n", __func__, id_index);
        return MTCMOS_MODULE_ERROR;
    }

    actual_id = platform_mtcmos_id_2_actual_id[id_index];

    if((actual_id < (int)VCC_ENUM_BEGIN) || (actual_id >= (int)VCC_ENUM_END)) {
    	printk("%s invalid actual_id:%d\n", __func__, actual_id);
        return MTCMOS_MODULE_ERROR;
    }

    lock(&reg_lock_mutex);
    if (MTCMOS_MODULE_OK == mtcmos_power_is_on(actual_id)) {
        ret = MTCMOS_MODULE_STATUS_ON;
    } else {
    	printk("%s mtcmos status no ready actual_id:%d\n", __func__, actual_id);
        ret = MTCMOS_MODULE_STATUS_OFF;
    }
    unlock(&reg_lock_mutex);

    return ret;
}

int __init mtcmos_power_init(void)
{
    unsigned int addr;
    unsigned int val;
    int id;

    ao_sctrl_base_addr = (unsigned int)IO_ADDRESS(SOC_SC_ON_BASE_ADDR);

    /*set mtcmos power up time*/
    for(id = (int)VCC_ENUM_BEGIN; id < (int)VCC_ENUM_END; id++)
    {
        lock(&reg_lock_mutex);
        addr = ao_sctrl_base_addr + pwup_time_reg_info[id].pwup_time_reg;
        val = readl(addr);
        val &= (~pwup_time_reg_info[id].pwup_time_mask);
        val |= (MTCMOS_PWON_TIME_COUNTER << pwup_time_reg_info[id].pwup_time_start_bit);
        writel(val, addr);
        unlock(&reg_lock_mutex);
    }
    return 0;
}

void __exit mtcmos_power_exit(void)
{
    return;
}

core_initcall(mtcmos_power_init);
module_exit(mtcmos_power_exit);
MODULE_LICENSE("GPL");









#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

