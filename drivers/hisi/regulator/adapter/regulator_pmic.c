/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : regulator_pmic.c
  �� �� ��   : ����
  ��    ��   : ����ΰ 176398
  ��������   : 2012��12��19��
  ����޸�   :
  ��������   : pmu��Դ��������㣬���ڶԽ�regulatorƽ̨����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��12��19��
    ��    ��   : ����ΰ 176398
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

#include "pmic_common.h"
#include "regulator_pmic.h"
#include "regulator_mntn.h"

extern struct regulator_init_data* platform_regulators;
PMIC_COMSUMER_INFO pmic_comsumer_info[PMU_CHANNEL_ID_BUTT];

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
int g_pmic_debug_switch  = DEBUG_OPEN;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
char* regulator_pmic_getname_by_id(int id)
{
    return platform_regulators[id].constraints.name;
}

void regulator_pmic_comsumerinfo_save(int regulator_id ,int onoff,int vol_mini,int vol_max)
{
    if(regulator_id >= PMU_CHANNEL_ID_BUTT)
    {
          PMIC_TRACE(g_pmic_debug_switch,"%s regulator_id >= PMU_CHANNEL_ID_BUTT\n", __func__);
          return ;
    }

    if(PMIC_INVAILD != onoff)
    {
        pmic_comsumer_info[regulator_id].comsumer_channel_status = onoff;
    }

    if((PMIC_INVAILD != vol_mini)&&(PMIC_INVAILD != vol_max))
    {
        pmic_comsumer_info[regulator_id].comsumer_voltage_mini= vol_mini;
        pmic_comsumer_info[regulator_id].comsumer_voltage_max= vol_max;
    }
}


int regulator_pmic_is_enabled(struct regulator_dev *dev)
{
    int regulator_id = 0;
    int ret = 0;
    regulator_id =  rdev_get_id(dev); /*get bat channel id*/
    ret = pmic_power_status(regulator_id);
    if (PMIC_MODULE_STATUS_ON == ret)
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}
int regulator_pmic_enable(struct regulator_dev *dev)
{
    int regulator_id = 0;
    int ret = 0;

    regulator_id = rdev_get_id(dev);

    PMIC_TRACE(g_pmic_debug_switch,"regulator_id %d  %s ON \n",regulator_id,regulator_pmic_getname_by_id(regulator_id));

    regulator_pmic_comsumerinfo_save(regulator_id,CHANNEL_SWI_ENABLE,PMIC_INVAILD,PMIC_INVAILD);

    ret = pmic_power_switch(regulator_id,CHANNEL_SWI_ENABLE);
    if (PMIC_MODULE_OK == ret) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}


int regulator_pmic_disable(struct regulator_dev *dev)
{
    int regulator_id = 0;
    int ret = 0;

    regulator_id =  rdev_get_id(dev);

    PMIC_TRACE(g_pmic_debug_switch,"regulator_id %d  %s OFF \n",regulator_id,regulator_pmic_getname_by_id(regulator_id));

    regulator_pmic_comsumerinfo_save(regulator_id,CHANNEL_SWI_DISABLE,PMIC_INVAILD,PMIC_INVAILD);

    ret = pmic_power_switch(regulator_id,CHANNEL_SWI_DISABLE);
    if (PMIC_MODULE_OK == ret) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}


int regulator_pmic_list_voltage(struct regulator_dev *dev,
              unsigned selector)
{
    int regulator_id = 0;
    int ret = 0;

    regulator_id =  rdev_get_id(dev);

    //return pmic_list_vol_tab(regulator_id,selector);

    ret = pmic_list_vol_tab(regulator_id,selector);

    if(-6 == ret)
    {
        ret = platform_regulators[regulator_id].constraints.min_uV;
    }

    return ret;

}


int regulator_pmic_get_voltage(struct regulator_dev *dev)
{
    int regulator_id = 0;
    int ret = 0;
    regulator_id =  rdev_get_id(dev);

    /*return pmic_get_voltage(regulator_id);*/
    ret = pmic_get_voltage(regulator_id);

    if(-6 == ret)
    {
        ret = platform_regulators[regulator_id].constraints.min_uV;
    }

    return ret;
}
int regulator_pmic_set_voltage(struct regulator_dev *dev,
            int min_uV, int max_uV, unsigned *selector)
{
    int  regulator_id = 0;


    regulator_id =  rdev_get_id(dev);

    PMIC_TRACE(g_pmic_debug_switch,"PMIC %d  %s set vol  max %d min %d\n",
    regulator_id,regulator_pmic_getname_by_id(regulator_id),max_uV,min_uV);

    regulator_pmic_comsumerinfo_save(regulator_id,PMIC_INVAILD,min_uV,max_uV);

    return pmic_set_voltage(regulator_id,min_uV,max_uV,selector);
}


unsigned int regulator_pmic_get_mode(struct regulator_dev *dev)
{
    return REGULATOR_MODE_NORMAL;
}


int regulator_pmic_set_mode(struct regulator_dev *dev, unsigned int mode)
{
    return 0;
}
unsigned int regulator_pmic_get_optimum_mode(struct regulator_dev *dev, int input_uV,
        int output_uV, int load_uA)
{
    return REGULATOR_MODE_NORMAL;
}

struct regulator_ops regulator_pmic_ops = {
    .is_enabled = regulator_pmic_is_enabled,
    .enable = regulator_pmic_enable,
    .disable = regulator_pmic_disable,
    .list_voltage = regulator_pmic_list_voltage,
    .get_voltage = regulator_pmic_get_voltage,
    .set_voltage = regulator_pmic_set_voltage,
    .get_mode = regulator_pmic_get_mode,
    .set_mode = regulator_pmic_set_mode,
    .get_optimum_mode = regulator_pmic_get_optimum_mode,
};

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

